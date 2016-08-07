/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/WesternChart.cpp
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

#include "WesternChart.h"

#include <math.h>
#include <wx/brush.h>
#include <wx/image.h>
#include <wx/log.h>

#include "ArabicParts.h"
#include "Aspect.h"
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
#include "Sheet.h"
#include "SymbolProvider.h"
#include "WesternChartConfig.h"

extern Config *config;

/*****************************************************
**
**   WesternChart   ---   Constructor
**
******************************************************/
WesternChart::WesternChart( const ChartType &charttype, ChartProperties *chartprops, Horoscope *h1, Horoscope* h2 )
		: BasicWesternChart( charttype, chartprops, h1, h2 )
{
	assert( chartprops );
}

/*****************************************************
**
**   WesternChart   ---   Destructor
**
******************************************************/
WesternChart::~WesternChart()
{
}

/*****************************************************
**
**   WesternChart   ---   OnDataChanged
**
******************************************************/
void WesternChart::OnDataChanged()
{

	GraphicalChart::OnDataChanged();

	/*
	* Note: if ! h1set: aries will be left (i.e. aries = 180
	*/
	switch( chartprops->getWesternChartDisplayConfig().chartOrientation )
	{
		// 1st house is left
		case 1:
			aries = h1 ? red_deg( 180 - h1->getHouse( HOUSE1, false ) ) : 180;
		break;

		// Meridian is top
		case 2:
			aries = h1 ? red_deg( 90 - h1->getWesternLongitude( OMERIDIAN )) : 180;
		break;

		// Aries 0
		case 3: aries = 180; break;

		// Aries 15
		case 4: aries = 165; break;

		// Cancer 0
		case 5: aries = 90; break;

		// Cancer 15
		case 6: aries = 75; break;

		// Libra 0
		case 7: aries = 0; break;

		// Libra 15
		case 8: aries = 345; break;

		// Capricorn 0
		case 9: aries = 270; break;

		// Capricorn 15
		case 10: aries = 255; break;

		// Ascendant left
		case 0:
		default:
			aries = h1 ? red_deg( 180 - h1->getWesternLongitude( OASCENDANT )) : 180;
		break;
	}
}

/*****************************************************
**
**   WesternChart   ---   updateAspects
**
******************************************************/
void WesternChart::updateAspects()
{
	assert( aexpert );
	aexpert->updateWesternAspects( chartprops, charttype );
	aexpert->setSortOrder( AS_TYPE_REV );
	aexpert->sort();
}

/*****************************************************
**
**   WesternChart   ---   getVisualObjectLongitude
**
******************************************************/
double WesternChart::getVisualObjectLongitude( const Horoscope *h, const ObjectId &p )
{
	assert( h );
	return h->getWesternLongitude( p );
}

/*****************************************************
**
**   WesternChart   ---   paintInternal
**
******************************************************/
void WesternChart::paintInternal( ChartConfig* )
{
	paintPalettes();

	if ( h1 )
	{
		if ( chartprops->getWesternChartDisplayConfig().showHouses ) paintHouses();
	}
	paintZodiac();
	if ( h1 )
	{
		if ( chartprops->getWesternChartDisplayConfig().showAspects ) paintAspects();
		if ( chartprops->getWesternChartDisplayConfig().showHouses ) paintHouseBordersAndAngles();
		paintAllPlanets( h1 );

		if ( h2 )
		{
			WesternChartConfig *wconf = getWChartConfig();
			const int tstyle = chartprops->getWesternChartDisplayConfig().transitStyle;
			paintChartFrame( tstyle > 0 ? wconf->innerTransitZodiac : wconf->outerTransitZodiac );
			paintDegreeMarkers( aries, tstyle > 0 ? wconf->innerTransitZodiac : wconf->outerTransitZodiac,
				tstyle > 0 ? wconf->innerTransitZodiac1Degree : wconf->outerTransitZodiac1Degree,
				tstyle > 0 ? wconf->innerTransitZodiac5Degree : wconf->outerTransitZodiac5Degree,
				tstyle > 0 ? wconf->innerTransitZodiac10Degree : wconf->outerTransitZodiac10Degree
			);
			paintAllPlanets( h2, charttype == CT_TRANSIT && chartprops->isFiltered() );
		}
	}
}

