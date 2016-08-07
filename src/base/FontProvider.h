/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/FontProvider.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
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


