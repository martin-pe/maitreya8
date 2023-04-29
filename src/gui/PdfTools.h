/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfTools.h
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

#ifndef _PDFTOOLS_H_
#define _PDFTOOLS_H_

#include <wx/string.h>

//class BasePdfDocument;
class ChartProperties;
class Horoscope;
class PdfDocumentConfig;
class Sheet;
class SheetConfig;
//class WriterConfig;

/*************************************************//**
*
*  
*
******************************************************/
class PdfTool
{
public: 
	PdfTool( PdfDocumentConfig* );

	~PdfTool();

	void printHoroscope( Horoscope*, const bool doSelect = false );

	void printTestpage( Horoscope *horoscope, ChartProperties *props );

	void printTestpage();

	bool doSheetExport( Sheet* );

protected:
	PdfDocumentConfig *pcfg;

	bool finishPrint( wxString filename );
	bool launchBrowser( wxString filename );

	void printHoroscope( Horoscope *horoscope, wxString filename );

	bool selectPrintoutId( int &type );
	bool selectFilename( wxString &fulname, wxString basename );

};


#endif


