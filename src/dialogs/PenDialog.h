/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PenDialog.h
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

#ifndef PENDIALOG_H
#define PENDIALOG_H

#include <wx/dialog.h>

// begin wxGlade: ::dependencies
#include <wx/spinctrl.h>
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

class MBrush;
class PenDialogPreviewWidget;
class wxButton;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxCommandEvent;
class wxPen;
class wxPanel;
class wxSpinCtrl;
class wxSpinEvent;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
* 
*
******************************************************/
class PenDialog: public wxDialog
{
	DECLARE_CLASS( PenDialog )

public:
    // begin wxGlade: PenDialog::ids
    // end wxGlade

	PenDialog( wxWindow* parent, const wxPen&, MBrush* = (MBrush*)NULL );
	~PenDialog();

	wxPen *getPen() const { return pen; }

private:
    // begin wxGlade: PenDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	wxPen *pen;
	MBrush *bgbrush;

	void updateAttributes();
	void OnColorPicker( wxColourPickerEvent& ) { updateAttributes(); }
	void OnSpin( wxSpinEvent& ) { updateAttributes(); }
	void OnChoice( wxCommandEvent& ) { updateAttributes(); }

protected:
    // begin wxGlade: PenDialog::attributes
    wxStaticBox* sizer_fg_staticbox;
    wxStaticBox* sizer_preview_staticbox;
    wxStaticText* label_fgcolor;
    wxColourPickerCtrl* button_fgcolor;
    wxStaticText* label_fgwidth;
    wxSpinCtrl* spin_fgwidth;
    wxStaticText* label_fgstyle;
    wxChoice* choice_fgstyle;
    PenDialogPreviewWidget* preview;
    wxPanel* panel_preview;
    wxStaticLine* static_line;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
}; // wxGlade: end class


#endif // PENDIALOG_H
