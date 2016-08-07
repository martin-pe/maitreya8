/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PrintoutConfigBase.h
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
#ifndef _PRINTOUTCONFIGBASE_H_
#define _PRINTOUTCONFIGBASE_H_

#include "ConfigBase.h"
#include <wx/string.h>

/*************************************************//**
*
* 
*
******************************************************/
class PdfCustomTextField : public ConfigBase
{
public:

	PdfCustomTextField()
	{
		enabled = false;
		showSeparator = false;
		orientation = 0;
		showOnFirstPage = false;
	}

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	bool enabled;
  wxString text;
	bool showSeparator;
	int orientation;
	bool showOnFirstPage;
};

/*************************************************//**
*
* 
*
******************************************************/
class PdfDocumentConfig : public ConfigBase
{
public:
	PdfDocumentConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	int getWxPaperSize();

	bool saveFileOption;
	bool showSelectDialog;
	int defaultPrintout;
	int paperFormat;
	bool launchPdfViewer;
	bool askOverwrite;
	bool compressPdf;

	int sheetStyle;
	int vGraphicSkin;
	int wGraphicSkin;
	wxString pdfViewerCommand;

	// custom text in PDF printout
	PdfCustomTextField customHeader;
	PdfCustomTextField customFooter;
};

#endif

