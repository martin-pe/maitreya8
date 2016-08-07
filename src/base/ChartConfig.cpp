/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ChartConfig.cpp
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

#include "ChartConfig.h"
#include "ColorConfig.h"
#include "Conf.h"

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

	signs.symbolZoomFactor = 100;
	signs.rotateSymbols = false;
	signs.symbolType = 0;

	//pen = *wxBLACK_PEN;
	pen = wxPen( config->colors->fgColor );
	brush = MBrush( config->colors->bgColor, wxSOLID );

	useTextColors = true;
	useSignColors = false;
	useHouseColors = false;
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

