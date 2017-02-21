/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PrintoutPanel.cpp
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

#include "PrintoutPanel.h"

#include "ChartProperties.h"
#include "Conf.h"
#include "Document.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "mvalidator.h"
#include "PdfTools.h"
#include "PrintoutConfig.h"
#include "SheetConfig.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/colordlg.h>
#include <wx/filename.h>
#include <wx/filedlg.h>
#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;
enum { CD_CHOICE_PRINTOUT_TYPE = wxID_HIGHEST + 1, CD_LAUNCH_VIEWER, CD_LAUNCH_COMMAND_BUTTON,
	CD_LAUNCH_COMMAND_TEXT, CD_SWITCH_VIEWERMODE, CD_CHOICE_CUSTOM_TEXT_TYPE, CD_SHOW_CUSTOM_TEXT,
	CD_CUSTOM_TEXT, CD_ORIENTATION, CD_LINE_SEPARATOR, CD_FIRST_PAGE, CD_PRINT_TEST_PAGE, CD_AGENDA
};

IMPLEMENT_CLASS( PrintoutPanel, ConfigPanel )

/*****************************************************
**
**   PrintoutPanel   ---   Constructor
**
******************************************************/
PrintoutPanel::PrintoutPanel( wxWindow* parent ) : ConfigPanel( parent )
{
	pconfig = new PdfDocumentConfig();
	config2model();

    // begin wxGlade: PrintoutPanel::PrintoutPanel
    sizer_printout_staticbox = new wxStaticBox(this, wxID_ANY, _("Printout Type"));
    sizer_view_options_staticbox = new wxStaticBox(this, wxID_ANY, _("View Options"));
    sizer_viewer_staticbox = new wxStaticBox(this, wxID_ANY, _("Pdf Viewer"));
    sizer_custom_text_staticbox = new wxStaticBox(this, wxID_ANY, _("Custom Text in PDF Output"));
    sizer_save_options_staticbox = new wxStaticBox(this, wxID_ANY, _("Save Options"));
    panel_description = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    const wxString choice_printout_type_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_printout_type = new wxChoice(this, CD_CHOICE_PRINTOUT_TYPE, wxDefaultPosition, wxDefaultSize, 3, choice_printout_type_choices, 0);
    text_printout_description = new wxStaticText(panel_description, wxID_ANY, _("dummy"));
    label_sheetstyle = new wxStaticText(this, wxID_ANY, _("Sheet Style"));
    const wxString choice_sheetstyle_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_sheetstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choice_sheetstyle_choices, 0);
    label_vstyle = new wxStaticText(this, wxID_ANY, _("Vedic Chart Style"));
    const wxString choice_vstyle_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_vstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choice_vstyle_choices, 0);
    label_wstyle = new wxStaticText(this, wxID_ANY, _("Western Chart Style"));
    const wxString choice_wstyle_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_wstyle = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 5, choice_wstyle_choices, 0);
    check_launchviewer = new wxCheckBox(this, CD_LAUNCH_VIEWER, _("Launch Pdf Viewer Automatically"));
    button_launchcommand = new wxButton(this, CD_LAUNCH_COMMAND_BUTTON, _("Command ..."));
    text_launchcommand = new wxTextCtrl(this, CD_LAUNCH_COMMAND_TEXT, wxEmptyString);
    button_test_page = new wxButton(this, CD_PRINT_TEST_PAGE, _("Print Test Page"));
    const wxString choice_custom_text_type_choices[] = {
        _("Header"),
        _("Footer")
    };
    choice_custom_text_type = new wxChoice(this, CD_CHOICE_CUSTOM_TEXT_TYPE, wxDefaultPosition, wxDefaultSize, 2, choice_custom_text_type_choices, 0);
    check_doshow_custom_text = new wxCheckBox(this, CD_SHOW_CUSTOM_TEXT, _("Print Custom Text"));
    button_agenda = new wxBitmapButton(this, CD_AGENDA, wxNullBitmap);
    text_custom = new wxTextCtrl(this, CD_CUSTOM_TEXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    label_orientation = new wxStaticText(this, wxID_ANY, _("Orientation"));
    const wxString choice_orientation_choices[] = {
        _("Left"),
        _("Center"),
        _("Right")
    };
    choice_orientation = new wxChoice(this, CD_ORIENTATION, wxDefaultPosition, wxDefaultSize, 3, choice_orientation_choices, 0);
    check_line_separator = new wxCheckBox(this, CD_LINE_SEPARATOR, _("Line Separator"));
    check_include_first_page = new wxCheckBox(this, CD_FIRST_PAGE, _("Include First Page"));
    label_paper = new wxStaticText(this, wxID_ANY, _("Paper Format"));
    const wxString choice_paperformat_choices[] = {
        _("A4 (210x297mm)"),
        _("Letter (8.5x11 in)"),
        _("Legal (8.5x14 in)")
    };
    choice_paperformat = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 3, choice_paperformat_choices, 0);
    check_overwrite = new wxCheckBox(this, wxID_ANY, _("Ask on Overwrite"));

    set_properties();
    do_layout();
    // end wxGlade

	choice_printout_type->Clear();
	choice_printout_type->Append( PrintoutConfigLoader::get()->getNamesAsArray());
	choice_printout_type->SetValidator( MChoiceValidator( &pconfig->defaultPrintout ));

	choice_sheetstyle->Clear();
	choice_sheetstyle->Append( SheetConfigLoader::get()->getNamesAsArray());
	choice_sheetstyle->SetValidator( MChoiceValidator( &pconfig->sheetStyle ));

	choice_vstyle->Clear();
	choice_vstyle->Append( VedicChartConfigLoader::get()->getNamesAsArray());
	choice_vstyle->SetValidator( MChoiceValidator( &pconfig->vGraphicSkin ));

	choice_wstyle->Clear();
	choice_wstyle->Append( WesternChartConfigLoader::get()->getNamesAsArray());
	choice_wstyle->SetValidator( MChoiceValidator( &pconfig->wGraphicSkin ));

	choice_paperformat->SetValidator( MChoiceValidator( &pconfig->paperFormat ));
	check_overwrite->SetValidator( MCheckValidator( &pconfig->askOverwrite ));
	check_launchviewer->SetValidator( MCheckValidator( &pconfig->launchPdfViewer ));

