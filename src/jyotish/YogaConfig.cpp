/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/YogaConfig.cpp
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

#include "YogaConfig.h"

#include "maitreya.h"
#include "Expression.h"
#include "FileConfig.h"
#include "JSonTool.h"

#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>
#include <wx/log.h>
#include <wx/wfstream.h>
#include <wx/xml/xml.h>

/*****************************************************
**
**   YogaConfig   ---   Constructor
**
******************************************************/
YogaConfig::YogaConfig( wxString name )
 : description( name )
{
	rule = (Rule*)NULL;
	description = _( "New Yoga" );
	allowHigherVargas = true;
}

/*****************************************************
**
**   YogaConfig   ---   Destructor
**
******************************************************/
YogaConfig::~YogaConfig()
{
	if ( rule ) delete rule;
}

IMPLEMENT_SINGLETON( YogaConfigLoader )

/*****************************************************
**
**   YogaConfigLoader   ---   Constructor
**
******************************************************/
YogaConfigLoader::YogaConfigLoader()
	: ConfigListLoader<YogaConfig>( YOGA_DIR_NAME, CrtLocalDir )
{
}

/*****************************************************
**
**   YogaConfigLoader   ---   loadSingleConfigInternal
**
******************************************************/
YogaConfig *YogaConfigLoader::loadSingleConfigInternal( wxJSONValue &v )
{
	YogaConfig *cfg = new YogaConfig;

	if ( ! v[ CFG_DESCRIPTION ].IsNull()) cfg->description = v[ CFG_DESCRIPTION ].AsString();
	else cfg->description = wxT( "error: description missing" );

	if ( ! v[ CFG_SOURCE ].IsNull()) cfg->source = v[ CFG_SOURCE ].AsString();
	else cfg->source = wxT( "source not set" );

	if ( ! v[ CFG_GROUP ].IsNull()) cfg->group = v[ CFG_GROUP ].AsString();
	else cfg->group = wxT( "group not set" );

	if ( ! v[ CFG_EFFECT ].IsNull()) cfg->effect = v[ CFG_EFFECT ].AsString();
	else cfg->effect = wxT( "error: effect not set" );

	if ( ! v[ CFG_RULE ].IsNull()) cfg->rulestr = v[ CFG_RULE ].AsString();
	else wxLogError( wxT( "rule for Yoga not set" ));

	if ( ! v[ CFG_HIGHER_VARGAS ].IsNull()) cfg->allowHigherVargas = v[ CFG_HIGHER_VARGAS ].AsBool();

	return cfg;
}

/*****************************************************
**
**   YogaConfigLoader   ---   loadSingleConfig
**
******************************************************/
void YogaConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	l.push_back( loadSingleConfigInternal( v ));
}

/*****************************************************
**
**   YogaConfigLoader   ---   loadConfig
**
******************************************************/
bool YogaConfigLoader::loadConfig( wxString filename, std::vector<YogaConfig*> &yl )
{
	//printf( "YogaConfigLoader::loadConfig %s\n", str2char( filename ));
  wxJSONReader reader;
	wxJSONValue root;

  wxFileInputStream stream( filename );
  const int numErrors = reader.Parse( stream, &root );
  if ( numErrors > 0 )
  {
    wxLogError( wxString::Format( wxT( "the JSON document %s is not well-formed" ), filename.c_str() ));
    const wxArrayString& errors = reader.GetErrors();
    for ( uint i = 0; i < errors.GetCount(); i++ )
    {
      wxLogError( errors[i] );
    }
    return false;
  }

	wxJSONValue &yconfigs = root[ CFG_CONFIGS ];

	const bool isArray = yconfigs.IsArray();
	//printf( "ARRAY %d size %d\n", isArray, yconfigs.Size());

	if ( isArray )
	{
		for ( int i = 0; i < yconfigs.Size(); i++ )
		{
			yl.push_back( loadSingleConfigInternal( yconfigs[i] ));
		}
	}
	else
	{
		wxLogError( wxString::Format( wxT( "cannon parse file %s, no config array found" ), filename.c_str()));
		return false;
	}
	return true;
}

/*****************************************************
**
**   YogaConfigLoader   ---   saveConfigs
**
******************************************************/
void YogaConfigLoader::saveConfigs( wxString filename, const std::vector<YogaConfig*> yl )
{
	wxJSONWriter writer;
	wxJSONValue root;

	//printf( "YogaConfigLoader::saveConfigs\n" );
	for ( uint i = 0; i < yl.size(); i++ )
	{
		wxJSONValue cfgroot;
		cfgroot[ CFG_DESCRIPTION ] = yl[i]->description;
		cfgroot[ CFG_GROUP ] = yl[i]->group;
		cfgroot[ CFG_SOURCE ] = yl[i]->source;
		cfgroot[ CFG_EFFECT ] = yl[i]->effect;
		cfgroot[ CFG_RULE ] = yl[i]->rulestr;
		cfgroot[ CFG_HIGHER_VARGAS ] = yl[i]->allowHigherVargas;

		root[ CFG_CONFIGS ].Append( cfgroot );
	}

	wxString s;
	writer.Write( root, s );
	PrintLn( s );

	wxFileOutputStream stream( filename );
	writer.Write( root, stream );

	//printf( "ENDE YogaConfigLoader::saveConfigs filename %s\n", str2char( filename ));

}

