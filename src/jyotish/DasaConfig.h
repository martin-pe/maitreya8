/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/DasaConfig.h
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

#ifndef _DASACONFIG_H_
#define _DASACONFIG_H_

#include <wx/string.h>
#include <set>

#include "ConfigListLoader.h"
#include "Dasa.h"
#include "maitreya.h"

using namespace std;

/*************************************************//**
*
* \brief config and descrition for a single Dasa configuration
*
******************************************************/
class DasaConfig
{
public:

	DasaConfig( wxString = wxEmptyString );

	bool matches( Dasa* );

	wxString description, effect, source, sourcefile;
	int lord, sublord, subsublord;
	bool isvalid;
	DasaId dasaId;
};

/*************************************************//**
*
*
*
******************************************************/
class DasaConfigLoader : public ConfigListLoader<DasaConfig>
{
  DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( DasaConfigLoader )

public:

	virtual void loadSingleConfig( wxJSONValue& );
	//void saveConfigs( wxString filename, const vector<DasaConfig*> );

	set<wxString> &getSources() { return sources; }

private:

	DasaConfigLoader();
	set<wxString> sources;

};

#endif


