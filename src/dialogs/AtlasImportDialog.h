/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AtlasImportDialog.h
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

