/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/guibase.h
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

#ifndef _GUIBASE_H_
#define _GUIBASE_H_

#include <wx/dialog.h>
#include <wx/event.h>
#include <wx/string.h>

#include "maitreya.h"

class wxButton;
class wxCheckBox;
class wxWindow;

#define DEFAULT_DIALOG_STYLE wxRESIZE_BORDER|wxSYSTEM_MENU|wxMAXIMIZE_BOX|wxCLOSE_BOX|wxCAPTION

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( TOOLPANEL_CHANGED, wxID_HIGHEST + 1000 )
DECLARE_EVENT_TYPE( TOOLPANEL_CREATEDOC, wxID_HIGHEST + 1001 )
DECLARE_EVENT_TYPE( SHOW_EPHEM_FILE_WARNING, wxID_HIGHEST + 1003 )
END_DECLARE_EVENT_TYPES()

wxString createImageHash( wxImage* );

bool setupObjectFilter( wxWindow *parent, const ObjectArray &planets, ObjectFilter& );
bool configureObjectFilter( wxWindow *parent, const ObjectArray &planets, ObjectArray &filter );

int doMessageBox( wxWindow *parent, const wxString message, const long style = wxOK | wxICON_INFORMATION );

void showHelpPopup( wxWindow *parent, wxString header, wxString contents );

/*************************************************//**
*
* 
*
******************************************************/
class DoNotShowAgainDialog : public wxDialog
{
public:
	DoNotShowAgainDialog( wxWindow *parent, wxString message, bool *item, const int style = wxOK );

private:

	void OnButton( wxCommandEvent& );

	wxCheckBox *check_not_ask_again;
	bool *cfgitem;
};


#endif

