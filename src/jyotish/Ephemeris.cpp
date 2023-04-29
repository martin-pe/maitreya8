/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Ephemeris.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2017 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "Ephemeris.h"

#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "DasaTool.h"
#include "DataSet.h"
#include "Lang.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Sheet.h"
#include "Table.h"

#include <wx/log.h>
#include <wx/stopwatch.h>

extern Config *config;

/*************************************************//**
*
*  \brief sort ingress events according to their date
*
******************************************************/
class IngressEventSorter
{
public:
	bool operator()( const IngressEvent &e1, const IngressEvent &e2 )
	{
		return( e1.jd < e2.jd );
	}
};

/*************************************************//**
*
*  \brief sort Lunar events according to their date
*
******************************************************/
class LunarEventSort
{
public:
	bool operator()( const LunarEvent &e1, const LunarEvent &e2 )
	{
		return( e1.jd < e2.jd );
	}
};

/*****************************************************
**
**   EphemPlanetData   ---   Constructor
**
******************************************************/
EphemPlanetData::EphemPlanetData( const ObjectId &i )
{
	pindex = i;
	for( int i = 0; i < MAX_DAY; i++ )
	{
		retro[i] = false;
		len[i] = speed[i] = 0.0;
		rasi[i] = R_NONE;
		nakshatra[i] = N27_NONE;
	}
}

/*****************************************************
**
**   EphemExpert   ---   Constructor
**
******************************************************/
EphemExpert::EphemExpert( const ChartProperties *props )
		: chartprops( props )
{
	d = new DataSet();
	d->setLocation( *config->defaultLocation );
	//show_header = config->view->showTextViewHeaders;
	show_header = true;
	init();
}

/*****************************************************
**
**   EphemExpert   ---   Destructor
**
******************************************************/
EphemExpert::~EphemExpert()
{
	delete d;
}

/*****************************************************
**
**   EphemExpert   ---   init
**
******************************************************/
void EphemExpert::init()
{
	int i;
	clen = cingress = ckp = cdetails = clunar = false;
	mydasa = D_NONE;
	dstchange = false;

	// reset variables
	for ( i = 0; i < MAX_DAY; i++ )
	{
		weekday[i] = tithi[i] = 0;
		jd[i] = st[i] =  sunrise[i] = sunset[i] = 0;
	}
	planetdata.clear();
}

/*****************************************************
**
**   EphemExpert   ---   prepareMonth
**
******************************************************/
void EphemExpert::prepareMonth( const int &m, const int &y, const bool &isLocal )
{
	double lastdst = 0;
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	TzUtil tzu;
	TzInfo tzinfo;

	year = y;
	month = m;
	this->isLocaltime = isLocal;

	assert( month >= 0 && month <= 12 );
	assert( year >= -2000 && year <= 4000 );

	init();

	nb_days = getMonthLength( year, month );
	assert( nb_days > 27 && nb_days < MAX_DAY );

	formatter->calculateDateIntegers( MDate::getCurrentJD(), current_day, current_month, current_year );

	// set jds and week days
	for ( int i = 0; i <= nb_days; i++ ) // calc JDs for this month and year
	{
		d->setDate( i+1, month, year, 0 );
		jd[i] = d->getJD();
		weekday[i] = ((int)(jd[i] + 2 )) % 7;

		if ( isLocaltime )
		{
			tzinfo = tzu.calculateTzInfoForJD( jd[i] );
			double tzdelta = tzinfo.tzhours + tzinfo.dsthours;
			jd[i] -= tzdelta / 24.0;

			// test for dst change during month
			if ( i > 0 )
			{
				if ( lastdst != tzinfo.dsthours )
				{
					//printf( "DST change day %d last is %f current %f\n", i+1, lastdst, tzinfo.dsthours );
					dstchange = true;
				}
			}
			lastdst = tzinfo.dsthours;
			//printf( "Ephem: tzdelta %f day %d\n", tzdelta, i+1 );
		}
	}
}

