/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfTools.cpp
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

#include "PdfTools.h"

#include "ChartProperties.h"
#include "Conf.h"
#include "FileConfig.h"
#include "guibase.h"
#include "Horoscope.h"
#include "PdfBase.h"
#include "PdfPainter.h"
#include "PrintoutConfig.h"
#include "PrintoutSheetCreator.h"
#include "SheetConfig.h"
#include "SheetWriter.h"

#include <wx/choicdlg.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/log.h>
#ifdef __WXMSW__
#include <wx/mimetype.h>
#endif

extern Config *config;

class PdfSheetWriter : public GenericSheetWriter
{
	DECLARE_CLASS( PdfSheetWriter )
public:

	/*****************************************************
	**
	**   PdfSheetWriter   ---   Constructor
	**
	******************************************************/
	PdfSheetWriter( Horoscope *horoscope, Sheet *sheet, SheetConfig *sheetcfg, WriterConfig *writercfg )
		: GenericSheetWriter( sheet, sheetcfg, writercfg, config->colors )
	{
		pcfg = config->print;
		pdfdoc = new BasePdfDocument( horoscope, pcfg );

		pageSize = MPoint( pdfdoc->GetPageWidth(), pdfdoc->GetPageHeight());
		table_cell_delta_x = 2;
		//table_cell_delta_y = 7;
		table_cell_delta_y = 4;
		table_widget_extra_y = 20;
		table_cell_align_delta = 3;
		table_empty_col_width = 10;

		doCenterAll = true;
		//doCenterAll = false;
	}

	/*****************************************************
	**
	**   PdfSheetWriter   ---   Destructor
	**
	******************************************************/
	~PdfSheetWriter()
	{
		delete pdfdoc;
	}

	/*****************************************************
	**
	**   PdfSheetWriter   ---   doPrint
	**
	******************************************************/
	bool doPrint( wxString filename )
	{
		bool ret = true;

		if ( filename.IsEmpty())
		{
			filename = wxFileName::CreateTempFileName( wxT( "testpage" ));
		}

		pdfdoc->SetCompression( pcfg->compressPdf );
		pdfdoc->AddPage();

		//printf( "START PdfSheetWriter::doPrint\n" );

		contentRect.x = pdfdoc->GetLeftMargin();
		contentRect.y = pdfdoc->GetTopMargin();
		contentRect.width = pdfdoc->GetPageWidth() - pdfdoc->GetLeftMargin() - pdfdoc->GetRightMargin();
		contentRect.height = 100000;
		//contentRect = MRect( 0, 0, 100000, 100000 );

		Painter *painter = new PdfPainter( pdfdoc );

		preformat( painter, contentRect.width );
		drawSheet( painter, contentRect );
		delete painter;

		pdfdoc->SaveAsFile( filename );

		//printf( "ENDE PdfSheetWriter::doPrint\n" );
		return ret;
	}

protected:

