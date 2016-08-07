/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/YogaEditor.h
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

#ifndef YOGAEDITOR_H
#define YOGAEDITOR_H

#include "BasicView.h"

#include "YogaConfig.h"
#include "YogaEditorPanel.h"

class wxBitmapButton;
class wxButton;
class wxCheckBox;
class wxChoice;
class wxListCtrl;
class wxListEvent;
class wxPanel;
class wxStaticBox;
class wxStaticText;
class wxTextCtrl;

class DragChart;
class	MinimalVargaHoroscope;
class Rule;
class YogaConfigLoader;
class YogaExpert;

/*************************************************//**
*
* \brief Editor tool for Yoga configurations
*
******************************************************/
class YogaEditor: public BasicView
{
	DECLARE_CLASS( YogaEditor )
public:

	YogaEditor( wxWindow*, ChildWindow* );
	~YogaEditor();

	virtual wxString getWindowLabel( const bool shortname = false );

	virtual bool queryClose();

private:

	// File menu and toolbar
	void OnNewFile( wxCommandEvent& );
	void OnOpenFile( wxCommandEvent& );
	void OnSaveFile( wxCommandEvent& ) { saveFile(); }
	void OnSaveFileAs( wxCommandEvent& ) { saveFileAs(); }
	void OnCloseFile( wxCommandEvent& ) { closeFile(); }
	void OnClose( wxCommandEvent& );

	// command buttons in dialog
	void OnNewSource( wxCommandEvent& );
	void OnNewGroup( wxCommandEvent& );
	void OnParseRule( wxCommandEvent& ) { parseRule(); }
	void OnEvaluateRule( wxCommandEvent& );
	void OnSelectObjects( wxCommandEvent& );
	void OnClearChart( wxCommandEvent& );

	void loadFile( wxString filename );
	bool closeFile();
	bool saveFile();
	bool saveFileAs();
	Rule *parseRule();
	void OnIdle( wxIdleEvent& );

	void updateUiItems();

	wxString filename, openPath, savePath;
	YogaEditorPanel *panel;
	YogaConfigLoader *loader;
	YogaExpert *yogaexpert;
	MinimalVargaHoroscope *evaluationChart;

};

#endif

