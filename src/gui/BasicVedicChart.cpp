/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicVedicChart.cpp
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

#include "BasicVedicChart.h"

#include "Aspect.h"
#include "astrobase.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "Sheet.h"
#include "SymbolProvider.h"
#include "Varga.h"
#include "VedicChartConfig.h"

#include <math.h>
#include <wx/log.h>

extern Config *config;

/*****************************************************
**
**   BasicVedicChart   ---   Constructor
**
******************************************************/
BasicVedicChart::BasicVedicChart( const ChartType &charttype, ChartProperties *chartprops, const uint &field_count,
	Horoscope *h1, Horoscope *h2 )
		: GraphicalChart( true, charttype, chartprops, h1, h2 ),
		field_count( field_count )
{
	assert( chartprops );
	assert( field_count == 12 || field_count == 28 );

	positionOffset = 0;
}

/*****************************************************
**
**   BasicVedicChart   ---   getVChartConfig
**
******************************************************/
VedicChartConfig *BasicVedicChart::getVChartConfig() const
{
	return VedicChartConfigLoader::get()->getConfig( chartprops->getVedicSkin());
}

/*****************************************************
**
**   BasicVedicChart   ---   getChartConfig
**
******************************************************/
ChartConfig *BasicVedicChart::getChartConfig() const
{
	return getVChartConfig();
}

/*****************************************************
**
**   BasicVedicChart   ---   getFieldForScreenPos
**
******************************************************/
uint BasicVedicChart::getFieldForScreenPos( const MPoint &p )
{
	//printf( "BasicVedicChart::getFieldForScreenPos %f %f count %d\n", p.real(), p.imag(), field_count );
	for( uint i = 0; i < field_count && i < fields.size(); i++ )
	{
		//printf( "BasicVedicChart::getFieldForScreenPos %f %f RECT %s\n", p.x, p.y, str2char( fields[i].rect.toString()) );
		if ( pointInRect( p, fields[i].rect ))
		{
			const double x = ( p.real() - fields[i].rect.x ) / fields[i].rect.width;
			const double y = ( p.imag() - fields[i].rect.y ) / fields[i].rect.height;

			switch( fields[i].fieldtype )
			{
				case FIELD_TYPE_FULL: // all fields in south indian chart, sbc, fixed rasis in east indian chart
					return i;
				break;

				case FIELD_TYPE_NE: // east: taurus and sag
					if ( x >= y ) return i;
				break;

				case FIELD_TYPE_SW: // east: gemini and scorpio
					if ( x <= y ) return i;
				break;

				case FIELD_TYPE_NW: // east: leo and pisces
					if ( 1 - x >= y ) return i;
				break;

				case FIELD_TYPE_SE: // east: virgo and aquarius
					if ( 1 - x <= y ) return i;
				break;

				case FIELD_TYPE_N: // north: houses 2 and 12
					if ( fabs( x - .5 ) <= .5 * ( 1 - y )) return i;
				break;

				case FIELD_TYPE_W: // north: houses 3 and 5
					if ( fabs( y - .5 ) <= .5 * ( 1 - x )) return i;
				break;

				case FIELD_TYPE_S: // north: houses 6 and 8
					if ( fabs( x - .5 ) <= .5 * y ) return i;
				break;

				case FIELD_TYPE_E: // north: houses 9 and 11
					if ( fabs( y - .5 ) <= .5 * x ) return i;
				break;

				case FIELD_TYPE_DIAMOND:// north: houses 1: 4: 7: 10
					if ( .5 - fabs( x - .5 ) >= fabs( y - .5 )) return i;
				break;

				default:
					printf( "BasicVedicChart::getFieldForScreenPos WARN: invalid field type %d at postion %u\n", fields[i].fieldtype, i );
				break;
			}
		}
	}
	//return UINT_FOR_NOT_FOUND;
	return UINT_FOR_OUTSIDE;
}

/*****************************************************
**
**   BasicVedicChart   ---   isFieldZoomed
**
******************************************************/
bool BasicVedicChart::isFieldZoomed( const uint &i )
{
	return ( markedField == i && markedFieldStatus & CHART_FIELD_STATUS_ZOOM );
}

