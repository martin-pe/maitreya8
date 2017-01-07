/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/PrintoutTextHelper.h
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

#ifndef _PRINTOUTTEXTHELPER_H_
#define _PRINTOUTTEXTHELPER_H_

class ChartProperties;
class Horoscope;
class Sheet;
class Table;

#include "maitreya.h"


/*************************************************//**
*
*  
*
******************************************************/
class PrintoutTextHelper
{
public:

	PrintoutTextHelper( Horoscope*, const ChartProperties*, Sheet* );
	~PrintoutTextHelper();

	void setShowHeader( const bool b = true ) { show_header = b; }

	void writePrintoutHeader( const int &mode, const bool &vedic );

	void writeTitle();

private:
	Table *writeBaseData( const int &mode, const bool &vedic );

	Horoscope *horoscope;
	ChartProperties *chartprops;
	Sheet *sheet;
	bool show_header;
};

#endif

