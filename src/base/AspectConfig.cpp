/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AspectConfig.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "AspectConfig.h"

#include "Conf.h"
#include "maitreya.h"

#include <wx/log.h>

extern Config *config;

IMPLEMENT_SINGLETON( AspectConfigLoader )

const static AspectConfig DefaultAspectConfigs[MAX_ASPECT_TYPES] =
{
	AspectConfig(  true, 5.0, wxPen( wxColour( 255, 200, 40 ), 10, wxSOLID )), // conj, orange
	AspectConfig(  true, 3.0, wxPen( wxColour( 0, 0, 255 ), 2, wxSOLID )), // opp, blue
	AspectConfig(  true, 3.0, wxPen( wxColour( 0, 255, 0 ), 2, wxSOLID )), // trine, green
	AspectConfig(  true, 3.0, wxPen( wxColour( 255, 0, 0 ), 2, wxSOLID )), // square, red
	AspectConfig(  true, 3.0, wxPen( wxColour( 0, 255, 0 ), 2, wxSOLID )), // sextile, green

	// Minor
	AspectConfig(  true, 1.0, wxPen( wxColour( 255, 0, 225 ), 1, wxSOLID )), // Quincunx, 5/12
	AspectConfig(  false, 1.0, wxPen( wxColour( 218, 89, 85 ), 1, wxSOLID )),     // 1/8
	AspectConfig(  false, 1.0, wxPen( wxColour( 218, 89, 85 ), 1, wxSOLID )),     // 3/8
	AspectConfig(  false, 1.0, wxPen( wxColour( 85, 218, 113 ), 1, wxSOLID )),  // 1/12
	AspectConfig(  false, 1.0, wxPen( wxColour( 85, 215, 218 ), 1, wxSOLID )), // 1/5
	AspectConfig(  false, 1.0, wxPen( wxColour( 85, 215, 218 ), 1, wxSOLID )), // 2/5

	// Declination
	AspectConfig(  false, 2.0, wxPen( wxColour( 133, 85, 218 ), 1, wxSOLID )), // parallele
	AspectConfig(  false, 2.0, wxPen( wxColour( 153, 218, 85 ), 1, wxSOLID )), // antiparallel

	// Ternary
	AspectConfig(  false, 1.0, wxPen( wxColour( 85, 189, 218 ), 1, wxSOLID )), // 1/7
	AspectConfig(  false, 1.0, wxPen( wxColour( 85, 189, 218 ), 1, wxSOLID )), // 2/7
	AspectConfig(  false, 1.0, wxPen( wxColour( 85, 189, 218 ), 1, wxSOLID )), // 3/7
	AspectConfig(  false, 1.0, wxPen( wxColour( 218, 85, 182 ), 1, wxSOLID )), // 1/9
	AspectConfig(  false, 1.0, wxPen( wxColour( 218, 85, 182 ), 1, wxSOLID )), // 2/9
	AspectConfig(  false, 1.0, wxPen( wxColour( 218, 85, 182 ), 1, wxSOLID )), // 4/9

	// Uranian
	AspectConfig(  false, 1.0, wxPen( wxColour( 0, 0, 0 ), 1, wxSOLID )), // 1/22.5
	AspectConfig(  false, 1.0, wxPen( wxColour( 0, 0, 0 ), 1, wxSOLID )), // 1/12.25
	AspectConfig(  false, 1.0, wxPen( wxColour( 0, 0, 0 ), 1, wxSOLID ))  // 1/5.625

};

/*****************************************************
**
**   AspectConfigLoader   ---   initAspects
**
******************************************************/
void AspectConfigLoader::initAspects()
{
	if ( config->aspects.size() != 0 && config->aspects.size() != MAX_ASPECT_TYPES )
	{
		wxLogWarning( wxT( "size of aspect configurations is %ld, expected %d" ), config->aspects.size(), MAX_ASPECT_TYPES );
		config->aspects.clear();
	}
	if ( config->aspects.size() == 0 )
	{
		config->aspects = getDefaultAspectConfigList();
	}
}

/*****************************************************
**
**   AspectConfigLoader   ---   getDefaultAspectConfigList
**
******************************************************/
vector<AspectConfig> AspectConfigLoader::getDefaultAspectConfigList()
{
	vector<AspectConfig> a;
	for ( uint i = 0; i < MAX_ASPECT_TYPES; i++ )
	{
		a.push_back( DefaultAspectConfigs[i] );
	}

	return a;
}

/*****************************************************
**
**   AspectConfig   ---   toString
**
******************************************************/
wxString AspectConfig::toString()
{
	wxString s;
	s << ( active ? wxT( "true" ) : wxT( "false" ))
	<< wxT( ":" ) << orbis
	<< wxT( ":" ) << pen.GetColour().GetAsString( wxC2S_HTML_SYNTAX )
	<< wxT( ":" ) << pen.GetWidth()
	<< wxT( ":" ) << pen.GetStyle();
	return s;
}

