/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ConfigListLoader.h
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

#ifndef _CONFIGLISTLOADER_H_
#define _CONFIGLISTLOADER_H_

#include "maitreya.h"

#include <wx/arrstr.h>
#include <wx/string.h>
#include <map>
#include <vector>

class wxJSONValue;

using namespace std;

enum ConfigResourceType { CrtPrivateFile, CrtGlobalFile, CrtPrivateDir, CrtGlobalDir };

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

	bool parseConfigFile( wxString file );
	bool traverseConfigDir( wxString dirname );

	bool needsReload();

	const ConfigResourceType resourceType;
	wxString privateResourcename;
	wxString globalResourcename;
	map<wxString, long> modtimes;

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

	vector<T*> &getConfigs()
	{
		if ( needsReload() || l.size() == 0 ) init();
		return l;
	}

	vector<T*> getConfigsDeep()
	{
		if ( needsReload() || l.size() == 0 ) init();
		vector<T*> r;
		for( uint i = 0; i < l.size(); i++ )
		{
			T *t = new T( *(l[i]));
			r.push_back( t );
			//r.push_back( new T( *(l[i]) ));
		}
		return r;
	}

	vector<T*> getDefaultConfigsDeep()
	{
		vector<T*> currentConfigs = getConfigsDeep();
		clear();
		loadDefaultConfigs();
		vector<T*> defaultConfigs = getConfigsDeep();
		setConfigs( currentConfigs );
		return defaultConfigs;
	}

	void setConfigs( vector<T*> t )
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
		for( uint i = 0; i < l.size(); i++ )
		{
			if ( l[i]->name == name ) return l[i];
		}
		return (T*)NULL;
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

	vector<T*> l;
};

#endif


