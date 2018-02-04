/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicRasiChart.cpp
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

#include "VedicRasiChart.h"

#include "Aspect.h"
#include "astrobase.h"
#include "Lang.h"
#include "ChartProperties.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "SymbolProvider.h"
#include "VedicChartConfig.h"

/*****************************************************
**
**   VedicRasiChart   ---   Constructor
**
******************************************************/
VedicRasiChart::VedicRasiChart( const ChartType &charttype, ChartProperties *chartprops, Horoscope *h1, Horoscope *h2 )
		: BasicVedicChart( charttype, chartprops, 12, h1, h2 )
{
	centerstring_graphic = false;
}

/*****************************************************
**
**   VedicRasiChart   ---   drawGrid
**
******************************************************/
void VedicRasiChart::drawGrid()
{
	VedicChartConfig *vconf = getVChartConfig();
	painter->setPen( vconf->pen.IsOk() ? vconf->pen : defaultPen  );

	painter->setTransparentBrush();
	//painter->setBrush( vconf->brush.IsOk() ? vconf->brush : defaultBrush  );

	double cornertol = 0;
  if ( vconf->outerRectangle.cornerRadius )
	{
		cornertol = .01 * Min( xmax, ymax ) * vconf->outerRectangle.cornerRadius;
		cornertol /= SQRT2 * 2;
	}
	//printf( "vconf->outerRectangle.cornerRadius %d cornertol %f\n", vconf->outerRectangle.cornerRadius, cornertol );

	switch( chartprops->getVedicChartDisplayConfig().indianChartType )
	{
		case VCT_NORTH:
		{
			if ( chartprops->getVedicChartDisplayConfig().centerInfoType == VCC_NOTHING )
			{
				painter->drawLine( xcenter - xr + cornertol, ycenter - yr + cornertol, xcenter + xr - cornertol, ycenter + yr - cornertol );
				painter->drawLine( xcenter - xr + cornertol, ycenter + yr - cornertol, xcenter + xr - cornertol, ycenter - yr + cornertol );
			}
			else
			{
				// break cross lines if a center string is available
				//const double innertol = .08;
				const double innertol = text_height / 300;
				painter->drawLine( xcenter - xr + cornertol, ycenter - yr + cornertol,
					xcenter - innertol * ( xr - cornertol ), ycenter - innertol * ( yr + cornertol ));
				painter->drawLine( xcenter + innertol * ( xr - cornertol ), ycenter + innertol * ( yr - cornertol ),
				xcenter + xr - cornertol, ycenter + yr - cornertol );

				painter->drawLine( xcenter - xr + cornertol, ycenter + yr - cornertol,
					xcenter - innertol * ( xr + cornertol ), ycenter + innertol * ( yr - cornertol ));
				painter->drawLine( xcenter + innertol * ( xr - cornertol ), ycenter - innertol * ( yr + cornertol ),
					xcenter + xr - cornertol, ycenter - yr + cornertol );
			}

			painter->drawLine( xcenter, ycenter + yr, xcenter + xr, ycenter );
			painter->drawLine( xcenter, ycenter + yr, xcenter - xr, ycenter );
			painter->drawLine( xcenter, ycenter - yr, xcenter - xr, ycenter );
			painter->drawLine( xcenter, ycenter - yr, xcenter + xr, ycenter );
		}
		break;
	
		case VCT_EAST:
		{
			// inner square of chart
			//const double xi = xr * 0.28;
			//const double yi = yr * 0.28;
			const double xi = xr / 3.0;
			const double yi = yr / 3.0;

			// vertical lines in the middle
			painter->drawLine( xcenter - xi, ycenter - yr, xcenter - xi, ycenter + yr );
			painter->drawLine( xcenter + xi, ycenter-yr, xcenter+xi, ycenter+yr );

			// horizontal lines in the middle
			painter->drawLine( xcenter - xr, ycenter + yi, xcenter + xr, ycenter + yi );
			painter->drawLine( xcenter - xr, ycenter - yi, xcenter + xr, ycenter - yi );

			// left diagonal lines
			painter->drawLine( xcenter - xr + cornertol, ycenter + yr - cornertol, xcenter - xi, ycenter + yi );
			painter->drawLine( xcenter - xr + cornertol, ycenter - yr + cornertol, xcenter - xi, ycenter - yi );

			// right diagonal lines
			painter->drawLine( xcenter + xr - cornertol, ycenter + yr - cornertol, xcenter + xi, ycenter + yi );
			painter->drawLine( xcenter + xr - cornertol, ycenter - yr + cornertol, xcenter + xi, ycenter - yi );
		}
		break;
		
		case VCT_SOUTH:
		default:
		{
			const double xstep = .5 * xr;
			const double ystep = .5 * yr;

			painter->drawLine( xcenter - xr + xstep, ycenter - yr, xcenter - xr + xstep, ycenter + yr  );
			painter->drawLine( xcenter - xr + 3 * xstep, ycenter - yr, xcenter - xr + 3 * xstep, ycenter + yr  );

			painter->drawLine( xcenter - xr + 2 * xstep, ycenter - yr, xcenter-xr + 2 * xstep, ycenter - yr + ystep  );
			painter->drawLine( xcenter - xr + 2 * xstep, ycenter + yr - ystep, xcenter - xr + 2 * xstep, ycenter + yr  );

			painter->drawLine( xcenter - xr, ycenter - yr + ystep, xcenter + xr, ycenter - yr + ystep  );
			painter->drawLine( xcenter - xr, ycenter - yr + 3 * ystep, xcenter + xr, ycenter - yr + 3 * ystep  );

			painter->drawLine( xcenter - xr, ycenter - yr + 2 * ystep, xcenter - xr + xstep, ycenter - yr + 2 * ystep  );
			painter->drawLine( xcenter + xr - xstep, ycenter - yr + 2 * ystep, xcenter + xr, ycenter - yr + 2 * ystep  );
		}
		break;
	}
}

