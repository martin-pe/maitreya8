/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ToolbarPanel.h
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
    wxStaticText* label_iconsize;
    wxChoice* choice_iconsize;
    // end wxGlade
}; // wxGlade: end class


#endif // TOOLBARPANEL_H
