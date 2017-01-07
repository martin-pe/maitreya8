/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/ShadBala.h
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

#ifndef _SHADBALA_H_
#define _SHADBALA_H_

#include "Expert.h"
#include "maitreya.h"

class Horoscope;
class Sheet;

/*************************************************//**
*
* \brief Calculates the six Balas of Vedic astrology
*
******************************************************/
class ShadBalaExpert : public Expert
{
public:

	ShadBalaExpert( Horoscope *h );
	virtual ~ShadBalaExpert() {}

	virtual void write( Sheet* );
	void updateAllBalas();

	double getShadBala( const int &planet ) const {
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return shadbala[planet];
	}
	double getPercentualShadBala( const int &planet ) const;

	// Sthaana Bala
	double getSthaanaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return sthaanabala[planet];
	}
	double getUchchaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return uchchabala[planet];
	}
	double getSaptavargajaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return samasaptavargajabala[planet];
	}
	double getOjhajugmaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return ojhajugmabala[planet];
	}
	double getKendradiBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return kendradibala[planet];
	}
	double getDrekkanaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return drekkanabala[planet];
	}

	double getSaptavargajaBala( const int &planet, const int &varga ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		assert( varga >= 0 && varga < 7 );
		return saptavargajabala[planet][varga];
	}
	double getMaximumSaptavargajaBala() const { return max_saptavargajabala; }

	double getDigBala( const int &planet ) const {
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return digbala[planet];
	}

	// Kala Bala
	double getKalaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return kalabala[planet];
	}
	double getNathonathaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return nathonathabala[planet];
	}
	double getPakshaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return pakshabala[planet];
	}
	double getTribhagaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return tribhagabala[planet];
	}
	double getVarshaMasaDinaHoraBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return varshamasadinahorabala[planet];
	}
	double getAyanaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return ayanabala[planet];
	}
	double getYudhdhaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return yudhdhabala[planet];
	}

	double getCheshtaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return cheshtabala[planet];;
	}
	double getNaisargikaBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return naisargikabala[planet];;
	}
	double getDrikBala( const int &planet ) const
	{
		ASSERT_VALID_SHADBALA_INDEX( planet );
		return drigbala[planet];;
	}

private:

	double sthaanabala[7], uchchabala[7], samasaptavargajabala[7], ojhajugmabala[7], kendradibala[7], drekkanabala[7];
	double digbala[7];
	double saptavargajabala[7][7];
	double max_saptavargajabala;

	double kalabala[7], nathonathabala[7], pakshabala[7], tribhagabala[7], varshamasadinahorabala[7],
	ayanabala[7], yudhdhabala[7];

	ObjectId varshalord, masalord, dinalord, horalord;
	double drigbala[7], drigbala_pair[7][7];
	double cheshtabala[7], naisargikabala[7], shadbala[7];
	int cheshtabalatype[7];
	double percentual_speed[7], speed[7];
	bool sunriseStatusOk;

	void clear();
	void updateSthaanaBala();
	void updateDigBala();
	void updateKalaBala();
	void updateNaisargikaBala();
	void updateCheshtaBala();
	void updateDrigBala();

	static const int required_shadbala[7];
	static const double circulation_time[7];
};

#endif