/*****************************************************
**
**   VedicRasiChart   ---   calculateCenterRect
**
******************************************************/
void VedicRasiChart::calculateCenterRect()
{
	//printf( "VedicRasiChart::calculateCenterRect\n" );
	MPoint p;
	const vector<wxString> v = getCenterString();
	centerRect = MRect();

	if ( centerstring_graphic ) setSymbolFont();
	else setGraphicFont();

	for( uint i = 0; i < v.size(); i++ )
	{
		p = painter->getTextExtent( v[i] );
		centerRect.height += p.imag();
		centerRect.width = Max( p.real(), centerRect.width );
	}
	if ( v.size())
	{
		centerRect.width += text_width;
	}
	else
	{
		centerRect.width += 4 * text_width;
		centerRect.height += 2 * text_height;
	}
	centerRect.x = xcenter - .5 * centerRect.width;
	centerRect.y = ycenter - .5 * centerRect.height;
	//printf( "centerRect x %f y %f w %f h %f\n", centerRect.x, centerRect.y, centerRect.width, centerRect.height );
}

/*****************************************************
**
**   VedicRasiChart   ---   finishChart
**
******************************************************/
void VedicRasiChart::finishChart()
{
	VedicChartConfig *vconf = getVChartConfig();

	// calculate center string rectangle now because inner sign symbols (angles) in North Indian chart must be aligned accordingly
	calculateCenterRect();

	if ( chartprops->getVedicChartDisplayConfig().indianChartType == VCT_NORTH)
	{
		// reasonable width
		const double rtol = xmax / 12.0;
		const int disp = chartprops->getVedicChartDisplayConfig().northIndianSignDisplayType;
		const MPoint p = painter->getTextExtent( getIndianSignLabel( R_ARIES, disp ) );

		// 1/4 of the chart
		const double xi = .5 * xr;
		const double yi = .5 * yr;

		// Rectangles for sign names resp. numbers
		const MRect sign_rect[12] = {
			MRect( xcenter - .5 * centerRect.width, centerRect.y - text_height, centerRect.width, text_height ),
			MRect( xcenter - xi - rtol / 2, ycenter - yi - rtol, rtol, rtol ),
			MRect( xcenter - xi - rtol, ycenter - yi - rtol / 2, rtol, rtol ),

			MRect( centerRect.x - p.real(), ycenter - .5 * rtol, p.real(), rtol ),
			MRect( xcenter - xi - rtol, ycenter + yi - rtol / 2, rtol, rtol ),
			MRect( xcenter - xi - rtol / 2, ycenter + yi, rtol, rtol ),

			MRect( xcenter - .5 * centerRect.width, centerRect.y + centerRect.height, centerRect.width, text_height ),
			MRect( xcenter + xi - rtol / 2, ycenter + yi, rtol, rtol ),
			MRect( xcenter + xi, ycenter + yi - rtol / 2, rtol, rtol ),

			MRect( centerRect.x + centerRect.width, ycenter - .5 * rtol, p.real(), rtol ),
			MRect( xcenter + xi, ycenter - yi - rtol / 2, rtol, rtol ),
			MRect( xcenter + xi - rtol / 2, ycenter - yi - rtol, rtol, rtol )
		};

		/*********************************************************
		**  draw number of houses
		*********************************************************/
		if ( ! ( chartprops->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_SYMBOL )) setGraphicFont();
		else setSymbolFont();
 
		setDefaultTextColor();
		for ( Rasi i = R_ARIES; i <= R_PISCES; i++ )
		{
			painter->drawTextFormatted( sign_rect[i], getIndianSignLabel( (Rasi)redRasi( getAscendant() + i ), disp ), Align::Center );

			// leave after ascendant for style == 0
			if ( chartprops->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_ASC ) break;
		}
	}
	else if ( chartprops->getVedicChartDisplayConfig().indianChartType == VCT_SOUTH && h1set && ! h2set )
	{
		// reasonable width
		const double rtol = 15.0;
		const int disp = chartprops->getVedicChartDisplayConfig().southIndianSignDisplayType;
		const MPoint p = painter->getTextExtent( getIndianSignLabel( R_ARIES, disp ) );

		// 1/4 of the chart
		const double xstep = .5 * xr;
		const double ystep = .5 * yr;

		// Rectangles for sign names resp. numbers
		const MRect sign_rect[12] = {
			MRect( xcenter - .5 * xstep - .5 * rtol, ycenter - ystep - rtol, rtol, rtol ),
			MRect( xcenter + .5 * xstep - .5 * rtol, ycenter - ystep - rtol, rtol, rtol ),
				MRect( xcenter + xstep, ycenter - ystep - 1.3 * rtol, 1.3 * rtol, rtol ),
			MRect( xcenter + xstep, ycenter - .5 * ystep - .5 * rtol, rtol, rtol ),
			MRect( xcenter + xstep, ycenter + .5 * ystep - .5 * rtol, rtol, rtol ),
				MRect( xcenter + xstep, ycenter + ystep, 1.2 * rtol, 1.2 * rtol ),

			MRect( xcenter + .5 * xstep - .5 * rtol, ycenter + ystep, rtol, rtol ),
			MRect( xcenter - .5 * xstep - .5 * rtol, ycenter + ystep, rtol, rtol ),
				MRect( xcenter - xstep - rtol, ycenter + ystep, 1.2 * rtol, 1.2 * rtol ),
			MRect( xcenter - xstep - rtol, ycenter + .5 * ystep - .5 * rtol, rtol, rtol ),
			MRect( xcenter - xstep - rtol, ycenter - .5 * ystep - .5 * rtol, rtol, rtol ),
				MRect( xcenter - xstep - 1.2 * rtol, ycenter - ystep - 1.2 * rtol, rtol, rtol )
		};

		if ( ! ( chartprops->getVedicChartDisplayConfig().southIndianSignDisplayType == VCN_SYMBOL )) setGraphicFont();
		else setSymbolFont();
 
		setDefaultTextColor();
		for ( Rasi i = R_ARIES; i <= R_PISCES; i++ )
		{
			painter->drawTextFormatted( sign_rect[i], getIndianSignLabel( (Rasi)redRasi( i ), disp ), Align::Center );
		}
		if ( chartprops->getVedicChartDisplayConfig().southIndianAscendantMarkup )
		{
			const MRect &rr = fields[getAscendant()].rect;
			if ( chartprops->getVedicChartDisplayConfig().southIndianAscendantMarkup == 2 ) 
			{
				wxPen pen( vconf->pen.IsOk() ? vconf->pen.GetColour() : defaultPen.GetColour(), 1, wxDOT  );
				painter->setPen( pen );
			}
			painter->drawLine( MPoint( rr.x, rr.y + rr.height * .9 ), MPoint( rr.x + rr.width * .1, rr.y + rr.height ));
		}
	}
	paintCenterString();
}

