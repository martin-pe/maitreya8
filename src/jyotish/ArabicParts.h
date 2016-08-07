/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/ArabicParts.h
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

#ifndef _ARABICPARTS_H_
#define _ARABICPARTS_H_

#include "BasicHoroscope.h"
#include <wx/string.h>

class Horoscope;
class VedicPlanet;
class VargaData;
class Sheet;

/*************************************************//**
*
* \brief names and rules of arabis parts
*
******************************************************/
struct ArabicPartDefinition
{
	wxString name;
	int  part[6];
	bool flip;  // flip addition+subtraction for night birth
};

/*************************************************//**
*
* \brief encapsulates calculation of Arabic Parts
*
******************************************************/
class ArabicPartsExpert
{
public:

	ArabicPartsExpert( Horoscope* );
	~ArabicPartsExpert();

	static wxString getObjectName( const int &p, const int &format );

	void update();
	ObjectPosition getObjectPosition( const int &p, const bool vedic );
	VargaData *getVargaData( const int &planet, const Varga& );
	VedicPlanet *getPlanet( const int &planet );
	void write( Sheet*, const bool &vedic, const bool show_header = true );

private:

	void updateAll();
	void updatePart( const int &p );
	wxString getItemDescription( Sheet*, const int &type, const ObjectId &obj );
	static ArabicPartDefinition getPart( const int &i );

	Horoscope *h;
	bool dirty;
	double vayanamsa, wayanamsa;
	ObjectPosition vpos[MAX_ARABIC_OBJECTS], wpos[MAX_ARABIC_OBJECTS];
	VedicPlanet *planets[MAX_ARABIC_OBJECTS];
};

#endif


