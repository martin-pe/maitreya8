/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ConfigListLoader.cpp
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

#include "ConfigListLoader.h"

#include "FileConfig.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/jsonreader.h>
#include <wx/log.h>
#include <wx/wfstream.h>

#define CFG_CONFIGS wxT( "configs" )

/*****************************************************
**
**   ConfigBaseLoader   ---   Constructor
**
******************************************************/
ConfigBaseLoader::ConfigBaseLoader( const wxString ftoken, const ConfigResourceType &type )
	: resourceType( type )
{
	const wxString separator = wxFileName::GetPathSeparator();
	switch( resourceType )
	{
		case CrtLocalFile:
		case CrtGlobalFile:
			localResourcename = FileConfig::get()->getConfigDir() + ftoken + CFG_JSON_EXTENSION;
			globalResourcename = FileConfig::get()->getResourceDir() + ftoken + CFG_JSON_EXTENSION;
		break;
		case CrtLocalDir:
		case CrtGlobalDir:
		case CrtCombinedDir:
			localResourcename = FileConfig::get()->getConfigDir() + ftoken + separator;
			globalResourcename = FileConfig::get()->getResourceDir() + ftoken + separator;
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   ConfigBaseLoader   ---   loadConfigs
**
******************************************************/
bool ConfigBaseLoader::loadConfigs()
{
	switch( resourceType )
	{
		case CrtLocalFile:
			return parseConfigFile( localResourcename );
		break;
		case CrtGlobalFile:
			return parseConfigFile( globalResourcename, true );
		break;
		case CrtLocalDir:
			return traverseConfigDir( localResourcename, true );
		break;
		case CrtGlobalDir:
			return traverseConfigDir( globalResourcename, true );
		break;
		case CrtCombinedDir:
		{
			bool b = traverseConfigDir( globalResourcename, true );
			traverseConfigDir( localResourcename );
			return b;
		}
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   ConfigBaseLoader   ---   loadDefaultConfigs
**
******************************************************/
bool ConfigBaseLoader::loadDefaultConfigs()
{
	switch( resourceType )
	{
		case CrtLocalFile:
		case CrtGlobalFile:
			return parseConfigFile( globalResourcename );
		break;
		case CrtLocalDir:
		case CrtGlobalDir:
		case CrtCombinedDir:
			return traverseConfigDir( globalResourcename );
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   ConfigBaseLoader   ---   traverseConfigDir
**
******************************************************/
bool ConfigBaseLoader::traverseConfigDir( wxString dirname, const bool mustExist )
{
	bool b = true;
	wxArrayString files;
	int count = 0;
	wxLogMessage( wxT( "ConfigBaseLoader::traverseConfigDir resource %s" ), dirname.c_str( ));

	if ( ! wxDir::Exists( dirname ))
	{
		if ( mustExist ) wxLogError( wxString::Format( wxT ( "config directory %s does not exist" ), dirname.c_str() ));
		return false;
	}

	//size_t l = 
	wxDir::GetAllFiles( dirname, &files, wxT( "*.json" ));
	files.Sort();
	for ( uint i = 0; i < files.GetCount(); i++ )
	{
		// filespec in wxDir::GetAllFiles is not reliable, so repeat filtering
		if ( files[i].Right( 5 ) == wxT( ".json" ))
		{
			count++;
			if ( ! parseConfigFile( files[i] )) b = false;
		}
	}
	if ( count == 0 )
	{
		wxLogError( wxString::Format( wxT ( "config directory %s contains no JSON files" ), dirname.c_str() ));
		b = false;
	}

	wxLogMessage( wxT( "ConfigBaseLoader::traverseConfigDir %s finished and found %d appropriate files" ), dirname.c_str(), count );
	return b;
}

/*****************************************************
**
**   ConfigBaseLoader   ---   parseConfigFile
**
******************************************************/
bool ConfigBaseLoader::parseConfigFile( wxString file, const bool mustExist )
{
	wxLogMessage( wxT( "ConfigBaseLoader::parseConfigFile file %s" ), file.c_str( ));
	if ( ! wxFileName::FileExists( file ))
	{
		if ( mustExist ) wxLogError( wxT( "File %s does not exist, using defaults" ), file.c_str());
		return false;
	}
	if ( ! wxFileName::IsFileReadable( file ))
	{
		wxLogError( wxT( "File %s is not readable, using defaults" ), file.c_str());
		return false;
	}
	if ( wxFileName::GetSize( file ) == 0 )
	{
		wxLogError( wxT( "File %s has NULL size, using defaults" ), file.c_str());
		return false;
	}

	wxJSONReader reader;
	wxJSONValue root;

	currentfile = file;
	wxFileInputStream stream( file );
	const int numErrors = reader.Parse( stream, &root );
	if ( numErrors > 0 )
	{
		wxLogError( wxString::Format( wxT( "the JSON document %s is not well-formed" ), file.c_str() ));
		const wxArrayString& errors = reader.GetErrors();
		for ( uint i = 0; i < errors.GetCount(); i++ )
		{
			wxLogError( errors[i] );
		}
		currentfile.clear();
		return false;
	}
	else wxLogMessage( wxT( "Document has no errors" ));
	modtimes[ file ] = ::wxFileModificationTime( file );

	wxJSONValue &jconfigs = root[ CFG_CONFIGS ];
	const bool isArray = jconfigs.IsArray();

	if ( isArray )
	{
		for ( int i = 0; i < jconfigs.Size(); i++ )
		{
			loadSingleConfig( jconfigs[i] );
		}
	}
	else // no array
	{
		wxLogError( wxT( "No valid sheet array found" ));
	}

	currentfile.clear();
	wxLogMessage( wxT( "ConfigBaseLoader::parseConfigFile  %si finished" ), file.c_str());
	return true;
}

/*****************************************************
**
**   ConfigBaseLoader   ---   needsReload
**
******************************************************/
bool ConfigBaseLoader::needsReload() 
{
	//static int i = 0;
	//printf( "Start ConfigBaseLoader::needsReload %d\n", i++ );
	for( std::map<wxString, long>::iterator iter = modtimes.begin(); iter != modtimes.end(); iter++ )
	{
		//printf( "ConfigBaseLoader::needsReload modtimes %ld ::wxFileModificationTime( filename ) %ld\n", iter->second, ::wxFileModificationTime( iter->first ));
		if ( ::wxFileModificationTime( iter->first ) != iter->second )
		{
			modtimes.clear();
			return true;
		}
	}
	return false;
}




