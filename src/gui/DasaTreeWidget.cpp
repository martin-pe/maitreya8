/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/DasaTreeWidget.cpp
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

#include "DasaTreeWidget.h"

#include <wx/sizer.h>

#include "Conf.h"
#include "DasaTool.h"
#include "ImageProvider.h"
#include "Lang.h"
#include "MenuProvider.h"
#include "TreeWidget.h"

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
DasaTreeWidget::DasaTreeWidget( wxWindow *parent, ChartProperties *props, wxWindowID winid, Horoscope *horoscope )
	: BasicWidget( parent, props, winid ),
	 horoscope( horoscope )
{
	uint i;
	DasaTreeItemClientData *item;
	wxTreeItemId id, root, vimsotid;

	currentjd = 0;

	DasaExpertFactory factory;
	for( int d = 0; d < MAX_DASAEXPERTS; d++ ) experts[d] = factory.getDasaExpert( (DasaId)d, horoscope );

	twidget = new TreeWidget( this, winid, wxDefaultPosition, wxDefaultSize, wxTR_HAS_BUTTONS | wxSUNKEN_BORDER );
	twidget->SetIndent( 30 );
	twidget->SetImageList( ImageProvider::get()->getDasaImageList());

	wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
	sizer_main->Add( twidget, 0, wxALL|wxEXPAND, 3);
	sizer_main->AddGrowableRow(0);
	sizer_main->AddGrowableCol(0);
	SetSizer(sizer_main);
	sizer_main->Fit(this);
	Layout();

	rootid = twidget->AddRoot( _( "Dasa Tree View" ), PIC_DASA );
	twidget->SetItemImage( rootid, PIC_DASA, wxTreeItemIcon_Expanded );
	baseid = twidget->AppendItem( rootid, _( "Standard" ), PIC_FOLDER );
	twidget->SetItemImage( baseid, PIC_FOLDERE, wxTreeItemIcon_Expanded );
	conditionalid = twidget->AppendItem( rootid, _( "Conditional" ), PIC_FOLDER );
	twidget->SetItemImage( conditionalid, PIC_FOLDERE, wxTreeItemIcon_Expanded );
	specialid = twidget->AppendItem( rootid, _( "Special" ), PIC_FOLDER );
	twidget->SetItemImage( specialid, PIC_FOLDERE, wxTreeItemIcon_Expanded );

	for ( i = 0; i < MAX_DASAEXPERTS; i++ )
	{
		if ( i == D_VIMSOTTARI || i == D_YOGINI || i == D_KALACHAKRA ) root = baseid;
		else if ( i >= D_ASHTOTTARI && i <= D_SHATTRIMSATSAMA ) root = conditionalid;
		else root = specialid;

		item = new DasaTreeItemClientData( experts[i], (Dasa*)NULL );
		id = twidget->AppendItem( root, getItemLabel( item ),  PIC_DASAROOT, PIC_DASAROOT, item );
		if ( i == D_VIMSOTTARI ) vimsotid = id;
		twidget->SetItemImage( id, PIC_DASAROOTE, wxTreeItemIcon_Expanded );
		twidget->SetItemHasChildren( id );
		twidget->SetItemBold( id, true );
	}

	Connect( wxEVT_COMMAND_TREE_ITEM_COLLAPSING, MyTreeEventHandler( DasaTreeWidget::collapse ));
	Connect( wxEVT_COMMAND_TREE_ITEM_EXPANDING, MyTreeEventHandler( DasaTreeWidget::expand ));
	Connect( wxEVT_COMMAND_TREE_SEL_CHANGING, MyTreeEventHandler( DasaTreeWidget::OnSelChanging ));
	//Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( DasaTreeWidget::OnMouseWheelEvent ));

	twidget->Expand( rootid );
	twidget->Expand( baseid );
	twidget->Expand( vimsotid );
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
**   DasaTreeWidget   ---   HandleMouseWheelEvent
**
******************************************************/
void DasaTreeWidget::HandleMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "WHEEL\n" );
	event.Skip();
}

/*****************************************************
**
**   DasaTreeWidget   ---   expand
**
******************************************************/
void DasaTreeWidget::expand( MyTreeEvent &event )
{
	//printf( "expand\n" );
	uint i;
	wxString s;
	std::vector<Dasa*> divector;
	wxTreeItemId id, id2;
	Dasa *dasa, *dasa2;
	DasaTreeItemClientData *ditem;
	int level = 0;
	int pic = PIC_DASATREE1;
	int epic = PIC_DASATREE1E;

	id = event.GetItem();
	DasaTreeItemClientData *data = (DasaTreeItemClientData*)twidget->GetItemData( id );

	// Root item
	if ( ! data ) return;

	// Children already there
	if ( twidget->GetChildrenCount( id ) > 0 ) return;

	
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
		id2 = twidget->AppendItem( id, getItemLabel( ditem ), pic, pic, ditem );
		twidget->SetItemImage( id2, epic, wxTreeItemIcon_Expanded );
		twidget->SetItemHasChildren( id2 );
		twidget->SetItemHidden( id2, (divector[i]->getEndJD() < horoscope->getJD() ));
	}
}

/*****************************************************
**
**   DasaTreeWidget   ---   collapse
**
******************************************************/
void DasaTreeWidget::collapse( MyTreeEvent& )
{
	//printf( "collapse\n" );
	//updateTextWidget( 0 );
}

/*****************************************************
**
**   DasaTreeWidget   ---   OnSelChanging
**
******************************************************/
void DasaTreeWidget::OnSelChanging( MyTreeEvent &event )
{
	//printf( "OnSelChanging\n" );
	wxTreeItemId id = event.GetItem();
	DasaTreeItemClientData *item = (DasaTreeItemClientData*)twidget->GetItemData( id );

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
	//printf( "EMIT\n" );
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
	std::vector<Dasa*> vdasa;
	int i = 0;
	long cook;

	if ( twidget->GetChildrenCount( masterid ) == 0 ) return;
	data = twidget->GetItemData( masterid );
	if ( ! data )
	{
		id = twidget->GetFirstChild( masterid, cook );
		while ( id.IsOk() )
		{
			updateDasa( id );
			id = twidget->GetNextSibling( id );
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

	id = twidget->GetFirstChild( masterid, cook );
	while ( id.IsOk() )
	{
		depitem = (DasaTreeItemClientData*)twidget->GetItemData( id );
		assert( depitem );
		delete depitem->getDasa();
		depitem->setDasa( vdasa[i] );
		twidget->SetItemText( id, getItemLabel( depitem ));
		twidget->SetItemHidden( id, vdasa[i]->getEndJD() < horoscope->getJD() );
		updateDasa( id );

		assert( i < 100 && i >= 0 );
		i++;
		id = twidget->GetNextSibling( id );
	}
}

/*****************************************************
**
**   DasaTreeWidget   ---   doPaint
**
******************************************************/
void DasaTreeWidget::doPaint( const wxRect &rect, const bool eraseBackground )
{
	//printf( "DasaTreeWidget::doPaint\n" );
	twidget->RefreshRect( rect, eraseBackground );
}


