/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfPainter.cpp
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

#include "PdfPainter.h"
#include "PdfBase.h"

#include "Conf.h"
#include "maitreya.h"
#include "FontProvider.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "Sheet.h"
#include "SymbolProvider.h"

#include <wx/log.h>

extern Config *config;

//#define PDF_TEXT_ZOOMFACTOR 1
#define PDF_TEXT_ZOOMFACTOR .1
#define PDF_DEFAULT_FGCOLOR *wxBLACK
#define PDF_DEFAULT_BGCOLOR *wxWHITE
#define PDF_PEN_PENWIDTH_FACTOR .5

/*****************************************************
**
**   PdfPainter   ---   Constructor
**
******************************************************/
PdfPainter::PdfPainter( BasePdfDocument *pdfdoc )
{
	pdf = pdfdoc;
	drawmode = wxPDF_STYLE_DRAW;

	defaultLineStyle = new wxPdfLineStyle( pdf->GetLineStyle());
	defaultTextColor = new wxPdfColour( pdf->GetTextColour() );
}

/*****************************************************
**
**   PdfPainter   ---   Destructor
**
******************************************************/
PdfPainter::~PdfPainter()
{
	delete defaultLineStyle;
	delete defaultTextColor;
}

/*****************************************************
**
**   PdfPainter   ---   setTransparentBrush
**
******************************************************/
void PdfPainter::setTransparentBrush()
{
	setBrush( MBrush( *wxBLACK, wxTRANSPARENT ));
}

/*****************************************************
**
**   PdfPainter   ---   setBrush
**
******************************************************/
void PdfPainter::setBrush( const MBrush &b )
{
	// TODO howto scale width und height 
	if ( b.style == wxSOLID )
	{
		if ( b.color.IsOk() )
		{
			drawmode = wxPDF_STYLE_FILL;
			pdf->SetFillColour( b.color );
		}
		else wxLogError( wxT( "bitmap in PdfPainter::setBrush is not transparent but color is not okay" ));
	}
	else if ( b.style == wxSTIPPLE )
	{
		wxImage image;
		//wxBitmap *bitmap = b.GetStipple();
		wxBitmap bitmap = ImageProvider::get()->getFileBasedBitmap( b.filename, b.rotateHue );
		if ( bitmap.IsOk())
		{
			image = bitmap.ConvertToImage();

			double width = image.GetWidth() / 3;
			double height = image.GetHeight() / 3;

			// setup a hash value for the image
			wxString s = createImageHash( &image );

			pdf->AddPattern( s, image, width, height );
			drawmode = wxPDF_STYLE_FILL;
			pdf->SetFillPattern( s );
			pdf->SetDrawPattern( s );
		}
		else wxLogError( wxT( "bitmap in PdfPainter::setBrush not okay" ));
	}
	else if ( b.style != wxTRANSPARENT )
	{
		// TODO SetFillPattern( wxString name )
		if ( b.color.IsOk() )
		{
			drawmode = wxPDF_STYLE_FILL;
			pdf->SetFillColour( b.color );
		}
		else wxLogError( wxT( "bitmap in PdfPainter::setBrush is not transparent but color is not okay" ));
		//setBrush( b.GetColour() );
	}
	else // must be transparent
	{
		drawmode = wxPDF_STYLE_DRAW;
	}
}

wxColour colorToGreyScale( const wxColour&c )
{
	int g = ( c.Red() + c.Green() + c.Blue()) / 3.0;
	return wxColour( g, g, g );
}

/*****************************************************
**
**   PdfPainter   ---   getDefaultBrush
**
******************************************************/
MBrush PdfPainter::getDefaultBrush()
{
	return MBrush( PDF_DEFAULT_BGCOLOR, wxTRANSPARENT );
}

