/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AspectConfig.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/
#ifndef _ASPECTCONFIG_H_
#define _ASPECTCONFIG_H_

#include <vector>
#include <wx/colour.h>
#include <wx/pen.h>
#include <wx/string.h>

#include "ConfigBase.h"
#include "maitreya.h"

using namespace std;

/*************************************************//**
*
* \brief astrological and graphical configuration for a single aspect
*
******************************************************/
class AspectConfig : public ConfigBase
{
public:
	AspectConfig()
	{
		active = true;
		orbis = 1.0;
		pen = *wxBLACK_PEN;
	}
	AspectConfig( const wxString& );
	AspectConfig( const bool &active, const double &orbis, const wxPen &pen )
	{
		this->active = active;
		this->orbis = orbis;
		this->pen = pen;
	}

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );
	wxString toString();

	bool active;
	double orbis;
	wxPen pen;
};

/*************************************************//**
*
* \brief loads aspect configuration from config stores information about default config
*
******************************************************/
class AspectConfigLoader
{
	DECLARE_SINGLETON( AspectConfigLoader )

public:

	void initAspects();

	vector<AspectConfig> getDefaultAspectConfigList();

private:

};

#endif

