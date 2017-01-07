/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/BasicHoroscope.h
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
#ifndef _BASICHOROSCOPE_H_
#define _BASICHOROSCOPE_H_

#include "maitreya.h"
#include "DataSet.h"

#include <wx/event.h>

/*************************************************//**
*
* \brief Planetary data with longitude, speed and latitude of planetary objects
*
*
* This class holds the planetary data of the astrological objects.
* These data are :
* - longitude in degrees
* - latitude relative to Ecliptic in degrees
* - speed in degrees per day
* - retrogression, i.e. the fact that speed is below zero
*
*   This class is not only used for pure astronomical objects
*   but also for derived objects like Upapadas or Kala Velas.
**
******************************************************/
class ObjectPosition
{
public:
	ObjectPosition( const double = 0, const double = 0, const double = 0, const MOVING_DIRECTION = MD_DIRECT );
	double longitude, latitude, speed;
	MOVING_DIRECTION direction;
};

/*************************************************//**
* \brief Base class for horoscopes with birth data, planets and houses
*
* This class manages the ephemeris objects (planets and houses) of charts.
* Advanced objects are supported by the subclass Horoscope.
**
******************************************************/
class BasicHoroscope : public wxEvtHandler
{
public:

	/**
	*  Constructor. Date is set to current date. Default location is used.
	*/
	BasicHoroscope();

	virtual ~BasicHoroscope();

	PlanetContext getPlanetContext() const { return context; }

	/**
	* Getter for object longitude, latitude and speed. This method is overwritten in variuos subclasses.
	*/
	virtual ObjectPosition getObjectPosition( const ObjectId &planet, const bool &vedic ) const;

	/**
	* Convenience method for access to object longitude. Longitude can be according to Vedic or western Ayanamsa.
	*/
	double getLongitude( const ObjectId &planet, const bool &vedic ) const;

	/**
	* Convenience method for access to Vedic object longitude.
	*/
	double getVedicLongitude( const ObjectId &planet ) const { return getLongitude( planet, true ); }

	/**
	* Convenience method for access to western object longitude.
	*/
	double getWesternLongitude( const ObjectId &planet ) const { return getLongitude( planet, false ); }

	/**
	* Convenience method for access to tropical object longitude (NO Ayanamsa).
	*/
	double getTropicalLongitude( const ObjectId &planet ) const;

	/**
	* Convenience method for access to object latitude (relative to Ecliptic).
	*/
	double getLatitude( const ObjectId &planet ) const;

	/**
	* Convenience method for access to object speed (degrees per day).
	*/
	double getSpeed( const ObjectId &planet ) const;

	/**
	* Convenience method for access to object retrogression.
	*/
	bool isRetrograde( const ObjectId &planet ) const;

	MOVING_DIRECTION getMovingDirection( const ObjectId &planet ) const;

	// name of chart owner
	const wxString getHName() const { return dataSet->getName(); }
	void setHName( wxString name ) { dataSet->setName( name ); }

	// remark entered by the user
	const wxString getRemark() const { return dataSet->getRemark(); }
	void setRemark( wxString remark ) { dataSet->setRemark( remark ); }

	// Date + location getter
	DataSet *getDataSet() const { return dataSet; }
	Location *getLocation() const { return dataSet->getLocation(); }
	double getJD() const { return dataSet->getJD(); }
	double getLongitude() const { return dataSet->getLocation()->getLongitude(); }
	double getLatitude() const { return dataSet->getLocation()->getLatitude(); }
	double getTimeZone() const { return dataSet->getLocation()->getTimeZone(); }
	double getDST() const { return dataSet->getLocation()->getDST(); }

	// Date + location setter
	void setDate( const int &day, const int &month, const int &year, const double &time ) { dataSet->setDate( day, month, year, time ); }
	void setDate( const double &jd ) { dataSet->setDate( jd ); }
	void setCurrentDate() { dataSet->setCurrentDate(); }
	void setLocation( wxString locName, const double &longitude, const double &latitude, const double &tz, const double &dst )
	{
		dataSet->setLocation( locName, longitude, latitude, tz, dst );
	}
	void setLocation( const Location &l ) { dataSet->setLocation( l ); }

	// file methods
	bool openFile( wxString filename );
	bool saveAs( wxString filename );
	bool save();

	// Ayanamsa
	double getAyanamsa( const bool &vedic ) const { return vedic ? iayanamsa : wayanamsa; }
	AYANAMSA getAyanamsaType( const bool &vedic ) const { return vedic ? iayanamsa_type : wayanamsa_type; }

	double getSiderealTime() const { return siderealTime; }

	// House stuff
	virtual double getHouse( const int& housenb, const bool& vedic, const bool getsandhi = false ) const;
	int getHousePos( const ObjectId& planet, const bool &vedic ) const;
	double getHouse4Longitude( const double& len, const bool &vedic ) const;


protected:

	FileDataSet *dataSet;
	double iayanamsa, wayanamsa, siderealTime;
	AYANAMSA iayanamsa_type, wayanamsa_type;
	double object_len[MAX_EPHEM_OBJECTS], object_lat[MAX_EPHEM_OBJECTS], object_speed[MAX_EPHEM_OBJECTS];
	double whousecusp[12], ihousecusp[12], ihousesandhi[12];
	PlanetContext context;

	/**
	 * Method for calculation of planetary positions. Must be called on every change of chart data.
	 * Is called from Horoscope::update().
	 */
	void updatePlanets();

	void updatePositionArrays();

private:
	ObjectPosition vpos[MAX_EPHEM_OBJECTS], wpos[MAX_EPHEM_OBJECTS];
	DECLARE_CLASS( BasicHoroscope )
};


#endif


