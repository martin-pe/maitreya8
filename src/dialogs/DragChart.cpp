/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DragChart.cpp
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

//#define wxUSE_GENERIC_DRAGIMAGE 1

#include <wx/app.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dcmemory.h>
#include <wx/image.h>
#include <wx/log.h>
#include <wx/timer.h>

#ifndef __WXMSW__
#include "wx/generic/dragimgg.h"
#define wxDragImage wxGenericDragImage
#else
#include "wx/dragimag.h"
#endif


#include "DragChart.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "Lang.h"
#include "Painter.h"
#include "Lang.h"
#include "SymbolProvider.h"
#include "VargaHoroscope.h"


extern Config *config;

// size of drag images
#define WDRAGIMAGE 24
#define HDRAGIMAGE 32

// Dragging modes
#define TEST_DRAG_NONE     0
#define TEST_DRAG_START    1
#define TEST_DRAG_DRAGGING 2

#define MESSAGE_TIMEOUT 1000

enum { MESSAGE_TIMER = wxID_HIGHEST + 2500 };


IMPLEMENT_CLASS( DragChart, wxScrolledWindow )

/*****************************************************
**
**   DragChart   ---   Constructor
**
******************************************************/
DragChart::DragChart( wxWindow* parent, wxWindowID id, MinimalVargaHoroscope *chart )
		: wxScrolledWindow( parent, id, wxDefaultPosition, wxDefaultSize, wxHSCROLL | wxVSCROLL | wxFULL_REPAINT_ON_RESIZE ), chart( chart )
{
	SetBackgroundColour( *wxWHITE );
	msgtimer = new wxTimer( this, MESSAGE_TIMER );

	addObject( OASCENDANT );
	for ( ObjectId i = OSUN ; i <= OSATURN; i++ )
	{
		addObject( i );
	}
	addObject( OMEANNODE );
	addObject( OMEANDESCNODE );

	SetCursor( wxCursor( wxCURSOR_ARROW ));
	dragMode = TEST_DRAG_NONE;
	draggedShape = 0;
	dragImage = 0;

	Connect( wxEVT_PAINT, wxPaintEventHandler( DragChart::OnPaint ));
	Connect( MESSAGE_TIMER, wxEVT_TIMER, wxTimerEventHandler( DragChart::OnTimer ));
	Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( DragChart::OnMouseLeftDown ));
	Connect( wxEVT_LEFT_UP, wxMouseEventHandler( DragChart::OnMouseLeftUp ));
	Connect( wxEVT_MOTION, wxMouseEventHandler( DragChart::OnMouseDrag ));
}

/*****************************************************
**
**   DragChart   ---   Destructor
**
******************************************************/
DragChart::~DragChart()
{
	ClearShapes();
	if ( dragImage ) delete dragImage;
}

/*****************************************************
**
**   DragChart   ---   addObject
**
******************************************************/
void DragChart::addObject( const ObjectId &id )
{
	displayList.Append( new DragShape( id ));
}

/*****************************************************
**
**   DragChart   ---   clearChart
**
******************************************************/
void DragChart::clearChart()
{
	wxList::compatibility_iterator node = displayList.GetFirst();
	while ( node )
	{
		DragShape* shape = (DragShape*) node->GetData();
		shape->setRasipos( -1 );
		node = node->GetNext();
	}
}

