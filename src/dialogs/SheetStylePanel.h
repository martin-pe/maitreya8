/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/SheetStylePanel.h
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

#ifndef SHEETSTYLEPANEL_H_
#define SHEETSTYLEPANEL_H_

#include "ConfigPanel.h"
#include<vector>

using namespace std;

// begin wxGlade: dependencies
// end wxGlade

class wxCheckBox;
class wxChoice;
class wxColourPickerCtrl;
class wxColourPickerEvent;
class wxButton;
class wxFlexGridSizer;
class wxSpinCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;

class ChartProperties;
class TextWidget;
class Horoscope;
class SheetConfig;

/*************************************************//**
*
* \brief ConfigPanel for color parameters
*
******************************************************/
class SheetStylePanel: public ConfigPanel
{
	DECLARE_CLASS( SheetStylePanel )

public:
    // begin wxGlade: SheetStylePanel::ids
    // end wxGlade

	SheetStylePanel( wxWindow* );
	~SheetStylePanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: SheetStylePanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnSelectStyle ( wxCommandEvent& );
	void model2gui( const uint &sel );
	void OnManageStyles( wxCommandEvent& );
	void populateStyleChoice();
	SheetConfig *getSheetConfig( const int& );

	void OnCommand( wxCommandEvent& );
	void OnTableColorPickerEvent( wxColourPickerEvent& );

	virtual void updateUi();

	ChartProperties *props;
	Horoscope *horoscope;
	int selected;
	vector<SheetConfig*> v;

protected:
    // begin wxGlade: SheetStylePanel::attributes
    wxStaticBox* sizer_preview_staticbox;
    wxStaticBox* sizer_table_border_staticbox;
    wxStaticBox* sizer_table_cells_staticbox;
    wxStaticBox* sizer_table_headers_staticbox;
    wxStaticText* label_style;
    wxChoice* choice_styles;
    wxButton* button_manage_styles;
    wxCheckBox* check_use_header_colors;
    wxStaticText* label_header_textcolor;
    wxColourPickerCtrl* button_header_textcolor;
    wxStaticText* label_header_bgcolor;
    wxColourPickerCtrl* button_header_bgcolor;
    wxCheckBox* check_use_cell_colors;
    wxStaticText* label_cell_textcolor;
    wxColourPickerCtrl* button_cell_textcolor;
    wxStaticText* label_cell_bgmode;
    wxChoice* choice_cell_bgmode;
    wxStaticText* label_all_row_bgcolor;
    wxColourPickerCtrl* button_all_cell_bgcolor;
    wxStaticText* label_even_row_bgcolor;
    wxColourPickerCtrl* button_even_cell_bgcolor;
    wxStaticText* label_odd_row_bgcolor;
    wxColourPickerCtrl* button_odd_cell_bgcolor;
    wxCheckBox* check_draw_border;
    TextWidget* preview;
    wxPanel* panel_preview;
    // end wxGlade
}; // wxGlade: end class


#endif // SHEETSTYLEPANEL_H_
