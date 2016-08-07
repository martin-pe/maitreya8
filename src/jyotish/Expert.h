/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Expert.h
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

#ifndef _EXPERT_H_
#define _EXPERT_H_

class Horoscope;

#include "maitreya.h"

/*************************************************//**
*
* \brief base class for experts (encapsulation of various astrological calculations)
*
******************************************************/
class Expert
{
public:

	Expert( Horoscope* = (Horoscope*)NULL );
	virtual ~Expert() {}

	Horoscope *getHoroscope() const { return horoscope; }
	void setHoroscope( Horoscope* h ) { horoscope = h; }

protected:
	Horoscope *horoscope;
};

/*************************************************//**
*
* \brief
*
******************************************************/
class VedicExpert : public Expert
{
public:
	VedicExpert( Horoscope *h, const Varga &varga );

	void setVarga( const Varga &v ) { varga = v; }
	Varga getVarga() const { return varga; }

	Rasi getRasi( const ObjectId& );
	double getRasiLongitude( const ObjectId& );

protected:
	Varga varga;

};

#endif


