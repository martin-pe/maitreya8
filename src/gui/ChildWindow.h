/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ChildWindow.h
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

#ifndef _CHILDWINDOW_H_
#define _CHILDWINDOW_H_

#include <wx/frame.h>
#include <vector>

#include "maitreya.h"
#include "ImageProvider.h"

class BasicView;
class ChartProperties;
class Document;
class DocumentManager;
struct ViewNode;

using namespace std;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( CHILD_ADDTORECENT, wxID_HIGHEST + 1100 )
DECLARE_EVENT_TYPE( CHILD_SETACTIVE, wxID_HIGHEST + 1101 )
DECLARE_EVENT_TYPE( CHILD_CLOSED, wxID_HIGHEST + 1102 )
DECLARE_EVENT_TYPE( CHILD_ADDED, wxID_HIGHEST + 1103 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
* \brief base class for child window based upon wxFrame
*
******************************************************/
class ChildWindow : public wxFrame
{
	DECLARE_CLASS( ChildWindow )
public:
	ChildWindow( wxFrame *parent, Document *doc, const BitmapId&, const wxSize& size = wxDefaultSize, const bool ismain = false );
	~ChildWindow();

	// is called by children: GraphicView on ew change and TextView onToolbarCommand
	virtual void setTitle() = 0;

	virtual bool isMultiple() const { return false; }

	ChartProperties *getProps() const { return props; }

	Document *getDoc() const { return doc; }

	int getViewListImageId() const;

	// determines if doc is owned, i.e. doc must be deleted including other children
	void setMainWindow( const bool& );
	bool isMainWindow() { return ismainwindow; }

	virtual void OnDataChanged() = 0;
	virtual void postCreate() = 0;

	virtual void OnCommand( wxCommandEvent &event );
	virtual bool dispatchCommand( const int& command );

	// send signal for ApplicationWindow list control (change of selection)
	void OnActivate( wxActivateEvent& );

protected:
	ChartProperties *props;
	Document *doc;
	const BitmapId bmpId;

	virtual void OnClose( wxCloseEvent &event );

private:

	void OnEphemFileWarning( wxCommandEvent& );

	bool ismainwindow, isvalid;
};

/*************************************************//**
*
* \brief child window containing a single BasicView
*
******************************************************/
class SimpleChildWindow : public ChildWindow
{
	DECLARE_CLASS( SimpleChildWindow )
public:

	SimpleChildWindow( wxFrame *parent, Document *doc, const BitmapId&, const wxSize& size = wxDefaultSize, const bool ismain = false );
	~SimpleChildWindow();

	void insertView( BasicView* );
	BasicView *getView() const { return view; }

	virtual void OnDataChanged();
	virtual void postCreate();
	virtual void setTitle();
	virtual bool dispatchCommand( const int& command );

protected:

	virtual void OnClose( wxCloseEvent &event );
	void OnActivate( wxActivateEvent& );
	void OnSize( wxSizeEvent& );

private:
	BasicView *view;
};

/*************************************************//**
*
* \brief encapsulates creation of a ChildWindow.
*
******************************************************/
class ChildWindowFactory
{
public:
	ChildWindow *createPartnerWindow( wxFrame* );
	ChildWindow *createSingleMainWindow( wxFrame *parent, Document *doc );
	ChildWindow *createMultipleMainWindow( wxFrame *parent, Document *doc, int = 0 );
	ChildWindow *createMainWindow( wxFrame*, Document*, const bool&, const int = 0 );
	ChildWindow *createChild( wxFrame *parent, Document *doc, const int id );
};

#endif

