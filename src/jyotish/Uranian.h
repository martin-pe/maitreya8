/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Uranian.h
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
#ifndef _URANIAN_H_
#define _URANIAN_H_

#include <map>

#include "Expert.h"
#include "Conf.h"
#include "Horoscope.h"
#include "UranianBase.h"

class ChartProperties;
class ObjectEvaluation;
class TransitHoroscope;
class UranianHelper;

/*************************************************//**
*
* \brief Horoscope subclass providing shifted zodiac
*
******************************************************/
class UranianHoroscope : public Horoscope
{
public:

	//UranianHoroscope( Horoscope*, const ObjectId = ONONE );
	UranianHoroscope( const int mode = 0 );
	//void setReferenceObject( const ObjectId &o ) { refobj = o; }
	void setMode( const int m ) { mode = m; }

	virtual ObjectPosition getObjectPosition( const ObjectId &planet, const bool &vedic ) const;

protected:

	//Horoscope *horoscope;
	//ObjectId refobj;
	int mode;

	DECLARE_CLASS( UranianHoroscope )
};

/*************************************************//**
*
* \brief encapsulates calculations in Uranian astrology
*
******************************************************/
class UranianExpert : public Expert
{
public:

	UranianExpert( ChartProperties* );
	UranianExpert( Horoscope *h, ChartProperties *chartprops );
	~UranianExpert();

	void initPartner( Horoscope*, Horoscope* );

	UranianCalculationResult *planetContext2Result( const PlanetContext& );

	void OnDataChanged();

	void calculateTransit( Horoscope*, const PlanetContext& );

	void calculatePartner( Horoscope*, Horoscope* );

	void calculateYearlyPreview( const int &year );

	void calculatePeriodPreview( const double &first_jd, const double &last_jd );

	std::map<ObjectId, ClusterMatchingList> createClusterMatchingList( const PlanetContext& );

	int getPreviewYear() const { return previewYear; }

	ChartProperties *getChartProperties() { return chartprops; }

private:

	void reset();
	void init();
	void initRadix();
	void initResultContext( Horoscope*, const PlanetContext& );

	void initPeriod();

	/*************************************************//**
	*
	* main method for match calculation.
	* params
	* - PlanetContext ctx: primary context, e.g. the radix horoscope or first partner
	* - PlanetContext refctx: secondary context, e.g. transit or second partner
	* - PlanetContext storectx: context for result storage
	* - orbis real value e.g. 1.5 for 90 min
	*
	* result will be stored in the UranianCalculationResult pointers vtransit etc.
	*
	******************************************************/
	void findMatchingEvents(
		const PlanetContext &ctx,
		const PlanetContext &refctx,
		const PlanetContext &storectx,
		const double &orbis );

	UranianMatch calculateMatch( const double &len1, const double &len2, const double &orbis );

	void calculateUEventDueDate( UEvent&, const double &sunpos );

	ChartProperties *chartprops;

	bool show_header;

	int previewYear;

	double year_length;

	class Calculator *calculator;

	UranianCalculationResult *result[MAX_PLANET_CONTEXT];

	TransitHoroscope *hperiod;
	Horoscope *hantiscia;
};

#endif