/*****************************************************
**
**   DragChart   ---   OnPaint
**
******************************************************/
void DragChart::OnPaint( wxPaintEvent& )
{
	int i;
	int planetsPerSign[12], signStepLength[12];
	wxList::compatibility_iterator node;

	wxCoord w, h;
	GetSize( &w, &h );

	lwidth = 2 * w / 3;
	rwidth = w - lwidth;
	height = h;
	xtol = 3;
	ytol = 3;
	xstep = ( lwidth - ( 2 * xtol )) / 4;
	ystep = ( height - ( 2 * ytol )) / 4;

	chart->cleanup();

	wxPaintDC dc( this );
	PrepareDC( dc );

	painter = new DcPainter( &dc );

	painter->setDefaultBrush();
	painter->setDefaultPen();
	painter->drawRectangle( 0, 0, w, h );

	drawChart();

	// reset variables
	for ( i = R_ARIES; i <= R_PISCES; i++ )
	{
		planetsPerSign[i] = 0;
		signStepLength[i] = WDRAGIMAGE;
	}

	// calculate number of planets per sign
	for ( i = R_ARIES; i <= R_PISCES; i++ )
	{
		node = displayList.GetFirst();
		while ( node )
		{
			DragShape* shape = (DragShape*) node->GetData();
			if ( shape->getRasipos() == i )
			{
				planetsPerSign[i]++;
			}
			node = node->GetNext();
		}
	}

	// calculate step length per sign
	for ( i = R_ARIES; i <= R_PISCES; i++ )
	{
		wxRect rasirect = getRasiCoord( i );
		if ( planetsPerSign[i] * WDRAGIMAGE > rasirect.width )
		{
			signStepLength[i] = rasirect.width / planetsPerSign[i];
		}
	}

	for ( i = -1; i < 12; i++ )
	{
		wxRect rasirect = getRasiCoord( i );
		int xleft = rasirect.x;
		int ytop = rasirect.y;
		node = displayList.GetFirst();
		while ( node )
		{
			DragShape* shape = (DragShape*) node->GetData();
			if ( shape->getRasipos() == i )
			{
				chart->setRasi( (ObjectId)shape->getObjectId(), (Rasi)i );
				shape->SetPosition( wxPoint( xleft, ytop ));

				if ( i >= R_ARIES ) xleft += signStepLength[i];
				else
				{
					xleft += WDRAGIMAGE;
					if ( xleft + WDRAGIMAGE > rasirect.x + rasirect.width )
					{
						xleft = rasirect.x;
						ytop += HDRAGIMAGE;
					}
				}
			}
			node = node->GetNext();
		}
	}
	DrawShapes( dc );

	if ( ! centerString.IsEmpty())
	{
		painter->drawTextFormatted( MRect( xtol, ytol, lwidth, height ), centerString, Align::Center );
	}

	delete painter;
	painter = 0;
}

/*****************************************************
**
**   DragChart   ---   drawChart
**
******************************************************/
void DragChart::drawChart()
{
	assert( painter );

	painter->drawRectangle( xtol, ytol, lwidth - 2 * xtol, height - 2 * ytol );
	painter->drawLine( xtol, ytol + ystep, lwidth - 2 * xtol, ytol + ystep );
	painter->drawLine( xtol, ytol + 2 * ystep, xstep, ytol + 2 * ystep );
	painter->drawLine( xtol + 3 * xstep, ytol + 2 * ystep, xtol + 4 * xstep, ytol + 2 *ystep );
	painter->drawLine( xtol, ytol + 3 * ystep, lwidth - 2 * xtol, ytol + 3 * ystep );

	painter->drawLine( xtol + xstep, ytol, xtol + xstep, height - 2 * ytol );
	painter->drawLine( xtol + 2 * xstep, ytol, xtol + 2 * xstep, ytol + ystep );
	painter->drawLine( xtol + 2 * xstep, ytol + 3 * ystep, xtol + 2 * xstep, ytol + 4 * ystep );
	painter->drawLine( xtol + 3 * xstep, ytol, xtol + 3 * xstep, height - 2 * ytol );

	wxRect r = getRasiCoord( -1 );
	painter->drawRectangle( r.x, r.y, r.width, r.height );
}

/*****************************************************
**
**   DragChart   ---   getRasiCoord
**
******************************************************/
wxRect DragChart::getRasiCoord( const int &r )
{
	assert( r >= -1 && r < 12 );
	if ( r == R_NONE ) return wxRect( lwidth + xtol, ytol, rwidth - 2 * xtol, height - 2 * ytol );
	const int coords[12][2] = {
		{ 1, 0 },
		{ 2, 0 },
		{ 3, 0 },
		{ 3, 1 },
		{ 3, 2 },
		{ 3, 3 },
		{ 2, 3 }, // Libra
		{ 1, 3 },
		{ 0, 3 },
		{ 0, 2 },
		{ 0, 1 },
		{ 0, 0 }
	};
	return wxRect( xtol + coords[r][0] * xstep, ytol + coords[r][1] * ystep, xstep, ystep );
}

