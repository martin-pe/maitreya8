/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ExportDialog.cpp
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

#include "ExportDialog.h"

#include "guibase.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/intl.h>
#include <wx/statbox.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statline.h>
#include <wx/stattext.h>

IMPLEMENT_CLASS( ExportDialog, wxDialog )

/*****************************************************
**
**   ExportDialog   ---   Constructor
**
******************************************************/
ExportDialog::ExportDialog(wxWindow* parent, const int &x, const int &y )
		:  wxDialog(parent, -1, wxT( "--"  ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE)
{
	sizex = x;
	sizey = y;
    // begin wxGlade: ExportDialog::ExportDialog
    sizer_size_staticbox = new wxStaticBox(this, wxID_ANY, _("Size"));
    label_x = new wxStaticText(this, wxID_ANY, _("Horizontal Size"));
    spin_x = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000);
    label_y = new wxStaticText(this, wxID_ANY, _("Vertical Size"));
    spin_y = new wxSpinCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 100000);
    check_show_dialog = new wxCheckBox(this, wxID_ANY, _("Don't show this dialog again"));
    static_line_1 = new wxStaticLine(this, wxID_ANY);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	check_show_dialog->SetValue( false );
	button_ok->SetFocus();
}

/*****************************************************
**
**   ExportDialog   ---   getSizes
**
******************************************************/
void ExportDialog::getSizes( int &x, int &y )
{
	x = spin_x->GetValue();
	y = spin_y->GetValue();
}

/*****************************************************
**
**   ExportDialog   ---   getCheckShowDialog
**
******************************************************/
bool ExportDialog::getCheckShowDialog()
{
	return check_show_dialog->GetValue();
}

/*****************************************************
**
**   ExportDialog   ---   set_properties
**
******************************************************/
void ExportDialog::set_properties()
{
    // begin wxGlade: ExportDialog::set_properties
    SetTitle(_("Export Picture"));
    // end wxGlade
	spin_x->SetValue( sizex );
	spin_y->SetValue( sizey );
}

/*****************************************************
**
**   ExportDialog   ---   do_layout(
**
******************************************************/
void ExportDialog::do_layout()
{
    // begin wxGlade: ExportDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(3, 1, 0, 0);
    wxBoxSizer* sizer_buttons = new wxBoxSizer(wxHORIZONTAL);
    sizer_size_staticbox->Lower();
    wxStaticBoxSizer* sizer_size = new wxStaticBoxSizer(sizer_size_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_size = new wxFlexGridSizer(2, 2, 3, 3);
    grid_size->Add(label_x, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_size->Add(spin_x, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_size->Add(label_y, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_size->Add(spin_y, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_size->Add(grid_size, 1, wxEXPAND, 0);
    sizer_size->Add(20, 20, 0, wxALL|wxEXPAND, 3);
    sizer_size->Add(check_show_dialog, 0, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_size, 1, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_main->Add(static_line_1, 0, wxALL|wxEXPAND, 3);
    sizer_buttons->Add(button_ok, 0, wxALL, 3);
    sizer_buttons->Add(button_cancel, 0, wxALL, 3);
    sizer_main->Add(sizer_buttons, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableCol(0);
    Layout();
    // end wxGlade
}

