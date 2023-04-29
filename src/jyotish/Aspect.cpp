/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Aspect.cpp
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

#include "Aspect.h"

#include "astrobase.h"
#include "maitreya.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "PlanetList.h"
#include "Sheet.h"
#include "Table.h"
#include "VargaHoroscope.h"
#include "VedicChartConfig.h"

#include <math.h>
#include <wx/tokenzr.h>

extern Config *config;

// Aspect groups
enum ASPECT_GROUP { AG_MAJOR = 0, AG_MINOR, AG_TERNARY, AG_DECLINATION, AG_ANTI_DECLINATION, AG_PLAIN_URANIAN };

/*************************************************//**
*
* \brief constants for aspect parameters and their defaults
*
******************************************************/
static const struct AspectTypeDefinition
{
	wxString name;
	ASPECT_GROUP aspect_group;
	int numerator;
	int divisor; // (denominator) >= 1
	double angle;
	wxString shortname;
}
AspectTypeDefinitions[MAX_ASPECT_TYPES] =
{
	// Major
	{ _( "Conjunction" ), AG_MAJOR, 1, 1, 0.0, wxT( "Cnj" ) },
	{ _( "Opposition" ), AG_MAJOR, 1, 2, 180.0, wxT( "Opp" ) },
	{ _( "Trine" ), AG_MAJOR, 1, 3, 120.0, wxT( "Tri" ) },
	{ _( "Square" ), AG_MAJOR, 1, 4, 90.0, wxT( "Sqr" ) },
	{ _( "Sextile" ), AG_MAJOR, 1, 6, 60.0, wxT( "Sxt" ) },

	// Minor
	{ _( "Quincunx" ), AG_MINOR, 5, 12, 150.0, wxT( "Qncx" ) },
	{ _( "Semisquare" ), AG_MINOR, 1, 8, 45.0, wxT( "1/8" ) },
	{ _( "Sesquisquare" ), AG_MINOR, 3, 8, 135.0, wxT( "3/8" ) },
	{ _( "Semisextile" ), AG_MINOR, 1, 12, 30.0, wxT( "1/12" ) },
	{ _( "Quintile" ), AG_MINOR, 1, 5, 72.0, wxT( "1/5" ) },
	{ _( "Biquintile" ), AG_MINOR, 2, 5, 144.0, wxT( "2/5" ) },

	// Declination
	{ _( "Parallele" ), AG_DECLINATION, 1, 1, 0.0, wxT( "Par" ) },
	{ _( "Contraparallele" ), AG_ANTI_DECLINATION, 1, 2, -1.0, wxT( "CPar" ) },

	// Ternary
	{ _( "Septile" ), AG_TERNARY, 1, 7, 51.42857, wxT( "1/7" ) },
	{ _( "Biseptile" ), AG_TERNARY, 2, 7, 102.85714, wxT( "2/7" ) },
	{ _( "Triseptile" ), AG_TERNARY, 3, 7, 154.28571, wxT( "3/7" ) },
	{ _( "Novile" ), AG_TERNARY, 1, 9, 40.0, wxT( "1/9" ) },
	{ _( "Binovile" ), AG_TERNARY, 2, 9, 80.0, wxT( "2/9" ) },
	{ _( "Quadnovile" ), AG_TERNARY, 4, 9, 160.0, wxT( "4/9" ) },

	// Plain Uranian
	{ wxT( "Quartersquare" ), AG_PLAIN_URANIAN, 1, 16, 22.5, wxT( "22.5" ) },
	{ wxT( "Quaver" ), AG_PLAIN_URANIAN, 1, 32, 11.25, wxT( "11.25" ) },
	{ wxT( "Semiquaver" ), AG_PLAIN_URANIAN, 1, 64, 5.625, wxT( "5.625" ) }
};
// Total: MAX_ASPECT_TYPES = 22

