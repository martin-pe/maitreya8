/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/MultipleViewPanel.cpp
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

#include "MultipleViewPanel.h"

#include "Conf.h"
#include "FileConfig.h"
#include "MultipleViewConfig.h"
#include "mvalidator.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;
enum { CD_SWITCH_MVIEW = wxID_HIGHEST + 1, CD_CHOICE_DEFAULTVIEW };

IMPLEMENT_CLASS( MultipleViewPanel, ConfigPanel )

/*****************************************************
**
**   MultipleViewPanel   ---   Constructor
**
******************************************************/
MultipleViewPanel::MultipleViewPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	mconfig = new MultipleViewConfiguration();
	config2model();

    // begin wxGlade: MultipleViewPanel::MultipleViewPanel
    sizer_mview_staticbox = new wxStaticBox(this, wxID_ANY, _("Multiple View Configuration"));
    sizer_nbstyle_staticbox = new wxStaticBox(this, wxID_ANY, _("Notebooks in Multiple View"));
    panel_description = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    check_use_mviews = new wxCheckBox(this, CD_SWITCH_MVIEW, _("Use Multiple Views"));
    label_defview = new wxStaticText(this, wxID_ANY, _("Default View"));
    const wxString choice_viewlist_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_viewlist = new wxChoice(this, CD_CHOICE_DEFAULTVIEW, wxDefaultPosition, wxDefaultSize, 4, choice_viewlist_choices, 0);
    text_view_description = new wxStaticText(panel_description, wxID_ANY, _("dummy"));
    label_nbstyle = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_nbstyle_choices[] = {
        _("Notebook"),
        _("Listbook"),
        _("Choicebook"),
        _("Toolbook"),
        _("Treebook")
    };
    choice_nbstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choice_nbstyle_choices, 0);
    label_orientation = new wxStaticText(this, wxID_ANY, _("Orientation"));
    const wxString choice_orientation_choices[] = {
        _("Top"),
        _("Bottom"),
        _("Left"),
        _("Right")
    };
    choice_orientation = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 4, choice_orientation_choices, 0);

    set_properties();
    do_layout();
    // end wxGlade

	text_view_description->SetBackgroundColour( GetBackgroundColour() );
	text_view_description->SetForegroundColour( GetForegroundColour() );

	choice_viewlist->Clear();
	choice_viewlist->Append( MultipleViewConfigLoader::get()->getNamesAsArray());

	choice_nbstyle->Clear();
#if wxUSE_NOTEBOOK
	choice_nbstyle->Append( _( "Notebook" ));
#endif
#if wxUSE_LISTBOOK
	choice_nbstyle->Append( _( "Listbook" ));
#endif
#if wxUSE_CHOICEBOOK
	choice_nbstyle->Append( _( "Choicebook" ));
#endif
#if wxUSE_TOOLBOOK
	choice_nbstyle->Append( _( "Toolbook" ));
#endif
#if wxUSE_TREEBOOK
	choice_nbstyle->Append( _( "Treebook" ));
#endif

	choice_viewlist->SetValidator( MChoiceValidator( &mconfig->defaultView ));
	check_use_mviews->SetValidator( MCheckValidator( &mconfig->useMultipleViews ));
	choice_nbstyle->SetValidator( MChoiceValidator( &mconfig->notebookStyle ));
	choice_orientation->SetValidator( MChoiceValidator( &mconfig->notebookOrientation ));

	Connect( CD_CHOICE_DEFAULTVIEW, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MultipleViewPanel::OnChoice ));
	Connect( CD_SWITCH_MVIEW, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MultipleViewPanel::OnChoice ));
}

/*****************************************************
**
**   MultipleViewPanel   ---   Life Cycle
**
******************************************************/
MultipleViewPanel::~MultipleViewPanel() { delete mconfig; }
void MultipleViewPanel::config2model() { *mconfig = *config->multipleView; }
void MultipleViewPanel::model2config() { *config->multipleView = *mconfig; }
void MultipleViewPanel::restoreDefaults() { *mconfig = MultipleViewConfiguration(); }

