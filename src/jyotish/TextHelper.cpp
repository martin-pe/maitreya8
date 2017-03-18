/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/TextHelper.cpp
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

#include "TextHelper.h"

#include <wx/log.h>
#include <wx/string.h>
#include <math.h>

#include "ArabicParts.h"
#include "Aspect.h"
#include "Ashtakavarga.h"
#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Dasa.h"
#include "DasaTool.h"
#include "maitreya.h"
#include "GenericTableWriter.h"
#include "Horoscope.h"
#include "Jaimini.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "PlanetList.h"
#include "Table.h"
#include "ShadBala.h"
#include "Sheet.h"
#include "SheetConfig.h"
#include "Varga.h"

extern Config *config;

/*****************************************************
**
**   TextHelper   ---   Constructor
**
******************************************************/
TextHelper::TextHelper( Horoscope *h, ChartProperties *props, Sheet *sheet )
		: horoscope( h ),
		chartprops( props ),
		sheet( sheet )
{
	//show_header = config->view->showTextViewHeaders;
	show_header = true;
}

/*****************************************************
**
**   TextHelper   ---   writeTextAnalysis
**
******************************************************/
int TextHelper::writeTextAnalysis( const int &mode, const Varga varga, const DasaId dasa )
{
	int ret = 0;

	switch ( mode )
	{
	case TM_BASE:
		writeBaseData();
		writeHoroscopeData();
		break;

	case TM_WESTERN_PLANETS:
		ret = writeWesternPlanetReport();
		break;

	case TM_VEDIC_PLANETS:
		ret = writeVedicPlanetReport();
		break;

	case TM_NAKSHATRA:
	{
		NakshatraExpert( horoscope ).write( sheet, chartprops, show_header );
	}
	break;

	case TM_ASHTAKAVARGA:
	{
		AshtakavargaExpert aexpert( horoscope, varga );
		aexpert.update();
		aexpert.write( sheet, show_header );
	}
	break;
	case TM_KP:
		ret = writeKp( dasa );
		break;

	case TM_BHAVA:
		writeBhavas();
		break;

	case TM_ARABICPARTS:
	{
		ArabicPartsExpert( horoscope ).write( sheet, chartprops->isVedic(), show_header );
	}
	break;

	case TM_VEDIC_ASPECTARIUM:
	{
		AspectExpert aspexpert( horoscope, horoscope );
		aspexpert.writeVedicAspectarium( sheet, chartprops, CT_RADIX, show_header );
	}
	break;

	case TM_WESTERN_ASPECTARIUM:
	{
		AspectExpert aspexpert( horoscope, horoscope );
		aspexpert.updateWesternAspects( chartprops, CT_RADIX );
		aspexpert.writeWesternAspectarium( sheet, chartprops );
		aspexpert.writeWesternAspectList( sheet, chartprops, CT_RADIX );
	}
	break;

	case TM_WESTERN_ASPECTARIUM_PLAIN:
	{
		AspectExpert aspexpert( horoscope, horoscope );
		aspexpert.updateWesternAspects( chartprops, CT_RADIX );
		aspexpert.writeWesternAspectarium( sheet, chartprops );
	}
	break;

	case TM_ASTRONOMICAL:
		ret = writeAstronomicalData();
		break;

	case TM_SHADBALA:
	{
		ShadBalaExpert sexpert( horoscope );
		sexpert.updateAllBalas();
		sexpert.write( sheet );
	}
	break;

	case TM_VARGA:
	{
		VargaExpert vexpert( horoscope );
		vexpert.write( sheet, chartprops->getVedicObjectStyle(), show_header );
	}
	break;

	case TM_DASA:
	{
		assert( dasa >= 0 && dasa < (int)MAX_DASAEXPERTS );
		DasaTool *tool = DasaTool::get();
		tool->writeShortReport( sheet, horoscope, (DasaId)dasa, show_header );
	}
	break;

	case TM_DASACOMPACT:
	{
		assert( dasa >= 0 && dasa < (int)MAX_DASAEXPERTS );
		DasaTool *tool = DasaTool::get();
		tool->writeCompactReport( sheet, horoscope, (DasaId)dasa, show_header );
	}
	break;

	case TM_DASALONG:
	{
		assert( dasa >= 0 && dasa < (int)MAX_DASAEXPERTS );
		DasaTool *tool = DasaTool::get();
		tool->writeComprehensiveReport( sheet, horoscope, (DasaId)dasa, show_header );
	}
	break;

	case TM_JAIMINI:
	{
		JaiminiExpert jexpert( horoscope, varga );
		jexpert.update();
		jexpert.write( sheet, show_header );
	}
	break;

	case TM_MAIN_VIEW:
		ret = writePlanets();
		break;

	default:
		printf( "TextHelper::writeTextAnalysis: invalid view mode %d\n", mode );
		writeBaseData();
		//assert( false );
		break;
	}
	if ( ret ) printf( "Warn: %d objects couldn't be calculated\n", ret );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writePlanets
**
******************************************************/
int TextHelper::writePlanets( const OBJECT_INCLUDES excludes )
{
	return chartprops->isVedic() ? writeVedicPlanets( excludes ) : writeWesternPlanets( excludes );
}

/*****************************************************
**
**   TextHelper   ---   writeVedicPlanets
**
******************************************************/
int TextHelper::writeVedicPlanets( const OBJECT_INCLUDES excludes )
{
	GenericTableWriter tw( horoscope );

	OBJECT_INCLUDES obs = chartprops->getVedicObjectStyle() & ~excludes;
	Table *table = tw.createObjectListTableByConfigKeys( obs,
		chartprops->getVedicColumnStyle(), true, TF_LONG );
	sheet->addItem( table );
	return table->errorcount;
}
	
/*****************************************************
**
**   TextHelper   ---   writeWesternPlanets
**
******************************************************/
int TextHelper::writeWesternPlanets( const OBJECT_INCLUDES excludes )
{
	GenericTableWriter tw( horoscope );
	OBJECT_INCLUDES obs = chartprops->getWesternObjectStyle() & ~excludes;
	Table *table = tw.createObjectListTableByConfigKeys( obs,
		chartprops->getWesternColumnStyle(), false, TF_LONG );
	sheet->addItem( table );
	return table->errorcount;
}

/*****************************************************
**
**   TextHelper   ---   writeBaseData
**
******************************************************/
void TextHelper::writeBaseData()
{
	GenericTableWriter tw( horoscope );

	TcColumnSet colset( TAB_LC_NONE );
	TcColumn col( TAB_CT_CUSTOM_KEY_VALUE );
	col.title = _( "Base Data" );

	col.cells.push_back( TcCell( TAB_CELL_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_FULL_DATE ));
	col.cells.push_back( TcCell( TAB_CELL_LOCAL_TIME ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_LONGITUDE ));
	col.cells.push_back( TcCell( TAB_CELL_LOCATION_LATITUDE ));
	col.cells.push_back( TcCell( TAB_CELL_TZ_NAME ));
	col.cells.push_back( TcCell( TAB_CELL_DST ));
	col.cells.push_back( TcCell( TAB_CELL_UNIVERSAL_TIME ));
	col.cells.push_back( TcCell( TAB_CELL_SIDEREAL_TIME ));
	col.cells.push_back( TcCell( TAB_CELL_JULIAN_DATE ));
	col.cells.push_back( TcCell( TAB_CELL_VEDIC_AYANAMSA ));
	col.cells.push_back( TcCell( TAB_CELL_WESTERN_AYANAMSA ));
	col.cells.push_back( TcCell( TAB_CELL_SUNRISE ));
	col.cells.push_back( TcCell( TAB_CELL_SUNSET ));

	colset.cols.push_back( col );
	Tc tc;
	tc.colsets.push_back( colset );
	Table *table = tw.createTable( &tc );
	sheet->addItem( table );
}

/*****************************************************
**
**   TextHelper   ---   writeHoroscopeData
**
******************************************************/
void TextHelper::writeHoroscopeData()
{
	GenericTableWriter tw( horoscope );

	TcColumnSet colset( TAB_LC_NONE );
	TcColumn col( TAB_CT_CUSTOM_KEY_VALUE );
	col.title = _( "Horoscope Data" );

	col.cells.push_back( TcCell( TAB_CELL_LUNAR_TITHI ));
	col.cells.push_back( TcCell( TAB_CELL_VARNA ));
	col.cells.push_back( TcCell( TAB_CELL_YONI ));
	col.cells.push_back( TcCell( TAB_CELL_GANA ));
	col.cells.push_back( TcCell( TAB_CELL_NADI ));
	col.cells.push_back( TcCell( TAB_CELL_RAJJU ));
	col.cells.push_back( TcCell( TAB_CELL_HORA_LORD ));
	col.cells.push_back( TcCell( TAB_CELL_DINA_LORD ));
	col.cells.push_back( TcCell( TAB_CELL_MASA_LORD ));
	col.cells.push_back( TcCell( TAB_CELL_VARSHA_LORD ));

	colset.cols.push_back( col );
	Tc tc;
	tc.colsets.push_back( colset );
	Table *table = tw.createTable( &tc );
	sheet->addItem( table );
}

/*****************************************************
**
**   TextHelper   ---   writeAstronomicalData
**
******************************************************/
int TextHelper::writeAstronomicalData()
{
	wxString pname;
	int ret = 0;
	ObjectId p;

	Calculator *calculator = CalculatorFactory().getCalculator();
	Formatter *f = Formatter::get();
	Lang lang;
	SheetFormatter fmt;

	ObjectArray obs1 = chartprops->getPlanetList();
	ObjectArray obs;
	for ( uint i = 0; i < obs1.size(); i++ )
	{
		if ( obs1[i] < MAX_EPHEM_OBJECTS ) obs.push_back( obs1[i] );
	}

	Table *table = new Table( 4, obs.size() + 1 );
	if ( show_header ) table->setHeader( _( "Astronomical Positions" ));
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  _( "Longitude" ));
	table->setHeader( 2,  _( "Latitute" ));
	table->setHeader( 3,  _( "Speed (deg/day)" ));
	//table->col_line[0] = true;
	table->col_alignment[0] = Align::Right;
	table->col_alignment[1] = Align::Right;

	int line = 1;
	for ( uint i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		if ( horoscope->getTropicalLongitude( p ) == 0 ) ret++;
		pname = lang.getObjectName( p, TF_LONG );
		table->setEntry( 0, line, pname );
		table->setEntry( 1, line, fmt.getPosFormatted( horoscope->getTropicalLongitude( p ), horoscope->getMovingDirection( p ), DEG_PRECISION_MORE ));

		if ( p != OASCENDANT && p != OMERIDIAN )
		{
			table->setEntry( 2, line, f->getLatitudeFormatted( horoscope->getLatitude( p ), DEG_PRECISION_MORE ));
			table->setEntry( 3, line, f->getLenFormatted( horoscope->getSpeed( p ), DEG_PRECISION_MORE ));
		}
		line++;
	}
	sheet->addItem( table );

	Table *t2 = new Table( 2, 5 );
	t2->setHeader( _( "Ayanamsa" ));
	t2->setHeader( 0,  _( "Name" ));
	t2->setHeader( 1,  _( "Value" ));
	line = 1;
	for ( AYANAMSA i = AY_LAHIRI; i <= AY_KRISHNAMURTI; i = (AYANAMSA)( i + 1 ) )
	{
		t2->setEntry( 0, line, lang.getAyanamsaName( i ));
		t2->setEntry( 1, line, f->getDegreesFormatted( calculator->calcAyanamsa( horoscope->getJD(), i ), DEG_PRECISION_MORE ));
		line++;
	}
	t2->setEntry( 0, 4, _( "Custom" ) );
	t2->setEntry( 1, 4, f->getDegreesFormatted( calculator->calcAyanamsa( horoscope->getJD(), AY_CUSTOM ), DEG_PRECISION_MORE ));
	sheet->addItem( t2 );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeBhavas
**
******************************************************/
void TextHelper::writeBhavas()
{
	int i;
	ObjectId o;
	Lang lang;
	wxString s;
	double len;
	SheetFormatter fmt;

	Table *table = new Table( 3, 13 );
	if ( show_header ) table->setHeader( _( "Bhavas" ));
	table->setHeader( 0,  _( "Bhava" ));
	table->setHeader( 1,  _( "Cusp" ));
	table->setHeader( 2,  _( "Sandhi" ));

	int line = 1;
	for ( i = HOUSE1; i <= HOUSE12; i++ )
	{
		table->setEntry( 0, line, lang.getBhavaName( i ));
		table->setEntry( 1, line, fmt.getPosFormatted( horoscope->getHouse(i, true, false)));
		table->setEntry( 2, line, fmt.getPosFormatted( horoscope->getHouse(i, true, true)));
		line++;
	}

	sheet->addItem( table );

	ObjectArray obs = chartprops->getPlanetList();
	Table *t2 = new Table( 3, obs.size() + 1 );
	t2->setHeader( _( "Planets in Bhavas" ));
	t2->setHeader( 0,  _( "Planet" ));
	t2->setHeader( 1,  _( "Longitude" ));
	t2->setHeader( 2,  _( "Bhava" ));
	line = 1;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		o = obs[p];
		len = horoscope->getVedicLongitude( o );
		t2->setEntry( 0, line, fmt.getObjectName( o, TF_LONG, true ));
		t2->setEntry( 1, line, fmt.getPosFormatted( len, horoscope->getMovingDirection( o )));
		t2->setEntry( 2, line, lang.getBhavaName( horoscope->getHousePos( o, true )));
		line++;
	}
	sheet->addItem( t2 );
}

