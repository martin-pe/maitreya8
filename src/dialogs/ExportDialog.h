/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ExportDialog.h
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

#ifndef EXPORTDIALOG_H
#define EXPORTDIALOG_H

// begin wxGlade: dependencies
// end wxGlade

#include <wx/dialog.h>

class wxButton;
class wxCheckBox;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
*   \brief panel for graphics export (size configuration)
*
******************************************************/
class ExportDialog: public wxDialog
{
	DECLARE_CLASS( ExportDialog )
public:
    // begin wxGlade: ExportDialog::ids
    // end wxGlade

	ExportDialog(wxWindow* parent, const int &sizex, const int &sizey );
	void getSizes( int &x, int &y );
	bool getCheckShowDialog();

private:
    // begin wxGlade: ExportDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	int sizex, sizey;

protected:
    // begin wxGlade: ExportDialog::attributes
    wxStaticBox* sizer_size_staticbox;
    wxStaticText* label_x;
    wxSpinCtrl* spin_x;
    wxStaticText* label_y;
    wxSpinCtrl* spin_y;
    wxCheckBox* check_show_dialog;
    wxStaticLine* static_line_1;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
};


#endif // EXPORTDIALOG_H