/*****************************************************
**
**   PdfPainter   ---   setPen
**
******************************************************/
void PdfPainter:: setPen( const wxPen &p )
{
	const double defaultWidth = 1;

	double width = p.GetWidth() * PDF_PEN_PENWIDTH_FACTOR;

	wxPdfLineStyle style;
	switch ( p.GetStyle() )
	{
		case wxDOT:
		{
			wxPdfArrayDouble dash1;
			dash1.Add( .5 );
			style = wxPdfLineStyle( width, wxPDF_LINECAP_NONE, wxPDF_LINEJOIN_MITER, dash1, 10.);
		}
		break;
		case wxLONG_DASH:
		{
			wxPdfArrayDouble dash2;
			dash2.Add( 3 );
			style  = wxPdfLineStyle( width, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash2, 0.);
			break;
		}
		break;
		case wxSHORT_DASH:
		{
			wxPdfArrayDouble dash3;
			dash3.Add( 1.5 );
			style  = wxPdfLineStyle( width, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash3, 0.);
		}
		case wxDOT_DASH:
		{
			wxPdfArrayDouble dash4;
			dash4.Add( 1.5 );
			dash4.Add( 3 );
			style  = wxPdfLineStyle( width, wxPDF_LINECAP_BUTT, wxPDF_LINEJOIN_MITER, dash4, 0.);
		}
		break;
		default:
		// Noting to do
		break;
	}

	style.SetWidth( width );
	if ( p.GetColour().IsOk() )
	{
		style.SetColour( p.GetColour() );
		pdf->SetDrawColour( p.GetColour());
	}
	pdf->SetLineStyle( style );
}

/*****************************************************
**
**   PdfPainter   ---   setPenColor
**
******************************************************/
void PdfPainter::setPenColor( const wxColour &c )
{
	wxPdfLineStyle style = pdf->GetLineStyle();
	style.SetColour( c );
	pdf->SetLineStyle( style );
}

/*****************************************************
**
**   PdfPainter   ---   getPenColor
**
******************************************************/
wxColour PdfPainter::getPenColor()
{
	return PDF_DEFAULT_FGCOLOR;
}

/*****************************************************
**
**   PdfPainter   ---   getDefaultPen
**
******************************************************/
wxPen PdfPainter::getDefaultPen()
{
	return wxPen( PDF_DEFAULT_FGCOLOR, 1, wxSOLID );
}

/*****************************************************
**
**   PdfPainter   ---   setDefaults
**
******************************************************/
void PdfPainter::setDefaults()
{
	setPen( getDefaultPen());
	setBrush( getDefaultBrush());
	pdf->SetTextColour( PDF_DEFAULT_FGCOLOR );
}

/*****************************************************
**
**   PdfPainter   ---   setGraphicFont
**
******************************************************/
void PdfPainter::setGraphicFont( const double zoom )
{
	currentFont = *FontProvider::get()->getFont( FONT_GRAPHIC_DEFAULT, zoom );
	pdf->SetFont( currentFont );
}

/*****************************************************
**
**   PdfPainter   ---   setSymbolFont
**
******************************************************/
void PdfPainter::setSymbolFont( const double zoom )
{
	currentFont = *FontProvider::get()->getFont( FONT_GRAPHIC_SYMBOLS, zoom );
	pdf->SetFont( currentFont );
}

/*****************************************************
**
**   PdfPainter   ---  setTextColor
**
******************************************************/
void PdfPainter::setTextColor( const wxColour &c )
{
	pdf->SetTextColour( c );
}

/*****************************************************
**
**   PdfPainter   ---  getDefaultTextColor
**
******************************************************/
wxColour PdfPainter::getDefaultTextColor()
{
	return PDF_DEFAULT_FGCOLOR;
}

/*****************************************************
**
**   PdfPainter   ---  getTextExtent
**
******************************************************/
MPoint PdfPainter::getTextExtent( const wxString &s )
{
	const double w = pdf->GetStringWidth( s );
	const double h = pdf->GetFontSize();
	//printf( "PdfPainter::getTextExtent w %f h %f\n", w, h );
	return MPoint( w, h );
}

/*****************************************************
**
**   PdfPainter   ---   getTextZoomFactor
**
******************************************************/
double PdfPainter::getTextZoomFactor( const double& )
{
	return PDF_TEXT_ZOOMFACTOR;
}

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const double &x, const double &y, const double &w, const double &h, const double rnd )
{
	//const double r = .3;
	const double r = 1.0;
	if ( rnd )
	{
		pdf->RoundedRect( x, y, w, h, r * rnd, wxPDF_CORNER_ALL, drawmode );
	}
	else
	{
		pdf->Rect( x, y, w, h, drawmode );
	}
}

