/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/DasaConfig.h
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

#ifndef _DASACONFIG_H_
#define _DASACONFIG_H_

#include <wx/string.h>
#include <set>

#include "ConfigListLoader.h"
#include "Dasa.h"
#include "maitreya.h"

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

	std::set<wxString> &getSources() { return sources; }

private:

	DasaConfigLoader();
	std::set<wxString> sources;

};

#endif