/*************************************************//**
*
* \brief sorts aspect lists
*
******************************************************/
class AspectSorter
{
public:
	AspectSorter( const ASPECT_SORTORDER &ord ) {
		order = ord;
	}
	bool operator()( const AspectEvent &e1, const AspectEvent &e2 ) const
	{
		switch ( order )
		{
		case AS_RPLANET:
			if ( e1.planet2 != e2.planet2 ) return ( e1.planet2 < e2.planet2 );
			return ( e1.planet1 < e2.planet1 );
		break;
		case AS_ORBIS:
			return( e1.orbis < e2.orbis );
		break;
		case AS_ORBIS_ABSOLUTE:
			return( fabs( e1.orbis ) < fabs( e2.orbis ));
		break;
		case AS_TYPE:
			if ( e1.aspectType != e2.aspectType ) return( e1.aspectType < e2.aspectType );
			return( fabs( e1.orbis ) < fabs( e2.orbis ));
		break;
		case AS_TYPE_REVERSE:
			if ( e1.aspectType != e2.aspectType ) return( e1.aspectType > e2.aspectType );
			return( fabs( e1.orbis ) > fabs( e2.orbis ));
		break;
		case AS_LPLANET:
		default:
			if ( e1.planet1 != e2.planet1 ) return ( e1.planet1 < e2.planet1 );
			return ( e1.planet2 < e2.planet2 );
		break;
		}
		return false;
	}
private:
	ASPECT_SORTORDER order;
};

/*****************************************************
**
**   AspectExpert   ---   Constructor
**
******************************************************/
AspectExpert:: AspectExpert()
	: Expert()
{
	sortorder = AS_RPLANET;
}

/*****************************************************
**
**   AspectExpert   ---   Constructor
**
******************************************************/
AspectExpert::AspectExpert( Horoscope *h, Horoscope *h2 )
	: Expert( h )
{
	setHoroscopes( h, h2 );
	sortorder = AS_RPLANET;
}

/*****************************************************
**
**   AspectExpert   ---   setHoroscopes
**
******************************************************/
void AspectExpert::setHoroscopes( Horoscope *h1, Horoscope *h2 )
{
	horoscope = h1;
	horoscope2 = h2 ? h2 : h1;
}

/*****************************************************
**
**   AspectExpert   ---   updateSbcAspects
**
******************************************************/
std::list<AspectEvent> &AspectExpert::updateSbcAspects( ChartProperties *chartprops, const bool &swapCharts )
{
	//printf( "AspectExpert::updateSbcAspects\n" );
	if ( ! horoscope )
	{
		printf( "WARN: AspectExpert::updateSbcAspects horoscope is NULL\n" );
		clear();
		return al;
	}
	assert( chartprops );
	const ObjectArray planets = chartprops->getVedicPlanetList();
	NakshatraId_28 nak;
	SarvatobhadraExpert expert;
	VedicChartBehaviorConfig &bcfg = chartprops->getVedicChartBehaviorConfig();
	std::vector<NakshatraId_28> vedhas;

	Horoscope *h = swapCharts ? horoscope2 : horoscope;

	for( uint i = 0; i < planets.size(); i++ )
	{
		ObjectId p = planets[i];
		if ( ! bcfg.includeAsMc && IS_ANGLE_OBJECT( p )) continue;

		nak = getNakshatra28( h->getVedicLongitude( p ));
		vedhas = expert.getVedhaNakshatras( nak );
		for ( uint j = 0; j < vedhas.size(); j++ )
		{
			if ( vedhas[j] == nak ) continue;
			al.push_back( AspectEvent( nak, vedhas[j], AT_SBC_VEDHA, VA_FULL, h->isBenefic( p ), swapCharts ));
		}
	}
	return al;
}

