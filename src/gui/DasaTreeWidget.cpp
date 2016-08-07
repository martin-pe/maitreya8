/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/DasaTreeWidget.cpp
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

#include "DasaTreeWidget.h"

#include "Conf.h"
#include "DasaTool.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "MenuProvider.h"

IMPLEMENT_CLASS( DasaTreeWidget, TreeWidget )

enum { PIC_DASA = 0, PIC_FOLDER, PIC_DASAROOT, PIC_DASATREE1, PIC_DASATREE2, PIC_DASATREE3,
	PIC_DASATREE4, PIC_DASATREE5, PIC_FOLDERE, PIC_DASAROOTE, PIC_DASATREE1E, PIC_DASATREE2E,
	PIC_DASATREE3E, PIC_DASATREE4E, PIC_DASATREE5E
};

DEFINE_EVENT_TYPE( DASATREE_CHANGED )

extern Config *config;

/*****************************************************
**
**   DasaTreeWidget   ---   Constructor
**
******************************************************/
DasaTreeWidget::DasaTreeWidget( wxWindow *parent, wxWindowID winid, Horoscope *horoscope, ChartProperties *props )
 : TreeWidget( parent, winid, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxSUNKEN_BORDER ),
	 horoscope( horoscope ),
	 props( props )
{
	uint i;
	DasaTreeItemClientData *item;
	wxTreeItemId id, root, vimsotid;

	currentjd = 0;

	DasaExpertFactory factory;
	for( int d = 0; d < MAX_DASAEXPERTS; d++ ) experts[d] = factory.getDasaExpert( (DasaId)d, horoscope );

	SetIndent( 30 );
	SetImageList( ImageProvider::get()->getDasaImageList());

	rootid = AddRoot( _( "Dasa Tree View" ), PIC_DASA );
	SetItemImage( rootid, PIC_DASA, wxTreeItemIcon_Expanded );
	baseid = AppendItem( rootid, _( "Standard" ), PIC_FOLDER );
	SetItemImage( baseid, PIC_FOLDERE, wxTreeItemIcon_Expanded );
	conditionalid = AppendItem( rootid, _( "Conditional" ), PIC_FOLDER );
	SetItemImage( conditionalid, PIC_FOLDERE, wxTreeItemIcon_Expanded );
	specialid = AppendItem( rootid, _( "Special" ), PIC_FOLDER );
	SetItemImage( specialid, PIC_FOLDERE, wxTreeItemIcon_Expanded );

	for ( i = 0; i < MAX_DASAEXPERTS; i++ )
	{
		if ( i == D_VIMSOTTARI || i == D_YOGINI || i == D_KALACHAKRA ) root = baseid;
		else if ( i >= D_ASHTOTTARI && i <= D_SHATTRIMSATSAMA ) root = conditionalid;
		else root = specialid;

		item = new DasaTreeItemClientData( experts[i], (Dasa*)NULL );
		id = AppendItem( root, getItemLabel( item ),  PIC_DASAROOT, PIC_DASAROOT, item );
		if ( i == D_VIMSOTTARI ) vimsotid = id;
		SetItemImage( id, PIC_DASAROOTE, wxTreeItemIcon_Expanded );
		SetItemHasChildren( id );
		SetItemBold( id, true );
	}

	Connect( wxEVT_COMMAND_TREE_ITEM_COLLAPSING, MyTreeEventHandler( DasaTreeWidget::collapse ));
	Connect( wxEVT_COMMAND_TREE_ITEM_EXPANDING, MyTreeEventHandler( DasaTreeWidget::expand ));
	Connect( wxEVT_COMMAND_TREE_SEL_CHANGING, MyTreeEventHandler( DasaTreeWidget::OnSelChanging ));
	Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( DasaTreeWidget::OnContextMenuEvent ));

	Expand( rootid );
	Expand( baseid );
	Expand( vimsotid );
}

/*****************************************************
**
**   DasaTreeWidget   ---   Destructor
**
******************************************************/
DasaTreeWidget::~DasaTreeWidget()
{
	for( int d = 0; d < MAX_DASAEXPERTS; d++ ) delete experts[d];
}

/*****************************************************
**
**   DasaTreeWidget   ---   expand
**
******************************************************/
void DasaTreeWidget::expand( MyTreeEvent &event )
{
	printf( "expand\n" );
	uint i;
	wxString s;
	vector<Dasa*> divector;
	wxTreeItemId id, id2;
	Dasa *dasa, *dasa2;
	DasaTreeItemClientData *ditem;
	int level = 0;
	int pic = PIC_DASATREE1;
	int epic = PIC_DASATREE1E;

	id = event.GetItem();
	DasaTreeItemClientData *data = (DasaTreeItemClientData*)GetItemData( id );

	// Root item
	if ( ! data ) return;

	// Children already there
	if ( GetChildrenCount( id ) > 0 ) return;

	
	ASSERT_VALID_DASA_ID( data->getDasaId() );
	DasaExpert *e = experts[data->getDasaId()];
	if ( data->isRootItem())
	{
		divector = e->getFirstLevel();
	}
	else
	{
		dasa = data->getDasa();
		divector = e->getNextLevel( dasa );
		dasa2 = dasa->getParent();
		while ( dasa2 )
		{
			level++;
			dasa2 = dasa2->getParent();
		}
		switch ( level )
		{
		case 0:
			pic = PIC_DASATREE2;
			epic = PIC_DASATREE2E;
			break;
		case 1:
			pic = PIC_DASATREE3;
			epic = PIC_DASATREE3E;
			break;
		case 2:
			pic = PIC_DASATREE4;
			epic = PIC_DASATREE4E;
			break;
		default:
			pic = PIC_DASATREE5;
			epic = PIC_DASATREE5E;
			break;
		}
	}

	for ( i = 0; i < divector.size(); i++ )
	{
		ditem = new DasaTreeItemClientData( data->getExpert(), divector[i] );
		id2 = AppendItem( id, getItemLabel( ditem ), pic, pic, ditem );
		SetItemImage( id2, epic, wxTreeItemIcon_Expanded );
		SetItemHasChildren( id2 );
		SetItemHidden( id2, (divector[i]->getEndJD() < horoscope->getJD() ));
	}
}

