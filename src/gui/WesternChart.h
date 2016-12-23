/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/WesternChart.h
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

#ifndef WESTERNCHART_H_
#define WESTERNCHART_H_

#include "BasicWesternChart.h"

class Horoscope;
class WesternChartConfig;
class Writer;

/*************************************************//**
*
*  western representation of graphical chart
*
******************************************************/
class WesternChart : public BasicWesternChart
{
public:

	WesternChart( const ChartType&, ChartProperties*, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	~WesternChart();

	//virtual void OnDataChanged();

private:

	virtual void updateAspects();

  virtual void paintInternal( ChartConfig* );
	void setupAries();

	double getLongitude4FrameSegment( const int &i, const bool alignToHouses );

	virtual double getVisualObjectLongitude( const Horoscope*, const ObjectId& );

	int lastaspect;
	//bool transitsoutside;

	void drawPalettes();
	void drawSigns();
	void drawHouses();
	void drawNakshatras();
	void drawEastIndianChartCenter();
	void drawInlayImage();

	void drawAllPlanets( const Horoscope *h, const bool applyFilter = false );
	void drawAngleSymbol( const ObjectId &p );
	bool shouldBeMagnified( const ObjectId &p, const Horoscope *h );

	void drawAspects();
	void drawSingleAspect( const double &len1, const double &len2, const ASPECT_TYPE& );


};

#endif

