/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ToolbarPanel.h
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

#ifndef TOOLBARPANEL_H
#define TOOLBARPANEL_H

#include "ConfigPanel.h"
#include "ImageProvider.h"
# include <map>

using namespace std;

// begin wxGlade: dependencies
// end wxGlade

class ToolBarCommand;
class ToolbarConfig;
class wxBitmapButton;
class wxButton;
class wxChoice;
class wxImageList;
class wxListEvent;
class wxListCtrl;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* \brief ConfigPanel for toolbar parameters
*
******************************************************/
class ToolbarPanel : public ConfigPanel
{
	DECLARE_CLASS( ToolbarPanel )

public:
    // begin wxGlade: ToolbarPanel::ids
    // end wxGlade

	ToolbarPanel( wxWindow* );
	~ToolbarPanel();

	virtual void config2model();
	virtual void model2config();
	virtual void restoreDefaults();

private:
    // begin wxGlade: ToolbarPanel::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	void OnLeft( wxCommandEvent& );
	void OnRight( wxCommandEvent& );
	void OnUp( wxCommandEvent& );
	void OnDown( wxCommandEvent& );

	void OnLeftSelected( wxListEvent& );
	void OnRightSelected( wxListEvent& );
	void OnChoice( wxCommandEvent& );

	virtual void updateUi();

	void updateItemConfig();
	void initToolItems();
	void initRightControl();
	void insertRightItemById( const int &commmandid, const int &pos, const bool doarrange = true );

	wxImageList *imageList;
	map< BitmapId, int > imageListIds;

	int left_sel, right_sel;
	ToolbarConfig *tconfig;

protected:
    // begin wxGlade: ToolbarPanel::attributes
    wxStaticBox* sizer_options_staticbox;
    wxStaticBox* sizer_selected_staticbox;
    wxStaticBox* sizer_available_staticbox;
    wxListCtrl* list1;
    wxBitmapButton* button_right;
    wxBitmapButton* button_left;
    wxBitmapButton* button_up;
    wxBitmapButton* button_down;
    wxListCtrl* list2;
    wxStaticText* label_toolstyle;
    wxChoice* choice_toolstyle;
    // end wxGlade
}; // wxGlade: end class


#endif // TOOLBARPANEL_H
