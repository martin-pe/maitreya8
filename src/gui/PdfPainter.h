/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfPainter.h
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

#ifndef _PDFPAINTER_H_
#define _PDFPAINTER_H_

#include "Painter.h"

#include <wx/colour.h>
#include <wx/string.h>

class BasePdfDocument;

class wxPdfColour;
class wxPdfLineStyle;

/*************************************************//**
*
* \brief painter implementation for PDF printout
*
******************************************************/
class PdfPainter : public Painter
{
public:
	PdfPainter( BasePdfDocument* );

	virtual ~PdfPainter();

	virtual void setDefaults();

	// Brush
	virtual void setTransparentBrush();
	virtual void setBrush( const MBrush& );
	virtual MBrush getDefaultBrush();

	// Pen
	virtual void setPen( const wxPen& );
	virtual void setPenColor( const wxColour& );
	virtual wxPen getDefaultPen();
	virtual wxColour getPenColor();

	// drawing methods
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 );
	virtual void drawRectangle( const wxRect& );

	virtual void drawPolygon(int n, MPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 );
	virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 );
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax );
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
	                      const double &w1, const double &w2 );
	virtual void drawPoint( const double &x, const double &y );
	virtual void drawBitmap( const wxBitmap&, const double&, const double&, const bool& );

	// text
	virtual void drawTextFormatted( const MRect&, const wxString &t, const int& align );
	virtual MPoint getTextExtent( const wxString &s );
	virtual void drawRotatedText( wxString /*text*/, const double& /*x*/, const double& /*y*/, const double& /*angle*/ );


	// fonts and text properties
	virtual void setTextBackgroundColor( const wxColour& ) {}
	virtual void setTextColor( const wxColour& );
	virtual wxColour getDefaultTextColor();
	virtual double getTextZoomFactor( const double &totalsize );
	virtual void setFont( const wxFont& );

protected:

	int drawmode;

	wxString symbolFontConfigName;

	wxPdfLineStyle *defaultLineStyle;
	wxPdfColour *defaultTextColor;

private:
	BasePdfDocument *pdf;
};

#endif


