/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfPainter.h
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

	// text
	virtual void drawTextFormatted( const MRect&, const wxString &t, const int& align );
	virtual MPoint getTextExtent( const wxString &s );
	virtual void drawSignSymbol( const int&x, const int &y, const Rasi&, const int type = 0, const int zoom = 100, const double angle = 0 );

	// fonts and text properties
	virtual void setSymbolFont( const double = 1.0 );
	void setGraphicFont( const double = 1.0 );
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