/*****************************************************
**
**   AspectExpert   ---   updateVedicRasiChartAspects
**
******************************************************/
std::list<AspectEvent> &AspectExpert::updateVedicRasiChartAspects( ChartProperties *chartprops, const Varga &varga,
	const bool &swapCharts )
{
	//printf( "AspectExpert::updateVedicRasiChartAspects\n" );
	if ( ! horoscope )
	{
		printf( "WARN: AspectExpert::updateVedicRasiChartAspects horoscope is NULL\n" );
		clear();
		return al;
	}
	assert( chartprops );
	int rasi, targetField;
	VedicChartBehaviorConfig &bcfg = chartprops->getVedicChartBehaviorConfig();
	const ASPECT_TYPE aspectType = bcfg.aspectType ? AT_RASI_DRISHTI : AT_GRAHA_DRISHTI;

	const ObjectArray planets = chartprops->getVedicPlanetList();

	Horoscope *h = swapCharts ? horoscope2 : horoscope;

	const int rasi_drishti[12][3] = {
		{ R_LEO, R_SCORPIO, R_AQUARIUS },
		{ R_CANCER, R_LIBRA, R_CAPRICORN },
		{ R_VIRGO, R_SAGITTARIUS, R_PISCES },

		{ R_TAURUS, R_SCORPIO, R_AQUARIUS },
		{ R_ARIES, R_LIBRA, R_CAPRICORN },
		{ R_GEMINI, R_SAGITTARIUS, R_PISCES },

		{ R_TAURUS, R_LEO, R_AQUARIUS },
		{ R_ARIES, R_CANCER, R_CAPRICORN },
		{ R_GEMINI, R_VIRGO, R_PISCES },

		{ R_TAURUS, R_LEO, R_SCORPIO },
		{ R_ARIES, R_CANCER, R_LIBRA },
		{ R_GEMINI, R_VIRGO, R_SAGITTARIUS }
	};

	for( uint i = 0; i < planets.size(); i++ )
	{
		ObjectId p = planets[i];
		if ( ! bcfg.includeAsMc && IS_ANGLE_OBJECT( p )) continue;

		rasi = (int)h->getVargaData( p, varga )->getRasi();

		if ( aspectType == AT_GRAHA_DRISHTI )
		{
			for ( int i = 1; i < 12; i++ )
			{
			targetField = red12( rasi + i );
				switch( i )
				{
					case 6:
						al.push_back( AspectEvent( rasi, targetField, AT_GRAHA_DRISHTI, VA_FULL, h->isBenefic( p ), swapCharts ));
					break;
					case 3:
					case 7:
						if ( p == OMARS ) al.push_back( AspectEvent( rasi, targetField, AT_GRAHA_DRISHTI, VA_FULL, false, swapCharts ));
						else if ( bcfg.grahaDrishtiMinimumStrength >= VA_75 )
						{
							al.push_back( AspectEvent( rasi, targetField, AT_GRAHA_DRISHTI, VA_75, h->isBenefic( p ), swapCharts ));
						}
					break;
					case 4:
					case 8:
						if ( p == OJUPITER ) al.push_back( AspectEvent( rasi, targetField, AT_GRAHA_DRISHTI, VA_FULL, true, swapCharts ));
						else if ( bcfg.grahaDrishtiMinimumStrength >= VA_50 )
						{
							al.push_back( AspectEvent( rasi, targetField, AT_GRAHA_DRISHTI, VA_50, h->isBenefic( p ), swapCharts ));
						}
					break;
					case 2:
					case 9:
						if ( p == OSATURN ) al.push_back( AspectEvent( rasi, targetField, AT_GRAHA_DRISHTI, VA_FULL, false, swapCharts ));
						else if ( bcfg.grahaDrishtiMinimumStrength >= VA_25 )
						{
							al.push_back( AspectEvent( rasi, targetField, AT_GRAHA_DRISHTI, VA_25, h->isBenefic( p ), swapCharts ));
						}
					break;
				}
			}
		}
		else if ( aspectType == AT_RASI_DRISHTI )
		{
			for( int i = 0; i < 3; i++ )
			{
				targetField = red12( rasi_drishti[rasi][i] );
				al.push_back( AspectEvent( rasi, targetField, AT_RASI_DRISHTI, VA_FULL, h->isBenefic( p ), swapCharts ));
			}
		}
		else
		{
			printf( "Wrong aspect type %d in AspectExpert::getVedicAspects\n", aspectType );
			assert( false );
		}
	}
	return al;
}