/*****************************************************
**
**   WesternChart   ---   getLongitude4FrameSegment
**
******************************************************/
double WesternChart::getLongitude4FrameSegment( const int &i, const bool alignToHouses )
{
	int i1 = i % 12;
	if ( alignToHouses )
	{
		return aries + h1->getHouse( i1, false );
	}
	else
	{
		return aries + i1 * 30;
	}
}

/*****************************************************
**
**   WesternChart   ---   paintPalettes
**
******************************************************/
void WesternChart::paintPalettes()
{
	//printf( "START WesternChart::paintPalettes\n" );
	WesternChartConfig *wconf = getWChartConfig();
	double d0, d1;
	int i;
	BgPalette *palette = (BgPalette*)NULL;
	PaletteManager *man = PaletteManager::get();

	for( list<GcChartFrameBgPattern>::iterator iter = wconf->patterns.begin(); iter != wconf->patterns.end(); iter++ )
	{
		/*
		GcChartFrame &frame0 = getFrameByName( iter->innerFrameName );
		GcChartFrame &frame1 = getFrameByName( iter->outerFrameName );
		*/
		GcChartFrame &frame0 = wconf->getFrameByName( iter->innerFrameName );
		GcChartFrame &frame1 = wconf->getFrameByName( iter->outerFrameName );
		palette = man->getPalette( iter->paletteName );

		if ( palette )
		{
			d0 = getLongitude4FrameSegment( 0, iter->alignToHouses );
			for ( i = 0; i < 12; i++ )
			{
				d1 = getLongitude4FrameSegment( i + 1, iter->alignToHouses );

				painter->setBrush( palette->getBrush( i ));
				drawChartFrameField( d0, d1, frame0, frame1 );
				d0 = d1;
			}
		}
		else
		{
			wxLogError( wxT( "palette '%s' in chart '%s' not available" ), iter->paletteName.c_str(), wconf->name.c_str() );
		}
	}
	//printf( "ENDE WesternChart::paintPalettes\n" );
}

/*****************************************************
**
**   WesternChart   ---   paintSingleAspect
**
******************************************************/
void WesternChart::paintSingleAspect( const double &len1, const double &len2, const ASPECT_TYPE &type )
{
	double glen1, glen2;
	wxString s;
	const double lpart1 = .2;
	const double lpart2 = .3;
	SymbolProvider sp;

	WesternChartConfig *wconf = getWChartConfig();

	glen1 = red_deg( aries + len1 );
	MPoint p1 = frameBorderPointToXy( glen1, wconf->aspects );

	glen2 = red_deg( aries + len2 );
	MPoint p2 = frameBorderPointToXy( glen2, wconf->aspects );

	if ( chartprops->getWesternChartDisplayConfig().showAspectSymbols && type != 0 )
	{
		painter->drawLine( p1, p1 + lpart1 * ( p2 - p1 ));
		painter->drawLine( p1 + lpart2 * ( p2 - p1 ), p2 );

		MRect rect( p1 + lpart1 * ( p2 - p1 ), ( lpart2 - lpart1 ) * ( p2 - p1 ));
		s = sp.getAspectCode( type );
		if ( s.IsEmpty())
		{
			setGraphicFont();
			painter->drawTextFormatted( rect, AspectExpert::getAspectShortDescription( type ), Align::Center );
		}
		else
		{
			setSymbolFont();
			painter->drawTextFormatted( rect, s, Align::Center );
		}
	}
	else
	{
		painter->drawLine( p1, p2 );
	}
	setGraphicFont();
}

