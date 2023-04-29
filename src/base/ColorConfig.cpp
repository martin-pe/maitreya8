/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ColorConfig.cpp
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

#include "ColorConfig.h"

#include "Conf.h"
#include "ImageProvider.h"
#include "mathbase.h"

#include <wx/colour.h>
#include <wx/log.h>
#include <wx/tokenzr.h>
#include <wx/stopwatch.h>
#include <vector>

extern Config *config;

#define SIGN_RED wxT( "#E39696" )
#define SIGN_YELLOW wxT( "#ece5a6" )
#define SIGN_GREEN wxT( "#b0eca6" )
#define SIGN_BLUE wxT( "#a6cdec" )

#define DEFAULT_FG_COLOR wxT( "black" )
#define DEFAULT_BG_COLOR wxT( "white" )
//#define DEFAULT_TABLE_HEADER_BG_COLOR wxT( "#92BCD5" )
#define DEFAULT_SBC_MARKED_FIELDS_COLOR wxT( "#FDFE98" )
#define DEFAULT_SBC_BENEFIC_COLOR wxT( "green" )
#define DEFAULT_SBC_MALEFIC_COLOR wxT( "maroon" )
#define DEFAULT_TRANSIT_COLOR wxT( "blue" )
#define DEFAULT_ERROR_BG_COLOR wxT( "#c15757" )
#define DEFAULT_CHART_MARKUP_BG_COLOR wxT( "#c15757" )

IMPLEMENT_SINGLETON( PaletteManager )

/*****************************************************
**
**   BgPalette   ---   getBrush
**
******************************************************/
MBrush BgPalette::getBrush( const uint &i )
{
	if ( brushes.size() > 0 )
	{
		return brushes[ i % brushes.size()];
	}
	else
	{
		// TODO error message
		return MBrush( *wxRED );
	}
}

/*****************************************************
**
**   PaletteManager   ---   Constructor
**
******************************************************/
PaletteManager::PaletteManager()
{
	init();
}

/*****************************************************
**
**   PaletteManager   ---   init
**
******************************************************/
void PaletteManager::init()
{
	loadFallbackConfig();
}

/*****************************************************
**
**   PaletteManager   ---   hasPalette
**
******************************************************/
bool PaletteManager::hasPalette( wxString name )
{
	return m[name] != NULL;
}

/*****************************************************
**
**   PaletteManager   ---   getAllKeys
**
******************************************************/
std::vector<wxString> PaletteManager::getAllKeys()
{
	std::vector<wxString> v;
	for( std::map<wxString, BgPalette*>::iterator iter = m.begin(); iter != m.end(); iter++ )
	{
		v.push_back( iter->first );
	}
	return v;
}

/*****************************************************
**
**   PaletteManager   ---   setPalette
**
******************************************************/
void PaletteManager::setPalette( wxString name, const BgPalette &palette )
{
	if ( m[ name ] ) delete m[ name ];
	m[ name ] = new BgPalette( palette );
}

/*****************************************************
**
**   PaletteManager   ---   getPalette
**
******************************************************/
BgPalette *PaletteManager::getPalette( wxString name )
{
	/*
	printf( "PaletteManager::getPalette size %ld\n", m.size());
	const wxLongLong starttime = wxGetLocalTimeMillis();
	BgPalette *p = (BgPalette*)NULL;
	if ( name == wxT( "signstock" ))
	{
		p = &config->chartColors->signPalette;
	}
	if ( name == wxT( "housestock" ))
	{
		p = &config->chartColors->housePalette;
	}
	else p = m[name];
	printf( "PaletteManager::getPalette mark 1 %ld millisec\n", (wxGetLocalTimeMillis() - starttime).ToLong());
	return p;
	*/

	if ( name == wxT( "signstock" ))
	{
		return &config->chartColors->signPalette;
	}
	if ( name == wxT( "housestock" ))
	{
		return &config->chartColors->housePalette;
	}
	else return m[name];
}