/*****************************************************
**
**   AspectExpert   ---   updateWesternAspects
**
******************************************************/
std::list<AspectEvent> &AspectExpert::updateWesternAspects( ChartProperties *chartprops, const ChartType& charttype )
{
	if ( ! horoscope )
	{
		printf( "WARN: AspectExpert::updateWesternAspects horoscope is NULL\n" );
		clear();
		return al;
	}
	assert( chartprops );

	uint p1, p2, p2start;
	ObjectId ob1, ob2;
	double orbis, lendiff;
	//AspectConfigLoader *loader = AspectConfigLoader::get();
	ObjectArray qlist = chartprops->getWesternPlanetList();
	ObjectArray plist = chartprops->getWesternPlanetList( chartprops->getObjectFilter());

	if ( ! horoscope2 )
	{
		horoscope2 = horoscope;
	}
	//const bool doubleSided = charttype == CT_PARTNER;

	//printf( "&AspectExpert::update doubleSided %d h2 %ld\n", doubleSided, (long)horoscope2 );

	al.clear();

	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		ob1 = plist[p1];

		// calculation of starting point for second parameter
		if ( charttype == CT_RADIX ) p2start = p1 + 1;
		else p2start = 0;

		for ( p2 = p2start; p2 < qlist.size(); p2++ )
		{
			ob2 = qlist[p2];

			// no aspects if both are Lunar Nodes
			if ( IS_LUNAR_NODE( ob1 ) && IS_LUNAR_NODE( ob2 )) continue;

			// no aspects if one of them is a house cusps
			if ( IS_HOUSE_OBJECT( ob1 ) || IS_HOUSE_OBJECT( ob2 )) continue;

			// aries excluded - NO
			//if ( ob1 == OARIES || ob2 == OARIES ) continue;

			// do not compare ascendent etc items that are alwys in opposition
			if ( ob1 == OASCENDANT && ob2 == ODESCENDANT ) continue;
			if ( ob1 == ODESCENDANT && ob2 == OASCENDANT ) continue;
			if ( ob1 == OMERIDIAN && ob2 == OIMUMCOELI ) continue;
			if ( ob1 == OIMUMCOELI && ob2 == OMERIDIAN ) continue;

			lendiff = fabs( horoscope->getWesternLongitude( ob1 ) - horoscope2->getWesternLongitude( ob2 ));
			if ( lendiff > 180 ) lendiff = 360 - lendiff;
			if ( lendiff < -180 ) lendiff = 360 + lendiff;

			for ( ASPECT_TYPE i = AT_CONJUNCTION; i <= AT_QUADNOVILE; i++ )
			{
				if ( ! config->aspects[i].active ) continue;

				switch ( AspectTypeDefinitions[i].aspect_group )
				{
				case AG_DECLINATION:
					orbis = fabs( horoscope->getLatitude( ob1 ) - horoscope2->getLatitude( ob2 ));
					break;
				case AG_ANTI_DECLINATION:
					orbis = fabs( horoscope->getLatitude( ob1 ) + horoscope2->getLatitude( ob2 ));
					break;
				default:
					orbis = fabs( lendiff - AspectTypeDefinitions[i].angle );
					break;
				}
				//printf( "planet1 %d planet2 %d aspect %d orbis %f\n", (int)ob1, (int)ob2, (int)i, orbis );
				if ( orbis < config->aspects[i].orbis )
				{
					al.push_back( AspectEvent( ob1, ob2, i, orbis ));
				}
			}
		}
	}
	return al;
}

/*****************************************************
**
**   AspectExpert   ---   sort
**
******************************************************/
void AspectExpert::sort()
{
	al.sort( AspectSorter( sortorder ));
}

/*****************************************************
**
**   AspectExpert   ---   clear()
**
******************************************************/
void AspectExpert::clear()
{
	al.clear();
}

