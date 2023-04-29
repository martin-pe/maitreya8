/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ViewNode.cpp
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

#include "ViewNode.h"

#include <wx/string.h>
#include <math.h>

#include "Commands.h"
#include "Conf.h"
#include "IdConverter.h"
#include "maitreya.h"

extern Config *config;

IMPLEMENT_CLASS( ViewNode, wxObject )
IMPLEMENT_CLASS( WidgetNode, ViewNode )
IMPLEMENT_CLASS( SplitterNode, ViewNode )
IMPLEMENT_CLASS( PageNode, ViewNode )
IMPLEMENT_CLASS( NotebookNode, ViewNode )

void dumpLine( wxString s, const int &level )
{
	for( int i = 0; i < level; i++ ) printf( "\t" );
	PrintLn( s );
}

//#define DEB_VIEWMODE_DESTRUCTORS

/*****************************************************
**
**   ViewNode   ---   Constructor
**
******************************************************/
ViewNode::ViewNode( const ViewNodeId &id, ViewNode *parent )
	: id ( id ),
	parent( parent )
{
	guified = false;
	widget = (wxWindow*)NULL;
}

/*****************************************************
**
**   ViewNode   ---   Destructor
**
******************************************************/
ViewNode::~ViewNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor ViewNode" << Endl;
#endif
}

/*****************************************************
**
**   WidgetNode   ---   Constructor
**
******************************************************/
WidgetNode::WidgetNode( ViewNode *parent )
	: ViewNode( WniWidget, parent )
{
	varga = rows = cols = 0;
	viewId = VIEW_EMPTY;
	textmode = config->viewprefs->textWindowPage;
	toolpanel = true;
	vedic = config->preferVedic;

	basicview = (BasicView*)NULL;
}

/*****************************************************
**
**   WidgetNode   ---   Constructor
**
******************************************************/
WidgetNode::WidgetNode( const int &command )
	: ViewNode( WniWidget, (ViewNode*)NULL )
{
	subtype = varga = rows = cols = 0;
	textmode = config->viewprefs->textWindowPage;
	toolpanel = true;
	vedic = config->preferVedic;
	basicview = (BasicView*)NULL;

	viewId = IdConverter::get()->commandId2ViewId( command );

	// most cases were already handled in IdConverter. continue only with special cases.
	switch ( command )
	{
	case CMD_NEW_WCHART:
		vedic = false;
		varga = 0;
		break;
	default:
		if ( command >= CMD_NEW_RASI && command <= CMD_NEW_RASI+30 )
		{
			vedic = true;
			varga = command-CMD_NEW_RASI;
		}
		break;
	}
}

/*****************************************************
**
**   WidgetNode   ---   Destructor
**
******************************************************/
WidgetNode::~WidgetNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor WidgetNode" << Endl;
#endif
}

/*****************************************************
**
**   WidgetNode   ---   clone
**
******************************************************/
ViewNode *WidgetNode::clone()
{
	WidgetNode *w = new WidgetNode( *this );
	for( uint i = 0; i < vargas.size(); i++ )
	{
		w->vargas.push_back( vargas[i] );
	}
	return w;
}

/*****************************************************
**
**   WidgetNode   ---   dump
**
******************************************************/
void WidgetNode::dump( const int level )
{
	dumpLine( wxT( "Start Widget" ), level );
	dumpLine( wxT( "{" ), level );
	dumpLine( wxString::Format( wxT( "viewId %d" ), viewId ), level +1 );
	dumpLine( wxString::Format( wxT( "Text mode %d" ), textmode ), level + 1 );
	dumpLine( wxString::Format( wxT( "Varga %d" ), varga ), level +1 );
	dumpLine( wxString::Format( wxT( "rows %d" ), rows ), level +1 );
	dumpLine( wxString::Format( wxT( "cols %d" ), cols ), level +1 );
	dumpLine( wxT( "--- Start VargaS " ), level +1 );
	for ( uint i = 0; i < vargas.size(); i++ )
	{
		dumpLine( wxString::Format( wxT( " - Varga: %d" ), vargas[i] ), level +2 );
	}
	dumpLine( wxT( "--- End VargaS " ), level + 1 );
	dumpLine( wxString::Format( wxT( "toolpanel %d" ), toolpanel ), level + 1 );
	dumpLine( wxT( "}" ), level );
	dumpLine( wxT( "End Widget" ), level );
}

