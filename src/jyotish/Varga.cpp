/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Varga.cpp
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

#include "Varga.h"

#include <wx/log.h>
#include <wx/string.h>
#include <wx/tokenzr.h>
#include <math.h>
#include <vector>

#include "astrobase.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "maitreya.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Sheet.h"
#include "Table.h"
#include "VargaBase.h"
#include "VedicPlanet.h"

extern Config *config;

/*****************************************************
**
**   VargaData   ---   Constructor
**
******************************************************/
VargaData::VargaData( const VedicPlanet *iplanet, const Varga &d )
		:	planet( iplanet ),
		division( d )
{
	rasi = R_NONE;
	dignity = DG_NONE;
	friendship = PF_NONE;
	vargaViswa = saptaVargajaBala = -1;
}

/*****************************************************
**
**   VargaExpert   ---   getDvadasamsaLongitude
**
******************************************************/
double VargaExpert::getDvadasamsaLongitude( const double& len ) const
{
	return red_deg( getRasi( len ) * 30 + getRasiLen( len ) * 12 );
}

/*****************************************************
**
**   VargaExpert   ---   calcVarga
**
******************************************************/
Rasi VargaExpert::calcVarga( const double& l, const Varga &division )
{
	ASSERT_VALID_VARGA( division );
	double len = l;
	double ret = 0;
	double basepos, rs;
	Rasi rasi;
	assert( horoscope );

	switch ( division )
	{
		// D1
	case V_RASI:
		ret = len;
		break;

		// D9
	case V_NAVAMSA:
		ret =  9 * len;
		break;

		// D2
	case V_HORA:
		switch ( config->vedicCalculation->vargaHoraMode )
		{
		case 0: // Normal mode: Parasara
			ret = a_red( len - 15, 60 ) + 90;
			break;
		case 1: // Continuous
			ret = 2 * len;
			break;
		default:
			assert( false );
			break;
		}
		break;

		// D3
	case V_DREKKANA:
		switch ( config->vedicCalculation->vargaDrekkanaMode )
		{
		case 0: // normal Mode: Parasara
			ret = ( floor( getRasiLen( len ) / 10 )  * 120 + getRasi( len ) * 30 + getRasiLen( 3 * len ));
			break;
		case 1: // Continuous
			ret = 3 * len;
			break;
		default:
			assert( false );
			break;
		}
		break;

		// D4
	case V_CHATURTAMSA:
		switch ( config->vedicCalculation->vargaChaturtamsaMode )
		{
		case 0: // Normal mode: Parasara
			ret = ( floor( getRasiLen( len ) / 7.5 )  * 90 + getRasi( len ) * 30 + getRasiLen( 4 * len ));
			break;
		case 1: // Continuous
			ret = 2 * len;
			break;
		default:
			assert( false );
			break;
		}
		break;

		// D6
	case V_SASTAMSA:
		ret =  6 * len;
		//ret = ( getRasi( len ) * 30 + getRasiLen( len ) * 5 );
		break;

		// D7
	case V_SAPTAMAMSA:
		basepos = getRasi( len ) * 30 + getRasiLen( len ) * 7;
		if ( isOddRasi( len ) == 1 ) ret = basepos;
		else ret = basepos + 180;
		break;

		// D8
	case V_ASHTAMSA:
		ret = 8 * len;
		break;

		// D10
	case V_DASAMSA:
		basepos = getRasi( len ) * 30 + getRasiLen( len ) * 10;
		if ( isOddRasi( len ) == 1 ) ret = basepos;
		else ret = basepos + 240;
		break;

		// D12
	case V_DVADASAMSA:
		ret = getDvadasamsaLongitude( len );
		break;

		// D16
	case V_SHODASAMSA:
		ret = 16 * len;
		break;

		// D20
	case V_VIMSAMSA:
		ret = 20 * len;
		break;

		// D24
	case V_SIDDHAMSA:
		basepos = getRasiLen( len ) * 24;
		if ( isOddRasi( len ) == 1 ) ret = basepos + 120;
		else ret = basepos + 90;
		break;

		// D27
	case V_BHAMSA:
		ret =  27 * len;
		break;

		// D30
	case V_TRIMSAMSA:
		rs = getRasiLen( len );
		if ( isOddRasi( len ) == 1 )
		{
			// Ar, Aq, Sa, Ge, Li
			if ( rs < 5 )
				ret = 30 * R_ARIES + rs*6;
			else if (( rs >=  5 ) && ( rs <= 10 ))
				ret = 30 * R_AQUARIUS + (rs - 5)*6;
			else if (( rs >= 10 ) && ( rs <= 18 ))
				ret = 30 * R_SAGITTARIUS + (rs - 10)/4 * 15;
			else if (( rs >=  18 ) && ( rs <= 25 ))
				ret = 30 * R_GEMINI + ( rs - 18 )/7 * 30;
			else if ( rs > 25 )
				ret = 30 * R_LIBRA + (rs - 25)*6;
		}
		else
		{
			// Ta, Vi, Pi, Cp, Sc
			if ( rs < 5 ) ret = 30 * R_TAURUS + (5-rs)*6;
			else if (( rs >=  5 ) && ( rs <= 10 )) ret = 30 * R_VIRGO + (10-rs)*6;
			else if (( rs >= 10 ) && ( rs <= 18 )) ret = 30 * R_PISCES + ( 18-rs)/4 * 15;
			else if (( rs >= 18 ) && ( rs <= 25 )) ret = 30 * R_CAPRICORN + (25-rs)/7 * 30;
			else if ( rs > 25 ) ret = 30 * R_SCORPIO + ( 30 -rs)*6;
		}
		break;

		// D40
	case V_CHATVARIMSAMSA:
		basepos = getRasiLen( len ) * 40;
		if ( isOddRasi( len ) == 1 ) ret = basepos;
		else ret = basepos + 180;
		break;

		// D45
	case V_AKSHAVEDAMSA:
		basepos = getRasiLen( len ) * 45;
		if ( inMovableSign( len )) ret = basepos;
		else if ( inFixedSign( len )) ret = basepos + 120;
		else ret = basepos + 240;
		break;

		// D60
	case V_SHASTIAMSA:
		ret = 60 * getRasiLen( len ) + getRasi( len ) * 30;
		break;

		// D108
	case V_ASHTOTTARAMSA:
		ret = getDvadasamsaLongitude( 9 * len );
		break;

		// D144
	case V_DVADASDVADASAMSA:
		ret = getDvadasamsaLongitude( getDvadasamsaLongitude ( len ));
		break;

		// Bhava
	case V_BHAVA:
		for ( int i = HOUSE1; i <= HOUSE12; i++ )
		{
			assert( horoscope );
			double hstart = 0, hend = 0;
			if ( config->vedicCalculation->houseUseCusps )
			{
				hstart = horoscope->getHouse( i, true, false );
				hend = horoscope->getHouse( red12(i+1), true, false );
			}
			else
			{
				hstart = horoscope->getHouse( red12(i-1), true, true );
				hend = horoscope->getHouse( i, true, true );
			}
			if (( hstart <= len && len < hend ) || ( hstart > hend && ( len >= hstart || len < hend )))
			{
				ret = red_deg( 30 * ( i + getRasi( horoscope->getHouse( HOUSE1, true ))) + 15 );
				break;
			}
		}
		break;

	default: // invalid varga
		assert( 0 );
		break;
	}

	// ret has degree value, so get rasi
	rasi = getRasi( red_deg( ret ));
	ASSERT_VALID_RASI( rasi );
	return rasi;
}

