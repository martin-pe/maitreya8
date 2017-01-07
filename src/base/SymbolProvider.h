/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/SymbolProvider.h
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

#ifndef _SYMBOLPROVIDER_H_
#define _SYMBOLPROVIDER_H_

#include <wx/string.h>
#include "maitreya.h"

class WriterConfig;

/*************************************************//**
*
*
*
******************************************************/
class SymbolProvider
{
public:

	SymbolProvider( WriterConfig* = (WriterConfig*)NULL );

	wxChar getSignCode( const Rasi &id ) const;
	wxChar getPlanetCode( const ObjectId &id ) const;
	wxChar getRetroCode( const MOVING_DIRECTION& ) const;

	wxChar getAspectCode( const ASPECT_TYPE& ) const;

private:
	WriterConfig *cfg;

};

#endif

