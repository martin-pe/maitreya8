/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfTools.h
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

#ifndef _PDFTOOLS_H_
#define _PDFTOOLS_H_

#include <wx/string.h>


//using namespace std;

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


