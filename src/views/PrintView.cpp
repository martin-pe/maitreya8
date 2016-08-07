/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/PrintView.cpp
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

#include <wx/choice.h>

#include "SheetView.h"
#include "ChildWindow.h"
#include "Conf.h"
#include "maitreya.h"
#include "Document.h"
#include "PdfTools.h"
#include "PrintoutConfig.h"
#include "PrintoutSheetCreator.h"
#include "SheetConfig.h"
#include "TextHelper.h"
#include "ToolPanel.h"

extern Config *config;

/*************************************************//**
*
* \brief view for various kinds of text output including toolbar controls
*
******************************************************/
class PrintView : public SheetView
{
	DECLARE_CLASS( PrintView )
public:
	PrintView( wxWindow *parent, ChildWindow *frame )
		: SheetView( parent, frame, VIEW_PRINT, true )
	{
		props = frame->getProps();
		SheetConfigLoader *sloader = SheetConfigLoader::get();

		mode = config->print->defaultPrintout;
		sheetStyle = config->print->sheetStyle;
		props->setWesternSkin( config->print->wGraphicSkin );
		props->setVedicSkin( config->print->vGraphicSkin );

		PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
		PrintoutConfig *cfg = loader->getConfig( mode );
		props->setVedic( cfg->vedic );

		//twidget->setWidgetOptions( WO_BUNDLE_SELECTABLETEXTVIEW );
		twidget->setCenterAll();
		twidget->setSheetConfig( sloader->getConfig( sheetStyle ));

		Connect( CMD_LAUNCH_VIEWER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PrintView::OnLaunchPdfViewer ));

		initToolItems();
		OnToolbarCommand();
	}

	~PrintView()
	{
		config->print->defaultPrintout = mode;
		config->print->vGraphicSkin = props->getVedicSkin();
		config->print->wGraphicSkin = props->getWesternSkin();
		config->print->sheetStyle = sheetStyle;
	}


	virtual wxString getWindowLabel( const bool /* shortname */ = false )
	{
		return _( "Print Preview" );
	} 

protected:

	/**************************************************************
	***
	**   PrintView   ---   initToolItems
	***
	***************************************************************/
	void initToolItems()
	{
		if ( toolbar )
		{
			wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_PRINTOUTTYPE );
			if ( choice_mode ) choice_mode->SetSelection( config->print->defaultPrintout );

			wxChoice *choice_western = (wxChoice*)toolbar->FindControl( TBS_WESTERNCHARTSTYLE );
			if ( choice_western ) choice_western->SetSelection( props->getWesternSkin());

			wxChoice *choice_vedic = (wxChoice*)toolbar->FindControl( TBS_VEDICCHARTSTYLE );
			if ( choice_vedic ) choice_vedic->SetSelection( props->getVedicSkin() );

			wxChoice *choice_sheetstyle = (wxChoice*)toolbar->FindControl( TBS_SHEETSTYLE );
			if ( choice_sheetstyle ) choice_sheetstyle->SetSelection( sheetStyle );
		}
	}

	/*****************************************************
	**
	**   PrintView   ---   OnToolbarCommand
	**
	******************************************************/
	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_PRINTOUTTYPE );
			if ( choice_mode )
			{
				mode = choice_mode->GetSelection();
				PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
				PrintoutConfig *cfg = loader->getConfig( mode );
				props->setVedic( cfg->vedic );
				printf( "VEDIC %d\n", cfg->vedic );
			}

			wxChoice *choice_western = (wxChoice*)toolbar->FindControl( TBS_WESTERNCHARTSTYLE );
			if ( choice_western )
			{
				props->setWesternSkin( choice_western->GetSelection());
			}

			wxChoice *choice_vedic = (wxChoice*)toolbar->FindControl( TBS_VEDICCHARTSTYLE );
			if ( choice_vedic )
			{
				props->setVedicSkin( choice_vedic->GetSelection());
			}

			wxChoice *choice_sheetstyle = (wxChoice*)toolbar->FindControl( TBS_SHEETSTYLE );
			if ( choice_sheetstyle )
			{
				sheetStyle = choice_sheetstyle->GetSelection();
				SheetConfigLoader *sloader = SheetConfigLoader::get();
				twidget->setSheetConfig( sloader->getConfig( sheetStyle ));
			}
		}
		updateView = true;
	}

	/**************************************************************
	***
	**   PrintView   ---   write
	***
	***************************************************************/
	virtual void write()
	{
		twidget->clearSheet();
		PrintoutSheetCreator c( doc, props, twidget->getSheet() );
		c.write( mode );
	}

	/*****************************************************
	**
	**   PrintView   ---   OnLaunchPdfViewer
	**
	******************************************************/
	void OnLaunchPdfViewer( wxCommandEvent& )
	{
		printf( "launch Viewer MODE %d\n", mode );
		PdfDocumentConfig pcfg;
		pcfg.defaultPrintout = mode;
		PdfTool( &pcfg ).printTestpage( doc, props );
	}

	int mode;
	int sheetStyle;
};

IMPLEMENT_CLASS( PrintView, SheetView )

/**************************************************************
***
**   ViewFactory   ---   createPrintView
***
***************************************************************/
BasicView *ViewFactory::createPrintView( wxWindow *parent, ChildWindow *frame )
{
	return new PrintView( parent, frame );
}

