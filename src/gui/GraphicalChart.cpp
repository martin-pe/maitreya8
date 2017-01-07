/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/GraphicalChart.cpp
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

#include "GraphicalChart.h"

#include "Aspect.h"
#include "ChartConfig.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "maitreya.h"
#include "Horoscope.h"
#include "ImageProvider.h"
#include "mathbase.h"
#include "Painter.h"
#include "PlanetList.h"

#include <wx/image.h>

extern Config *config;

/*****************************************************
**
**   GraphicalChart   ---   Constructor
**
******************************************************/
GraphicalChart::GraphicalChart( const bool &vedic, const ChartType &charttype, ChartProperties *chartprops, Horoscope *h1, Horoscope *h2 )
 : vedic( vedic ),
 charttype( charttype ),
 	chartprops( chartprops )
{
	valid = false;
	aexpert = new AspectExpert;

	markedField = -1;
	markedFieldStatus = 0;
	extrazoom = 1.0;
	setHoroscopes( h1, h2 );
}

/*****************************************************
**
**   GraphicalChart   ---   Destructor
**
******************************************************/
GraphicalChart::~GraphicalChart()
{
	delete aexpert;
}

/*****************************************************
**
**   GraphicalChart   ---   setHoroscopes
**
******************************************************/
void GraphicalChart::setHoroscopes( Horoscope *h1, Horoscope *h2  )
{
//enum ChartType { CT_RADIX, CT_TRANSIT, CT_PARTNER };
	this->h1 = h1;
	h1set = ( h1 != (Horoscope*)NULL );
	this->h2 = h2;
	h2set = ( h2 != (Horoscope*)NULL );

	// h1 must be set unless chart type is partner -> no, composite in partner view has type Radix (it is only one chart)
	//assert( h1set || charttype == CT_PARTNER );

	// h2 must be set for transit type
	assert( h2set || charttype == CT_RADIX || charttype == CT_PARTNER );

	switch( charttype )
	{
		case CT_RADIX:
			aexpert->setHoroscopes( h1, h1 );
		break;
		case CT_TRANSIT:
			aexpert->setHoroscopes( h2, h1 );
		break;
		case CT_PARTNER:
			aexpert->setHoroscopes( h1, h2 );
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   GraphicalChart   ---   OnDataChanged
**
******************************************************/
void GraphicalChart::OnDataChanged()
{
	//printf( "GraphicalChart::OnDataChanged\n" );
	valid = false;
	calculateObjectList();
	aexpert->clear();
	updateAspects();
}

/*****************************************************
**
**   GraphicalChart   ---   getPlanetRetro
**
******************************************************/
bool GraphicalChart::getPlanetRetro( const ObjectId &planet, const int &chart_id )
{
	Horoscope *h = getHoroscope( chart_id );
	if ( h ) return h->isRetrograde( planet );
	else return false;
}

/*****************************************************
**
*   GraphicalChart   ---   getHoroscope
**
******************************************************/
 Horoscope *GraphicalChart::getHoroscope( const int &chart_id )
{
	Horoscope *h = ( chart_id == 0 ? h1 : h2 );
	if ( ! h )
	{
		printf( "WARN: horoscope in GraphicalChart::getHoroscope is NULL\n" );
		return (Horoscope*)NULL;
	}
	return h;
}

/*****************************************************
**
*   GraphicalChart   ---   isPlanetBenefic
**
******************************************************/
bool GraphicalChart::isPlanetBenefic( const ObjectId &planet, const int &chart_id )
{
	Horoscope *h = getHoroscope( chart_id );
	if ( h ) return h->isBenefic( planet );
	else return false;
}

/*****************************************************
**
**   GraphicalChart   ---   calculateObjectList
**
******************************************************/
void GraphicalChart::calculateObjectList()
{
	const OBJECT_INCLUDES excludes = OI_ALL_HOUSES | OI_4_HOUSES; // | OI_ARIES;
  obs = vedic ? 
	chartprops->getVedicPlanetList( excludes ) : chartprops->getWesternPlanetList( excludes );
}

/*****************************************************
**
**   GraphicalChart   ---   paint
**
******************************************************/
void GraphicalChart::paint( Painter *painter, const MRect &r, const wxRect *rrect )
{
	rect = r;

	refreshRect = (wxRect*)rrect;

	//printf( "GraphicalChart::paint type %d vedic %d x %f y %f w %f h %f\n", (int)charttype,
		//chartprops->isVedic(), r.x, r.y, r.width, r.height);

	cfg =	getChartConfig();

	this->painter = painter;

	if ( ! valid )
	{
		rxmax = .5 * rect.width;
		rymax = .5 * rect.height;
		rmax = Min( rxmax, rymax );
		xcenter = rect.x  + rect.width / 2;
		ycenter = rect.y + rect.height / 2;

		painterzoom = painter->getTextZoomFactor( Min( rect.width, rect.height ));
		
		setup();
		valid = true;
	}

	// set pen and brush of config, otherwise default resources
	defaultPen = ( cfg->pen.IsOk() ? cfg->pen : painter->getDefaultPen());
  defaultBrush = ( cfg->brush.IsOk() ? cfg->brush : painter->getDefaultBrush() );

	// paint brackground
	//painter->setPen( defaultPen );
	painter->setTransparentPen();
	painter->setBrush( defaultBrush );
	painter->drawRectangle( rect );

	setGraphicFont();
	MPoint p = painter->getTextExtent( wxT( "A" ));
	text_width = p.real();
	text_height = p.imag();

	paintInternal( cfg );
}

/*****************************************************
**
**   GraphicalChart   ---   setGraphicFontZoom
**
******************************************************/
void GraphicalChart::setGraphicFontZoom( const double &zoom )
{
	//printf( "GraphicalChart::setGraphicFont %f %f %f\n", zoom, extrazoom, painterzoom );
	painter->setGraphicFontZoom( zoom * extrazoom * painterzoom );
}

/*****************************************************
**
**   GraphicalChart   ---   setSymbolFontZoom
**
******************************************************/
void GraphicalChart::setSymbolFontZoom( const double &zoom )
{
	//printf( "GraphicalChart::setSymbolFont  zoom %f extrazoom %f painterzoom %f\n",  zoom, extrazoom, painterzoom );
	painter->setSymbolFontZoom( zoom * extrazoom * painterzoom );
}

/*****************************************************
**
**   GraphicalChart   ---   setDefaultTextColor
**
******************************************************/
void GraphicalChart::setDefaultTextColor( const FIELD_PART part )
{
	wxColour color = painter->getDefaultTextColor();
	if ( part == FP_OUTER ) color = config->colors->transitFgColor;
	else if ( getChartConfig()->textColor.IsOk()) color = getChartConfig()->textColor;
	else if ( defaultPen.GetColour().IsOk() ) color = defaultPen.GetColour();
	painter->setTextColor( color );
	//painter->setTextColor( defaultPen.GetColour().IsOk() ?  defaultPen.GetColour() : painter->getDefaultTextColor());
}

/*****************************************************
**
**   GraphicalChart   ---   tryToSetPen
**
******************************************************/
void GraphicalChart::tryToSetPen( const wxPen &pen )
{
	if ( pen.IsOk() ) painter->setPen( pen );
	else painter->setPen( defaultPen );
}

/*****************************************************
**
**   GraphicalChart   ---   paintMarkerLine
**
******************************************************/
void GraphicalChart::paintMarkerLine( const double &alfa, const GcChartFrame &c1, const GcChartFrame &c2 )
{
	//printf( "GraphicalChart::paintMarkerLine frametype is %d c1 %d c2 %d\n", cfg->frameType, c1.frameType, c2.frameType );
	const MPoint p1 = frameBorderPointToXy( alfa, c1 );
	const MPoint p2 = frameBorderPointToXy( alfa, c2 );
	painter->drawLine( p1, p2 );
}

/*****************************************************
**
**   GraphicalChart   ---   paintObjectRing
**
******************************************************/
void GraphicalChart::paintObjectRing( const double &angle, const GcObjectRing &r )
{
	//printf( "GraphicalChart::paintObjectRing frametype is frame %d cfg  %d\n", r.frameType, cfg->frameType );
	if ( r.show )
	{
		tryToSetPen( r.pen );
		for ( int i = 0; i < 360; i++ )
		{
			if ( ! ( i % 10 )) paintMarkerLine( angle + i, r, r.magnify( r.wdegree10 ));
			else if ( ! ( i % 5 )) paintMarkerLine( angle + i, r, r.magnify( r.wdegree5 ));
			else paintMarkerLine( angle + i, r, r.magnify( r.wdegree1 ));
		}
		if ( r.showInnerFrame ) paintChartFrame( r );
		if ( r.show1DegreeFrame ) paintChartFrame( r.magnify( r.wdegree1 ));
		if ( r.show5DegreeFrame ) paintChartFrame( r.magnify( r.wdegree5 ));
		if ( r.show10DegreeFrame ) paintChartFrame( r.magnify( r.wdegree10 ));
		if ( r.showOuterFrame ) paintChartFrame( r.magnify( r.width ));
	}
}

/*****************************************************
**
**   GraphicalChart   ---   paintDegreeMarkers
**
******************************************************/
// TODO braucht man den noch
void GraphicalChart::paintDegreeMarkers( const double &aries,
	const GcChartFrame &zodiacFrame,
	const GcChartFrame &degree1Frame,
	const GcChartFrame &degree5Frame,
	const GcChartFrame &degree10Frame
	)
{
	for ( int i = 1; i < 360; i++ )
	{
		if ( ! ( i % 10 )) paintMarkerLine( aries + i, zodiacFrame, degree10Frame );
		else if ( ! ( i % 5 )) paintMarkerLine( aries + i, zodiacFrame, degree5Frame );
		else paintMarkerLine( aries + i, zodiacFrame, degree1Frame );
	}
}

/*****************************************************
**
**   GraphicalChart   ---   frameBorderPointToXy
**
******************************************************/
MPoint GraphicalChart::frameBorderPointToXy( const double &angle, const GcChartFrame &frame )
{
	const double alfa = red_deg( angle );
	const double a = alfa * PI / 180;
	double lambda;

	//printf( "GraphicalChart::frameBorderPointToXy frametype is frame %d cfg  %d\n", frame.frameType, cfg->frameType );

	CHART_FRAME type = ( frame.frameType != CF_INHERITED ) ? frame.frameType : cfg->frameType;
	switch( type )
	{
		case CF_CIRCLE:
		{
			const double radius = rmax * frame.radius / 100.0;
			return MPoint( xcenter + radius * cos( alfa * PI / 180 ), ycenter - radius * sin( alfa * PI / 180 ) );
			//return MPoint( xcenter, ycenter ) + polar( radius, a );
		}
		break;
		case CF_ELLIPSE:
		{
			const double xradius = rxmax * frame.radius / 100.0;
			const double yradius = rymax * frame.radius / 100.0;
			return MPoint( xcenter + xradius * cos( alfa * PI / 180 ), ycenter - yradius * sin( alfa * PI / 180 ) );
		}
		break;
		case CF_SQUARE:
		{
			const double radius = rmax * frame.radius / 100.0;
			if ( alfa > 315 || alfa <= 45 )
			{
				return MPoint( xcenter + radius, ycenter - SQRT2 * radius * sin( a ) );
			}
			else if ( alfa < 135 )
			{
				return MPoint( xcenter + SQRT2 * radius * cos( a ), ycenter - radius );
			}
			else if ( alfa < 225 )
			{
				return MPoint( xcenter - radius, ycenter - SQRT2 * radius * sin( a ) );
			}
			else // 225 < alfa < 315
			{
				return MPoint( xcenter + SQRT2 * radius * cos( a ), ycenter + radius );
			}
		}
		break;

		case CF_RECTANGLE:
		{
			const double xradius = rxmax * frame.radius / 100.0;
			const double yradius = rymax * frame.radius / 100.0;
			if ( alfa > 315 || alfa <= 45 )
			{
				return MPoint( xcenter + xradius, ycenter - SQRT2 * yradius * sin( a ) );
			}
			else if ( alfa < 135 )
			{
				return MPoint( xcenter + SQRT2 * xradius * cos( a ), ycenter - yradius );
			}
			else if ( alfa < 225 )
			{
				return MPoint( xcenter - xradius, ycenter - SQRT2 * yradius * sin( a ) );
			}
			else // 225 < alfa < 315
			{
				return MPoint( xcenter + SQRT2 * xradius * cos( a ), ycenter + yradius );
			}
		}
		break;

		case CF_TRIANGLE:
		{
			const double xradius = rxmax * frame.radius / 100.0;
			const double yradius = rymax * frame.radius / 100.0;

			const MPoint pt( xcenter, ycenter - yradius );
			const MPoint pl( xcenter - xradius / SQRT2, ycenter + yradius / SQRT2 );
			const MPoint pr( xcenter + xradius / SQRT2, ycenter + yradius / SQRT2 );

			if ( alfa > 330 || alfa < 90 )
			{
				lambda = red_deg( 90 - alfa ) / 120.0;
				return lambda * pr + ( 1 - lambda ) * pt;
			}
			else if ( alfa < 210 )
			{
				lambda = ( 210 - alfa ) / 120.0;
				return lambda * pt + ( 1 - lambda ) * pl;
			}
			else
			{
				lambda = red_deg( 330 - alfa ) / 120.0;
				return lambda * pl + ( 1 - lambda ) * pr;
			}
		}
		break;

		default:
			printf( "GraphicalChart::frameBorderPointToXy Invalid chart frame types %d/%d\n", cfg->frameType, frame.frameType );
			assert( false );
		break;
	}
}

/*****************************************************
**
**   GraphicalChart   ---   drawChartFrameField
**
******************************************************/
void GraphicalChart::drawChartFrameField( const double &alfa0, const double &alfa1, const GcChartFrame &frame0, const GcChartFrame &frame1 )
{ 
  int i;
	const double a0 = red_deg( alfa0 );
	const double a1 = red_deg( alfa1 );
	ASSERT_VALID_DEGREE( a0 );
	ASSERT_VALID_DEGREE( a1 );

	switch( cfg->frameType )
	{
		case CF_CIRCLE:
		case CF_ELLIPSE:
		{
			const int nbsteps = 6;
			const int ntotal =  2 * ( nbsteps + 1 );
			MPoint points[ntotal];
			double a = a0;
			const double astep = red_deg( a1 - a0 ) / nbsteps;

			for ( i = 0; i <= nbsteps; i++ )
			{
				points[i] = frameBorderPointToXy( a, frame0 );
				a += astep;
			}
			a = a1;
			for ( i = nbsteps + 1; i < ntotal; i++ )
			{
				points[i] = frameBorderPointToXy( a, frame1 );
				a -= astep;
			}
			painter->drawPolygon( ntotal, points );
		}
		break;

		case CF_SQUARE:
		case CF_RECTANGLE:
		{
			const int quadrant0 = (int)((a0 + 45 ) / 90 );
			const int quadrant1 = (int)((a1 + 45 ) / 90 );
			const bool qdrchange = (bool)(quadrant0 != quadrant1 );
			const double qdrcorner = 90 * (int)( Max( a0, a1 ) / 90 ) + 45;

			//const int ntotal =  qdrchange ? 6 : 4;
			const int ntotal =  6;
			MPoint points[ntotal];
			int i = 0;

			points[i++] = frameBorderPointToXy( a0, frame0 );
			if ( qdrchange ) points[i++] = frameBorderPointToXy( qdrcorner, frame0 );
			points[i++] = frameBorderPointToXy( a1, frame0 );
			points[i++] = frameBorderPointToXy( a1, frame1 );
			if ( qdrchange ) points[i++] = frameBorderPointToXy( qdrcorner, frame1 );
			points[i++] = frameBorderPointToXy( a0, frame1 );
			//painter->drawPolygon( ntotal, points );
			painter->drawPolygon( i, points );
		}
		break;
		case CF_TRIANGLE:
			printf( "TODO: Invalid chart frame type %d\n", cfg->frameType );
		break;
		default:
			printf( "GraphicalChart::drawChartFrameField Invalid chart frame type %d\n", cfg->frameType );
			assert( false );
		break;
	}

} 

/*****************************************************
**
**   GraphicalChart   ---   paintChartFrame
**
******************************************************/
void GraphicalChart::paintChartFrame( const GcChartFrame &frame )
{
  if ( ! frame.show ) return;

  if ( frame.pen.IsOk() )
	{
		painter->setPen( frame.pen );
	}
  else
	{
		painter->setPen( defaultPen );
	}

  if ( frame.brush.IsOk() ) painter->setBrush( frame.brush );
  else painter->setTransparentBrush();

	int type = ( frame.frameType != CF_INHERITED ) ? frame.frameType : cfg->frameType;
	switch( type )
	{
		case CF_CIRCLE:
		{
			const double radius = rmax * frame.radius / 100;
			painter->drawEllipse( xcenter - radius, ycenter - radius, 2 * radius, 2 * radius );
		}
		break;
		case CF_ELLIPSE:
		{
			const double xradius = rxmax * frame.radius / 100;
			const double yradius = rymax * frame.radius / 100;
			painter->drawEllipse( xcenter - xradius, ycenter - yradius, 2 * xradius, 2 * yradius );
		}
		break;
		case CF_SQUARE:
		{
			const double radius = rmax * frame.radius / 100;
			painter->drawRectangle( xcenter - radius, ycenter - radius, 2 * radius, 2 * radius );
		}
		break;
		case CF_RECTANGLE:
		{
			const double xradius = rxmax * frame.radius / 100;
			const double yradius = rymax * frame.radius / 100;
			painter->drawRectangle( xcenter - xradius, ycenter - yradius, 2 * xradius, 2 * yradius );
		}
		break;
		case CF_TRIANGLE:
		{
			MPoint points[3];
			points[0] = frameBorderPointToXy( 90, frame );
			points[1] = frameBorderPointToXy( 210, frame );
			points[2] = frameBorderPointToXy( 330, frame );
			painter->drawPolygon( 3, points );
			//const double radius = rmax * frame.radius / 100;
			//painter->drawEllipse( xcenter - radius, ycenter - radius, 2 * radius, 2 * radius );
		}
		break;
		default:
			printf( "GraphicalChart::paintChartFrame Invalid chart frame types %d/%d\n", cfg->frameType, frame.frameType );
			assert( false );
		break;
	}
}

/*****************************************************
**
**   GraphicalChart   ---   paintArrow
**
******************************************************/
void GraphicalChart::paintArrow( const MPoint &p, const MPoint &q )
{
	painter->drawLine( p, q  );

	// size of arrow, must be scaled fpr pdf
	const double size = rmax / 80.0;

	// length factor for the intersection point of arrow and line
	const double lambda = size / abs( q - p );

	// point on the line a root for the arrow
	const MPoint a =  lambda * p + ( 1 - lambda ) * q;

	const MPoint b = findOrthogonalPoint( a - q, size );

  MPoint mp[3] = { q, a + b, a - b };
	painter->drawPolygon( 3, mp );
}

/*****************************************************
**
**   GraphicalChart   ---   paintArrow
**
******************************************************/
void GraphicalChart::paintArrow( const double &a, GcChartRing &arrow )
{
	MPoint p = frameBorderPointToXy( a, arrow.magnify( arrow.width ));

	const wxPen pen = arrow.pen.IsOk() ? arrow.pen : defaultPen;
	painter->setPen( pen );
	painter->setBrush( pen.GetColour() );

	if ( arrow.radius == 0 )
	{
		// TODO methodeframeBorderPointToXy ersetzen
		paintArrow( frameBorderPointToXy( a + 180, arrow.magnify( arrow.width )), p );	
	}
	else
	{
		paintArrow( frameBorderPointToXy( a, arrow ), p );	
		paintMarkerLine( a+180, arrow.radius, arrow.radius + arrow.width );
	}
}