/*****************************************************
**
**   WesternChart   ---   paintAspects
**
******************************************************/
void WesternChart::paintAspects()
{
	int zoom;
	assert( h1 );

	ObjectPosition pos1, pos2;
	list<AspectEvent>::iterator iter;
	WesternChartConfig *wconf = getWChartConfig();

	const int smallzoom = 1;
	const int bigzoom = 2;
	int p1marked = h1marked;
	int p2marked = h2marked;

	Horoscope *haspecting = (Horoscope*)NULL;
	Horoscope *haspected = (Horoscope*)NULL;
	switch( charttype )
	{
		case CT_RADIX:
			haspecting = haspected = h1;
		break;
		case CT_TRANSIT:
			haspecting = h2;
			haspected = h1;
		break;
		case CT_PARTNER:
			haspecting = h1;
			haspected = h2;
			p1marked = h2marked;
			p2marked = h1marked;
		break;
		default:
			assert( false );
		break;
	}

	lastaspect = -1;
	wxColour color;

	paintChartFrame( wconf->aspects );
	painter->setPen( defaultPen );
	painter->setTextColor( defaultPen.GetColour());

	painter->setPen( wxPen( *wxRED, wxSOLID, 5 ));

	list<AspectEvent> al = aexpert->getAspectList();
	//printf( "WesternChart::paintAspects al size %d\n", (int)al.size() );

	/*  shifted to the end of the section
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		paintLengthMarker( aries + haspecting->getWesternLongitude( (*iter).planet1 ), wconf->aspects.radius );
		paintLengthMarker( aries + haspected->getWesternLongitude( (*iter).planet2 ), wconf->aspects.radius );
	}
	*/

	color = config->colors->fgColor;
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		ASSERT_VALID_ASPECT( iter->aspectType );
		if ( chartprops->getWesternChartDisplayConfig().showAspectColors )
		{
			color = config->aspects[ iter->aspectType ].pen.GetColour();
		}
		//printf( "h1marked %d h2 marked %d P1 %d P2 %d\n", (int)h1marked, (int)h2marked, (int)iter->planet1, (int)iter->planet2 );
		
		zoom = smallzoom;
		if ( chartprops->getWesternChartBehaviorConfig().magnifyAspects )
		{
			switch( charttype )
			{
				case CT_RADIX:
					if ( p1marked == iter->planet1 || p1marked == iter->planet2 ) zoom = bigzoom;
				break;
				case CT_PARTNER:
				case CT_TRANSIT:
					if ( p1marked == iter->planet2 || p2marked == iter->planet1 ) zoom = bigzoom;
				break;
				default:
					assert( false );
				break;
			}
		}
		painter->setPen(
			wxPen( color,
			zoom * config->aspects[ iter->aspectType ].pen.GetWidth(),
			config->aspects[ iter->aspectType ].pen.GetStyle() ));
		painter->setTextColor( color );
		paintSingleAspect(
			haspecting->getWesternLongitude( iter->planet1 ),
			haspected->getWesternLongitude( iter->planet2 ),
			iter->aspectType );
	}
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		paintLengthMarker( aries + haspecting->getWesternLongitude( (*iter).planet1 ), wconf->aspects.radius );
		paintLengthMarker( aries + haspected->getWesternLongitude( (*iter).planet2 ), wconf->aspects.radius );
	}

}

