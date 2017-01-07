/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/SymbolProvider.cpp
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

#include "SymbolProvider.h"

#include "Conf.h"
#include "maitreya.h"

extern Config *config;

/*****************************************************
**
**   SymbolProvider   ---   Constructor
**
******************************************************/
SymbolProvider::SymbolProvider( WriterConfig *c )
{
	cfg = c ? c : config->writer;
}

/*****************************************************
**
**   SymbolProvider   ---   getSignCode
**
******************************************************/
wxChar SymbolProvider::getSignCode( const Rasi &id ) const
{
	if ( id == R_NONE || id == R_ERROR ) return SYMBOL_CODE_ERROR;
	ASSERT_VALID_RASI( id );

	static const wxChar k_sign_default[12] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L' };
	static const wxChar cp_alternative = 'V';

	if ( id == R_CAPRICORN && cfg->capricornSymbol ) return cp_alternative;
	else return k_sign_default[id];
}

/*****************************************************
**
**   SymbolProvider   ---   getPlanetCode
**
******************************************************/
wxChar SymbolProvider::getPlanetCode( const ObjectId &id ) const
{
	if ( id == ONONE || id == OERROR ) return SYMBOL_CODE_ERROR;
	assert( id >= 0 );
	if ( id == OARIES ) return 'A';
	if ( id >= MAX_EPHEM_OBJECTS ) return SYMBOL_CODE_ERROR;

	static const wxChar k_symbol[MAX_EPHEM_OBJECTS] =
	{
		// Sun ... Saturn
		'a', 'b', 'c', 'd', 'e', 'f', 'g', 

		// Uranus ... Pluto
		'h', 'i', 'j', 

		// Dragon head (mean and true)
		'T', 'T', 

		// Dragon tail (mean and true)
		'U', 'U', 

		'M', // Asc
		'N', // MC
		'O', // Desc
		'P', // IC

		// 8 Uranian
		'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 

		// Chiron, Pholus
		's', 't', 

		// Planetoids
		'u', 'v', 'w', 'x', 

		// Lilith
		'y' 

		// Earth
		//'z' 

	};
	static const wxChar uranus_alternative = 'W';
	static const wxChar pluto_alternative1 = 'X';
	static const wxChar pluto_alternative2 = 'Y';

	switch ( id )
	{
		case OURANUS:
			return cfg->uranusSymbol ? uranus_alternative :   k_symbol[id];
		break;
		case OPLUTO:
			if ( cfg->plutoSymbol == 2 ) return  pluto_alternative2;
			else if ( cfg->plutoSymbol == 1 ) return  pluto_alternative1;
			else return k_symbol[id];
		break;
		default:
		break;
	}
	return k_symbol[id];
}

/*****************************************************
**
**   SymbolProvider   ---   getRetroCode
**
******************************************************/
wxChar SymbolProvider::getRetroCode( const MOVING_DIRECTION &id ) const
{
	static const wxChar direct = 'Q';
	static const wxChar retro = 'R';
	
	//enum MOVING_DIRECTION { MD_NONE = -1, MD_DIRECT = 0, MD_RETROGRADE, MD_STATIONARY };

	//static const wxChar stationary = 'S';
	switch( id )
	{
		case MD_DIRECT:
			return direct;
		break;
		case MD_RETROGRADE:
			return retro;
		break;
		case MD_NONE:
		case MD_STATIONARY:
		default:
			return SYMBOL_CODE_ERROR;
		break;
	}
}

/*****************************************************
**
**   SymbolProvider   ---   getAspectCode
**
******************************************************/
wxChar SymbolProvider::getAspectCode( const ASPECT_TYPE &id ) const
{
	/*
enum ASPECT_TYPE { AT_CONJUNCTION = 0, AT_OPPOSITION, AT_TRINE, AT_SQUARE,
	AT_SEXTILE, AT_QUINCUNX, AT_SEMISQUARE, AT_SESQUISQUARE, AT_SEMISEXTILE,
	AT_QUINTILE, AT_BIQUINTILE, AT_PARALLELE, AT_CONTRAPARALLELE,
	AT_SEPTILE, AT_BISEPTILE, AT_TRISEPTILE, AT_NOVILE,
	AT_BINOVILE, AT_QUADNOVILE
};
	*/
	switch( id )
	{
		case AT_CONJUNCTION:
			return '0';
		break;
		case AT_OPPOSITION:
			return '1';
		break;
		case AT_TRINE:
			return '2';
		break;
		case AT_SQUARE:
			return '3';
		break;
		case AT_SEXTILE:
			return '4';
		break;
		case AT_QUINCUNX:
			return '6';
		break;
		case AT_SEMISQUARE:
			return '7';
		break;
		case AT_SESQUISQUARE:
			return '8';
		break;
		case AT_SEMISEXTILE:
			return '5';
		break;
		case AT_QUARTERSQUARE:
			return '9';
		break;
		case AT_QUAVER:
			return ':';
		break;
		case AT_SEMIQUAVER:
			return ';';
		break;
		case AT_PARALLELE:
			return '<';
		break;
		case AT_CONTRAPARALLELE:
			return '=';
		break;
		case AT_QUINTILE:
		case AT_BIQUINTILE:
		case AT_SEPTILE:
		case AT_BISEPTILE:
		case AT_TRISEPTILE:
		case AT_NOVILE:
		case AT_BINOVILE:
		case AT_QUADNOVILE:
			return 0;
		break;
		default:
			return SYMBOL_CODE_ERROR;
		break;
	}
}

