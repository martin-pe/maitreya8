/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PdfBase.cpp
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


#include "PdfBase.h"

#include "FontProvider.h"
#include "Horoscope.h"
#include "Lang.h"
#include "PrintoutConfigBase.h"

#include <wx/font.h>

//#define PDF_UNIT wxT( "mm" )
#define PDF_UNIT wxT( "pt" )

#define PDF_ROW_SCALING_FACTOR .5
#define PDF_HEADER_Y 8.0
#define PDF_HEADER_Y_SKIP 4
#define PDF_FOOTER_Y -15.0
#define PDF_TINYFONT_SCALINGFACTOR .6
#define PDF_SYMBOLFONT_SCALINGFACTOR 4


/*****************************************************
**
**   BasePdfDocument   ---   Constructor
**
******************************************************/
BasePdfDocument::BasePdfDocument( Horoscope *h, PdfDocumentConfig *c )
 : wxPdfDocument( wxPORTRAIT, PDF_UNIT, (wxPaperSize)c->getWxPaperSize() ),
 horoscope( h ),
 pcfg( c )
{
	//printf( "LEFT MARGIN %f TOP MARGIN %f\n", GetLeftMargin(), GetTopMargin());
	wxString s;
  s << GUI_APP_NAME << wxT( " " ) << getApplicationVersion();
	SetCreator( s );

	SetFontSubsetting( true );
}

/*****************************************************
**
**   BasePdfDocument   ---   setHeaderFont
**
******************************************************/
void BasePdfDocument::setHeaderFont()
{
	FontProvider *f = FontProvider::get();
	SetFont( *f->getFont( FONT_PDF_TINY ));
}

/*****************************************************
**
**   BasePdfDocument   ---   Header
**
******************************************************/
void BasePdfDocument::Header()
{
	//printf( "BasePdfDocument::Header\n" );
	setHeaderFont();

	if ( pcfg->customHeader.enabled && ( PageNo() != 1 || pcfg->customHeader.showOnFirstPage ))
	{
		//printf( "Print custom header on page %d\n", PageNo() );

		// Position at 1.5 cm from top TODO einheit
		SetY( PDF_HEADER_Y );

		const double height = PDF_ROW_SCALING_FACTOR * GetFontSize();
		const int border = wxPDF_BORDER_NONE;

		// w, h, text, border, align, fill
		MultiCell( 0, height, 
			setCustomFieldText( pcfg->customHeader.text ),
			border,
			getPdfAlign( pcfg->customHeader.orientation ), 0
		);

		if( pcfg->customHeader.showSeparator )
		{
			const double y = GetY() + PDF_HEADER_Y_SKIP;
			const double y2 = GetPageWidth() - GetLeftMargin();
			//printf( "Draw Header Separator at %f left %f right %f\n", y, GetLeftMargin(), y2 );
			Line( GetLeftMargin(), y, y2, y );
		}

	}
	SetY( GetY() + PDF_HEADER_Y_SKIP );
}

/*****************************************************
**
**   BasePdfDocument   ---   Footer
**
******************************************************/
void BasePdfDocument::Footer()
{
	//printf( "BasePdfDocument::Footer\n" );
	setHeaderFont();

	if ( pcfg->customFooter.enabled && ( PageNo() != 1 || pcfg->customFooter.showOnFirstPage ))
	{
		//printf( "Print custom footer on page %d\n", PageNo() );

		// Position at 1.5 cm from bottom TODO einheit
		SetY( PDF_FOOTER_Y );
		const double height = PDF_ROW_SCALING_FACTOR * GetFontSize();

		const int border = wxPDF_BORDER_NONE;

		if( pcfg->customFooter.showSeparator )
		{
			const double y = GetY() - PDF_HEADER_Y_SKIP;
			const double y2 = GetPageWidth() - GetLeftMargin();
			//printf( "Draw Footer Separator at %f left %f right %f\n", y, GetLeftMargin(), y2 );
			Line( GetLeftMargin(), y, y2, y );
		}

		// w, h, text, border, ln, align
		Cell( 0, height, 
			setCustomFieldText( pcfg->customFooter.text ),
			border,
			0, getPdfAlign( pcfg->customFooter.orientation )
		);
	}
}

/*****************************************************
**
**   BasePdfDocument   ---   setCustomFieldText
**
******************************************************/
wxString BasePdfDocument::setCustomFieldText( wxString s )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();

	if ( horoscope )
	{
		s.Replace( wxT( "$name" ), horoscope->getHName());
		s.Replace( wxT( "$date" ), formatter->formatDateString( horoscope->getJD()));
		s.Replace( wxT( "$fulldate" ), formatter->formatFullDateString( horoscope->getJD()));
		s.Replace( wxT( "$location" ), horoscope->getLocation()->getLocName());
	}
	s.Replace( wxT( "$creationdate" ), wxDateTime().Now().FormatDate());
	s.Replace( wxT( "$version" ), getApplicationVersion());
	s.Replace( wxT( "$page" ), wxString::Format( wxT( "%d" ), PageNo()));

	return AllTrim( s );
}

/*****************************************************
**
**   BasePdfDocument   ---   getPdfAlign
**
******************************************************/
int BasePdfDocument::getPdfAlign( const int &orientation )
{
	switch( orientation )
	{
		case 1:
			return wxPDF_ALIGN_CENTER;
		break;
		case 2:
			return wxPDF_ALIGN_RIGHT;
		break;
		default:
			return wxPDF_ALIGN_LEFT;
		break;
	}
}


