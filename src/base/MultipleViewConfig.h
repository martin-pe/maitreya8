/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/MultipleViewConfig.h
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
#ifndef _MULTIPLEVIEWCONFIG_H_
#define _MULTIPLEVIEWCONFIG_H_

#include "ConfigListLoader.h"
#include "maitreya.h"
#include "ViewNode.h"

#include <wx/arrstr.h>
#include <map>

using namespace std;

class wxJSONValue;

/*************************************************//**
*
* 
*
******************************************************/
class MultipleViewConfig : public ViewNode
{
public:

	MultipleViewConfig( wxString = wxEmptyString );
	virtual ~MultipleViewConfig();

	virtual MultipleViewConfig *clone();
	virtual void dump( const int ) {}

	wxString name, description;
	ViewNode *viewnode;
	int sizex, sizey;
	bool vedic;

};

/*************************************************//**
*
* \brief contains all multiple view definitions
*
******************************************************/
class MultipleViewConfigLoader : public ConfigListLoader<MultipleViewConfig>
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( MultipleViewConfigLoader )

public:

	wxArrayString getNamesAsArray();

private:

	MultipleViewConfigLoader();

	virtual void loadSingleConfig( wxJSONValue& );

	VIEW_ID getViewId( wxString );

	ViewNode *parseWidgetNode( ViewNode*, wxJSONValue& );
	ViewNode *parseSplitterNode( ViewNode*, wxJSONValue& );
	ViewNode *parseWindowNode( ViewNode*, wxJSONValue& );
	ViewNode *parseNotebookNode( ViewNode*, wxJSONValue& );
	PageNode *parsePageNode( ViewNode*, wxJSONValue& );

	map<wxString, VIEW_ID> viewtypes;

};

#endif


