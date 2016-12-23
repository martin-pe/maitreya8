/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/BrushDialog.h
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

#ifndef BRUSHDIALOG_H
#define BRUSHDIALOG_H

#include <wx/dialog.h>
#include "maitreya.h"


// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

class BrushDialogPreviewWidget;
class wxButton;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxPanel;
class wxSlider;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

/*************************************************//**
*
* 
*
******************************************************/
class BrushDialog: public wxDialog
{
	DECLARE_CLASS( BrushDialog )
public:
    // begin wxGlade: BrushDialog::ids
    // end wxGlade

	BrushDialog( wxWindow* parent, const MBrush& );

	MBrush getBrush() const { return brush; }

private:
    // begin wxGlade: BrushDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	MBrush brush;

	void updateUi();

	void OnColorPicker( wxColourPickerEvent& );
	void OnStyleChoice( wxCommandEvent& );
	void OnCommand( wxCommandEvent& );
	void OnOK( wxCommandEvent& );

protected:
    // begin wxGlade: BrushDialog::attributes
    wxStaticBox* sizer_bg_staticbox;
    wxStaticBox* sizer_preview_staticbox;
    wxStaticText* label_bgcolor;
    wxColourPickerCtrl* button_bgcolor;
    wxStaticText* label_bgstyle;
    wxChoice* choice_bgstyle;
    wxStaticText* label_bgimage;
    wxChoice* choice_bgimage;
    wxStaticText* label_rotatehue;
    wxSlider* slider_rotatehue;
    BrushDialogPreviewWidget* preview;
    wxPanel* panel_preview;
    wxStaticLine* static_line;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
}; // wxGlade: end class


#endif // BRUSHDIALOG_H
