/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/WesternChartConfig.h
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

#ifndef _WESTERNCHARTCONFIG_H_
#define _WESTERNCHARTCONFIG_H_

#include "ChartConfig.h"
#include "ConfigBase.h"
#include "ConfigListLoader.h"
#include "maitreya.h"

#include <list>

class wxJSONValue;

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

	GcObjectRing rPlanets;
	GcChartRing houses, nakshatras;

	GcChartFrame aspects;
	GcChartFrame eastIndianCenter;
	GcInlayImage image;

	GcObjectRing rInnerTransitPlanets, rOuterTransitPlanets;
	GcChartRing angles;

	int planetMarkerLength;
	double forcedZodiacStart;

	std::list<GcChartFrameBgPattern> patterns;
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

