/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VargaHoroscope.h
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

#ifndef _VargaHOROSCOPE_H_
#define _VargaHOROSCOPE_H_

#include <Varga.h>

class Horoscope;

/*************************************************//**
*
* \brief minimal abstract chart representation
*
******************************************************/
class AbstractVargaHoroscope
{
public:

	virtual Rasi getRasi( const ObjectId &planet ) const = 0;
	virtual int getNakshatra27( const ObjectId &planet ) const = 0;
	virtual int getNakshatra28( const ObjectId &planet ) const = 0;

	virtual bool isRetrograde( const ObjectId &planet ) const = 0;
	virtual bool isBenefic( const ObjectId& ) const = 0;

	int getBhava( const ObjectId &planet ) const;
	bool isMalefic( const ObjectId &p ) const { return ! isBenefic( p ); }
};

/*************************************************//**
*
* \brief virtual Horoscope in Varga chart giving access to astrological properties
*
******************************************************/
class VargaHoroscope : public AbstractVargaHoroscope
{
public:

	VargaHoroscope( Horoscope*, const Varga = V_RASI );
	virtual ~VargaHoroscope() {}

	virtual Rasi getRasi( const ObjectId &planet ) const;
	virtual int getNakshatra27( const ObjectId &planet ) const;
	virtual int getNakshatra28( const ObjectId &planet ) const;

	virtual bool isRetrograde( const ObjectId &planet ) const;
	virtual bool isBenefic( const ObjectId& ) const;

	Horoscope *getHoroscope() const { return horoscope; }
	void setHoroscope( Horoscope *h ) { horoscope = h; }

	void setVarga( const Varga& );
	Varga getVarga() const { return varga; }

	double getRasiLongitude( const ObjectId &planet ) const;


private:

	Horoscope *horoscope;
	Varga varga;
};
/*************************************************//**
*
* \brief minimal concrete chart representation
*
******************************************************/
class MinimalVargaHoroscope : public AbstractVargaHoroscope
{
public:

	MinimalVargaHoroscope();
	virtual ~MinimalVargaHoroscope() {}

	void cleanup();

	void setRasi( const ObjectId &id, const Rasi &rasi );

	virtual Rasi getRasi( const ObjectId &planet ) const;
	virtual int getNakshatra27( const ObjectId& ) const { return -1; }
	virtual int getNakshatra28( const ObjectId& ) const { return -1; }

	virtual bool isRetrograde( const ObjectId& ) const { return false; }
	virtual bool isBenefic( const ObjectId& ) const { return false; }

private:

	Rasi ephem_objects_rasi[MAX_EPHEM_OBJECTS];
	Rasi extended_objects_rasi[MAX_EXTENDED_OBJECTS];
	Rasi arabic_objects_rasi[MAX_ARABIC_OBJECTS];

};

#endif


