/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/SheetView.cpp
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

#include "SheetView.h"

extern Config *config;

IMPLEMENT_CLASS( SheetView, BasicView )

/*****************************************************
**
**   SheetView   ---   Constructor
**
******************************************************/
SheetView::SheetView( wxWindow *parent, ChildWindow *frame, const VIEW_ID &viewId, const bool &showToolbar )
		: BasicView( parent, frame, viewId, showToolbar )
{
	twidget = new SheetWidget( this, props );
	widget = twidget;
	//supportGraphicExport = true;

	updateView = true;

	Connect( wxEVT_IDLE, wxIdleEventHandler( SheetView::OnIdle ));
}

/*****************************************************
**
**   SheetView   ---   Destructor
**
******************************************************/
SheetView::~SheetView()
{
}

/*****************************************************
**
**   SheetView   ---   OnDataChanged
**
******************************************************/
void SheetView::OnDataChanged()
{
	//BasicView::OnDataChanged();
	OnToolbarCommand();
	updateView = true;
}

/*****************************************************
**
**   SheetView   ---   dispatchCommand
**
******************************************************/
/*
bool SheetView::dispatchCommand( const int &command )
{
	printf( "SheetView::dispatchCommand COMMAND %d\n", command );
	switch ( command )
	{
		case CMD_EXPORT_TEXT:
			twidget->exportAs( WE_TEXT );
		break;
		case CMD_EXPORT_HTML:
			twidget->exportAs( WE_HTML );
		break;
		case CMD_EXPORT_PDF:
			twidget->exportAs( WE_PDF );
		break;
		case CMD_EXPORT_IMAGE:
			twidget->exportAs( WE_IMAGE );
		break;
		default:
			return BasicView::dispatchCommand( command );
		break;
	}
	return true;
}
*/

/**************************************************************
***
**   SheetView   ---   exportAs
***
***************************************************************/
void SheetView::exportAs( const WidgetExportType &type )
{
	twidget->exportAs( type );
}

/**************************************************************
***
**   SheetView   ---   OnIdle
***
***************************************************************/
void SheetView::OnIdle( wxIdleEvent& )
{
	if ( ! updateView ) return;

	//int ret = 0;
	assert( twidget );
	//Writer *writer = twidget->getWriter();
	//writer->clearItems();

	// delegate
	write();
	twidget->Scroll( 0, 0 );
	twidget->OnDataChanged();

	updateView = false;
}