/*****************************************************
**
**   MultipleViewPanel   ---   updateUi
**
******************************************************/
void MultipleViewPanel::updateUi()
{
	int sel = choice_viewlist->GetSelection();
	wxCoord w, h;
	panel_description->GetSize( &w, &h );
	if ( sel == -1 )
		text_view_description->SetLabel( wxT( "Error" ));
	else
		text_view_description->SetLabel( MultipleViewConfigLoader::get()->getConfig( sel )->description );
	text_view_description->Wrap( (int)w - 12 );

	const bool multipleMode = check_use_mviews->GetValue();
	printf( "MultipleViewPanel::enableMViewConfigItems mode %d\n", multipleMode );

	label_defview->Enable( multipleMode );
	choice_viewlist->Enable( multipleMode );
	text_view_description->Enable( multipleMode );

	label_nbstyle->Enable( multipleMode );
	choice_nbstyle->Enable( multipleMode );
	label_orientation->Enable( multipleMode );
	choice_orientation->Enable( multipleMode );
}

/*****************************************************
**
**   MultipleViewPanel   ---   OnChoice
**
******************************************************/
void MultipleViewPanel::OnChoice( wxCommandEvent& )
{
	updateUi();
	setDirty();
}

/*****************************************************
**
**   MultipleViewPanel   ---   set_properties
**
******************************************************/
void MultipleViewPanel::set_properties()
{
    // begin wxGlade: MultipleViewPanel::set_properties
    choice_viewlist->SetSelection(0);
    text_view_description->SetMinSize(wxSize(150, 100));
    choice_nbstyle->SetSelection(0);
    choice_orientation->SetSelection(0);
    // end wxGlade
}

/*****************************************************
**
**   MultipleViewPanel   ---   do_layout
**
******************************************************/
void MultipleViewPanel::do_layout()
{
    // begin wxGlade: MultipleViewPanel::do_layout
    wxFlexGridSizer* grid_main = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(1, 1, 3, 3);
    sizer_nbstyle_staticbox->Lower();
    wxStaticBoxSizer* sizer_nbstyle = new wxStaticBoxSizer(sizer_nbstyle_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_nbstyle = new wxFlexGridSizer(2, 2, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(2, 1, 3, 3);
    sizer_mview_staticbox->Lower();
    wxStaticBoxSizer* sizer_mview = new wxStaticBoxSizer(sizer_mview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_mview = new wxFlexGridSizer(3, 1, 0, 0);
    wxBoxSizer* sizer_description = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* grid_select_view = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_mview->Add(check_use_mviews, 0, wxALL, 3);
    grid_select_view->Add(label_defview, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_select_view->Add(choice_viewlist, 0, wxALL|wxEXPAND, 3);
    grid_select_view->AddGrowableCol(1);
    grid_mview->Add(grid_select_view, 1, wxALL|wxEXPAND, 3);
    sizer_description->Add(text_view_description, 0, wxALL, 3);
    panel_description->SetSizer(sizer_description);
    grid_mview->Add(panel_description, 1, wxALL|wxEXPAND, 3);
    grid_mview->AddGrowableRow(2);
    grid_mview->AddGrowableCol(0);
    sizer_mview->Add(grid_mview, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_mview, 1, wxALL|wxEXPAND, 3);
    grid_main->Add(sizer_left, 1, wxEXPAND, 0);
    grid_nbstyle->Add(label_nbstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nbstyle->Add(choice_nbstyle, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    grid_nbstyle->Add(label_orientation, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_nbstyle->Add(choice_orientation, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_nbstyle->Add(grid_nbstyle, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_nbstyle, 1, wxALL|wxEXPAND, 3);
    grid_main->Add(sizer_right, 1, wxEXPAND, 0);
    SetSizer(grid_main);
    grid_main->Fit(this);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createMultipleViewPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createMultipleViewPanel( wxWindow *parent )
{
	return new MultipleViewPanel( parent );
}

