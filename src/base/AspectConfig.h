/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AspectConfig.h
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