/*****************************************************
**
**   EphemExpert   ---   calcLongitude
**
******************************************************/
int EphemExpert::calcLongitude()
{
	int i, ret = 0;
	ObjectId p;
	uint i1;
	double dummy;
	Calculator *calculator = CalculatorFactory().getCalculator();

	std::vector<ObjectId> obs = chartprops->getPlanetList();

	planetdata.clear();
	for ( i1 = 0; i1 < obs.size(); i1++ )
	{
		p = obs[i1];
		if ( IS_ANGLE_OBJECT( p ) || p == OARIES || p > MAX_EPHEM_OBJECTS ) continue;
		EphemPlanetData pdata( p );
		for ( i = 0; i <= nb_days; i++ ) // calc length and retrogression
		{
			//d->setDate( i+1, month, year, -tz );
			d->setDate( jd[i] );
			calculator->calcPositionSpeed( d, p, pdata.len[i], dummy, pdata.speed[i], true, chartprops->isVedic() );
			pdata.retro[i] = ( pdata.speed[i] < 0 );
			pdata.rasi[i] = getRasi( pdata.len[i] );
			pdata.nakshatra[i] = getNakshatra27( pdata.len[i] );
		}

		if ( pdata.len[0] == 0 ) ret++;

		planetdata.push_back( pdata );
	}
	clen = true;
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   calcKP
**
******************************************************/
void EphemExpert::calcKP( const DasaId &dasaId, DasaExpert *expert )
{
	KPEvent *e;
	std::vector<KPEvent>::iterator iter;
	const wxLongLong starttime = wxGetLocalTimeMillis();

	Calculator *calculator = CalculatorFactory().getCalculator();
	//if ( ! expert->hasKpFeatures() || dasaId == D_KALACHAKRA ) return;
	if ( ! expert->hasKpFeatures()) return;

	if ( !clen  ) calcLongitude();
	kpevents = expert->getKPEventList( planetdata[OMOON].len[0], planetdata[OMOON].len[nb_days], jd[0] ); // calculates all lord/sublord events

	for ( iter = kpevents.begin(); iter != kpevents.end(); iter++ )
	{
		e = (KPEvent*)(&(*iter));
		d->setDate( e->jd );
		e->jd = calculator->calcPlanetaryEvent( d, e->len, 1, chartprops->isVedic() ); // get the dates for the events
	}
	ckp = true;
	mydasa = dasaId;
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "EphemExpert::calcKP in %ld millisec" ), totaltime.ToLong()));
}

/*****************************************************
**
**   EphemExpert   ---   writeKp
**
******************************************************/
void EphemExpert::writeKp( Sheet *sheet, const DasaId &dasaId )
{
	Lang lang;
	int lord;
	std::vector<KPEvent>::iterator iter;
	TzUtil tzu;
	wxString s;
	DasaTool *tool = DasaTool::get();
	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasaId );
	SheetFormatter fmt;

	writeHeaderInfo( sheet );
	Table *table = (Table*)NULL;

	const wxLongLong starttime = wxGetLocalTimeMillis();

	if ( ! expert->hasKpFeatures() )
	{
		s.Printf( _( "K.P. events not supported for %s Dasa." ), tool->getDasaName( dasaId ).c_str());
		sheet->addLine( s );
		return;
	}

	if ( ! chartprops->isVedic() )
	{
		sheet->addLine( _( "Not supported in western mode." ));
		return;
	}
	if ( ! ckp || mydasa != dasaId ) calcKP( dasaId, expert );

	lord = ONONE;
	int index = -1;
	for ( iter = kpevents.begin(); iter != kpevents.end(); iter++ )
	{
		// test if index has changed
		if ( iter->dasaindex != index )
		{
			index = iter->dasaindex;
			if ( table )
			{
				table->setHeader( expert->getDasaLordNameF( lord, TF_LONG ));
				sheet->addItem( table );
				table = (Table*)NULL;
			}
			table = new Table ( 5, 1 );
			table->setHeader( 0, _( "Day" ));
			table->setHeader( 1, _( "Time" ));
			table->setHeader( 2, _( "Lord" ));
			table->setHeader( 3, _( "Sublord" ));
			table->setHeader( 4, _( "Lunar Position" ));
		}
		assert( table );

		Row row( table, 5, table->getNbRows());
		TzFormattedDate fd = tzu.getDateFormatted( iter->jd, isLocaltime );

		s.Printf( wxT( "%02d " ), fd.dayOfMonth );
		s << fd.weekDay;
		row.value[0].text = s;
		row.value[1].text = fd.timeFormatted;
		row.value[2].text = expert->getDasaLordNameF( iter->lord, TF_LONG );
		row.value[3].text = expert->getDasaLordNameF( iter->sublord, TF_LONG );
		row.value[4].text = fmt.getPosFormatted( iter->len, MD_DIRECT );
		table->addRow( row );

		lord = iter->lord;
	}
	if ( table )
	{
		table->setHeader( expert->getDasaLordName( lord, TF_LONG ));
		sheet->addItem( table );
	}

	delete expert;
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "EphemExpert::writeKP in %ld millisec" ), totaltime.ToLong()));
}

