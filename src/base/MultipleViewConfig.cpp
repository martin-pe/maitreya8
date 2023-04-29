/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/MultipleViewConfig.cpp
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

#include "MultipleViewConfig.h"

#include <wx/jsonval.h>
#include <wx/log.h>
#include <wx/string.h>
#include <math.h>

#include "Conf.h"
#include "maitreya.h"
#include "JSonTool.h"

extern Config *config;

/*****************************************************
**
**   MultipleViewConfig   ---   Constructor
**
******************************************************/
MultipleViewConfig::MultipleViewConfig( wxString name ) 
 : ViewNode( WniNone, 0 ),
 name( name )
{
	sizex = sizey = 500;
	viewnode = (ViewNode*)NULL;
	vedic = config->preferVedic;
}

/*****************************************************
**
**   MultipleViewConfig   ---   Destructor
**
******************************************************/
MultipleViewConfig::~MultipleViewConfig()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor MultipleViewConfig" << Endl;
#endif
	if ( viewnode ) delete viewnode;
}

/*****************************************************
**
**   MultipleViewConfig   ---   clone
**
******************************************************/
MultipleViewConfig *MultipleViewConfig::clone()
{
  MultipleViewConfig *m = new MultipleViewConfig( *this );
	if ( m->viewnode ) m->viewnode = this->viewnode->clone();
	return m;
}

IMPLEMENT_SINGLETON( MultipleViewConfigLoader )

