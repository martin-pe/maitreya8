/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasImportDialog.h
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

#ifndef ATLASIMPORTDIALOG_H
#define ATLASIMPORTDIALOG_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

// prevent compiler error based upon usage of wxGauge95
#if defined(__WXMSW__)
#include <wx/gauge.h>
#endif

#include <wx/dialog.h>

class wxButton;
class wxGauge;
class wxPanel;
class wxStaticText;
class wxTimer;
class wxTimerEvent;

class AtlasImporter;

/*************************************************//**
*
*
*
******************************************************/
class AtlasImportDialog : public wxDialog
{

public:
    // begin wxGlade: AtlasImportDialog::ids
    // end wxGlade

	AtlasImportDialog(wxWindow* parent, wxString title = wxEmptyString );
	~AtlasImportDialog();

	bool run();
	bool setData( wxString sqlfile, wxString dbfile );
	bool setDefaultFiles();
	wxString getErrorMessage();

private:
    // begin wxGlade: AtlasImportDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnTimer( wxTimerEvent& );
	void OnCancel( wxCommandEvent& );
	void OnRun( wxCommandEvent& );

	bool checkFiles();

	AtlasImporter *importer;
	wxTimer *timer;
	wxString sqlfile, dbfile;
	bool close_called;

protected:
    // begin wxGlade: AtlasImportDialog::attributes
    wxGauge* gauge;
    wxStaticText* text_status;
    wxPanel* panel_gauge;
    wxButton* button_cancel;
    // end wxGlade
}; // wxGlade: end class


#endif // ATLASIMPORTDIALOG_H

