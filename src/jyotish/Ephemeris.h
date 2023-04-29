/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Ephemeris.h
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

#ifndef _EPHEMERIS_H_
#define _EPHEMERIS_H_

#include <wx/string.h>
#include <list>
#include <vector>

#include "maitreya.h"
#include "GenericDasa.h"

class ChartProperties;
class DasaExpert;
class DataSet;
class Sheet;

#define MAX_DAY 32

/*************************************************//**
*
* \brief holds data for an ingress event in EphemView
*
******************************************************/
class IngressEvent
{
public:
	IngressEvent( double _jd, ObjectId _planet, int _which, int _type )
	{
		jd = _jd;
		planet = _planet;
		which = _which;
		type = _type;
	}

	double jd;
	ObjectId planet;
	int which, type;
};

/*************************************************//**
*
* \brief holds data for a Lunar event in EphemView
*
******************************************************/
struct LunarEvent
{
	LunarEvent( const double &a, const double &j, const double& s, const double &m  )
	{
		angle = a;
		jd  = j;
		slen = s;
		mlen = m;
	}
	double angle, jd, mlen, slen;
};

/*************************************************//**
*
* \brief planetary data for a single planet in Ephemeris
*
******************************************************/
struct EphemPlanetData
{
	EphemPlanetData( const ObjectId &i );

	ObjectId pindex;

	bool retro[MAX_DAY];
	double len[MAX_DAY], speed[MAX_DAY];
	Rasi rasi[MAX_DAY];
	Nakshatra nakshatra[MAX_DAY];
};

/*************************************************//**
*
* \brief encapsulates calculation of Ephemeris in EphemView
*
******************************************************/
class EphemExpert
{
public:

	EphemExpert( const ChartProperties* );
	~EphemExpert();

	void setShowHeader( const bool b = true ) { show_header = b; }

	void setPreferVedic( const bool& );
	void prepareMonth( const int &month, const int &year, const bool &isLocaltime );

	int getMonth() { return month; }
	int getYear() { return year; }
	bool testDayIndexForCurrent( const int& );
	bool isCurrentMonth();

	int writeIngress( Sheet* );
	void writeKp( Sheet*, const DasaId& );
	void writeDetails( Sheet* );
	int writeLongitudes( Sheet* );
	void writeLunar( Sheet* );

	double getLongitude( const int &, const int &day );
	double getSpeed( const int &, const int &day );
	bool getRetro( const int &, const int &day );
	int calcMonth();
	int getNumberOfDays() const { return nb_days; }

	bool getPlanetRetro( const uint &plistindex, const int &day ) const;
	double getPlanetLongitude( const uint &plistindex, const int &day ) const;
	double getPlanetSpeed( const uint &plistindex, const int &day ) const;
	int getPlanetRasi( const uint &plistindex, const int &day ) const;
	int getPlanetNakshatra( const uint &plistindex, const int &day ) const;
	ObjectId getPlanetId( const uint &plistindex ) const;
	uint getPlanetdataSize() const { return planetdata.size(); }

private:
	void init();
	int calcLongitude();
	void calcDetails();
	int calcIngress();
	void calcKP( const DasaId&, DasaExpert* );
	void calcLunar();

	void testIngressEvent( const int &t1, const int &t2, const ObjectId &planet, const int &type, const bool &r );
	void writeHeaderInfo( Sheet* );

	void addLunarEvent( const int &stdate, const double& target, const double& sdiff, const double &ediff );
	void addLunarSpecialEvent( const int &stdate, const double& target, const double& sdiff, const double &ediff );

	const ChartProperties *chartprops;
	DataSet *d;
	std::vector<EphemPlanetData> planetdata;
	int nb_days, year, month, current_day, current_month, current_year;
	bool clen, cingress, ckp, cdetails, clunar, isLocaltime;
	bool dstchange;
	double jd[MAX_DAY], st[MAX_DAY], sunrise[MAX_DAY], sunset[MAX_DAY];
	int weekday[MAX_DAY], tithi[MAX_DAY];
	DasaId mydasa;

	std::vector<KPEvent> kpevents;
	std::list<IngressEvent> ingressEvents;
	std::list<LunarEvent> lunarEvents;

	bool show_header;
	const static wxChar *month_name[12];
};

#endif