/*****************************************************
**
**   VedicRasiChart   ---   getNorthIndianSignLabel
**
******************************************************/
wxString VedicRasiChart::getIndianSignLabel( const Rasi &rasi, const int &disp )
{
	wxString s;
	Lang lang;
	SymbolProvider sp;

	switch( disp )
	{
		case VCN_ASC:
			s.Printf( wxT( "%d" ),  rasi + 1 );
		break;
		case VCN_NUMBER:
			s.Printf( wxT( "%d" ),  rasi + 1 );
		break;
		case VCN_SHORT:
		{
			s = lang.getSignName( rasi, TF_SHORT );
		}
		break;
		default:
			s = sp.getSignCode( rasi );
		break;
	}
	return s;
}

/*****************************************************
**
**   VedicRasiChart   ---   setupChart
**
******************************************************/
void VedicRasiChart::setupChart()
{
	// bugfix 2017-6-20: ascendant was only set in constructor, so there was no update of asc in North Indian chart
	if ( h1set ) ascendant = getPlanetField( OASCENDANT, 0 );
	else ascendant = R_ARIES;
	positionOffset = 0;
	//printf( "Hallo h1set %d ascendant %d\n", h1set, ascendant );

	fields.clear();
	switch( chartprops->getVedicChartDisplayConfig().indianChartType )
	{
		case VCT_NORTH:
			positionOffset = ascendant;
			setupNorth();
		break;
		case VCT_EAST:
			setupEast();
		break;
		case VCT_SOUTH:
			setupSouth();
		break;
		default:
			printf( "ERROR chartprops->getVedicChartDisplayConfig().indianChartType %d\n", chartprops->getVedicChartDisplayConfig().indianChartType );
			assert( false );
		break;
	}

	if ( h1set ) writeChartContents( 0 );
	if ( h2set ) writeChartContents( 1, charttype == CT_TRANSIT && chartprops->isFiltered()  );
}

