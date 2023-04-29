/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/UranianHelper.h
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
#ifndef _URANIANHELPER_H_
#define _URANIANHELPER_H_

#include "UranianBase.h"
#include "Conf.h"
#include "maitreya.h"

#include <list>
#include <map>
#include <vector>

class ChartProperties;
class Horoscope;
class Table;
class TransitExpert;
class UranianExpert;
class Sheet;

/*************************************************//**
*
* \brief
*
******************************************************/
class ConjunctionTextItem
{
public:
	ConjunctionTextItem( const int &p1, const int &p2, wxString text )
			: p1 ( p1 ), p2( p2 ), text( text )
	{}

	const int p1, p2;
	wxString text;
};

/*************************************************//**
*
* \brief
*
******************************************************/
class MidpointTextItem
{
public:
	MidpointTextItem( const int &p, const int &p1, const int &p2, wxString text )
			: p( p ), p1 ( p1 ), p2( p2 ), text( text )
	{}

	const int p, p1, p2;
	wxString text;
};

/*************************************************//**
*
* \brief
*
******************************************************/
class UranianTextLoader
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( UranianTextLoader )

public:
	~UranianTextLoader();

	void cleanup();

	MidpointTextItem *getMidpointTextItem( const int &p, const int &p1, const int &p2, const int &mode );
	ConjunctionTextItem *getConjunctionTextItem( const int &p1, const int &p2, const int &mode );

	wxString getText4UEvent( const UEvent&, const bool witteMode );

	bool hasItemsForMode( const int &mode );

private:
	UranianTextLoader();

	int getTextItem( wxString s );
	bool loadItems( const int &mode );

	std::map<wxString, ObjectId>  wobjects;
	std::vector<MidpointTextItem*> midpoint_e, midpoint_w;
	std::vector<ConjunctionTextItem*> conjunction_w;
};

enum URANIAN_TUPLE_TYPE { UTT_SUMS, UTT_DIFFERENCES, UTT_MIDPOINTS, UTT_ANT_MIDPOINTS, UTT_REFLECTION_POINTS };

/*************************************************//**
*
* \brief encapsulates report writing in Uranian astrology
*
******************************************************/
class UranianHelper
{
public:

	UranianHelper( UranianExpert* );

	void writeTupleTable( Sheet*, const URANIAN_TUPLE_TYPE&, const PlanetContext& );
	Table *createTupleTable( const URANIAN_TUPLE_TYPE&, const PlanetContext& );

	void writeSensitivePoints( Sheet*, const PlanetContext& );
	void writeTextAnalysis( Sheet*, const PlanetContext& );
	void writeSpecialPoints( Sheet*, const PlanetContext& );
	void writeUEvents( Sheet*, const PlanetContext&, const bool showDate = false );

	void writeMatchingClusters( Sheet*, const PlanetContext& );
	void writeMatchingClusterPart( Sheet *sheet, const PlanetContext &ctx,
		std::map<ObjectId, ClusterMatchingList> &m, const uint &istart, const uint &iend );

	void writeYearlyPreview( Sheet* );

	void writeTransits( Sheet*, TransitExpert* );
	void writePartner( Sheet* );

	void writeRadixAnalysis( Sheet* );

private:

	bool isOneContext( std::list<UEvent>&, const PlanetContext& );

	UranianExpert *expert;

};


#endif

