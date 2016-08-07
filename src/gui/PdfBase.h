/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfBase.h
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


