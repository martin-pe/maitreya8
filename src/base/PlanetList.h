/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PlanetList.h
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
#ifndef _PLANETLIST_H_
#define _PLANETLIST_H_

#include <vector>
#include "maitreya.h"

bool isObjectIdLessThan( const ObjectId&, const ObjectId&, const bool &vedic );

/*************************************************//**
*
* \brief creates lists of planets for Vedic and western astrology
*
******************************************************/
class PlanetList
{
public:
	ObjectArray getWesternObjectList( const OBJECT_INCLUDES &style );
	ObjectArray getVedicObjectList( const OBJECT_INCLUDES &style );
	ObjectArray getVedicObjectListWithoutLagna( const OBJECT_INCLUDES &style );

	void dumpObjectStyle( const OBJECT_INCLUDES &style );

	ObjectArray getDefaultVedicObjectList();
	ObjectArray getDefaultVedicObjectListWithLagna();
	ObjectArray getVedicPlanetOnlyList();

private:
	void addOuterObjectList( const OBJECT_INCLUDES &style, const bool &vedic, ObjectArray& );
	void addHouses( const OBJECT_INCLUDES &style, ObjectArray& );
	ObjectArray getVedicObjectListInternal( const OBJECT_INCLUDES &style, const bool &withLagna );

};

#endif


