/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/YogaConfig.h
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

#ifndef _YOGACONFIG_H_
#define _YOGACONFIG_H_

#include <wx/string.h>

#include "maitreya.h"
#include "ConfigListLoader.h"

// for legacy xml support
class wxXmlNode;
class Rule;

using namespace std;

/*************************************************//**
*
* \brief config and descrition for a single Yoga configuration
*
******************************************************/
class YogaConfig
{
public:

	YogaConfig( wxString name = wxEmptyString );
	~YogaConfig();

	wxString description, group, effect, source, rulestr, sourcefile;
	bool allowHigherVargas;
	Rule *rule;
};

/*************************************************//**
*
* \brief reads and contains the system wide Yoga configs
*
******************************************************/
class YogaConfigLoader : public ConfigListLoader<YogaConfig>
{
  DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( YogaConfigLoader )

public:

	virtual void loadSingleConfig( wxJSONValue& );

	void saveConfigs( wxString filename, const vector<YogaConfig*> );
	bool loadConfigXml( wxString filename, vector<YogaConfig*>& );
	bool loadConfig( wxString filename, vector<YogaConfig*>& );

private:
	YogaConfigLoader();

	YogaConfig *loadSingleConfigInternal( wxJSONValue& );

	void reportWrongRootNode( wxXmlNode* );
	void reportWrongNodeName( wxXmlNode* );
};

#endif

