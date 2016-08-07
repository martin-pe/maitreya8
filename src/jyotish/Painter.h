/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Painter.h
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

#ifndef _PAINTER_H_
#define _PAINTER_H_

#include "GraphConfig.h"
#include <wx/font.h>
#include <wx/gdicmn.h>
#include <wx/string.h>


class ColorConfig;
class DasaColorConfig;
class MString;
class MToken;
class WriterConfig;
class wxBitmap;
class wxColour;
class wxDC;

/*************************************************//**
*
* \brief abstract class for paiting graphical items and text on a GUI
*
******************************************************/
class Painter
{
public:

	Painter( WriterConfig* = (WriterConfig*)NULL, ColorConfig* = (ColorConfig*)NULL  );
	virtual ~Painter() {}

	virtual void setDefaults() = 0;

	// brush
	virtual void setTransparentBrush() = 0;
	virtual MBrush getDefaultBrush() = 0;
	virtual void setBrush( const MBrush& ) = 0;
	void setDefaultBrush() { setBrush( getDefaultBrush()); }

	// pen
	virtual void setPen( const wxPen& ) = 0;
	void setTransparentPen();
	virtual void setPenColor( const wxColour& ) = 0;
	virtual void setPenWidth( const int& ) {}
	void setDefaultPen() { setPen( getDefaultPen()); }
	virtual wxColour getPenColor() = 0;
	virtual wxPen getDefaultPen() = 0;

	// drawing methods
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 ) = 0;
	void drawRectangle( const MRect&, const double = 0 );
	virtual void drawPolygon(int n, MPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 ) = 0;
	virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 ) = 0;
	void drawLine( const MPoint &p1, const MPoint &p2 );
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax ) = 0;
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
		const double &w1, const double &w2 ) = 0;
	virtual void drawPoint( const double &x, const double &y ) = 0;

	// TODO not implemented by PdfPainter, required for chart drawing (background images)
	virtual void drawBitmap( const wxBitmap&, const double&, const double&, const bool& ) {}

	// text
	virtual void drawTextFormatted( const MRect&, const wxString&, const int &align ) = 0;
	void drawMString( const MRect&, MString&, const int &align );

	virtual MPoint getTextExtent( const wxString& ) = 0;
	MPoint getTextExtent( const MToken& );
	MPoint getTextExtent( const MString& );

	virtual void drawSignSymbol( const int&x, const int &y, const Rasi&, const int type = 0, const int zoom = 100,
		const double angle = 0 ) = 0;

	// fonts and text properties
	void setDefaultTextColor();
	virtual void setTextColor( const wxColour& ) = 0;
	virtual wxColour getDefaultTextColor() = 0;
	virtual void setTextBackgroundColor( const wxColour& ) = 0;
	virtual void setSymbolFont( const double = 1.0 ) = 0;
	virtual void setGraphicFont( const double = 1.0 ) = 0;
	virtual double getTextZoomFactor( const double &totalsize ) = 0;
	virtual void setFont( const wxFont& ) = 0;
	wxFont getCurrentFont() const { return currentFont; }

	WriterConfig *writercfg;
	ColorConfig *colorcfg;

protected:

	void drawSingleMStringLine( const MRect &r, MString &f, const int& align );

	wxFont currentFont;
};


/*************************************************//**
*
* \brief Painter implementation for wx device context (GUI and file export via memory DC)
*
******************************************************/
class DcPainter: public Painter
{
public:
	DcPainter( wxDC*, WriterConfig* = (WriterConfig*)NULL, ColorConfig* = (ColorConfig*)NULL );

	virtual void setDefaults();

	// brush
	virtual void setTransparentBrush();
	virtual void setBrush( const MBrush& );
	MBrush getDefaultBrush();

	// pen
	virtual void setPen( const wxPen& );
	virtual void setPenColor( const wxColour& );
	virtual void setPenWidth( const int& );
	virtual wxPen getDefaultPen();
	virtual wxColour getPenColor();

	// drawing methods
	virtual void drawRectangle( const double &x, const double &y, const double &w, const double &h, const double = 0 );
	virtual void drawPolygon(int n, MPoint points[], wxCoord xoffset = 0, wxCoord yoffset = 0 );
	virtual void drawLine( const double &x1, const double &y1, const double &x2, const double &y2 );
	virtual void drawEllipse( const double &x0, const double &y0, const double &xmax, const double &ymax );
	virtual void drawArc( const double &x0, const double &y0, const double &xmax, const double &ymax,
		const double &w1, const double &w2 );
	virtual void drawPoint( const double &x, const double &y );
	virtual void drawBitmap( const wxBitmap &bitmap, const double &x0, const double &y0, const bool &transparent );

	// text
	virtual void drawTextFormatted( const MRect&, const wxString&, const int &align );
	virtual MPoint getTextExtent( const wxString& );
	virtual void drawSignSymbol( const int&x, const int &y, const Rasi&, const int type = 0, const int zoom = 100,
		const double angle = 0 );

	// fonts and text properties
	virtual void setSymbolFont( const double = 1.0 );
	virtual void setGraphicFont( const double = 1.0 );
	virtual void setTextColor( const wxColour& );
	virtual wxColour getDefaultTextColor();
	virtual void setTextBackgroundColor( const wxColour& );
	virtual double getTextZoomFactor( const double &totalsize );
	virtual void setFont( const wxFont& );

private:

	wxDC *dc;
};


#endif