/*****************************************************
**
**   DasaTreeWidget   ---   collapse
**
******************************************************/
void DasaTreeWidget::collapse( MyTreeEvent& )
{
	printf( "collapse\n" );
	//updateTextWidget( 0 );
}

/*****************************************************
**
**   DasaTreeWidget   ---   OnSelChanging
**
******************************************************/
void DasaTreeWidget::OnSelChanging( MyTreeEvent &event )
{
	printf( "OnSelChanging\n" );
	wxTreeItemId id = event.GetItem();
	DasaTreeItemClientData *item = (DasaTreeItemClientData*)GetItemData( id );

	if ( item && item->getDasa() )
	{
		currentjd = item->getDasa()->getStartJD();
	}
	else
	{
		currentjd = 0;
	}

	emitItemChanged( item );	
}

/*****************************************************
**
**   DasaTreeWidget   ---   getItemLabel
**
******************************************************/
wxString DasaTreeWidget::getItemLabel( DasaTreeItemClientData *item )
{
	DasaTool *tool = DasaTool::get();
	ASSERT_VALID_DASA_ID( item->getDasaId() );
	DasaExpert *expert = experts[item->getDasaId()];

	WriterConfig cfg = *config->writer;
	cfg.signSymbols = cfg.planetSymbols = false;

	if ( item->isRootItem())
	{
		return tool->getDasaName( item->getDasaId() );
	}
	else
	{
		DateTimeFormatter *formatter = DateTimeFormatter::get();
		Lang lang;
		wxString s;

		Location *loc = horoscope->getDataSet()->getLocation();
		double timediff = loc->getTimeZone() + loc->getDST();
		timediff /= 24;
		const double jd = horoscope->getJD() + timediff;

		s << formatter->formatDateString( 

			// dasaShowDateMode 0 = startdate 1 = end date
			config->vedicCalculation->dasaShowDateMode == 1 ? item->getDasa()->getEndJD() : Max( jd, item->getDasa()->getStartJD() ))
			<< SPACE << expert->getDasaDescription( item->getDasa(), TF_LONG, &cfg );
		return s;
	}
}

/*****************************************************
**
**   DasaTreeWidget   ---   OnDataChanged
**
******************************************************/
void DasaTreeWidget::OnDataChanged()
{
	updateDasa( rootid );
	emitItemChanged( 0 );	
}


/*****************************************************
**
**   DasaTreeWidget   ---   emitItemChanged
**
******************************************************/
void DasaTreeWidget::emitItemChanged( DasaTreeItemClientData *item )
{
	// TODO POST
	printf( "EMIT\n" );
	wxCommandEvent event( DASATREE_CHANGED, GetId() );
	event.SetClientObject( item );
	wxPostEvent( GetParent(), event );
}

/*****************************************************
**
**   DasaTreeWidget   ---   updateDasa
**
******************************************************/
void DasaTreeWidget::updateDasa( const wxTreeItemId &masterid )
{
	wxTreeItemId id;
	TreeItemClientData *data;
	DasaTreeItemClientData *item, *depitem;
	vector<Dasa*> vdasa;
	int i = 0;
	long cook;

	if ( GetChildrenCount( masterid ) == 0 ) return;
	data = GetItemData( masterid );
	if ( ! data )
	{
		id = GetFirstChild( masterid, cook );
		while ( id.IsOk() )
		{
			updateDasa( id );
			id = GetNextSibling( id );
		}
		return;
	}
	item = (DasaTreeItemClientData*)data;
	ASSERT_VALID_DASA_ID( item->getDasaId() );
	DasaExpert *expert = experts[item->getDasaId()];

	if ( item->isRootItem() )
		vdasa = expert->getFirstLevel();
	else
		vdasa = expert->getNextLevel( item->getDasa() );

	id = GetFirstChild( masterid, cook );
	while ( id.IsOk() )
	{
		depitem = (DasaTreeItemClientData*)GetItemData( id );
		assert( depitem );
		delete depitem->getDasa();
		depitem->setDasa( vdasa[i] );
		SetItemText( id, getItemLabel( depitem ));
		SetItemHidden( id, vdasa[i]->getEndJD() < horoscope->getJD() );
		updateDasa( id );

		assert( i < 100 && i >= 0 );
		i++;
		id = GetNextSibling( id );
	}
}

/*****************************************************
**
**   DasaTreeWidget   ---   OnContextMenuEvent
**
******************************************************/
void DasaTreeWidget::OnContextMenuEvent( wxMouseEvent& event)
{
	int x, y;
	x = event.m_x;
	y = event.m_y;
	wxWindow *window = (wxWindow*)event.GetEventObject();
	window->ClientToScreen( &x, &y );
	this->ScreenToClient( &x, &y );

	wxMenu *menu = ContextMenuProvider().getWidgetMenu( props, 0 );
	//wxMenu *menu = ContextMenuProvider().getWidgetMenu( this );

	PopupMenu( menu, x, y );
	delete menu;
	printf( "OnContextMenuEvent x %d y %d\n", x, y );
}


