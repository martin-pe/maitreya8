/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PrintoutSheetCreator.h
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

#ifndef _PRINTOUTSHEETCREATOR_H_
#define _PRINTOUTSHEETCREATOR_H_

class ChartProperties;
class Horoscope;
class PrintoutItem;
class Sheet;
class SheetItem;
class SheetWidgetItem;

/*************************************************//**
*
*  
*
******************************************************/
class PrintoutSheetCreator
{
public:
	PrintoutSheetCreator( Horoscope*, ChartProperties* );

	void write( Sheet*, const int &id );

protected:

	void addItem( Sheet*, PrintoutItem* );
	//SheetWidgetItem *getWidgetItem( PrintoutItem* );
	//SheetItem *printoutItem2SheetItem( PrintoutItem* );

	Horoscope *horoscope;
	ChartProperties *chartprops;
};


#endif


