/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/WesternChart.cpp
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
#include "Dasa.h"
#include "GenericDasa.h"
#include "GraphConfig.h"
#include "guibase.h"
#include "Horoscope.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "maitreya.h"
#include "mathbase.h"
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
/*
void WesternChart::OnDataChanged()
{
	GraphicalChart::OnDataChanged();
}
*/

/*****************************************************
**
**   WesternChart   ---   setupAries
**
******************************************************/
void WesternChart::setupAries()
{
	/*
	* Aries is the angle relative to the right side (y=0) of the chart.
	* direction is counter-clockwise - like the coordinate system in the plane.
	* Example: is 0 Leo is on top then aries will be at 330 deg.
	*
	* Note: if ! h1set: aries will be left (i.e. aries = 180)
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
		case 3: aries = 90; break;

		// Aries 15
		case 4: aries = 75; break;

		// Cancer 0
		case 5: aries = 0; break;

		// Cancer 15
		case 6: aries = 345; break;

		// Leo 0
		case 7: aries = 330; break;

		// Libra 0
		case 8: aries = 270; break;

		// Libra 15
		case 9: aries = 255; break;

		// Capricorn 0
		case 10: aries = 180; break;

		// Capricorn 15
		case 11: aries = 165; break;

		// Ascendant left
		case 0:
		default:
			aries = h1 ? red_deg( 180 - h1->getWesternLongitude( OASCENDANT )) : 180;
		break;
	}
	WesternChartConfig *wconf = getWChartConfig();
	//printf( "Zodiac start at %f\n", wconf->forcedZodiacStart );
	if ( wconf->forcedZodiacStart >= 0 && wconf->forcedZodiacStart < 360 ) aries = wconf->forcedZodiacStart;
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
	aexpert->setSortOrder( AS_TYPE_REVERSE );
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
	WesternChartConfig *wconf = getWChartConfig();
	setupAries();
	drawPalettes();

	if ( wconf->image.show == true && ! wconf->image.filename.IsEmpty()) drawInlayImage();

	if ( h1 )
	{
		//if ( chartprops->getWesternChartDisplayConfig().showHouses ) drawHouses();
	}
	if ( wconf->eastIndianCenter.show ) drawEastIndianChartCenter();
	if ( wconf->signs.show ) drawSigns();
	if ( wconf->nakshatras.show ) drawNakshatras();

	paintObjectRing( aries, wconf->rPlanets );
	if ( h1 )
	{
		if ( chartprops->getWesternChartDisplayConfig().showAspects ) drawAspects();
		if ( chartprops->getWesternChartDisplayConfig().showHouses ) drawHouses();
		drawAllPlanets( h1 );

		if ( h2 )
		{
			WesternChartConfig *wconf = getWChartConfig();
			const int tstyle = chartprops->getWesternChartDisplayConfig().secondchartStyle;
			paintObjectRing( aries, tstyle > 0 ? wconf->rInnerTransitPlanets : wconf->rOuterTransitPlanets );
			drawAllPlanets( h2, charttype == CT_TRANSIT && chartprops->isFiltered() );
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
**   WesternChart   ---   drawPalettes
**
******************************************************/
void WesternChart::drawPalettes()
{
	//printf( "START WesternChart::drawPalettes\n" );
	WesternChartConfig *wconf = getWChartConfig();
	double d0, d1;
	int i;
	BgPalette *palette = (BgPalette*)NULL;
	PaletteManager *man = PaletteManager::get();

	for( list<GcChartFrameBgPattern>::iterator iter = wconf->patterns.begin(); iter != wconf->patterns.end(); iter++ )
	{
		palette = man->getPalette( iter->paletteName );

		if ( palette )
		{
			d0 = getLongitude4FrameSegment( 0, iter->alignToHouses );
			for ( i = 0; i < 12; i++ )
			{
				d1 = getLongitude4FrameSegment( i + 1, iter->alignToHouses );

				painter->setBrush( palette->getBrush( i ));
				drawChartFrameField( d0, d1, iter->innerFrame, iter->outerFrame );
				d0 = d1;
			}
		}
		else
		{
			wxLogError( wxT( "palette '%s' in chart '%s' not available" ), iter->paletteName.c_str(), wconf->name.c_str() );
		}
	}
	//printf( "ENDE WesternChart::drawPalettes\n" );
}

/*****************************************************
**
**   WesternChart   ---   drawSingleAspect
**
******************************************************/
void WesternChart::drawSingleAspect( const double &len1, const double &len2, const ASPECT_TYPE &type )
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
**   WesternChart   ---   drawAspects
**
******************************************************/
void WesternChart::drawAspects()
{
	int zoom;
	assert( h1 );

	ObjectPosition pos1, pos2;
	list<AspectEvent>::iterator iter;
	WesternChartConfig *wconf = getWChartConfig();

	if ( ! wconf->aspects.show ) return;

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
		drawSingleAspect(
			haspecting->getWesternLongitude( iter->planet1 ),
			haspected->getWesternLongitude( iter->planet2 ),
			iter->aspectType );
	}
	tryToSetPen( wconf->rPlanets.pen );
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		paintLengthMarker( aries + haspecting->getWesternLongitude( (*iter).planet1 ), wconf->aspects.radius );
		paintLengthMarker( aries + haspected->getWesternLongitude( (*iter).planet2 ), wconf->aspects.radius );
	}
}

