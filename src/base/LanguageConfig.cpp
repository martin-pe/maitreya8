/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/LanguageConfig.cpp
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

#include "LanguageConfig.h"

#include <wx/tokenzr.h>

#include "Conf.h"
#include "FileConfig.h"

extern Config *config;

IMPLEMENT_SINGLETON( LanguageConfig )

/*****************************************************
**
**   LanguageConfig   ---   Constructor
**
******************************************************/
LanguageConfig::LanguageConfig()
{
	locale = 0;
	init();
}

/*****************************************************
**
**   CLASS LangSorter
**
******************************************************/
class LangSorter
{
public:
	bool operator()( const LanguageEntry &l1, const LanguageEntry &l2 ) const
	{
		return l1.name.Cmp( l2.name ) < 0;
	}
};

/*****************************************************
**
**   LanguageConfig   ---   setLanguage
**
******************************************************/
void LanguageConfig::setLanguage( const int& language )
{
	if ( locale != 0 ) delete locale;
	locale = new wxLocale( language );
	//wxLocale::AddCatalogLookupPathPrefix( wxT( "." ));
	wxLocale::AddCatalogLookupPathPrefix( wxT( "./gui" ));

	wxLocale::AddCatalogLookupPathPrefix( wxT( "/usr/share" ));
	wxLocale::AddCatalogLookupPathPrefix( wxT( "/usr/share/locale" ));
#ifdef __WXMAC__
	wxLocale::AddCatalogLookupPathPrefix( MAC_RESOURCE_DIRECTORY );
#endif

	wxLocale::AddCatalogLookupPathPrefix( FileConfig::get()->getBinDir() );
	//wxLocale::AddCatalogLookupPathPrefix( wxT( "../../po" ) );
	locale->AddCatalog( wxT( "maitreya8" ));

	wxString ss = locale->GetInfo( wxLOCALE_DECIMAL_POINT, wxLOCALE_CAT_NUMBER );
	//printf( "LOCALE %s\n", str2char( ss ) );
}

/*****************************************************
**
**   LanguageConfig   ---   init
**
******************************************************/
void LanguageConfig::init()
{
	wxString s;
	wxStringTokenizer t( config->langList);
	std::list<LanguageEntry>::iterator iter;
	int theLang = -1;

#ifdef __WXMSW__
//#if true
	bool found;
	// init langentries vector
	langentries.clear();
	langentries.push_back( LanguageEntry( wxLANGUAGE_ENGLISH_US, wxT( "en" ), _( "English" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_GERMAN, wxT( "de" ), _( "German" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_ITALIAN, wxT( "it" ), _( "Italian" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_POLISH, wxT( "pl" ), _( "Polish" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_RUSSIAN, wxT( "ru" ), _( "Russian" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_TELUGU, wxT( "te" ), _( "Telugu" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_HUNGARIAN, wxT( "hu" ), _( "Hungarian" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_ROMANIAN, wxT( "ro" ), _( "Romanian" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_SPANISH, wxT( "es" ), _( "Spanish" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_FRENCH, wxT( "fr" ), _( "French" )));
	langentries.push_back( LanguageEntry( wxLANGUAGE_TAMIL, wxT( "ta" ), _( "Tamil" )));

	while ( t.HasMoreTokens() )
	{
		s = t.GetNextToken();
		found = false;
		for ( iter = langentries.begin(); iter != langentries.end(); iter++ )
		{
			if ((*iter).code == s )
			{
				found = true;
				break;
			}
		}
		if ( found == false )
		{
			const wxLanguageInfo *info = wxLocale::FindLanguageInfo( s );
			if ( info == 0 )
			{
				printf( "Warning: unknown language %s", str2char ( s ));
			}
			else
			{
				langentries.push_back( LanguageEntry( info->Language, s, info->Description ));
			}
		}
	}
	langentries.sort( LangSorter() );
	const wxLanguageInfo *mylang = wxLocale::FindLanguageInfo( config->lang );
	if ( mylang != 0 )
	{
		for ( iter = langentries.begin(); iter != langentries.end(); iter++ )
		{
			if ( (*iter).code == config->lang )
			{
				theLang = (*iter).wxId;
				break;
			}
		}
	}


	if ( theLang == -1 ) theLang = wxLANGUAGE_ENGLISH_US;
#else
	theLang = wxLANGUAGE_DEFAULT;
#endif
	setLanguage( theLang );

	// Re-translate names of langlist because list was set before locale init
	// New languages must be set manually
	for ( iter = langentries.begin(); iter != langentries.end(); iter++ )
	{
		theLang = (*iter).wxId;
		switch ( theLang )
		{
		case wxLANGUAGE_ENGLISH_US:
			(*iter).name = _( "English" );
			break;
		case wxLANGUAGE_GERMAN:
			(*iter).name = _( "German" );
			break;
		case wxLANGUAGE_RUSSIAN:
			(*iter).name = _( "Russian" );
			break;
		case wxLANGUAGE_TELUGU:
			(*iter).name = _( "Telugu" );
			break;
		case wxLANGUAGE_POLISH:
			(*iter).name = _( "Polish" );
			break;
		case wxLANGUAGE_ITALIAN:
			(*iter).name = _( "Italian" );
			break;
		case wxLANGUAGE_HUNGARIAN:
			(*iter).name = _( "Hungarian" );
			break;
		case wxLANGUAGE_ROMANIAN:
			(*iter).name = _( "Romanian" );
			break;
		case wxLANGUAGE_SPANISH:
			(*iter).name = _( "Spanish" );
			break;
		case wxLANGUAGE_FRENCH:
			(*iter).name = _( "French" );
			break;
		case wxLANGUAGE_TAMIL:
			(*iter).name = _( "Tamil" );
			break;
		}
	}
}