/*****************************************************
**
**   TextHelper   ---   writeKp
**
******************************************************/
int TextHelper::writeKp( const DasaId &dasaindex )
{
	wxString s;
	Lang lang;
	int p, ret = 0;
	ObjectId o;
	double len, hlen;
	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasaindex );
	DasaTool *tool = DasaTool::get();
	KpData kp;
	SheetFormatter fmt;

	if ( ! expert->hasKpFeatures() ) // Not supported by all (e.g. Jaimini)
	{
		sheet->addLine( _( "Not supported" ) );
		return 0;
	}
	horoscope->updateKP( dasaindex );

	if ( show_header )
	{
		sheet->addHeader( _( "Krishnamurti Paddhati" ));
		s.Printf( wxT( "%s: %s" ), _( "Dasa"), tool->getDasaName( dasaindex ).c_str());
		sheet->addParagraph( s );
	}

	OBJECT_INCLUDES kpstyle = chartprops->getVedicObjectStyle();
	//if ( chartprops->getVedicObjectStyle() & OI_ASCENDANT ) kpstyle -= OI_ASCENDANT;
	//if ( chartprops->getVedicObjectStyle() & OI_MERIDIAN ) kpstyle -= OI_MERIDIAN;
	ObjectArray obs = PlanetList().getVedicObjectList( kpstyle );

	Table *table = new Table( 7, obs.size()+13 );
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  _( "Longitude" ));
	table->setHeader( 2,  _( "Sign Lord" ));
	table->setHeader( 3,  _( "Bhava" ));
	table->setHeader( 4,  _( "KP Lord" ));
	table->setHeader( 5,  _( "Sublord" ));
	table->setHeader( 6,  _( "Subsublord" ));
	for( int i = 0; i < 7; i++ ) table->col_alignment[i] = Align::Center;

	int line = 1;
	for ( uint i1 = 0; i1 < obs.size(); i1++ )
	{
		o = obs[i1];
		len = horoscope->getVedicLongitude( o );
		if ( len == 0 ) ret++;
		table->setEntry( 0, line, fmt.getObjectName( o, TF_LONG, true ) );
		table->setEntry( 1, line, fmt.getPosFormatted( len, horoscope->getMovingDirection( o )));
		table->setEntry( 2, line, fmt.getObjectName( getLord( getRasi( len )), TF_LONG, true ) );
		s.Printf( wxT( "%02d" ), horoscope->getHousePos( o, true ) + 1 );
		table->setEntry( 3, line, s );

		kp = horoscope->getKPLords( o );
		table->setEntry( 4, line, expert->getDasaLordNameF( kp.lord, TF_LONG ));
		table->setEntry( 5, line, expert->getDasaLordNameF( kp.sublord, TF_LONG ));
		table->setEntry( 6, line, expert->getDasaLordNameF( kp.subsublord, TF_LONG ));
		line++;
	}
	for ( p = HOUSE1; p <= HOUSE12; p++ )
	{
		hlen = horoscope->getHouse( p, true, false );
		table->setEntry( 0, line, lang.getBhavaName( p ));
		table->setEntry( 1, line, fmt.getPosFormatted( hlen ));
		table->setEntry( 2, line, fmt.getObjectName( getLord(getRasi( hlen )), TF_LONG, true ) );

		kp = horoscope->getHouseKPLords( p );
		table->setEntry( 4, line, expert->getDasaLordNameF( kp.lord, TF_LONG ));
		table->setEntry( 5, line, expert->getDasaLordNameF( kp.sublord, TF_LONG ));
		table->setEntry( 6, line, expert->getDasaLordNameF( kp.subsublord, TF_LONG ));
		line++;
	}
	sheet->addItem( table );
	delete expert;
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeVedicPlanetReport
**
******************************************************/
int TextHelper::writeVedicPlanetReport()
{
	VargaExpert expert;
	ObjectId i, j;
	Lang lang;
	Calculator *calculator = CalculatorFactory().getCalculator();
	wxString s1;
	double len;
	int ret = 0;
	SheetFormatter fmt;

	JaiminiExpert jexpert( horoscope, V_RASI );
	jexpert.calcCharaKarakas();
	NakshatraExpert nexpert;

	ObjectArray obs = chartprops->getVedicPlanetList();
	Table *table = new Table( 5, obs.size() + 1 );
	if ( show_header ) table->setHeader( _( "Vedic Planets" ));
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  _( "Longitude" ));
	table->setHeader( 2,  _( "Karaka" ));
	table->setHeader( 3,  _( "Navamsa" ));
	table->setHeader( 4,  _( "Nakshatra" ));
	table->col_alignment[0] = Align::Center;
	table->col_alignment[1] = Align::Right;
	table->col_alignment[3] = Align::Center;

	//const int inode = config->iLunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	int line = 1;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		len = horoscope->getVedicLongitude( i );
		if ( len == 0 ) ret++;
		table->setEntry( 0, line, fmt.getObjectName( i, TF_LONG, true ));
		table->setEntry( 1, line, fmt.getPosFormatted( len, horoscope->getMovingDirection( i )));

		if ( i <= OSATURN ) j = i;
		else if ( i == OMEANNODE || i == OTRUENODE ) j = (ObjectId)7;
		else j = ONONE;
		if ( j != ONONE )  table->setEntry( 2, line, lang.getKarakaName( jexpert.getCharaKarakaProperty( j )));

		table->setEntry( 3, line, fmt.getSignName( horoscope->getVargaData( i, V_NAVAMSA )->getRasi(), TF_LONG ));
		table->setEntry( 4, line, lang.getNakshatra27Name( ::getNakshatra27( len ), TF_LONG ) );
		line++;
	}
	sheet->addItem( table );

	Table *t4 = new Table( 2, 5 );
	t4->setHeader( _( "Qualities" ));
	t4->setHeader( 0,  _( "Quality" ));
	t4->setHeader( 1,  _( "Value" ));
	double mlen = horoscope->getVedicLongitude( OMOON );

	line = 1;
	t4->setEntry( 0, line, _( "Varna" ) );
	t4->setEntry( 1, line++, lang.getVarnaName( getVarna( mlen )));

	Yoni yoni = nexpert.getYoni( mlen );
	t4->setEntry( 0, line, _( "Yoni" ) );
	t4->setEntry( 1, line++, yoni.name );

	t4->setEntry( 0, line, _( "Gana" ) );
	t4->setEntry( 1, line++, lang.getGanaName( nexpert.getGana( mlen )));

	t4->setEntry( 0, line, _( "Nadi" ) );
	t4->setEntry( 1, line++, lang.getNadiName( nexpert.getNadi( mlen )));
	sheet->addItem( t4 );

	Table *t5 = new Table( 3, 3 );
	t5->setHeader( _( "Moon's Nakshatra and Pada Portions" ));
	t5->setHeader( 0,  _( "Quality" ));
	t5->setHeader( 1,  _( "Value" ));
	t5->setHeader( 2,  _( "Value (Percent)" ));
	t5->setEntry( 0, 1, _( "Nakshatra" ) );
	double nportion = calculator->calcNakshatraPortion( horoscope->getDataSet(), mlen, false );
	s1.Printf( wxT( "%1.6f" ), nportion );
	t5->setEntry( 1, 1, s1 );

	s1.Printf( wxT( "%01.2f%%" ), 100.0 * nportion );
	t5->setEntry( 2, 1, s1 );

	t5->setEntry( 0, 2, _( "Pada" ) );
	double pportion = calculator->calcNakshatraPortion( horoscope->getDataSet(), mlen, true );
	s1.Printf( wxT( "%1.6f" ), pportion );
	t5->setEntry( 1, 2, s1 );

	s1.Printf( wxT( "%01.2f%%" ), 100.0 * pportion );
	t5->setEntry( 2, 2, s1 );

	sheet->addItem( t5 );

	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeWesternPlanetReport
