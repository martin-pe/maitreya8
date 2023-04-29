/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/MultipleViewConfig.h
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
#ifndef _MULTIPLEVIEWCONFIG_H_
#define _MULTIPLEVIEWCONFIG_H_

#include "ConfigListLoader.h"
#include "maitreya.h"
#include "ViewNode.h"

#include <wx/arrstr.h>
#include <map>

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

	std::map<wxString, VIEW_ID> viewtypes;

};

#endif


