/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/WesternChartConfig.h
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

#ifndef _WESTERNCHARTCONFIG_H_
#define _WESTERNCHARTCONFIG_H_

#include "ChartConfig.h"
#include "ConfigBase.h"
#include "ConfigListLoader.h"
#include "maitreya.h"

#include <list>

class wxJSONValue;

using namespace std;

/*************************************************//**
*
* 
*
******************************************************/
class WesternChartBehaviorConfig : public ConfigBase
{
public:
	WesternChartBehaviorConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	bool zoomOnMouseOver;
	int zoomFactor;
	bool magnifyAspects;
};

/*************************************************//**
*
* \brief configuration for a single western chart
*
******************************************************/
class WesternChartConfig : public ChartConfig
{
public:

	WesternChartConfig( wxString name = wxEmptyString );

	virtual void setDefaultConfig();
	GcChartFrame &getFrameByName( wxString name );

	GcChartFrame zodiac;
	GcChartFrame zodiac1Degree;
	GcChartFrame zodiac5Degree;
	GcChartFrame zodiac10Degree;
	GcChartFrame zodiacSymbols;
	GcChartFrame outerZodiac;

	GcChartFrame innerHouse;
	GcChartFrame outerHouse;
	GcChartFrame houseNumbers;

	GcChartFrame planets;
	GcChartFrame aspects;

	GcChartFrame innerTransitZodiac;
	GcChartFrame innerTransitZodiac1Degree;
	GcChartFrame innerTransitZodiac5Degree;
	GcChartFrame innerTransitZodiac10Degree;
	GcChartFrame innerTransitPlanets;

	GcChartFrame outerTransitZodiac;
	GcChartFrame outerTransitZodiac1Degree;
	GcChartFrame outerTransitZodiac5Degree;
	GcChartFrame outerTransitZodiac10Degree;
	GcChartFrame outerTransitPlanets;

	int planetMarkerLength;

	list<GcChartFrameBgPattern> patterns;

	GcHouses houses;

	GcArrow angles;
};

/*************************************************//**
*
* \brief contains configs for western charts
*
******************************************************/
class WesternChartConfigLoader : public ConfigListLoader<WesternChartConfig>
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( WesternChartConfigLoader )

public:

	virtual void loadSingleConfig( wxJSONValue& );
	//void saveConfigs();

private:
	WesternChartConfigLoader();
};

#endif

