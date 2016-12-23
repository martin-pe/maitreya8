/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/MultipleChildWindow.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "MultipleChildWindow.h"

#include <wx/string.h>
#include <wx/choicebk.h>
#include <wx/log.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/splitter.h>

#ifdef wxUSE_LISTBOOK
#include <wx/listbook.h>
#endif

#ifdef wxUSE_TOOLBOOK
#include <wx/toolbook.h>
#endif

#ifdef wxUSE_TREEBOOK
#include <wx/treebook.h>
#endif

#include "BasicView.h"
#include "Conf.h"
#include "Document.h"
#include "guibase.h"
#include "IdConverter.h"
#include "ImageProvider.h"
#include "MultipleViewConfig.h"
#include "SplitterWidget.h"

extern Config *config;

IMPLEMENT_CLASS( MultipleChildWindow, ChildWindow )

/*****************************************************
**
**   MultipleChildWindow   ---   Constructor
**
******************************************************/
MultipleChildWindow::MultipleChildWindow( wxFrame *parent, Document *doc, const BitmapId &bid, const wxSize &size, const bool ismain )
		: ChildWindow( parent, doc, bid, size, ismain )
{
	setTitle();
	viewconfig = (MultipleViewConfig*)NULL;

	Connect( wxEVT_SIZE, wxSizeEventHandler( MultipleChildWindow::OnSize ));
}

/*****************************************************
**
**   MultipleChildWindow   ---   Destructor
**
******************************************************/
MultipleChildWindow::~MultipleChildWindow()
{
	printf( "Destructor MultipleChildWindow size is %d\n", (int)viewlist.size());
	saveViewStatus( viewconfig->viewnode );
	delete viewconfig;
	viewlist.clear();
}

