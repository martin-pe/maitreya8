/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/LanguageConfig.h
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
#ifndef _LANGUAGECONFIG_H_
#define _LANGUAGECONFIG_H_

#include <wx/string.h>
#include <list>
#include "maitreya.h"

class wxLocale;

using namespace std;

/*************************************************//**
*
*
*
******************************************************/
class LanguageEntry
{
public:
	LanguageEntry( const int &id, const wxString &langcode, const wxString &thename )
	{
		wxId = id;
		code = langcode;
		name = thename;
	}
	int wxId;
	wxString code;
	wxString name;
};

/*************************************************//**
*
*  applicatino wide language configuration
*
******************************************************/
class LanguageConfig
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( LanguageConfig )

public:

	list<LanguageEntry> getLanguages() { return langentries; }
	wxLocale *getLocale() { return locale; }
	void setLanguage( const int& language );

private:

	LanguageConfig();

	void init();
	wxLocale *locale;
	list<LanguageEntry> langentries;
};

#endif

