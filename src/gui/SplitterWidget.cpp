/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SplitterWidget.cpp
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

#include "SplitterWidget.h"

IMPLEMENT_CLASS2( SplitterWidget, wxSplitterWindow, KeyEventObserver )

/*****************************************************
**
**   SplitterWidget   ---   Constructor
**
******************************************************/
SplitterWidget::SplitterWidget( wxWindow *parent, const wxWindowID id, const wxPoint &pos, const wxSize size, long style )
		: wxSplitterWindow( parent, id, pos, size, style )
{
	SetMinimumPaneSize( 50 );

// GTK needs evt_right_down, not needed on windows
	//Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( SplitterWidget::onMouseRight ));

	//Connect( wxEVT_COMMAND_SPLITTER_DOUBLECLICKED, wxSplitterEventHandler( SplitterWidget::OnSplitterDoubleClick ));
	//Connect( wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( SplitterWidget::OnSplitterSashPosChanged ));
	//Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( SplitterWidget::OnKeyDown ));
	//Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( SplitterWidget::OnSetFocus ));
}

/*****************************************************
**
**   SplitterWidget   ---   Destructor
**
******************************************************/
SplitterWidget::~SplitterWidget()
{
}

/*****************************************************
**
**   SplitterWidget   ---   onMouseRight
**
******************************************************/
void SplitterWidget::onMouseRight( wxMouseEvent& event)
{
	int x = event.m_x;
	int y = event.m_y;
	wxWindow *window = (wxWindow*)event.GetEventObject();
	window->ClientToScreen( &x, &y );
	GetParent()->ScreenToClient( &x, &y );
	event.m_x = x;
	event.m_y = y;
	event.SetEventObject( wxSplitterWindow::GetParent());
	wxPostEvent( GetParent(), event );
}

/*****************************************************
**
**   SplitterWidget   ---   OnKeyDown
**
******************************************************/
void SplitterWidget::OnKeyDown( wxKeyEvent &event )
{
	//printf( "KEY DOWN in SplitterWidget::OnKeyDown\n" );
	event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
	event.Skip();
}

/*****************************************************
**
**   SplitterWidget   ---   OnSetFocus
**
******************************************************/
void SplitterWidget::OnSetFocus( wxFocusEvent& )
{
	// TODO: Frage wie wird der Focus weitergegeben?
	//printf( "SET FOCUS in SplitterWidget::OnFocusEvent\n" );
}

/*****************************************************
**
**   SplitterWidget   ---   OnSize
**
******************************************************/
void SplitterWidget::OnSize( wxSizeEvent &event )
{
	if ( GetWindow1() ) GetWindow1()->Refresh();
	if ( GetWindow2()) GetWindow2()->Refresh();
	//SetSize( event.GetSize() );
	event.Skip();
}

/*****************************************************
**
**   SplitterWidget   ---   OnSplitterDoubleClick
**
******************************************************/
void SplitterWidget::OnSplitterDoubleClick(wxSplitterEvent& event)
{
	// no nothing because default behaviour will remove one of the widgets
	event.Veto();
}

/*****************************************************
**
**   SplitterWidget   ---   OnSplitterSashPosChanged
**
******************************************************/
void SplitterWidget::OnSplitterSashPosChanged(wxSplitterEvent& )
{
	printf( "SplitterWidget::OnSplitterSashPosChanged\n" );
	if ( GetWindow1() ) GetWindow1()->Refresh();
	if ( GetWindow2() ) GetWindow2()->Refresh();
}

/*****************************************************
**
**   SplitterWidget   ---   OnPositionChanged
**
******************************************************/
void SplitterWidget::OnPositionChanged(wxSplitterEvent& event)
{
	wxSize size1 = wxSplitterWindow::GetSize();
	size1.x = event.GetSashPosition();
	GetWindow1()->SetSize( size1 );
	GetWindow1()->Refresh();
	wxSize size2 = wxSplitterWindow::GetSize();
	size2.x -= event.GetSashPosition();
	GetWindow2()->SetSize( size2 );
	GetWindow2()->Refresh();
	//event.Skip();
}

