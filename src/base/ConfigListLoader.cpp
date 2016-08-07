/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ConfigListLoader.cpp
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
		case CrtPrivateFile:
		case CrtGlobalFile:
			privateResourcename = FileConfig::get()->getConfigDir() + ftoken + CFG_JSON_EXTENSION;
			globalResourcename = FileConfig::get()->getResourceDir() + ftoken + CFG_JSON_EXTENSION;
		break;
		case CrtPrivateDir:
		case CrtGlobalDir:
			privateResourcename = FileConfig::get()->getConfigDir() + ftoken + separator;
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
		case CrtPrivateFile:
			return parseConfigFile( privateResourcename );
		break;
		case CrtGlobalFile:
			return parseConfigFile( globalResourcename );
		break;
		case CrtPrivateDir:
			return traverseConfigDir( privateResourcename );
		break;
		case CrtGlobalDir:
			return traverseConfigDir( globalResourcename );
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
		case CrtPrivateFile:
		case CrtGlobalFile:
			return parseConfigFile( globalResourcename );
		break;
		case CrtPrivateDir:
		case CrtGlobalDir:
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
bool ConfigBaseLoader::traverseConfigDir( wxString dirname )
{
	bool b = true;
	wxString f, filename;
	wxLogMessage( wxT( "ConfigBaseLoader::traverseConfigDir resource %s" ), dirname.c_str( ));

	if ( ! wxDir::Exists( dirname ))
	{
		wxLogError( wxString::Format( wxT ( "config directory %s does not exist" ), dirname.c_str() ));
		return false;
	}

	wxDir dir( dirname );
	if ( ! dir.IsOpened() )
	{
		wxLogError( wxString::Format( wxT ( "cannot open config directory %s" ), dirname.c_str() ));
	}
	else
	{
		bool cont = dir.GetFirst( &f, wxT( "*.json" ), wxDIR_FILES );
		if ( ! cont )
		{
			wxLogWarning( wxString::Format( wxT ( "config directory %s is empty" ), dirname.c_str() ));
		}
		while ( cont )
		{
			// Windows implementation doesn't filter extension
			if ( f.Right( 5 ) == wxT( ".json" ))
			{
				filename = dirname + f;
				if ( ! parseConfigFile( filename )) b = false;
			}

			cont = dir.GetNext( &f );
		}
	}
	wxLogMessage( wxT( "ConfigBaseLoader::traverseConfigDir  %s finished" ), dirname.c_str( ));
	return b;
}

/*****************************************************
**
**   ConfigBaseLoader   ---   parseConfigFile
**
******************************************************/
bool ConfigBaseLoader::parseConfigFile( wxString file )
{
	wxLogMessage( wxT( "ConfigBaseLoader::parseConfigFile file %s" ), file.c_str( ));
	if ( ! wxFileName::FileExists( file ))
	{
		wxLogError( wxT( "File %s does not exist, using defaults" ), file.c_str());
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
	for( map<wxString, long>::iterator iter = modtimes.begin(); iter != modtimes.end(); iter++ )
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




