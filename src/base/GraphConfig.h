/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/GraphConfig.h
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
	GcRegion() { show = true; }

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
	GcChartFrame( const double radius = 100.0, const CHART_FRAME fr = CF_INHERITED )
	 : GcRegion()
	{
		this->radius = radius;
		frameType = fr;
	}

	GcChartFrame magnify( const double &width ) const
	{
		GcChartFrame frame( *this );
		frame.radius = this->radius + width;
		return frame;
	}

	double radius;
	CHART_FRAME frameType;
};

/*************************************************//**
*
*
*
******************************************************/
class GcChartRing : public GcChartFrame
{
public:
	GcChartRing( const double radius = 100.0, const CHART_FRAME frame = CF_INHERITED )
	 : GcChartFrame( radius, frame )
	{
		width = 10;
		wLabels = 5;
		labelSize = 100;
		labelStyle = 0;
		showOuterFrame = showInnerFrame = showLabels = true;
		rotateLabels = false;
	}

	double width;
	double wLabels;

	// in percent
	int labelSize;

	// 0 = default, others depend
	int labelStyle;

	bool showOuterFrame;
	bool showLabels;
	bool rotateLabels;
	bool showInnerFrame;

	wxString paletteName;
	wxColour globalTextColor;
};

/*************************************************//**
*
*
*
******************************************************/
class GcObjectRing : public GcChartRing
{
public:

	GcObjectRing( const double radius = 100.0, const CHART_FRAME frameType = CF_INHERITED )
	 : GcChartRing( radius, frameType )
	{
		wdegree1 = 1;
		wdegree5 = 2;
		wdegree10 = 3;
		show1DegreeFrame = show5DegreeFrame = show10DegreeFrame = showOuterFrame = false;
	}

	double wdegree1;
	double wdegree5;
	double wdegree10;
	bool show1DegreeFrame;
	bool show5DegreeFrame;
	bool show10DegreeFrame;
};

/*************************************************//**
*
*
*
******************************************************/
class GcRectangle : public GcRegion
{
public:
	GcRectangle() : GcRegion()
	{
		doubleOuterLine = false;
		radius = 100.0;
		cornerRadius = 0;
	}

	bool doubleOuterLine;
	int cornerRadius;
	double radius;
};

/*************************************************//**
*
*
*
******************************************************/
class GcInlayImage : public GcChartFrame
{
public:
	GcInlayImage( const double radius = 100.0, const CHART_FRAME frame = CF_INHERITED )
	 : GcChartFrame( radius, frame )
	{
	}

	wxString filename;

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

	GcChartFrame innerFrame, outerFrame;
	wxString paletteName;
	bool alignToHouses;
};

#endif