/*****************************************************
**
**   MultipleChildWindow   ---   saveViewStatus
**
******************************************************/
void MultipleChildWindow::saveViewStatus( ViewNode *node )
{
	wxString s;
	if ( ! node )
	{
		wxLogError( wxT( "MultipleChildWindow::saveViewStatus, view is NULL" ));
		return;
	}
	printf( "SAVE view status %s\n", str2char( node->configEntryName ));
	//s = node->getConfigEntryName();
	//PrintLn( s );

	switch( node->id )
	{
		case WniWidget:
		{
			printf( "Widget\n" );
		}
		break;
		case WniSplitter:
		{
			printf( "Splitter\n" );
			//assert ( node->GetClassInfo()->IsKindOf( CLASSINFO( SplitterNode )));
			//SplitterNode *sn = (SplitterNode*)node;

			SplitterNode *sn = wxDynamicCast( node, SplitterNode );
			assert( sn );
			saveViewStatus( sn->viewnode1 );
			saveViewStatus( sn->viewnode2 );
		}
		break;
		case WniPageNode:
		{
			printf( "Page\n" );
			//assert ( node->GetClassInfo()->IsKindOf( CLASSINFO( PageNode )));
			//PageNode *pn = (PageNode*)node;
			PageNode *pn = wxDynamicCast( node, PageNode );
			assert( pn );
			saveViewStatus( pn->viewnode );
		}
		break;
		case WniNotebook:
		{
			printf( "Notebook\n" );
			//assert(  node->GetClassInfo()->IsKindOf( CLASSINFO( NotebookNode )));
			//NotebookNode *nn = (NotebookNode*)node;

			NotebookNode *nn = wxDynamicCast( node, NotebookNode );
			assert( nn );
			for( uint i = 0; i < nn->pagenodes.size(); i++ )
			{
				saveViewStatus( nn->pagenodes[i] );
			}
		}
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   MultipleChildWindow   ---   setTitle
**
******************************************************/
void MultipleChildWindow::setTitle()
{
	assert( doc );
	SetTitle( doc->getHName());
}

/*****************************************************
**
**   MultipleChildWindow   ---   OnDataChanged
**
******************************************************/
void MultipleChildWindow::OnDataChanged()
{
	printf( "MultipleChildWindow::OnDataChanged\n" );
	for ( uint i = 0; i < viewlist.size(); i++ ) viewlist[i]->OnDataChanged();
	Refresh();
}

/*****************************************************
**
**   MultipleChildWindow   ---   initViews
**
******************************************************/
void MultipleChildWindow::initViews( MultipleViewConfig *cfg )
{
	assert( cfg );
	viewconfig = cfg->clone();
	props->setVedic( viewconfig->vedic );

	// prevent empty views
	if ( ! viewconfig->viewnode )
	{
		printf( "WARN MultipleChildWindow::initViews is NULL, appending empty view\n" );
		viewconfig->viewnode = new WidgetNode( VIEW_EMPTY );	
	}
	viewconfig->viewnode->dump();

	insertNode( this, viewconfig->viewnode );
}

/**************************************************************
***
**   MultipleChildWindow   ---   insertNode
***
***************************************************************/
wxWindow *MultipleChildWindow::insertNode( wxWindow *parent, ViewNode *node )
{
	ViewFactory f;
	IdConverter *idc = IdConverter::get();

	printf( "MultipleChildWindow::insertNode start, %d views parent is a %s\n", (int)viewlist.size(), str2char( wxString( parent->GetClassInfo()->GetClassName())));

	//if ( ! node ) return 0;
	assert( node );
	if ( ! node->parent )
	{
		printf( "ROOT\n" );
		node->configEntryName = viewconfig->name;
	}
	else
	{
		printf( "NON ROOT raw name %s parent configentryname %s\n", str2char( node->configEntryName ), str2char( node->parent->configEntryName ));
		node->configEntryName << node->parent->configEntryName;
	}
	node->configEntryName << wxT( "." );

	switch( node->id )
	{
		case WniWidget:
		{
			node->configEntryName << wxT( "widget" );

			//assert( node->GetClassInfo()->IsKindOf( CLASSINFO( WidgetNode )));
			//WidgetNode* wn = (WidgetNode*)node;

			WidgetNode *wn = wxDynamicCast( node, WidgetNode );
			assert( wn );
			//BasicView *view = f.createView( parent, this, wn );

			assert( ! wn->basicview );
			wn->basicview = f.createView( parent, this, wn );
			//wn->parent = node;
			assert( wn->basicview );
			wn->guified = true;

			wn->basicview->doLayout();
			viewlist.push_back( wn->basicview );
			printf( "Fertig widget configEntryName %s\n", str2char( node->configEntryName ));
			return wn->basicview;
		}
		break;
		case WniSplitter:
		{
			node->configEntryName << wxT( "splitter" );

			//assert ( node->GetClassInfo()->IsKindOf( CLASSINFO( SplitterNode )));
			//SplitterNode *sn = (SplitterNode*)node;

			SplitterNode *sn = wxDynamicCast( node, SplitterNode );
			assert( sn );
			SplitterWidget *splitter = new SplitterWidget( parent, -1, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE|wxSP_NOBORDER );
			splitter->SetSashGravity( 0.5 );
			node->widget = splitter;

			if ( ! sn->viewnode1 ) sn->viewnode1 = new WidgetNode( VIEW_EMPTY );	
			sn->viewnode1->configEntryName << node->configEntryName << wxT( "splitter1" );
			sn->window1 = insertNode( splitter, sn->viewnode1 );

			if ( ! sn->viewnode2 ) sn->viewnode2 = new WidgetNode( VIEW_EMPTY );	
			sn->viewnode2->configEntryName << node->configEntryName << wxT( "splittern2" );
			sn->window2 = insertNode( splitter, sn->viewnode2 );
			sn->guified = true;

			if ( sn->dir ) splitter->SplitVertically( sn->window1, sn->window2 );
			else splitter->SplitHorizontally( sn->window1, sn->window2 );
			splitter->Layout();
			printf( "Fertig splitter configEntryName %s\n", str2char( node->configEntryName ));
			return splitter;
		}
		break;
		case WniPageNode:
		{
			//assert ( node->GetClassInfo()->IsKindOf( CLASSINFO( PageNode )));
			//PageNode *pn = (PageNode*)node;

			PageNode *pn = wxDynamicCast( node, PageNode );
			assert( pn );
			pn->configEntryName << wxT( "page" );

			if ( ! pn->viewnode ) pn->viewnode = new WidgetNode( VIEW_EMPTY );	
			pn->viewnode->configEntryName << node->configEntryName << wxT( "pagenode" );
			pn->widget = insertNode( parent, pn->viewnode );
			pn->guified = true;

			printf( "Fertig pagenode configEntryName %s\n", str2char( node->configEntryName ));
			return pn->widget;
		}
		break;
		case WniNotebook:
		{
			ImageProvider *ip = ImageProvider::get();
			//assert(  node->GetClassInfo()->IsKindOf( CLASSINFO( NotebookNode )));
			//NotebookNode *nn = (NotebookNode*)node;

			NotebookNode *nn = wxDynamicCast( node, NotebookNode );
			assert( nn );

			wxBookCtrlBase *bookCtrl = createNotebook( parent, nn );
			bookCtrl->AssignImageList( ImageProvider::get()->createViewImageList() );
			nn->widget = bookCtrl;

			for( uint i = 0; i < nn->pagenodes.size(); i++ )
			{
				assert( nn->pagenodes[i] );

				nn->pagenodes[i]->configEntryName << node->configEntryName << wxString::Format( wxT( "notebook%02d" ), (int)i );
				wxWindow *window = insertNode( bookCtrl, nn->pagenodes[i] );

				BitmapId bbid = BM_ERROR;
				BasicView *view = searchNextBasicView( nn->pagenodes[i] );
				if ( view )
				{
					bbid = idc->viewId2BitmapId( view->getViewId() );
				}
				else wxLogWarning( wxT( "MultipleChildWindow::insertNode: could not find view ID for page node %d " ), i );
				bookCtrl->AddPage( window, nn->pagenodes[i]->name, i == 0, ip->getViewImageListIndex4BitmapId( bbid ));

			}
			nn->guified = true;
			printf( "Fertig notebook configEntryName %s\n", str2char( node->configEntryName ));
			return bookCtrl;
		}
		break;
		default:
			assert( false );
		break;

	}
	assert( false );
	return 0;
}

/*****************************************************
**
**   MultipleChildWindow   ---   createNotebook
**
******************************************************/
wxBookCtrlBase *MultipleChildWindow::createNotebook( wxWindow *parent, NotebookNode * /*node*/ )
{
	wxBookCtrlBase *ctrl = (wxBookCtrlBase*)NULL;

	int orient;

	switch ( config->multipleView->notebookOrientation )
	{
	case 1:
		orient = wxNB_BOTTOM;
		break;
	case 2:
		orient = wxNB_LEFT;
		break;
	case 3:
		orient = wxNB_RIGHT;
		break;
	default:
		orient = wxNB_TOP;
		break;
	}
	long style = orient;
	switch ( config->multipleView->notebookStyle )
	{
	case 0:
		ctrl = new wxNotebook( parent, -1, wxDefaultPosition, wxDefaultSize, style );
		break;
	case 1:
		ctrl = new wxListbook( parent, -1, wxDefaultPosition, wxDefaultSize, style );
		break;
#if wxUSE_CHOICEBOOK
	case 2:
		ctrl = new wxChoicebook( parent, -1, wxDefaultPosition, wxDefaultSize, style );
		//ctrl = new ChoicebookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
#endif
#if wxUSE_TOOLBOOK
	case 3:
		ctrl = new wxToolbook( parent, -1, wxDefaultPosition, wxDefaultSize, style );
		//ctrl = new ToolbookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
#endif
#if wxUSE_TREEBOOK
	case 4:
		ctrl = new wxTreebook( parent, -1, wxDefaultPosition, wxDefaultSize, style );
		//ctrl = new TreebookNodeContainer( parent, mchild, mnode, pos, size, orient );
		break;
#endif
	default:
		break;
	}

	if ( ! ctrl ) ctrl = new wxNotebook( parent, -1, wxDefaultPosition, wxDefaultSize, style );
	assert( ctrl );
	ctrl->SetImageList( ImageProvider::get()->createViewImageList());

	return ctrl;
}

/*****************************************************
*
**   MultipleChildWindow   ---   isViewVisible
**
******************************************************/
bool MultipleChildWindow::isViewVisible( BasicView * /*view*/ )
{
	bool visible = false;

	// TODO geht über die Parents, solgange bis parent == 0 oder Notebook, bei dem eine anderen Seite aktiv ist
	
	return visible;
}

/*****************************************************
**
**   MultipleChildWindow   ---   searchNextBasicView
**
******************************************************/
BasicView *MultipleChildWindow::searchNextBasicView( ViewNode *node )
{
	BasicView *view = (BasicView*)NULL;

	if ( node )
	{
		switch( node->id )
		{
			case WniWidget:
			{
				WidgetNode *wn = (WidgetNode*)node;
				if ( wn->basicview ) return wn->basicview;
			}
			break;
			case WniSplitter:
			{
				SplitterNode *sn = (SplitterNode*)node;
				if ( sn->viewnode1 ) return searchNextBasicView( sn->viewnode1 );
				else if ( sn->viewnode2 ) return searchNextBasicView( sn->viewnode2 );
			}
			break;
			case WniPageNode:
			{
				PageNode *pn = (PageNode*)node;
				if ( pn->viewnode ) return searchNextBasicView( pn->viewnode );
				printf( "ERROR: page node has no viewnode\n" );
			}
			break;
			case WniNotebook:
			{
				NotebookNode *nn = (NotebookNode*)node;
				for( uint i = 0; i < nn->pagenodes.size(); i++ )
				{
					if ( nn->pagenodes[i] ) return searchNextBasicView( nn->pagenodes[i] );
				}
			}
			break;
			default:
			break;
		}
	}
	return view;
}

/*****************************************************
**
**   MultipleChildWindow   ---   postCreateNode
**
******************************************************/
void MultipleChildWindow::postCreateNode( ViewNode *node, wxWindow* /*parent*/ )
{
	static int count = 1;
	printf( "START post create %d\n", count++ );

	assert( node );
	switch( node->id )
	{
		case WniWidget:
		{
			printf( "Post create widget\n" );

			//assert( node->GetClassInfo()->IsKindOf( CLASSINFO( WidgetNode )));
			//WidgetNode* wn = (WidgetNode*)node;

			WidgetNode *wn = wxDynamicCast( node, WidgetNode );
			assert( wn );
			if ( wn->basicview )
			{
				printf( "Start widget post create and layout\n" );
				wn->basicview->postCreate();
			}
			else
			{
				printf( "ERROR: basicview is NULL\n" );
				//assert( false );
			}
		}
		break;
		case WniSplitter:
		{
			//assert ( node->GetClassInfo()->IsKindOf( CLASSINFO( SplitterNode )));
			//SplitterNode *sn = (SplitterNode*)node;

			SplitterNode *sn = wxDynamicCast( node, SplitterNode );
			assert( sn );
			printf( "Post create splitter sash is %d and relative pos %d\n", sn->sashpos, sn->sashpos_rel );

			//SplitterWidget *splitter = (SplitterWidget*)node->widget;
			//assert( splitter );
			//assert ( splitter->GetClassInfo()->IsKindOf( CLASSINFO( SplitterWidget )));

			wxWindow *ww = node->widget;
			//SplitterWidget *splitter = wxDynamicCast( node->widget, SplitterWidget );
			//SplitterWidget *splitter = wxDynamicCast( ww, SplitterWidget );
			wxSplitterWindow *splitter = wxDynamicCast( ww, wxSplitterWindow );
			assert( splitter );

			const wxSize size = splitter->GetSize();
			int sash = 200;
			if ( sn->sashpos_rel > 0 )
			{ 
				sash = ( sn->dir ? size.GetX() : size.GetY() ) * sn->sashpos_rel / 100;
			} 
			else if ( sn->sashpos > 0 && sn->sashpos < 100 )
			{
				sash = sn->sashpos;
			}
			else
			{
				sash = ( sn->dir ? size.GetX() : size.GetY() ) / 2;    // emulate 50%
			}
			splitter->SetSashPosition( sash );

			if ( sn->viewnode1 ) postCreateNode( sn->viewnode1 );
			if ( sn->viewnode2 ) postCreateNode( sn->viewnode2 );

		}
		break;
		case WniPageNode:
		{
			printf( "Post create page\n" );

			//assert ( node->GetClassInfo()->IsKindOf( CLASSINFO( PageNode )));
			//PageNode *pn = (PageNode*)node;

			PageNode *pn = wxDynamicCast( node, PageNode );
			assert( pn );
			if ( pn->viewnode ) postCreateNode( pn->viewnode );

			// TODO bestimme commandTarget
			pn->commandTarget = searchNextBasicView( pn->viewnode );
		}
		break;
		case WniNotebook:
		{
			printf( "Post create notebook\n" );
			//assert(  node->GetClassInfo()->IsKindOf( CLASSINFO( NotebookNode )));
			//NotebookNode *nn = (NotebookNode*)node;

			NotebookNode *nn = wxDynamicCast( node, NotebookNode );
			assert( nn );
			for( uint i = 0; i < nn->pagenodes.size(); i++ )
			{
				postCreateNode( nn->pagenodes[i] );
			}
			assert ( nn->widget );
			wxBookCtrlBase *bookCtrl = (wxBookCtrlBase*)nn->widget;

#ifdef _WX_V2_
			// SendSizeEvent exists only for wxFrame on wx2.8, so emululate it
			wxSizeEvent event( bookCtrl->GetSize(), bookCtrl->GetId() );
			event.SetEventObject( bookCtrl );
			wxPostEvent( bookCtrl->GetEventHandler(), event );
#else
			bookCtrl->SendSizeEvent();
#endif

		}
		break;
		default:
			assert( false );
		break;

	}
}

/*****************************************************
**
**   MultipleChildWindow   ---   postCreate
**
******************************************************/
void MultipleChildWindow::postCreate()
{
	printf( "MultipleChildWindow::postCreate\n" );

	assert( viewconfig->viewnode );
	viewconfig->viewnode->dump( 0 );

	postCreateNode( viewconfig->viewnode, this );

	// set focus, otherwise key handlers will not work
	if ( viewlist.size() > 0 ) 
	{
		viewlist[0]->SetFocus();
	}
}

/*****************************************************
**
**   MultipleChildWindow   ---   OnSize
**
******************************************************/
void MultipleChildWindow::OnSize( wxSizeEvent &event )
{
	//printf( "MultipleChildWindow ->>OnSize %d -- %d\n", event.GetSize().x, event.GetSize().y  );
	IdConverter::get()->setConfigSizeForViewId( VIEW_MULTIPLE, event.GetSize(), props->isVedic() );
	event.Skip();
}

/**************************************************************
***
**   ChildWindowFactory   ---   createMainWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createMainWindow( wxFrame *parent, Document *doc, const bool &useMultipleView, const int viewId )
{
	return useMultipleView ? createMultipleMainWindow( parent, doc, viewId ) : createSingleMainWindow( parent, doc );
}

/**************************************************************
***
**   ChildWindowFactory   ---   createMultipleMainWindow
***
***************************************************************/
ChildWindow *ChildWindowFactory::createMultipleMainWindow( wxFrame *parent, Document *doc, int id )
{
	MultipleViewConfigLoader *cfgloader = MultipleViewConfigLoader::get();
	MultipleChildWindow *child;

	IdConverter *idc = IdConverter::get();
	const wxSize size = idc->getSizeForViewId( VIEW_MULTIPLE, config->preferVedic );
	const BitmapId bmpId = idc->viewId2BitmapId( VIEW_MULTIPLE );

	if ( id < 0 || id >= (int)cfgloader->getSize()) id = config->multipleView->defaultView;

	MultipleViewConfig *viewconfig = MultipleViewConfigLoader::get()->getConfig( id );
	child = new MultipleChildWindow( parent, doc, bmpId, size, true );
	doc->setMainWindow( child );
	child->initViews( viewconfig );

	return child;
}


