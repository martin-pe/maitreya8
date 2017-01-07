/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VedicChartConfig.h
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

#ifndef _VEDICCHARTCONFIG_H_
#define _VEDICCHARTCONFIG_H_

#include "ChartConfig.h"
#include "ConfigBase.h"
#include "ConfigListLoader.h"
#include "maitreya.h"

/*************************************************//**
*
* 
*
******************************************************/
class VedicChartBehaviorConfig : public ConfigBase
{
public:
	VedicChartBehaviorConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	bool highlightOnMouseOver;
	bool zoomOnMouseOver;
	int zoomFactor;

	int aspectingObjectsMode;
	int aspectedObjectsMode;
	int allAspectsMode;
	int aspectType;
	VEDICASPECT_STRENGTH grahaDrishtiMinimumStrength;
	int beneficMaleficDisplay;
	bool includeAsMc;
};

/*************************************************//**
*
* 
*
******************************************************/
class VedicChartConfig : public ChartConfig
{
public:

	VedicChartConfig( wxString name = wxEmptyString );

	virtual void setDefaultConfig();

	GcRectangle outerRectangle;

	GcRegion markedFields;

	wxString palette;

	bool useSymbols;
};

/*************************************************//**
*
*
*
******************************************************/
class VedicChartConfigLoader : public ConfigListLoader<VedicChartConfig>
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( VedicChartConfigLoader )

public:

	virtual void loadSingleConfig( wxJSONValue& );
	//void saveConfigs();

private:
	VedicChartConfigLoader();
};

#endif

