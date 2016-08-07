/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/WesternChartConfig.cpp
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

#include "WesternChartConfig.h"

#include "Conf.h"
#include "FileConfig.h"
#include "GraphConfig.h"
#include "JSonTool.h"
#include "maitreya.h"
#include "mathbase.h"
#include "SheetConfig.h"

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
	ChartConfig::setDefaultConfig();
	this->name = name;
	setDefaultConfig();
}

/*****************************************************
**
**   WesternChartConfig   ---   setDefaultConfig
**
******************************************************/
void WesternChartConfig::setDefaultConfig()
{
	ChartConfig::setDefaultConfig();

	zodiac.radius        = 79.0;

	zodiac1Degree.radius = 80.0;
	zodiac1Degree.show   = false;
	zodiac5Degree.radius = 81.0;
	zodiac5Degree.show   = false;
	zodiac10Degree.radius = 82;
	zodiac10Degree.show   = false;

	zodiacSymbols.radius = 86;
	zodiacSymbols.show   = false;
	outerZodiac.radius   = 92.0;

	innerHouse.radius = 50.0;
	outerHouse.radius = 79.0;
	houseNumbers.radius = 53.0;

	planets.radius = 73.0;
	aspects.radius = 50.0;

	innerTransitZodiac.radius  = 50.0;
	innerTransitZodiac1Degree.radius = 51.0;
	innerTransitZodiac5Degree.radius = 52.0;
	innerTransitZodiac10Degree.radius = 53.0;
	innerTransitPlanets.radius = 55.0;

	outerTransitZodiac.radius  = 96.0;
	outerTransitZodiac1Degree.radius = 98.0;
	outerTransitZodiac5Degree.radius = 100.0;
	outerTransitZodiac10Degree.radius = 102.0;
	outerTransitPlanets.radius = 106.0;

	planetMarkerLength = 2;
}
/*****************************************************
**
**   WesternChartConfig   ---   getFrameByName
**
******************************************************/
GcChartFrame &WesternChartConfig::getFrameByName( wxString fname )
{
  if ( ! fname.CmpNoCase( wxT( "zodiac" ))) return zodiac;
  else if ( ! fname.CmpNoCase( wxT( "zodiac1Degree" ))) return zodiac1Degree;
  else if ( ! fname.CmpNoCase( wxT( "zodiac5Degree" ))) return zodiac5Degree;
  else if ( ! fname.CmpNoCase( wxT( "zodiac10Degree" ))) return zodiac10Degree;
  else if ( ! fname.CmpNoCase( wxT( "zodiacSymbols" ))) return zodiacSymbols;
  else if ( ! fname.CmpNoCase( wxT( "outerZodiac" ))) return outerZodiac;

  else if ( ! fname.CmpNoCase( wxT( "innerHouse" ))) return innerHouse;
  else if ( ! fname.CmpNoCase( wxT( "outerHouse" ))) return outerHouse;
  else if ( ! fname.CmpNoCase( wxT( "houseNumbers" ))) return houseNumbers;

  else if ( ! fname.CmpNoCase( wxT( "planets" ))) return planets;
  else if ( ! fname.CmpNoCase( wxT( "aspects" ))) return aspects;

  else if ( ! fname.CmpNoCase( wxT( "innerTransitZodiac" ))) return innerTransitZodiac;
  else if ( ! fname.CmpNoCase( wxT( "innerTransitZodiac1Degree" ))) return innerTransitZodiac1Degree;
  else if ( ! fname.CmpNoCase( wxT( "innerTransitZodiac5Degree" ))) return innerTransitZodiac5Degree;
  else if ( ! fname.CmpNoCase( wxT( "innerTransitZodiac10Degree" ))) return innerTransitZodiac10Degree;
  else if ( ! fname.CmpNoCase( wxT( "innerTransitPlanets" ))) return innerTransitPlanets;

  else if ( ! fname.CmpNoCase( wxT( "outerTransitZodiac" ))) return outerTransitZodiac;
  else if ( ! fname.CmpNoCase( wxT( "outerTransitZodiac1Degree" ))) return outerTransitZodiac1Degree;
  else if ( ! fname.CmpNoCase( wxT( "outerTransitZodiac5Degree" ))) return outerTransitZodiac5Degree;
  else if ( ! fname.CmpNoCase( wxT( "outerTransitZodiac10Degree" ))) return outerTransitZodiac10Degree;
  else if ( ! fname.CmpNoCase( wxT( "outerTransitPlanets" ))) return outerTransitPlanets;

  wxLogError( wxT( "wrong frame reference '%s' in western chart config '%s'" ), fname.c_str(), name.c_str() );
  return zodiac;
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
		else wxLogError( wxString::Format( wxT( "could not find base config for extension %s - using default configuration for that entry" ), baseconfig.c_str()));
		printf( "Found Extend relation: %s pointer is %ld\n", str2char( baseconfig ), (long)basecfg);
	}

	if ( ! cfg ) cfg = new WesternChartConfig;
	cfg->name = v[ CFG_NAME ].AsString();

	if ( ! v[ CFG_PEN ].IsNull()) tool.readPen( v[CFG_PEN ], cfg->pen );
	if ( ! v[ CFG_BRUSH ].IsNull()) tool.readBrush( v[ CFG_BRUSH ], cfg->brush );

	if ( ! v[ CFG_FRAME_TYPE ].IsNull()) cfg->frameType = tool.readFrameType( v[ CFG_FRAME_TYPE ].AsString());
	tool.readChartFrame( v[ CFG_ZODIAC ], cfg->zodiac );
	tool.readChartFrame( v[ CFG_ZODIAC_1_DEGREE ], cfg->zodiac1Degree );
	tool.readChartFrame( v[ CFG_ZODIAC_5_DEGREE ], cfg->zodiac5Degree );
	tool.readChartFrame( v[ CFG_ZODIAC_10_DEGREE ], cfg->zodiac10Degree );
	tool.readChartFrame( v[ CFG_ZODIAC_SYMBOLS ], cfg->zodiacSymbols );
	tool.readChartFrame( v[ CFG_OUTER_ZODIAC ], cfg->outerZodiac );

	tool.readChartFrame( v[ CFG_INNER_HOUSE ], cfg->innerHouse );
	tool.readChartFrame( v[ CFG_OUTER_HOUSE ], cfg->outerHouse );
	tool.readChartFrame( v[ CFG_HOUSE_NUMBERS ], cfg->houseNumbers );

	tool.readChartFrame( v[ CFG_PLANETS ], cfg->planets );
	tool.readChartFrame( v[ CFG_ASPECTS ], cfg->aspects );

	tool.readChartFrame( v[ CFG_INNER_TRANSIT_ZODIAC ], cfg->innerTransitZodiac );
	tool.readChartFrame( v[ CFG_INNER_TRANSIT_ZODIAC_1_DEGREE ], cfg->innerTransitZodiac1Degree );
	tool.readChartFrame( v[ CFG_INNER_TRANSIT_ZODIAC_5_DEGREE ], cfg->innerTransitZodiac5Degree );
	tool.readChartFrame( v[ CFG_INNER_TRANSIT_ZODIAC_10_DEGREE ], cfg->innerTransitZodiac10Degree );
	tool.readChartFrame( v[ CFG_INNER_TRANSIT_PLANETS ], cfg->innerTransitPlanets );

	tool.readChartFrame( v[ CFG_OUTER_TRANSIT_ZODIAC ], cfg->outerTransitZodiac );
	tool.readChartFrame( v[ CFG_OUTER_TRANSIT_ZODIAC_1_DEGREE ], cfg->outerTransitZodiac1Degree );
	tool.readChartFrame( v[ CFG_OUTER_TRANSIT_ZODIAC_5_DEGREE ], cfg->outerTransitZodiac5Degree );
	tool.readChartFrame( v[ CFG_OUTER_TRANSIT_ZODIAC_10_DEGREE ], cfg->outerTransitZodiac10Degree );
	tool.readChartFrame( v[ CFG_OUTER_TRANSIT_PLANETS ], cfg->outerTransitPlanets );

	if ( ! v[ CFG_PLANET_MARKER_LENGTH ].IsNull()) cfg->planetMarkerLength = v[ CFG_PLANET_MARKER_LENGTH ].AsInt();

	tool.readZodiacalSigns( v[ CFG_ZODIACALSIGNS ], cfg->signs );
	if ( ! cfg->signs.paletteName.IsEmpty())
	{
		GcChartFrameBgPattern palette;
		palette.innerFrameName = wxT( "zodiac" );
		palette.outerFrameName = wxT( "outerZodiac" );
		palette.paletteName = cfg->signs.paletteName;
		palette.alignToHouses = false;
		cfg->patterns.push_back( palette );
	}
	tool.readHouses( v[ CFG_HOUSES ], cfg->houses );
	if ( ! cfg->houses.paletteName.IsEmpty())
	{
		GcChartFrameBgPattern palette;
		palette.innerFrameName = wxT( "innerHouse" );
		palette.outerFrameName = wxT( "outerHouse" );
		palette.paletteName = cfg->houses.paletteName;
		palette.alignToHouses = true;
		cfg->patterns.push_back( palette );
	}

	tool.readPatterns( v[ CFG_PATTERNS ], cfg->patterns );

	l.push_back( cfg );
}

/*****************************************************
**
**   WesternChartConfigLoader   ---   Constructor
**
******************************************************/
WesternChartConfigLoader::WesternChartConfigLoader()
: ConfigListLoader<WesternChartConfig>( CFG_WESTERNCHART, CrtGlobalFile )	
{
}

