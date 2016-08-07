/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ColorConfig.h
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

#ifndef _COLORCONFIG_H_
#define _COLORCONFIG_H_

#include "ConfigBase.h"
#include "maitreya.h"

#include <map>
#include <vector>
#include <wx/brush.h>
#include <wx/colour.h>
#include <wx/pen.h>
#include <wx/string.h>

using namespace std;

/*************************************************//**
*
* \brief configuration for bar diagram styles, i.e. graphical Dasa, varga etc diagrams
*
******************************************************/
class BarDiagramStyle : public ConfigBase
{
public:
	BarDiagramStyle();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxString name;
	wxPen pen;
	MBrush brush;
};

/*************************************************//**
*
* 
*
******************************************************/
class BarDiagramConfig : public ConfigBase
{
public:
	BarDiagramConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	static int getConfigId( const int &i, const bool &rasitype );

	BarDiagramStyle style[MAX_BARDIAGRAM_COLORS];

	int cornerRadius;
	bool doItemZoom;
	int itemZoom;
	bool showToolbarMessages;
};

/*************************************************//**
*
* 
*
******************************************************/
class BgPalette : public ConfigBase
{
public:
	bool isValid() const { return brushes.size() > 0; }

	vector<MBrush> brushes;

	MBrush getBrush( const uint &i );

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );
};

/*************************************************//**
*
* \brief configuration for chart colors
*
******************************************************/
class ObjectColorConfig
{
public:
	ObjectColorConfig();
	wxColour signFgColor[4];
	wxColour houseFgColor[4];
	wxColour planetFgColor[MAX_EPHEM_OBJECTS];

	BgPalette signPalette;
	BgPalette housePalette;

	wxColour &getSignFgColor( const int &i );
	wxColour &getHouseFgColor( const int &i );
	wxColour &getPlanetColor( const int &i );

};

/*************************************************//**
*
* 
*
******************************************************/
class PaletteManager
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( PaletteManager );

public:

	bool hasPalette( wxString name );
	BgPalette *getPalette( wxString name );

	vector<wxString> getAllKeys();
	void setPalette( const wxString name, const BgPalette& );
	void loadFallbackConfig();

private:
	PaletteManager();
	void init();

	map<wxString, BgPalette*> m;
};

/*************************************************//**
*
* \brief general configuration for colors
*
******************************************************/
class ColorConfig : public ConfigBase
{
public:
	ColorConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxColour fgColor;
	wxColour bgColor;
	wxColour sbcMarkedFieldsBgColor;
	wxColour beneficFgColor;
	wxColour maleficFgColor;
	wxColour transitFgColor;
	wxColour errorBgColor;
	wxColour chartMarkupBgColor;
};


#endif