/*****************************************************
**
**   AspectExpert   ---   getAspectName
**
******************************************************/
wxString AspectExpert::getAspectName( const int&i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	return AspectTypeDefinitions[i].name;
}

/*****************************************************
**
**   AspectExpert   ---   getAspectShortDescription
**
******************************************************/
wxString AspectExpert::getAspectShortDescription( const int&i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );
	wxString s;
	if ( AspectTypeDefinitions[i].aspect_group == AG_DECLINATION ) s << wxT( "p" );
	else if ( AspectTypeDefinitions[i].aspect_group == AG_ANTI_DECLINATION ) s << wxT( "cp" );
	else s << AspectTypeDefinitions[i].numerator << wxT( "/" ) << AspectTypeDefinitions[i].divisor;
	return s;
}

/*****************************************************
**
**   AspectExpert   ---   getAspectDescription
**
******************************************************/
wxString AspectExpert::getAspectDescription( const int&i )
{
	assert( i >= 0 && i < MAX_ASPECT_TYPES );

	//wxString s = wxGetTranslation( (const wxChar*)AspectTypeDefinitions[i].name );
	wxString s = wxGetTranslation( AspectTypeDefinitions[i].name.c_str() );

	switch ( AspectTypeDefinitions[i].aspect_group )
	{
	case AG_DECLINATION:
	case AG_ANTI_DECLINATION:
		s << wxT( " (decl)" );
		break;
	case AG_MAJOR:
	case AG_MINOR:
	case AG_TERNARY:
	default:
		s << wxT( " (" ) << AspectTypeDefinitions[i].numerator << wxT( "/" ) << AspectTypeDefinitions[i].divisor << wxT( ")" );
		break;
	}
	return s;
}

/*****************************************************
**
**   AspectExpert   ---   writeWesternAspectarium
**
******************************************************/
void AspectExpert::writeWesternAspectarium( Sheet *sheet, ChartProperties *chartprops, const bool show_header )
{
	Table * table = getWesternAspectariumTable( sheet, chartprops );
	if ( show_header ) table->setHeader( _( "Aspectarium" ));
	sheet->addItem( table );
}

/*****************************************************
**
**   AspectExpert   ---   getWesternAspectariumTable
**
******************************************************/
Table *AspectExpert::getWesternAspectariumTable( Sheet* /*sheet*/, ChartProperties *chartprops )
{
	std::list<AspectEvent>::iterator iter;
	uint p1, p2;
	SheetFormatter fmt;
	MString ms;

	OBJECT_INCLUDES style = chartprops->getWesternObjectStyle();
	style &= ~OI_ALL_HOUSES;
	style &= ~OI_4_HOUSES;
	//style &= ~OI_ARIES;

	ObjectArray plist = PlanetList().getWesternObjectList( style );
	Table *table = new Table( plist.size() + 1, plist.size() + 1 );
	table->uniformLayout = true;
	table->setCenterAll();

	table->setHeader( 0, wxEmptyString );
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		table->setHeader( p1 + 1, fmt.getObjectName( plist[p1], TF_SHORT ));
		table->setHeaderEntry( 0, p1 + 1, fmt.getObjectName( plist[p1], TF_MEDIUM ));
		table->setEntry( p1 + 1, p1 + 1, wxT( "-" ));
	}
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		for ( p2 = 0; p2 < plist.size(); p2++ )
		{
			if ( p1 == p2 ) continue;
			ms.clear();
			for ( iter = al.begin(); iter != al.end(); iter++ )
			{
				if ( (*iter).planet1 == plist[p1] && (*iter).planet2 == plist[p2] )
				{
					ms.add( MToken( TTSE_ASPECT, (int)(*iter).aspectType, TF_MEDIUM ));
				}
			}
			table->setEntry( p1 + 1, p2 + 1, ms );
		}
	}
	return table;
}

