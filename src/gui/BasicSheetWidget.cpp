/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicSheetWidget.cpp
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

#include "BasicSheetWidget.h"

#include "Conf.h"
#include "Exporter.h"
#include "guibase.h"
#include "Painter.h"
#include "PdfTools.h"
#include "SheetConfig.h"

#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/stopwatch.h>

extern Config *config;

IMPLEMENT_CLASS( BasicSheetWidget, BasicWidget )

/*****************************************************
**
**   BasicSheetWidget   ---   Constructor
**
******************************************************/
BasicSheetWidget::BasicSheetWidget( wxWindow *parent, ChartProperties *props,
	SheetConfig *scfg, WriterConfig *wcfg, ColorConfig *ccfg )
	: BasicWidget( parent, props )
{
	writercfg = wcfg ? wcfg: config->writer;

	colorcfg = ccfg ? ccfg : config->colors;
	assert( colorcfg );
	sheetcfg = (SheetConfig*)NULL;

	sheet = new Sheet( writercfg );

	setWidgetOptions( WO_EXPORT_PDF | WO_EXPORT_PLAINTEXT | WO_EXPORT_HTMLTEXT | WO_EXPORT_CSVTEXT );
}

/*****************************************************
**
**   BasicSheetWidget   ---   Destructor
**
******************************************************/
BasicSheetWidget::~BasicSheetWidget()
{
	delete sheet;
}

/*****************************************************
**
**   BasicSheetWidget   ---   setWriterConfig
**
******************************************************/
void BasicSheetWidget::setWriterConfig( WriterConfig *wcfg )
{
	writercfg = wcfg;
}

/*****************************************************
**
**   BasicSheetWidget   ---   setSheetConfig
**
******************************************************/
void BasicSheetWidget::setSheetConfig( SheetConfig *scfg )
{
	assert( scfg );
	sheetcfg = scfg;
	//sheetConfigOverride = true;
}

/*****************************************************
**
**   BasicSheetWidget   ---   getSheetConfig
**
******************************************************/
SheetConfig *BasicSheetWidget::getSheetConfig()
{
	SheetConfig *sc = ( sheetcfg ? sheetcfg : SheetConfigLoader::get()->getConfig( config->view->sheetStyle ));
	return sc;
}

/*****************************************************
**
**   BasicSheetWidget   ---   clearSheet
**
******************************************************/
void BasicSheetWidget::clearSheet()
{
	sheet->clear();
}

/*****************************************************
**
**   BasicSheetWidget   ---   exportAs
**
******************************************************/
void BasicSheetWidget::exportAs( const WidgetExportType &type )
{
	printf( "BasicSheetWidget::exportAs %d\n", type );

	switch( type  )
	{
		case WeText:
		case WeCsv:
		case WeHtml:
			doTextExport( type );
		break;
		case WePdf:
			doPdfExport();
		break;
		case WeImage:
			doImageExport();
		break;
		default:
			assert( false );
		break;
	}
	OnDataChanged();
}

/*****************************************************
**
**   BasicSheetWidget   ---   doTextExport
**
******************************************************/
void BasicSheetWidget::doTextExport( const WidgetExportType &type )
{
	wxString filename, filetypes, title;

	switch( type  )
	{
		case WeText:
			filename = wxT( "out.txt" );
			filetypes = wxT( "Text (*.txt)|*.txt|All files (*)|*.*" );
			title = _( "Export as Plain Text" );
		break;
		case WeCsv:
			filename = wxT( "out.csv" );
			filetypes = wxT( "Text (*.csv)|*.csv|All files (*)|*.*" );
			title = _( "Export as Csv Text" );
		break;
		case WeHtml:
			filename = wxT( "out.html" );
			filetypes = wxT( "Html (*.html)|*.html|All files (*)|*.*" );
			title = _( "Export as HTML Text" );
		break;
		default:
			assert( false );
		break;
	}
	int style = wxFD_SAVE;
	if ( config->view->exportAskOnOverwrite ) style |= wxFD_OVERWRITE_PROMPT;

	wxFileDialog exportFileDialog( this, title, config->viewprefs->defExportPath, filename, filetypes, style, wxDefaultPosition );
	if ( exportFileDialog.ShowModal() == wxID_OK )
	{
		filename = exportFileDialog.GetPath();
		config->viewprefs->defExportPath = exportFileDialog.GetDirectory();
		Exporter *exporter = ExporterFactory().getExporter( type );
		wxString s =  exporter->exportSheet( sheet );
		//Cout( s );
		wxFile file;
		file.Create( filename, true );
		file.Write( s );
		file.Close();
		doMessageBox( this, wxString::Format( _("Text exported to %s"), filename.c_str()));
	}
}

/*****************************************************
**
**   BasicSheetWidget   ---   doPdfExport
**
******************************************************/
void BasicSheetWidget::doPdfExport()
{
	Sheet *pdfsheet = sheet->cloneClean();

	PdfTool tool( (PdfDocumentConfig*)NULL );
	tool.doSheetExport( pdfsheet );
	delete pdfsheet;
}