/*****************************************************
**
**   WesternChart   ---   paintHouses
**
******************************************************/
void WesternChart::paintHouses()
{
	double len, houselen;
	MPoint p;
	Lang lang;
	WesternChartConfig *wconf = getWChartConfig();

	bool showColors = chartprops->getWesternChartDisplayConfig().showHouseColors;

	assert( h1 );
	//painter->setTransparentPen();

	painter->setPen( defaultPen );

	// 
	setGraphicFont( .8 );
	//setGraphicFont();

	paintChartFrame( wconf->innerHouse );

	// House numbers: leave them if transits are on inner house circle
	const int houseNumberStyle = chartprops->getWesternChartDisplayConfig().houseNumberStyle;
	bool paintHousenumbers = wconf->houses.paintHouseNumbers && houseNumberStyle;
	if ( paintHousenumbers )
	{
		if ( h2 && ! transitsoutside ) paintHousenumbers = false;
	}
	if ( paintHousenumbers )
	{
		wxColour textColor;
		if ( ! showColors )
		{
			setDefaultTextColor();
		}

		for ( int i = HOUSE1; i <= HOUSE12; i++ )
		{
			houselen = red_deg( h1->getHouse( i == HOUSE12 ? HOUSE1 : i + 1, false ) - h1->getHouse( i, false ));
			len = red_deg( aries + h1->getHouse( i, false ) + .5 * houselen );

			p = frameBorderPointToXy( len, wconf->houseNumbers );
			if ( showColors )
			{
				if ( wconf->houses.globalTextColor.IsOk() )
				{
					painter->setTextColor( wconf->houses.globalTextColor );
				}
				else
				{
					painter->setTextColor( config->chartColors->getHouseFgColor( i ));
				}
			}
			painter->drawTextFormatted( MRect( p.real() - psize/2, p.imag() - psize/2, psize, psize), getHouseNumberFormatted( i, houseNumberStyle ), Align::Center );
		}
	}
}

/*****************************************************
**
**   WesternChart   ---   paintHouseBordersAndAngles
**
******************************************************/
void WesternChart::paintHouseBordersAndAngles()
{
	double len;
	WesternChartConfig *wconf = getWChartConfig();

	painter->setPen( defaultPen );
	if ( ! transitsoutside )
	{
		// Ascendant and Descendant
		paintArrow( red_deg( aries + h1->getWesternLongitude( OASCENDANT )), wconf->angles );
		if ( chartprops->getWesternObjectStyle() & OI_ASCENDANT ) { paintAngleSymbol( OASCENDANT ); }
		if ( chartprops->getWesternObjectStyle() & OI_DESCENDANT ) { paintAngleSymbol( ODESCENDANT ); }

		// Meridian and Imum Coeli
		paintArrow( red_deg( aries + h1->getWesternLongitude( OMERIDIAN )), wconf->angles );
		if ( chartprops->getWesternObjectStyle() & OI_MERIDIAN ) { paintAngleSymbol( OMERIDIAN ); }
		if ( chartprops->getWesternObjectStyle() & OI_IMUMCOELI ) { paintAngleSymbol( OIMUMCOELI ); }
	}

	painter->setPen( defaultPen );
	for ( int i = HOUSE1; i <= HOUSE12; i++ )
	{
		//if ( ! transitsoutside && ( i == HOUSE1 || i == HOUSE4 || i == HOUSE7 || i == HOUSE10 )) continue;
		len = h1->getHouse( i, false );
		paintMarkerLine( aries + len, wconf->innerHouse, wconf->outerHouse );
	}
}

/*****************************************************
**
**   WesternChart   ---   paintAngleSymbol
**
******************************************************/
void WesternChart::paintAngleSymbol( const ObjectId &planet )
{
	const int degOffset = 3;
	WesternChartConfig *wconf = getWChartConfig();
	SymbolProvider sp;

	double len = red_deg( degOffset + aries + h1->getWesternLongitude( planet ));
	MPoint point = frameBorderPointToXy( len, wconf->angles.outerFrame );
	h1rect[planet] = MRect( point.real() - psize/2, point.imag() - psize/2, psize, psize);

	double zoom = 1.0;
	if ( shouldBeMagnified( planet, h1 ))
	//if (  h1marked == planet && chartprops->getWesternChartBehaviorConfig().zoomOnMouseOver )
	{
		zoom = .01 * chartprops->getWesternChartBehaviorConfig().zoomFactor;
	}
	setSymbolFont( zoom );
	//setGraphicFont( zoom );

	if ( chartprops->getWesternChartDisplayConfig().showPlanetColors ) painter->setTextColor( wconf->getPlanetColor( planet ) );
	painter->drawTextFormatted( h1rect[planet], sp.getPlanetCode( planet ), Align::Center );
	//painter->drawTextFormatted( h1rect[planet], code, Align::Center );
}

