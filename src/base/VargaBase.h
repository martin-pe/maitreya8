/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VargaBase.h
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

#ifndef _VARGABASE_H_
#define _VARGABASE_H_

#include <wx/string.h>
#include <vector>

#include "maitreya.h"

/*************************************************//**
*
* 
*
******************************************************/
class VargaConfigLoader
{
	DECLARE_SINGLETON( VargaConfigLoader )

public:

	void init();

	wxString getVargaName( const Varga& ) const;

	wxString getVargaSignification( const Varga& );

	int getVargaDivision( const Varga& ) const;

	Varga getVargaIndexByDivision( const int& ) const;

	Varga getVargaIndexByDivisionString( const wxString ) const;

	wxString getDefaultVargaSignification( const Varga& );

private:

	const static int vargaDivisions[NB_VARGAS];
};


#endif


