/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Painter.cpp
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

#include "Painter.h"

#include "Aspect.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "FontProvider.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "maitreya.h"
#include "mathbase.h"
#include "Sheet.h"
#include "SymbolProvider.h"

#include <wx/dc.h>
#include <wx/log.h>
#include <wx/stopwatch.h>
#include <wx/tokenzr.h>

extern Config *config;

/*****************************************************
**
**   Painter   ---   Constructor
**
******************************************************/
Painter::Painter( WriterConfig *c, ColorConfig *ccfg )
{
	writercfg = c ? c : config->writer;
	colorcfg = ccfg ? ccfg : config->colors;
}

/*****************************************************
**
**   Painter   ---   setTransparentPen
**
******************************************************/
void Painter::setTransparentPen()
{
	setPen( *wxTRANSPARENT_PEN );
}

/*****************************************************
**
**   Painter   ---   drawRectangle
**
******************************************************/
void Painter::drawRectangle( const MRect &r, const double rnd )
{
	drawRectangle( r.x, r.y, r.width, r.height, rnd );
}

/*****************************************************
**
**   Painter   ---   setDefaultTextColor
**
******************************************************/
void Painter::setDefaultTextColor()
{
	setTextColor( getDefaultTextColor());
}

/*****************************************************
**
**   Painter   ---   drawLine
**
******************************************************/
void Painter::drawLine( const MPoint &p1, const MPoint &p2 )
{
	drawLine( p1.real(), p1.imag(), p2.real(), p2.imag() );
}