/*****************************************************
**
**   AspectExpert   ---   writeVedicAspectarium
**
******************************************************/
void AspectExpert::writeVedicAspectarium( Sheet *sheet, ChartProperties *chartprops,
	const ChartType& /*charttype*/, const bool show_header )
{
	int rasidiff, drishti;
	uint p1, p2;
	wxString s, s1;
	SheetFormatter fmt;

	//printf( "AspectExpert::writeVedicAspectarium h1 set %d h2set %d\n", horoscope != 0, horoscope2 != 0 );
	if ( ! horoscope || ! horoscope2  )
	{
		sheet->addHeader( _( "No horoscope set" ));
		return;
	}

	OBJECT_INCLUDES style = chartprops->getVedicObjectStyle();
	style &= ~OI_ALL_HOUSES;
	style &= ~OI_4_HOUSES;
	//style &= ~OI_ARIES;

	ObjectArray plist = PlanetList().getVedicObjectList( style );
	Table *table = new Table( plist.size() + 1, plist.size() + 1 );
	table->uniformLayout = true;
	if ( show_header ) table->setHeader( _( "Vedic Aspectarium" ));

	table->setHeader( 0, wxEmptyString );
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		table->setHeader( p1 + 1, fmt.getObjectName( plist[p1], TF_MEDIUM, true ));
		table->setHeaderEntry( 0, p1 + 1, fmt.getObjectName( plist[p1], TF_MEDIUM, true ));
		table->setEntry( p1 + 1, p1 + 1, wxT( "-" ));
	}
	for ( p1 = 0; p1 < plist.size(); p1++ )
	{
		for ( p2 = 0; p2 < plist.size(); p2++ )
		{
			if ( p1 == p2 ) continue;
			rasidiff = red12( getRasi( horoscope->getVedicLongitude( plist[p1] ))
			                  - getRasi( horoscope2->getVedicLongitude( plist[p2] )));
			drishti = (int)(60.0 * getGrahaDrishtiValue( plist[p2], rasidiff ));
			table->setEntry( p1 + 1, p2 + 1, wxString::Format( wxT( "%d" ), drishti ));
		}
	}
	sheet->addItem( table );
}

/*****************************************************
**
**   AspectExpert   ---   writeWesternAspectList
**
******************************************************/
void AspectExpert::writeWesternAspectList( Sheet *sheet, ChartProperties*, const ChartType &charttype, const bool show_header )
{
	Formatter *formatter = Formatter::get();
	SheetFormatter fmt;
	int line = 1;

	setSortOrder( AS_RPLANET );
	sort();

	Table *table = new Table( 4, al.size() + 1 );
	if ( show_header ) table->setHeader( _( "Aspects" ));
	switch( charttype )
	{
		case CT_TRANSIT:
			table->setHeader( 0, _( "Running" ) );
			table->setHeader( 1, _( "Radix" ) );
		break;
		case CT_PARTNER:
			assert( horoscope );
			table->setHeader( 0, horoscope->getHName());
			table->setHeader( 1, horoscope2->getHName());
		break;
		case CT_RADIX:
		default:
			table->setHeader( 0, _( "Aspecting" ) );
			table->setHeader( 1, _( "Aspected" ) );
		break;
	}
	table->setHeader( 2, _( "Type" ) );
	table->setHeader( 3, _( "Orbis" ) );

	for ( std::list<AspectEvent>::iterator iter = al.begin(); iter != al.end(); iter++ )
	{
		table->setEntry( 0, line, fmt.getObjectName( (*iter).planet1, TF_LONG ));
		table->setEntry( 1, line, fmt.getObjectName( (*iter).planet2, TF_LONG ));
		//table->setEntry( 2, line, fmt.getAspectSymbol( (*iter).aspectType ));
		MToken token( TTSE_ASPECT, (int)(*iter).aspectType, TF_MEDIUM );
		MString m;
		m.add( token );
		table->setEntry( 2, line, m );
		table->setEntry( 3, line++,  formatter->getLenFormatted( (*iter).orbis ));
	}
	sheet->addItem( table );
}


