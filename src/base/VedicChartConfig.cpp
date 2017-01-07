/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/VedicChartConfig.cpp
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

#include "VedicChartConfig.h"

#include "ColorConfig.h"
#include "FileConfig.h"
#include "GraphConfig.h"
#include "JSonTool.h"
#include "maitreya.h"
#include "SheetConfig.h"

#include <wx/filename.h>
#include <wx/jsonwriter.h>
#include <wx/log.h>
#include <wx/wfstream.h>

/*****************************************************
**
**   VedicChartBehaviorConfig   ---   Constructor
**
******************************************************/
VedicChartBehaviorConfig::VedicChartBehaviorConfig()
{
	highlightOnMouseOver = false;
	zoomOnMouseOver = true;
	zoomFactor = 102;

	aspectedObjectsMode = 2;
	aspectingObjectsMode = 3;
	allAspectsMode = 4;

	// Graha Drishti
	aspectType = 0;
	grahaDrishtiMinimumStrength = VA_FULL;
	beneficMaleficDisplay = 0;
	includeAsMc = false;
};

/*****************************************************
**
**   VedicChartConfig   ---   Constructor
**
******************************************************/
VedicChartConfig::VedicChartConfig( wxString name )
		: ChartConfig()
{
	ChartConfig::setDefaultConfig();
	setDefaultConfig();
	this->name = name;
}

/*****************************************************
**
**   VedicChartConfig   ---   setDefaultConfig
**
******************************************************/
void VedicChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	useSymbols = true;
}

IMPLEMENT_SINGLETON( VedicChartConfigLoader )

/*****************************************************
**
**   VedicChartConfigLoader   ---   Constructor
**
******************************************************/
VedicChartConfigLoader::VedicChartConfigLoader()
: ConfigListLoader<VedicChartConfig>( CFG_VEDICCHART, CrtCombinedDir )	
{
}

/*****************************************************
**
**   VedicChartConfigLoader   ---   loadSingleConfig
**
******************************************************/
void VedicChartConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	JSonTool tool( globalResourcename );
	VedicChartConfig *cfg = (VedicChartConfig*)NULL;

	if ( ! v[ CFG_EXTENDS ].IsNull())
	{
		wxString baseconfig = v[ CFG_EXTENDS ].AsString();
		VedicChartConfig *basecfg = getByName( baseconfig );
		if ( basecfg ) cfg = new VedicChartConfig( *basecfg );
		else wxLogError(
			wxString::Format( wxT( "could not find base config for extension %s - using default configuration for that entry" ),
			baseconfig.c_str()));
		//printf( "Found Extend relation: %s pointer is %ld\n", str2char( baseconfig ), (long)basecfg);
	}
	if ( ! cfg ) cfg = new VedicChartConfig;

	cfg->name = v[ CFG_NAME ].AsString();

	if ( ! v[ CFG_PEN ].IsNull()) tool.readPen( v[CFG_PEN ], cfg->pen );
	if ( ! v[ CFG_BRUSH ].IsNull()) tool.readBrush( v[ CFG_BRUSH ], cfg->brush );
	if ( ! v[ CFG_TEXTCOLOR ].IsNull()) cfg->textColor = wxColour( v[ CFG_TEXTCOLOR ].AsString());
	if ( ! v[ CFG_OUTER_RECTANGLE ].IsNull()) tool.readRectangle( v[ CFG_OUTER_RECTANGLE ], cfg->outerRectangle );
	if ( ! v[ CFG_FRAME_TYPE ].IsNull()) tool.readFrameType( v[ CFG_FRAME_TYPE ], cfg->frameType );
	if ( ! v[ CFG_SIGNS ].IsNull()) tool.readChartRing( v[ CFG_SIGNS ], cfg->signs );
	if ( ! v[ CFG_MARKED_FIELDS ].IsNull()) tool.readRegion( v[ CFG_MARKED_FIELDS ], cfg->markedFields );

	if ( ! v[ CFG_PALETTE ].IsNull()) cfg->palette = v[ CFG_PALETTE ].AsString();

	l.push_back( cfg );
}


