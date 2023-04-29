/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ViewNode.h
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
#ifndef _VIEWNODE_H_
#define _VIEWNODE_H_

#include "maitreya.h"
#include <wx/object.h>
#include <wx/string.h>
#include <vector>

class BasicView;
class wxBookCtrlBase;
class wxWindow;

enum ViewNodeId { WniNone = -1, WniWidget, WniSplitter, WniPageNode, WniNotebook };

/*************************************************//**
*
* \brief abstract class representing a node for MultipleChildWindow
*
******************************************************/
struct ViewNode : public wxObject
{
	ViewNode( const ViewNodeId &id, ViewNode *parent );
	virtual ~ViewNode();
	virtual ViewNode *clone() = 0;

	virtual void dump( const int level = 0 ) = 0;

	const ViewNodeId id;
	wxString configEntryName;
	bool guified;

	ViewNode *parent;
	wxWindow *widget;

	DECLARE_CLASS( ViewNode )
};

/*************************************************//**
*
* \brief ViewNode as widget
*
******************************************************/
struct WidgetNode : public ViewNode
{
	WidgetNode( ViewNode *p );
	WidgetNode( const int &command );

	virtual ~WidgetNode();
	virtual ViewNode *clone();

	virtual void dump( const int level = 0 );

	VIEW_ID viewId;
	int subtype;
	int varga;
	int textmode;
	int rows;
	int cols;
	bool vedic;
	bool toolpanel;

	BasicView *basicview;

	std::vector<int> vargas;

	DECLARE_CLASS( ViewNode )
};

/*************************************************//**
*
* \brief node as splitter, containing 2 other nodes
*
******************************************************/
struct SplitterNode : public ViewNode
{
	SplitterNode( ViewNode *parent )
	 : ViewNode( WniSplitter, parent )
	{
		viewnode1 = viewnode2 = 0;
		dir = 0;
		sashpos = 200;
		sashpos_rel = 0;
		window1 = (wxWindow*)NULL;
		window2 = (wxWindow*)NULL;
	}
	virtual ~SplitterNode();
	virtual ViewNode *clone();

	virtual void dump( const int level = 0 );

	ViewNode *viewnode1;
	ViewNode *viewnode2;

	wxWindow *window1;
	wxWindow *window2;

	int dir;
	int sashpos;
	int sashpos_rel;

	DECLARE_CLASS( SplitterNode )
};

/*************************************************//**
*
* \brief node as page of a NotebookNode
*
******************************************************/
struct PageNode : public ViewNode
{
	PageNode( ViewNode *parent );
	virtual ~PageNode();
	virtual ViewNode *clone();

	virtual void dump( const int level = 0 );

	ViewNode *viewnode;
	BasicView *commandTarget;
	wxString name;

	DECLARE_CLASS( PageNode )
};

/*************************************************//**
*
* \brief node as notebook containing several pages
*
******************************************************/
struct NotebookNode : public ViewNode
{
	NotebookNode( ViewNode *parent )
	  : ViewNode( WniNotebook, parent )
	{
	}
	virtual ~NotebookNode();
	virtual ViewNode *clone();

	virtual void dump( const int level = 0 );

	std::vector<PageNode*> pagenodes;
	wxString name;

	DECLARE_CLASS( NotebookNode )
};

#endif

