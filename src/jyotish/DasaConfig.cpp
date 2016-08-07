/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/DasaConfig.cpp
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

#include "DasaConfig.h"

#include "DasaTool.h"
#include "FileConfig.h"
#include "JSonTool.h"
#include "maitreya.h"

#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/jsonwriter.h>
#include <wx/log.h>
#include <wx/wfstream.h>

/*****************************************************
**
**   DasaConfig   ---   Constructor
**
******************************************************/
DasaConfig::DasaConfig( wxString name )
 : description( name )
{
	description = _( "New Dasa" );
	lord = sublord = subsublord = -1;
	isvalid = true;
	dasaId = D_NONE;
}

/*****************************************************
**
**   DasaConfig   ---   matches
**
******************************************************/
bool DasaConfig::matches( Dasa *dasa )
{
	if ( ! dasa ) return false;
	//printf( "DasaConfig::matches dasaId %d level %d lord %d\n", (int)dasaId, dasa->getLevel(), dasa->getDasaLord());

	if ( this->dasaId != dasa->getDasaId() ) return false;
	if ( dasa->getLevel() == 0 )
	{
		return this->lord == dasa->getDasaLord() && this->sublord == ONONE;
	}
	else if ( dasa->getLevel() == 1 )
	{
		Dasa *parent = dasa->getParent();
		if ( parent )
		{
			return this->lord == dasa->getDasaLord() && this->sublord == parent->getDasaLord();
		}
	}
	return false;
}

IMPLEMENT_SINGLETON( DasaConfigLoader )

/*****************************************************
**
**   DasaConfigLoader   ---   Constructor
**
******************************************************/
DasaConfigLoader::DasaConfigLoader()
	: ConfigListLoader<DasaConfig>( DASA_DIR_NAME, CrtGlobalDir )
{
}

/*****************************************************
**
**   DasaConfigLoader   ---   loadSingleConfig
**
******************************************************/
void DasaConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	DasaTool *tool = DasaTool::get();
	DasaConfig *cfg = new DasaConfig;

	if ( ! v[ CFG_DESCRIPTION ].IsNull()) cfg->description = v[ CFG_DESCRIPTION ].AsString();
	else cfg->description = wxT( "error: description missing" );

	if( ! v[ CFG_EFFECT ].IsNull()) cfg->effect = v[ CFG_EFFECT ].AsString();
	else cfg->effect = wxT( "error: effect not set" );

	if ( ! v[ CFG_SOURCE ].IsNull())
	{
		cfg->source = v[ CFG_SOURCE ].AsString();
		sources.insert( cfg->source );
	}
	else cfg->source = wxT( "error: source not set" );

	if ( ! v[ CFG_LORD ].IsNull())
	{
		cfg->lord = (int)tool->planet2ObjectId( v[ CFG_LORD ].AsString());
		if ( cfg->lord == OERROR )
		{
			cfg->isvalid = false;
			wxLogError( wxString::Format( wxT( "wrong lord %s in file %s" ), v[ CFG_LORD ].AsString().c_str(), currentfile.c_str()));
		}
	}

	if ( ! v[ CFG_SUBLORD ].IsNull())
	{
		cfg->sublord = (int)tool->planet2ObjectId( v[ CFG_SUBLORD ].AsString());
		if ( cfg->sublord == OERROR )
		{
			cfg->isvalid = false;
			wxLogError( wxString::Format( wxT( "wrong sublord %s in file %s" ), v[ CFG_SUBLORD ].AsString().c_str(), currentfile.c_str()));
		}
	}

	if ( ! v[ CFG_DASA ].IsNull())
	{
		cfg->dasaId = tool->dasa2DasaId( v[ CFG_DASA ].AsString());
	}
	l.push_back( cfg );
}

/*****************************************************
**
**   DasaConfigLoader   ---   saveConfigs
**
******************************************************/
/*
void DasaConfigLoader::saveConfigs( wxString filename, const vector<DasaConfig*> yl )
{
	wxJSONWriter writer;
	wxJSONValue root;

	// TODO not finished yes
	assert( false );

	printf( "DasaConfigLoader::saveConfigs\n" );
	for ( uint i = 0; i < yl.size(); i++ )
	{
		wxJSONValue cfgroot;
		cfgroot[ CFG_DESCRIPTION ] = yl[i]->description;
		cfgroot[ CFG_SOURCE ] = yl[i]->source;
		cfgroot[ CFG_EFFECT ] = yl[i]->effect;

		root[ CFG_CONFIGS ].Append( cfgroot );
	}

	wxString s;
	writer.Write( root, s );
	PrintLn( s );

	wxFileOutputStream stream( filename );
	writer.Write( root, stream );

	printf( "ENDE DasaConfigLoader::saveConfigs filename %s\n", str2char( filename ));
}
*/


