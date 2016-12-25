/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ConfigDialog.cpp
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

#include "ConfigDialog.h"

#include <wx/app.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/treebook.h>

#include "Conf.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "guibase.h"

enum { CONFIG_NOTEBOOK = wxID_HIGHEST + 3000, CD_RESTORE_DEFAULTS };

IMPLEMENT_CLASS( ConfigBaseDialog, wxDialog )
IMPLEMENT_CLASS( ConfigDialog, ConfigBaseDialog )
IMPLEMENT_CLASS( SimpleConfigDialog, ConfigBaseDialog )

extern Config *config;

/*****************************************************
**
**   SimpleConfigDialog   ---   Constructor
**
******************************************************/
SimpleConfigDialog::SimpleConfigDialog( wxWindow* parent, const int &id )
		:  ConfigBaseDialog( parent )
{
	ConfigPanelFactory factory;

	wxBoxSizer* sizer_main = new wxBoxSizer( wxVERTICAL );
	configpanel = factory.createPanel( id, this );
	configpanel->InitDialog();
	configpanel->Validate();

	sizer_main->Add( configpanel, 1, wxALL|wxEXPAND, 3);
	sizer_main->Add( new wxStaticLine(this, -1), 0, wxEXPAND, 0);
	sizer_main->Add(buttonsizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxFIXED_MINSIZE , 3);
	SetAutoLayout( true );
	SetSizer( sizer_main );
	Layout();
	SetIcon( ImageProvider::get()->getIcon( BM_CONFIG ));

	wxString title;
	title << _( "Configuration" ) << wxT( ": " ) << factory.getPanelName( id );
	SetTitle( title );

	SetSize( config->viewprefs->sizes.sSimpleConfigDialog );
	Connect( wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SimpleConfigDialog::OnApply ));
}

/*****************************************************
**
**   SimpleConfigDialog   ---   Destructor
**
******************************************************/
SimpleConfigDialog::~SimpleConfigDialog()
{
	config->viewprefs->sizes.sSimpleConfigDialog = GetSize();
}

/*****************************************************
**
**   SimpleConfigDialog   ---   saveData
**
******************************************************/
void SimpleConfigDialog::saveData()
{
	configpanel->TransferDataFromWindow();
	configpanel->model2config();
}

/*****************************************************
**
**   SimpleConfigDialog   ---   OnApply
**
******************************************************/
void SimpleConfigDialog::OnApply( wxCommandEvent& )
{
	saveData();
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   ConfigBaseDialog   ---   Constructor
**
******************************************************/
ConfigBaseDialog::ConfigBaseDialog( wxWindow* parent )
		:  wxDialog( parent, -1, _( "Configuration" ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
	buttonsizer = new wxBoxSizer( wxHORIZONTAL );
	ok_button =new wxButton(this, wxID_OK, _("OK"));
	apply_button = new wxButton(this, wxID_APPLY, _("Apply"));
	restore_button = new wxButton( this, CD_RESTORE_DEFAULTS, _( "Restore Defaults" ));
	cancel_button = new wxButton(this, wxID_CANCEL, _("Cancel"));
	buttonsizer->Add( ok_button, 0, wxALL, 3);
	buttonsizer->Add( apply_button, 0, wxALL, 3);
	buttonsizer->Add( restore_button, 0, wxALL, 3);
	buttonsizer->Add( cancel_button, 0, wxALL, 3);

	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigBaseDialog::OnOK ));
	Connect( wxID_CANCEL, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigBaseDialog::OnCancel ));
	Connect( wxID_ANY, CONFIG_PANEL_CHANGED, wxCommandEventHandler( ConfigBaseDialog::OnConfigPanelChanged ));
}

/*****************************************************
**
**   ConfigBaseDialog   ---   OnConfigPanelChanged
**
******************************************************/
void ConfigBaseDialog::OnConfigPanelChanged( wxCommandEvent &e )
{
	//printf( "ConfigBaseDialog::OnConfigPanelChanged int is %d\n", e.GetInt() );
	apply_button->Enable( e.GetInt() );
}

