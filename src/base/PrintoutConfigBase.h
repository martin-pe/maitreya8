/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PrintoutConfigBase.h
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