**
******************************************************/
int TextHelper::writeWesternPlanetReport()
{
	wxString s;
	double len;
	Lang lang;
	int ret = 0;
	SheetFormatter fmt;

	ObjectArray obs = chartprops->getWesternPlanetList();
	Table *table = new Table( 3, obs.size() + 1 );
	if ( show_header ) table->setHeader( _( "Western Planets" ));
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  _( "Longitude" ));
	table->setHeader( 2,  _( "House" ));
	table->col_alignment[0] = Align::Center;

	int line = 1;
	for ( uint i = 0; i < obs.size(); i++ )
	{
		len = horoscope->getWesternLongitude( obs[i] );
		if ( len == 0 ) ret++;
		table->setEntry( 0, line, fmt.getObjectName( obs[i], TF_LONG, false ) );
		table->setEntry( 1, line, fmt.getPosFormatted( len, horoscope->getMovingDirection( obs[i] ), DEG_PRECISION_SECOND ) );
		s.Printf( wxT( "%02d" ), horoscope->getHousePos( obs[i], false ) + 1 );
		table->setEntry( 2, line, s );
		line++;
	}
	sheet->addItem( table );
	return ret;
}

/*****************************************************
**
**   TextHelper   ---   writeTransitReport
**
******************************************************/
int TextHelper::writeTransitReport( const PlanetContext &ctx )
{
	assert( horoscope );

	OBJECT_INCLUDES obs = OI_NONE;
	if ( ctx == PcTransit || ctx == PcDirection ) obs = OI_ARIES;
	return writePlanets( obs );
}

