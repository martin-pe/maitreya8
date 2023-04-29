/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/YogaConfig.h
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

#ifndef _YOGACONFIG_H_
#define _YOGACONFIG_H_

#include <wx/string.h>
#include <vector>

#include "maitreya.h"
#include "ConfigListLoader.h"

// for legacy xml support
class wxXmlNode;
class Rule;

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

	void saveConfigs( wxString filename, const std::vector<YogaConfig*> );
	bool loadConfigXml( wxString filename, std::vector<YogaConfig*>& );
	bool loadConfig( wxString filename, std::vector<YogaConfig*>& );

private:
	YogaConfigLoader();

	YogaConfig *loadSingleConfigInternal( wxJSONValue& );

	void reportWrongRootNode( wxXmlNode* );
	void reportWrongNodeName( wxXmlNode* );
};

#endif