/*****************************************************
**
**   PaletteManager   ---   loadFallbackConfig
**
******************************************************/
void PaletteManager::loadFallbackConfig()
{
	/*
	{
		BgPalette *p = new BgPalette;
		p->brushes.push_back( MBrush( wxColour( SIGN_RED )));
		p->brushes.push_back( MBrush( wxColour( SIGN_YELLOW )));
		p->brushes.push_back( MBrush( wxColour( SIGN_GREEN )));
		p->brushes.push_back( MBrush( wxColour( SIGN_BLUE )));
		m[ wxT( "signstock" ) ] = p;
	}
		
	{
		BgPalette *p = new BgPalette;
		p->brushes.push_back( MBrush( wxColour( SIGN_RED )));
		p->brushes.push_back( MBrush( wxColour( SIGN_YELLOW )));
		p->brushes.push_back( MBrush( wxColour( SIGN_GREEN )));
		p->brushes.push_back( MBrush( wxColour( SIGN_BLUE )));
		m[ wxT( "housestock" ) ] = p;
	}
	*/
		
	{
		BgPalette *p = new BgPalette;
		p->brushes.push_back( MBrush( wxColour( wxT( "#db545f" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#eda068" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#fbfc55" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#8dd83d" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#52c939" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#54ca8e" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#56c7f5" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#64aaf5" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#5686df" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#8084d6" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#f253b0" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#e1528f" ))));
		m[ wxT( "rainbow" ) ] = p;
	}

	{
		BgPalette *p = new BgPalette;
		p->brushes.push_back( MBrush( wxColour( wxT( "#de402b" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#eaaaa2" ))));
		m[ wxT( "redstyle" ) ] = p;
	}

	{
		BgPalette *p = new BgPalette;
		p->brushes.push_back( MBrush( wxColour( wxT( "#FE9F79" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#D1B38B" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#EFE58B" ))));
		p->brushes.push_back( MBrush( wxColour( wxT( "#AFDFE5" ))));
		m[ wxT( "desert" ) ] = p;
	}

	{
		wxString bmpname = wxT( "madera.jpg" );
		BgPalette *p = new BgPalette;
		p->brushes.push_back( MBrush( bmpname, 330 ));
		p->brushes.push_back( MBrush( bmpname, 15 ));
		p->brushes.push_back( MBrush( bmpname, 60 ));
		p->brushes.push_back( MBrush( bmpname, 150 ));
		m[ wxT( "madera" ) ] = p;
	}

	{
		wxString bmpname = wxT( "marble.jpg" );
		BgPalette *p = new BgPalette;
		p->brushes.push_back( MBrush( bmpname, 330 ));
		p->brushes.push_back( MBrush( bmpname, 15 ));
		p->brushes.push_back( MBrush( bmpname, 60 ));
		p->brushes.push_back( MBrush( bmpname, 150 ));
		m[ wxT( "marble" ) ] = p;
	}
}

/*****************************************************
**
**   BarDiagramConfig   ---   Constructor
**
******************************************************/
BarDiagramConfig::BarDiagramConfig()
{
	cornerRadius = 5;
	doItemZoom = true;
	itemZoom = 1;
	showToolbarMessages = true;
	//ImageProvider *ip = ImageProvider::get();;

	static const struct DasaListDescription
	{
		wxString name;
		unsigned char fgcolor[3];
		unsigned char bgcolor[3];
		int bgstyle;
		wxString bmpname;
	}
	configlist[] =
	{
		// Signs: Jataka Parijata I, 23; planets: II 19
		{  _( "Aries" ), { 255, 255, 255 }, { 255, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Taurus" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
		{  _( "Gemini" ), { 0, 0, 0 }, { 0, 204, 51 }, wxSOLID, wxEmptyString },
		{  _( "Cancer" ), { 0, 0, 0 }, { 255, 160, 255 }, wxSOLID, wxEmptyString },
		{  _( "Leo" ), { 0, 0, 0 }, { 226, 255, 211 }, wxSOLID, wxEmptyString },
		{  _( "Virgo" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSTIPPLE, wxT( "piebald.png" ) },
		{  _( "Libra" ), { 255, 255, 255 }, { 0, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Scorpio" ), { 0, 0, 0 }, { 229, 169, 104 }, wxSOLID, wxEmptyString },
		{  _( "Sagittarius" ), { 255, 255, 255 }, { 167, 123, 76 }, wxSOLID, wxEmptyString },
		{  _( "Capricorn" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSTIPPLE, wxT( "rainbow.png" ) },
		{  _( "Aquarius" ), { 255, 255, 255 }, { 104, 77, 47 }, wxSOLID, wxEmptyString },
		{  _( "Pisces" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
		{  _( "Sun" ), { 0, 0, 0 }, { 255, 94, 7 }, wxSOLID, wxEmptyString },
		{  _( "Moon" ), { 0, 0, 0 }, { 255, 255, 255 }, wxSOLID, wxEmptyString },
		{  _( "Mercury" ), { 0, 0, 0 }, { 102, 204, 102 }, wxSOLID, wxEmptyString },
		{  _( "Venus" ), { 0, 0, 0 }, { 255, 102, 255 }, wxSTIPPLE, wxT( "rainbow.png" ) },
		{  _( "Mars" ), { 255, 255, 255 }, { 204, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Jupiter" ), { 0, 0, 0 }, { 255, 255, 102 }, wxSOLID, wxEmptyString },
		{  _( "Saturn" ), { 255, 255, 255 }, { 0, 0, 0 }, wxSOLID, wxEmptyString },
		{  _( "Rahu" ), { 255, 255, 255 }, { 82, 109, 198 }, wxSOLID, wxEmptyString },
		{  _( "Ketu" ), { 0, 0, 0 }, { 132, 132, 132 }, wxSOLID, wxEmptyString },
		{  _( "Ascendant" ), { 0, 0, 0 }, { 132, 132, 132 }, wxSOLID, wxEmptyString }
	};
	for ( int i = 0; i < MAX_BARDIAGRAM_COLORS; i++ )
	{
		style[i].name = configlist[i].name;

		style[i].pen = wxPen( wxColour( configlist[i].fgcolor[0], configlist[i].fgcolor[1], configlist[i].fgcolor[2] ), 1, wxSOLID );

		if ( configlist[i].bgstyle != wxSTIPPLE )
		{
			style[i].brush = MBrush( wxColour( configlist[i].bgcolor[0], configlist[i].bgcolor[1], configlist[i].bgcolor[2] ), configlist[i].bgstyle );
		}
		else
		{
			style[i].brush = MBrush( configlist[i].bmpname );
		}
	}
}

/*****************************************************
**
**   BarDiagramConfig   ---   getDasaColorConfigId
**
******************************************************/
int BarDiagramConfig::getConfigId( const int &i, const bool &rasitype )
{
	int p;
	if ( rasitype )
	{
		assert( i >= 0 && i < 12 );
		return i;
	}
	else
	{
		if ( i >= OSUN && i <= OSATURN ) p = i;
		else if ( i == OMEANNODE || i == OTRUENODE || i == 8 ) p = 7;
		else if ( i == OMEANDESCNODE || i == OTRUEDESCNODE ) p = 8;
		else if ( i == OASCENDANT || i == 7 ) p = 9; // caution: Ascendant on 7 (AV) TODO
		else assert( false );
		return p + 12;
	}
}

/*****************************************************
**
**   BarDiagramStyle   ---   Constructor
**
******************************************************/
BarDiagramStyle::BarDiagramStyle()
{
	pen = *wxBLACK_PEN;
	brush = MBrush( *wxWHITE );
}

/*****************************************************
**
**   ColorConfig   ---   Constructor
**
******************************************************/
ColorConfig::ColorConfig()
{
	// Base Colors
	fgColor = wxColour( DEFAULT_FG_COLOR );
	bgColor = wxColour( DEFAULT_BG_COLOR );
	sbcMarkedFieldsBgColor = wxColour( DEFAULT_SBC_MARKED_FIELDS_COLOR );
	beneficFgColor = wxColour( DEFAULT_SBC_BENEFIC_COLOR );
	maleficFgColor = wxColour( DEFAULT_SBC_MALEFIC_COLOR );
	transitFgColor = wxColour( DEFAULT_TRANSIT_COLOR );
	errorBgColor = wxColour( DEFAULT_ERROR_BG_COLOR );
	chartMarkupBgColor = wxColour( DEFAULT_ERROR_BG_COLOR );

}

/*****************************************************
**
**   ObjectColorConfig   ---   getPlanetColor
**
******************************************************/
wxColour &ObjectColorConfig::getPlanetColor( const int &i )
{
	//assert( i >= 0 && i < MAX_EPHEM_OBJECTS );
	if( i >= 0 && i < MAX_EPHEM_OBJECTS ) return planetFgColor[i];
	else return config->colors->fgColor;
}

/*****************************************************
**
**   ObjectColorConfig   ---   getSignFgColor
**
******************************************************/
wxColour &ObjectColorConfig::getSignFgColor( const int &i )
{
	return signFgColor[i%4];
}

/*****************************************************
**
**   ObjectColorConfig   ---   getHouseFgColor
**
******************************************************/
wxColour &ObjectColorConfig::getHouseFgColor( const int &i )
{
	return houseFgColor[i%4];
}

/*****************************************************
**
**   ObjectColorConfig   ---   Constructor
**
******************************************************/
ObjectColorConfig::ObjectColorConfig()
{
	int i;
	const static wxChar *k_sign_house_fg_color[4]  = {
		wxT( "red" ), wxT( "yellow" ), wxT( "green" ), wxT( "blue" )
	};
	const static wxChar *k_planet_color[MAX_EPHEM_OBJECTS]  = {
		wxT( "red" ),    // Sun
		wxT( "blue" ),   // Moon
		wxT( "green" ),  // Merc
		wxT( "yellow" ), // Ven
		wxT( "red" ),    // Mars
		wxT( "red" ),    // Jup
		wxT( "yellow" ), // Sat
		wxT( "green" ),  // Uran
		wxT( "blue" ),   // Nep
		wxT( "blue" ),   // Plu
		wxT( "gray" ),   // Mean Node
		wxT( "gray" ),   // True Node
		wxT( "gray" ),   // Desc Mean Node
		wxT( "gray" ),   // Desc True Node
		wxT( "red" ),    // Ascendant
		wxT( "yellow" ), // MC
		wxT( "green" ), // Descendant
		wxT( "blue" ), // IC
		wxT( "yellow" ), // Cupido
		wxT( "navy" ),   // Hades
		wxT( "green" ),  // Zeus
		wxT( "navy" ),   // Kronos
		wxT( "red" ),    // Apollon
		wxT( "yellow" ), // Admetos
		wxT( "red" ),    // Vulkanus
		wxT( "blue" ),   // Poseidon
		wxT( "red" ),   // Chiron
		wxT( "red" ),   // Pholus
		wxT( "purple" ),   // Plan1
		wxT( "purple" ),   //
		wxT( "purple" ),   //
		wxT( "purple" ),   // Plan4
		wxT( "gray" )   // Lilith
	};
	for ( i = 0; i < 4; i++ )
	{
		signFgColor[i] = k_sign_house_fg_color[i];
		houseFgColor[i] = k_sign_house_fg_color[i];
	}
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) planetFgColor[i] = k_planet_color[i];

	signPalette.brushes.push_back( MBrush( wxColour( SIGN_RED )));
	signPalette.brushes.push_back( MBrush( wxColour( SIGN_YELLOW )));
	signPalette.brushes.push_back( MBrush( wxColour( SIGN_GREEN )));
	signPalette.brushes.push_back( MBrush( wxColour( SIGN_BLUE )));

	housePalette.brushes.push_back( MBrush( wxColour( SIGN_RED )));
	housePalette.brushes.push_back( MBrush( wxColour( SIGN_YELLOW )));
	housePalette.brushes.push_back( MBrush( wxColour( SIGN_GREEN )));
	housePalette.brushes.push_back( MBrush( wxColour( SIGN_BLUE )));
}