#define XML_NODE_CONFIG wxT( "Config" )
#define XML_NODE_DESCRIPTION wxT( "Description" )
#define XML_NODE_GROUP wxT( "Group" )
#define XML_NODE_SOURCE wxT( "Source" )
#define XML_NODE_EFFECT wxT( "Effect" )
#define XML_NODE_RULESTR wxT( "Rule" )
#define XML_NODE_ALLOW_HIGHER_VARGAS wxT( "AllowHigherVargas" )
#define XMLNODE2BOOLEAN( k, v ) { v = ( k.Lower() == wxT( "true" )); }

/*****************************************************
**
**   YogaConfigLoader   ---   loadConfigXml
**
******************************************************/
bool YogaConfigLoader::loadConfigXml( wxString filename, std::vector<YogaConfig*> &ycl )
{
	//printf( "YogaConfigLoader::loadConfigXml\n" );
	if ( ! wxFileName::FileExists( filename ))
	{
		wxLogError( wxT( "File %s does not exist, using defaults" ), filename.c_str());
		return false;
	}
	if ( ! wxFileName::IsFileReadable( filename ))
	{
		wxLogError( wxT( "File %s is not readable, using defaults" ), filename.c_str());
		return false;
	}
	if ( wxFileName::GetSize( filename ) == 0 )
	{
		wxLogError( wxT( "File %s has NULL size, using defaults" ), filename.c_str());
		return false;
	}

	wxXmlDocument *doc = new wxXmlDocument( filename );
	if ( ! doc->IsOk() )
	{
		delete doc;
		wxLogError( wxT( "PARSE ERROR in file %s, using defaults" ), filename.c_str());
		return false;
	}

	wxXmlNode *cur = doc->GetRoot()->GetChildren();

	while ( cur != 0 )
	{
		YogaConfig *cfg = new YogaConfig;
		if ( cur->GetName() == XML_NODE_CONFIG)
		{
			wxXmlNode *child = cur->GetChildren();
			wxString key;
			while ( child )
			{
				key = child->GetName();
				if ( key == XML_NODE_GROUP ) cfg->group = child->GetNodeContent();
				else if ( key == XML_NODE_DESCRIPTION ) cfg->description = child->GetNodeContent();
				else if ( key == XML_NODE_EFFECT ) cfg->effect = child->GetNodeContent();
				else if ( key == XML_NODE_SOURCE ) cfg->source = child->GetNodeContent();
				else if ( key == XML_NODE_RULESTR ) cfg->rulestr = child->GetNodeContent();
				else if ( key == XML_NODE_ALLOW_HIGHER_VARGAS )
					XMLNODE2BOOLEAN( child->GetNodeContent(), cfg->allowHigherVargas )
				else reportWrongNodeName( child );
				child = child->GetNext();
			}

			ycl.push_back( cfg );
		}
		else reportWrongRootNode( cur );
		cur = cur->GetNext();
	}
	delete doc;

	return true;
}

/*****************************************************
**
**   YogaConfigLoader   ---   reportWrongRootNode
**
******************************************************/
void YogaConfigLoader::reportWrongRootNode( wxXmlNode *node )
{
	wxString name = node->GetName();
#ifdef _WX_V2_
	if ( name != wxT( "comment" )) wxLogError( wxT( "Wrong root node %s in file %s" ),
		name.c_str(), localResourcename.c_str());
#else
	if ( name != wxT( "comment" )) wxLogError( wxT( "Wrong root node %s at line %d in file %s" ),
		name.c_str(), node->GetLineNumber(), localResourcename.c_str());
#endif
}

/*****************************************************
**
**   YogaConfigLoader   ---   reportWrongNodeName
**
******************************************************/
void YogaConfigLoader::reportWrongNodeName( wxXmlNode *node )
{
	assert( node );
	wxString name = node->GetName();
#ifdef _WX_V2_
	if ( name != wxT( "comment" )) wxLogError( wxT( "Wrong node name %s in file %s" ),
		name.c_str(), localResourcename.c_str());
#else
	if ( name != wxT( "comment" )) wxLogError( wxT( "Wrong node name %s at line %d in file %s" ),
		name.c_str(), node->GetLineNumber(), localResourcename.c_str());
#endif
}


