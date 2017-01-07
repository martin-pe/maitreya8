/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/SheetConfig.h
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
	wxColour headerBgColor;

	bool useCellColors;
	wxColour cellTextColor;

	int cellBgMode;
	wxColour allCellBgColor;
	wxColour oddCellBgColor;
	wxColour evenCellBgColor;

	bool useGrid;
};

/*************************************************//**
*
* 
*
******************************************************/
class SheetConfig : public ConfigBase
{
public:

	SheetConfig( const SheetConfig& );
	SheetConfig( wxString name = wxEmptyString );

	virtual ~SheetConfig();

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxString name;
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


