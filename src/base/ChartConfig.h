/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ChartConfig.h
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

#ifndef _CHARTCONFIG_H_
#define _CHARTCONFIG_H_

#include "GraphConfig.h"

#include <wx/colour.h>
#include <wx/string.h>

class wxJSONValue;

/*************************************************//**
*
* \brief common class for chart configuration (western and Vedic)
*
******************************************************/
class ChartConfig 
{
public:
	ChartConfig();
	virtual ~ChartConfig() {}

	virtual void setDefaultConfig();

	wxColour getTextColor();

	GcChartRing signs;

	CHART_FRAME frameType;

	wxPen pen;
	MBrush brush;
	wxColour textColor;

	wxString name;

	wxColour &getPlanetColor( const int& );

};


#endif