/*****************************************************
**
**   WesternChart   ---   shouldBeMagnified
**
******************************************************/
bool WesternChart::shouldBeMagnified( const ObjectId &p, const Horoscope *h )
{
	if ( ! chartprops->getWesternChartBehaviorConfig().zoomOnMouseOver ) return false;

	if ( h == h1 && h1marked == p ) return true;
	if ( h == h2 && h2marked == p ) return true;

	list<AspectEvent>::iterator iter;
	list<AspectEvent> al = aexpert->getAspectList();
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		switch( charttype )
		{
			case CT_RADIX:
				if ( iter->planet1 == h1marked && iter->planet2 == p ) return true;
				if ( iter->planet2 == h1marked && iter->planet1 == p ) return true;
			break;
			case CT_PARTNER:
			case CT_TRANSIT:
				if ( h == h1 )
				{
					if ( iter->planet1 == h2marked && iter->planet2 == p ) return true;
				}
				else if ( h == h2 )
				{
					if ( iter->planet2 == h1marked && iter->planet1 == p ) return true;
				}
			break;
			default:
				assert( false );
			break;
		}
	}

	return false;
}

/*****************************************************
**
**   WesternChart   ---   paintAllPlanets
**
******************************************************/
void WesternChart::paintAllPlanets( const Horoscope *h, const bool applyFilter )
{
	int align;
	ObjectId planet;
	Lang lang;
	SymbolProvider sp;
	double ff = 15;
	wxString psymbol;
	MPoint point;
	double zoom;
	SheetFormatter fmt;
	WesternChartConfig *wconf = getWChartConfig();

	painter->setPen( defaultPen );
	painter->setTextColor( defaultPen.GetColour());

	// decrease size for ppain text items eg arabic points
	const double text_zoom_addon = .7;

	/*
	ObjectId mp = ONONE;
	if ( h == h1 && h1marked != ONONE ) mp = h1marked;
	else if ( h == h2 && h2marked != ONONE ) mp = h2marked;
	//printf( "WesternChart::paintAllPlanets MARKED %d\n", (int)mp );
	*/

	ObjectArray o = applyFilter ? chartprops->getWesternPlanetList( chartprops->getObjectFilter()) : obs;

	for ( uint i = 0; i < o.size(); i++ )
	{
		setDefaultTextColor();
		planet = o[i];

		MRect &therect = ( h == h2 ? h2rect[planet] : h1rect[planet] );

		// loop for angles in simple radix horoscope
		if ( IS_ANGLE_OBJECT( planet ) && h2 == (Horoscope*)NULL ) continue;

		// loop for radix angles in transit horoscope depends on position of transit planets (outer/inner)
		if ( IS_ANGLE_OBJECT( planet ) && h == h1 && h2 && ! transitsoutside ) continue;

		// mark planet positions on zodiac
		GcChartFrame frame;
		if ( h == h2 )
		{
			if ( transitsoutside ) frame = wconf->outerTransitZodiac;
			else frame = wconf->innerTransitZodiac;
		}
		else
		{
			frame = wconf->zodiac;
		}

		paintLengthMarker( aries + getVisualObjectLongitude( h, planet ), frame );
		point = getCoordinatesForPlanet( planet, h );

		therect = MRect( point.real() - ff, point.imag() - ff, 2 * ff, 2 * ff);
		//zoom = planet == mp ? .01 * config->westernChartBehavior->zoomFactor : 1.0;

		zoom = 1.0;
		if ( shouldBeMagnified( planet, h ))
		{
			zoom = .01 * chartprops->getWesternChartBehaviorConfig().zoomFactor;
		}

		if ( chartprops->getWesternChartDisplayConfig().showPlanetColors )
		{
			painter->setTextColor( wconf->getPlanetColor( planet ));
		}

		psymbol = sp.getPlanetCode( planet );
		//if ( psymbol.IsEmpty() )
		if ( psymbol == SYMBOL_CODE_ERROR )
		{
			setGraphicFont( zoom * text_zoom_addon );
			align = Align::Center;
			//painter->drawTextFormatted( therect, writer->getObjectName( planet, TF_LONG ), align );
			//painter->drawTextFormatted( therect, fmt.getObjectName( planet, TF_LONG ), align );
			//MString objectname = fmt.getObjectName( planet, TF_LONG );
			MString objectname = fmt.getObjectName( planet, TF_MEDIUM );
			//painter->drawMString( therect, fmt.getObjectName( planet, TF_LONG ), align );
			painter->drawMString( therect, objectname, align );
		}
		else
		{
			setSymbolFont( zoom );
			painter->drawTextFormatted( therect, psymbol, Align::Center );
		}

		// Retrogression
		if (( chartprops->getWesternChartDisplayConfig().showRetro ) && h->isRetrograde( planet ))
		{
			setSymbolFont( zoom );

			align = Align::Right+Align::Bottom;
			painter->drawTextFormatted( MRect( point.real(), point.imag() + psize/2, psize, psize),
				//wxT( "_" ),
				SymbolProvider().getRetroCode( MD_RETROGRADE ),
				align );
		}
	}
	setGraphicFont();
}

