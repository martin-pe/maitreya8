/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ColorConfig.h
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
class BgPalette // : public ConfigBase
{
public:
	bool isValid() const { return brushes.size() > 0; }

	std::vector<MBrush> brushes;

	MBrush getBrush( const uint &i );

	//virtual void load( wxJSONValue&, wxString = wxEmptyString );
	//virtual void save( wxJSONValue&, wxString = wxEmptyString );
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

	std::vector<wxString> getAllKeys();
	void setPalette( const wxString name, const BgPalette& );
	void loadFallbackConfig();

private:
	PaletteManager();
	void init();

	std::map<wxString, BgPalette*> m;
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

