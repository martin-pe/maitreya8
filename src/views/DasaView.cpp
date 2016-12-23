/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/DasaView.cpp
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

#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
//#include <wx/treebase.h>  // for wxTreeItemId

#include "astrobase.h"
#include "BasicView.h"
#include "Conf.h"
#include "Dasa.h"
#include "DasaTool.h"
#include "DasaTreeWidget.h"
#include "Document.h"
#include "FontProvider.h"
#include "ImageProvider.h"
#include "guibase.h"
#include "Lang.h"
#include "maitreya.h"
#include "SplitterWidget.h"
#include "TextWidget.h"
#include "ToolPanel.h"

extern Config *config;

enum { DASAWINDOW_CTRL = wxID_HIGHEST + 1, DW_SPLITTER, DW_TREE, DASAWINDOW_PANEL };

enum { PIC_DASA = 0, PIC_FOLDER, PIC_DASAROOT, PIC_DASATREE1, PIC_DASATREE2, PIC_DASATREE3,
	PIC_DASATREE4, PIC_DASATREE5, PIC_FOLDERE, PIC_DASAROOTE, PIC_DASATREE1E, PIC_DASATREE2E,
	PIC_DASATREE3E, PIC_DASATREE4E, PIC_DASATREE5E
};

using namespace std;

/*************************************************//**
* 
*  \brief bottom panel in Dasa tree view 
* 
******************************************************/
class DasaPanel : public wxPanel
{ 
public:
  
