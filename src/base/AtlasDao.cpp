/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AtlasDao.cpp
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


#include "AtlasDao.h"

#include "Conf.h"

#include "wx/tokenzr.h"
#include "wx/wxsqlite3.h"

extern Config *config;

#define MAX_ERROR_STACK_SIZE 10

// Field usercode in table geonames: data from system (geonames.sql) / edited by user / new entry
enum { GEONAMES_USERCODE_SYSTEM = 0, GEONAMES_USERCODE_EDITED, GEONAMES_USERCODE_NEW };

// View names for atlas select
static const wxChar *atlas_viewname[3] = { wxT( "atlas" ), wxT( "atlas_ascii" ), wxT( "atlas_alias" ) };

/*****************************************************
**
**   AtlasEntry   ---   clear
**
******************************************************/
void AtlasEntry::clear()
{
	id = rowid = 0;
	name.Clear();
	asciiname.Clear();
	aliases.Clear();
	country_code.Clear();
	country.Clear();
	feature_code.Clear();
	feature_class.Clear();
	feature_name.Clear();
	admin1_code.Clear();
	admin.Clear();
	population = elevation = 0;
	a_longitude = a_latitude = 0;
	c_longitude = c_latitude = 0;
	timezone.Clear();
	tzoffset = 0;
}

/*****************************************************
**
**   AtlasEntry   ---   setters and getters
**
******************************************************/
void AtlasEntry::setLongitude( const double &l )
{
	a_longitude = fabs( l );
	c_longitude = l < 0 ? 1 : 0;
}

void AtlasEntry::setLatitude( const double &l )
{
	a_latitude = fabs( l );
	c_latitude = l < 0 ? 1 : 0;
}

double AtlasEntry::getLongitude() const { return( c_longitude == 1 ? - a_longitude : a_longitude ); }
double AtlasEntry::getLatitude() const { return( c_latitude == 1 ? - a_latitude : a_latitude ); }

/*****************************************************
**
**   AtlasDao   ---   Constructor
**
******************************************************/
AtlasDao::AtlasDao( const wxString s )
		: BaseDao( s )
{
}

/*****************************************************
**
**   AtlasDao   ---   getCountryName
**
******************************************************/
wxString AtlasDao::getCountryName( wxString iso )
{
	wxString s;
	queryString = wxT( "SELECT name FROM countries where iso = ?" );
	try
	{
		wxSQLite3Database *db = getDbHandle();
		if ( db )
		{
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, iso );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow()) s = set.GetAsString( 0 );
			stmt.Finalize();
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}
	return s;
}

/*****************************************************
**
**   AtlasDao   ---   getAllAdminNamesForCountry
**
******************************************************/
std::list<wxString> AtlasDao::getAllAdminNamesForCountry( const wxString &country_name )
{
	std::list<wxString> l;
	try
	{
		wxSQLite3Database *db = getDbHandle();
		if ( db )
		{
			queryString = wxT( "SELECT iso FROM countries where name = ?" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, country_name );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow())
			{
				wxString country_code = set.GetAsString( 0 );
				queryString = wxT( "SELECT name FROM admincodes where country = ?" );
				wxSQLite3Statement stmt2 = db->PrepareStatement( queryString );
				stmt2.Bind( 1, country_code );
				wxSQLite3ResultSet set2 = stmt2.ExecuteQuery();
				while (set2.NextRow())
				{
					l.push_back( set2.GetAsString( 0 ));
				}
				stmt2.Finalize();
			}
			stmt.Finalize();
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}
	return l;
}

/*****************************************************
**
**   AtlasDao   ---   getFeatureName
**
******************************************************/
wxString AtlasDao::getFeatureName( wxString feature_class, wxString feature_code )
{
	wxString s = feature_code;
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			queryString = wxT( "SELECT description1 FROM featurecodes where feature_class =? and feature_code=?" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, feature_code );
			stmt.Bind( 2, feature_class );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow()) s = set.GetAsString( 0 );
			else
				//printf( "WARN: no feature name for class %s and code %s\n", feature_class.c_str(), feature_code.c_str() );
				printf( "WARN: no feature name for class %s and code %s\n", str2char( feature_class ), str2char( feature_code ) );
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return s;
}

/*****************************************************
**
**   AtlasDao   ---   getAdminName
**
******************************************************/
wxString AtlasDao::getAdminName( wxString country_code, wxString admin1_code )
{
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			queryString = wxT( "SELECT name FROM admincodes where country =? and admin1code=?" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, country_code );
			stmt.Bind( 2, admin1_code );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow()) return set.GetAsString( 0 );
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return wxEmptyString;
}

