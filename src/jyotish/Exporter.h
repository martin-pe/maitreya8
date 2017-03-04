/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Exporter.h
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

#ifndef _EXPORTER_H_
#define _EXPORTER_H_

#include "maitreya.h"

class Sheet;
class SheetConfig;
class WriterConfig;

/*************************************************//**
*
* 
*
******************************************************/
class Exporter
{
public:

	Exporter( SheetConfig *scfg = (SheetConfig*)NULL, WriterConfig* = (WriterConfig*)NULL );
	virtual ~Exporter() {}
	virtual wxString exportSheet( Sheet* ) = 0;

protected:

	SheetConfig *sheetcfg;
	WriterConfig *writercfg;
	wxString s;

};


/*************************************************//**
*
* 
*
******************************************************/
class ExporterFactory
{
public:
	Exporter *getExporter( const WidgetExportType&, SheetConfig* = (SheetConfig*)NULL, WriterConfig* = (WriterConfig*)NULL );

protected:
	Exporter *getHtmlExporter( SheetConfig* = (SheetConfig*)NULL, WriterConfig* = (WriterConfig*)NULL );
	Exporter *getPlainHtmlExporter( SheetConfig *sheetcfg, WriterConfig *writercfg );

};

#endif


