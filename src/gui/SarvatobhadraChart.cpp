/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SarvatobhadraChart.cpp
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

#include "SarvatobhadraChart.h"

#include "Aspect.h"
#include "astrobase.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "maitreya.h"
#include "ColorConfig.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "Painter.h"
#include "SymbolProvider.h"
#include "VedicChartConfig.h"

#include <vector>
#include <wx/brush.h>
#include <wx/font.h>

using namespace std;

extern Config *config;

#define RTOL_DOUBLE_FRAME .0125
#define SARVATOBHADRA_EXTRAZOOM .6

// Upper line
const wxChar * sanskrit_ee[2] = { wxT( "ee" ), wxT( "\u0908" ) };
const wxChar * sanskrit_a[2] = { wxT( "a" ), wxT( "\u0905" ) };

// line 2
const wxChar * sanskrit_ri[2] = { wxT( "ri" ), wxT( "\u090B" ) };
const wxChar * sanskrit_g[2] = { wxT( "g" ), wxT( "\u0917" ) };
const wxChar * sanskrit_s[2] = { wxT( "s" ), wxT( "\u0938" ) };
const wxChar * sanskrit_d[2] = { wxT( "d" ), wxT( "\u0926" ) };
const wxChar * sanskrit_ch[2] = { wxT( "ch" ), wxT( "\u091A" ) };
const wxChar * sanskrit_l[2] = { wxT( "l" ), wxT( "\u0932" ) };
const wxChar * sanskrit_u[2] = { wxT( "u" ), wxT( "\u0909" ) };

// line 3
const wxChar * sanskrit_kh[2] = { wxT( "kh" ), wxT( "\u0916" ) };
const wxChar * sanskrit_ai[2] = { wxT( "ai" ), wxT( "\u0910" ) };
const wxChar * sanskrit_li[2] = { wxT( "li" ), wxT( "\u090C" ) };

// line 4
const wxChar * sanskrit_j[2] = { wxT( "j" ), wxT( "\u091C" ) };
const wxChar * sanskrit_ah[2] = { wxT( "ah" ), wxT( "\u0905\u0903" ) };
const wxChar * sanskrit_o[2] = { wxT( "o" ), wxT( "\u0913" ) };
const wxChar * sanskrit_v[2] = { wxT( "v" ), wxT( "\u0935" ) };

// line 5
const wxChar * sanskrit_bh[2] = { wxT( "bh" ), wxT( "\u092D" ) };
const wxChar * sanskrit_k[2] = { wxT( "k" ), wxT( "\u0915" ) };

// line 6
const wxChar * sanskrit_y[2] = { wxT( "y" ), wxT( "\u092F" ) };
const wxChar * sanskrit_am[2] = { wxT( "am" ), wxT( "\u0905\u0902" ) };
const wxChar * sanskrit_au[2] = { wxT( "au" ), wxT( "\u0914" ) };
const wxChar * sanskrit_h[2] = { wxT( "h" ), wxT( "\u0939" ) };

// line 7
const wxChar * sanskrit_n[2] = { wxT( "n" ), wxT( "\u0928" ) };
const wxChar * sanskrit_e[2] = { wxT( "e" ), wxT( "\u090F" ) };
const wxChar * sanskrit_dd[2] = { wxT( "d" ), wxT( "\u0921" ) };

// line 8
const wxChar * sanskrit_t[2] = { wxT( "t" ), wxT( "\u0924" ) };
const wxChar * sanskrit_r[2] = { wxT( "r" ), wxT( "\u0930" ) };
const wxChar * sanskrit_p[2] = { wxT( "p" ), wxT( "\u092A" ) };
const wxChar * sanskrit_th[2] = { wxT( "th" ), wxT( "\u091F" ) };
const wxChar * sanskrit_m[2] = { wxT( "m" ), wxT( "\u092E" ) };
const wxChar * sanskrit_uu[2] = { wxT( "uu" ), wxT( "\u090A" ) };

// Bottom line
const wxChar * sanskrit_i[2] = { wxT( "i" ), wxT( "\u0907" ) };
const wxChar * sanskrit_aa[2] = { wxT( "aa" ), wxT( "\u0906" ) };

