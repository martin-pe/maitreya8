/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ChartConfig.cpp
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

#include "ChartConfig.h"
#include "ColorConfig.h"
#include "Conf.h"

#include <wx/jsonval.h>

extern Config *config;

/*****************************************************
**
**   ChartConfig   ---   Constructor
**
******************************************************/
ChartConfig::ChartConfig()
{
	name = wxT( "Default" );
}

/*****************************************************
**
**   ChartConfig   ---   setDefaultConfig
**
******************************************************/
void ChartConfig::setDefaultConfig()
{
	frameType = CF_CIRCLE;

	signs.labelSize = 100;
	signs.rotateLabels = false;
	signs.labelStyle = 0;

	//pen = *wxBLACK_PEN;
	pen = wxPen( config->colors->fgColor );
	brush = MBrush( config->colors->bgColor, wxSOLID );
}

/*****************************************************
**
**   ChartConfig   ---   getTextColor
**
******************************************************/
wxColour ChartConfig::getTextColor()
{
	if ( textColor.IsOk()) return textColor;
	return pen.GetColour();
}

/*****************************************************
**
**   ChartConfig   ---   getPlanetColor
**
******************************************************/
wxColour &ChartConfig::getPlanetColor( const int &i )
{
	// TODO
	return config->chartColors->getPlanetColor( i );
	/*
	if ( planetColor.size() == 0 ) return config->colors->>getPlanetColor( i );
	else return planetColor[i % planetColor.size()];
	*/
}

