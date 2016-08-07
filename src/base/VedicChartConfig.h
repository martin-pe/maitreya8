/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VedicChartConfig.h
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