/*****************************************************
**
**   SarvatobhadraChart   ---   Constructor
**
******************************************************/
SarvatobhadraChart::SarvatobhadraChart( const ChartType &charttype, ChartProperties *chartprops, Horoscope* h1, Horoscope* h2 )
		: BasicVedicChart( charttype, chartprops, 28, h1, h2 )
{
	init();
}

/*****************************************************
**
**   SarvatobhadraChart   ---   Destructor
**
******************************************************/
SarvatobhadraChart::~SarvatobhadraChart()
{
}

/*****************************************************
**
**   SarvatobhadraChart   ---   init
**
******************************************************/
void SarvatobhadraChart::init()
{
	extrazoom = SARVATOBHADRA_EXTRAZOOM;

	for ( int i = 0; i < 9; i++ )
		for ( int j = 0; j < 9; j++ )
			mark[i][j] = 0;

	if ( h1 )
	{
		int x, y;
		getNakshatraIndex( ::getNakshatra28( h1->getVedicLongitude( OMOON )), x, y ) ;
		mark[x][y] = 1;
		getRasiIndex( getRasi( h1->getVedicLongitude( OASCENDANT )), x, y );
		mark[x][y] = 1;
		getWeekdayIndex( h1->getDataSet()->getWeekDay(), x, y );
		mark[x][y] = 1;
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   updateAspects
**
******************************************************/
void SarvatobhadraChart::updateAspects()
{
	assert( aexpert );
	aexpert->updateSbcAspects( chartprops, false );
	if ( charttype == CT_PARTNER )
	{
		aexpert->updateSbcAspects( chartprops, true );
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   setupChart
**
******************************************************/
void SarvatobhadraChart::setupChart()
{
	xstep = xr / 4.5;
	ystep = yr / 4.5;

	transitrect = MRect( xcenter - 4 * xstep, ycenter - 4 * ystep, 8 * xstep, 8 * ystep );

	GRAVITATION gravitation = GRAVITATION_S;
	fields.clear();
	for( NakshatraId_28 i = N28_ASWINI; i <= N28_REVATI; i++ )
	{
		if ( i >= N28_KRITTIKA && i <= N28_ASLESHA ) { gravitation = GRAVITATION_W; }
		else if ( i >= N28_MAKHA && i <= N28_VISAKHA ) { gravitation = GRAVITATION_N; }
		else if ( i >= N28_ANURADHA && i <= N28_SRAVANA ) { gravitation = GRAVITATION_E; }
		else { gravitation = GRAVITATION_S; }

		fields.push_back( ChartField( getNakshatraCoord( i ), gravitation, FIELD_TYPE_FULL ));
	}
  if ( h1set ) writeChartContents( 0 );
	if ( h2set ) writeChartContents( 1, charttype == CT_TRANSIT && chartprops->isFiltered() );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   finishChart
**
******************************************************/
void SarvatobhadraChart::finishChart()
{
	wxString s, qq;
	Lang lang;
	MPoint p;
	int quality;

	const bool doprops = h1 && chartprops->getVedicChartDisplayConfig().showSbcNakshatraQuality;

	setDefaultTextColor();

	if ( doprops )
	{
		NakshatraExpert().getSbcNakshatraProperties( getNakshatra28( h1->getVedicLongitude( OMOON )), nakshatraProps );
	}
	else nakshatraProps.clear();

	// draw Nakshatra names
	for ( NakshatraId_28 i = N28_ASWINI ; i <= N28_REVATI; i++ )
	{
		if ( doprops )
		{
			quality = nakshatraProps[i].taraQuality;
			if ( quality == TARA_PROP_BENEFIC ) qq = wxT( " B" );
			else if ( quality == TARA_PROP_MALEFIC ) qq = wxT( " M" );
		}
		s = lang.getNakshatra28Name( i, TF_MEDIUM ) + qq;
		p = painter->getTextExtent( s );
		if ( p.real() > .9 * xstep ) s = lang.getNakshatra28Name( i, TF_SHORT ) + qq;

		setGraphicFontZoom( isFieldZoomed( i ) ? chartprops->getVedicChartBehaviorConfig().zoomFactor / 100.0 : 1.0 );
		painter->drawTextFormatted( getNakshatraCoord( i ), s, Align::HCenter + Align::Bottom );
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   drawGrid
**
******************************************************/
void SarvatobhadraChart::drawGrid()
{
	Lang lang;
	int i, j;
	MRect rect, fieldrect;
	SymbolProvider sp;
	VedicChartConfig *vconf = getVChartConfig();

	paintOuterRectangle();

	painter->setPen( defaultPen );
	painter->setTransparentBrush();

	for ( i = 1; i <= 8; i++ )
	{
		painter->drawLine( xcenter - xr + i * xstep, ycenter - yr, xcenter - xr + i * xstep, ycenter + yr );
		painter->drawLine( xcenter - xr , ycenter - yr + i * ystep , xcenter + xr, ycenter - yr + i * ystep );
	}

	painter->setPen( defaultPen );
	if ( vconf->markedFields.brush.IsOk() ) painter->setBrush( vconf->markedFields.brush );
	else painter->setBrush( config->colors->sbcMarkedFieldsBgColor );

	for ( i = 0; i < 9; i++ )
	{
		for ( j = 0; j < 9; j++ ) if ( mark[i][j] ) painter->drawRectangle( getFieldCoord( i, j ));
	}
	painter->setPen( defaultPen );
	painter->setTransparentBrush();

	// Static text portions
	const int symbolindex = chartprops->getVedicChartDisplayConfig().showSbcSanskritSymbols ? 1 : 0;

#define DRAW_STATIC_FIELD( a, b, t ) \
	fieldrect = getFieldCoord( a, b ); \
	if ( ! refreshRect || ( refreshRect && refreshRect->Intersects( fieldrect.toWxRect() ))) \
	{ \
		painter->drawTextFormatted( fieldrect, t, Align::HCenter + Align::Bottom ); \
	}

#define DRAW_STATIC_FIELD_SANSKRIT( a, b, t ) \
	fieldrect = getFieldCoord( a, b ); \
	if ( ! refreshRect || ( refreshRect && refreshRect->Intersects( fieldrect.toWxRect() ))) \
	{ \
		painter->drawTextFormatted( fieldrect, t[symbolindex], Align::HCenter + Align::Bottom ); \
	}

	DRAW_STATIC_FIELD_SANSKRIT( 0, 0, sanskrit_ee )
	DRAW_STATIC_FIELD_SANSKRIT( 8, 0, sanskrit_a )

	// Line 2
	DRAW_STATIC_FIELD_SANSKRIT( 1, 1, sanskrit_ri )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 1, sanskrit_g )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 1, sanskrit_s )
	DRAW_STATIC_FIELD_SANSKRIT( 4, 1, sanskrit_d )
	DRAW_STATIC_FIELD_SANSKRIT( 5, 1, sanskrit_ch )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 1, sanskrit_l )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 1, sanskrit_u )

	// Line 3
	DRAW_STATIC_FIELD_SANSKRIT( 1, 2, sanskrit_kh )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 2, sanskrit_ai )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 2, sanskrit_li )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 2, sanskrit_a )

	// Line 4
	DRAW_STATIC_FIELD_SANSKRIT( 1, 3, sanskrit_j )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 3, sanskrit_ah )
	DRAW_STATIC_FIELD( 4, 3, lang.getWeekdayName( 5 ).Left(3) )
	DRAW_STATIC_FIELD_SANSKRIT( 5, 3, sanskrit_o )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 3, sanskrit_v )

	// Line 5
	DRAW_STATIC_FIELD_SANSKRIT( 1, 4, sanskrit_bh )
	DRAW_STATIC_FIELD( 3, 4, lang.getWeekdayName( 4 ).Left(3) )
	DRAW_STATIC_FIELD( 4, 4, lang.getWeekdayName( 6 ).Left(3) )
	DRAW_STATIC_FIELD( 5, 4, lang.getWeekdayName( 0 ).Left(3) + wxT( " " ) + lang.getWeekdayName( 2 ).Left(3) )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 4, sanskrit_k )

	// Line 6
	DRAW_STATIC_FIELD_SANSKRIT( 1, 5, sanskrit_y )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 5, sanskrit_am )
	DRAW_STATIC_FIELD( 4, 5, lang.getWeekdayName( 1 ).Left(3) + wxT( " " ) + lang.getWeekdayName( 3 ).Left(3))
	DRAW_STATIC_FIELD_SANSKRIT( 5, 5, sanskrit_au )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 5, sanskrit_h )

	// Line 7
	DRAW_STATIC_FIELD_SANSKRIT( 1, 6, sanskrit_n )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 6, sanskrit_e )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 6, sanskrit_li )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 6, sanskrit_dd )

	// Line 8
	DRAW_STATIC_FIELD_SANSKRIT( 1, 7, sanskrit_kh )
	DRAW_STATIC_FIELD_SANSKRIT( 2, 7, sanskrit_t )
	DRAW_STATIC_FIELD_SANSKRIT( 3, 7, sanskrit_r )
	DRAW_STATIC_FIELD_SANSKRIT( 4, 7, sanskrit_p )
	DRAW_STATIC_FIELD_SANSKRIT( 5, 7, sanskrit_th )
	DRAW_STATIC_FIELD_SANSKRIT( 6, 7, sanskrit_m )
	DRAW_STATIC_FIELD_SANSKRIT( 7, 7, sanskrit_uu )

	// Line 9
	DRAW_STATIC_FIELD_SANSKRIT( 0, 8, sanskrit_i )
	DRAW_STATIC_FIELD_SANSKRIT( 8, 8, sanskrit_aa )
