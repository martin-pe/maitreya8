/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/DasaConfig.cpp
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
			return this->lord == parent->getDasaLord() && this->sublord == dasa->getDasaLord();
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