/*****************************************************
**
**   DragChart   ---   echo
**
******************************************************/
void DragChart::echo( wxString message )
{
	if ( msgtimer->IsRunning()) msgtimer->Stop();
	centerString = message;

	if ( ! message.IsEmpty()) msgtimer->Start( MESSAGE_TIMEOUT, true );
	Refresh();
}

/*****************************************************
**
**   DragChart   ---   OnTimer
**
******************************************************/
void DragChart::OnTimer( wxTimerEvent& )
{
	if ( msgtimer->IsRunning()) msgtimer->Stop();
	centerString.Clear();
	Refresh();
}

/*****************************************************
**
**   DragChart   ---   OnMouseLeftDown
**
******************************************************/
void DragChart::OnMouseLeftDown( wxMouseEvent &event )
{
	DragShape* shape = FindShape( event.GetPosition() );
	if ( shape )
	{
		// We tentatively start dragging, but wait for
		// mouse movement before dragging properly.

		dragMode = TEST_DRAG_START;
		dragStartPos = event.GetPosition();
		draggedShape = shape;
		draggedShape->setHighlight( false );
	}
}

/*****************************************************
**
**   DragChart   ---   OnMouseLeftUp
**
******************************************************/
void DragChart::OnMouseLeftUp( wxMouseEvent &event )
{
	if ( dragMode != TEST_DRAG_NONE )
	{
		// Finish dragging
		dragMode = TEST_DRAG_NONE;
		if ( ! draggedShape || ! dragImage) return;

		draggedShape->setRasipos( -1 );
		draggedShape->setHighlight( false );
		wxPoint droppoint = draggedShape->getMidpoint()+ event.GetPosition() - dragStartPos;

		for ( int i = R_ARIES; i <= R_PISCES; i++ )
		{
			if ( getRasiCoord( i ).Contains( droppoint ))
			{
				draggedShape->setRasipos( i );
				//draggedShape->setHighlight( true );
				break;
			}
		}

		dragImage->Hide();
		dragImage->EndDrag();
		delete dragImage;
		dragImage = 0;

		draggedShape->SetShow( true );
		draggedShape = 0;
		Refresh( true );
	}
}

/*****************************************************
**
**   DragChart   ---   OnMouseDrag
**
******************************************************/
void DragChart::OnMouseDrag( wxMouseEvent &event )
{
	if ( dragMode != TEST_DRAG_NONE )
	{
		if ( dragMode == TEST_DRAG_START )
		{
			// We will start dragging if we've moved beyond a couple of pixels
			const int tolerance = 2;
			const int dx = abs(event.GetPosition().x - dragStartPos.x);
			const int dy = abs(event.GetPosition().y - dragStartPos.y);
			if ( dx <= tolerance && dy <= tolerance ) return;

			// Start the drag.
			dragMode = TEST_DRAG_DRAGGING;

			if ( dragImage ) delete dragImage;

			// Erase the dragged shape from the canvas
			draggedShape->SetShow( false );

			// redraw immediately
			Refresh( true );
			Update();

			dragImage = new wxDragImage( draggedShape->GetBitmap(), wxCursor( wxCURSOR_HAND ));

			// The offset between the top-left of the shape image and the current shape position
			wxPoint beginDragHotSpot = dragStartPos - draggedShape->GetPosition();

			if ( ! dragImage->BeginDrag( beginDragHotSpot, this, false ))
			{
				delete dragImage;
				dragImage = 0;
				dragMode = TEST_DRAG_NONE;

			}
			else
			{
				dragImage->Move( event.GetPosition() );
				dragImage->Show();
			}
		}
		else if ( dragMode == TEST_DRAG_DRAGGING )
		{
			wxPoint shapepoint = draggedShape->GetPosition()+ event.GetPosition() - dragStartPos;
			for ( int i = 0; i < 12; i++ )
			{
				if ( getRasiCoord( i ).Contains( shapepoint ))
				{
					draggedShape->setHighlight( true );
					break;
				}
			}
			dragImage->Move(event.GetPosition());
		}
	}
}

/*****************************************************
**
**   DragChart   ---   DrawShapes
**
******************************************************/
void DragChart::DrawShapes(wxDC& dc)
{
	wxList::compatibility_iterator node = displayList.GetFirst();
	while (node)
	{
		DragShape* shape = (DragShape*) node->GetData();
		if ( shape->IsShown() && draggedShape != shape )
		{
			shape->Draw( dc );
		}
		node = node->GetNext();
	}
}