#undef DRAW_STATIC_FIELD
#undef DRAW_STATIC_FIELD_SANSKRIT

	// Signs
	if ( config->writer->signSymbols ) setSymbolFont();
#define DRAW_SIGN_SYMBOL( x, y, index ) \
	painter->drawTextFormatted( getFieldCoord( x, y ), \
	config->writer->signSymbols ? (wxString)sp.getSignCode( index ) : lang.getSignName( index, TF_MEDIUM ), \
		Align::HCenter + Align::Bottom );

	DRAW_SIGN_SYMBOL( 5, 2, R_ARIES )
	DRAW_SIGN_SYMBOL( 6, 3, R_TAURUS )
	DRAW_SIGN_SYMBOL( 6, 4, R_GEMINI )
	DRAW_SIGN_SYMBOL( 6, 5, R_CANCER )
	DRAW_SIGN_SYMBOL( 5, 6, R_LEO )
	DRAW_SIGN_SYMBOL( 4, 6, R_VIRGO )
	DRAW_SIGN_SYMBOL( 3, 6, R_LIBRA )
	DRAW_SIGN_SYMBOL( 2, 5, R_SCORPIO )
	DRAW_SIGN_SYMBOL( 2, 4, R_SAGITTARIUS )
	DRAW_SIGN_SYMBOL( 2, 3, R_CAPRICORN )
	DRAW_SIGN_SYMBOL( 3, 2, R_AQUARIUS )
	DRAW_SIGN_SYMBOL( 4, 2, R_PISCES )
