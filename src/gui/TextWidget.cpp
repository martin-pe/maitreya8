/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/TextWidget.cpp
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

#include "TextWidget.h"

#include <wx/html/htmlwin.h>
#include <wx/sizer.h>

#include "Exporter.h"
#include "FontProvider.h"
#include "Sheet.h"
#include "SheetConfig.h"

IMPLEMENT_CLASS( TextWidget, BasicSheetWidget )

/*************************************************//**
*
*  workaround event handler
*
******************************************************/
class TextWidgetEventHandler: public wxEvtHandler
{
public:
	TextWidgetEventHandler()
	{
		Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( TextWidgetEventHandler::OnContextMenuEvent ));
	}

protected:
	void OnContextMenuEvent( wxMouseEvent &event )
	{
		//printf( "TextWidgetEventHandler::OnContextMenuEvent\n" );
		event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
		event.Skip();
	}
};

/*****************************************************
**
**   TextWidget   ---   Constructor
**
******************************************************/
TextWidget::TextWidget( wxWindow *parent, ChartProperties *props, SheetConfig *scfg,
	WriterConfig *wcfg, ColorConfig *ccfg )
	: BasicSheetWidget( parent, props, scfg, wcfg, ccfg )
{
	init();
}

/*****************************************************
**
**   TextWidget   ---   Constructor
**
******************************************************/
TextWidget::TextWidget( wxWindow *parent, ChartProperties *props, const int &wo )
	: BasicSheetWidget( parent, props )
{
	init();
	setWidgetOptions( wo );
}

/*****************************************************
**
**   TextWidget   ---   init()
**
******************************************************/
void TextWidget::init()
{
	FontProvider *f = FontProvider::get();
	html = new wxHtmlWindow( this, -1 );

	wxFont *defaultFont = f->getFont( FONT_TEXT_DEFAULT );
	wxFont *textFont = f->getFont( FONT_PLAIN_DEFAULT );
	const int ss = defaultFont->GetPointSize();
	const int sizes[7] = { ss-4, ss-2, ss, ss+1, ss+2, ss+3, ss+5};
	html->SetFonts( defaultFont->GetFaceName(), textFont->GetFaceName(), sizes );

	wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
	sizer_main->Add( html, 0, wxALL|wxEXPAND, 3);
	sizer_main->AddGrowableRow(0);
	sizer_main->AddGrowableCol(0);
	SetSizer(sizer_main);
	sizer_main->Fit(this);
	Layout();

	html->PushEventHandler( new TextWidgetEventHandler());
}

/*****************************************************
**
**   TextWidget   ---   OnDataChanged
**
******************************************************/
void TextWidget::OnDataChanged()
{
	Exporter *exporter = ExporterFactory().getExporter( WeHtml, getSheetConfig(), writercfg );
	wxString s = exporter->exportSheet( sheet );
	html->SetPage( s );
	//printf( "Finished TextWidget::OnDataChanged %s\n", str2char( s ));
}

/*****************************************************
**
**   TextWidget   ---   doPaint
**
******************************************************/
void TextWidget::doPaint( const wxRect &rect, const bool eraseBackground )
{
	//printf( "TextWidget::doPaint\n" );
	html->RefreshRect( rect, eraseBackground );
	//html->Refresh();
}