	/*****************************************************
	**
	**   PdfSheetWriter   ---   handlePageBreak
	**
	******************************************************/
	void handlePageBreak( SheetItem *item0 )
	{
		bool dobreak =
			item0->rect.y > pdfdoc->GetTopMargin() // do not break if page is still empty
			&& item0->rect.y + item0->rect.height > pdfdoc->GetPageHeight() - pdfdoc->GetTopMargin(); // break if item would be too large

		//printf( "PdfSheetWriter::handlePageBreak dobreak %d\n", dobreak );

		if ( dobreak )
		{
			double y0 = item0->rect.y + item0->rect.height;
			if ( dobreak )
			{
				pdfdoc->AddPage();
				y0 = pdfdoc->GetTopMargin();
				ycursor = y0;
			}

			// shift all items beginning with item0
			bool doshift = false;
			for( std::list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
			{
				if ( *iter == item0 )
				{
					doshift = true;
				}
				if ( doshift )
				{
					//printf( "START MOVE after page break\n" );
					(*iter)->moveTo( (*iter)->rect.x, y0 );
					y0 += (*iter)->rect.height;
					//printf( "FINISHED MOVE after page break\n" );
				}
			}
		}
	}

	/*****************************************************
	**
	**   PdfSheetWriter   ---   getFontIdForItem
	**
	******************************************************/
	FONT_ID getFontIdForItem( const SHEET_TEXT_ITEM_SUBTYPE &type )
	{
		//printf( "PdfSheetWriter   ---   getFontIdForItem %d\n", (int)type );
		switch( type )
		{
			case WitHeader:
				return FONT_PDF_HEADER;
			break;
			case WitParagraph:
			case WitLine:
				return FONT_PDF_DEFAULT;
			break;
			case WitTableCell:
				return FONT_PDF_TABLE_CONTENTS;
			break;
			case WitTableHeader:
				return FONT_PDF_TABLE_HEADER;
			break;
			default:
				assert( false );
			break;
		}
	}


	PdfDocumentConfig *pcfg;
	BasePdfDocument *pdfdoc;
};

IMPLEMENT_CLASS( PdfSheetWriter, GenericSheetWriter )

/*****************************************************
**
**   PdfTool   ---   Constructor
**
******************************************************/
PdfTool::PdfTool( PdfDocumentConfig *cfg )
{
	if ( cfg ) pcfg = new PdfDocumentConfig( *cfg );
	else pcfg = new PdfDocumentConfig( *config->print );
}

/*****************************************************
**
**   PdfTool   ---   Destructor
**
******************************************************/
PdfTool::~PdfTool()
{
	delete pcfg;
}

/*****************************************************
**
**   PdfTool   ---   printTestpage
**
******************************************************/
void PdfTool::printTestpage()
{
	Horoscope *h = new Horoscope;
	h->update();
	wxString filename = wxFileName::CreateTempFileName( wxT( "testpage" ));
	printHoroscope( h, filename );
	delete h;
}

/*****************************************************
**
**   PdfTool   ---   printTestpage
**
******************************************************/
void PdfTool::printTestpage( Horoscope *horoscope, ChartProperties *props )
{
	Sheet *sheet = new Sheet;
	SheetConfigLoader *sloader = SheetConfigLoader::get();
	wxString filename = wxFileName::CreateTempFileName( wxT( "testpage" ));

	PrintoutSheetCreator c( horoscope, props );
	c.write( sheet, pcfg->defaultPrintout );

	PdfSheetWriter *pdfwriter = new PdfSheetWriter( horoscope, sheet, sloader->getConfig( pcfg->sheetStyle ), config->writer );
	pdfwriter->doPrint( filename );

	if ( pcfg->launchPdfViewer ) launchBrowser( filename );
	else
	{
		doMessageBox( (wxWindow*)NULL, wxString::Format( _( "Chart printed to file %s." ), filename.c_str() ), wxOK | wxCENTRE );
	}
}

/*****************************************************
**
**   PdfTool   ---   doSheetExport
**
******************************************************/
bool PdfTool::doSheetExport( Sheet *sheet )
{
  SheetConfigLoader *loader = SheetConfigLoader::get();
	SheetConfig *sheetcfg = loader->getConfig( config->print->sheetStyle );

	wxString filename;
	const bool b = selectFilename( filename, wxT( "out" ) );
	if ( ! b ) return false;

	// TODO Horoscope is NULL
	PdfSheetWriter *pdfwriter = new PdfSheetWriter( (Horoscope*)NULL, sheet, sheetcfg, config->writer );
	pdfwriter->setCenterAll();
	pdfwriter->doPrint( filename );

	if ( pcfg->launchPdfViewer ) launchBrowser( filename );
	else
	{
		doMessageBox( (wxWindow*)NULL, wxString::Format( _( "Chart printed to file %s." ), filename.c_str() ), wxOK | wxCENTRE );
	}
	delete pdfwriter;
	return true;
}

/*****************************************************
**
**   PdfTool   ---   selectPrintoutId
**
******************************************************/
bool PdfTool::selectPrintoutId( int &type )
{
	PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
	wxSingleChoiceDialog dialog( 0, _( "Select Printout" ), GUI_APP_NAME, loader->getNamesAsArray() );
	dialog.SetSelection( pcfg->defaultPrintout );
	if ( IS_VALID_SIZE( config->viewprefs->sizes.sPrintoutSelectionDialog ))
	{
		dialog.SetSize( config->viewprefs->sizes.sPrintoutSelectionDialog );
	}
	if ( dialog.ShowModal() == wxID_OK )
	{
		config->viewprefs->sizes.sPrintoutSelectionDialog = dialog.GetSize();
		type = dialog.GetSelection();
		return true;
	}
	//return pcfg->defaultPrintout;
	return false;
}

/*****************************************************
**
**   PdfTool   ---   selectFilename
**
******************************************************/
bool PdfTool::selectFilename( wxString &fullname, wxString basename )
{
	fullname.Clear();

	if ( basename.IsEmpty()) basename = wxT( "unknown" );
	wxString filename;
	filename << basename << wxT( ".pdf" );
	const static wxString filetypes = wxT( "Portable Document Format (*.pdf)|*.pdf|All files (*)| *.*" );

	// override prompt is handled later
	wxFileDialog exportFileDialog( NULL,
		_("Save Document" ),
		config->viewprefs->defExportPath,
		filename,
		filetypes,
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT,
		wxDefaultPosition );
	if ( exportFileDialog.ShowModal() == wxID_OK )
	{
		fullname << exportFileDialog.GetDirectory() << wxFileName::GetPathSeparator() << exportFileDialog.GetFilename();
	}
	else
	{
		return false;
	}
	config->viewprefs->defExportPath = exportFileDialog.GetDirectory();
	return true;
}

/*****************************************************
**
**   PdfTool   ---   printHoroscope
**
******************************************************/
void PdfTool::printHoroscope( Horoscope *horoscope, const bool doSelect )
{
	wxString filename;

	if ( doSelect )
	{
		if ( ! selectPrintoutId( pcfg->defaultPrintout )) return;
		if ( ! selectFilename( filename, horoscope->getHName())) return;
	}
	else
	{
		if ( horoscope->getHName().IsEmpty())
		{
			horoscope->setHName( wxT( "unknown" ));
		}
		filename = config->viewprefs->defExportPath;
		if ( ! filename.IsEmpty()) filename << wxFileName::GetPathSeparator();
		filename << horoscope->getHName() << wxT( ".pdf" );

		if ( pcfg->askOverwrite && wxFile::Exists( filename ))
		{
			if ( 
			doMessageBox( 0,
				wxString::Format( _( "File %s exists. Do you want to overwrite?" ), filename.c_str())
				,wxYES_NO | wxCENTRE ) == wxID_NO ) return;
		}
	}
	printHoroscope( horoscope, filename );
}

/*****************************************************
**
**   PdfTool   ---   printHoroscope
**
******************************************************/
void PdfTool::printHoroscope( Horoscope *horoscope, wxString filename )
{
	ChartProperties *props = new ChartProperties;
	props->setWesternSkin( config->print->wGraphicSkin );
	props->setVedicSkin( config->print->vGraphicSkin );

	PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
	PrintoutConfig *theconfig = loader->getConfig( pcfg->defaultPrintout );
	props->setVedic( theconfig->vedic );

	Sheet *sheet = new Sheet;
	SheetConfigLoader *sloader = SheetConfigLoader::get();

	PrintoutSheetCreator c( horoscope, props );
	c.write( sheet, pcfg->defaultPrintout );

	PdfSheetWriter *pdfwriter = new PdfSheetWriter( horoscope, sheet, sloader->getConfig( pcfg->sheetStyle ), config->writer );
	pdfwriter->doPrint( filename );

	finishPrint( filename );

	delete pdfwriter;
	delete props;
	delete sheet;
}

/*****************************************************
**
**   PdfTool   ---   finishPrint
**
******************************************************/
bool PdfTool::finishPrint( wxString filename )
{
	if ( pcfg->launchPdfViewer ) return launchBrowser( filename );

	//else
	doMessageBox( (wxWindow*)NULL, wxString::Format( _( "Chart printed to file %s." ), filename.c_str() ), wxOK | wxCENTRE );
	return false;
}

/*****************************************************
**
**   PdfTool   ---   launchBrowser
**
******************************************************/
bool PdfTool::launchBrowser( wxString filename )
{
	wxString command;

#ifdef __WXMSW__
	wxFileType *filetype = wxMimeTypesManager().GetFileTypeFromExtension( wxT( "pdf" ));
	if ( filetype != 0 )
	{
		command = filetype->GetOpenCommand( filename );
	}
	if ( filetype == 0 || command.IsEmpty())
	{
		doMessageBox( 0, _( "Cannot launch pdf viewer." ), wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}
#else

	if ( pcfg->pdfViewerCommand.IsEmpty())
	{
		doMessageBox( 0,
			_( "Pdf viewer command is empty. Please configure the command in the configuration dialog." ),
			wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}

	// wxmac has generic open command
#ifndef __WXMAC__
	else if ( ! wxFileName::FileExists( pcfg->pdfViewerCommand ))
	{
		doMessageBox( 0,
			wxString::Format( _( "Pdf viewer file \"%s\" does not exist. Please configure the command in the configuration dialog." ),
				pcfg->pdfViewerCommand.c_str()),
			wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}
#endif

	command << pcfg->pdfViewerCommand << wxT( " \"" ) << filename << wxT( "\"" );
#endif
	long r = wxExecute( command );
	if ( r == -1 )
	{
		doMessageBox( 0,
			wxString::Format( _( "Can't launch pdf viewer, command is \"%s\" (error code %ld)" ), command.c_str(), r ),
			wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}
	return true;
}