/*****************************************************
**
**   MultipleViewConfigLoader   ---   Constructor
**
******************************************************/
MultipleViewConfigLoader::MultipleViewConfigLoader()
 : ConfigListLoader<MultipleViewConfig>( CFG_MULTIPLEVIEW, CrtCombinedDir )	
{
	viewtypes[ wxT( "planets" ) ] = VIEW_PLANETLIST;
	viewtypes[ wxT( "graphic" ) ] = VIEW_GRAPHIC;
	viewtypes[ wxT( "yoga" ) ] = VIEW_YOGA;
	viewtypes[ wxT( "dasa" ) ] = VIEW_DASA_TREE;
	viewtypes[ wxT( "text" ) ] = VIEW_TEXT;
	viewtypes[ wxT( "solar" ) ] = VIEW_SOLAR;
	viewtypes[ wxT( "sbc" ) ] = VIEW_SBC;
	viewtypes[ wxT( "gdasa" ) ] = VIEW_GRAPHICALDASA;
	viewtypes[ wxT( "uranian" ) ] = VIEW_URANIAN;
	viewtypes[ wxT( "transit" ) ] = VIEW_TRANSIT;
	viewtypes[ wxT( "grid" ) ] = VIEW_GRAPHICGRID;
	viewtypes[ wxT( "av" ) ] = VIEW_ASHTAKAVARGA;
	viewtypes[ wxT( "vimsopaka" ) ] = VIEW_VIMSOPAKABALA;
	viewtypes[ wxT( "shadbala" ) ] = VIEW_SHADBALA;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   loadSingleConfig
**
******************************************************/
void MultipleViewConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	ViewNode *node = (ViewNode*)NULL;
	MultipleViewConfig *cfg = new MultipleViewConfig;

	cfg->name = v[ CFG_NAME ].AsString();
	if ( ! v[ CFG_DESCRIPTION ].IsNull()) cfg->description = v[ CFG_DESCRIPTION ].AsString();
	if ( ! v[ CFG_VEDIC ].IsNull()) cfg->vedic = v[ CFG_VEDIC ].AsBool();

	if ( ! v[ CFG_NOTEBOOK ].IsNull()) node = parseNotebookNode( 0, v[ CFG_NOTEBOOK ] );
	if ( ! v[ CFG_WIDGET ].IsNull()) node = parseWidgetNode( 0, v[ CFG_WIDGET ] );
	if ( ! v[ CFG_SPLITTER ].IsNull()) node = parseSplitterNode( 0, v[ CFG_SPLITTER ] );

	cfg->viewnode = node;
	l.push_back( cfg );
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   getNamesAsArray
**
******************************************************/
wxArrayString MultipleViewConfigLoader::getNamesAsArray()
{
	wxArrayString a;
	for( uint i = 0; i < l.size(); i++ )
	{
		a.Add( l[ i ]->name );
	}
	return a;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   getViewId
**
******************************************************/
VIEW_ID MultipleViewConfigLoader::getViewId( wxString key )
{
	std::map<wxString, VIEW_ID>::iterator it = viewtypes.find( key );
	if ( it != viewtypes.end()) return it->second;

	wxLogError( wxT( "Wrong view type %s in file %s, returning default" ), key.c_str(), globalResourcename.c_str());
	return VIEW_EMPTY;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseWidgetNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseWidgetNode( ViewNode *parent, wxJSONValue &v )
{
	WidgetNode *node = new WidgetNode( parent );
	if ( ! v[ CFG_VIEWTYPE ].IsNull()) node->viewId = getViewId( v[ CFG_VIEWTYPE ].AsString() );
	if ( ! v[ CFG_VARGA ].IsNull()) node->varga = v[ CFG_VARGA ].AsInt();
	if ( ! v[ CFG_TEXTMODE ].IsNull()) node->textmode = v[ CFG_TEXTMODE ].AsInt();
	if ( ! v[ CFG_SHOWTOOLBAR ].IsNull()) node->toolpanel = v[ CFG_SHOWTOOLBAR ].AsBool();

	if ( ! v[ CFG_ROWCOUNT ].IsNull()) node->rows = v[ CFG_ROWCOUNT ].AsInt();
	if ( ! v[ CFG_COLCOUNT ].IsNull()) node->cols = v[ CFG_COLCOUNT ].AsInt();

	if ( ! v[ CFG_VARGAS ].IsNull() && v[ CFG_VARGAS ].IsArray())
	{
		for ( int i = 0; i < v[ CFG_VARGAS ].Size(); i++ )
		{
			node->vargas.push_back( v[ CFG_VARGAS ][i].AsInt() );
		}
	}
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseSplitterNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseSplitterNode( ViewNode *parent, wxJSONValue &v )
{
	SplitterNode *node = new SplitterNode( parent );
	if ( ! v[ CFG_WINDOW1 ].IsNull()) node->viewnode1 = parseWindowNode( node, v[ CFG_WINDOW1 ] );
	if ( ! v[ CFG_WINDOW2 ].IsNull()) node->viewnode2 = parseWindowNode( node, v[ CFG_WINDOW2 ] );

	if ( ! v[ CFG_SPLITDIRECTION ].IsNull())
	{
		if ( ! v[ CFG_SPLITDIRECTION ].AsString().CmpNoCase( wxT( "vertical" ))) node->dir = 1;
		else node->dir = 0;
	}

	if ( ! v[ CFG_SASHPOS ].IsNull()) node->sashpos = v[ CFG_SASHPOS ].AsInt();
	if ( ! v[ CFG_SASHPOSPERCENT ].IsNull()) node->sashpos_rel = v[ CFG_SASHPOSPERCENT ].AsInt();

	if ( ! node->viewnode1 )
	{
		wxLogWarning( wxT( "MultipleViewConfigLoader::parseSplitterNode: ViewNode1 is NULL" ));
	}
	if ( ! node->viewnode1 )
	{
		wxLogWarning( wxT( "MultipleViewConfigLoader::parseSplitterNode: ViewNode2 is NULL" ));
	}
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parsePageNode
**
******************************************************/
PageNode *MultipleViewConfigLoader::parsePageNode( ViewNode *parent, wxJSONValue &v )
{
	PageNode *node = new PageNode( parent );

	if ( ! v[ CFG_NAME ].IsNull()) node->name = v[ CFG_NAME ].AsString();
	if ( ! v[ CFG_NOTEBOOK ].IsNull()) node->viewnode = parseNotebookNode( parent, v[ CFG_NOTEBOOK ] );
	else if ( ! v[ CFG_WIDGET ].IsNull()) node->viewnode = parseWidgetNode( parent, v[ CFG_WIDGET ] );
	else if ( ! v[ CFG_SPLITTER ].IsNull()) node->viewnode = parseSplitterNode( parent, v[ CFG_SPLITTER ] );

	if ( ! node->viewnode ) wxLogError( wxT( "MultipleViewConfigLoader::parsePageNode view node of page node is null" ));

	//PrintLn( v.Dump());
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseNotebookNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseNotebookNode( ViewNode *parent, wxJSONValue &v )
{
	NotebookNode *node = new NotebookNode( parent );

	wxJSONValue &pages = v[ CFG_PAGES ];
	if ( pages.IsNull()) wxLogError( wxT( "no page in notebook" ));
	else
	{
		const bool isarray = pages.IsArray();
		if ( isarray )
		{
			for ( int i = 0; i < pages.Size(); i++ )
			{
				node->pagenodes.push_back( parsePageNode( node, pages[i] ));
			}
		}
		else wxLogError( wxT( "pages in notebook is not an array" ));
	}
	return node;
}

/*****************************************************
**
**   MultipleViewConfigLoader   ---   parseWindowNode
**
******************************************************/
ViewNode *MultipleViewConfigLoader::parseWindowNode( ViewNode *parent, wxJSONValue &v )
{
	ViewNode *node = (ViewNode*)NULL;

	if ( ! v[ CFG_NOTEBOOK ].IsNull()) node = parseNotebookNode( parent, v[ CFG_NOTEBOOK ] );
	if ( ! v[ CFG_WIDGET ].IsNull()) node = parseWidgetNode( parent, v[ CFG_WIDGET ] );
	if ( ! v[ CFG_SPLITTER ].IsNull()) node = parseSplitterNode( parent, v[ CFG_SPLITTER ] );

	if ( ! node ) wxLogError( wxT( "MultipleViewConfigLoader::parseWindowNode view node is null" ));
	return node;
}


