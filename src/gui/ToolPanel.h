/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ToolPanel.h
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

#ifndef _TOOLPANEL_H_
#define _TOOLPANEL_H_

#include <wx/toolbar.h>
#include <vector>

#include "ImageProvider.h"

using namespace std;

// Toolbar constants
enum TOOBLBAR_ITEM { TBS_TEXTMODE = wxID_HIGHEST + 10000, TBS_Varga, TBS_DASA, 
	TBS_PARTNER1 , TBS_PARTNER2, TBS_DATE, TBS_TIME, TBS_GRADKREIS, TBS_PRINTOUTTYPE,
	TBS_YEAR, TBS_NBYEARS, TBS_ECL_TYPE, TBS_TRANSITMODE, TBS_ORBIS, TBS_SORT,
	TBS_ANTARDASALEVEL, TBS_SBCMODE, TBS_SBCTEXT, TBS_MONTH,
	TBS_EPHEMMODE, TBS_TZ, TBS_EPHEMDEG, TBS_EPHEMPLANETS, TBS_YOGAGROUPFILTER,
	TBS_YOGAFIRSTHOUSE, TBS_YOGASOURCEFILTER, TBS_DATE_PROGRESS,
	TBS_WESTERNCHARTSTYLE, TBS_VEDICCHARTSTYLE, TBS_SHEETSTYLE, TBS_LAST
};
#define IS_TB_CHOICE_ITEM( i ) ( i >= TBS_TEXTMODE && i < TBS_LAST )

enum { TB_SEPARATOR = wxID_HIGHEST + 10999 };

enum { TB_TEXT = wxID_HIGHEST + 11200 };
#define IS_TB_TEXT_ITEM( i ) ( i == TB_TEXT )

enum { TBC_DASA_ZOOM = wxID_HIGHEST + 11300 };
#define IS_TB_COMBO_ITEM( i ) ( i == TBC_DASA_ZOOM )

/*************************************************//**
*
* \brief encapsulates creation of toolbar
*
******************************************************/
class ToolbarFactory
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( ToolbarFactory )
public:

	wxToolBar *createToolbar( wxWindow*, const int &type );

	void populateMainToolbar( wxToolBar* );

	int getToolbarStyle();

	vector<int> getFullCommandIdList();
	vector<int> getDefaultCommandIdList();

private:
	ToolbarFactory();
	void init();

	void create( wxToolBar*, const int* );
	void createByCommandId( wxToolBar*, const int& );
	void createChoiceItem( wxToolBar*, const int& );
	void createTextField( wxToolBar* );
	void createComboItem( wxToolBar*, const int &item );

	void addTool( wxToolBar*, const int&,  const wxItemKind = wxITEM_NORMAL );
	void addControl( wxToolBar*, wxControl*, const wxString &label );
};

#endif


