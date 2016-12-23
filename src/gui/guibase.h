/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/guibase.h
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
DECLARE_EVENT_TYPE( CREATE_ENTRY_CHART, wxID_HIGHEST + 1004 )
END_DECLARE_EVENT_TYPES()

wxString createImageHash( wxImage* );

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

