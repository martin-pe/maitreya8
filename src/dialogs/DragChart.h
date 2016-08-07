/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DragChart.h
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

#ifndef DRAGCHART_H
#define DRAGCHART_H

#include <wx/bitmap.h>
#include <wx/log.h>
#include <wx/scrolwin.h>

#include "maitreya.h"

class wxDragImage;
class wxImage;
class wxTimer;
class wxTimerEvent;

class MinimalVargaHoroscope;
class DragShape;
class Painter;

/*************************************************//**
*
* \brief Drag an drop chart
*
******************************************************/
class DragChart : public wxScrolledWindow
{
	DECLARE_CLASS( DragChart )
public:

	DragChart( wxWindow* parent, wxWindowID id = wxID_ANY, MinimalVargaHoroscope *chart = 0 );
	~DragChart();

	void clearChart();
	void addObject( const ObjectId& );

	void echo( wxString );

private:

	void OnPaint( wxPaintEvent& );
	void OnTimer( wxTimerEvent& );

	void OnMouseLeftDown( wxMouseEvent& );
	void OnMouseLeftUp( wxMouseEvent& );
	void OnMouseDrag( wxMouseEvent& );

	void DrawShapes( wxDC& );
	void EraseShape( DragShape*, wxDC& );
	void ClearShapes();
	void drawChart();
	wxRect getRasiCoord( const int &r );

	DragShape* FindShape( const wxPoint& ) const;

	Painter *painter;
	MinimalVargaHoroscope *chart;
	int dragMode, lwidth, rwidth, height, xtol, ytol, xstep, ystep;

	wxList displayList;
	DragShape *draggedShape;
	wxPoint dragStartPos;
	wxDragImage * dragImage;
	wxString centerString;
	wxTimer *msgtimer;
};

/*************************************************//**
*
* \brief Drag and drop object
*
******************************************************/
class DragShape: public wxObject
{
public:
	DragShape(const ObjectId& );
	~DragShape();

	bool HitTest( const wxPoint& pt ) const;
	bool Draw( wxDC& );

	wxPoint GetPosition() const { return pos; }
	void SetPosition( const wxPoint& p ) { pos = p; }

	wxPoint getMidpoint() const;

	wxRect GetRect() const { return wxRect( pos.x, pos.y, bitmap.GetWidth(), bitmap.GetHeight() ); }

	wxBitmap& GetBitmap() const { return (wxBitmap&)bitmap; }

	bool IsShown() const { return show; }
	void SetShow(bool s ) { show = s; }

	void setRasipos( const int r ) { rasipos = r; }
	int getRasipos() const { return rasipos; }

	ObjectId getObjectId() const { return objectid; }

	void setHighlight( const bool &b ) { highlight = b; }

protected:
	wxPoint pos;
	wxBitmap bitmap;
	bool show;
	bool highlight;
	int rasipos;
	ObjectId objectid;

	bool createImage();

};


#endif // DRAGCHART_H
