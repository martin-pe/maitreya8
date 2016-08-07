/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PrintoutConfig.h
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
#ifndef _PRINTOUTCONFIG_H_
#define _PRINTOUTCONFIG_H_

#include <vector>

#include "ConfigListLoader.h"
#include "PrintoutConfigBase.h"
#include "maitreya.h"

class wxJSONValue;

using namespace std;

enum PD_ITEM_TYPE { PD_HEADER = 0, PD_DASA_SUMMARY, PD_VEDIC_CHART_PAIR, PD_WESTERN_CHART,
	PD_SBC, PD_ASPECTARIUM, PD_ASHTAKAVARGA, PD_URANIAN, PD_YOGAS, PD_VARGA_DIAGRAMS, PD_ERROR };

/*************************************************//**
*
* 
*
******************************************************/
class PrintoutConfigItem : public ConfigBase
{
public:
	PrintoutConfigItem( wxString = wxEmptyString ) {}
	PD_ITEM_TYPE getTypeId() const { return typeId; }

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxString type;

	vector<int> vargaIds;
	int dasaId;
	int subtype;
	bool vedic;

	PD_ITEM_TYPE typeId;

protected:

};

/*************************************************//**
*
* 
*
******************************************************/
class PrintoutConfig : public ConfigBase
{
public:

	PrintoutConfig( wxString name = wxEmptyString );

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	vector<PrintoutConfigItem> items;
	wxString name;
	wxString description;
	bool vedic;

};

/*************************************************//**
*
* \brief reads and contains configs for printouts
*
******************************************************/
class PrintoutConfigLoader : public ConfigListLoader<PrintoutConfig>
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( PrintoutConfigLoader )

public:

	virtual void loadSingleConfig( wxJSONValue& );

private:

	PrintoutConfigLoader();
	PD_ITEM_TYPE itemType2Int( wxString );

};

#endif

