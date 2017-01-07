/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/TextWidget.h
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


#ifndef _TEXTWIDGET_H_
#define _TEXTWIDGET_H_

#include "BasicSheetWidget.h"

class ChartProperties;
class ColorConfig;
class Sheet;
class SheetConfig;
class WriterConfig;
class wxHtmlWindow;

/*************************************************//**
*
*  basic widget holding a HTML text view item
*
******************************************************/
class TextWidget : public BasicSheetWidget
{
	DECLARE_CLASS( TextWidget )
public:
	TextWidget( wxWindow *parent, ChartProperties*, SheetConfig* = (SheetConfig*)NULL,
		WriterConfig* = (WriterConfig*)NULL, ColorConfig* = (ColorConfig*)NULL );

	TextWidget( wxWindow *parent, ChartProperties*, const int &widgetOptions );

	virtual void OnDataChanged();

protected:

	void init();
	virtual void doPaint( const wxRect &rect, const bool eraseBackground = true );

	wxHtmlWindow *html;
};

#endif