/*****************************************************
**
**   SplitterNode   ---   Destructor
**
******************************************************/
SplitterNode::~SplitterNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor SplitterNode" << Endl;
#endif
	if ( viewnode1 ) delete viewnode1;
	if ( viewnode2 ) delete viewnode2;
}

/*****************************************************
**
**   SplitterNode   ---   clone
**
******************************************************/
ViewNode *SplitterNode::clone()
{
	SplitterNode *s = new SplitterNode( *this );
	if ( s->viewnode1 ) s->viewnode1  = viewnode1->clone();
	if ( s->viewnode2 ) s->viewnode2 = viewnode2->clone();
	return s;
}

/*****************************************************
**
**   SplitterNode   ---   dump
**
******************************************************/
void SplitterNode::dump( const int level )
{
	dumpLine( wxString::Format( wxT( "Start Splitter Direction: %d" ), dir ), level );
	dumpLine( wxT( "{" ), level );
	dumpLine( wxString::Format( wxT( "saspos %d" ), sashpos ), level + 1 );
	dumpLine( wxString::Format( wxT( "sasposrel %d" ), sashpos_rel ), level + 1 );
	if ( viewnode1 ) viewnode1->dump( level + 1 );
	else dumpLine( wxT( "ERROR ViewNode1 empty" ), level + 1 );
	if ( viewnode2 ) viewnode2->dump( level + 1 );
	dumpLine ( wxT( "ERROR ViewNode2 empty" ), level + 1 );
	dumpLine( wxT( "}" ), level );
	dumpLine( wxT( "End Splitter" ), level );
}

/*****************************************************
**
**   PageNode   ---   Constructor
**
******************************************************/
PageNode::PageNode( ViewNode *parent )
	: ViewNode( WniPageNode, parent )
{
	viewnode = (ViewNode*)NULL;
	name = wxT( "dummy" );
	commandTarget = (BasicView*)NULL;
}

/*****************************************************
**
**   PageNode   ---   Destructor
**
******************************************************/
PageNode::~PageNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor PageNode window is " << widget->getName() << Endl;
#endif
	if ( viewnode ) delete viewnode;
}

/*****************************************************
**
**   PageNode   ---   clone
**
******************************************************/
ViewNode *PageNode::clone()
{
	PageNode *pn = new PageNode( *this );
	if ( pn->viewnode ) pn->viewnode = viewnode->clone();
	return pn;
}

/*****************************************************
**
**   PageNode   ---   dump
**
******************************************************/
void PageNode::dump( const int level )
{
	dumpLine( wxString::Format( wxT( "Start Page %s" ), name.c_str() ), level );
	dumpLine( wxT( "{" ), level );
	//dumpLine( wxString::Format( wxT( " viewnode %ld" ), (long)viewnode ), level +1 );
	if ( viewnode ) viewnode->dump( level + 1 );
	else dumpLine( wxT( "ERROR Page Empty" ), level + 1 );
	dumpLine( wxT( "}" ), level );
	dumpLine( wxT( "End Page" ), level );
}

/*****************************************************
**
**   NotebookNode   ---   Destructor
**
******************************************************/
NotebookNode::~NotebookNode()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor NotebookNode window is " << widget->getName() << Endl;
#endif
	uint i;
	for ( i = 0; i < pagenodes.size(); i++ )
	{
		delete pagenodes[i];
	}
	pagenodes.clear();
}

/*****************************************************
**
**   NotebookNode   ---   clone
**
******************************************************/
ViewNode *NotebookNode::clone()
{
	NotebookNode *n = new NotebookNode( *this );
	for( uint i = 0; i < pagenodes.size(); i++ )
	{
		if ( pagenodes[i] ) n->pagenodes[i] = (PageNode*)pagenodes[i]->clone();
	}
	return n;
}

/*****************************************************
**
**   NotebookNode   ---   dump
**
******************************************************/
void NotebookNode::dump( const int level )
{
	uint i;
	dumpLine( wxT( "Start NOTEBOOK "  ), level );
	dumpLine( wxT( "{" ), level );
	for ( i = 0; i < pagenodes.size(); i++ )
	{
		dumpLine( wxString::Format( wxT( "Page No %d" ), i+1 ), level + 1 );
		pagenodes[i]->dump( level + 1 );
	}
	dumpLine( wxT( "}" ), level );
	dumpLine( wxT( "End NOTEBOOK "  ), level );
}