/*****************************************************
**
**   DragChart   ---   EraseShape
**
******************************************************/
void DragChart::EraseShape(DragShape* shape, wxDC& dc)
{
	wxSize sz = GetClientSize();
	wxRect rect( 0, 0, sz.x, sz.y );

	wxRect rect2(shape->GetRect());
	dc.SetClippingRegion( rect2.x, rect2.y, rect2.width, rect2.height );
	dc.DestroyClippingRegion();
}

/*****************************************************
**
**   DragChart   ---   ClearShapes
**
******************************************************/
void DragChart::ClearShapes()
{
	wxList::compatibility_iterator node = displayList.GetFirst();
	while ( node )
	{
		DragShape* shape = (DragShape*)node->GetData();
		delete shape;
		node = node->GetNext();
	}
	displayList.Clear();
}

/*****************************************************
**
**   DragChart   ---   FindShape
**
******************************************************/
DragShape* DragChart::FindShape(const wxPoint& pt) const
{
	wxList::compatibility_iterator node = displayList.GetFirst();
	while ( node )
	{
		DragShape* shape = (DragShape*)node->GetData();
		if ( shape->HitTest( pt )) return shape;
		node = node->GetNext();
	}
	return 0;
}

/*****************************************************
**
**   DragShape   ---   Constructor
**
******************************************************/
DragShape::DragShape(const ObjectId &index )
{
	objectid = index;
	createImage();
	pos.x = 0;
	pos.y = 0;
	show = true;
	highlight = false;
	rasipos = R_NONE;
}

/*****************************************************
**
**   DragShape   ---   Constructor
**
******************************************************/
DragShape::~DragShape()
{
}

/*****************************************************
**
**   DragShape   ---   createImage
**
******************************************************/
bool DragShape::createImage()
{
	SymbolProvider sp;
	bitmap.Create( WDRAGIMAGE, HDRAGIMAGE );
	wxMemoryDC dc;
	dc.SelectObject( bitmap );
	DcPainter painter( &dc );

	wxColour magic(141, 141, 141 );
	painter.setBrush( magic );
	painter.setTextColor( *wxBLACK );

	// TODO Quatsch
	painter.setPen( wxPen( magic, wxTRANSPARENT ));

	dc.DrawRectangle(0,0, WDRAGIMAGE, HDRAGIMAGE );

	wxString str = sp.getPlanetCode( objectid );
	painter.setSymbolFont();
	painter.drawTextFormatted( MRect( 0, 0, WDRAGIMAGE, HDRAGIMAGE ), sp.getPlanetCode( objectid ), Align::Center );

	dc.SelectObject( wxNullBitmap );

	wxMask *mask = new wxMask( bitmap, magic );
	bitmap.SetMask( mask );

	return bitmap.IsOk();
}

/*****************************************************
**
**   DragShape   ---   HitTest
**
******************************************************/
bool DragShape::HitTest(const wxPoint& pt) const
{
	wxRect rect( GetRect() );
	return rect.Contains( pt.x, pt.y );
}

/*****************************************************
**
**   DragShape   ---   Draw
**
******************************************************/
bool DragShape::Draw( wxDC& dc )
{
	if ( bitmap.Ok() )
	{
		wxMemoryDC memDC;
		memDC.SelectObject( bitmap );
		dc.Blit( pos.x, pos.y, bitmap.GetWidth(), bitmap.GetHeight(), &memDC, 0, 0, wxCOPY, true );

		// TODO geht nicht
		if ( highlight )
		{
			dc.SetPen( *wxWHITE_PEN );
			dc.SetBrush( *wxTRANSPARENT_BRUSH );
			dc.DrawRectangle( pos.x,  pos.y,  bitmap.GetWidth(),  bitmap.GetHeight() );
		}
		return true;
	}
	else return false;
}

/*****************************************************
**
**   DragShape   ---   getMidpoint
**
******************************************************/
wxPoint DragShape::getMidpoint() const
{
	return wxPoint( pos.x + WDRAGIMAGE / 2, pos.y + HDRAGIMAGE / 2 );
}
