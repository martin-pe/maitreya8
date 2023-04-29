/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Aspect.h
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
#ifndef _ASPECT_H_
#define _ASPECT_H_

#include <list>
#include <wx/colour.h>
#include <wx/string.h>

#include "AspectConfig.h"
#include "maitreya.h"
#include "Expert.h"

class ChartProperties;
class Horoscope;
class Table;
class Sheet;

/*************************************************//**
*
* \brief holds data an event in aspect calculation
*
******************************************************/
class AspectEvent
{
public:
	AspectEvent( const ObjectId &p1, const ObjectId &p2, const ASPECT_TYPE &type, const double &orb )
	{
		planet1 = p1;
		planet2 = p2;
		field1 = field2 = -1;
		aspectType = type;
		orbis = orb;
		strength = 0;
		benefic = false;
		swapCharts = false;
	}

	AspectEvent( const int &f1, const int &f2, const ASPECT_TYPE &type, const int &s, const bool &b, const int &swap )
	{
		planet1 = ONONE;
		field1 = f1;
		planet2 = ONONE;
		field2 = f2;
		aspectType = type;
		orbis = 0;
		strength = s;
		benefic = b;
		swapCharts = swap;
	}

	ObjectId planet1, planet2;
	int field1, field2;
	ASPECT_TYPE aspectType;
	double orbis;
	int strength;
	bool benefic;
	bool swapCharts;
};

/*************************************************//**
*
* \brief encapsulates calculation of aspects for a given chart
*
******************************************************/
class AspectExpert : public Expert
{
public:

	AspectExpert();
	AspectExpert( Horoscope *h, Horoscope *h2 );

	void setHoroscopes( Horoscope*, Horoscope* );

	void clear();
	std::list<AspectEvent> &updateWesternAspects( ChartProperties*, const ChartType& );
	std::list<AspectEvent> &updateVedicRasiChartAspects( ChartProperties*, const Varga&, const bool &swapCharts );
	std::list<AspectEvent> &updateSbcAspects( ChartProperties*, const bool &swapCharts );

	void setSortOrder( const ASPECT_SORTORDER &order ) { sortorder = order; }
	void sort();

	static wxString getAspectName( const int&i );
	static wxString getAspectDescription( const int&i );
	static wxString getAspectShortDescription( const int&i );

	std::list<AspectEvent> &getAspectList() { return al; }
	void writeWesternAspectList( Sheet*, ChartProperties*, const ChartType&, const bool show_header = true );

	Table *getWesternAspectariumTable( Sheet*, ChartProperties* );
	void writeWesternAspectarium( Sheet*, ChartProperties*, const bool show_header = true  );

	void writeVedicAspectarium( Sheet*, ChartProperties*, const ChartType&, const bool show_header = true );

private:

	ASPECT_SORTORDER sortorder;
	std::list<AspectEvent> al;
	Horoscope *horoscope2;

};

#endif

