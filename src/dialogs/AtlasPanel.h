/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasPanel.h
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

#ifndef ATLASPANEL_H
#define ATLASPANEL_H

#include "ConfigPanel.h"


// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

class wxButton;
class wxCheckBox;
class wxListBox;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class AtlasConfig;

/*************************************************//**
*
* \brief ConfigPanel for atlas configuration
*
******************************************************/
class AtlasPanel: public ConfigPanel
{
	DECLARE_CLASS( AtlasPanel )

    // begin wxGlade: AtlasPanel::ids
    // end wxGlade

	AtlasPanel( wxWindow* );
	~AtlasPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: AtlasPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnChooseDbFile( wxCommandEvent& );
	void OnTextChanged( wxCommandEvent& );
	virtual void updateUi();

	void OnChooseSqlFile( wxCommandEvent& );
	void updateSqlFileStatus();

	void OnStartImport( wxCommandEvent& );

	void OnCountries( wxCommandEvent& );
	void updateCountryList();

	bool getFileStatus( wxString filename, wxString &message );

	AtlasConfig *aconfig;

protected:
    // begin wxGlade: AtlasPanel::attributes
    wxStaticBox* sizer_favcountries_staticbox;
    wxStaticBox* sizer_import_staticbox;
    wxStaticBox* sizer_db_staticbox;
    wxButton* button_dbfile;
    wxTextCtrl* text_dbfile;
    wxStaticText* text_dbstatus;
    wxPanel* panel_status;
    wxButton* button_sqlfile;
    wxTextCtrl* text_sqlfile;
    wxStaticText* text_sqlfilestatus;
    wxPanel* panel_sqlfilestatus;
    wxButton* button_startimport;
    wxListBox* list_countries;
    wxButton* button_countries;
    // end wxGlade
}; // wxGlade: end class


#endif // ATLASPANEL_H
