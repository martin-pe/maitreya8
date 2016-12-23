/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfBase.h
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

#ifndef _PDFBASE_H_
#define _PDFBASE_H_

// must be here because pdfcolour has no reference to wxColourDatabase
#include <wx/gdicmn.h>
#include <wx/pdfdoc.h>

class Horoscope;
class PdfDocumentConfig;

/*************************************************//**
*
* \brief adds Footer and font methods to wxPdfDocument
*
******************************************************/
class BasePdfDocument : public wxPdfDocument
{
public:

	BasePdfDocument( Horoscope*, PdfDocumentConfig* );

protected:

	virtual void Footer();
	virtual void Header();

	wxString setCustomFieldText( wxString );
	int getPdfAlign( const int &orientation );
	void setHeaderFont();

	const Horoscope *horoscope;
	const PdfDocumentConfig *pcfg;
};

#endif