/*****************************************************
**
**   Painter   ---   drawSingleMStringLine
**
******************************************************/
void Painter::drawSingleMStringLine( const MRect &r, MString &f, const int& align )
{
	assert( f.formattedLines.size() == 0 );
	wxString s;
	MPoint p;
	Lang lang( writercfg );
	wxFont oldFont = getCurrentFont();
	const int drawalign = Align::Left + Align::VCenter;
	SheetFormatter formatter( writercfg );

	if ( ! f.isEmpty() && ( f.size.real() == 0 ||  f.size.imag() == 0 ))
	{
		printf( "WARN: size not set\n" );
		f.size = getTextExtent( f );
	}

	//printf( "PAINT -- --- - - - - x %f y %f w %f h %f SIZE x %f y %f\n", r.x, r.y, r.width, r.height, size.real(), size.imag() );
	//printf( " ----- %s\n", str2char( formatter.fragment2PlainText( f )));

	double x0 = r.x;
	double y0 = r.y;

	if ( align & Align::HCenter )
	{
		x0 += .5 * ( r.width - f.size.real());
	}
	else if ( align & Align::Right )
	{
		x0 = x0 + r.width - f.size.real();
	}

	// offset for subscriptum and superscriptum
	const double yoffset = .5 * f.size.imag();

	double yy = y0; // + .5 * ( r.height - size.imag());
	for( list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
	{
		switch ( iter->fontFormat )
		{
			case TTFF_SUBSCRPTUM:
				yy = y0 + yoffset;
			break;
			case TTFF_SUPERSCRPTUM:
				yy = y0 - yoffset;
			break;
			case TTFF_NORMAL:
			default:
				yy = y0;
			break;
		}

		setFont( oldFont );

		wxChar symbol = 0;
		SymbolProvider sp( writercfg );
		switch ( iter->entity )
		{
			case TTSE_PLANET:
				if ( writercfg->planetSymbols ) symbol = sp.getPlanetCode( (ObjectId)iter->entityId );
				if ( ! symbol || symbol == SYMBOL_CODE_ERROR )
					s = formatter.getObjectNamePlain( (ObjectId)iter->entityId, iter->textFormat, iter->vedic );
			break;
			case TTSE_SIGN:
				if ( writercfg->signSymbols ) symbol = sp.getSignCode( (Rasi)iter->entityId );
				if ( ! symbol ) s = lang.getSignName( (Rasi)iter->entityId, iter->textFormat ); //, writercfg->vedicSignNames );
			break;
			case TTSE_ASPECT:
				symbol = SymbolProvider().getAspectCode( (ASPECT_TYPE)iter->entityId );
				if ( ! symbol ) s = AspectExpert::getAspectShortDescription( (int)iter->entityId );
			break;
			case TTSE_DIRECTION:
				symbol = sp.getRetroCode( (MOVING_DIRECTION)iter->entityId );
				if ( ! symbol ) s = wxT( "R" );
			break;
			default:
				symbol = 0;
				s = iter->text;
			break;
		}
		if ( symbol && symbol != SYMBOL_CODE_ERROR )
		{
			setSymbolFont();
			drawTextFormatted( MRect( x0, yy, r.width, r.height ), symbol, drawalign );
			p = getTextExtent( symbol );
		}
		else
		{
			drawTextFormatted( MRect( x0, yy, r.width, r.height ), s, drawalign );
			p = getTextExtent( s );
		}

		x0 += p.real();
	}
}

//#define SHOW_STOP_WATCH
/*****************************************************
**
**   Painter   ---   drawMString
**
******************************************************/
void Painter::drawMString( const MRect &r, MString &f, const int& align )
{
#ifdef SHOW_STOP_WATCH
	static wxLongLong totaltime = 0;
	const wxLongLong starttime = wxGetLocalTimeMillis();
#endif

	if ( f.formattedLines.size() == 0 )
	{
		if ( ! f.isEmpty() && f.size.real() == 0 )
		{
			printf( "Painter::drawMString - size not set\n" );
			f.size = getTextExtent( f );
		}
		drawSingleMStringLine( r, f, align );
		//return;
	}
	else
	{
		double y0 = r.y;
		if ( align & Align::Top )
		{
			// nothing
		}
		else if ( align & Align::Bottom )
		{
			y0 = y0 + r.height - f.size.imag();
		}
		else // default: align & align::VCenter
		{
			y0 += .5 * ( r.height - f.size.imag());
		}

		MRect rect( r.x, y0, r.width, r.height );

		int line = 0;
		for( list<MString>::iterator iter = f.formattedLines.begin(); iter != f.formattedLines.end(); iter++ )
		{
			line++;
			rect.height = iter->size.imag();
			//printf( "  --->>> Line %d width %f h %f\n", line, iter->size.real(), iter->size.imag() );
			drawSingleMStringLine( rect, *iter, align );
			rect.y += rect.height;
		}
	}
#ifdef SHOW_STOP_WATCH
	const wxLongLong duration = wxGetLocalTimeMillis() - starttime;
	totaltime += duration;
	wxLogMessage( wxString::Format( wxT( "Painter::drawTextFormatted in %ld msec, total %ld" ), duration.ToLong(), totaltime.ToLong() ));
#endif

}

/*****************************************************
**
**   Painter   ---   getTextExtent
**
******************************************************/
MPoint Painter::getTextExtent( const MToken &token )
{
	const wxFont oldFont = getCurrentFont();
	wxString s;
	Lang lang;
	SheetFormatter formatter( writercfg );
	MPoint p;

	wxChar symbol = 0;
	SymbolProvider sp( writercfg );
	switch ( token.entity )
	{
		case TTSE_PLANET:
			if ( writercfg->planetSymbols ) symbol = sp.getPlanetCode( (ObjectId)token.entityId );
			if ( ! symbol ) s = formatter.getObjectNamePlain( (ObjectId)token.entityId, token.textFormat, token.vedic );
		break;
		case TTSE_SIGN:
			if ( writercfg->signSymbols ) symbol = sp.getSignCode( (Rasi)token.entityId );
			if ( ! symbol ) s = lang.getSignName( (Rasi)token.entityId, token.textFormat ); //, writercfg->vedicSignNames );
		break;
		case TTSE_ASPECT:
			symbol = SymbolProvider().getAspectCode( (ASPECT_TYPE)token.entityId );
			if ( ! symbol ) s = AspectExpert::getAspectShortDescription( (int)token.entityId );
		break;
		case TTSE_DIRECTION:
			symbol = sp.getRetroCode( (MOVING_DIRECTION)token.entityId );
			if ( ! symbol ) s = wxT( "R" );
		break;
		default:
			symbol = 0;
			s = token.text;
		break;
	}
	if ( symbol )
	{
		setSymbolFont();
		p = getTextExtent( symbol );
	}
	else
	{
		p = getTextExtent( s );
	}

	setFont( oldFont );
	return p;
}

/*****************************************************
**
**   Painter   ---   getTextExtent
**
******************************************************/
MPoint Painter::getTextExtent( const MString &f )
{
	double x = 0, y = 0;
	MPoint p;

	for( list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
	{
		p = getTextExtent( *iter );
		x += p.real();
		y = Max( y, p.imag());
	}
	return MPoint( x, y );
}

/*****************************************************
**
**   DcPainter   ---   Constructor
**
******************************************************/
DcPainter:: DcPainter( wxDC *d, WriterConfig *writercfg, ColorConfig *ccfg )
 : Painter( writercfg, ccfg )
{
	dc = d;
}

/*****************************************************
**
**   DcPainter   ---   setDefaults
**
******************************************************/
void DcPainter::setDefaults()
{
	setDefaultPen();
	setDefaultBrush();
	setTextColor( getDefaultTextColor());
}

/*****************************************************
**
**   DcPainter   ---   setBrush
**
******************************************************/
void DcPainter::setBrush( const MBrush &brush )
{
	//printf( "DcPainter::setBrush ok %d style %d color %s stipple file '%s'\n",
		//brush.IsOk(), brush.style, str2char( brush.color.GetAsString()), str2char( brush.filename ));
	if ( ! brush.IsOk() )
	{
		wxLogError( wxT( "DcPainter::setBrush: brush not okay, using default brush" ));
		setDefaultBrush();
	}
	else
	{
  switch ( brush.style )
  {
		case wxTRANSPARENT:
			dc->SetBrush( *wxTRANSPARENT_BRUSH );
			break;
		case wxSTIPPLE:
			{
				wxBitmap bmp = ImageProvider::get()->getFileBasedBitmap( brush.filename, brush.rotateHue );
				if ( bmp.IsOk() )
				{
					dc->SetBrush( wxBrush( bmp ));
				}
				else
				{
					wxLogError( wxT( "DcPainter::setBrush: bitmap for brush not okay, using red background instead" ));
					dc->SetBrush( wxBrush( *wxRED ));
				}
			}
			break;
		case wxSOLID:
		case wxBDIAGONAL_HATCH:
		case wxCROSSDIAG_HATCH:
		case wxFDIAGONAL_HATCH:
		case wxCROSS_HATCH:
		case wxHORIZONTAL_HATCH:
		case wxVERTICAL_HATCH:
				if ( brush.color.IsOk())
				{
					dc->SetBrush( wxBrush( brush.color, brush.style ));
				}
				else
				{
					if ( config->colors )
					{
						dc->SetBrush( wxBrush( config->colors->bgColor, brush.style ));
					}
					else
					{
						wxLogError( wxT( "DcPainter::setBrush: cannot set fallback background color, config is NULL, using white instead" ));
						dc->SetBrush( wxBrush( *wxWHITE ));
					}
				}
			break;
		default:
			//wxLogError( wxT( "DcPainter::setBrush: unknown brush style %s" ), brush.toString().c_str() );
			wxLogError( wxT( "DcPainter::setBrush: unknown brush style %s" ), brush.style );
			dc->SetBrush( wxBrush( *wxBLUE ));
			break;
		}
	}
}

/*****************************************************
**
**   DcPainter   ---   setTransparentBrush
**
******************************************************/
void DcPainter::setTransparentBrush()
{
	dc->SetBrush( *wxTRANSPARENT_BRUSH );
}

/*****************************************************
**
**   DcPainter   ---   getDefaultBrush
**
******************************************************/
MBrush DcPainter::getDefaultBrush()
{
	return MBrush( colorcfg->bgColor, wxSOLID );
}

/*****************************************************
**
**   DcPainter   ---   setPen
**
******************************************************/
void DcPainter::setPen( const wxPen &p )
{
	//printf( "DcPainter::setPen OK %d color OK %d\n", p.IsOk(), p.GetColour().IsOk());
	if ( ! p.IsOk() )
	{
		wxLogError( wxT( "pen not okay, using default pen" ));
		setDefaultPen();
	}
	else dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   getDefaultPen
**
******************************************************/
wxPen DcPainter::getDefaultPen()
{
	return wxPen( colorcfg->fgColor, 1, wxSOLID );
}

/*****************************************************
**
**   DcPainter   ---   getPenColor
**
******************************************************/
wxColour DcPainter::getPenColor()
{
	return colorcfg->fgColor;
}

/*****************************************************
**
**   DcPainter   ---   drawRectangle
**
******************************************************/
void DcPainter::drawRectangle( const double &x, const double &y, const double &w, const double &h, const double rnd )
{
	//printf( "DcPainter::drawRectangle x %f y %f w %f h %f rnd %f\n", x, y, w, h, rnd );
	if ( rnd == 0 )
		dc->DrawRectangle( a_rund( x ), a_rund( y ), a_rund( w ), a_rund( h ));
	else
		dc->DrawRoundedRectangle( a_rund( x ), a_rund( y ), a_rund( w ), a_rund( h ), rnd );
}

/*****************************************************
**
**   DcPainter   ---   drawPolygon
**
******************************************************/
void DcPainter::drawPolygon(int n, MPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	wxPoint *wxpoints = new wxPoint[n];

	for( int i  = 0; i < n; i++ )
	{
		wxpoints[i] = pointToWxPoint( points[i] );
	}
	dc->DrawPolygon( n, wxpoints, xoffset, yoffset );
	delete[] wxpoints;
}

/*****************************************************
**
**   DcPainter   ---   drawLine
**
******************************************************/
void DcPainter::drawLine( const double &x1, const double &y1, const double &x2, const double &y2 )
{
	dc->DrawLine( a_rund( x1 ), a_rund( y1 ), a_rund( x2 ), a_rund( y2 ));
}

/*****************************************************
**
**   DcPainter   ---   drawTextFormatted
**
******************************************************/
void DcPainter::drawTextFormatted( const MRect &r, const wxString &t, const int& align )
{
	if ( t.IsEmpty() ) return;

	//printf( "DcPainter::drawTextFormatted x %f y %f w %f h %f s %s\n", r.x, r.y, r.width, r.height, str2char( t ));
	wxCoord w, h;
	dc->GetTextExtent( t, &w, &h );
	int xmitte, ymitte, x, y;

	ymitte = 2 * (int)r.y + (int)r.height;
	ymitte /= 2;
	if ( align & Align::VCenter ) y = ymitte - h /2;
	else if ( align & Align::Top ) y = r.y;
	else y = r.y + r.height - h;

	xmitte = 2 * r.x + r.width;
	xmitte /= 2;
	if ( align & Align::HCenter ) x = xmitte - w /2;
	else if ( align & Align::Left ) x = r.x;
	else x = r.x + r.width - w;

	dc->DrawText( t, a_rund( x ), a_rund( y ));
}

/*****************************************************
**
**   DcPainter   ---   getTextExtent
**
******************************************************/
MPoint DcPainter::getTextExtent( const wxString &s )
{
	wxCoord w, h;
	dc->GetTextExtent( s, &w, &h );
	//printf( "DcPainter::getTextExtent w %d h %d %s\n", w, h, str2char( s ));
	return MPoint( w, h );
}

/*****************************************************
**
**   DcPainter   ---   drawEllipse
**
******************************************************/
void DcPainter::drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax )
{
	dc->DrawEllipse( a_rund(x0), a_rund(y0), a_rund(xmax), a_rund(ymax) );
}

/*****************************************************
**
**   DcPainter   ---   drawArc
**
******************************************************/
void DcPainter::drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
	const double &w1, const double &w2 )
{
	dc->DrawEllipticArc( a_rund(x0), a_rund(y0), a_rund(xmax), a_rund(ymax), a_rund(w1), a_rund( w2) );
}

/*****************************************************
**
**   DcPainter   ---   drawPoint
**
******************************************************/
void DcPainter::drawPoint( const double &x, const double &y )
{
	dc->DrawPoint( a_rund(x), a_rund(y) );
}

/*****************************************************
**
**   DcPainter   ---   setPenColor
**
******************************************************/
void DcPainter::setPenColor( const wxColour &c )
{
	if ( ! c.IsOk() )
	{
		wxLogError( wxT( "pen color not okay, using default pen" ));
		setDefaultPen();
	}
	else
	{
		wxPen currentPen = dc->GetPen();
		if ( currentPen.IsOk() )
		{
			dc->SetPen( wxPen( c, currentPen.GetWidth(), currentPen.GetStyle() ));
		}
		else dc->SetPen( wxPen( c, 1, wxSOLID ));
	}
}

/*****************************************************
**
**   DcPainter   ---   setPenWidth
**
******************************************************/
void DcPainter::setPenWidth( const int &i )
{
	wxPen p = dc->GetPen();
	p.SetWidth( i );
	dc->SetPen( p );
}

/*****************************************************
**
**   DcPainter   ---   setSymbolFont
**
******************************************************/
void DcPainter::setSymbolFont( const double zoom )
{
	currentFont = *FontProvider::get()->getFontZoom( FONT_GRAPHIC_SYMBOLS, zoom );
	dc->SetFont( currentFont );
}

/*****************************************************
**
**   DcPainter   ---   setGraphicFont
**
******************************************************/
void DcPainter::setGraphicFont( const double zoom )
{
	currentFont = *FontProvider::get()->getFontZoom( FONT_GRAPHIC_DEFAULT, zoom );
	dc->SetFont( currentFont );
}

/*****************************************************
**
**   DcPainter   ---   setTextColor
**
******************************************************/
void DcPainter::setTextColor( const wxColour& c )
{
	dc->SetTextForeground( c );
}

/*****************************************************
**
**   DcPainter   ---   getDefaultTextColor
**
******************************************************/
wxColour DcPainter::getDefaultTextColor()
{
	return wxColour( colorcfg->fgColor );
}

/*****************************************************
**
**   DcPainter   ---   setTextColor
**
******************************************************/
void DcPainter::setTextBackgroundColor( const wxColour &c )
{
	dc->SetTextBackground( c );
}

/*****************************************************
**
**   DcPainter   ---   drawSignSymbol
**
******************************************************/
void DcPainter::drawSignSymbol( const int&x, const int &y, const Rasi &sign, const int type, const int /* zoom */, const double angle )
{
	Lang lang;
	SymbolProvider sp;
	MPoint p;
	wxString s;

	switch( type )
	{
		case 1:
			s = lang.getSignName( sign, TF_SHORT );
		break;
		case 2:
			s = lang.getSignName( sign, TF_MEDIUM );
		break;
		case 3:
			s = lang.getSignName( sign, TF_LONG );
		break;
		case 0:
		default:
			s = sp.getSignCode( sign );
		break;
	}

	p = getTextExtent( s );
	//printf( "DcPainter::drawSignSymbol Text extent w %f h %f\n", w, h );

	setTransparentBrush();
	setDefaultPen();

	/*
	drawRectangle( x-3, y-3, 6, 6 );
	const int aaa = 15;
	drawEllipse( x-aaa, y-aaa, 2 * aaa, 2 * aaa );
	//drawRectangle( x-10, y-10, 20, 20 );
	*/

	double x3 = x - .5 * p.real() * cos( 1.75 * PI + angle * DEG2RAD );
	double y3 = y + .5 * p.imag() * sin( 1.75 * PI+ angle * DEG2RAD );

	if ( angle )
	{
		dc->DrawRotatedText( s, a_rund( x3 ), a_rund( y3 ), angle );
	}
	else
	{
		dc->DrawText( s, a_rund( x3 ), a_rund( y3 ) );
	}
	//dc->DrawRotatedText( s, a_rund( x3 ), a_rund( y3 ), angle );
}

/*****************************************************
**
**   DcPainter   ---   drawBitmap
**
******************************************************/
void DcPainter::drawBitmap( const wxBitmap &bitmap, const double &x0, const double &y0, const bool &transparent )
{
	dc->DrawBitmap( bitmap, a_rund( x0 ), a_rund( y0 ), transparent );
}

/*****************************************************
**
**   DcPainter   ---   getTextZoomFactor
**
******************************************************/
double DcPainter::getTextZoomFactor( const double &totalsize )
{
	/*
	// 400 (minimum of width and height) pixel is 100%, means e.g. 12pt font will have 12pt
	double scalingfactor = totalsize / 400.0;
	//return totalsize / scalingfactor;

	double ret = 100.0 * scalingfactor;

	//printf( "DcPainter::getTextZoomFactor totalsize %f scalingfactor %f ret %f\n", totalsize, scalingfactor, ret  );
	return ret;
*/
	return totalsize / 400.0;
}

/*****************************************************
**
**   DcPainter   ---   setFont
**
******************************************************/
void DcPainter::setFont( const wxFont &font )
{
	currentFont = font;
	dc->SetFont( font );
}


