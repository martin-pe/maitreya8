/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicWesternChart.h
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

#ifndef BASICWESTERNCHART_H_
#define BASICWESTERNCHART_H_

#include "GraphicalChart.h"
#include <map>

class Horoscope;
class WesternChartConfig;
class Writer;

using namespace std;

/*************************************************//**
*
*  
*
******************************************************/
class BasicWesternChart : public GraphicalChart
{
public:

	BasicWesternChart( const ChartType&, ChartProperties*, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	~BasicWesternChart();

	virtual bool OnResponsiveChartEvent( const MPoint p = MPoint( -1, -1 ), const int keyMod = 0 );

protected:

	virtual void setup();

	virtual double getVisualObjectLongitude(  const Horoscope*, const ObjectId& ) = 0;

	virtual ChartConfig *getChartConfig() const;

	MPoint getCoordinatesForPlanet( const ObjectId &planet, const Horoscope *h );

	double getPRadius( const ObjectId &planet, const Horoscope *h );

	void paintLengthMarker( const double &alfa, const GcChartFrame& );

	GRADKREIS gradkreis;

	bool transitsoutside;
 	double psize, aries;


	Writer *writer;

	// marked objects
	//vector<MRect> h1rects, h2rects;
	map<ObjectId, MRect> h1rect, h2rect;
	ObjectId h1marked, h2marked;

	WesternChartConfig *getWChartConfig() const;
};

#endif

