/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Ashtakavarga.h
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
#ifndef _ASHTAKAVARGA_H_
#define _ASHTAKAVARGA_H_

#include<wx/string.h>

#include "Expert.h"

class Sheet;

enum { REKHA, TRIKONA, EKADHI, RasiSAMA, NAKSHATRASAMA };
enum { GRAHAPINDA, RASIPINDA, YOGAPINDA };

/*************************************************//**
*
* \brief encapsulates calculation of Ashtaka Varga
*
******************************************************/
class AshtakavargaExpert : public VedicExpert
{
public:

	AshtakavargaExpert( Horoscope *h, const Varga &varga ) : VedicExpert( h, varga ) {}
	void update();

	// get Ashtakavarga value for rasi
	// type is one of REKHA, TRIKONA, EKADHI, RasiSAMA, NAKSHATRASAMA
	int getItem( const int &type, const ObjectId &planet, const int &rasi );

	// returns the pinda value, pinda is one of GRAHAPINDA, RASIPINDA, YOGAPINDA
	int getPinda( const int &type, const int& ) const;

	// returns the sums for each rasi, type is one of REKHA, TRIKONA, EKADHI, RasiSAMA, NAKSHATRASAMA
	int getSarva( const int &type, const Rasi& ) const;

	int getPlanetSarva( const int &type, const ObjectId& ) const;

	void write( Sheet*, const bool show_header = false );
	void writeSingleAv( Sheet*, const int& );

private:


	void calcRekha();
	void calcSarva();
	void calcPinda();
	void calcTrikonaShodana();
	void calcEkadhipatyaPair( const Rasi &b1, const Rasi &b2 );
	void calcEkadhipatyaShodana();
	int getSingleRekha( const int &i, const int &j, const int &k );

	int rekha[8][12], trikona[8][12], ekadhi[8][12];
	int sarvaRekha[12], sarvaTrikona[12], sarvaEkadhi[12];
	int psarvaRekha[8], psarvaTrikona[8], psarvaEkadhi[8];
	int grahaPinda[12], rasiPinda[12], yogaPinda[12];

	int planetNumber[12];
};

#endif