#ifdef __WXMSW__
	button_launchcommand->Enable( false );
	text_launchcommand->Enable( false );
#else

#ifndef __WXMAC__
	text_launchcommand->SetValidator( MFilenameValidator( &pconfig->pdfViewerCommand ));
#else
	text_launchcommand->SetValidator( MTextValidator( &pconfig->pdfViewerCommand ));
#endif

	button_launchcommand->Enable( pconfig->launchPdfViewer );
	text_launchcommand->Enable( pconfig->launchPdfViewer );
#endif

	Connect( CD_CHOICE_PRINTOUT_TYPE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PrintoutPanel::OnCommandEvent ));
	Connect( CD_LAUNCH_VIEWER, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PrintoutPanel::OnCommandEvent ));

	Connect( CD_LAUNCH_COMMAND_BUTTON, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PrintoutPanel::OnChooseCommand ));
	Connect( CD_LAUNCH_COMMAND_TEXT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PrintoutPanel::OnTextChanged ));

	Connect( CD_SHOW_CUSTOM_TEXT, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PrintoutPanel::OnShowCustomText ));

	Connect( CD_CHOICE_CUSTOM_TEXT_TYPE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PrintoutPanel::OnCommandEvent ));
	Connect( CD_CUSTOM_TEXT, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( PrintoutPanel::OnCustomTextParamChange ));
	Connect( CD_ORIENTATION, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PrintoutPanel::OnCustomTextParamChange ));
	Connect( CD_LINE_SEPARATOR, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PrintoutPanel::OnCustomTextParamChange ));
	Connect( CD_FIRST_PAGE, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( PrintoutPanel::OnCustomTextParamChange ));
	Connect( CD_PRINT_TEST_PAGE, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PrintoutPanel::OnPrintTestPage ));
	Connect( CD_AGENDA, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PrintoutPanel::OnAgenda ));
}

/*****************************************************
**
**   PrintoutPanel   ---   standard override methods
**
******************************************************/
PrintoutPanel::~PrintoutPanel() { delete pconfig; }
void PrintoutPanel::config2model() { *pconfig = *config->print; }
void PrintoutPanel::model2config() { *config->print = *pconfig; }
void PrintoutPanel::restoreDefaults() { *pconfig = PdfDocumentConfig(); }

