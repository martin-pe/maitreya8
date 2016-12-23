/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/FontPanel.h
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

#ifndef FONTPANEL_H
#define FONTPANEL_H

#include "ConfigPanel.h"
#include "FontProvider.h"

// begin wxGlade: dependencies
// end wxGlade

class wxButton;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for font parameters
*
******************************************************/
class FontPanel: public ConfigPanel
{
	DECLARE_CLASS( FontPanel )

public:
    // begin wxGlade: FontPanel::ids
    // end wxGlade

	FontPanel( wxWindow* );

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: FontPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChooseFont( wxCommandEvent& );

	void setTextControlItems();
	void setTextControlItem( const FONT_ID&, wxFont* );

	wxTextCtrl *commandId2TextCtrl( const int &id );
	wxTextCtrl *fontId2TextCtrl( const int &id );

protected:
    // begin wxGlade: FontPanel::attributes
    wxStaticBox* sizer_pdfviews_staticbox;
    wxStaticBox* sizer_plaintext_staticbox;
    wxStaticBox* sizer_graphic_views_staticbox;
    wxStaticBox* sizer_textviews_staticbox;
    wxStaticText* l_text_default;
    wxTextCtrl* t_text_default;
    wxButton* b_text_default;
    wxStaticText* l_text_header;
    wxTextCtrl* t_text_header;
    wxButton* b_text_header;
    wxStaticText* l_text_table_contents;
    wxTextCtrl* t_text_table_contents;
    wxButton* b_text_table_contents;
    wxStaticText* l_text_table_header;
    wxTextCtrl* t_text_table_header;
    wxButton* b_text_table_header;
    wxStaticText* l_graphic_default;
    wxTextCtrl* t_graphic_default;
    wxButton* b_graphic_default;
    wxStaticText* l_graphic_symbols;
    wxTextCtrl* t_graphic_symbols;
    wxButton* b_graphic_symbols;
    wxStaticText* l_plain_default;
    wxTextCtrl* t_plain_default;
    wxButton* b_plain_default;
    wxStaticText* l_pdf_default;
    wxTextCtrl* t_pdf_default;
    wxButton* b_pdf_default;
    wxStaticText* l_pdf_header;
    wxTextCtrl* t_pdf_header;
    wxButton* b_pdf_header;
    wxStaticText* l_pdf_table_contents;
    wxTextCtrl* t_pdf_table_contents;
    wxButton* b_pdf_table_contents;
    wxStaticText* l_pdf_table_header;
    wxTextCtrl* t_pdf_table_header;
    wxButton* b_pdf_table_header;
    wxStaticText* l_pdf_tiny;
    wxTextCtrl* t_pdf_tiny;
    wxButton* b_pdf_tiny;
    // end wxGlade
}; // wxGlade: end class


#endif // COLORFONTPANEL_H
