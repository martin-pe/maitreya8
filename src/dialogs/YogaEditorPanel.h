/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/YogaEditorPanel.h
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

#ifndef YOGAEDITORPANEL_H
#define YOGAEDITORPANEL_H

#include <wx/panel.h>

#include "YogaConfig.h"
//#include "BasicWidget.h"

#include <vector>

using namespace std;

// begin wxGlade: ::dependencies
#include <wx/listctrl.h>
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade

class wxBitmapButton;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxListCtrl;
class wxListEvent;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

class DragChart;
class MinimalVargaHoroscope;
class YogaConfig;

enum { YE_YOGA_PANEL = wxID_HIGHEST + 100, YE_YOGA_LIST, YE_OPENFILE,
	YE_PARSE_RULE, YE_FORMAT_RULE, YE_FORMAT_ALL_RULES, YE_NEW_SOURCE,
	YE_NEW_GROUP, YE_DESCRIPTION, YE_EVALUATE, YE_UP, YE_DOWN, YE_DELETE_YOGA,
	YE_NEW_YOGA, YE_COPY_YOGA, YE_CLEAR_CHART, YE_OBJECTS,
	YE_TEXT_EFFECT, YE_TEXT_RULE, YE_TEXT_DESCRIPTION,
	YE_CHOICE_GROUP, YE_CHOICE_SOURCE, YE_CHECK_VargaS
};

/*************************************************//**
*
* \brief Main panel for YogaEditor
*
******************************************************/
//class YogaEditorPanel: public BasicWidget
class YogaEditorPanel : public wxPanel
{
	DECLARE_CLASS( YogaEditorPanel )

public:
	// begin wxGlade: YogaEditorPanel::ids
	// end wxGlade

	YogaEditorPanel( wxWindow*, wxWindowID id, MinimalVargaHoroscope* );

	void addSource( wxString );

	void addGroup( wxString );

	void loadYogas( const vector<YogaConfig*> );

	void saveYogas( vector<YogaConfig*>& );

	void clearGui();

	void clearChart();

	wxString getRuleText() const;

	bool isDirty() const { return dirty; }
	void setDirty( const bool b = true ) { dirty = b; }

	void echoOnDragChart( wxString );

	void newItem();

private:
	// begin wxGlade: YogaEditorPanel::methods
	void set_properties();
	void do_layout();
	// end wxGlade

	void OnFieldChanged( wxCommandEvent& );
	void updateFieldStatus();

	void OnYogaListLeftSelected( wxListEvent& );

	void OnYogaListItemUp( wxCommandEvent& );
	void OnYogaListItemDown( wxCommandEvent& );
	void OnYogaListNewItem( wxCommandEvent& ) {
		newItem();
	}
	void OnYogaListDeleteItem( wxCommandEvent& );
	void OnYogaListCopyItem( wxCommandEvent& );

	void propagateDirtyEvent();

	MinimalVargaHoroscope *evaluationChart;

	bool propagateUserInput, dirty;

protected:

	virtual void doPaint( bool = true, const wxRect* = 0 ) {}

	// begin wxGlade: YogaEditorPanel::attributes
	wxStaticBox* sizer_right_staticbox;
	wxStaticBox* sizer_dragchart_staticbox;
	wxStaticBox* sizer_rule_staticbox;
	wxStaticBox* sizer_effect_staticbox;
	wxStaticBox* sizer_yogalist_staticbox;
	wxListCtrl* list_yogas;
	wxBitmapButton* button_up;
	wxBitmapButton* button_down;
	wxBitmapButton* button_new_yoga;
	wxBitmapButton* button_delete_yoga;
	wxBitmapButton* button_copy_yoga;
	wxStaticText* label_description;
	wxTextCtrl* text_description;
	wxStaticText* label_group;
	wxChoice* choice_group;
	wxBitmapButton* button_new_group;
	wxStaticText* label_source;
	wxChoice* choice_source;
	wxBitmapButton* button_new_source;
	wxCheckBox* check_higher_vargas;
	wxTextCtrl* text_effect;
	wxTextCtrl* text_rule;
	DragChart* drag_chart;
	wxButton* button_parse_rule;
	wxButton* button_evaluate;
	wxButton* button_clear_chart;
	// end wxGlade
}; // wxGlade: end class


#endif // YOGAEDITORPANEL_H
