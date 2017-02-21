/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ObjectFilterDialog.cpp
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

#include "ObjectFilterDialog.h"

// begin wxGlade: ::extracode
// end wxGlade

#include <wx/button.h>
#include <wx/checklst.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>

#include "Conf.h"
#include "guibase.h"
#include "Sheet.h"

extern Config *config;

enum { OD_ALL = wxID_HIGHEST + 300, OD_FROM_MARS, OD_UPTO_MARS, OD_NONE };

/*****************************************************
**
**   ObjectFilterDialog   ---   Constructor
**
******************************************************/
ObjectFilterDialog::ObjectFilterDialog(wxWindow* parent, const ObjectArray &p, const ObjectFilter &f )
		: wxDialog( parent, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE ),
		planets( p ),
		filter( f )
{
    // begin wxGlade: ObjectFilterDialog::ObjectFilterDialog
    sizer_right_staticbox = new wxStaticBox(this, wxID_ANY, _("Quick Select"));
    sizer_list_staticbox = new wxStaticBox(this, wxID_ANY, _("Included Objects"));
    list = new wxCheckListBox(this, wxID_ANY);
    button_all = new wxButton(this, OD_ALL, _("Select All"));
    button_upto_mars = new wxButton(this, OD_UPTO_MARS, _("Up to Mars"));
    button_from_mars = new wxButton(this, OD_FROM_MARS, _("Slower than Mars"));
    button_none = new wxButton(this, OD_NONE, _("Deselect All"));
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade


	SheetFormatter fmt;
	for ( uint i = 0; i < planets.size(); i++ )
	{
		list->Append( fmt.fragment2PlainText( fmt.getObjectName( planets[i], TF_LONG )));
	}
	for ( uint i = 0; i < planets.size(); i++ )
	{
		// check item if it is included in filter list OR filter list is empty (i.e. check all then)
		list->Check( i, filter.size() == 0 || filter.find( planets[i] ) != filter.end() );
	}

	Connect( OD_ALL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectFilterDialog::OnAll ));
	Connect( OD_FROM_MARS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectFilterDialog::OnFromMars ));
	Connect( OD_UPTO_MARS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectFilterDialog::OnUptoMars ));
	Connect( OD_NONE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectFilterDialog::OnNone ));
	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ObjectFilterDialog::OnOK ));

	if ( IS_VALID_SIZE( config->viewprefs->sizes.sObjectFilterDialog )) SetSize( config->viewprefs->sizes.sObjectFilterDialog );
}

/*****************************************************
**
**   ObjectFilterDialog   ---   Destructor
**
******************************************************/
ObjectFilterDialog::~ObjectFilterDialog()
{
	config->viewprefs->sizes.sObjectFilterDialog = GetSize();
}

/*****************************************************
**
**   ObjectFilterDialog   ---   OnOK
**
******************************************************/
void ObjectFilterDialog::OnOK( wxCommandEvent& )
{
	filter.clear();

	for( uint i = 0; i < planets.size(); i++ )
	{
		if ( list->IsChecked( i ))
		{
			filter.insert( planets[i] );
		}
	}
	EndModal( wxID_OK );
}

/*****************************************************
**
**   ObjectFilterDialog   ---   OnAll
**
******************************************************/
void ObjectFilterDialog::OnAll( wxCommandEvent& )
{
	//printf( "OnAll\n" );
	for ( uint i = 0; i < planets.size(); i++ ) list->Check( i, true );
}

/*****************************************************
**
**   ObjectFilterDialog   ---   OnFromMars
**
******************************************************/
void ObjectFilterDialog::OnFromMars( wxCommandEvent& )
{
	//printf( "OnFromMars\n" );
	for ( uint i = 0; i < planets.size(); i++ ) list->Check( i, planets[i] >= OMARS && ! IS_ANGLE_OBJECT( planets[i] ));
}

/*****************************************************
**
**   ObjectFilterDialog   ---   OnUptoMars
**
******************************************************/
void ObjectFilterDialog::OnUptoMars( wxCommandEvent& )
{
	//printf( "OnUptoMars\n" );
	for ( uint i = 0; i < planets.size(); i++ ) list->Check( i, planets[i] <= OMARS || IS_ANGLE_OBJECT( planets[i] ));
}

/*****************************************************
**
**   ObjectFilterDialog   ---   OnNone
**
******************************************************/
void ObjectFilterDialog::OnNone( wxCommandEvent& )
{
	//printf( "OnNone\n" );
	for ( uint i = 0; i < planets.size(); i++ ) list->Check( i, false );
}

/*****************************************************
**
**   ObjectFilterDialog   ---   set_properties
**
******************************************************/
void ObjectFilterDialog::set_properties()
{
    // begin wxGlade: ObjectFilterDialog::set_properties
    SetTitle(_("Configure Object Filter"));
    button_ok->SetDefault();
    // end wxGlade
}

/*****************************************************
**
**   ObjectFilterDialog   ---   do_layout
**
******************************************************/
void ObjectFilterDialog::do_layout()
{
    // begin wxGlade: ObjectFilterDialog::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_bottom = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_top = new wxBoxSizer(wxHORIZONTAL);
    sizer_right_staticbox->Lower();
    wxStaticBoxSizer* sizer_right = new wxStaticBoxSizer(sizer_right_staticbox, wxVERTICAL);
    sizer_list_staticbox->Lower();
    wxStaticBoxSizer* sizer_list = new wxStaticBoxSizer(sizer_list_staticbox, wxVERTICAL);
    sizer_list->Add(list, 1, wxALL|wxEXPAND, 3);
    sizer_top->Add(sizer_list, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_all, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_upto_mars, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(button_from_mars, 0, wxALL, 3);
    sizer_right->Add(button_none, 0, wxALL|wxEXPAND, 3);
    sizer_top->Add(sizer_right, 0, wxALL|wxALIGN_BOTTOM, 3);
    sizer_main->Add(sizer_top, 1, wxEXPAND, 0);
    wxStaticLine* static_line_2 = new wxStaticLine(this, wxID_ANY);
    sizer_main->Add(static_line_2, 0, wxALL|wxEXPAND, 3);
    sizer_bottom->Add(button_ok, 0, wxALL, 3);
    sizer_bottom->Add(button_cancel, 0, wxALL, 3);
    sizer_main->Add(sizer_bottom, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

