/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PlanetList.h
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
#ifndef _PLANETLIST_H_
#define _PLANETLIST_H_

#include <vector>
#include "maitreya.h"

using namespace std;

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


