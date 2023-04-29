/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ConfigListLoader.h
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

#ifndef _CONFIGLISTLOADER_H_
#define _CONFIGLISTLOADER_H_

#include "maitreya.h"

#include <wx/arrstr.h>
#include <wx/string.h>
#include <map>
#include <vector>

class wxJSONValue;

enum ConfigResourceType { CrtLocalFile, CrtGlobalFile, CrtLocalDir, CrtGlobalDir, CrtCombinedDir };

/*************************************************//**
*
* 
*
******************************************************/
class ConfigBaseLoader
{
public:
	virtual ~ConfigBaseLoader() {}

protected:

	ConfigBaseLoader( const wxString ftoken, const ConfigResourceType &type );

	virtual void loadSingleConfig( wxJSONValue& ) = 0;

	bool loadConfigs();
	bool loadDefaultConfigs();

	bool parseConfigFile( wxString file, const bool mustExist = false );
	bool traverseConfigDir( wxString dirname, const bool mustExist = false );

	bool needsReload();

	const ConfigResourceType resourceType;
	wxString localResourcename;
	wxString globalResourcename;
	std::map<wxString, long> modtimes;

	// for logging only
	wxString currentfile;
};

/*************************************************//**
*
* 
*
******************************************************/
template<class T> class ConfigListLoader : public ConfigBaseLoader
{
public:

	std::vector<T*> &getConfigs()
	{
		if ( needsReload() || l.size() == 0 ) init();
		return l;
	}

	std::vector<T*> getConfigsDeep()
	{
		if ( needsReload() || l.size() == 0 ) init();
		std::vector<T*> r;
		for( uint i = 0; i < l.size(); i++ )
		{
			T *t = new T( *(l[i]));
			r.push_back( t );
			//r.push_back( new T( *(l[i]) ));
		}
		return r;
	}

	std::vector<T*> getDefaultConfigsDeep()
	{
		std::vector<T*> currentConfigs = getConfigsDeep();
		clear();
		loadDefaultConfigs();
		std::vector<T*> defaultConfigs = getConfigsDeep();
		setConfigs( currentConfigs );
		return defaultConfigs;
	}

	void setConfigs( std::vector<T*> t )
	{
		clear();
		for( uint i = 0; i < t.size(); i++ )
		{
			l.push_back( t[i] );
		}
	}

	T* getConfig( const uint &i )
	{
		if ( needsReload() || l.size() == 0 ) init();
		if ( i < l.size()) return l[i];
		return l[0];
	}

	T* getDefaultConfig()
	{
		return new T( wxT( "Default" ));
	}

	T* getByName( wxString name )
	{
		if ( needsReload() || l.size() == 0 ) init();
		for( uint i = 0; i < l.size(); i++ )
		{
			if ( l[i]->name == name ) return l[i];
		}
		return (T*)NULL;
	}

	uint getIndexByName( wxString name )
	{
		if ( needsReload() || l.size() == 0 ) init();
		for( uint i = 0; i < l.size(); i++ )
		{
			if ( l[i]->name == name ) return i;
		}
		return UINT_FOR_NOT_FOUND;
	}

	uint getSize() { return l.size(); }

	wxArrayString getNamesAsArray()
	{
		if ( needsReload() || l.size() == 0 ) init();
		wxArrayString a;
		for( uint i = 0; i < l.size(); i++ )
		{
			a.Add( l[i]->name );
		}
		return a;
	}


protected:
	
	ConfigListLoader<T>( wxString ftoken, const ConfigResourceType &type )
	 :  ConfigBaseLoader( ftoken, type )
	{
	}

	void init()
	{
		clear();
		loadConfigs();
		if ( l.size() == 0 ) initFallback();
		assert( l.size() > 0 );
	}

	void clear()
	{
		for( uint i = 0; i < l.size(); i++ )
		{
			delete l[i];
		}
		l.clear();
	}

	void initFallback()
	{
		clear();
		l.push_back( getDefaultConfig());
	}

	std::vector<T*> l;
};

#endif


