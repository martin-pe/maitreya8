/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/FontPanel.cpp
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

#include "FontPanel.h"

#include "Conf.h"
#include "FontProvider.h"

#include <wx/button.h>
#include <wx/font.h>
#include <wx/fontdlg.h>
#include <wx/fontenum.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

// fontdialog crashes otherwise
#ifdef __WXMAC__
	#undef wxFontDialog
	#include "wx/osx/fontdlg.h"
#endif

extern Config *config;
enum { CD_GRAPHIC_DEFAULT = wxID_HIGHEST + 1, CD_GRAPHIC_SYMBOLS, 
	CD_TEXT_DEFAULT, CD_TEXT_HEADER, CD_TEXT_TABLE_CONTENTS, CD_TEXT_TABLE_HEADER,
	CD_PDF_DEFAULT, CD_PDF_HEADER, CD_PDF_TABLE_CONTENTS, CD_PDF_TABLE_HEADER, CD_PDF_TINY,
	CD_PLAIN_DEFAULT };

#define CD_FIRST_FONT CD_GRAPHIC_DEFAULT

IMPLEMENT_CLASS( FontPanel, ConfigPanel )

/*****************************************************
**
**   FontPanel   ---   Constructor
**
******************************************************/
FontPanel::FontPanel( wxWindow* parent ) : ConfigPanel( parent )
{
    // begin wxGlade: FontPanel::FontPanel
    sizer_graphic_views_staticbox = new wxStaticBox(this, wxID_ANY, _("Graphical Views"));
    sizer_plaintext_staticbox = new wxStaticBox(this, wxID_ANY, _("Plain Text Views"));
    sizer_pdfviews_staticbox = new wxStaticBox(this, wxID_ANY, _("Printout (Pdf)"));
    sizer_textviews_staticbox = new wxStaticBox(this, wxID_ANY, _("Text Views"));
    l_text_default = new wxStaticText(this, wxID_ANY, _("Default Text"));
    t_text_default = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_text_default = new wxButton(this, CD_TEXT_DEFAULT, _("Choose..."));
    l_text_header = new wxStaticText(this, wxID_ANY, _("Text Header"));
    t_text_header = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_text_header = new wxButton(this, CD_TEXT_HEADER, _("Choose..."));
    l_text_table_contents = new wxStaticText(this, wxID_ANY, _("Table Contents"));
    t_text_table_contents = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_text_table_contents = new wxButton(this, CD_TEXT_TABLE_CONTENTS, _("Choose..."));
    l_text_table_header = new wxStaticText(this, wxID_ANY, _("Table Header"));
    t_text_table_header = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_text_table_header = new wxButton(this, CD_TEXT_TABLE_HEADER, _("Choose..."));
    l_graphic_default = new wxStaticText(this, wxID_ANY, _("Text"));
    t_graphic_default = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_graphic_default = new wxButton(this, CD_GRAPHIC_DEFAULT, _("Choose..."));
    l_graphic_symbols = new wxStaticText(this, wxID_ANY, _("Symbols"));
    t_graphic_symbols = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_graphic_symbols = new wxButton(this, CD_GRAPHIC_SYMBOLS, _("Choose..."));
    l_plain_default = new wxStaticText(this, wxID_ANY, _("Default Text"));
    t_plain_default = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_plain_default = new wxButton(this, CD_PLAIN_DEFAULT, _("Choose..."));
    l_pdf_default = new wxStaticText(this, wxID_ANY, _("Default Text"));
    t_pdf_default = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_pdf_default = new wxButton(this, CD_PDF_DEFAULT, _("Choose..."));
    l_pdf_header = new wxStaticText(this, wxID_ANY, _("Text Header"));
    t_pdf_header = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_pdf_header = new wxButton(this, CD_PDF_HEADER, _("Choose..."));
    l_pdf_table_contents = new wxStaticText(this, wxID_ANY, _("Table Contents"));
    t_pdf_table_contents = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_pdf_table_contents = new wxButton(this, CD_PDF_TABLE_CONTENTS, _("Choose..."));
    l_pdf_table_header = new wxStaticText(this, wxID_ANY, _("Table Header"));
    t_pdf_table_header = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_pdf_table_header = new wxButton(this, CD_PDF_TABLE_HEADER, _("Choose..."));
    l_pdf_tiny = new wxStaticText(this, wxID_ANY, _("Page Header and Footer"));
    t_pdf_tiny = new wxTextCtrl(this, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    b_pdf_tiny = new wxButton(this, CD_PDF_TINY, _("Choose..."));

    set_properties();
    do_layout();
    // end wxGlade

	Connect( wxID_ANY, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( FontPanel::OnChooseFont ));

	setTextControlItems();
}

/*****************************************************
**
**   FontPanel   ---   config2model
**
******************************************************/
void FontPanel::config2model()
{
}

/*****************************************************
**
**   FontPanel   ---   model2config
**
******************************************************/
void FontPanel::model2config()
{
	FontProvider *fc = FontProvider::get();
	for( int id = 0; id < MAX_FONTS; id++ )
	{
		fc->setFont( (FONT_ID)id, fontId2TextCtrl( id )->GetFont() );
	}
}

/*****************************************************
**
**   FontPanel   ---   setTextControlItems
**
******************************************************/
void FontPanel::setTextControlItems()
{
	FontProvider *fc = FontProvider::get();
	for( int id = 0; id < MAX_FONTS; id++ )
	{
		setTextControlItem( (FONT_ID)id, fc->getFont( (FONT_ID)id ));
	}
}

/*****************************************************
**
**   FontPanel   ---   setTextControlItem
**
******************************************************/
void FontPanel::setTextControlItem( const FONT_ID &id, wxFont *font )
{
	wxString s;
	wxTextCtrl *ctrl = fontId2TextCtrl( id );
	ctrl->SetFont( *font );

	switch( id )
	{
		case  FONT_GRAPHIC_SYMBOLS:
			s = wxT( "ABC abc" );
		break;
		default:
			s << font->GetFaceName() << wxString::Format( wxT( " %d" ), font->GetPointSize() );
		break;
	}
	ctrl->ChangeValue( s );
	ctrl->Refresh();
}

/*****************************************************
**
**   FontPanel   ---   commandId2TextCtrl
**
******************************************************/
wxTextCtrl *FontPanel::commandId2TextCtrl( const int &id )
{
	return fontId2TextCtrl( id - CD_FIRST_FONT );
}

/*****************************************************
**
**   FontPanel   ---   fontId2TextCtrl
**
******************************************************/
wxTextCtrl *FontPanel::fontId2TextCtrl( const int &id )
{
	ASSERT_VALID_FONT_ID( id );
	switch( id )
	{
		case FONT_GRAPHIC_DEFAULT:     return t_graphic_default; break;
		case FONT_GRAPHIC_SYMBOLS:     return t_graphic_symbols; break;
		case FONT_TEXT_DEFAULT:        return t_text_default; break;
		case FONT_TEXT_HEADER:         return t_text_header; break;
		case FONT_TEXT_TABLE_CONTENTS: return t_text_table_contents; break;
		case FONT_TEXT_TABLE_HEADER:   return t_text_table_header; break;
		case FONT_PDF_DEFAULT:         return t_pdf_default; break;
		case FONT_PDF_HEADER:          return t_pdf_header; break;
		case FONT_PDF_TABLE_CONTENTS:  return t_pdf_table_contents; break;
		case FONT_PDF_TABLE_HEADER:    return t_pdf_table_header; break;
		case FONT_PDF_TINY:            return t_pdf_tiny; break;
		case FONT_PLAIN_DEFAULT:       return t_plain_default; break;

		default:
			printf( "FontPanel::fontId2TextCtrl: invalid id %d, exiting\n", id );
			assert( false );
		break;
	}
}

/*****************************************************
**
**   FontPanel   ---   OnChooseFont
**
******************************************************/
void FontPanel::OnChooseFont( wxCommandEvent &event )
{
	wxTextCtrl *t = commandId2TextCtrl( event.GetId());
	assert( t );

	wxFontData data;
	data.SetInitialFont( t->GetFont());

	wxFontDialog dialog( this, data );
	if ( dialog.ShowModal() == wxID_OK )
	{
		wxFont font = dialog.GetFontData().GetChosenFont();
		setTextControlItem( (FONT_ID)(event.GetId() - CD_FIRST_FONT), &font );
	}
	setDirty();
}

/*****************************************************
**
**   FontPanel   ---   restoreDefaults
**
******************************************************/
void FontPanel::restoreDefaults()
{
	FontProvider *fc = FontProvider::get();
	for( int id = 0; id < MAX_FONTS; id++ )
	{
		setTextControlItem( (FONT_ID)id, fc->getFallbackFont( (FONT_ID)id ));
	}
}

/*****************************************************
**
**   FontPanel   ---   set_properties
**
******************************************************/
void FontPanel::set_properties()
{
    // begin wxGlade: FontPanel::set_properties
    // end wxGlade
}

/*****************************************************
**
**   FontPanel   ---   do_layout
**
******************************************************/
void FontPanel::do_layout()
{
    // begin wxGlade: FontPanel::do_layout
    wxFlexGridSizer* sizer_lr = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_right = new wxFlexGridSizer(2, 1, 0, 0);
    sizer_pdfviews_staticbox->Lower();
    wxStaticBoxSizer* sizer_pdfviews = new wxStaticBoxSizer(sizer_pdfviews_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_pdfviews = new wxFlexGridSizer(5, 3, 3, 3);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_plaintext_staticbox->Lower();
    wxStaticBoxSizer* sizer_plaintext = new wxStaticBoxSizer(sizer_plaintext_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_plaintext = new wxFlexGridSizer(1, 3, 3, 3);
    sizer_graphic_views_staticbox->Lower();
    wxStaticBoxSizer* sizer_graphic_views = new wxStaticBoxSizer(sizer_graphic_views_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_graphic_views = new wxFlexGridSizer(2, 3, 3, 3);
    sizer_textviews_staticbox->Lower();
    wxStaticBoxSizer* sizer_textviews = new wxStaticBoxSizer(sizer_textviews_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_textviews = new wxFlexGridSizer(5, 3, 3, 3);
    grid_textviews->Add(l_text_default, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_textviews->Add(t_text_default, 0, wxALL|wxEXPAND, 3);
    grid_textviews->Add(b_text_default, 0, wxALL, 3);
    grid_textviews->Add(l_text_header, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_textviews->Add(t_text_header, 0, wxALL|wxEXPAND, 3);
    grid_textviews->Add(b_text_header, 0, wxALL, 3);
    grid_textviews->Add(l_text_table_contents, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_textviews->Add(t_text_table_contents, 0, wxALL|wxEXPAND, 3);
    grid_textviews->Add(b_text_table_contents, 0, wxALL, 3);
    grid_textviews->Add(l_text_table_header, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_textviews->Add(t_text_table_header, 0, wxALL|wxEXPAND, 3);
    grid_textviews->Add(b_text_table_header, 0, wxALL, 3);
    grid_textviews->AddGrowableCol(1);
    sizer_textviews->Add(grid_textviews, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_textviews, 1, wxALL|wxEXPAND, 3);
    grid_graphic_views->Add(l_graphic_default, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_graphic_views->Add(t_graphic_default, 0, wxALL|wxEXPAND, 3);
    grid_graphic_views->Add(b_graphic_default, 0, wxALL, 3);
    grid_graphic_views->Add(l_graphic_symbols, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_graphic_views->Add(t_graphic_symbols, 0, wxALL|wxEXPAND, 3);
    grid_graphic_views->Add(b_graphic_symbols, 0, wxALL, 3);
    grid_graphic_views->AddGrowableCol(1);
    sizer_graphic_views->Add(grid_graphic_views, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_graphic_views, 1, wxALL|wxEXPAND, 3);
    grid_plaintext->Add(l_plain_default, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_plaintext->Add(t_plain_default, 0, wxALL|wxEXPAND, 3);
    grid_plaintext->Add(b_plain_default, 0, wxALL, 3);
    grid_plaintext->AddGrowableCol(1);
    sizer_plaintext->Add(grid_plaintext, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_plaintext, 1, wxALL|wxEXPAND, 3);
    sizer_left->AddGrowableCol(0);
    sizer_lr->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    grid_pdfviews->Add(l_pdf_default, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdfviews->Add(t_pdf_default, 0, wxALL|wxEXPAND, 3);
    grid_pdfviews->Add(b_pdf_default, 0, wxALL, 3);
    grid_pdfviews->Add(l_pdf_header, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdfviews->Add(t_pdf_header, 0, wxALL|wxEXPAND, 3);
    grid_pdfviews->Add(b_pdf_header, 0, wxALL, 3);
    grid_pdfviews->Add(l_pdf_table_contents, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdfviews->Add(t_pdf_table_contents, 0, wxALL|wxEXPAND, 3);
    grid_pdfviews->Add(b_pdf_table_contents, 0, wxALL, 3);
    grid_pdfviews->Add(l_pdf_table_header, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdfviews->Add(t_pdf_table_header, 0, wxALL|wxEXPAND, 3);
    grid_pdfviews->Add(b_pdf_table_header, 0, wxALL, 3);
    grid_pdfviews->Add(l_pdf_tiny, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_pdfviews->Add(t_pdf_tiny, 0, wxALL|wxEXPAND, 3);
    grid_pdfviews->Add(b_pdf_tiny, 0, wxALL, 3);
    grid_pdfviews->AddGrowableCol(1);
    sizer_pdfviews->Add(grid_pdfviews, 1, wxEXPAND, 0);
    sizer_right->Add(sizer_pdfviews, 1, wxALL|wxEXPAND, 3);
    sizer_right->AddGrowableCol(0);
    sizer_lr->Add(sizer_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_lr);
    sizer_lr->Fit(this);
    sizer_lr->AddGrowableRow(0);
    sizer_lr->AddGrowableCol(0);
    sizer_lr->AddGrowableCol(1);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createFontPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createFontPanel( wxWindow *parent )
{
	return new FontPanel( parent );
}