/*****************************************************
**
**   BasicVedicChart   ---   paintField
**
******************************************************/
void BasicVedicChart::paintField( const int &i )
{
	MRect rr = fields[i].rect;

	//printf( "ZOOM %d while %f and %f\n", config->vedicChartBehavior->zoomFactor, .5 * ( rr.width + rr.height ), a );

	if ( isFieldZoomed( i ))
	{
		const double avgSize  = .5 * ( rr.width + rr.height );

		double newSize  = avgSize * chartprops->getVedicChartBehaviorConfig().zoomFactor;
		newSize /= 100.0;
		const double a = newSize - avgSize;
		rr.x -= .5 * a;
		rr.y -= .5 * a;
		rr.width += a;
		rr.height += a;
	}

	switch( fields[i].fieldtype )
	{
		case FIELD_TYPE_FULL: // all fields in south indian chart, sbc, fixed rasis in east indian chart
			painter->drawRectangle( rr );
		break;

		case FIELD_TYPE_NE: // east: taurus and sag
		{
			MPoint mp[3] =
			{
				MPoint( rr.x, rr.y ), 
				MPoint( rr.x + rr.width, rr.y ), 
				MPoint( rr.x  + rr.width, rr.y + rr.height )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_SW: // east: gemini and scorpio
		{
			MPoint mp[3] =
			{
				MPoint( rr.x, rr.y ), 
				MPoint( rr.x, rr.y + rr.height ), 
				MPoint( rr.x  + rr.width, rr.y + rr.height )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_NW: // east: leo and pisces
		{
			MPoint mp[3] =
			{
				MPoint( rr.x, rr.y ), 
				MPoint( rr.x, rr.y + rr.height ), 
				MPoint( rr.x  + rr.width, rr.y )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_SE: // east: virgo and aquarius
		{
			MPoint mp[3] = {
				MPoint( rr.x, rr.y + rr.height ), 
				MPoint( rr.x + rr.width, rr.y ), 
				MPoint( rr.x  + rr.width, rr.y + rr.height )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_N: // north: houses 2 and 12
		{
			MPoint mp[3] = {
				MPoint( rr.x, rr.y ), 
				MPoint( rr.x + rr.width, rr.y ), 
				MPoint( rr.x  + 0.5 * rr.width, rr.y + rr.height )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_W: // north: houses 3 and 5
		{
			MPoint mp[3] = {
				MPoint( rr.x, rr.y ), 
				MPoint( rr.x, rr.y + rr.height ), 
				MPoint( rr.x  + rr.width, rr.y + 0.5 * rr.height )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_S: // north: houses 6 and 8
		{
			MPoint mp[3] = {
				MPoint( rr.x, rr.y + rr.height ), 
				MPoint( rr.x + rr.width, rr.y + rr.height ), 
				MPoint( rr.x  + 0.5 * rr.width, rr.y )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_E: // north: houses 9 and 11
		{
			MPoint mp[3] = {
				MPoint( rr.x + rr.width, rr.y ), 
				MPoint( rr.x + rr.width, rr.y + rr.height ), 
				MPoint( rr.x, rr.y + 0.5 * rr.height )
			}; 
			painter->drawPolygon( 3, mp );
		}
		break;

		case FIELD_TYPE_DIAMOND:// north: houses 1: 4: 7: 10
		{
			MPoint mp[4] = {
				MPoint( rr.x + 0.5 * rr.width, rr.y ), 
				MPoint( rr.x + rr.width, rr.y + 0.5 * rr.height ), 
				MPoint( rr.x + 0.5 * rr.width, rr.y + rr.height ), 
				MPoint( rr.x, rr.y + 0.5 * rr.height )
			}; 
			painter->drawPolygon( 4, mp );
		}
		break;

		default:
			printf( "WARN: invalid field type %d in BasicVedicChart::paintField\n", fields[i].fieldtype );
			painter->drawRectangle( rr );
		break;
	}
}

/**************************************************************
***
**   BasicVedicChart   ---   OnResponsiveChartEvent
***
***************************************************************/
bool BasicVedicChart::OnResponsiveChartEvent( const MPoint p, const int /* keyMod */ )
{
	bool changed = false;

	uint f = UINT_FOR_NOT_FOUND;
	int status = 0;

	if ( p != MPoint( -1, -1 ))
	{
		f = getFieldForScreenPos( p );
		//printf( "F %d\n", f );
		if ( positionOffset != 0 )
		{
			//f = a_red( f + positionOffset, field_count );
		}
	}
	//printf( "Marked field is %d f is %d markedFieldStatus %d offset %d\n", markedField, f, markedFieldStatus, positionOffset );

	if ( markedField != f ) changed = true;
	markedField = f;

	if ( f == UINT_FOR_NOT_FOUND )
	{
		markedFieldStatus = 0;
	}
	else
	{
		const VedicChartBehaviorConfig &bcfg = chartprops->getVedicChartBehaviorConfig();

		if ( bcfg.highlightOnMouseOver ) status |= CHART_FIELD_STATUS_MARKUP;
		if ( bcfg.zoomOnMouseOver ) status |= CHART_FIELD_STATUS_ZOOM;

		if ( bcfg.aspectingObjectsMode == 1 ) status |= CHART_FIELD_STATUS_ASPECTING;
		if ( bcfg.aspectedObjectsMode == 1 ) status |= CHART_FIELD_STATUS_ASPECTED;
		if ( bcfg.allAspectsMode == 1 && charttype != CT_PARTNER ) status |= CHART_FIELD_STATUS_ALL_ASPECTS;

		if ( wxGetKeyState( WXK_SHIFT ))
		{
			if ( bcfg.aspectingObjectsMode == 2 ) status |= CHART_FIELD_STATUS_ASPECTING;
			if ( bcfg.aspectedObjectsMode == 2 ) status |= CHART_FIELD_STATUS_ASPECTED;
			if ( bcfg.allAspectsMode == 2 && charttype != CT_PARTNER ) status |= CHART_FIELD_STATUS_ALL_ASPECTS;
		}

		if ( wxGetKeyState( WXK_CONTROL ))
		{
			if ( bcfg.aspectingObjectsMode == 3 ) status |= CHART_FIELD_STATUS_ASPECTING;
			if ( bcfg.aspectedObjectsMode == 3 ) status |= CHART_FIELD_STATUS_ASPECTED;
			if ( bcfg.allAspectsMode == 3 && charttype != CT_PARTNER ) status |= CHART_FIELD_STATUS_ALL_ASPECTS;
		}

		if ( wxGetKeyState( WXK_ALT ))
		{
			if ( bcfg.aspectingObjectsMode == 4 ) status |= CHART_FIELD_STATUS_ASPECTING;
			if ( bcfg.aspectedObjectsMode == 4 ) status |= CHART_FIELD_STATUS_ASPECTED;
			if ( bcfg.allAspectsMode == 4 && charttype != CT_PARTNER ) status |= CHART_FIELD_STATUS_ALL_ASPECTS;
		}

		// test if mouse position is within transit rect (used for non cummutative partner aspects)
		if ( charttype == CT_RADIX || charttype == CT_TRANSIT ) status |= CHART_FIELD_STATUS_POS_FULL;
		else
		{
			if ( pointInRect( p, transitrect )) status |= CHART_FIELD_STATUS_POS_INNER;
			else status |= CHART_FIELD_STATUS_POS_OUTER;
		}
		//printf( "Status now %d\n", status );

		if ( status != markedFieldStatus ) changed = true;
		markedFieldStatus = status;
	}

	//printf( "Ende BasicVedicChart::OnResponsiveChartEvent point %f %f markedField %d markedFieldStatus %d status %d changed %d\n", p.real(), p.imag(), markedField, markedFieldStatus, status, changed );
	return changed;
}

/*****************************************************
**
**   BasicVedicChart   ---   writeChartContents
**
******************************************************/
void BasicVedicChart::writeChartContents( const int &chart_id, const bool applyFilter )
{
	ObjectId planet;
	uint f;
	wxString lname, sname, symbol, av;
	bool retro, avmode;
	SymbolProvider sp;
	SheetFormatter fmt;

	if ( chart_id == 0 && ! h1set ) printf( "WARN BasicVedicChart::writeChartContents no chart set for id 0\n" );
	if ( chart_id == 1 && ! h2set ) printf( "WARN BasicVedicChart::writeChartContents no chart set for id 1\n" );

	ObjectArray o = applyFilter ? chartprops->getVedicPlanetList( chartprops->getObjectFilter()) : obs;
	for ( uint i = 0; i < o.size(); i++ )
	{
		planet = o[i];

		// do not paint ascendant in north indian chart
		if ( field_count == 12 && chart_id == 0 && planet == OASCENDANT && chartprops->getVedicChartDisplayConfig().indianChartType == VCT_NORTH ) continue;

		// do not paint ascendant in south indian chart if markup of AS is enabled
		if ( field_count == 12 && ! h2set && planet == OASCENDANT && chartprops->getVedicChartDisplayConfig().southIndianAscendantMarkup ) continue;

		f = a_red( getPlanetField( planet, chart_id ) - positionOffset, field_count );
		assert( f < fields.size());

		ChartContents &cc = fields[f].getContents( chart_id );

		cc.planets.push_back( planet );

		lname = fmt.getObjectNamePlain( planet, TF_LONG, true );
		sname = fmt.getObjectNamePlain( planet, TF_MEDIUM, true );

		// retrogression
		retro = ( chartprops->getVedicChartDisplayConfig().showRetro ) && getPlanetRetro( planet, chart_id )
		        && planet != OMEANNODE && planet != OMEANDESCNODE;
		if ( retro )
		{
			//lname += wxT( " " );
			lname += _( "(R)" );
			sname += _( "R" );
		}

		// see if transit mode, varga, av and valid planet
		avmode = ( IS_AVPLANET( planet ) && charttype == CT_TRANSIT && chart_id == 1 && field_count == 12 && chartprops->getVedicChartDisplayConfig().showAshtakavarga );

		if ( avmode )
		{
			av = wxString::Format( wxT( " %d" ), getAshtakavargaPoints( planet, f ));
			sname += av;
			lname += av;
		}

		// symbol
		symbol.Clear();
		if ( ! avmode && config->writer->planetSymbols && getVChartConfig()->useSymbols && planet <= MAX_EPHEM_OBJECTS ) symbol = sp.getPlanetCode( planet );
		if ( ! symbol.IsEmpty() )
		{
			cc.graphicitems.push_back( ChartGraphicItem( symbol, planet, retro ));
		}
		else
		{
			ChartTextItem item( lname, sname, retro );
			cc.textitems.push_back( item );
		}
	}
}

/*****************************************************
**
**   BasicVedicChart   ---   setAspectPainterStyle
**
******************************************************/
void BasicVedicChart::setAspectPainterStyle( const int &i, const bool isBenefic )
{
	VedicChartConfig *vconf = getVChartConfig();
	int style;

  wxColour color;
	switch ( chartprops->getVedicChartBehaviorConfig().beneficMaleficDisplay )
	{
		case 0: // specific
			if ( isBenefic ) color = config->colors->beneficFgColor;
			else color = config->colors->maleficFgColor;
		break;
		case 1: // background
			color = vconf->pen.IsOk() ? vconf->pen.GetColour() : config->colors->fgColor;
		break;
		case 2: // grey
			color = *wxLIGHT_GREY;
		break;
	}

	switch( i )
	{
		case VA_25:
			style = wxDOT;
		break;
		case VA_50:
			style = wxSHORT_DASH;
		break;
		case VA_75:
			style = wxLONG_DASH;
		break;
		case VA_FULL:
			style = wxSOLID;
		break;
		default:
			printf( "BasicVedicChart::setAspectPen WARN: wrong aspect strength %d\n", i );
			style = wxSOLID;
		break;
	}
	painter->setPen( wxPen( color, 1, style ));
	painter->setBrush( color );
}

/*****************************************************
**
**   BasicVedicChart   ---   paintAspectsForFieldPair
**
******************************************************/
void BasicVedicChart::paintAspectsForFieldPair( const int &f1, const int &f2 )
{
	const double dist = 4;
	MPoint p1, p2;
	MRect rect;
	int acount = 0;
	list<AspectEvent> &al = aexpert->getAspectList();
	list<AspectEvent>::iterator iter;
	AspectEvent *event = (AspectEvent*)NULL;

	//printf( "BasicVedicChart::paintAspectsForFieldPair %d %d\n", f1, f2 );

	// first step determine field parts and whether apsect relation has to swapped
	FIELD_PART part1 = FP_ALL;
	FIELD_PART part2 = FP_ALL;
	bool swap = false;

	if ( charttype == CT_TRANSIT )
	{
		part1 = FP_OUTER;
		part2 = FP_INNER;
	}
	else if ( charttype == CT_PARTNER )
	{
		const bool aspecting = markedFieldStatus & CHART_FIELD_STATUS_ASPECTING;
		const bool aspected = markedFieldStatus & CHART_FIELD_STATUS_ASPECTED;
		//printf( "aspecting %d aspected %d\n", aspecting, aspected );

		//const bool markedFull = markedFieldStatus & CHART_FIELD_STATUS_POS_FULL;
		const bool markedInner = markedFieldStatus & CHART_FIELD_STATUS_POS_INNER;
		const bool markedOuter = markedFieldStatus & CHART_FIELD_STATUS_POS_OUTER;
		//assert( markedFull ^ markedInner ^ markedOuter );

		if (( markedInner && aspected ) || ( markedOuter && aspecting ))
		{
			part1 = FP_INNER;
			part2 = FP_OUTER;
		}
		else if (( markedInner && aspecting ) || ( markedOuter && aspected ))
		{
			part1 = FP_OUTER;
			part2 = FP_INNER;
			swap = true;
		}
	}

	// second step: find number of matching aspects
	for ( iter = al.begin(); iter != al.end(); iter++ )
	{
		if ( (*iter).field1 == f1 && (*iter).field2 == f2 && (*iter).swapCharts == swap )
		{
			event = (AspectEvent*)&(*iter);
			acount++;
		}
	}
	//printf( "ACOUNT %d H2SET %d\n", acount, h2set );


	if ( acount > 0 )
	{
		// correct field index for north indian chart
		int g1 = f1;
		int g2 = f2;
		if ( positionOffset != 0 )
		{
			g1 = a_red( f1 - positionOffset, field_count );
			g2 = a_red( f2 - positionOffset, field_count );
		}
		if ( part1 != FP_ALL )
		{
			rect = fields[g1].calculateModifiedRect( part1, MPoint( xr - 0.5 * transitrect.width, yr - 0.5 * transitrect.height ));
			p1 = rect.getCenter();
			rect = fields[g2].calculateModifiedRect( part2, MPoint( xr - 0.5 * transitrect.width, yr - 0.5 * transitrect.height ));
			p2 = rect.getCenter();
		}
		else
		{
			p1 = fields[g1].getCenter();
			p2 = fields[g2].getCenter();
		}
		if ( acount == 1 )
		{
			assert( event );
			setAspectPainterStyle( event->strength, event->benefic );
			paintArrow( p1, p2 );
		}
		else // acount > 1, draw aspects in parallele style
		{
			MPoint c = findOrthogonalPoint( p1 - p2, acount * dist );
			int i = 0;
			for ( iter = al.begin(); iter != al.end(); iter++ )
			{
				if ( (*iter).field1 == f1 && (*iter).field2 == f2 && (*iter).swapCharts == swap )
				{
					double lambda = i++;
					lambda /= (double)( acount - 1 );
					MPoint q1 = lambda * ( p1 - c ) + ( 1 - lambda ) * ( p1 + c );
					MPoint q2 = lambda * ( p2 - c ) + ( 1 - lambda ) * ( p2 + c );
					setAspectPainterStyle( (*iter).strength, (*iter).benefic );
					paintArrow( q1, q2 );
				}
			}
		}
	}
}

/*****************************************************
**
**   BasicVedicChart   ---   setup
**
******************************************************/
void BasicVedicChart::setup()
{
	//AspectExpert experti( horoscope );

	xmax = rect.width;
	ymax = rect.height;

	// set total chart width and height to 95%, so radius is 47.5%
	const double rfactor =  0.00475 * getVChartConfig()->outerRectangle.radius;

	//printf( "BasicVedicChart::setup Radius %f\n", getVChartConfig()->outerRectangle.radius );

	xr = xmax * rfactor;
	yr = ymax * rfactor;

	fields.clear();
	setupChart();
}

/*****************************************************
**
**   BasicVedicChart   ---   paintInternal
**
******************************************************/
void BasicVedicChart::paintInternal( ChartConfig *cfg )
{
	wxArrayInt a;
	BgPalette *palette = (BgPalette*)NULL;

	//printf( "BasicVedicChart::paintInternal markedField %d\n", markedField );

	assert( cfg );
	VedicChartConfig *vconf = getVChartConfig();

	// paint field backgrounds if fields are not marked or zoomed
	if ( ! vconf->palette.IsEmpty() )
	{
		PaletteManager *man = PaletteManager::get();
		palette = man->getPalette( vconf->palette );
		if ( palette )
		{
			painter->setTransparentPen();
			for ( uint i = 0; i < field_count; i++ )
			{
				if (( markedField == i ) && ( markedFieldStatus & CHART_FIELD_STATUS_MARKUP || markedFieldStatus & CHART_FIELD_STATUS_ZOOM )) continue;

				painter->setBrush( palette->getBrush( i ));
				paintField( i );
			}
		}
		else
		{
			wxLogError( wxT( "no palette named '%s', referenced in config named '%s'" ), vconf->palette.c_str(), vconf->name.c_str() );
		}
	}

	setDefaultTextColor();

  painter->setPen( vconf->pen.IsOk() ? vconf->pen : defaultPen  );
	painter->setDefaultBrush();
	drawGrid();
	paintOuterRectangle();

	// draw markup resp. zoomed fields
  painter->setPen( vconf->pen.IsOk() ? vconf->pen : defaultPen  );
	for ( uint i = 0; i < field_count; i++ )
	{
		if (( markedField == i ) && ( markedFieldStatus & CHART_FIELD_STATUS_MARKUP
			|| markedFieldStatus & CHART_FIELD_STATUS_ZOOM  ))
		{

			// geht nicht wenn background gesetzt ist 
			if ( markedFieldStatus & CHART_FIELD_STATUS_MARKUP )
			{
				painter->setBrush( config->colors->chartMarkupBgColor );
			}
			else if ( isChartFieldMarked( i ))
			{
				if ( vconf->markedFields.brush.IsOk() ) painter->setBrush( vconf->markedFields.brush );
				else painter->setBrush( config->colors->sbcMarkedFieldsBgColor );
			}
			else if ( palette )
			{
				painter->setBrush( palette->getBrush( i ));
			}
			else
			{
				// background must be painted inorder to overrride other fields
				//GcWidgetBackground *bg = &vconf->widgetBackground;
				if ( vconf->brush.IsOk() )
				{
					painter->setBrush( vconf->brush );
				}
				else
				{
					painter->setDefaultBrush();
				}
			}
			paintField( i );
		}
	}

	// Draw text items
	for ( uint i = 0; i < field_count; i++ )
	{
		if ( h1set && ! h2set )
		{
			drawFieldText( i, FP_ALL );
		}
		else
		{
			if ( h1set ) drawFieldText( i, FP_INNER );
			if ( h2set ) drawFieldText( i, FP_OUTER );
		}
	}

	// additional rectangle separating the chart
	if ( h1set && h2set )
	{
		painter->setTransparentBrush();

		// maybe Nil in case of SBC
		if ( ! transitrect.isNil() ) painter->drawRectangle( transitrect );
	}

	/*
	printf( "\nmarkedField %d markedFieldStatus CHART_FIELD_STATUS_POS_FULL %d\n", markedField, markedFieldStatus & CHART_FIELD_STATUS_POS_FULL );
	printf( "markedField %d markedFieldStatus CHART_FIELD_STATUS_POS_INNER %d\n", markedField, markedFieldStatus & CHART_FIELD_STATUS_POS_INNER );
	printf( "markedField %d markedFieldStatus CHART_FIELD_STATUS_POS_OUTER %d\n\n", markedField, markedFieldStatus & CHART_FIELD_STATUS_POS_OUTER );
	*/

	if ( markedFieldStatus & CHART_FIELD_STATUS_ALL_ASPECTS )
	{
		for( uint j = 0; j < field_count; j++ )
		{
			for( uint target = 0; target < field_count; target++ )
			{
				paintAspectsForFieldPair( j, target );
			}
		}
	}
	else
	{
		for( uint j = 0; j < field_count; j++ )
		{
			//printf( "markedField %d j %d markedFieldStatus %d\n", markedField, j, markedFieldStatus );
			if (( markedField == j ) && ( markedFieldStatus & CHART_FIELD_STATUS_ASPECTED ))
			{
				for( uint target = 0; target < field_count; target++ )
				{
					paintAspectsForFieldPair( a_red( j + positionOffset, field_count ), target );
				}
			}

			// TODO der unterstützt nicht mehrfache Aspekte nebeneinander
			if (( markedField == j ) && ( markedFieldStatus & CHART_FIELD_STATUS_ASPECTING ))
			{
				for(  int source = 0; source < (int)field_count; source++ )
				{
					paintAspectsForFieldPair( source, a_red( j + positionOffset, field_count ) );
				}
			}
		}
	}
	finishChart();
}

/*****************************************************
**
**   BasicVedicChart   ---   drawFieldText
**
******************************************************/
void BasicVedicChart::drawFieldText( const uint &f, const FIELD_PART &part )
{
	assert( f < field_count );

	//printf( "BasicVedicChart::drawFieldText field %d mode %d\n", f, mode );

	ChartField &field = fields[f];
	const double zoomfactor = isFieldZoomed( f ) ? chartprops->getVedicChartBehaviorConfig().zoomFactor / 100.0 : 1.0;

	wxString s;
	assert( part == FP_ALL || part == FP_INNER || part == FP_OUTER );

	ChartContents &cc = field.getContents( part == FP_OUTER ? 1 : 0 );


	const int align = field.align;
	MRect rect = field.calculateModifiedRect( part, MPoint( xr - 0.5 * transitrect.width, yr - 0.5 * transitrect.height ));

	const double border = rect.width / 15.0;

	if ( align & Align::Left )
	{
		rect.x += border;
		rect.width -= border;
	}
	else if ( align & Align::Right )
	{
		rect.width -= border;
	}
	if ( align & Align::Top )
	{
		rect.y += border;
		rect.height -= border;
	}
	else if ( align & Align::Bottom )
	{
		rect.height -= border;
	}

	// number of text items
	uint titems = cc.textitems.size();
	// number of graphic items
	uint gitems = cc.graphicitems.size();

	if ( titems == 0 && gitems == 0 ) return; // nothing to do

	// Maximum number of lines for complete contents
	int max_lines = (int)Max(rect.height/ text_height, 1 );
	//printf( "rect.height %f text_height %f max_lines %d\n", rect.height, text_height, max_lines );

	int total_lines = 0;

	// grahic items per line: put them into one line if more than 3, else 2 lines
	int gitems_pline = 0;
	if ( gitems > 3 )
	{
		total_lines = 2;
		gitems_pline = ( gitems + 1 ) / 2;
	}
	else if ( gitems > 0 )
	{
		total_lines = 1;
		gitems_pline = gitems;
	}

	// text items per line
	int titems_pline = 0;
	if ( titems > 0 )
	{
		const int left_lines = max_lines - total_lines;
		if ( left_lines <= 1 )
		{
			// put all items in one line, no place left
			titems_pline = titems;
			total_lines++;
		}
		else
		{
			// distribute the items over the left lines
			titems_pline = 1 + titems / left_lines;
			total_lines = total_lines + titems / titems_pline;
		}
	}

	int total_height = total_lines * text_height;
	int starty = rect.y;
	if ( align & Align::Bottom ) starty = rect.y + rect.height - total_height;
	else if ( align & Align::VCenter ) starty = rect.y + ( rect.height - total_height ) / 2;

	MRect trect = rect;
	trect.y = starty;
	trect.height = text_height;

	// paint graphic items
	vector<ChartGraphicItem> g;
	uint i = 0;
	int j = 0;
	setSymbolFont( zoomfactor );
	while ( i < gitems )
	{
		g.push_back( cc.graphicitems[i++] );

		if ( ++j >= gitems_pline )
		{
			drawGraphicItemLine( trect, g, align, part );
			j = 0;

			trect.y += text_height;
			g.clear();
		}
	}
	if ( g.size() > 0 )
	{
		drawGraphicItemLine( trect, g, align, part );
		trect.y += text_height;
	}

	// paint text items
	setGraphicFont( zoomfactor );
	i = 0;
	j = 0;
	s.Clear();
	while ( i < titems )
	{
		if ( s.Len() > 0 ) s << wxT( " " );

		if ( titems_pline == 1
		        && cc.textitems[i].longname.Len() * text_width < rect.width )
		{
			s << cc.textitems[i++].longname;
		}
		else s << cc.textitems[i++].shortname;

		if ( ++j >= titems_pline )
		{
			drawTextItemLine( trect, s, align, part );
			j = 0;
			trect.y += text_height;
			s.Clear();
		}
	}
	if ( s.Len() > 0 ) drawTextItemLine( trect, s, align, part );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawSingleGraphicItem
**
******************************************************/
void BasicVedicChart::drawSingleGraphicItem( const MRect &rect, const ChartGraphicItem &g, const FIELD_PART &part )
{
	VedicChartConfig *vconf = getVChartConfig();
	if ( vconf->useTextColors && chartprops->getVedicChartDisplayConfig().showPlanetColors )
	{
		painter->setTextColor( vconf->getPlanetColor( g.pindex ));
	}
	else
	{
		setDefaultTextColor( part );
	}
	painter->drawTextFormatted( rect, g.name, Align::Center );
	if ( g.retro ) painter->drawTextFormatted( MRect( rect.x + text_width, rect.y+text_height/3, rect.width, rect.height ), wxT( "_" ), Align::Center);
}

/*****************************************************
**
**   BasicVedicChart   ---   drawTextItemLine
**
******************************************************/
void BasicVedicChart::drawTextItemLine( MRect rect, const wxString &s, const int &align, const FIELD_PART &part )
{
	setDefaultTextColor( part );
	MPoint p = painter->getTextExtent( s );

	if ( align & Align::Right ) rect.x = rect.x + rect.width - p.real();
	else if ( align & Align::HCenter ) rect.x = rect.x + .5 * ( rect.width - p.real() );

	//painter->drawRectangle( rect );

	// TODO

	painter->drawTextFormatted( rect, s, Align::Left + Align::Top );
}

/*****************************************************
**
**   BasicVedicChart   ---   drawGraphicItemLine
**
******************************************************/
void BasicVedicChart::drawGraphicItemLine( MRect rect, const vector<ChartGraphicItem> g, const int &align, const FIELD_PART &part )
{
	double leftx = rect.x;
	double xstep = 2 * text_width;
	double total_width = g.size() * xstep;

	if ( align &  Align::Right ) leftx = rect.x + rect.width - total_width;
	else if ( align & Align::HCenter ) leftx = rect.x + ( rect.width - total_width ) / 2;

	MRect trect = rect;
	trect.x = leftx;
	trect.width = xstep;
	for ( uint i = 0; i < g.size(); i++ )
	{
		drawSingleGraphicItem( trect, g[i], part );
		trect.x += xstep;
	}
}

/*****************************************************
**
**   BasicVedicChart   ---   drawTextFormatted
**
******************************************************/
void BasicVedicChart::drawTextFormatted( const MRect &rect, const wxString t, const int& align )
{
	painter->drawTextFormatted( rect, t, align );
}

#define RTOL_DOUBLE_FRAME .012

/*****************************************************
**
**   BasicVedicChart   ---   paintOuterRectangle
**
******************************************************/
void BasicVedicChart::paintOuterRectangle()
{
	VedicChartConfig *vconf = getVChartConfig();
  if ( ! vconf->outerRectangle.show ) return;

  painter->setPen( vconf->outerRectangle.pen.IsOk() ? vconf->outerRectangle.pen : defaultPen  );

  if ( vconf->outerRectangle.brush.IsOk() ) painter->setBrush( vconf->outerRectangle.brush );
  else painter->setTransparentBrush();

  MRect coord( xcenter - xr, ycenter - yr, 2 * xr, 2 * yr );
  painter->drawRectangle( coord, .01 * Min( xmax, ymax ) * vconf->outerRectangle.cornerRadius );

	if ( vconf->outerRectangle.doubleOuterLine )
	{
		// distance of second frame: derive from rmax -> prevent scaling errors in pdf output
		const double delta = rmax / 100.0;
		painter->drawRectangle( MRect( coord.x - delta, coord.y - delta, coord.width + 2 * delta, coord.height + 2 * delta ));
	}
	painter->setPen( defaultPen );
}


