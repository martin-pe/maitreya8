/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicRasiChart.cpp
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
			//const double mtol = xmax / 100.0;

			// diagonal from egde to egde paint one line or two depending on chart center
			painter->drawLine( xcenter - xr + cornertol, ycenter - yr + cornertol, xcenter + xr - cornertol, ycenter + yr - cornertol );
			painter->drawLine( xcenter - xr + cornertol, ycenter + yr - cornertol, xcenter + xr - cornertol, ycenter - yr + cornertol );

			painter->drawLine( xcenter, ycenter + yr, xcenter + xr, ycenter );
			painter->drawLine( xcenter, ycenter + yr, xcenter - xr, ycenter );
			painter->drawLine( xcenter, ycenter - yr, xcenter - xr, ycenter );
			painter->drawLine( xcenter, ycenter - yr, xcenter + xr, ycenter );
		}
		break;
	
		case VCT_EAST:
		{
			// inner square of chart
			const double xi = xr * 0.28;
			const double yi = yr * 0.28;

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
**   VedicRasiChart   ---   finishChart
**
******************************************************/
void VedicRasiChart::finishChart()
{
	Lang lang;
	SymbolProvider sp;
	wxString s;
	Rasi current, i;
	VedicChartConfig *vconf = getVChartConfig();

	if ( chartprops->getVedicChartDisplayConfig().indianChartType == VCT_NORTH)
	{
		// used for sign number/symbol display
		const double rtol = xmax / 15.0;

		// 1/4 of the chart
		const double xi = .5 * xr;
		const double yi = .5 * yr;

		// Rectangles for sign names resp. numbers
		const MRect sign_rect[12] = {
			MRect( xcenter - rtol, ycenter - rtol, 2 * rtol, rtol ),
			MRect( xcenter - xi - rtol / 2, ycenter - yi - rtol, rtol, rtol ),
			MRect( xcenter - xi - rtol, ycenter - yi - rtol / 2, rtol, rtol ),
			MRect( xcenter - 2 * rtol, ycenter - rtol, 2 * rtol, rtol ),
			MRect( xcenter - xi - rtol, ycenter + yi - rtol / 2, rtol, rtol ),
			MRect( xcenter - xi - rtol / 2, ycenter + yi, rtol, rtol ),

			MRect( xcenter - rtol, ycenter, 2 * rtol, rtol ),
			MRect( xcenter + xi - rtol / 2, ycenter + yi, rtol, rtol ),
			MRect( xcenter + xi, ycenter + yi - rtol / 2, rtol, rtol ),
			MRect( xcenter, ycenter - rtol, 2 * rtol, rtol ),
			MRect( xcenter + xi, ycenter - yi - rtol / 2, rtol, rtol ),
			MRect( xcenter + xi - rtol / 2, ycenter - yi - rtol, rtol, rtol )
		};

		/*********************************************************
		**  draw number of houses
		*********************************************************/
		if ( ! ( chartprops->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_SYMBOL )) setGraphicFont();
		else setSymbolFont();

		for ( i = R_ARIES; i <= R_PISCES; i++ )
		{
			current = (Rasi)redRasi( getAscendant() + i );
			switch( chartprops->getVedicChartDisplayConfig().northIndianSignDisplayType )
			{
				case VCN_ASC:
					s.Printf( wxT( "%d" ),  current + 1 );
				break;
				case VCN_NUMBER:
					s.Printf( wxT( "%d" ),  current + 1 );
				break;
				case VCN_SHORT:
				{
					s = lang.getSignName( current, TF_SHORT );
				}
				break;
				default:
					s = sp.getSignCode( current );
				break;
			}
			painter->drawTextFormatted( sign_rect[i], s, Align::Center );

			// leave after ascendant for style == 0
			if ( chartprops->getVedicChartDisplayConfig().northIndianSignDisplayType == VCN_ASC ) break;
		}
	}
	else if ( chartprops->getVedicChartDisplayConfig().indianChartType == VCT_SOUTH && h1set && ! h2set )
	{
		if ( chartprops->getVedicChartDisplayConfig().southIndianAscendantMarkup )
		{
			const MRect &rr = fields[getAscendant()].rect;
			if ( chartprops->getVedicChartDisplayConfig().southIndianAscendantMarkup == 2 ) 
			{
				wxPen pen( vconf->pen.IsOk() ? vconf->pen.GetColour() : defaultPen.GetColour(), 1, wxDOT  );
				painter->setPen( pen );
			}
			painter->drawLine( MPoint( rr.x, rr.y + rr.height ), MPoint( rr.x + rr.width, rr.y ));
		}
	}

	paintCenterString();
}

/*****************************************************
**
**   VedicRasiChart   ---   setupChart
**
******************************************************/
void VedicRasiChart::setupChart()
{
	positionOffset = 0;
	ascendant = getPlanetField( OASCENDANT, 0 );

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
	const double xi = xr*0.28;
	const double yi = yr*0.28;

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

	setDefaultTextColor();

	if ( centerstring_graphic ) setSymbolFont();
	else setGraphicFont();

	painter->drawTextFormatted( MRect( xcenter - xr, ycenter - yr, 2 * xr, 2 * yr ), getCenterString(), Align::Center );
}

