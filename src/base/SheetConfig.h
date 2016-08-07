/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/SheetConfig.h
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

#ifndef _SHEETCONFIG_H_
#define _SHEETCONFIG_H_

#include "ConfigBase.h"
#include "ConfigListLoader.h"
#include "maitreya.h"

#include <wx/colour.h>
#include <wx/pen.h>

/*************************************************//**
*
* 
*
******************************************************/
class TableStyle : public ConfigBase
{
public:

	TableStyle();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	bool useHeaderColors;
	wxColour headerTextColor;
	MBrush headerBrush;

	bool useCellColors;
	wxColour cellTextColor;

	int cellBgMode;
	MBrush cellBrush;
	MBrush oddCellBrush;
	MBrush evenCellBrush;

	int outerBorderStyle;
	wxPen outerBorderPen;

	int headerBorderStyle;
	wxPen headerBorderPen;

	int cellBorderStyle;
	wxPen cellBorderPen;
	int gridStyle;

	int shadowStyle;
	int shadowWidth;
	MBrush shadowBrush;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetConfig : public ConfigBase
{
public:

	SheetConfig();
	SheetConfig( wxString name );
	//SheetConfig( const SheetConfig& );
	//void operator=( const SheetConfig& );

	virtual ~SheetConfig();

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxString name;
	wxColour textColor;
	MBrush brush;

	int selectionMode;
	MBrush selectedItemBrush;

	TableStyle tablestyle;

};

/*************************************************//**
*
* 
*
******************************************************/
class SheetConfigLoader : public ConfigListLoader<SheetConfig>
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( SheetConfigLoader )

public:

	virtual void loadSingleConfig( wxJSONValue& );
	void saveConfigs();

private:
	SheetConfigLoader();
};

#endif