/*****************************************************
**
**   VedicRasiChart   ---   setupSouth
**
******************************************************/
void VedicRasiChart::setupSouth()
{
	const double xstep = .5 * xr;
	const double ystep = .5 * yr;

	fields.push_back( ChartField( MRect( xcenter - xstep, ycenter - 2 * ystep, xstep, ystep ), GRAVITATION_S ));
	fields.push_back( ChartField( MRect( xcenter, ycenter - 2 * ystep, xstep, ystep ), GRAVITATION_S ));
	fields.push_back( ChartField( MRect( xcenter + xstep, ycenter - 2 * ystep, xstep, ystep ), GRAVITATION_SW ));
	fields.push_back( ChartField( MRect( xcenter + xstep, ycenter - ystep, xstep, ystep ), GRAVITATION_W ));
	fields.push_back( ChartField( MRect( xcenter + xstep, ycenter, xstep, ystep ), GRAVITATION_W ));
	fields.push_back( ChartField( MRect( xcenter + xstep, ycenter + ystep, xstep, ystep ), GRAVITATION_NW ));

	fields.push_back( ChartField( MRect( xcenter, ycenter + ystep, xstep, ystep ), GRAVITATION_N ));
	fields.push_back( ChartField( MRect( xcenter - xstep, ycenter + ystep, xstep, ystep ), GRAVITATION_N ));
	fields.push_back( ChartField( MRect( xcenter - 2 * xstep, ycenter + ystep, xstep, ystep  ), GRAVITATION_NE ));
	fields.push_back( ChartField( MRect( xcenter - 2 * xstep, ycenter, xstep, ystep ), GRAVITATION_E ));
	fields.push_back( ChartField( MRect( xcenter - 2 * xstep, ycenter-ystep, xstep, ystep ), GRAVITATION_E ));
	fields.push_back( ChartField( MRect( xcenter - 2 * xstep, ycenter - 2 * ystep, xstep, ystep ), GRAVITATION_SE ));

	transitrect = MRect( xcenter - 1.5 * xstep, ycenter - 1.5 * ystep, 3 * xstep, 3 * ystep );
}

