/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DialogElements.h
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


#ifndef _DIALOGELEMENTS_H_
#define _DIALOGELEMENTS_H_

#include <wx/choice.h>
#include <wx/event.h>
#include <wx/grid.h>
#include <wx/panel.h>

#include "maitreya.h"

class wxStaticText;

/*************************************************//**
*
* \brief basic grid class adding some mouse and key events to wxGrid
*
******************************************************/
class MyGrid : public wxGrid
{
	DECLARE_CLASS( ChildWindow )

public:
	MyGrid( wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
	        const wxSize& size = wxDefaultSize, long style = wxWANTS_CHARS, const wxString& name = wxPanelNameStr);

protected:
	void OnMouseEvent( wxMouseEvent& );
	void OnKeyDown( wxKeyEvent& );

};

/*************************************************//**
*
* 
*
******************************************************/
class GradkreisChoice : public wxChoice
{
public:
	GradkreisChoice( wxWindow *parent, int id, int selected = 0 );
	virtual wxSize DoGetBestSize() const;

	GRADKREIS getGradkreis() const 
	{
		GRADKREIS gk = (GRADKREIS)GetSelection();
		//ASSERT_VALID_GRADKREIS( gk );
		return gk;
	}
};

/*************************************************//**
*
* \brief choice for year length
*
******************************************************/
class YearLengthChoice : public wxChoice
{
public:
	YearLengthChoice( wxWindow *parent, int id, const bool vedic, int selected = 0 );
	double getYearLength();

private:
	bool vedic;
};

/*************************************************//**
*
* \brief choice for sort orders in Uranian astrology
*
******************************************************/
class SortChoice : public wxChoice
{
public:
	SortChoice( wxWindow *parent, int id, int selected = 0 );

	ASPECT_SORTORDER getSortOrder() const;
};

/*************************************************//**
*
* \brief ref object in Uranian astrology
*
******************************************************/
class UranianReferenceObjectChoice : public wxChoice
{
public:
	UranianReferenceObjectChoice( wxWindow *parent, int id, int selected = 0 );

	ObjectId getReferenceObject() const;
};

/*************************************************//**
*
* \brief choice with transit modes for TransitView
*
******************************************************/
class TransitModeChoice : public wxChoice
{
	DECLARE_CLASS( TransitModeChoice )
public:
	TransitModeChoice( wxWindow *parent, int id );

	PlanetContext getTransitMode() const;
	void setTransitMode( const PlanetContext& );
};

/*************************************************//**
*
* 
*
******************************************************/
class TextViewModeChoice : public wxChoice
{
public:
	TextViewModeChoice( wxWindow *parent, const int &id, const int selected = 0 );

	static wxString getItemName( const int& );
};

/*************************************************//**
*
* \brief simple text label for toolbar
*
******************************************************/
class ToolbarLabel : public wxControl
{
	DECLARE_CLASS( ToolbarLabel )
public:

	ToolbarLabel( wxWindow *parent, wxWindowID id, wxPoint pos, wxSize size, wxString tooltip = wxEmptyString );
	void SetLabel( const wxString& );

private:
	wxStaticText *theText;
};

/*************************************************//**
*
* 
*
******************************************************/
class MultiLineStaticText : public wxPanel
{
	DECLARE_CLASS( MultiLineStaticText )
public:

	MultiLineStaticText( wxWindow *parent, wxWindowID id, wxPoint pos = wxDefaultPosition, wxSize size = wxDefaultSize );
	void SetLabel( const wxString& );
	virtual wxString GetLabel() const ;

private:
	void OnSize( wxSizeEvent& );
	wxStaticText *text;
};

#endif