/*****************************************************
**
**   PrintoutPanel   ---   updateUi
**
******************************************************/
void PrintoutPanel::updateUi()
{
	//printf ( "PrintoutPanel::updateUi\n" );

	// 1st step: update text filed of printout type selection
	wxString s;

	if ( choice_printout_type->GetSelection() != -1 )
	{
		PrintoutConfig *pp = PrintoutConfigLoader::get()->getConfig( choice_printout_type->GetSelection() );
		text_printout_description->SetLabel( pp->description );

		const wxSize size = panel_description->GetSize();
		text_printout_description->Wrap( (int)size.x ); // - 12 );
	}

	const int sel = choice_custom_text_type->GetSelection();
	//printf( " PrintoutPanel custom text type %d\n", sel );
	switch ( sel )
	{
		case 0:
			check_doshow_custom_text->SetValue( pconfig->customHeader.enabled );
			text_custom->ChangeValue( pconfig->customHeader.text );
			choice_orientation->SetSelection( pconfig->customHeader.orientation );
			check_line_separator->SetValue( pconfig->customHeader.showSeparator );
			check_include_first_page->SetValue( pconfig->customHeader.showOnFirstPage );
		break;
		case 1:
			check_doshow_custom_text->SetValue( pconfig->customFooter.enabled );
			text_custom->ChangeValue( pconfig->customFooter.text );
			choice_orientation->SetSelection( pconfig->customFooter.orientation );
			check_line_separator->SetValue( pconfig->customFooter.showSeparator );
			check_include_first_page->SetValue( pconfig->customFooter.showOnFirstPage );
		break;
		default:
			assert( false );
		break;
	}

	const bool enabled = sel ? pconfig->customFooter.enabled : pconfig->customHeader.enabled;
	text_custom->Enable( enabled );
	label_orientation->Enable( enabled );
	choice_orientation->Enable( enabled );
	check_line_separator->Enable( enabled );
	check_include_first_page->Enable( enabled );
	button_agenda->Enable( enabled );
}

/*****************************************************
**
**   PrintoutPanel   ---   OnCommandEvent
**
******************************************************/
void PrintoutPanel::OnCommandEvent( wxCommandEvent &event )
{
	event.Skip();
	updateUi();
}

/*****************************************************
**
**   PrintoutPanel   ---   OnTextChanged
**
******************************************************/
void PrintoutPanel::OnTextChanged( wxCommandEvent& )
{
	//printf( "PrintoutPanel::OnTextChanged NOT CAPTURED\n" );
	Validate();
	setDirty();
}

/*****************************************************
**
**   PrintoutPanel   ---   OnCustomTextParamChange
**
******************************************************/
void PrintoutPanel::OnCustomTextParamChange( wxCommandEvent& )
{
	const int sel = choice_custom_text_type->GetSelection();
	//printf( "PrintoutPanel::OnCustomTextParamChange SEL %d\n", sel );
	saveCustomTextParams( sel );
	setDirty();
}

/*****************************************************
**
**   PrintoutPanel   ---   OnShowCustomText
**
******************************************************/
void PrintoutPanel::OnShowCustomText( wxCommandEvent&  )
{
	const int sel = choice_custom_text_type->GetSelection();
	//printf( "PrintoutPanel::OnShowCustomText SEL %d\n", sel );
	saveCustomTextParams( sel );
	updateUi();
}