/*****************************************************
**
**   AtlasDao   ---   getCountryCodeForName
**
******************************************************/
wxString AtlasDao::getCountryCodeForName( wxString name )
{
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			queryString = wxT( "SELECT iso FROM countries where name =?" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, name );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow()) return set.GetAsString( 0 );
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return wxEmptyString;
}

/*****************************************************
**
**   AtlasDao   ---   getAdminCodeForCountryAndName
**
******************************************************/
wxString AtlasDao::getAdminCodeForCountryAndName( wxString country_code, wxString name )
{
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			queryString = wxT( "SELECT admin1code FROM admincodes where country =? and name=?" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, country_code );
			stmt.Bind( 2, name );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow()) return set.GetAsString( 0 );
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return wxEmptyString;
}

/*****************************************************
**
**   AtlasDao   ---   getAllTimezones
**
******************************************************/
std::list<TimezoneEntry> AtlasDao::getAllTimezones()
{
	std::list<TimezoneEntry> list;
	try
	{
		wxSQLite3Database *db = getDbHandle();
		if ( db )
		{
			queryString = wxT( "SELECT name,country_code,offset FROM timezones" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			while (set.NextRow())
			{
				list.push_back( TimezoneEntry( set.GetAsString( 0 ), set.GetAsString( 1 ), set.GetDouble( 2 ) ));
			}
			set.Finalize();
			stmt.Finalize();
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}
	return list;
}

/*****************************************************
**
**   AtlasDao   ---   getAllCountries
**
******************************************************/
std::list<AtlasCountry> AtlasDao::getAllCountries()
{
	std::list<AtlasCountry> list;
	try
	{
		wxSQLite3Database *db = getDbHandle();
		if ( db )
		{
			queryString = wxT( "SELECT iso,name FROM countries" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			while (set.NextRow())
			{
				list.push_back( AtlasCountry( set.GetAsString( 0 ), set.GetAsString( 1 ) ));
			}
			set.Finalize();
			stmt.Finalize();
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}
	list.sort( AtlasCountrySorter() );
	return list;
}

/*****************************************************
**
**   AtlasDao   ---   getEntries
**
******************************************************/
std::vector<AtlasEntry> AtlasDao::getEntries( wxString filter, wxString country_code, const int &mode, const int &limit, const int &offset )
{
	std::vector<AtlasEntry> list;
	assert( mode >= 0 && mode < 3 );
	bool f = ! filter.IsEmpty();
	bool c = ! country_code.IsEmpty();

	queryString.Clear();
	queryString << wxT( "SELECT id,name,country_code,admin1_code,latitude,longitude FROM " ) << atlas_viewname[mode];

	if ( f && !c ) queryString << wxT( " WHERE name like ?" );
	else if ( ! f && c ) queryString << wxT( " WHERE country_code=?" );
	else if ( f && c ) queryString <<  wxT( " WHERE name like ? AND country_code=?" );

	queryString << wxT( " ORDER BY NAME LIMIT ? OFFSET ?" );

	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			int index = 1;
			if ( f && !c )
			{
				stmt.Bind( index++, filter + wxT( "%" ) );
			}
			else if ( ! f && c )
			{
				stmt.Bind( index++, country_code );
			}
			else if ( f && c )
			{
				stmt.Bind( index++, filter + wxT( "%" ));
				stmt.Bind( index++, country_code );
			}
			stmt.Bind( index++, limit );
			stmt.Bind( index++, offset );

			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			int count = 0;
			while (set.NextRow() && count < ATLAS_MAX_GRID_ELEMENTS )
			{
				AtlasEntry entry;
				entry.id = set.GetInt( 0 );
				entry.name = set.GetAsString( 1 );
				entry.country_code = set.GetAsString( 2 );
				entry.admin1_code = set.GetAsString( 3 );
				entry.setLatitude( set.GetDouble( 4 ));
				entry.setLongitude( set.GetDouble( 5 ));
				entry.rowid = offset + count++;
				list.push_back( entry );
			}
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return list;
}

/*****************************************************
**
**   AtlasDao   ---   getMatchCount
**
******************************************************/
int AtlasDao::getMatchCount( wxString filter, wxString country_code, const int &mode )
{
	bool f = ! filter.IsEmpty();
	bool c = ! country_code.IsEmpty();

	queryString.Clear();
	queryString << wxT( "SELECT COUNT(*) FROM " ) << atlas_viewname[mode];
	if ( f && !c )
	{
		queryString << wxT( " WHERE name like ?" );
	}
	else if ( ! f && c )
	{
		queryString << wxT( " WHERE country_code=?" );
	}
	else if ( f && c )
	{
		queryString <<  wxT( " WHERE name like ? AND country_code=?" );
	}

	int count = 0;
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			if ( f && !c )
			{
				stmt.Bind( 1, filter + wxT( "%" ) );
			}
			else if ( ! f && c )
			{
				stmt.Bind( 1, country_code );
			}
			else if ( f && c )
			{
				stmt.Bind( 1, filter + wxT( "%" ));
				stmt.Bind( 2, country_code );
			}
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow() )
			{
				count = set.GetInt( 0 );
			}
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return count;
}

/*****************************************************
**
**   AtlasDao   ---   getFullEntry
**
******************************************************/
AtlasEntry AtlasDao::getFullEntry( int id )
{
	AtlasEntry entry;
	queryString.Clear();
	queryString << wxT( "SELECT g.featureid,g.name,g.asciiname,g.country_code,g.feature_class,g.feature_code,g.admin1_code," )
		<< wxT( "g.population,g.elevation,g.latitude,g.longitude,g.timezone,t.offset FROM geonames g, timezones t " )
		<< wxT( "WHERE g.timezone=t.name AND g.featureid = ?" );

	try
	{
		wxSQLite3Database *db = getDbHandle();
		if ( db )
		{
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, id );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow())
			{
				entry.id = set.GetInt( 0 );
				entry.name = set.GetAsString( 1 );
				entry.asciiname = set.GetAsString( 2 );
				entry.country_code = set.GetAsString( 3 );
				entry.feature_code = set.GetAsString( 4 );
				entry.feature_class = set.GetAsString( 5 );
				entry.admin1_code = set.GetAsString( 6 );
				entry.population = set.GetInt( 7 );
				entry.elevation = set.GetInt( 8 );
				entry.setLatitude( set.GetDouble( 9 ));
				entry.setLongitude( set.GetDouble( 10 ));
				entry.timezone = set.GetAsString( 11 );
				entry.tzoffset = set.GetDouble( 12 );
			}
			set.Finalize();
			stmt.Finalize();

			queryString = wxT( "SELECT alias FROM geonames_aliases WHERE featureid= ?" );
			wxSQLite3Statement stmt2 = db->PrepareStatement( queryString );
			stmt2.Bind( 1, id );
			wxSQLite3ResultSet set2 = stmt2.ExecuteQuery();
			int count = 0;
			while (set2.NextRow())
			{
				if ( count > 0 ) entry.aliases += wxT( ", " );
				entry.aliases += set2.GetAsString( 0 );
				count++;
			}
			set2.Finalize();
			stmt2.Finalize();
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}

	return entry;
}

/*****************************************************
**
**   AtlasDao   ---   saveEntry
**
******************************************************/
void AtlasDao::saveEntry( AtlasEntry &entry )
{
	bool exists = entryExists( entry.id );
	if ( exists ) saveExistingEntry( entry );
	else saveNewEntry( entry );
	saveAliasNames( entry.id, entry.country_code, entry.aliases );
}

/*****************************************************
**
**   AtlasDao   ---   saveExistingEntry
**
******************************************************/
void AtlasDao::saveExistingEntry( const AtlasEntry &entry )
{
	queryString.Clear();
	queryString << wxT( "UPDATE geonames SET name=?, asciiname=?, latitude=?, longitude=?, country_code=?, " )
		<< wxT( "admin1_code=?, population=?, elevation=?, timezone=?, usercode=? WHERE featureid=?" );

	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, entry.name );
			stmt.Bind( 2, entry.asciiname );
			stmt.Bind( 3, entry.getLatitude() );
			stmt.Bind( 4, entry.getLongitude() );
			stmt.Bind( 5, entry.country_code );
			stmt.Bind( 6, entry.admin1_code );
			stmt.Bind( 7, entry.population );
			stmt.Bind( 8, entry.elevation );
			stmt.Bind( 9, entry.timezone );
			stmt.Bind( 10, GEONAMES_USERCODE_EDITED );
			stmt.Bind( 11, entry.id );

			stmt.ExecuteUpdate();
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
}

/*****************************************************
**
**   AtlasDao   ---   saveNewEntry
**
******************************************************/
void AtlasDao::saveNewEntry(  AtlasEntry &entry )
{

	if( entry.id == 0 )
	{
		entry.id = getNewFeatureId();
	}
	try
	{
		wxSQLite3Database *db = getDbHandle();
		if ( db )
		{
			queryString.Clear();
			queryString << wxT( "INSERT INTO geonames (featureid, name, asciiname, latitude, longitude, country_code, " )
				<< wxT( "admin1_code, population, elevation, timezone, usercode ) " )
				<< wxT( "VALUES( ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ? )" );

			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, entry.id );
			stmt.Bind( 2, entry.name );
			stmt.Bind( 3, entry.asciiname );
			stmt.Bind( 4, entry.getLatitude() );
			stmt.Bind( 5, entry.getLongitude() );
			stmt.Bind( 6, entry.country_code );
			stmt.Bind( 7, entry.admin1_code );
			stmt.Bind( 8, entry.population );
			stmt.Bind( 9, entry.elevation );
			stmt.Bind( 10, entry.timezone );
			stmt.Bind( 11, GEONAMES_USERCODE_NEW );

			stmt.ExecuteUpdate();
			stmt.Finalize();
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}
}

/*****************************************************
**
**   AtlasDao   ---   saveAliasNames
**
******************************************************/
void AtlasDao::saveAliasNames( const int &id, wxString /* country_code */, wxString names )
{
	wxString s;
	uint i;
	bool b;

	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			// alias names in database
			std::vector<wxString> db_names;

			// alias names in database that occur in input string, other ones must be deleted from database
			std::vector<bool> found;

			queryString = wxT( "SELECT alias FROM geonames_aliases WHERE featureid=?" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, id );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			while (set.NextRow())
			{
				db_names.push_back( set.GetString( 0 ));
				found.push_back( false );
			}
			stmt.Finalize();

			wxStringTokenizer tk( names, wxT( "," ));
			while( tk.HasMoreTokens())
		  {
				s = AllTrim( tk.GetNextToken());
				b = false;
				for( i = 0; i < db_names.size(); i++ )
				{
					if ( db_names[i] == s )
					{
						found[i] = true;
						b = true;
						break;
					}
				}
				if ( ! b )
				{
					// string s not found in DB, insert
					queryString = wxT( "INSERT INTO geonames_aliases VALUES( ?, ? )" );
					wxSQLite3Statement stmt2 = db->PrepareStatement( queryString );
					stmt2.Bind( 1, id );
					stmt2.Bind( 2, s );
					stmt2.ExecuteUpdate();
					stmt2.Finalize();
				}
			}

			// find DB items to delete
			for( i = 0; i < db_names.size(); i++ )
			{
				if ( found[i] == false )
				{
					queryString = wxT( "DELETE FROM geonames_aliases WHERE featureid=? and alias=?" );
					wxSQLite3Statement stmt3 = db->PrepareStatement( queryString );
					stmt3.Bind( 1, id );
					stmt3.Bind( 2, db_names[i] );
					stmt3.ExecuteUpdate();
					stmt3.Finalize();
				}
			}
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
}

/*****************************************************
**
**   AtlasDao   ---   getNewFeatureId
**
******************************************************/
int AtlasDao::getNewFeatureId()
{
	int id = 0;
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			queryString = wxT( "SELECT MAX( featureid ) FROM geonames" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow()) id = set.GetInt( 0 );
			stmt.Finalize();
			id++;
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return id;
}

/*****************************************************
**
**   AtlasDao   ---   entryExists
**
******************************************************/
bool AtlasDao::entryExists( const int &id )
{
	bool b = false;
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			queryString = wxT( "SELECT EXISTS( SELECT 1 FROM geonames WHERE featureid = ?)" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, id );
			wxSQLite3ResultSet set = stmt.ExecuteQuery();
			if (set.NextRow()) b = set.GetInt( 0 );
			stmt.Finalize();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return b;
}

/*****************************************************
**
**   AtlasDao   ---   deleteEntry
**
******************************************************/
void AtlasDao::deleteEntry( const int& id )
{
	try
	{
		wxSQLite3Database *db = getDbHandle();
		if ( db )
		{
			queryString = wxT( "DELETE FROM geonames WHERE featureid =?" );
			wxSQLite3Statement stmt = db->PrepareStatement( queryString );
			stmt.Bind( 1, id );
			stmt.ExecuteUpdate();
			stmt.Finalize();

			queryString = wxT( "DELETE FROM geonames_aliases WHERE featureid = ?" );
			wxSQLite3Statement stmt2 = db->PrepareStatement( queryString );
			stmt2.Bind( 1, id );
			stmt2.ExecuteUpdate();
			stmt2.Finalize();
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}
}

