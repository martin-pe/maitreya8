/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/WesternChart.h
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

	virtual void OnDataChanged();

private:

	virtual void updateAspects();

  virtual void paintInternal( ChartConfig* );

	void paintPalettes();
	double getLongitude4FrameSegment( const int &i, const bool alignToHouses );

	virtual double getVisualObjectLongitude( const Horoscope*, const ObjectId& );

	int lastaspect;
	//bool transitsoutside;

	void paintZodiac();
	void paintAllPlanets( const Horoscope *h, const bool applyFilter = false );
	void paintAngleSymbol( const ObjectId &p );
	bool shouldBeMagnified( const ObjectId &p, const Horoscope *h );

	void paintAspects();
	void paintSingleAspect( const double &len1, const double &len2, const ASPECT_TYPE& );

	void paintHouses();

	void paintHouseBordersAndAngles();

};

#endif

