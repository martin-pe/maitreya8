/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/GraphConfig.h
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

#ifndef _GRAPHCONFIG_H_
#define _GRAPHCONFIG_H_

#include "maitreya.h"

#include <wx/pen.h>
#include <wx/colour.h>

/*************************************************//**
*
*
*
******************************************************/
class GcRegion
{
public:
	GcRegion();

	MBrush brush;
	wxPen pen;
	wxColour textColor;

	bool show;
};

/*************************************************//**
*
*
*
******************************************************/
class GcChartFrame : public GcRegion
{
public:
	GcChartFrame( const double radius = 100.0 );

	double radius;
	CHART_FRAME frameType;
};

/*************************************************//**
*
*
*
******************************************************/
class GcRectangle : public GcRegion
{
public:
	GcRectangle();

	bool doubleOuterLine;
	int cornerRadius;
	double radius;
};

/*************************************************//**
*
*
*
******************************************************/
class GcArrow
{
public:
	GcArrow();

	GcChartFrame outerFrame;
	GcChartFrame innerFrame;

	wxPen pen;
};

/*************************************************//**
*
*
*
******************************************************/
class GcChartFrameBgPattern
{
public:

	GcChartFrameBgPattern()
	{
		alignToHouses = false;
	}

	GcChartFrameBgPattern( wxString frame0, wxString frame1, wxString palette, const bool &b )
	{
		innerFrameName = frame0;
		outerFrameName = frame1;
		paletteName = palette;
		alignToHouses = b;
	}

	wxString innerFrameName, outerFrameName;
	wxString paletteName;
	bool alignToHouses;
};

/*************************************************//**
*
*
*
******************************************************/
class GcZodiacalSigns
{
public:
	GcZodiacalSigns()
	{
		symbolZoomFactor = 300;
		rotateSymbols = false;
		symbolType = 0;
	}

	wxString paletteName;
	wxColour globalTextColor;

	int symbolZoomFactor;
  bool rotateSymbols;
	int symbolType;
};

/*************************************************//**
*
*
*
******************************************************/
class GcHouses
{
public:
	GcHouses() { paintHouseNumbers = true; }

	wxString paletteName;
	wxColour globalTextColor;

	bool paintHouseNumbers;
};

#endif