/*****************************************************
**
**   VargaExpert   ---   write
**
******************************************************/
void VargaExpert::write( Sheet *sheet, const OBJECT_INCLUDES &style, const bool /*show_header*/ )
{
	assert( horoscope );

	int gv;
	ObjectId p;
	uint i;
	wxString s;
	VedicPlanet *planet;
	Lang lang;
	PlanetList planetlist;
	SheetFormatter fmt;
	VargaConfigLoader *loader = VargaConfigLoader::get();

	const ObjectArray obs = planetlist.getVedicObjectList( style );
	const ObjectArray obsp = planetlist.getVedicPlanetOnlyList();

	Table *t1 = new Table( NB_VARGAS, obs.size()+1 );
	Table *t2 = new Table( NB_VARGAS, 8 );
	Table *t3 = new Table( NB_VARGAS, 8 );

	t1->setHeader(  _( "Vargas" ));
	t1->setHeader( 0, wxT( "" ));
	for ( Varga v = V_FIRST; v < V_LAST; v++ )
	{
		s.Printf( wxT( "D-%d" ), loader->getVargaDivision( v ));
		t1->setHeader( v+1, s );
		t2->setHeader( v+1, s );
		t3->setHeader( v+1, s );
	}
	int line = 1;
	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		t1->setEntry( 0, line, fmt.getObjectName( p, TF_MEDIUM, true ));

		for ( Varga v = V_FIRST; v < V_LAST; v++ )
		{
			t1->setEntry( v+1, line, fmt.getSignName( horoscope->getVargaData( p, v )->getRasi()));
		}
		line++;
	}
	sheet->addItem( t1 );

	/*
	* Dignity
	*/
	t2->setHeader(  _( "Dignities" ));
	t2->setHeader( 0, wxT( "" ));

	line = 1;
	for ( i = 0; i < obsp.size(); i++ )
	{
		p = obsp[i];

		t2->setEntry( 0, line, fmt.getObjectName( p, TF_MEDIUM ));
		planet = horoscope->getPlanet( p );

		for ( Varga v = V_FIRST; v < V_LAST; v++ )
		{
			//s.Printf( wxT( "%02d" ), (int)horoscope->getVargaData( p, v )->getVargaViswa() );
			t2->setEntry( v+1, line, lang.getDignityName( horoscope->getVargaData( p, v )->getDignity(), TF_SHORT ) );
		}
		line++;
	}
	sheet->addItem( t2 );

	/*
	* Varga Viswa
	*/
	t3->setHeader(  _( "Varga Viswa" ));
	t3->setHeader( 0, wxT( "" ));

	line = 1;
	for ( i = 0; i < obsp.size(); i++ )
	{
		p = obsp[i];
		t3->setEntry( 0, line, fmt.getObjectName( p, TF_MEDIUM ));
		planet = horoscope->getPlanet( p );

		for ( Varga v = V_FIRST; v < V_LAST; v++ )
		{
			s.Printf( wxT( "%d" ), (int)horoscope->getVargaData( p, v )->getVargaViswa() );
			t3->setEntry( v+1, line, s );
		}
		line++;
	}
	sheet->addItem( t3 );

	Table *t4 = new Table( 15, 8 );
	t4->setHeader(  _( "Vimsopaka Bala" ));
	t4->setHeader( 0, wxT( "" ));
	t4->setHeader( 1, _( "V-6" ));
	t4->setHeader( 2, _( "Own Vargas" ));
	t4->setHeader( 3, wxEmptyString );
	t4->setHeader( 4, wxT( "V-7" ));
	t4->setHeader( 5, _( "Own Vargas" ));
	t4->setHeader( 6, wxEmptyString );
	t4->setHeader( 7, wxT( "V-10" ));
	t4->setHeader( 8, _( "Own Vargas" ));
	t4->setHeader( 9, wxEmptyString );
	t4->setHeader( 10, wxT( "V-16" ));
	t4->setHeader( 11, _( "Own Vargas" ));
	t4->setHeader( 12, wxEmptyString );
	t4->setHeader( 13, _( "Average" ));
	t4->setHeader( 14, _( "Percent" ));
	line = 1;
	for ( i = 0; i < obsp.size(); i++ )
	{
		p = obsp[i];
		t4->setEntry( 0, line,  fmt.getObjectName( p, TF_MEDIUM ));
		planet = horoscope->getPlanet( p );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 0 ));
		t4->setEntry( 1,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 0 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 0, gv );
		t4->setEntry( 2,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 1 ));
		t4->setEntry( 4,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 1 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 1, gv );
		t4->setEntry( 5,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 2 ));
		t4->setEntry( 7,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 2 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 2, gv );
		t4->setEntry( 8,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getVimsopakaBala( 3 ));
		t4->setEntry( 10,  line, s );

		gv = planet->getVimsopakaBalaGoodVargas( 3 );
		s.Printf( wxT( "%d" ), gv );
		if ( gv > 1 ) s << wxT( " - " ) << lang.getVimsopakaDignityName( 3, gv );
		t4->setEntry( 11,  line, s );

		s.Printf( wxT( "%02.1f" ), planet->getAverageVimsopakaBala());
		t4->setEntry( 13,  line, s );

		s.Printf( wxT( "%02.1f%%" ), 5.0 * planet->getAverageVimsopakaBala());
		t4->setEntry( 14,  line, s );

		line++;
	}
	sheet->addItem( t4 );
}



