/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ViewNode.h
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
#ifndef _VIEWNODE_H_
#define _VIEWNODE_H_

#include "maitreya.h"
#include <wx/object.h>
#include <wx/string.h>
#include <vector>

class BasicView;
class wxBookCtrlBase;
class wxWindow;

using namespace std;

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
	bool uranian;
	bool toolpanel;

	BasicView *basicview;

	vector<int> vargas;

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

	vector<PageNode*> pagenodes;
	wxString name;

	DECLARE_CLASS( NotebookNode )
};

#endif

