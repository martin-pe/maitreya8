/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AtlasLogic.h
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

#ifndef ATLASLOGIC_H
#define ATLASLOGIC_H

#include "AtlasDao.h"
#include "maitreya.h"

#include <list>
#include <map>
#include <vector>
#include <wx/event.h>

using namespace std;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( ATLAS_COUNT_HASNEWS, wxID_HIGHEST + 1500 )
DECLARE_EVENT_TYPE( ATLAS_FETCH_HASNEWS, wxID_HIGHEST + 1501 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
*
*
******************************************************/
class AtlasLogic
{
	friend class AtlasLogicWorker;
	friend class AtlasLogicCountWorker;
	friend class AtlasLogicFetchWorker;

public:

	AtlasLogic( const bool runWorkerThreads = false );
	~AtlasLogic();

	list<AtlasCountry> getAllCountries();
	list<AtlasCountry> getFavouriteCountries( vector<wxString> );
	wxString getCountryCodeForName( wxString name );

	list<TimezoneEntry> getAllTimezones();

	list<wxString> getAllAdminNamesForCountry( const wxString &country_code );
	wxString getAdminCodeForCountryAndName( wxString country_code, wxString name );

	void setFilterConditions( wxString f, wxString c, const int& m );
	void updateFilter();

	AtlasEntry *getEntryByRowId( const int &rowid );
	AtlasEntry getFullEntry( const int &featureid );

	bool countHasNews();
	int getCount();
	void aknowledgeCountHasNews();

	bool fetchHasNews();
	void aknowledgeFetchHasNews();

	void saveEntry( AtlasEntry& );
	void deleteEntry( const int& id );

private:

	class AtlasLogicSharedSection *sharedSection;
	class AtlasLogicCountWorker *countWorker;
	class AtlasLogicFetchWorker *fetchWorker;
	class AtlasLogicLifeCycleWorker *lifeCycleWorker;

	class AtlasDao *dao;
	map<wxString, wxString> countrynames;
	map<wxString, wxString> featurenames;
	map<wxString, wxString> adminnames;

	wxString getCountryName( wxString iso );
	wxString getAdminName( wxString country_code, wxString admin1_code );
	wxString getFeatureName( wxString feature_class, wxString feature_code );
	void resetEntries();

	wxString country;
	wxString filter;
	int mode;
	AtlasEntry *entries[ATLAS_MAX_GRID_ELEMENTS];

protected:


};


#endif

