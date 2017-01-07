/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Expert.h
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


