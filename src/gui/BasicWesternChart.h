/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicWesternChart.h
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