#undef DRAW_SIGN_SYMBOL
	setGraphicFont();

	// Draw planets
	setDefaultTextColor();
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getPlanetField
**
******************************************************/
int SarvatobhadraChart::getPlanetField( const ObjectId &planet, const int &chart_id )
{
	Horoscope *h = getHoroscope( chart_id );
	if ( h ) return ::getNakshatra28( h->getVedicLongitude( planet ));
	else return 0;
}

/*****************************************************
**
*   SarvatobhadraChart   ---   isChartFieldMarked
**
******************************************************/
bool SarvatobhadraChart::isChartFieldMarked( const int &nak )
{
	int x, y;
	getNakshatraIndex( (NakshatraId_28)nak, x, y );
	return mark[x][y];
}

/*****************************************************
**
**   SarvatobhadraChart   ---   handleMouseLeftEvent
**
******************************************************/
bool SarvatobhadraChart::handleMouseLeftEvent( int x, int y, wxRect &rect )
{
	//printf( "MOUSE LEF IN SBC\n" );
	if ( getFieldIndex( &x, &y ))
	{
		mark[x][y] = ! mark[x][y];
		rect = getFieldCoord( x, y ).toWxRect();
		return true;
	}
	return false;
}

/*****************************************************
**
**   SarvatobhadraChart   ---   markField
**
******************************************************/
void SarvatobhadraChart::markField( int x, int y )
{
	if ( getFieldIndex( &x, &y ))
	{
		mark[x][y] = ! mark[x][y];
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getFieldIndex
**
******************************************************/
bool SarvatobhadraChart::getFieldIndex( int *x, int *y )
{
	if ( *x < xcenter - xr ) return false;
	if ( *x > xcenter + xr ) return false;
	if ( *y < ycenter -yr ) return false;
	if ( *y > ycenter + yr ) return false;

	*x = (int)(( *x - xcenter + xr ) / xstep);
	*y = (int)(( *y - ycenter + yr ) / ystep);
	return true;
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getFieldCoord
**
******************************************************/
MRect SarvatobhadraChart::getFieldCoord( const int &x, const int &y )
{
	return MRect( xcenter - xr + x * xstep, ycenter - yr + y * ystep, xstep, ystep );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getWeekdayIndex
**
******************************************************/
void SarvatobhadraChart::getWeekdayIndex( const int &w, int &x, int &y )
{
	assert( w >= 0 && w < 7 );
	switch ( w )
	{
	case 0:
	case 2:
		x = 5;
		y = 4;
		break;
	case 1:
	case 3:
		x = 4;
		y = 5;
		break;
	case 4:
		x = 3;
		y = 4;
		break;
	case 5:
		x = 4;
		y = 3;
		break;
	default:
		x = 4;
		y = 4;
		break;
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getWeekdayCoord
**
******************************************************/
MRect SarvatobhadraChart::getWeekdayCoord( const int &w )
{
	int x, y;
	getWeekdayIndex( w, x, y );
	return getFieldCoord( x, y );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getRasiIndex
**
******************************************************/
void SarvatobhadraChart::getRasiIndex( const Rasi &r, int &x, int &y )
{
	assert( r >= 0 && r < 12 );
	switch ( r )
	{
	case R_ARIES:
		x = 5;
		y = 2;
		break;
	case R_TAURUS:
		x = 6;
		y = 3;
		break;
	case R_GEMINI:
		x = 6;
		y = 4;
		break;
	case R_CANCER:
		x = 6;
		y = 5;
		break;
	case R_LEO:
		x = 5;
		y = 6;
		break;
	case R_VIRGO:
		x = 4;
		y = 6;
		break;
	case R_LIBRA:
		x = 3;
		y = 6;
		break;
	case R_SCORPIO:
		x = 2;
		y = 5;
		break;
	case R_SAGITTARIUS:
		x = 2;
		y = 4;
		break;
	case R_CAPRICORN:
		x = 2;
		y = 3;
		break;
	case R_AQUARIUS:
		x = 3;
		y = 2;
		break;
	case R_PISCES:
	default:
		x = 4;
		y = 2;
		break;
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getRasiCoord
**
******************************************************/
MRect SarvatobhadraChart::getRasiCoord( const Rasi &r )
{
	int x, y;
	getRasiIndex( r, x, y );
	return getFieldCoord( x, y );
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getNakshatraIndex
**
******************************************************/
void SarvatobhadraChart::getNakshatraIndex( const NakshatraId_28 &i, int &x, int &y )
{
	ASSERT_VALID_NAKSHATRA_28( i );
	if ( i < 2 )
	{
		x = i + 6;
		y = 0;
	}
	else if ( i >= 2 && i <= 8 )
	{
		x = 8;
		y = i-1;
	}
	else if ( i >= 9 && i <= 15 )
	{
		x = 16 - i;
		y = 8;
	}
	else if ( i >= 16 && i <= 22 )
	{
		x = 0;
		y =  8 - ( i - 15 );
	}
	else
	{
		x = i - 22;
		y = 0;
	}
}

/*****************************************************
**
**   SarvatobhadraChart   ---   getNakshatraCoord
**
******************************************************/
MRect SarvatobhadraChart::getNakshatraCoord( const NakshatraId_28 &i )
{
	int x, y;
	assert( i >= 0 && i <= 27 );
	getNakshatraIndex( i, x, y );
	return MRect( xcenter - xr + ( x * xstep ), ycenter - yr + ( y * ystep ), xstep, ystep );
}

/*****************************************************
**
**   ChartFactory   ---   createSarvatobhadraChart
**
******************************************************/
GraphicalChart *ChartFactory::createSarvatobhadraChart( const ChartType &charttype, ChartProperties *chartprops,
	Horoscope *h1, Horoscope *h2 )
{
	return new SarvatobhadraChart( charttype, chartprops, h1, h2 );
}


