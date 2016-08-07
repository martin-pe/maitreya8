/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DasaPanel.cpp
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

#include "DasaPanel.h"

#include <wx/bmpbuttn.h>
#include <wx/intl.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "guibase.h"
#include "ImageProvider.h"

/*****************************************************
**
**   DasaPanel   ---   Constructor
**
******************************************************/
DasaPanel::DasaPanel(wxWindow* parent, int id )
		: wxCollapsiblePane(parent, id, wxEmptyString )
{
	wxWindow *win = GetPane();

	button_entry_chart = new wxBitmapButton(win, DASA_CREATE_ENTRY_CHART, wxNullBitmap);
	button_entry_chart->SetBitmapLabel( ImageProvider::get()->getBitmap( BM_NEW ));
	button_entry_chart->SetMinSize(wxSize(32, 32));
	button_entry_chart->SetToolTip(_("Create Entry Chart"));

	label_type = new wxStaticText(win, -1, _("type"));
	label_type->SetMinSize(wxSize(120,22));
	label_startdate = new wxStaticText(win, -1, _("start"));
	label_startdate->SetMinSize(wxSize(120,22));
	label_enddate = new wxStaticText(win, -1, _("end"));
	label_enddate->SetMinSize(wxSize(120,22));
	label_duration = new wxStaticText(win, -1, _("duration"));
	label_duration->SetMinSize(wxSize(120,22));

	//wxStaticBox *staticbox = new wxStaticBox(win, -1, _("Properties of Selected Yoga"));
	//wxStaticBoxSizer* sizer_main = new wxStaticBoxSizer(staticbox, wxHORIZONTAL);
	wxFlexGridSizer* sizer = new wxFlexGridSizer(4, 1, 0, 0);
	sizer->Add(label_type, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
	sizer->Add(label_startdate, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
	sizer->Add(label_enddate, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
	sizer->Add(label_duration, 0, wxLEFT|wxRIGHT|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
	//sizer_main->Add( sizer );
	//sizer->Fit(this);
	//SetAutoLayout(true);
	sizer->AddGrowableRow(0);
	sizer->AddGrowableCol(0);
	win->SetSizer(sizer);
	Expand();
}

/*****************************************************
**
**   DasaPanel   ---   clear
**
******************************************************/
void DasaPanel::clear()
{
	label_type->SetLabel( wxEmptyString );
	label_startdate->SetLabel( wxEmptyString );
	label_enddate->SetLabel( wxEmptyString);
	label_duration->SetLabel( wxEmptyString );;
	//button_entry_chart->Enable( false );
}