/*****************************************************
**
**   VedicRasiChart   ---   setupNorth
**
******************************************************/
void VedicRasiChart::setupNorth()
{
	// 1/4 of the chart
	const double xi = .5 * xr;
	const double yi = .5 * yr;

	// starts with 1st diamond field in the north = ascendant.
	fields.push_back( ChartField( MRect( xcenter - xi, ycenter - yr, xr, yr ), GRAVITATION_S, FIELD_TYPE_DIAMOND ));
	fields.push_back( ChartField( MRect( xcenter - xr, ycenter - yr, xr, yi ), GRAVITATION_S, FIELD_TYPE_N ));
	fields.push_back( ChartField( MRect( xcenter - xr, ycenter - yr, xi, yr ), GRAVITATION_E, FIELD_TYPE_W ));

	fields.push_back( ChartField( MRect( xcenter - xr, ycenter - yi, xr, yr ), GRAVITATION_E, FIELD_TYPE_DIAMOND ));
	fields.push_back( ChartField( MRect( xcenter - xr, ycenter, xi, yr ), GRAVITATION_E, FIELD_TYPE_W ));
	fields.push_back( ChartField( MRect( xcenter - xr, ycenter + yi, xr, yi ), GRAVITATION_N, FIELD_TYPE_S ));

	fields.push_back( ChartField( MRect( xcenter - xi, ycenter, xr, yr ), GRAVITATION_N, FIELD_TYPE_DIAMOND ));
	fields.push_back( ChartField( MRect( xcenter, ycenter + yi, xr, yi ), GRAVITATION_N, FIELD_TYPE_S ));
	fields.push_back( ChartField( MRect( xcenter + xi, ycenter, xi, yr ), GRAVITATION_W, FIELD_TYPE_E ));

	fields.push_back( ChartField( MRect( xcenter, ycenter - yi, xr, yr ), GRAVITATION_W, FIELD_TYPE_DIAMOND ));
	fields.push_back( ChartField( MRect( xcenter + xi, ycenter - yr, xi, yr ), GRAVITATION_W, FIELD_TYPE_E ));
	fields.push_back( ChartField( MRect( xcenter, ycenter - yr, xr, yi ), GRAVITATION_S, FIELD_TYPE_N ));

	// middle square for separation of 2 charts (transit or partner mode)
	const double ri_prop = .4;
	const double xm = ri_prop * xi + ( 1 - ri_prop ) * xr;
	const double ym = ri_prop * yi + ( 1 - ri_prop ) * yr;
	transitrect = MRect( xcenter - xm, ycenter - ym, 2 * xm, 2 * ym );
}

