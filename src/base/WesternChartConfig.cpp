/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/WesternChartConfig.cpp
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

#include "WesternChartConfig.h"

#include "Conf.h"
#include "GraphConfig.h"
#include "JSonTool.h"
#include "maitreya.h"

#include <wx/filename.h>
#include <wx/jsonreader.h>
#include <wx/jsonval.h>
#include <wx/jsonwriter.h>
#include <wx/log.h>
#include <wx/wfstream.h>

extern Config *config;

/*****************************************************
**
**   WesternChartBehaviorConfig   ---   Constructor
**
******************************************************/
WesternChartBehaviorConfig::WesternChartBehaviorConfig()
{
	zoomOnMouseOver = true;
	zoomFactor = 120;
	magnifyAspects = true;
}

/*****************************************************
**
**   WesternChartConfig   ---   Constructor
**
******************************************************/
WesternChartConfig::WesternChartConfig( wxString name )
	: ChartConfig()
{
	this->name = name;
	ChartConfig::setDefaultConfig();
	WesternChartConfig::setDefaultConfig();
}

/*****************************************************
**
**   WesternChartConfig   ---   setDefaultConfig
**
******************************************************/
void WesternChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	rPlanets.radius = 79;
	rPlanets.width = -6;

	signs.radius = 79;
	signs.width = 13;
	signs.wLabels = 7;
	signs.labelSize = 70;

	houses.radius = 50;
	houses.width = 29;
	houses.wLabels = 3;
	houses.labelSize = 70;

	aspects.radius = 50.0;

	nakshatras.show = false;
	nakshatras.wLabels = 7;
	nakshatras.labelSize = 70;

	eastIndianCenter.show = false;
	image.show = false;

	angles.radius = 50;
	angles.width = 47;
	angles.wLabels = 45;
	angles.showOuterFrame = angles.showInnerFrame = false;

	rInnerTransitPlanets.radius  = 50.0;
	rInnerTransitPlanets.width = 5;
	rOuterTransitPlanets.radius  = 96.0;
	rOuterTransitPlanets.width = 10;

	planetMarkerLength = 2;
	forcedZodiacStart = -1;
}

IMPLEMENT_SINGLETON( WesternChartConfigLoader )

/*****************************************************
**
**   WesternChartConfigLoader   ---   loadSingleConfig
**
******************************************************/
void WesternChartConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	JSonTool tool( globalResourcename );
	WesternChartConfig *cfg = (WesternChartConfig*)NULL;

	if ( ! v[ CFG_EXTENDS ].IsNull())
	{
		wxString baseconfig = v[ CFG_EXTENDS ].AsString();
		WesternChartConfig *basecfg = getByName( baseconfig );
		if ( basecfg ) cfg = new WesternChartConfig( *basecfg );
		else wxLogError( wxT( "could not find base config for extension %s - using default configuration for that entry" ), baseconfig.c_str());
		//printf( "Found Extend relation: %s pointer is %ld\n", str2char( baseconfig ), (long)basecfg );
	}

	if ( ! cfg ) cfg = new WesternChartConfig;
	tool.readChartConfig( v, cfg );

	if ( ! v[ CFG_FRAME_TYPE ].IsNull()) tool.readFrameType( v[ CFG_FRAME_TYPE ], cfg->frameType );
	if ( ! v[ CFG_RPLANETS ].IsNull()) tool.readObjectRing( v[ CFG_RPLANETS ], cfg->rPlanets );
	if ( ! v[ CFG_RINNERTRANSITPLANETS ].IsNull()) tool.readObjectRing( v[ CFG_RINNERTRANSITPLANETS ], cfg->rInnerTransitPlanets );
	if ( ! v[ CFG_ROUTERTRANSITPLANETS ].IsNull())  tool.readObjectRing( v[ CFG_ROUTERTRANSITPLANETS ], cfg->rOuterTransitPlanets );

	if ( ! v[ CFG_ASPECTS ].IsNull()) tool.readChartFrame( v[ CFG_ASPECTS ], cfg->aspects );
	if ( ! v[ CFG_EASTINDIANCENTER ].IsNull()) tool.readChartFrame( v[ CFG_EASTINDIANCENTER ], cfg->eastIndianCenter );
	if ( ! v[ CFG_IMAGE ].IsNull()) tool.readInlayImage( v[ CFG_IMAGE ], cfg->image );

	if ( ! v[ CFG_PLANET_MARKER_LENGTH ].IsNull()) cfg->planetMarkerLength = v[ CFG_PLANET_MARKER_LENGTH ].AsInt();
	if ( ! v[ CFG_ZODIACSTART ].IsNull()) cfg->forcedZodiacStart = v[ CFG_ZODIACSTART ].AsInt();

	tool.readChartRing( v[ CFG_SIGNS ], cfg->signs );
	tool.readChartRing( v[ CFG_NAKSHATRAS ], cfg->nakshatras );
	if ( ! cfg->signs.paletteName.IsEmpty())
	{
		GcChartFrameBgPattern palette;
		palette.innerFrame = GcChartFrame( cfg->signs );
		palette.outerFrame = GcChartFrame( cfg->signs ).magnify( cfg->signs.width );
		palette.paletteName = cfg->signs.paletteName;
		palette.alignToHouses = false;
		cfg->patterns.push_back( palette );
	}
	tool.readChartRing( v[ CFG_HOUSES ], cfg->houses );
	if ( ! cfg->houses.paletteName.IsEmpty())
	{
		GcChartFrameBgPattern palette;
		palette.innerFrame = GcChartFrame( cfg->houses );
		palette.outerFrame = GcChartFrame( cfg->houses ).magnify( cfg->houses.width );
		palette.paletteName = cfg->houses.paletteName;
		palette.alignToHouses = true;
		cfg->patterns.push_back( palette );
	}

	tool.readChartRing( v[ CFG_ANGLES ], cfg->angles );
	tool.readPatterns( v[ CFG_PATTERNS ], cfg->patterns );

	l.push_back( cfg );
}

/*****************************************************
**
**   WesternChartConfigLoader   ---   Constructor
**
******************************************************/
WesternChartConfigLoader::WesternChartConfigLoader()
: ConfigListLoader<WesternChartConfig>( CFG_WESTERNCHART, CrtCombinedDir )	
{
}