/*****************************************************
**
**   PrintoutPanel   ---   saveCustomTextParams
**
******************************************************/
void PrintoutPanel::saveCustomTextParams( const int &sel )
{
	//printf( "PrintoutPanel::saveCustomTextParams SEL %d\n", sel );
	switch( sel )
	{
		case 0:
			pconfig->customHeader.enabled = check_doshow_custom_text->GetValue();
			pconfig->customHeader.text = text_custom->GetValue().Trim();
			pconfig->customHeader.showSeparator = check_line_separator->GetValue();
			pconfig->customHeader.orientation = choice_orientation->GetSelection();
			pconfig->customHeader.showOnFirstPage = check_include_first_page->GetValue();
		break;
		case 1:
			pconfig->customFooter.enabled = check_doshow_custom_text->GetValue();
			pconfig->customFooter.text = text_custom->GetValue().Trim();
			pconfig->customFooter.showSeparator = check_line_separator->GetValue();
			pconfig->customFooter.orientation = choice_orientation->GetSelection();
			pconfig->customFooter.showOnFirstPage = check_include_first_page->GetValue();
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   PrintoutPanel   ---   OnAgenda
**
******************************************************/
void PrintoutPanel::OnAgenda( wxCommandEvent& )
{
	//printf( "AGENDA\n" );
	wxString title, s;
	
	title << wxT( "Agenda of Variables" ) << Endl;
	s << wxT( "$name: name of the native" ) << Endl
		<< wxT( "$date: birth date of the native" ) << Endl
		<< wxT( "$fulldate: birth date and time" ) << Endl
		<< wxT( "$location: location of birth" ) << Endl
		<< wxT( "$page: current page" ) << Endl
		<< wxT( "$version: version of the software" ) << Endl
		<< wxT( "$creationdate: date of document creation" );
	showHelpPopup( this, title, s );
}

/*****************************************************
**
**   PrintoutPanel   ---   OnPrintTestPage
**
******************************************************/
void PrintoutPanel::OnPrintTestPage( wxCommandEvent& )
{
	//printf( "PrintoutPanel::OnPrintTestPage - printout is %d\n", pconfig->defaultPrintout );
	TransferDataFromWindow();
	PdfDocumentConfig p( *pconfig );
	p.askOverwrite = false;

	Horoscope h;
	h.update();

	ChartProperties props;
	props.setWesternSkin( pconfig->wGraphicSkin );
	props.setVedicSkin( pconfig->vGraphicSkin );
	//printf( "PROPS wskin %d vskin %d\n", pconfig->wGraphicSkin, pconfig->vGraphicSkin );

	PdfTool tool( &p );
	tool.printTestpage( &h, &props );
}

/*****************************************************
**
**   PrintoutPanel   ---   OnChooseCommand
**
******************************************************/
void PrintoutPanel::OnChooseCommand( wxCommandEvent& )
{
	wxFileDialog dialog( this, _("Choose Pdf Viewer" ));

	if ( dialog.ShowModal() == wxID_OK )
	{
		pconfig->pdfViewerCommand = dialog.GetPath();
		text_launchcommand->ChangeValue( pconfig->pdfViewerCommand );
		setDirty();
	}
}

/*****************************************************
**
**   PrintoutPanel   ---   set_properties
**
******************************************************/
void PrintoutPanel::set_properties()
{
	ImageProvider *ip = ImageProvider::get();
	button_agenda->SetBitmapLabel( ip->getBitmap( BM_TIP ));
	//button_agenda->SetBitmapLabel( ip->getScaledBitmap( BM_TIP, 24 ));

    // begin wxGlade: PrintoutPanel::set_properties
    choice_printout_type->SetSelection(0);
    panel_description->SetMinSize(wxSize(-1, 100));
    choice_sheetstyle->SetSelection(0);
    choice_vstyle->SetSelection(0);
    choice_wstyle->SetSelection(0);
    choice_custom_text_type->SetSelection(0);
    button_agenda->SetMinSize(wxSize(32, 32));
    button_agenda->SetToolTip(_("Explain signification of variables"));
    text_custom->SetMinSize(wxSize(300,100));
    choice_orientation->SetSelection(0);
    choice_paperformat->SetSelection(0);
    // end wxGlade
	text_printout_description->SetBackgroundColour( GetBackgroundColour() );
	text_printout_description->SetForegroundColour( GetForegroundColour() );
}

/*****************************************************
**
**   PrintoutPanel   ---   do_layout
**
******************************************************/
void PrintoutPanel::do_layout()
{
    // begin wxGlade: PrintoutPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_save_options_staticbox->Lower();
    wxStaticBoxSizer* sizer_save_options = new wxStaticBoxSizer(sizer_save_options_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_paper_format = new wxBoxSizer(wxHORIZONTAL);
    sizer_custom_text_staticbox->Lower();
    wxStaticBoxSizer* sizer_custom_text = new wxStaticBoxSizer(sizer_custom_text_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_custom_text = new wxFlexGridSizer(7, 1, 0, 0);
    wxFlexGridSizer* grid_orientation = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* grid_header = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(4, 1, 0, 0);
    sizer_viewer_staticbox->Lower();
    wxStaticBoxSizer* sizer_viewer = new wxStaticBoxSizer(sizer_viewer_staticbox, wxVERTICAL);
    sizer_view_options_staticbox->Lower();
    wxStaticBoxSizer* sizer_view_options = new wxStaticBoxSizer(sizer_view_options_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_options = new wxFlexGridSizer(2, 1, 3, 3);
    wxFlexGridSizer* grid_options2 = new wxFlexGridSizer(3, 2, 3, 3);
    sizer_printout_staticbox->Lower();
    wxStaticBoxSizer* sizer_printout = new wxStaticBoxSizer(sizer_printout_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_printout2 = new wxFlexGridSizer(7, 1, 0, 0);
    wxFlexGridSizer* sizer_description = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_printout2->Add(choice_printout_type, 0, wxALL, 3);
    sizer_description->Add(text_printout_description, 0, wxALL|wxEXPAND, 3);
    panel_description->SetSizer(sizer_description);
    sizer_description->AddGrowableRow(0);
    sizer_description->AddGrowableCol(0);
    sizer_printout2->Add(panel_description, 1, wxALL|wxEXPAND, 3);
    sizer_printout2->AddGrowableRow(2);
    sizer_printout2->AddGrowableCol(0);
    sizer_printout->Add(sizer_printout2, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_printout, 1, wxEXPAND, 3);
    grid_options2->Add(label_sheetstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options2->Add(choice_sheetstyle, 0, wxALL|wxEXPAND, 3);
    grid_options2->Add(label_vstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options2->Add(choice_vstyle, 0, wxALL|wxEXPAND, 3);
    grid_options2->Add(label_wstyle, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options2->Add(choice_wstyle, 0, wxALL|wxEXPAND, 3);
    grid_options->Add(grid_options2, 1, wxALL, 3);
    sizer_view_options->Add(grid_options, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_view_options, 1, wxEXPAND, 0);
    sizer_viewer->Add(check_launchviewer, 0, wxALL|wxEXPAND, 3);
    sizer_viewer->Add(button_launchcommand, 0, wxALL, 3);
    sizer_viewer->Add(text_launchcommand, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
    sizer_left->Add(sizer_viewer, 1, wxALL|wxEXPAND, 3);
    sizer_left->Add(button_test_page, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_left->AddGrowableCol(0);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    grid_custom_text->Add(choice_custom_text_type, 0, wxALL, 3);
    grid_header->Add(check_doshow_custom_text, 0, wxALL, 3);
    grid_header->Add(button_agenda, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_header->AddGrowableCol(1);
    grid_custom_text->Add(grid_header, 1, wxEXPAND, 0);
    grid_custom_text->Add(text_custom, 0, wxALL|wxEXPAND, 3);
    grid_orientation->Add(label_orientation, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_orientation->Add(choice_orientation, 0, wxALL, 3);
    grid_orientation->AddGrowableCol(0);
    grid_custom_text->Add(grid_orientation, 1, wxALL|wxEXPAND, 3);
    grid_custom_text->Add(check_line_separator, 0, wxALL|wxEXPAND, 3);
    grid_custom_text->Add(check_include_first_page, 0, wxALL|wxEXPAND, 3);
    grid_custom_text->AddGrowableRow(2);
    grid_custom_text->AddGrowableCol(0);
    sizer_custom_text->Add(grid_custom_text, 1, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_custom_text, 1, wxALL|wxEXPAND, 3);
    sizer_paper_format->Add(label_paper, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5);
    sizer_paper_format->Add(choice_paperformat, 0, wxALL, 3);
    sizer_save_options->Add(sizer_paper_format, 1, wxALL|wxEXPAND, 3);
    sizer_save_options->Add(check_overwrite, 0, wxALL|wxEXPAND, 3);
    sizer_right->Add(sizer_save_options, 1, wxEXPAND, 0);
    sizer_right->AddGrowableRow(2);
    sizer_right->AddGrowableCol(0);
    sizer_main->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    // end wxGlade
}


/*****************************************************
**
**   ConfigPanelFactory   ---   createPrintoutPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory:: createPrintoutPanel( wxWindow *parent )
{
	return new PrintoutPanel( parent );
}