/*****************************************************
**
**   VedicRasiChart   ---   setupEast
**
******************************************************/
void VedicRasiChart::setupEast()
{
	// inner square of chart
	//const double xi = xr*0.28;
	//const double yi = yr*0.28;
	const double xi = xr / 3.0;
	const double yi = yr / 3.0;

	 // 8 rectangles according to directions north, north west ...
	 const MRect r_n( xcenter - xi, ycenter - yr, 2 * xi, yr - yi );
	 const MRect r_nw( xcenter - xr, ycenter - yr, xr - xi, yr - yi );
	 const MRect r_w( xcenter - xr, ycenter - yi, xr - xi, 2 * yi );
	 const MRect r_sw( xcenter - xr, ycenter + yi, xr - xi, yr - yi );
	 const MRect r_s( xcenter - xi, ycenter + yi, 2 * xi, yr - yi );
	 const MRect r_se( xcenter + xi, ycenter + yi, xr - xi, yr - yi );
	 const MRect r_e( xcenter + xi, ycenter - yi, xr - xi, 2 * yi );
	 const MRect r_ne( xcenter + xi, ycenter - yr, xr - xi, yr - yi );

	fields.push_back( ChartField( r_n, GRAVITATION_S, FIELD_TYPE_FULL ));
	fields.push_back( ChartField( r_nw, GRAVITATION_SE, FIELD_TYPE_NE ));
	fields.push_back( ChartField( r_nw, GRAVITATION_SE, FIELD_TYPE_SW ));

	fields.push_back( ChartField( r_w, GRAVITATION_E, FIELD_TYPE_FULL));
	fields.push_back( ChartField( r_sw, GRAVITATION_NE, FIELD_TYPE_NW ));
	fields.push_back( ChartField( r_sw, GRAVITATION_NE, FIELD_TYPE_SE ));

	fields.push_back( ChartField( r_s, GRAVITATION_N, FIELD_TYPE_FULL));
	fields.push_back( ChartField( r_se, GRAVITATION_NW, FIELD_TYPE_SW ));
	fields.push_back( ChartField( r_se, GRAVITATION_NW, FIELD_TYPE_NE ));

	fields.push_back( ChartField( r_e, GRAVITATION_W, FIELD_TYPE_FULL));
	fields.push_back( ChartField( r_ne, GRAVITATION_SW, FIELD_TYPE_SE));
	fields.push_back( ChartField( r_ne, GRAVITATION_SW, FIELD_TYPE_NW ));

	// middle square for separation of 2 charts
	const double ri_prop = .4;
	const double xm = ri_prop * xi + ( 1 - ri_prop ) * xr;
	const double ym = ri_prop * yi + ( 1 - ri_prop ) * yr;
	transitrect = MRect( xcenter - xm, ycenter - ym, 2 * xm, 2 * ym );
}

/*****************************************************
**
**   VedicRasiChart   ---   paintCenterString
**
******************************************************/
void VedicRasiChart::paintCenterString()
{
	if ( h1set && chartprops->getVedicChartDisplayConfig().centerInfoType == VCC_NOTHING ) return;
	vector<wxString> v = getCenterString();
	if ( v.size() == 0 ) return;
	//printf( "VedicRasiChart::paintCenterString text_height %f v size %ld\n", text_height, v.size() );

	setDefaultTextColor();
	if ( centerstring_graphic ) setSymbolFont();
	else setGraphicFont();

	double ystep = 0;
	if ( v.size() > 0 ) ystep = centerRect.height / v.size();
	//painter->drawRectangle( centerRect );

	for( uint i = 0; i < v.size(); i++ )
	{
		painter->drawTextFormatted( MRect( centerRect.x, centerRect.y + i * ystep, centerRect.width, ystep ), v[i], Align::Top + Align::HCenter );
	}
}

