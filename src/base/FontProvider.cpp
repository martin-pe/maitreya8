/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/FontProvider.cpp
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

#include "FontProvider.h"
#include "Conf.h"
#include "maitreya.h"

#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/tokenzr.h>
#include <wx/stopwatch.h>

extern Config *config;

#define MAX_FONT_SIZE 500

/**************************************************************
***
**   FontConfig   ---   Constructor
***
***************************************************************/
FontConfig::FontConfig()
{
	init();
}

/**************************************************************
***
**   FontConfig   ---   init
***
***************************************************************/
void FontConfig::init()
{
	pointSize = 10;
	family = wxFONTFAMILY_DEFAULT;
	style = wxFONTSTYLE_NORMAL;
	weight = wxFONTWEIGHT_NORMAL;
	underline = false;
	ok = false;
}

IMPLEMENT_SINGLETON( FontProvider )

/**************************************************************
***
**   FontProvider   ---   Constructor
***
***************************************************************/
FontProvider::FontProvider()
{

	// Symbol Fonts
	const int default_symbol_size = 12;
	defaultfd[FONT_GRAPHIC_SYMBOLS].facename = SYMBOL_FONT_NAME;
	defaultfd[FONT_GRAPHIC_SYMBOLS].pointSize = default_symbol_size;
	defaultfd[FONT_GRAPHIC_SYMBOLS].family = wxFONTFAMILY_DECORATIVE;

	defaultfd[FONT_GRAPHIC_DEFAULT].family = wxFONTFAMILY_SWISS;

	defaultfd[FONT_TEXT_HEADER].weight = wxFONTWEIGHT_BOLD;
	defaultfd[FONT_TEXT_HEADER].pointSize = 12;
	defaultfd[FONT_TEXT_TABLE_HEADER].weight = wxFONTWEIGHT_BOLD;

	// same for PDF
	defaultfd[FONT_PDF_HEADER].weight = wxFONTWEIGHT_BOLD;
	defaultfd[FONT_PDF_HEADER].pointSize = 10;
	defaultfd[FONT_PDF_TABLE_HEADER].weight = wxFONTWEIGHT_BOLD;

	// plain text with fixed width
	defaultfd[FONT_PLAIN_DEFAULT].family = wxFONTFAMILY_MODERN;

	// pdf tiny font for header and footer of the document
	defaultfd[FONT_PDF_TINY].pointSize = 8;

/*
#if defined(__WXMAC__)
	defaultfd[FONT_PDF_DEFAULT].facename = wxT( "Times New Roman" );
	defaultfd[FONT_PDF_HEADER].facename = wxT( "Times New Roman" );
	defaultfd[FONT_PDF_TABLE_HEADER].facename = wxT( "Times New Roman" );
	defaultfd[FONT_PDF_TABLE_CONTENTS].facename = wxT( "Times New Roman" );
	defaultfd[FONT_GRAPHIC_DEFAULT].facename = wxT( "Arial" );
#endif
*/
	
	for( int i = 0; i < MAX_FONTS; i++ )
	{
		if ( ! config->font[i].isOk() )
		{
			config->font[i] = defaultfd[i];
			config->font[i].setOk( true );
		}
	}
}

/**************************************************************
***
**   FontProvider   ---   getFallbackFont
***
***************************************************************/
wxFont *FontProvider::getFallbackFont( const FONT_ID &id )
{
	ASSERT_VALID_FONT_ID( id )
	return wxTheFontList->FindOrCreateFont( defaultfd[id].pointSize, defaultfd[id].family,
		defaultfd[id].style, defaultfd[id].weight, defaultfd[id].underline, defaultfd[id].facename
	);
}

/**************************************************************
***
**   FontProvider   ---   setFont
***
***************************************************************/
void FontProvider::setFont( const FONT_ID &id, const wxFont &f )
{
	ASSERT_VALID_FONT_ID( id )
	config->font[id].pointSize = f.GetPointSize();
	config->font[id].family = f.GetFamily();
	config->font[id].style = f.GetStyle();
	config->font[id].weight = f.GetWeight();
	config->font[id].facename = f.GetFaceName();
	config->font[id].setOk();
	//printf( "FontProvider::setFont id %d font %s\n", (int)id, str2char( fd[id].toString()));
}