/*****************************************************
**
**   WesternChart   ---   paintZodiac
**
******************************************************/
void WesternChart::paintZodiac()
{
	int i;
	double a;
	MPoint signpos;

	WesternChartConfig *wconf = getWChartConfig();
	GcZodiacalSigns *signs = &wconf->signs;

	painter->setPen( defaultPen );

	bool showColors = chartprops->getWesternChartDisplayConfig().showSignColors;

	// Sign background, if required
	a = aries;

	if ( ! showColors )
	{
		setDefaultTextColor();
	}

	//printf( "signs->symbolZoomFactor %d\n", signs->symbolZoomFactor );
	if ( signs->symbolType == 0 ) setSymbolFont( .01 * signs->symbolZoomFactor );
	//if ( signs->symbolType != 2 ) painter->setSymbolFont( .01 * signs->symbolZoomFactor );
	//printf( "WesternChart::paintZodiac zoom factor %d\n", signs->symbolZoomFactor );
	a = aries + 15;
	for ( i = 0; i < 12; i++ )
	{
		signpos = frameBorderPointToXy( a, wconf->zodiacSymbols );
		if ( chartprops->getWesternChartDisplayConfig().showSignColors )
		{
			if ( showColors )
			{
				if ( wconf->signs.globalTextColor.IsOk() )
				{
					painter->setTextColor( wconf->signs.globalTextColor );
				}
				else
				{
					painter->setTextColor( config->chartColors->getSignFgColor( i ));
				}
			}
		}
		painter->drawSignSymbol( signpos.real(), signpos.imag(), (Rasi)i,
			signs->symbolType, signs->symbolZoomFactor, signs->rotateSymbols ? a - 90 : 0 );
		a += 30.0;
	}

	paintChartFrame( wconf->zodiac );
	painter->setPen( defaultPen );
	a = aries;
	for ( i = 0; i < 12; i++ )
	{
		paintMarkerLine( a, wconf->zodiac, wconf->outerZodiac );
		a += 30.0;
	}

	paintDegreeMarkers( aries, wconf->zodiac, wconf->zodiac1Degree, wconf->zodiac5Degree, wconf->zodiac10Degree );
	paintChartFrame( wconf->zodiac1Degree );
	paintChartFrame( wconf->zodiac5Degree );
	paintChartFrame( wconf->zodiac10Degree );

	paintChartFrame( wconf->outerHouse );
	paintChartFrame( getWChartConfig()->outerZodiac );
}

/*****************************************************
**
**   ChartFactory   ---   createWesternChart
**
******************************************************/
GraphicalChart *ChartFactory::createWesternChart( const ChartType &charttype, ChartProperties *chartprops,
	Horoscope *h1, Horoscope *h2 )
{
	return new WesternChart( charttype, chartprops, h1, h2 );
}