/*****************************************************
**
**   ConfigDialog   ---   Constructor
**
******************************************************/
ConfigDialog::ConfigDialog( wxWindow* parent )
		:  ConfigBaseDialog( parent )
{
	ConfigPanelFactory factory;
	const static int page_types[NB_PANELS] = {
		1, 1, 1, 1, 1, 1, // General
		1, 0, 0, 0, 0,    // User Interface
		1, 0, 0,          // Vedic
		1, 0, 0, 0,       // Western
		1                 // Miscellaneous
	};
	wxString title;

	wxBoxSizer* sizer_main = new wxBoxSizer( wxVERTICAL );
	notebook = new wxTreebook( this, CONFIG_NOTEBOOK );
	for ( int i = CONFIGPANEL_FIRST; i <= CONFIGPANEL_LAST; i++ )
	{
		title = factory.getPanelName( i );
		configpanel[i] = 0;
		panel[i] = new wxPanel( notebook, -1 );
		if ( page_types[i] )
		{
			notebook->AddPage( panel[i], title );
		}
		else
		{
			notebook->AddSubPage( panel[i], title );
		}
	}

	sizer_main->Add( notebook, 1, wxALL|wxEXPAND, 3);
	sizer_main->Add( new wxStaticLine(this, -1), 0, wxEXPAND, 0);
	sizer_main->Add(buttonsizer, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxFIXED_MINSIZE , 3);
	SetAutoLayout( true );
	SetSizer( sizer_main );
	Layout();
	SetIcon( ImageProvider::get()->getIcon( BM_CONFIG ));

	SetSize( config->viewprefs->sizes.sConfigDialog );
	if ( config->viewprefs->pConfigDialog.x > 0 || config->viewprefs->pConfigDialog.y > 0 )
	{
		SetPosition( wxPoint( config->viewprefs->pConfigDialog.x, config->viewprefs->pConfigDialog.y ));
	}

	Connect( wxID_APPLY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigDialog::OnApply ));
	Connect( CD_RESTORE_DEFAULTS, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ConfigDialog::OnRestoreDefaults ));
#ifdef _WX_V2_
	Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGING, wxTreebookEventHandler( ConfigDialog::OnTreebookChanging ));
	Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED, wxTreebookEventHandler( ConfigDialog::OnTreebookChanged ));
#else
	Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGING, wxBookCtrlEventHandler( ConfigDialog::OnTreebookChanging ));
	Connect( CONFIG_NOTEBOOK, wxEVT_COMMAND_TREEBOOK_PAGE_CHANGED, wxBookCtrlEventHandler( ConfigDialog::OnTreebookChanged ));
#endif

	// Uncomment to expand all base nodes
	//for ( int i = CONFIGPANEL_FIRST; i <= CONFIGPANEL_LAST; i++ ) notebook->ExpandNode( i, true );
	notebook->SetSelection( config->viewprefs->configDialogActivePage );
	notebook->ExpandNode( config->viewprefs->configDialogActivePage );
}

/*****************************************************
**
**   ConfigDialog   ---   Destructor
**
******************************************************/
ConfigDialog::~ConfigDialog()
{
	config->viewprefs->configDialogActivePage = notebook->GetSelection();
	config->viewprefs->sizes.sConfigDialog = GetSize();
	const wxPoint p = GetPosition();
	config->viewprefs->pConfigDialog = wxSize( p.x, p.y );
}

/*****************************************************
**
**   ConfigDialog   ---   saveData
**
******************************************************/
void ConfigDialog::saveData()
{
	for ( int i = 0; i < NB_PANELS; i++ )
	{
		if ( configpanel[i] && configpanel[i]->isDirty() )
		{
			configpanel[i]->TransferDataFromWindow();
			configpanel[i]->model2config();

			if ( i == CONFIGPANEL_TOOLBAR )
			{
				wxCommandEvent e( CONFIG_TOOLBAR_CHANGED, GetId() );
				wxPostEvent( GetParent(), e );
			}
			configpanel[i]->setDirty( false );
		}
	}
}

/*****************************************************
**
**   ConfigDialog   ---   showPanel
**
******************************************************/
ConfigPanel *ConfigDialog::showPanel( const int &sel )
{
	ConfigPanelFactory factory;

	assert( sel >= 0 && sel < NB_PANELS );
	wxString title;
	title << _( "Configuration" ) << wxT( ": " ) << factory.getPanelName( sel );
	SetTitle( title );
	if ( ! configpanel[sel] )
	{
		panel[sel]->Show( false );
		configpanel[sel] = factory.createPanel( sel, panel[sel] );
		configpanel[sel]->InitDialog();
		configpanel[sel]->Validate();

    wxBoxSizer* sizer= new wxBoxSizer( wxVERTICAL );
		sizer->Add( configpanel[sel], 1, wxEXPAND, 0 );

		/*
		if ( configpanel[sel]->showDefaultButton )
		{
			wxButton *bb = new wxButton( panel[sel], CD_RESTORE_DEFAULTS, wxT( "Restore Defaults" ));
			sizer->Add( bb, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 3 );
		}
		*/

		panel[sel]->SetSizer(sizer);
		panel[sel]->Layout();
		panel[sel]->Show( true );
		//panel[sel]->SetFocus();
	}
	configpanel[sel]->onActivate();
	restore_button->Enable( configpanel[sel]->hasRestoreButton() );
	apply_button->Enable( configpanel[sel]->isDirty());
	return configpanel[sel];
}