/*****************************************************
**
**   PdfPainter   ---  drawRectangle
**
******************************************************/
void PdfPainter::drawRectangle( const wxRect &rect )
{
	drawRectangle( rect.x, rect.y, rect.width, rect.height );
}

/*****************************************************
**
**   PdfPainter   ---   drawPolygon
**
******************************************************/
void PdfPainter::drawPolygon(int n, MPoint points[], wxCoord xoffset, wxCoord yoffset )
{
	wxPdfArrayDouble x, y;
	for ( int i = 0; i < n; i++ )
	{
		x.Add( points[i].real() + xoffset );
		y.Add( points[i].imag() + yoffset );
	}
	pdf->Polygon( x, y, drawmode );
}

/*****************************************************
**
**   PdfPainter   ---  drawLine
**
******************************************************/
void PdfPainter::drawLine( const double &x1, const double &y1, const double &x2, const double &y2 )
{
	pdf->Line( x1, y1, x2, y2 );
}

/*****************************************************
**
**   PdfPainter   ---  drawTextFormatted
**
******************************************************/
void PdfPainter::drawTextFormatted( const MRect &r, const wxString &t, const int& align )
{
	double oldx = pdf->GetX();
	double oldy = pdf->GetY();
	double x1 = r.x;
	double y1 = r.y;

	pdf->SetXY( r.x, y1 );

	MPoint p = getTextExtent( t );

	if ( align & Align::Top )
	{
		y1 = r.y + r.height; // + .5 * sh;
	}
	else if ( align & Align::Bottom )
	{
		y1 = r.y + r.height; // + .5 * sh;
	}
	else if ( align & Align::VCenter )
	{
		y1 = r.y + .5 * ( r.height + p.imag() );
	}
	if ( align & Align::HCenter )
	{
		x1 = r.x + .5 * ( r.width - p.real() );
	}
	else if ( align & Align::Right )
	{
		x1 = r.x + r.width - p.real();
	}
	pdf->Text( x1, y1, t );
	//drawEllipse( x1, y1, 2, 2 );
	pdf->SetXY( oldx, oldy );
}

/*****************************************************
**
**   PdfPainter   ---   drawEllipse
**
******************************************************/
void PdfPainter::drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2 ); // Cannot be filled
}

/*****************************************************
**
**   PdfPainter   ---   drawArc
**
******************************************************/
void PdfPainter::drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
                          const double &w1, const double &w2 )
{
	pdf->Ellipse( x0 + xmax/2 , y0 + ymax/2, xmax/2, ymax/2, 0, w1, w2 );
}

/*****************************************************
**
**   PdfPainter   ---   drawPoint
**
******************************************************/
void PdfPainter::drawPoint( const double &x, const double &y )
{
	pdf->Line( x, y, x, y );
}

/*****************************************************
**
**   PdfPainter   ---   drawSignSymbol
**
******************************************************/
void PdfPainter::drawSignSymbol( const int&x, const int &y, const Rasi &sign, const int type, const int /* zoom */, const double angle )
{
	SymbolProvider sp;
	wxString s;
	Lang lang;

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
	MPoint p = getTextExtent( s );


	/*
	setTransparentBrush();
	setDefaultPen();
	drawRectangle( x-3, y-3, 6, 6 );
	drawRectangle( x-10, y-10, 20, 20 );
	*/

	double x4 = x - .5 * p.real() * cos( .25 * PI + angle * DEG2RAD );
	double y4 = y + .5 * p.imag() * sin( .25 * PI + angle * DEG2RAD );

	pdf->RotatedText( x4, y4, s, angle );
}

/*****************************************************
**
**   PdfPainter   ---   setFont
**
******************************************************/
void PdfPainter::setFont( const wxFont &font )
{
	//printf( "PdfPainter::setFont size is %d\n", font.GetPointSize());
	currentFont = font;
	pdf->SetFont( font );
}