  DasaPanel(wxWindow* parent, const int id )
	 : wxPanel(parent, id )
	{
		FontProvider *fc = FontProvider::get();

		button_entry_chart = new wxBitmapButton(this, CMD_ENTRY_CHART, wxNullBitmap);
		button_entry_chart->SetBitmapLabel( ImageProvider::get()->getBitmap( BM_NEW ));
		button_entry_chart->SetMinSize(wxSize( 32, 32 ));
		button_entry_chart->SetToolTip(_("Create Entry Chart"));

		text_header = new wxStaticText(this, -1, _("dummy"));
		text_header->SetFont( *fc->getFont( FONT_TEXT_HEADER ));
		text_lord = new wxStaticText(this, -1, _("dummy"));
		text_startdate = new wxStaticText(this, -1, _("dummy"));
		text_enddate = new wxStaticText(this, -1, _("dummy"));
		text_duration = new wxStaticText(this, -1, _("dummy"));

		wxBoxSizer* sizer_main = new wxBoxSizer( wxHORIZONTAL );
		wxBoxSizer* sizer_text = new wxBoxSizer( wxVERTICAL );

		sizer_text->Add(text_header, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		wxFlexGridSizer* sizer = new wxFlexGridSizer(4, 2, 3, 3);
		sizer->Add( new  wxStaticText( this, wxID_ANY, _("Lord")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_lord, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		sizer->Add( new  wxStaticText( this, wxID_ANY, _("From")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_startdate, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		sizer->Add( new  wxStaticText( this, wxID_ANY, _("To")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_enddate, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);

		sizer->Add( new  wxStaticText( this, wxID_ANY, _("Duration")), 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
		sizer->Add(text_duration, 1, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 3);
		sizer->AddGrowableCol(1);

		sizer_text->Add(sizer, 0, wxALL|wxEXPAND, 3);
		sizer_main->Add(sizer_text, 1, wxALL, 3);
		sizer_main->Add(button_entry_chart, 0, wxALL|wxALIGN_RIGHT, 3);

		SetSizer(sizer_main);
		sizer_main->Fit(this);
		enableButton( true );
	}

  void clear()
	{
		text_lord->SetLabel( wxEmptyString );
		text_startdate->SetLabel( wxEmptyString );
		text_enddate->SetLabel( wxEmptyString);
		text_duration->SetLabel( wxEmptyString );;
		enableButton( false );
	}

	void setHeader( wxString s ) { text_header->SetLabel( s ); }
	void setLord( wxString s ) { text_lord->SetLabel( s ); }
	void setStartDate( wxString s ) { text_startdate->SetLabel( s ); }
	void setEndDate( wxString s ) { text_enddate->SetLabel( s ); }
	void setDuration( wxString s ) { text_duration->SetLabel( s ); }
	void enableButton( const bool b ) { button_entry_chart->Enable( b ); }
  
private:

  wxBitmapButton* button_entry_chart;
  wxStaticText* text_header;
  wxStaticText* text_lord;
  wxStaticText* text_startdate;
  wxStaticText* text_enddate;
  wxStaticText* text_duration;

};

/*************************************************//**
*
* \brief base tree view for Dasas
*
******************************************************/
class DasaBaseTreeView : public BasicView
{
	DECLARE_CLASS( DasaBaseTreeView )

public:

	/*****************************************************
	**
	**   DasaBaseTreeView   ---   Constructor
	**
	******************************************************/
	DasaBaseTreeView( wxWindow *parent, ChildWindow *frame, const VIEW_ID &viewID, const bool &showToolPanel )
			: BasicView( parent, frame, viewID, showToolPanel )
	{
		props->setFixedVedic();
		Connect( DW_TREE, DASATREE_CHANGED, wxCommandEventHandler( DasaBaseTreeView::OnDasaTreeChanged ));
		Connect( CMD_ENTRY_CHART, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaBaseTreeView::OnEntryChart ));
	}

	/*****************************************************
	**
	**   DasaBaseTreeView   ---   OnDataChanged
	**
	******************************************************/
	void OnDataChanged()
	{
		assert( treewidget );
		treewidget->OnDataChanged();
	}

protected:

	void OnDasaTreeChanged( wxCommandEvent &event )
	{
		printf( "OnDasaTreeChanged\n" );
		bool b = (bool)event.GetClientObject();
		if ( toolbar )
		{
			wxButton *entry_chart_button = (wxButton*)toolbar->FindControl( CMD_ENTRY_CHART );
			if ( entry_chart_button ) entry_chart_button->Enable( b );
			else printf( "Error: button not found\n" );
		}
		else
		{
			printf( "Error: no toolpanel\n" );
		}
		treeChanged( (DasaTreeItemClientData*)event.GetClientObject());
	}

	/*****************************************************
	**
	**   DasaBaseTreeView   ---   OnEntryChart
	**
	******************************************************/
	void OnEntryChart( wxCommandEvent& )
	{
		printf( "DasaBaseTreeView::OnEntryChart\n" );
		assert( treewidget );
		const double currentjd = treewidget->getCurrentJD();

		if ( currentjd == 0 )
		{
			doMessageBox( this, wxT( "Error: can't determine date for entry chart." ));
		}
		else
		{
			Document *d = new Document;
			d->setLocation( *doc->getLocation() );
			d->setDate( currentjd );
			d->setHName( wxString::Format( wxT( "%s: %s" ), doc->getHName().c_str(), _( "Entry Chart" )));

			wxCommandEvent event( CREATE_ENTRY_CHART, GetParent()->GetId() );
			event.SetEventObject( (wxObject*)d );
			wxPostEvent( GetParent(), event );
		}
	}

	virtual void treeChanged( DasaTreeItemClientData* ) = 0;

	DasaTreeWidget *treewidget;

};

IMPLEMENT_CLASS( DasaBaseTreeView, BasicView )


/*************************************************//**
*
* \brief tree view for Dasas
*
******************************************************/
class DasaTreeView : public DasaBaseTreeView
{
	DECLARE_CLASS( DasaTreeView )

public:

	/*****************************************************
	**
	**   DasaTreeView   ---   Constructor
	**
	******************************************************/
	DasaTreeView( wxWindow *parent, ChildWindow *frame )
			: DasaBaseTreeView( parent, frame, VIEW_DASA_TREE, false )
	{
		treewidget = new DasaTreeWidget( this, props, DW_TREE, doc );
		widget = treewidget;
		showToolPanel( true );
		Connect( CMD_ENTRY_CHART, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DasaTreeView::OnEntryChart ));
	}

	virtual wxString getWindowLabel( const bool ) { return _( "Dasa/Tree" ); }

	/*****************************************************
	**
	**   DasaTreeView   ---   showToolPanel
	**
	******************************************************/
	void showToolPanel( const bool b )
	{
		if ( b )
		{
			DasaPanel *dasapanel = new DasaPanel( this, DASAWINDOW_PANEL );
			detailpanel = dasapanel;
		}
		else
		{
			if ( detailpanel ) delete detailpanel;
			detailpanel = (wxPanel*)NULL;
		}
		updateDetailPanel( 0 );
	}


	virtual void treeChanged( DasaTreeItemClientData *item )
	{
		updateDetailPanel( item );
		printf( "DasaTreeView::treeChanged\n" );
	}

	/*****************************************************
	**
	**   DasaTreeView   ---   updateDetailPanel
	**
	******************************************************/
	void updateDetailPanel( DasaTreeItemClientData *item )
	{
		if ( ! detailpanel ) return;

		const static wxString k_dasaname[5] = { _( "Mahadasa" ), _( "Antardasa" ),
			_( "Pratyantardasa" ), _( "Sooksmantardasa" ), _( "Pranadasa" ) };

		Lang lang;
		wxString dasalords, s;
		DasaTool *tool = DasaTool::get();

		DasaPanel *panel = (DasaPanel*)detailpanel;

		if ( ! item )
		{
			panel->clear();
			panel->setHeader( _( "Dasa Details" ));
			panel->enableButton( false );
			return;
		}
		DasaExpert *expert = item->getExpert();
		if ( item->isRootItem())
		{
			panel->clear();
			panel->setHeader( wxString::Format( _( "%s Dasa" ), tool->getDasaName( expert->getDasaId() ).c_str()));
			panel->enableButton( false );
		}
		else
		{
			// Dasa/Antardasa type, prepend
			Dasa *dasa = item->getDasa();
			int level = 0;
			while ( dasa )
			{
				if ( level > 0 ) dasalords.Prepend( wxT( "/" ));

				if ( expert->isRasiDasaExpert() )
					dasalords.Prepend( lang.getSignName( (Rasi)dasa->getDasaLord()));
				else
					dasalords.Prepend( lang.getObjectName( (ObjectId)dasa->getDasaLord(), TF_LONG, true ));
				level++;
				dasa = dasa->getParent();
			}
			panel->setLord( dasalords );

			// Dasa Header, comes after type because level is used
			if (	level <= 5  ) s = k_dasaname[level-1];
			else s = wxString::Format( _( "Antardasa Level %d" ), level );
			panel->setHeader( wxString::Format( _( "%s of %s Dasa" ), s.c_str(), tool->getDasaName( expert->getDasaId() ).c_str()));

			// reset dasa
			dasa = item->getDasa();

			// Start and end of Dasa
			TzUtil tzu;
			const bool isLocaltime = true;
			TzFormattedDate startdate = tzu.getDateFormatted( dasa->getStartJD(), isLocaltime );
			panel->setStartDate( startdate.fullDateTimeTzFormatted );

			TzFormattedDate enddate = tzu.getDateFormatted( dasa->getEndJD(), isLocaltime );
			panel->setEndDate( enddate.fullDateTimeTzFormatted );

			s = getDasaDuration( dasa->getEndJD() - Max( doc->getJD(), dasa->getStartJD()));
			panel->setDuration( s );

			//currentjd = dasa->getStartJD();
			panel->enableButton( true );
		}
	}

protected:

};

IMPLEMENT_CLASS( DasaTreeView, DasaBaseTreeView )

/*************************************************//**
*
* \brief composite tree view for Dasas
*
******************************************************/
class DasaCompositeView : public DasaBaseTreeView
{
	DECLARE_CLASS( DasaCompositeView )

public:

	/*****************************************************
	**
	**   DasaCompositeView   ---   Constructor
	**
	******************************************************/
	DasaCompositeView( wxWindow *parent, ChildWindow *frame )
			: DasaBaseTreeView( parent, frame, VIEW_DASA_COMPOSITE, true )
	{
		splitter = new wxSplitterWindow( this, DW_SPLITTER );
		treewidget = new DasaTreeWidget( splitter, props, DW_TREE, doc );
		theText = new TextWidget( splitter, props );
		splitter->SplitVertically( treewidget, theText );
	
		widget = splitter;
		initToolItems();

		Connect( CMD_TOGGLE_SPLIT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( DasaCompositeView::OnSplitUnsplit ));
		Connect( DW_SPLITTER, wxEVT_COMMAND_SPLITTER_SASH_POS_CHANGED, wxSplitterEventHandler( DasaCompositeView::OnSplitterSashPosChanged ));
		Connect( DW_SPLITTER, wxEVT_COMMAND_SPLITTER_DOUBLECLICKED, wxSplitterEventHandler( DasaCompositeView::OnSplitterDoubleClick ));
	}

	virtual wxString getWindowLabel( const bool ) { return _( "Dasa/Composite" ); }

	/*****************************************************
	**
	**   DasaCompositeView   ---   postCreate
	**
	******************************************************/
	virtual void postCreate()
	{
		splitter->SetSashPosition( config->viewprefs->sashDasaTree );
		//toolbar->ToggleTool( CMD_TOGGLE_SPLIT, config->viewprefs->doSplitDasaTree );
	}

protected:

	/**************************************************************
	***
	**   DasaCompositeView   ---   initToolItems
	***
	***************************************************************/
	void initToolItems()
	{
		if ( toolbar )
		{
			wxChoice *choice_sourcefilter = (wxChoice*)toolbar->FindControl( TBS_YOGASOURCEFILTER );
			if ( choice_sourcefilter )
			{
				choice_sourcefilter->Clear();
				choice_sourcefilter->Append( _( "All Sources" ));
				set<wxString> &sources = DasaTool::get()->getDasaSources();
				for( set<wxString>::iterator iter = sources.begin(); iter != sources.end(); iter++ )
				{
					choice_sourcefilter->Append( *iter );
				}

				choice_sourcefilter->SetSelection( 0 );
			}
			toolbar->EnableTool( CMD_ENTRY_CHART, false );

			toolbar->ToggleTool( CMD_TOGGLE_SPLIT, true );
		}
	}

	/*****************************************************
	**
	**   DasaCompositeView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxChoice *choice_sourcefilter = (wxChoice*)toolbar->FindControl( TBS_YOGASOURCEFILTER );
			if ( choice_sourcefilter )
			{
				const int sel = choice_sourcefilter->GetSelection();
				if ( sel != 0 ) sourcefilter = choice_sourcefilter->GetString( sel );
				else sourcefilter.Clear();
			}
		}
		updateText();
	}

	/*****************************************************
	**
	**   DasaCompositeView   ---   treeChanged
	**
	******************************************************/
	virtual void treeChanged( DasaTreeItemClientData *item )
	{
		printf( "DasaCompositeView::treeChanged\n" );
		currentItem = item;

		toolbar->EnableTool( CMD_ENTRY_CHART, item && item->getDasa() );
		updateText();
	}

	/*****************************************************
	**
	**   DasaCompositeView   ---   updateText
	**
	******************************************************/
	void updateText()
	{
		printf( "DasaCompositeView::updateText\n" );
		DasaTool *tool = DasaTool::get();
		theText->clearSheet();
		if ( currentItem )
		{
			tool->writeDasaDocumentation( theText->getSheet(), currentItem->getExpert(), currentItem->getDasa(), sourcefilter );
		}
		theText->OnDataChanged();
	}

	/*****************************************************
	**
	**   DasaCompositeView   ---   split
	**
	******************************************************/
	void split( const bool &b )
	{
		printf( "Split param checked %di splitter isplit %d\n", b, splitter->IsSplit() );
		if ( b )
		{
			splitter->SplitVertically( treewidget, theText );
			splitter->SetSashPosition( config->viewprefs->sashDasaTree );
		}
		else
		{
			config->viewprefs->sashDasaTree = splitter->GetSashPosition();
			splitter->Unsplit();
			printf( "Unsplit finished\n" );
		}
		printf( "Sash2 %d\n", config->viewprefs->sashDasaTree );
	}

	/*****************************************************
	**
	**   DasaCompositeView   ---   OnSplitUnsplit
	**
	******************************************************/
	void OnSplitUnsplit( wxCommandEvent &event )
	{
		printf( "Split/Unsplit checked %d\n", event.IsChecked() );
		config->viewprefs->doSplitDasaTree = event.IsChecked();
		split( event.IsChecked());
	}

	/*****************************************************
	**
	**   DasaCompositeView   ---   OnSplitterSashPosChanged
	**
	******************************************************/
	void OnSplitterSashPosChanged(wxSplitterEvent& )
	{
		config->viewprefs->sashDasaTree = splitter->GetSashPosition();
		if ( splitter->GetWindow1() ) splitter->GetWindow1()->Refresh();
		if ( splitter->GetWindow2() ) splitter->GetWindow2()->Refresh();
	}

	/*****************************************************
	**
	**   DasaCompositeView   ---   OnSplitterDoubleClick
	**
	******************************************************/
	void OnSplitterDoubleClick(wxSplitterEvent& )
	{
		printf( "OnSplitterDoubleClick\n" );
		if ( toolbar ) toolbar->ToggleTool( CMD_TOGGLE_SPLIT, false );
	}

protected:
	TextWidget *theText;
	wxSplitterWindow *splitter;
	wxString sourcefilter;
	DasaTreeItemClientData *currentItem;

};

IMPLEMENT_CLASS( DasaCompositeView, DasaBaseTreeView )

/**************************************************************
***
**   ViewFactory   ---   createDasaTreeView
***
***************************************************************/
BasicView *ViewFactory::createDasaTreeView( wxWindow *parent, ChildWindow *frame )
{
	return new DasaTreeView( parent, frame );
}


/**************************************************************
***
**   ViewFactory   ---   createDasaCompositeView
***
***************************************************************/
BasicView *ViewFactory::createDasaCompositeView( wxWindow *parent, ChildWindow *frame )
{
	return new DasaCompositeView( parent, frame );
}



