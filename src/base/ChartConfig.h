/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ChartConfig.h
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

#ifndef _CHARTCONFIG_H_
#define _CHARTCONFIG_H_

#include "GraphConfig.h"

#include <wx/colour.h>
#include <wx/string.h>

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

	GcZodiacalSigns signs;

	CHART_FRAME frameType;

	wxPen pen;
	MBrush brush;
	wxColour textColor;

	// TODO ggf ableiten von einer Basisklasse
	wxString name;
	wxString description;

	bool useTextColors;
	bool useHouseColors;
	bool useSignColors;

	wxColour &getPlanetColor( const int& );

};


#endif

