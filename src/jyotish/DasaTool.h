/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/DasaTool.h
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

#ifndef _DASATOOL_H_
#define _DASATOOL_H_

#include "maitreya.h"
#include <map>
#include <set>

class Dasa;
class DasaExpert;
class Horoscope;
class Sheet;

/*************************************************//**
*
* 
*
******************************************************/
class DasaTool
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( DasaTool )

public:

	wxString getDasaName( const DasaId& );

	ObjectId planet2ObjectId( const wxString );

	DasaId dasa2DasaId( const wxString );
	
	wxString getDasaLevelName( const int& );
	void calculateMahaAntarLords( Dasa *dasa, int &lord, int &sublord );
	void writeDasaEffect( Sheet*, Dasa*, const wxString sourcefilter = wxEmptyString  );

	wxString getDasaLordChain( Dasa *dasa, const bool addStartAndEndDate = false );
	void writeDasaDocumentation( Sheet*, DasaExpert* = (DasaExpert*)NULL, Dasa* = (Dasa*)NULL, const wxString sourcefilter = wxEmptyString );
	std::set<wxString> &getDasaSources();
	void dumpDasa( Dasa*, wxString& );

	void writeShortReport( Sheet*, Horoscope*, const DasaId&, const bool show_header = false );
	void writeCompactReport( Sheet*, Horoscope*, const DasaId&, const bool show_header = false );
	void writeComprehensiveReport( Sheet*, Horoscope*, const DasaId&, const bool show_header = false );

private:
	DasaTool();
	std::map<wxString, ObjectId> objects;

};

#endif

