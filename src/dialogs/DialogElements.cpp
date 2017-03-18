/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DialogElements.cpp
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

#include "DialogElements.h"

#include "astrobase.h"
#include "maitreya.h"
#include "Conf.h"
#include "DialogElements.h"
#include "guibase.h"
#include "Sheet.h"

#include <math.h>

#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/settings.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

IMPLEMENT_CLASS( MyGrid, wxGrid )
IMPLEMENT_CLASS( ToolbarLabel, wxControl )
IMPLEMENT_CLASS( MultiLineStaticText, wxPanel )
IMPLEMENT_CLASS( TransitModeChoice, wxChoice )

/*****************************************************
**
**   MyGrid   ---   Constructor
**
******************************************************/
MyGrid::MyGrid( wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
                long style, const wxString& name )
		: wxGrid( parent, id, pos, size, style, name )
{
	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( MyGrid::OnMouseEvent ));
	Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( MyGrid::OnKeyDown ));
}

/*****************************************************
**
**   MyGrid   ---   OnMouseEvent
**
******************************************************/
void MyGrid::OnMouseEvent( wxMouseEvent &event )
{
	int x, y;
	const int offset = 3;
	GetViewStart( &x, &y );
	if ( event.GetWheelRotation() < 0 ) y += offset;
	else y -= offset;
	Scroll( x, y );
}

/*****************************************************
**
**   MyGrid   ---   OnKeyDown
**
******************************************************/
void MyGrid::OnKeyDown( wxKeyEvent &event )
{
	long keycode = event.GetKeyCode();
	if ( keycode == WXK_ESCAPE ) {
		GetParent()->Close();
	}
	event.Skip();
}