/*****************************************************
**
**   TextHelper   ---   writeSynastryReport
**
******************************************************/
int TextHelper::writeSynastryReport( Horoscope *h2, AspectExpert* /*aexpert*/ )
{
	assert( horoscope );
	assert( h2 );
	SheetFormatter fmt;
	ObjectId p;
	ObjectPosition pos, pos2;
	const TEXT_FORMAT format = TF_MEDIUM;

	ObjectArray obs = chartprops->getPlanetList();

	Table *table = new Table( 3, obs.size() + 1 );
	if ( show_header ) table->setHeader( _( "Partner Positions" ));
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  horoscope->getHName());
	table->setHeader( 2,  h2->getHName());

	for ( uint i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		pos = horoscope->getObjectPosition( p, chartprops->isVedic() );
		pos2 = h2->getObjectPosition( p, chartprops->isVedic() );

		table->setEntry( 0, i + 1, fmt.getObjectName( p, format, chartprops->isVedic() ));
		table->setEntry( 1, i + 1, fmt.getPosFormatted( pos.longitude, pos.direction, DEG_PRECISION_SECOND, format ));
		if ( IS_EPHEM_OBJECT( obs[p] ) && horoscope->getTropicalLongitude( obs[p] ) == 0 ) table->errorcount++;

		table->setEntry( 2, i + 1, fmt.getPosFormatted( pos2.longitude, pos2.direction, DEG_PRECISION_SECOND, format ));
		if ( IS_EPHEM_OBJECT( obs[p] ) && h2->getTropicalLongitude( obs[p] ) == 0 ) table->errorcount++;
	}
	sheet->addItem( table );
	return 0;
}


