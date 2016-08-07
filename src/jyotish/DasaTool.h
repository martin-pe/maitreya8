/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/DasaTool.h
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

#ifndef _DASATOOL_H_
#define _DASATOOL_H_

#include "maitreya.h"
#include <map>
#include <set>

class Dasa;
class DasaExpert;
class Horoscope;
class Sheet;

using namespace std;

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
	set<wxString> &getDasaSources();
	void dumpDasa( Dasa*, wxString& );

	void writeShortReport( Sheet*, Horoscope*, const DasaId&, const bool show_header = false );
	void writeCompactReport( Sheet*, Horoscope*, const DasaId&, const bool show_header = false );
	void writeComprehensiveReport( Sheet*, Horoscope*, const DasaId&, const bool show_header = false );

private:
	DasaTool();
	map<wxString, ObjectId> objects;

};

#endif