/*****************************************************
**
**   YearLengthChoice   ---   Constructor
**
******************************************************/
YearLengthChoice::YearLengthChoice( wxWindow *parent, int id, const bool /* vedic */, int selected )
		: wxChoice( parent, id )
{
	SetToolTip( _( "Year Length" ));
	Append( wxT( "365.25" ));
	Append( _( "Tropical (365.2422)" ));
	Append( wxT( "365" ));
	Append( wxT( "360" ));
	Append( _( "Custom" ));
	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   YearLengthChoice   ---   getYearLength
**
******************************************************/
double YearLengthChoice::getYearLength()
{
	return calcYearLength( GetSelection(), vedic );
}

/*****************************************************
**
**   GradkreisChoice   ---   Constructor
**
******************************************************/
GradkreisChoice::GradkreisChoice( wxWindow *parent, int id, int selected )
		: wxChoice( parent, id )
{
	SetToolTip( _( "Gradkreis" ));
	Append( wxT( "360" ));
	Append( wxT( "180" ));
	Append( wxT( "90" ));
	Append( wxT( "45" ));
	Append( wxT( "22.5" ));
	Append( wxT( "11.25" ));
	Append( wxT( "5.625" ));
	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   GradkreisChoice   ---   DoGetBestSize
**
******************************************************/
wxSize GradkreisChoice::DoGetBestSize() const
{
	//printf( "DoGetBestSize :::\n" );
	return wxChoice::DoGetBestSize();
}

/*****************************************************
**
**   SortChoice   ---   Constructor
**
******************************************************/
SortChoice::SortChoice( wxWindow *parent, int id, int selected )
		: wxChoice( parent, id, wxDefaultPosition, wxSize( 110, -1 ))
{
	SetToolTip( _( "Sort Order" ));
	Append( _( "Right Side" ));
	Append( _( "Left Side" ));
	Append( _( "Orbis" ));
	Append( _( "Orbis Absolute" ));
	Append( _( "Orbis Reverse" ));
	Append( _( "Gradkreis" ));
	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   SortChoice   ---   getSortOrder
**
******************************************************/
ASPECT_SORTORDER SortChoice::getSortOrder() const
{
	switch( GetSelection())
	{
		case 0:
			return AS_RPLANET;
		break;
		case 1:
			return AS_LPLANET;
		break;
		case 2:
			return AS_ORBIS;
		break;
		case 3:
			return AS_ORBIS_ABSOLUTE;
		break;
		case 4:
			return AS_ORBIS_REVERSE;
		break;
		case 5:
			return AS_TYPE;
		break;
		default:
			wxLogError( wxString::Format( wxT( "wrong sort order value %d in choice box" ), GetSelection()));
		break;
		
	}
	return AS_NONE;
}

/*****************************************************
**
**   UranianReferenceObjectChoice   ---   Constructor
**
******************************************************/
UranianReferenceObjectChoice::UranianReferenceObjectChoice( wxWindow *parent, int id, int selected )
		: wxChoice( parent, id, wxDefaultPosition ) //, wxSize( 110, -1 ))
{
	SetToolTip( _( "Reference Object" ));
	Append( _( "Sun on Cap" ));
	Append( _( "Sun on Cap (m)" ));
	Append( _( "Cap on Sun" ));
	Append( _( "Cap on Sun (m)" ));

	SetSize( GetBestSize());
	SetSelection( selected );
}

/*****************************************************
**
**   UranianReferenceObjectChoice   ---   getReferenceObject
**
******************************************************/
ObjectId UranianReferenceObjectChoice::getReferenceObject() const
{
	switch( GetSelection())
	{
		case 0:
			return OSUN;
		break;
		case 1:
			return OMOON;
		break;
		default:
			return ONONE;
		break;
	}
}

/*****************************************************
**
**   TransitModeChoice   ---   Constructor
**
******************************************************/
TransitModeChoice::TransitModeChoice( wxWindow *parent, int id )
	: wxChoice( parent, id ) 
{
	SetToolTip( _( "Mode" ));
	Append( _( "Transits" ));
	Append( _( "Directions" ));
	Append( _( "Solar Arc" ));
	Append( _( "Reverse Solar Arc" ));
	Append( _( "Shifted Gravitation Point" ));
	Append( _( "Shifted Meridian" ));
	Append( _( "Lunar Arc" ));
	Append( _( "Constant Arc" ));
	SetSize( GetBestSize());
}

/*****************************************************
**
**   TransitModeChoice   ---   setTransitMode
**
******************************************************/
void TransitModeChoice::setTransitMode( const PlanetContext &ctx )
{
	ASSERT_VALID_TRANSIT_CONTEXT( ctx );
	SetSelection( ctx - PcTransit );
}

/*****************************************************
**
**   TransitModeChoice   ---   getTransitMode
**
******************************************************/
PlanetContext TransitModeChoice::getTransitMode() const
{
	const PlanetContext ctx = (PlanetContext)(GetSelection() + (int)PcTransit);
	ASSERT_VALID_TRANSIT_CONTEXT( ctx );
	return ctx;
}


#define MAX_TEXT_VIEW_MODES 17

/*****************************************************
**
**   TextViewModeChoice   ---   Constructor
**
******************************************************/
TextViewModeChoice::TextViewModeChoice( wxWindow *parent, const int &id, const int /* selected */ )
	: wxChoice( parent, id, wxDefaultPosition, wxDefaultSize )
{
	for( int i = 0; i < MAX_TEXT_VIEW_MODES; i++ )
	{
		Append( getItemName( i ));
	}
	SetToolTip( _( "Mode" ));
	SetSize( DoGetBestSize());
}

/*****************************************************
**
**   TextViewModeChoice   ---   getItemName
**
******************************************************/
wxString TextViewModeChoice::getItemName( const int &i )
{
	const static wxString text_viewmodes[MAX_TEXT_VIEW_MODES] =
	{
		_("Base Data"),
		_("Arabic Parts"),
		_("Ashtakavarga"),
		_("Astronomical"),
		_("Bhava"),
		_("Dasa (Short)"),
		_("Dasa (Compact)"),
		_("Dasa (Long)"),
		_("Jaimini"),
		_("Krishnamurti Paddhati"),
		_("Nakshatra"),
		_("Shadbala"),
		_("Vargas"),
		_("Vedic Aspectarium"),
		_("Vedic Planets"),
		_("Western Aspectarium"),
		_("Western Planets")
	};
	if ( i < 0 || i >= MAX_TEXT_VIEW_MODES ) return wxT( "Error" );
	else return text_viewmodes[i];
}

/*****************************************************
**
**   ToolbarLabel   ---   Constructor
**
******************************************************/
ToolbarLabel::ToolbarLabel( wxWindow *parent, wxWindowID id, wxPoint pos, wxSize size, wxString /* tooltip */ )
	: wxControl( parent, id, pos, size )
{
	wxPanel *panel = new wxPanel(this, -1, pos, size, wxEXPAND | wxSUNKEN_BORDER | wxTAB_TRAVERSAL );
	wxBoxSizer* sizerp = new wxBoxSizer( wxVERTICAL );
	//wxBoxSizer* sizert = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* sizert = new wxBoxSizer( wxVERTICAL );
	//sizerp->Add( panel, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 3 );
	//sizerp->Add( panel, 1, wxALL | wxEXPAND | wxALIGN_CENTER_VERTICAL, 3 );
	sizerp->Add( panel, 1, wxEXPAND | wxALIGN_CENTER_VERTICAL );

	theText = new wxStaticText( panel, -1, wxEmptyString, wxDefaultPosition, size );
	sizert->Add( theText, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3 );
	panel->SetSizer(sizert);
	sizert->Fit(panel);
	sizerp->Fit(this);
	sizerp->SetSizeHints(this);
	sizert->SetSizeHints(panel);
	SetSizer(sizerp);
	Layout();
}

/*****************************************************
**
**   ToolbarLabel   ---   SetLabel
**
******************************************************/
void ToolbarLabel::SetLabel( const wxString &s )
{
	theText->SetLabel( s );
}

/*****************************************************
**
**   MultiLineStaticText   ---   Constructor
**
******************************************************/
MultiLineStaticText::MultiLineStaticText( wxWindow *parent, wxWindowID id, wxPoint pos, wxSize size )
 : wxPanel( parent, id, pos, size, wxSUNKEN_BORDER|wxTAB_TRAVERSAL )
{
	text = new wxStaticText( this, wxID_ANY, wxEmptyString );
	text->SetMinSize(wxSize( size.x - 6, size.y - 6 ));

	wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);
	sizer->Add(text, 0, wxALL|wxEXPAND, 3);

	SetSizer( sizer );
	sizer->Fit(this);
	Layout();

	Connect( wxEVT_SIZE, wxSizeEventHandler( MultiLineStaticText::OnSize ));
}

/*****************************************************
**
**   MultiLineStaticText   ---   SetLabel
**
******************************************************/
void MultiLineStaticText::SetLabel( const wxString &s )
{
	text->SetLabel( s );
}

/*****************************************************
**
**   MultiLineStaticText   ---   GetLabel
**
******************************************************/
wxString MultiLineStaticText::GetLabel() const
{
	return text->GetLabel();
}

/*****************************************************
**
**   MultiLineStaticText   ---   OnSize
**
******************************************************/
void MultiLineStaticText::OnSize( wxSizeEvent &event )
{
	printf( "ON size %d\n", event.GetSize().x );
	wxString s = text->GetLabel();
	s.Replace( wxT( "\n" ), wxEmptyString );	
	text->SetLabel( s );
	PrintLn( text->GetLabel());
	text->SetSize( event.GetSize());
	text->Wrap( event.GetSize().x - 12 );
	text->Layout();
	text->Refresh();
	//event.Skip();
}