/*****************************************************
**
**   WesternChart   ---   drawHouses
**
******************************************************/
void WesternChart::drawHouses()
{
	double len, houselen;
	MPoint p;
	WesternChartConfig *wconf = getWChartConfig();
	assert( h1 );

	tryToSetPen( wconf->houses.pen );
	setGraphicFontZoom( .8 );
	paintChartFrame( wconf->houses );

	// House numbers: leave them if transits are on inner house circle
	const int houseNumberStyle = chartprops->getWesternChartDisplayConfig().houseNumberStyle;
	bool paintHousenumbers = wconf->houses.showLabels && houseNumberStyle;
	if ( paintHousenumbers )
	{
		if ( h2 && ! transitsoutside ) paintHousenumbers = false;
	}
	if ( paintHousenumbers )
	{
		for ( int i = HOUSE1; i <= HOUSE12; i++ )
		{
			if ( wconf->textColor.IsOk())
			{
				painter->setTextColor( wconf->textColor );
			}
			else if ( chartprops->getWesternChartDisplayConfig().showHouseColors )
			{
				painter->setTextColor( config->chartColors->getHouseFgColor( i ));
			}
			else painter->setTextColor( defaultPen.GetColour() );

			houselen = red_deg( h1->getHouse( i == HOUSE12 ? HOUSE1 : i + 1, false ) - h1->getHouse( i, false ));
			len = red_deg( aries + h1->getHouse( i, false ) + .5 * houselen );

			p = frameBorderPointToXy( len, GcChartFrame( wconf->houses.radius + wconf->houses.wLabels, wconf->houses.frameType ));
			painter->drawTextFormatted(
				MRect( p.real() - psize/2, p.imag() - psize/2, psize, psize),
				getHouseNumberFormatted( i, houseNumberStyle ),
				Align::Center );
		}
	}

	// house borders
	for ( int i = HOUSE1; i <= HOUSE12; i++ )
	{
		len = h1->getHouse( i, false );
		paintMarkerLine( aries + len, wconf->houses, GcChartFrame( wconf->houses.radius + wconf->houses.width, wconf->houses.frameType ));
	}
	paintChartFrame( wconf->houses.magnify( wconf->houses.width ));

	// arrows for angles
	tryToSetPen( wconf->angles.pen );
	if ( ! transitsoutside )
	{
		paintArrow( red_deg( aries + h1->getWesternLongitude( OASCENDANT )), wconf->angles );
		if ( chartprops->getWesternObjectStyle() & OI_ASCENDANT ) drawAngleSymbol( OASCENDANT );
		if ( chartprops->getWesternObjectStyle() & OI_DESCENDANT ) drawAngleSymbol( ODESCENDANT );

		paintArrow( red_deg( aries + h1->getWesternLongitude( OMERIDIAN )), wconf->angles );
		if ( chartprops->getWesternObjectStyle() & OI_MERIDIAN ) drawAngleSymbol( OMERIDIAN );
		if ( chartprops->getWesternObjectStyle() & OI_IMUMCOELI ) drawAngleSymbol( OIMUMCOELI );
	}
}

