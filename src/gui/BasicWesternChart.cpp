/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicWesternChart.cpp
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

#include "BasicWesternChart.h"

#include <math.h>
#include <wx/brush.h>
#include <wx/image.h>

#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "maitreya.h"
#include "GraphConfig.h"
#include "guibase.h"
#include "Horoscope.h"
#include "ImageProvider.h"
#include "mathbase.h"
#include "Lang.h"
#include "Painter.h"
#include "SymbolProvider.h"
#include "WesternChartConfig.h"

extern Config *config;

/*****************************************************
**
**   BasicWesternChart   ---   Constructor
**
******************************************************/
BasicWesternChart::BasicWesternChart( const ChartType &charttype, ChartProperties *chartprops, Horoscope *h1, Horoscope* h2 )
		: GraphicalChart( false, charttype, chartprops, h1, h2 )
{
	assert( chartprops );
	//printf( "BasicWesternChart::BasicWesternChart h1 %ld h2 %ld\n", (long)h1, (long)h2 );

	OBJECT_INCLUDES ss = chartprops->getWesternObjectStyle();
	ss |= OI_ALL_HOUSES;
	ss |= OI_4_HOUSES;
	setup();
}

/*****************************************************
**
**   BasicWesternChart   ---   Destructor
**
******************************************************/
BasicWesternChart::~BasicWesternChart()
{
}

/*****************************************************
**
**   BasicWesternChart   ---   getWChartConfig
**
******************************************************/
WesternChartConfig *BasicWesternChart::getWChartConfig() const
{
	return WesternChartConfigLoader::get()->getConfig( chartprops->getWesternSkin());
}

/*****************************************************
**
**   BasicWesternChart   ---   getChartConfig
**
******************************************************/
ChartConfig *BasicWesternChart::getChartConfig() const 
{
	return getWChartConfig();
}

/*****************************************************
**
**   BasicWesternChart   ---   setup
**
******************************************************/
void BasicWesternChart::setup()
{
	//printf( "BasicWesternChart::setup \n" );
	h1rect.clear();
	h2rect.clear();

/*
	for( uint i = 0; i < obs.size(); i++ )
	{
		h1rects.push_back( MRect());
		h2rects.push_back( MRect());
	}
	*/
	h1marked = h2marked = ONONE;

	// reduce radius if second chart has planets outside outerZodiac
	// transit style 0 = outside, 1 = inside
	//if ( transitmode && chartprops->getWesternChartDisplayConfig().secondchartStyle == 0 )
	if ( h2 && chartprops->getWesternChartDisplayConfig().secondchartStyle == 0 )
	{
		rmax *= .9;
		rxmax *= .9;
		rymax *= .9;
		transitsoutside =  true;
	}
	else
	{
		transitsoutside =  false;
	}

	// scalingfactor for house numbers
	// TODO
	//textzoom *= .6;

	// TODO
	psize = rmax/20;
}


/*****************************************************
**
**   BasicWesternChart   ---   OnResponsiveChartEvent
**
******************************************************/
bool BasicWesternChart::OnResponsiveChartEvent( const MPoint p, const int /* keyMod */ )
{
	const ObjectId old1_marked = h1marked;
	const ObjectId old2_marked = h2marked;

	h1marked = ONONE;
	h2marked = ONONE;

	//for( uint i = 0; i < obs.size() && i < h1rects.size() && i < h2rects.size(); i++ )
	for( uint i = 0; i < obs.size(); i++ )
	{
		if ( pointInRect( p, h1rect[obs[i]] ))
		{
			//printf( "MATCH RECT1 %d planet %d\n", i, obs[i] );
			h1marked = obs[i];
			return h1marked != old1_marked;
		}
		if ( pointInRect( p, h2rect[obs[i]] ))
		{
			//printf( "MATCH RECT2 %d planet %d\n", i, obs[i] );
			h2marked = obs[i];
			return h2marked != old2_marked;
		}
	}
	//printf( "OnResponsiveChartEvent RETURN %d\n", old1_marked != ONONE || old2_marked != ONONE );

	// this point can only be reached if there is no match in the chart. repaint is only required if one was marked before
	return old1_marked != ONONE || old2_marked != ONONE;
}

/*****************************************************
**
**   BasicWesternChart   ---   getPRadius
**
******************************************************/
double BasicWesternChart::getPRadius( const ObjectId &planet, const Horoscope *h )
{
	uint i;
	ObjectId p;
	const int factor = 1;
	WesternChartConfig *wconf = getWChartConfig();

	double ret = rmax * ( wconf->rPlanets.radius + wconf->rPlanets.width ) / 100;
	if ( h == h2 )
	{
		if ( chartprops->getWesternChartDisplayConfig().secondchartStyle > 0 )
			ret = rmax * ( wconf->rInnerTransitPlanets.radius + wconf->rInnerTransitPlanets.width ) / 100;
		else
			ret = rmax * ( wconf->rOuterTransitPlanets.radius + wconf->rOuterTransitPlanets.width ) / 100;
	}

	double len, dist;
	double mylen = getVisualObjectLongitude( h, planet );

	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		if ( p >= planet ) continue;
		if (( p == OASCENDANT || p == OMERIDIAN || p == ODESCENDANT || p == OIMUMCOELI ) && ( chartprops->getWesternChartDisplayConfig().showHouses )) continue;
		len = getVisualObjectLongitude( h, p );
		dist = ::fabs( mylen - len );
		if ( dist > 300 ) dist = fabs( dist - 360 );
		if ( dist < 5 )
		{
			if ( h == h2 ) ret -= Min( factor * psize, factor * psize * ( 5 - dist)/5 );
			else ret += Min( factor * psize, factor * psize * ( 5 - dist)/5 );
		}
	}
	return ret;
}

/*****************************************************
**
**   BasicWesternChart   ---   getCoordinatesForPlanet
**
******************************************************/
MPoint BasicWesternChart::getCoordinatesForPlanet( const ObjectId &planet, const Horoscope *h )
{
	double len = getVisualObjectLongitude( h, planet );
	double glen = red_deg( aries + len );
	double r = getPRadius( planet, h );
	return frameBorderPointToXy( glen, GcChartFrame( 100 * r / rmax ));
}

/*****************************************************
**
**   BasicWesternChart   ---   paintLengthMarker
**
******************************************************/
void BasicWesternChart::paintLengthMarker( const double &alfa, const GcChartFrame &frame )
{
	WesternChartConfig *wconf = getWChartConfig();

	//printf( "BasicWesternChart::paintLengthMarker radius %f pmlength %f\n", frame.radius, wconf->planetMarkerLength );
	paintMarkerLine( alfa, GcChartFrame( frame.radius - wconf->planetMarkerLength ),
		GcChartFrame( frame.radius + wconf->planetMarkerLength ));
}