/*****************************************************
**
**   ConfigDialog   ---   OnTreebookChanging
**
******************************************************/
#ifdef _WX_V2_
void ConfigDialog::OnTreebookChanging( wxTreebookEvent &event )
#else
void ConfigDialog::OnTreebookChanging( wxBookCtrlEvent &event )
#endif
{
	//printf( "ConfigDialog::OnTreebookChanging sel %d old sel %d\n", event.GetOldSelection(), event.GetSelection());
	if ( event.GetOldSelection() != -1 && configpanel[event.GetOldSelection()] != 0 )
	{
		bool dirty = configpanel[event.GetOldSelection()]->isDirty();
		if ( dirty )
		{
			const int a = doMessageBox( this, _( "Configuration has changed. Do you want to save?" ), wxYES_NO | wxCANCEL | wxCENTRE | wxICON_QUESTION );
			//printf( "DIRTYi %d\n", a );

			switch( a )
			{
				case wxID_YES:
					doApply();
				break;
				case wxID_NO:
					configpanel[event.GetOldSelection()]->setDirty( false );
					configpanel[event.GetOldSelection()]->InitDialog();
					configpanel[event.GetOldSelection()]->Validate();
				break;
				case wxID_CANCEL:
					event.Veto();
					return;
				break;
				default:
					assert( false );
				break;
			}
		}
	}
}

/*****************************************************
**
**   ConfigDialog   ---   OnTreebookChanged
**
******************************************************/
#ifdef _WX_V2_
void ConfigDialog::OnTreebookChanged( wxTreebookEvent &event )
#else
void ConfigDialog::OnTreebookChanged( wxBookCtrlEvent &event )
#endif
{
	//printf( "ConfigDialog::OnTreebookChanged sel %d old sel %d\n", event.GetOldSelection(), event.GetSelection());
	showPanel( event.GetSelection());
}

/*****************************************************
**
**   ConfigDialog   ---   OnApply
**
******************************************************/
void ConfigDialog::OnApply( wxCommandEvent& )
{
	doApply();
}

/*****************************************************
**
**   ConfigDialog   ---   doApply
**
******************************************************/
void ConfigDialog::doApply()
{
	saveData();
	for ( int i = CONFIGPANEL_FIRST; i <= CONFIGPANEL_LAST; i++ )
	{
		if ( configpanel[i] )
		{
			configpanel[i]->InitDialog();
			configpanel[i]->Validate();
		}
	}
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   ConfigDialog   ---   OnSize
**
******************************************************/
void ConfigDialog::OnSize( wxSizeEvent &event )
{
	config->viewprefs->sizes.sConfigDialog = event.GetSize();
	event.Skip();
}

/*****************************************************
**
**   ConfigBaseDialog   ---   OnOK
**
******************************************************/
void ConfigBaseDialog::OnOK( wxCommandEvent& )
{
	saveData();
	wxCommandEvent e( CONFIG_CHANGED, GetId() );
	wxPostEvent( GetParent(), e );
	EndModal( wxID_OK );
}

/*****************************************************
**
**   ConfigBaseDialog   ---   OnCancel
**
******************************************************/
void ConfigBaseDialog::OnCancel( wxCommandEvent& )
{
	EndModal( wxID_CANCEL );
}

/*****************************************************
**
**   ConfigDialog   ---   OnRestoreDefaults
**
******************************************************/
void ConfigDialog::OnRestoreDefaults( wxCommandEvent& )
{
	const int sel = notebook->GetSelection();
	//printf( "ConfigDialog::OnRestoreDefaults sel %d\n", sel );
	if ( sel != wxNOT_FOUND )
	{
		assert( sel >= 0 && sel < NB_PANELS );
		configpanel[sel]->restoreDefaults();
		configpanel[sel]->TransferDataToWindow();
		configpanel[sel]->updateUi();
		configpanel[sel]->Validate();
		configpanel[sel]->setDirty();
	}
}


