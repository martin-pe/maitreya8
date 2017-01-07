/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/FontProvider.h
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

#ifndef _FONTPROVIDER_H_
#define _FONTPROVIDER_H_

#include <wx/font.h>
#include <wx/string.h>
#include "ConfigBase.h"
#include "maitreya.h"

/*************************************************//**
*
* \brief encapsulates the needed parameters of a font
*
******************************************************/
class FontConfig : public ConfigBase
{
public:
	FontConfig();

	bool isOk() { return ok; }
	void setOk( const bool b = true ) { ok = b; }

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	int pointSize;
	int family;
	int style;
	int weight;
	int underline;
	wxString facename;

private:
	void init();

	bool ok;
};

/*************************************************//**
*
* \brief Provides access to fonts, singleton
*
******************************************************/
class FontProvider
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( FontProvider )
	
public:

	wxFont *getFont( const FONT_ID&, const int increase = 0 );
	wxFont *getFontBySize( const FONT_ID&, const int &pointSize );

	wxFont *getFontZoom( const FONT_ID&, const double &zoom );

	void setFont( const FONT_ID&, const wxFont& );

	wxFont *getFallbackFont( const FONT_ID& );

	wxString getSymbolFontName();

private:

	FontProvider();

	/**
	 * \brief system default font descriptions
	 */
	FontConfig defaultfd[MAX_FONTS];
};

#endif


