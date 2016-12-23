/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ExportDialog.h
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