/*****************************************************
**
**   EphemExpert   ---   calcIngress
**
******************************************************/
int EphemExpert::calcIngress()
{
	ObjectId p;
	int ret = 0;
	if ( !clen  ) ret = calcLongitude();
	ingressEvents.clear();

	// Achtung: das geht nicht immer für den Mond, weil der mehr als ein Event am Tag haben kann
	for ( int i = 0; i < nb_days; i++ )
	{
		d->setDate( i+1, month, year, 12 );
		for ( uint j = 0; j < planetdata.size(); j++ )
		{
			p = planetdata[j].pindex;
			if ( p == OARIES || p == OASCENDANT || p == OMERIDIAN ) continue;

			if( planetdata[j].rasi[i] != planetdata[j].rasi[i+1] )
			{
				testIngressEvent( planetdata[j].rasi[i], planetdata[j].rasi[i+1], p, 0, planetdata[j].retro[i] );
			}

			if ( chartprops->isVedic() )
			{
				// Nakshatra changes
				if ( planetdata[j].nakshatra[i] !=  planetdata[j].nakshatra[i+1] )
				{
					const int base_nak = planetdata[j].nakshatra[i];
					const int diff_nak = red27( planetdata[j].nakshatra[i+1] -  base_nak );
					switch( diff_nak )
					{
						case 1:
							testIngressEvent( base_nak, red27( base_nak + 1 ), p, 1, planetdata[j].retro[i] );
						break;
						case 2:
							testIngressEvent( base_nak, red27( base_nak + 1 ), p, 1, planetdata[j].retro[i] );
							testIngressEvent( red27( base_nak + 1 ), red27( base_nak + 2 ), p, 1, planetdata[j].retro[i+1] );
						break;
						default:
						break;
					}
				}
			}
		}
	}
	ingressEvents.sort( IngressEventSorter() );
	cingress = true;
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   testIngressEvent
**
******************************************************/
void EphemExpert::testIngressEvent( const int &t1, const int &t2, const ObjectId &planet, const int &type, const bool &r )
{
	double tjd, targetlen;
	if ( t1 != t2 )
	{
		Calculator *calculator = CalculatorFactory().getCalculator();
		targetlen = ( r ? t1 : t2 ) * ( type ? 13.3333333333 : 30 );
		tjd = calculator->calcPlanetaryEvent( d, targetlen, planet, chartprops->isVedic() );
		ingressEvents.push_back( IngressEvent( tjd, planet, t2, type ));
	}
}

/*****************************************************
**
**   EphemExpert   ---   writeHeaderInfo
**
******************************************************/
void EphemExpert::writeHeaderInfo( Sheet *sheet )
{
	if ( ! show_header ) return;

	Lang lang;
	wxString s;
	s.Printf( wxT ( "%s %d" ), lang.getMonthName( month-1 ).c_str(), year );
	sheet->addHeader( s );
}

/*****************************************************
**
**   EphemExpert   ---   writeIngress
**
******************************************************/
int EphemExpert::writeIngress( Sheet *sheet )
{
	int ret = 0;
	Lang lang;
	std::list<IngressEvent>::iterator iter;
	wxString s, thetime;
	TzUtil tzu;
	SheetFormatter fmt;

	if ( ! cingress ) ret = calcIngress();
	writeHeaderInfo( sheet );

	Table *table = new Table( 4, ingressEvents.size()+1 );
	table->setHeader( 0, _( "Day" ));
	table->setHeader( 1, _( "Time" ));
	table->setHeader( 2, _( "Planet" ));
	table->setHeader( 3, chartprops->isVedic() ? _( "Sign/Nakshatra" ) : _( "Sign" ));

	int line = 1;
	for ( iter = ingressEvents.begin(); iter != ingressEvents.end(); iter++ )
	{
		TzFormattedDate fd = tzu.getDateFormatted( iter->jd, isLocaltime );
		thetime = fd.timeFormatted;

		s.Printf( wxT( "%02d %s" ), fd.dayOfMonth, fd.weekDay.c_str());
		if ( testDayIndexForCurrent( fd.dayOfMonth )) table->setHeaderEntry( 0, line, s );
		else	table->setEntry( 0, line, s );

		table->setEntry( 1, line, thetime );
		table->setEntry( 2, line, fmt.getObjectName( iter->planet, TF_LONG, chartprops->isVedic() ));
		if ( iter->type == 0 )
			table->setEntry( 3, line, fmt.getSignName( (Rasi)iter->which, TF_LONG ));
		else
			table->setEntry( 3, line, lang.getNakshatra27Name( (Nakshatra)iter->which, TF_LONG ));

		line++;
	}
	sheet->addItem( table );
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   addLunarEvent
**
******************************************************/
void EphemExpert::addLunarEvent( const int &stdate, const double& target, const double& sdiff, const double &ediff )
{
	double slen, mlen, cjd;
	Calculator *calculator = CalculatorFactory().getCalculator();

	//printf( "EphemExpert::addLunarEvent stdate %d target %f sdiff %f ediff %f\n", stdate, target, sdiff, ediff );

	d->setDate( jd[stdate] + ( target - sdiff ) / ( ediff - sdiff ));

	cjd = calculator->calcSunMoonEvent( d, target, slen, mlen );
	double aya = calculator->calcAyanamsa( cjd, chartprops->isVedic() ? config->vedicCalculation->ayanamsa : config->vedicCalculation->ayanamsa );
	lunarEvents.push_back(
		LunarEvent( target, cjd,
			chartprops->isVedic() ? red_deg( slen - aya ) : slen, chartprops->isVedic() ? red_deg( mlen - aya ) : mlen ));
}


/*****************************************************
**
**   EphemExpert   ---   addLunarSpecialEvent
**
******************************************************/
void EphemExpert::addLunarSpecialEvent( const int &stdate, const double& target, const double& sdiff, const double &ediff )
{
	if ( sdiff < target && ediff >= target ) addLunarEvent( stdate, target, sdiff, ediff );
}

/*****************************************************
**
**   EphemExpert   ---   calcLunar
**
******************************************************/
void EphemExpert::calcLunar()
{
	int i, t, t2;
	double diff, diff2;
	lunarEvents.clear();

	if ( !clen  ) calcLongitude();

	diff = red_deg( planetdata[OMOON].len[0] - planetdata[OSUN].len[0] );
	t = (int)(diff/12);
	for ( i = 1; i <= nb_days; i++ )
	{
		diff2 = red_deg( planetdata[OMOON].len[i] - planetdata[OSUN].len[i] );
		t2 = (int)(diff2/12);
		
		if ( t2 != t )
		{
			addLunarEvent( i-1, t2 * 12, diff, diff2 );

			// Happens once a month, i.e. double step
			// if ( t2 - t > 1 )
			// bugfix: t2 may loop immediately to 1 with t remaining high (!= 0)
			if ( t2 - t > 1 || ( t2 == 1 && t != 0 ) )
			{
				addLunarEvent( i-1, ( t + 1 ) * 12, diff, diff2 );
			}
		}
		// Squares
		addLunarSpecialEvent( i-1, 90, diff, diff2 );
		addLunarSpecialEvent( i-1, 270, diff, diff2 );

		// Semi Squares
		addLunarSpecialEvent( i-1, 45, diff, diff2 );
		addLunarSpecialEvent( i-1, 135, diff, diff2 );
		addLunarSpecialEvent( i-1, 225, diff, diff2 );
		addLunarSpecialEvent( i-1, 315, diff, diff2 );

		diff = diff2;
		t = t2;
	}

	lunarEvents.sort( LunarEventSort() );
	clunar = true;
}

/*****************************************************
**
**   EphemExpert   ---   writeLunar
**
******************************************************/
void EphemExpert::writeLunar( Sheet *sheet )
{
	if ( ! clunar ) calcLunar();
	Lang lang;
	wxString s, thetime;
	std::list<LunarEvent>::iterator iter;
	LunarEvent *e;
	TzUtil tzu;
	SheetFormatter fmt;

	writeHeaderInfo( sheet );
	Table *table = new Table( 7, lunarEvents.size()+1 );
	table->setHeader( 0, _( "Day" ));
	table->setHeader( 1, _( "Time" ));
	table->setHeader( 2, _( "Sun" ));
	table->setHeader( 3, _( "Moon" ));
	table->setHeader( 4, _( "Angle" ));
	table->setHeader( 5, _( "Tithi" ));
	table->setHeader( 6, _( "Western" ));

	int line = 1;
	for ( iter = lunarEvents.begin(); iter != lunarEvents.end(); iter++ )
	{
		e = (LunarEvent*)(&(*iter));
		TzFormattedDate fd = tzu.getDateFormatted( e->jd, isLocaltime );
		thetime = fd.timeFormatted;
		//if ( dstchange ) thetime << wxT( " " ) << fd.timezoneFormatted;

		s.Printf( wxT( "%02d %s" ), fd.dayOfMonth, fd.weekDay.c_str());
		if ( testDayIndexForCurrent( fd.dayOfMonth )) table->setHeaderEntry( 0, line, s );
		else	table->setEntry( 0, line, s );

		table->setEntry( 1, line, thetime );
		table->setEntry( 2, line, fmt.getPosFormatted( e->slen, MD_DIRECT, DEG_PRECISION_SECOND, TF_MEDIUM ));
		table->setEntry( 3, line, fmt.getPosFormatted( e->mlen, MD_DIRECT, DEG_PRECISION_SECOND, TF_MEDIUM ));

		int angle = (int)( red_deg( e->mlen - e->slen ) + .00001 );
		if ( angle >= 360 ) angle -= 360;
		s.Printf( wxT( "%d" ), angle );
		table->setEntry( 4, line, s );

		if ( ! ( angle % 12 ))
		{
			table->setEntry( 5, line, lang.getTithiName( angle / 12 ) );
		}
		if ( angle == 0 ) table->setEntry( 6, line, _( "New Moon" ));
		else if ( angle == 180 ) table->setEntry( 6, line, _( "Full Moon" ));
		else if ( angle == 60 || angle == 300 ) table->setEntry( 6, line, _( "Sextile" ));
		else if ( angle == 90 ) table->setEntry( 6, line, _( "Half Moon (Waxing)" ));
		else if ( angle == 120 || angle == 240 ) table->setEntry( 6, line, _( "Trine" ));
		else if ( angle == 270 ) table->setEntry( 6, line, _( "Half Moon (Waning)" ));

		// Semi squares
		else if (( angle == 45 || angle == 135 || angle == 225 || angle == 315 ))
			table->setEntry( 6, line, _( "Semi Square" ));
		line++;
	}
	sheet->addItem( table );
}

/*****************************************************
**
**   EphemExpert   ---   calcDetails
**
******************************************************/
void EphemExpert::calcDetails()
{
	int i;
	Calculator *calculator = CalculatorFactory().getCalculator();
	double mlen, slen, dummy, diff;

	for ( i = 0; i < nb_days; i++ )
	{
		d->setDate( i+1, month, year, 0 ); // Must be 0, correct offset will follow in formatting

		// Only the time will be affected: location ist default! (not 0 - 0)
		st[i] = calculator->calcSiderealTime( d->getJD(), d->getLocation()->getLongitude() );
		calculator->calcSunRiseSunSet( d, &sunrise[i], &sunset[i] );

		d->setDate( sunrise[i] );
		calculator->calcPosition( d, OMOON, mlen, dummy, false, false );
		calculator->calcPosition( d, OSUN, slen, dummy, false, false );
		diff = red_deg( mlen - slen );
		tithi[i] = (int)(diff/12);
	}

	cdetails = true;
}

/*****************************************************
**
**   EphemExpert   ---   writeDetails
**
******************************************************/
void EphemExpert::writeDetails( Sheet *sheet )
{
	int i;
	wxString s, tz_str, thetime;
	Lang lang;
	Formatter *formatter = Formatter::get();
	TzUtil tzu;

	if ( ! cdetails ) calcDetails();
	writeHeaderInfo( sheet );

	int line = 1;
	int nb_leaps = 0;
	for ( i = 0; i < nb_days; i++ )
	{
		if ( i > 0 && weekday[i] == 0 ) nb_leaps++;
	}
	Table *table = new Table( 5, nb_days+1+nb_leaps );
	//table->setHeader(  wxString::Format( wxT ( "%s %d" ), lang.getMonthName( month-1 ).c_str(), year ));

	table->setHeader( 0, _( "Day" ));
	table->setHeader( 1, _( "Sidereal Time" ));
	table->setHeader( 2, _( "Sunrise" ));
	table->setHeader( 3, _( "Sunset" ));
	table->setHeader( 4, _( "Tithi (Sunrise)" ));

	line = 1;
	for ( i = 0; i < nb_days; i++ )
	{
		// blank line on weekend
		if ( i > 0 && weekday[i] == 0 )
		{
			for ( int j = 0; j < 5; j++ ) table->setHeaderEntry( j, line, wxEmptyString );
			line++;
		}

		s.Printf( wxT( "%02d %s" ), i+1, lang.getWeekdayName( weekday[i] ).Left(2).c_str() );
		if ( testDayIndexForCurrent( i+1 )) table->setHeaderEntry( 0, line, s );
		else	table->setEntry( 0, line, s );

		table->setEntry( 1, line, formatter->getTimeFormatted( st[i] ) );

		TzFormattedDate fd = tzu.getDateFormatted( sunrise[i], isLocaltime );
		thetime = fd.timeFormatted;
		//if ( dstchange ) thetime << wxT( " " ) << fd.timezoneFormatted;
		table->setEntry( 2, line, thetime );

		fd = tzu.getDateFormatted( sunset[i], isLocaltime );
		table->setEntry( 3, line, fd.timeFormatted );

		table->setEntry( 4, line, lang.getTithiName( tithi[i] ));
		line++;
	}
	sheet->addItem( table );
}

/*****************************************************
**
**   EphemExpert   ---   getLongitude
**
******************************************************/
double EphemExpert::getLongitude( const int &i, const int &day )
{
	return planetdata[i].len[day];
}

/*****************************************************
**
**   EphemExpert   ---   getSpeed
**
******************************************************/
double EphemExpert::getSpeed( const int &i, const int &day )
{
	assert( i >= 0 && i < (int)planetdata.size());
	return planetdata[i].speed[day];
}

/*****************************************************
**
**   EphemExpert   ---   getRetro
**
******************************************************/
bool EphemExpert::getRetro( const int &i, const int &day )
{
	assert( i >= 0 && i < (int)planetdata.size());
	return planetdata[i].retro[day];
}

/*****************************************************
**
**   EphemExpert   ---   calcMonth
**
******************************************************/
int EphemExpert::calcMonth()
{
	if ( !clen  ) return calcLongitude();
	else return 0;
}

/*****************************************************
**
**   EphemExpert   ---   testDayIndexForCurrent
**
******************************************************/
bool EphemExpert::testDayIndexForCurrent( const int &i )
{
	return ( month == current_month && year == current_year && i == current_day );
}

/*****************************************************
**
**   EphemExpert   ---   isCurrentMonth
**
******************************************************/
bool EphemExpert::isCurrentMonth()
{
	return ( month == current_month && year == current_year );
}

/*****************************************************
**
**   EphemExpert   ---   writeLongitudes
**
******************************************************/
int EphemExpert::writeLongitudes( Sheet *sheet )
{
	int ret = 0;
	wxString s;
	int i, j, numcols;
	uint i1;
	Lang lang;
	MString f;
	const TEXT_FORMAT format = TF_SHORT;
	MOVING_DIRECTION dir;
	SheetFormatter fmt;

	if ( !clen  ) ret = calcLongitude();
	writeHeaderInfo( sheet );
	numcols = planetdata.size() + 1;

	int nb_leaps = 0;
	for ( i = 0; i < nb_days; i++ )
	{
		if ( i > 0 && weekday[i] == 0 ) nb_leaps++;
	}
	Table *table = new Table( numcols, nb_days+nb_leaps+2 );
	table->setHeader( 0, _( "Day" ));

	int col = 1;
	int line = 1;
	for ( i = 0; i < nb_days; i++ )
	{
		if ( i > 0 && weekday[i] == 0 )
		{
			for ( j = 0; j < numcols; j++ ) table->setHeaderEntry( j, line, wxEmptyString );
			line++;
		}

		s.Printf( wxT( "%02d %s" ), i+1, lang.getWeekdayName( weekday[i] ).Left(2).c_str() );
		if ( testDayIndexForCurrent( i+1 )) table->setHeaderEntry( 0, line, s );
		else	table->setEntry( 0, line, s );

		col = 1;
		for ( i1 = 0; i1 < planetdata.size(); i1++ )
		{
			dir = MD_NONE;
			if ( planetdata[i1].retro[i] || ( i == nb_days - 1 && planetdata[i1].retro[i] ))
			{
				dir = MD_RETROGRADE;
			}
			else if ( i > 1 && planetdata[i1].retro[i] != planetdata[i1].retro[i-1] )
			{
				dir = ( planetdata[i1].retro[i] ? MD_RETROGRADE : MD_DIRECT );
			}
			f = fmt.getPosFormatted( planetdata[i1].len[i], dir, DEG_PRECISION_MINUTE, format );
			if ( chartprops->isVedic() )
			{
				f.add( MToken( lang.getNakshatra27Name( planetdata[i1].nakshatra[i], TF_SHORT ) ));
			}
			table->setEntry( col, line, f );

			col++;
		}

		line++;
	}
	// Header
	table->setHeaderEntry( 0, line, _( "Day" ));
	col = 1;
	for ( i1 = 0; i1 < planetdata.size(); i1++ )
	{
		f = fmt.getObjectName( planetdata[i1].pindex, TF_MEDIUM, chartprops->isVedic() );
		table->setHeaderEntry( col, line, f );
		table->setHeaderEntry( col, 0, f );
		col++;
	}
	sheet->addItem( table );
	return ret;
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetRetro
**
******************************************************/
bool EphemExpert::getPlanetRetro( const uint &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex < MAX_EPHEM_OBJECTS );
	assert( plistindex < planetdata.size());
	return planetdata[plistindex].retro[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetLongitude
**
******************************************************/
double EphemExpert::getPlanetLongitude( const uint &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex < MAX_EPHEM_OBJECTS );
	assert( plistindex < planetdata.size());
	return planetdata[plistindex].len[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetSpeed
**
******************************************************/
double EphemExpert::getPlanetSpeed( const uint &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex < MAX_EPHEM_OBJECTS );
	assert( plistindex < planetdata.size());
	return planetdata[plistindex].speed[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetRasi
**
******************************************************/
int EphemExpert::getPlanetRasi( const uint &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex < MAX_EPHEM_OBJECTS );
	assert( plistindex < planetdata.size());
	return planetdata[plistindex].rasi[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetNakshatra
**
******************************************************/
int EphemExpert::getPlanetNakshatra( const uint &plistindex, const int &day ) const
{
	assert( day >= 0 && day < MAX_DAY );
	assert( plistindex < MAX_EPHEM_OBJECTS );
	assert( plistindex < planetdata.size());
	return planetdata[plistindex].nakshatra[day];
}

/*****************************************************
**
**   EphemExpert   ---   getPlanetId
**
******************************************************/
ObjectId EphemExpert::getPlanetId( const uint &plistindex ) const
{
	assert( plistindex < MAX_EPHEM_OBJECTS );
	assert( plistindex < planetdata.size());
	return planetdata[plistindex].pindex;
}

