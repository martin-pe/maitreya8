/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/VargaHoroscope.h
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