/*****************************************************
**
**   WesternChart   ---   drawAngleSymbol
**
******************************************************/
void WesternChart::drawAngleSymbol( const ObjectId &planet )
{
	const int degOffset = 3;
	WesternChartConfig *wconf = getWChartConfig();
	SymbolProvider sp;

	double len = red_deg( degOffset + aries + h1->getWesternLongitude( planet ));
	MPoint point = frameBorderPointToXy( len, wconf->angles.magnify( wconf->angles.wLabels ));
	//h1rect[planet] = MRect( point.real() - psize/2, point.imag() - psize/2, psize, psize);
	h1rect[planet] = MRect( point.real() - psize/2, point.imag() - psize/2, psize, psize);

	double zoom = 1.0;
	if ( shouldBeMagnified( planet, h1 ))
	//if (  h1marked == planet && chartprops->getWesternChartBehaviorConfig().zoomOnMouseOver )
	{
		zoom = .01 * chartprops->getWesternChartBehaviorConfig().zoomFactor;
	}
	setSymbolFontZoom( zoom );
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
				if ( h == h1 )
				{
					if ( iter->planet2 == h2marked && iter->planet1 == p ) return true;
				}
				else if ( h == h2 )
				{
					if ( iter->planet1 == h1marked && iter->planet2 == p ) return true;
				}
			break;
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
**   WesternChart   ---   drawAllPlanets
**
******************************************************/
void WesternChart::drawAllPlanets( const Horoscope *h, const bool applyFilter )
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

	const bool doSkipAries = h->getPlanetContext() == PcRadix || h->getPlanetContext() == PcTransit || h->getPlanetContext() == PcDirection;

	painter->setPen( defaultPen );
	painter->setTextColor( defaultPen.GetColour());

	// decrease size for ppain text items eg arabic points
	const double text_zoom_addon = .7;

	ObjectArray o = applyFilter ? chartprops->getWesternPlanetList( chartprops->getObjectFilter()) : obs;

	// mark planet positions on zodiac
	GcObjectRing frame;
	if ( h == h2 )
	{
		if ( transitsoutside ) frame = wconf->rOuterTransitPlanets;
		else frame = wconf->rInnerTransitPlanets;
	}
	else frame = wconf->rPlanets;
	//tryToSetPen( frame.pen );

	for ( uint i = 0; i < o.size(); i++ )
	{
		setDefaultTextColor();
		planet = o[i];
		//printf( "drawAllPlanets i %u planet %d\n", i, planet );

		if ( planet == OARIES && doSkipAries ) continue;

		MRect &therect = ( h == h2 ? h2rect[planet] : h1rect[planet] );

		// loop for angles in simple radix horoscope
		if ( IS_ANGLE_OBJECT( planet ) && h2 == (Horoscope*)NULL ) continue;

		// loop for radix angles in transit horoscope depends on position of transit planets (outer/inner)
		if ( IS_ANGLE_OBJECT( planet ) && h == h1 && h2 && ! transitsoutside ) continue;

		paintLengthMarker( aries + getVisualObjectLongitude( h, planet ), frame );
		point = getCoordinatesForPlanet( planet, h );

		therect = MRect( point.real() - ff, point.imag() - ff, 2 * ff, 2 * ff);
		//zoom = planet == mp ? .01 * config->westernChartBehavior->zoomFactor : 1.0;

		zoom = 1.0;
		if ( shouldBeMagnified( planet, h ))
		{
			zoom = .01 * chartprops->getWesternChartBehaviorConfig().zoomFactor;
		}

		if ( wconf->textColor.IsOk())
		{
			painter->setTextColor( wconf->textColor );
		}
		else if ( chartprops->getWesternChartDisplayConfig().showPlanetColors )
		{
			painter->setTextColor( wconf->getPlanetColor( planet ));
		}
		else painter->setTextColor( defaultPen.GetColour() );

		psymbol = sp.getPlanetCode( planet );
		//if ( psymbol.IsEmpty() )
		if ( psymbol == SYMBOL_CODE_ERROR )
		{
			setGraphicFontZoom( zoom * text_zoom_addon );
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
			setSymbolFontZoom( zoom );
			painter->drawTextFormatted( therect, psymbol, Align::Center );
		}

		// Retrogression
		if (( chartprops->getWesternChartDisplayConfig().showRetro ) && h->isRetrograde( planet ))
		{
			setSymbolFontZoom( zoom );

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
**   WesternChart   ---   drawInlayImage
**
******************************************************/
void WesternChart::drawInlayImage()
{
	ImageProvider *ip = ImageProvider::get();
	WesternChartConfig *wconf = getWChartConfig();

	if ( wconf->image.show && ! wconf->image.filename.IsEmpty())
	{
		//paintChartFrame( wconf->image );
		wxImage img = ip->getFileBasedBitmap( wconf->image.filename ).ConvertToImage();

		// not in wx 2.8
		//const wxSize size = img.GetSize();
		const wxSize size ( img.GetWidth(), img.GetHeight());

		const double height = rymax * wconf->image.radius / 50.0;
		const double xnew = size.x * height / size.y;
		img.Rescale( xnew, height );
		painter->drawBitmap( wxBitmap( img ), xcenter - .5 * xnew, ycenter - .5 * height, 0 );
	}
}

/*****************************************************
**
**   WesternChart   ---   drawNakshatras
**
******************************************************/
void WesternChart::drawNakshatras()
{
	int i;
	MPoint pos, p;
	wxString s;
	Lang lang;
	WesternChartConfig *wconf = getWChartConfig();
	GenericNakshatraDasaExpert *expert = (GenericNakshatraDasaExpert*)DasaExpertFactory().getDasaExpert( D_VIMSOTTARI );

	setGraphicFontZoom( .01 * wconf->nakshatras.labelSize );
	tryToSetPen( wconf->nakshatras.pen );
	/*
	painter->drawRotatedText( wxT( "planet" ), 50, 50, 45 );
	painter->drawRotatedText( wxT( "planet" ), 100, 50, 60 );
	painter->drawRotatedText( wxT( "planet" ), 200, 50, 90 );
	painter->drawRotatedText( wxT( "planet" ), 300, 50, 135 );
	painter->drawRotatedText( wxT( "planet" ), 400, 50, 180 );
	painter->drawRotatedText( wxT( "planet" ), 500, 50, 270 );
	painter->drawRotatedText( wxT( "planet" ), 600, 50, 0 );
	painter->drawRotatedText( wxT( "planet" ), 700, 50, 210 );
	static double aa = 0;
	painter->drawRotatedText( wxT( "planet" ), 100, 50, 5 * aa++ );
	*/

	if ( wconf->nakshatras.show )
	{
		paintChartFrame( wconf->nakshatras );
		paintChartFrame( wconf->nakshatras.magnify( wconf->nakshatras.width ));
		double a = aries;
		setDefaultTextColor();
		//setGraphicFont( .5 );

		for ( i = 0; i < 27; i++ )
		{
			// a is now at start of Nakshatra and will be shifted to the center then
			paintMarkerLine( a, wconf->nakshatras, wconf->nakshatras.magnify( wconf->nakshatras.width ));
			a += .5 * NAKSHATRA_LEN;

			pos = frameBorderPointToXy( a, wconf->nakshatras.magnify( wconf->nakshatras.wLabels ));
			s = lang.getNakshatra27Name( (NakshatraId_27)i, TF_MEDIUM );
			painter->drawRotatedText( s, pos.real(), pos.imag(), a - 90 );

			pos = frameBorderPointToXy( a, wconf->nakshatras.magnify( .5 * wconf->nakshatras.wLabels ));
			int dasaindex = expert->getDasaIndex4Nakshatra( (NakshatraId_27)i );
			int years = expert->getDasaDuration( dasaindex );

			//printf( "I %d dasaindex %d years %d\n", i, dasaindex, years );

			s.Clear();
			s <<  expert->getDasaLordName( expert->getDasaLordByIndex( dasaindex ), TF_LONG ) << SPACE << years;
			painter->drawRotatedText( s, pos.real(), pos.imag(), a - 90 );

			a += .5 * NAKSHATRA_LEN;
		}
	}
}

/*****************************************************
**
**   WesternChart   ---   drawEastIndianChartCenter
**
******************************************************/
void WesternChart::drawEastIndianChartCenter()
{
	WesternChartConfig *wconf = getWChartConfig();
	if ( wconf->eastIndianCenter.show )
	{
		tryToSetPen( wconf->eastIndianCenter.pen );

		GcChartFrame &frame = wconf->eastIndianCenter;
		const MPoint aries0 = frameBorderPointToXy( aries, frame );
		const MPoint cancer0 = frameBorderPointToXy( aries + 90, frame );
		const MPoint libra0 = frameBorderPointToXy( aries + 180, frame );
		const MPoint capricorn0 = frameBorderPointToXy( aries + 270, frame );

		painter->drawLine(
			frameBorderPointToXy( aries, frame ),
			frameBorderPointToXy( aries + 210, frame )
		);
		painter->drawLine(
			frameBorderPointToXy( aries + 30, frame ),
			frameBorderPointToXy( aries + 180, frame )
		);
		painter->drawLine(
			frameBorderPointToXy( aries + 90, frame ),
			frameBorderPointToXy( aries + 300, frame )
		);
		painter->drawLine(
			frameBorderPointToXy( aries + 120, frame ),
			frameBorderPointToXy( aries + 270, frame )
		);

		painter->drawLine(
			MPoint( aries0.real(), cancer0.imag() ),
			frameBorderPointToXy( aries + 330, frame )
		);
		painter->drawLine(
			MPoint( libra0.real(), cancer0.imag() ),
			frameBorderPointToXy( aries + 60, frame )
		);
		painter->drawLine(
			MPoint( libra0.real(), capricorn0.imag() ),
			frameBorderPointToXy( aries + 150, frame )
		);
		painter->drawLine(
			MPoint( aries0.real(), capricorn0.imag() ),
			frameBorderPointToXy( aries + 240, frame )
		);
	}
}

/*****************************************************
**
**   WesternChart   ---   drawSigns
**
******************************************************/
void WesternChart::drawSigns()
{
	Lang lang;
	wxString s;
	SymbolProvider sp;
	MPoint signpos;

	WesternChartConfig *wconf = getWChartConfig();
	GcChartRing &signs = wconf->signs;
	tryToSetPen( wconf->signs.pen );

	painter->setPen( defaultPen );
	//bool showColors = chartprops->getWesternChartDisplayConfig().showSignColors;
	//if ( ! showColors ) setDefaultTextColor();

	//printf( "signs->symbolZoomFactor %d\n", signs->symbolZoomFactor );
	if ( signs.labelStyle == 0 ) setSymbolFontZoom( .01 * signs.labelSize );
	else setGraphicFontZoom( .01 * signs.labelSize );
	//if ( signs->symbolType != 2 ) painter->setSymbolFont( .01 * signs->symbolZoomFactor );
	//printf( "WesternChart::paintZodiac zoom factor %d\n", signs->symbolZoomFactor );

	double a = aries + 15;
	for ( int i = 0; i < 12; i++ )
	{
		signpos = frameBorderPointToXy( a, signs.magnify( signs.wLabels ));
		if ( wconf->textColor.IsOk())
		{
			painter->setTextColor( wconf->textColor );
		}
		else if ( chartprops->getWesternChartDisplayConfig().showSignColors )
		{
			painter->setTextColor( signs.globalTextColor.IsOk() ? signs.globalTextColor : config->chartColors->getSignFgColor( i ));
		}
		else painter->setTextColor( defaultPen.GetColour() );
		
		/*
		if ( chartprops->getWesternChartDisplayConfig().showSignColors )
		{
			if ( showColors )
			{
				painter->setTextColor( signs.globalTextColor.IsOk() ? signs.globalTextColor : config->chartColors->getSignFgColor( i ));
			}
		}
		*/
		switch( signs.labelStyle )
		{
			case 1:
				s = lang.getSignName( (Rasi)i, TF_SHORT );
			break;
			case 2:
				s = lang.getSignName( (Rasi)i, TF_MEDIUM );
			break;
			case 3:
				s = lang.getSignName( (Rasi)i, TF_LONG );
			break;
			case 0:
			default:
				s = sp.getSignCode( (Rasi)i );
			break;
		}
		painter->drawRotatedText( s, signpos.real(), signpos.imag(), signs.rotateLabels ? a - 90 : 0 );
		a += 30.0;
	}

	if ( signs.showInnerFrame ) paintChartFrame( signs );

	// draw markers between signs if either inner or outer frame is shown, i.e. do not draw if both are hidden
	if ( signs.showInnerFrame || signs.showOuterFrame )
	{
		for ( int i = 0; i < 12; i++ ) paintMarkerLine( aries + i * 30, signs, signs.magnify( signs.width ));
	}
	if ( signs.showOuterFrame ) paintChartFrame( signs.magnify( signs.width ));
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