/**************************************************************
***
**   FontProvider   ---   getSymbolFontName
***
***************************************************************/
wxString FontProvider::getSymbolFontName()
{
	return SYMBOL_FONT_NAME;
}

/**************************************************************
***
**   FontProvider   ---   setFontZoom
***
***************************************************************/
wxFont *FontProvider::getFontZoom( const FONT_ID &id, const double &zoom )
{
	ASSERT_VALID_FONT_ID( id )
	//printf( "FontProvider::getFontZoom id %d zoom %f\n", id, zoom );
  //const wxLongLong starttime = wxGetLocalTimeMillis();

	if ( ! config->font[id].isOk() )
	{
		printf( "FontProvider::getFont setting default font description for font #%d\n", id );
		config->font[id] = defaultfd[id];
	}

	int size = zoom == 1.0 ? config->font[id].pointSize : (int)( zoom * (double)config->font[id].pointSize);
	//printf( "Font Size is %d, zoom was %f\n", size, zoom );
	if ( size <= 0 || size > MAX_FONT_SIZE )
	{
		printf( "ERROR FontProvider::getFontZoom; invalid font size %d for id %d, zoom was %f, setting default\n",
			size, (int)id, zoom );
		size = 10;
	}

	/*
	wxFont *font = wxTheFontList->FindOrCreateFont( size, config->font[id].family, config->font[id].style,
		config->font[id].weight, config->font[id].underline, config->font[id].facename );
	printf( "FontProvider::getFontZoom mark 1 %ld millisec\n", (wxGetLocalTimeMillis() - starttime).ToLong());
	return font;
	*/

	return wxTheFontList->FindOrCreateFont( size, config->font[id].family, config->font[id].style,
		config->font[id].weight, config->font[id].underline, config->font[id].facename );
}

/**************************************************************
***
**   FontProvider   ---   getFontBySize
***
***************************************************************/
wxFont *FontProvider::getFontBySize( const FONT_ID &id, const int &pointSize )
{
	ASSERT_VALID_FONT_ID( id )

	if ( ! config->font[id].isOk() )
	{
		printf( "FontProvider::getFont setting default font description for font #%d\n", id );
		config->font[id] = defaultfd[id];
	}

	if ( pointSize <= 0 || pointSize > MAX_FONT_SIZE )
	{
		printf( "ERROR FontProvider::getFont; invalid font size %d for id %d, setting default\n",
			pointSize, (int)id );
		return getFont( id );
	}

	//printf( "FontProvider::getFont %d %s\n", (int)id, str2char( fd[id].toString()));
	return wxTheFontList->FindOrCreateFont( pointSize, config->font[id].family,
		config->font[id].style, config->font[id].weight, config->font[id].underline, config->font[id].facename );
}

/**************************************************************
***
**   FontProvider   ---   getFont
***
***************************************************************/
wxFont *FontProvider::getFont( const FONT_ID &id, const int increase )
{
	ASSERT_VALID_FONT_ID( id )

	if ( ! config->font[id].isOk() )
	{
		printf( "FontProvider::getFont setting default font description for font #%d\n", id );
		config->font[id] = defaultfd[id];
	}

	int size = config->font[id].pointSize + increase;
	if ( size <= 0 || size > MAX_FONT_SIZE )
	{
		printf( "ERROR FontProvider::getFont; invalid font size %d for id %d, increase was %d, setting default\n",
			size, (int)id, increase );
		size = 10;
	}

	//printf( "FontProvider::getFont %d %s\n", (int)id, str2char( fd[id].toString()));
	return wxTheFontList->FindOrCreateFont( config->font[id].pointSize + increase, config->font[id].family,
		config->font[id].style, config->font[id].weight, config->font[id].underline, config->font[id].facename );
}

