/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/guibase.cpp
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

#include "Commands.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "Painter.h"
#include "Sheet.h"

#include <wx/app.h>
#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choicdlg.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/panel.h>
#include <wx/popupwin.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/statline.h>
#include <wx/stattext.h>

#include "Conf.h"

extern Config *config;

DEFINE_EVENT_TYPE( CREATE_ENTRY_CHART )

/*************************************************//**
*
* \brief Popup window for variable explanation
*
******************************************************/
class SimpleHelpPopupWidget : public wxPopupTransientWindow
{
public:
  SimpleHelpPopupWidget( wxWindow *parent, wxString title, wxString message )
   : wxPopupTransientWindow( parent )
  {
		FontProvider *fc = FontProvider::get();
    wxPanel *panel = new wxPanel( this, wxID_ANY );

    wxStaticText *text1 = new wxStaticText( panel, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTRE );
		text1->SetFont( *fc->getFont( FONT_TEXT_TABLE_HEADER ));

    wxStaticText *text2 = new wxStaticText( panel, wxID_ANY, message );
		text2->SetFont( *fc->getFont( FONT_TEXT_TABLE_CONTENTS ));

    wxBoxSizer *sizer = new wxBoxSizer( wxVERTICAL );
    sizer->Add( text1, 0, wxALL | wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 3 );
    sizer->Add( new wxStaticLine( panel ), 0, wxALL | wxEXPAND, 0 );
    sizer->Add( text2, 0, wxALL, 3 );
    panel->SetSizer( sizer );
    sizer->Fit( panel );
    SetClientSize( panel->GetSize() );
  }
};

/**************************************************************
***
**   showHelpPopup
***
***************************************************************/
void showHelpPopup( wxWindow *parent, wxString header, wxString contents )
{
	SimpleHelpPopupWidget *popup = new SimpleHelpPopupWidget( parent, header, contents );

	wxMouseState m = wxGetMouseState();

	// wx 2.8 doesnt have that method
	//const wxPoint pos = m.GetPosition();
	const wxPoint pos( m.GetX(), m.GetY());
	popup->Position( pos, wxSize( 0, 0 ) );
	popup->Popup(); 
}

/**************************************************************
***
**    doMessageBox
***
***************************************************************/
int doMessageBox( wxWindow *parent, const wxString message, const long style )
{
	return wxMessageDialog( parent, message, GUI_APP_NAME, style | wxCENTRE ).ShowModal();
}

#define IMAGE_HASH_SIZE 10

/**************************************************************
***
**    createImageHash
***
***************************************************************/
wxString createImageHash( wxImage *image )
{
	wxString s;
	unsigned char *data = image->GetData();

	// 3 chars rgb for each pixel
	const int imagesize = 3 * ( image->GetWidth() * image->GetHeight());

	for( int i = 0; i < IMAGE_HASH_SIZE && i < imagesize; i++ )
	{
		s << *data;
		data++;
	}

	//printf( "size %d s Hash %s\n", imagesize, str2char( s ) );
	return s;
}

/*************************************************//**
*
*    DoNotShowAgainDialog   ---   Constructor
*
******************************************************/
DoNotShowAgainDialog::DoNotShowAgainDialog( wxWindow *parent, wxString message, bool *item, const int style )
	:	wxDialog( parent, -1, _( "Information" ), wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE ),
	cfgitem( item )
{
	wxButton *okbutton = (wxButton*)NULL;
	wxButton *cancelbutton = (wxButton*)NULL;
	wxButton *yesbutton = (wxButton*)NULL;
	wxButton *nobutton = (wxButton*)NULL;

	wxBoxSizer* sizer_main = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* sizer_lr = new wxBoxSizer( wxHORIZONTAL );
	wxBoxSizer* sizer_text = new wxBoxSizer( wxVERTICAL );
	wxBoxSizer* sizer_button = new wxBoxSizer( wxHORIZONTAL );

	wxStaticText *messageLabel = new wxStaticText(this, wxID_ANY, message );
	messageLabel->Wrap( 500 );

	//wxCheckBox(this, wxID_ANY, _("Use Refraction"));
	check_not_ask_again = new wxCheckBox(this, wxID_ANY, _("Do not show this message again"));
	//check_not_ask_again->SetValue( *cfgitem );

	if ( style & wxOK )
	{
		okbutton = new wxButton( this, wxID_OK, _( "OK" ));
		sizer_button->Add( okbutton, 0, wxALL|wxALIGN_CENTER, 3 );
	}
	if ( style & wxCANCEL )
	{
		cancelbutton = new wxButton( this, wxID_CANCEL, _( "Cancel" ));
		sizer_button->Add( cancelbutton, 0, wxALL|wxALIGN_CENTER, 3 );
	}
	if ( style & wxYES_NO )
	{
		yesbutton = new wxButton( this, wxID_YES, _( "Yes" ));
		sizer_button->Add( yesbutton, 0, wxALL|wxALIGN_CENTER, 3 );
		nobutton = new wxButton( this, wxID_NO, _( "No" ));
		sizer_button->Add( nobutton, 0, wxALL|wxALIGN_CENTER, 3 );
	}

	wxStaticBitmap *bmp = new wxStaticBitmap( this, -1, ImageProvider::get()->getBitmap( BM_MAITREYA ));
	sizer_lr->Add( bmp, 0, wxALL|wxALIGN_CENTER_VERTICAL, 30 );

	sizer_text->Add( messageLabel, 0, wxALL, 5 );
	sizer_text->Add( check_not_ask_again, 0, wxALL, 5 );
	sizer_lr->Add( sizer_text );

	sizer_main->Add( sizer_lr );
	sizer_main->Add( sizer_button, 0, wxALL|wxALIGN_CENTER, 3 );
	SetSizer(sizer_main);
	sizer_main->Fit(this);
	Layout();

	if ( okbutton ) okbutton->SetFocus();
	else if ( yesbutton ) yesbutton->SetFocus();

	Connect( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DoNotShowAgainDialog::OnButton ));
}


/*************************************************//**
*
*    DoNotShowAgainDialog   ---   OnButton
*
******************************************************/
void DoNotShowAgainDialog::OnButton( wxCommandEvent &event )
{
	//*cfgitem = check_not_ask_again->IsChecked();
	if ( check_not_ask_again->IsChecked()) *cfgitem = false;
	EndModal( event.GetId() );
}

