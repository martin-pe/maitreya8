/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/BrushDialog.h
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
