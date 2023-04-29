/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Horoscope.h
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

#ifndef _HOROSCOPE_H_
#define _HOROSCOPE_H_

#include "DataSet.h"
#include "BasicHoroscope.h"

class ChartProperties;
class VedicPlanet;
class VargaData;
class ArabicPartsExpert;

/*************************************************//**
*
*
*
******************************************************/
struct KpData
{
  KpData() { lord = sublord = subsublord = ONONE; }

	// kp lords can be either planets or rasi, similar to Dasa lords
	int lord, sublord, subsublord;
};


/*************************************************//**
*
*  \brief main astrological class for charts holding most features of Vedic astrology
*
******************************************************/
class Horoscope : public BasicHoroscope
{
	friend class CompositHoroscope;
public:

	Horoscope();
	virtual ~Horoscope();

	// calculation of planetary positions
	void update();

	virtual ObjectPosition getObjectPosition( const ObjectId &i, const bool &vedic ) const;
	VedicPlanet *getPlanet( const ObjectId &i ) const;
	VargaData *getVargaData( const ObjectId &planet, const Varga &varga ) const;

	double getSunrise()  const { return sunrise; }
	double getSunset()   const { return sunset; }
	bool getDayBirth()   const { return isDayBirth; }

	bool isBenefic( const ObjectId& ) const;

	// not included in update() because Dasa index may change
	void updateKP( const DasaId &dasaindex ) const;
	KpData getKPLords( const ObjectId &planet ) const;
	KpData getHouseKPLords( const int &house ) const;

protected:
	mutable KpData kp_lord[MAX_EPHEM_OBJECTS], ekp_lord[MAX_EXTENDED_OBJECTS], akp_lord[MAX_ARABIC_OBJECTS], hkp_lord[12];

	VedicPlanet *ephem_planets[MAX_EPHEM_OBJECTS];

	ObjectPosition *evpos[MAX_EXTENDED_OBJECTS];
	ObjectPosition *ewpos[MAX_EXTENDED_OBJECTS];
	VedicPlanet *extended_planets[MAX_EXTENDED_OBJECTS];
	VedicPlanet *house_planet_wrapper[12];

	double sunrise, sunset, arieslen;
	bool isDayBirth;
	bool moonBenefic, mercuryBenefic;

	ArabicPartsExpert *arabic_expert;
	double lagna_len[NUM_LAGNA];
	double upagraha_len[NUM_UPA];
	double arabic_len[MAX_ARABIC_OBJECTS];

	void updateObjectPositions();

private:
	DECLARE_CLASS( Horoscope )
};

#endif



