/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/AshtottariDasa.h
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
#ifndef _ASHTOTTARIDASA_H_
#define _ASHTOTTARIDASA_H_

#include "maitreya.h"
#include "Dasa.h"

class Horoscope;

/*************************************************//**
*
* \brief calculation of Ashtottari Dasa
*
******************************************************/
class AshtottariDasaExpert : public DasaExpert
{
public:

	AshtottariDasaExpert( Horoscope *h ) : DasaExpert( D_ASHTOTTARI, h ) {}

	virtual vector<Dasa*> getFirstLevel();
	virtual vector<Dasa*> getNextLevel( Dasa* );

	virtual vector<KPEvent> getKPEventList( const double &startlen, const double &endlen, const double &startjd );
	virtual KpData getKPLords( const double& ) { return KpData(); }

	virtual int getParamayus() const { return 108; }

	virtual bool isGrahaDasaExpert() const { return true; }

private:
	const static int dasa_lord[8];
	const static int dasa_dur[8];
};

#endif

