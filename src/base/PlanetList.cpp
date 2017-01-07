/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PlanetList.cpp
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

#include "PlanetList.h"

#include "Conf.h"
#include <wx/log.h>

extern Config *config;

/*****************************************************
**
**   PlanetList   ---   addOuterObjectList
**
******************************************************/
void PlanetList::addOuterObjectList( const OBJECT_INCLUDES &style, const bool &vedic, ObjectArray &r )
{
	ObjectId i;
	uint j;
	for ( i = OCUPIDO; i <= OLILITH; i++ )
	{
		if ( ! ( style & OI_URANIAN_INNER ) && i >= OCUPIDO && i <= OKRONOS ) continue;
		if ( ! ( style & OI_URANIAN_OUTER ) && i >= OAPOLLON && i <= OPOSEIDON ) continue;
		if ( ! ( style & OI_CHIRON ) && i == OCHIRON ) continue;
		if ( ! ( style & OI_PHOLUS ) && i == OPHOLUS ) continue;
		if ( ! ( style & OI_PLANETOIDS ) && i >= OCERES && i <= OVESTA ) continue;
		if ( ! ( style & OI_LILITH ) && i == OLILITH ) continue;
		r.push_back( i );
	}

	// houses are handled separately
	for ( i = FIRST_EXTENDED_OBJECT; i <= LAST_EXTENDED_OBJECT; i++ )
	{
		if ( i == OARIES && ( ! ( style & OI_ARIES ) || config->western->planetOrder == 1 )) continue;
		if ( ! ( style & OI_UPAGRAHAS ) && i >= ODHUMA && i <= OUPAKETU ) continue;
		if ( i >= OKALA && i <= OMANDI )
		{
			if ( ! ( style & OI_KALAVELAS )) continue;
			else if ( config->vedicCalculation->kalavela.lord[i-OKALA] == KALAVELA_LORD_NONE ) continue;
		}
		if ( i >= OBHAVA_LAGNA && i <= OGHATIKA_LAGNA  && ! ( style & OI_SPECIALLAGNAS )) continue;
		if ( i == OD9LAGNA && ( ! ( style & OI_D9_LAGNA ) || ! ( style & OI_D9_LAGNA ))) continue;
		//if ( IS_HOUSE_OBJECT( i ) && ! ( style & OI_ALL_HOUSES )) continue;

		r.push_back( i );
	}

	dumpObjectStyle( style );
	if ( style & OI_ARABICPARTS )
	{
		for ( j = 0; j < ( vedic ? config->vedic->arabicParts.size() :  config->western->arabicParts.size()); j++ )
		{
			r.push_back( OFORTUNE + ( vedic ? config->vedic->arabicParts[j] : config->western->arabicParts[j] ));
		}
	}
}

/*****************************************************
**
**   PlanetList   ---   addHouses
**
******************************************************/
void PlanetList::addHouses( const OBJECT_INCLUDES &style, ObjectArray &r )
{

	if ( style & OI_ALL_HOUSES )
	{
		for ( ObjectId i = OHOUSE1; i <= OHOUSE12; i++ )
		{
			r.push_back( i );
		}
	}

	if ( style & OI_4_HOUSES )
	{
		r.push_back( OHOUSE2 );
		r.push_back( OHOUSE3 );
		r.push_back( OHOUSE11 );
		r.push_back( OHOUSE12 );
	}
}

/*****************************************************
**
**   isObjectIdLessThan
**
******************************************************/
bool isObjectIdLessThan( const ObjectId &p1, const ObjectId &p2, const bool &vedic )
{
	if ( ! vedic && config->western->planetOrder == 1 )
	{
		if ( p1 == OMERIDIAN ) return p2 != OMERIDIAN;
		else if ( p2 == OMERIDIAN ) return false;
		else if ( p1 == OARIES ) return p2 != OARIES;
		else if ( p2 == OARIES ) return false;
		else if ( p1 == OSUN ) return p2 != OSUN;
		else if ( p2 == OSUN ) return false;
		else if ( p1 == OASCENDANT ) return p2 != OASCENDANT;
		else if ( p2 == OASCENDANT ) return false;
		else if ( p1 == OMOON ) return p2 != OMOON;
		else if ( p2 == OMOON ) return false;
		else if ( IS_ASC_NODE( p1 )) return ! IS_ASC_NODE( p2 );
		else if ( IS_ASC_NODE( p2 )) return false;
	}
	return p1 < p2;
}

/*****************************************************
**
**   PlanetList   ---   getWesternObjectList
**
******************************************************/
ObjectArray PlanetList::getWesternObjectList( const OBJECT_INCLUDES &style )
{
	ObjectArray r;
	ObjectId i;
	const bool istruenode = config->westernCalculation->lunarNodeMode == LUNAR_NODE_TRUE;

	if ( config->western->planetOrder == 1 )
	{
		if ( style & OI_MERIDIAN ) r.push_back( OMERIDIAN );
		if ( style & OI_ARIES && style & OI_ARIES ) r.push_back( OARIES );
		if ( style & OI_INNER ) r.push_back( OSUN );
		if ( style & OI_ASCENDANT ) r.push_back( OASCENDANT );
		if ( style & OI_INNER ) r.push_back( OMOON );
		if ( style & OI_DRAGONHEAD ) r.push_back( istruenode ? OTRUENODE : OMEANNODE );
		if ( style & OI_DRAGONTAIL ) r.push_back( istruenode ? OTRUEDESCNODE: OMEANDESCNODE );

		if ( style & OI_INNER ) for ( i = OMERCURY; i <= OSATURN; i++ ) r.push_back( i );
		if ( style & OI_OUTER )
		{
			r.push_back( OURANUS );
			r.push_back( ONEPTUNE );
			r.push_back( OPLUTO );
		}
		addOuterObjectList( style, false, r );
		if ( style & OI_DESCENDANT ) r.push_back( ODESCENDANT );
		if ( style & OI_IMUMCOELI ) r.push_back( OIMUMCOELI );
		addHouses( style, r );
	}
	else
	{
		if ( style & OI_INNER ) for ( i = OSUN; i <= OSATURN; i++ ) r.push_back( i );
		if ( style & OI_OUTER )
		{
			r.push_back( OURANUS );
			r.push_back( ONEPTUNE );
			r.push_back( OPLUTO );
		}
		if ( style & OI_DRAGONHEAD ) r.push_back( istruenode ? OTRUENODE : OMEANNODE );
		if ( style & OI_DRAGONTAIL ) r.push_back( istruenode ? OTRUEDESCNODE: OMEANDESCNODE );
		addOuterObjectList( style, false, r );
		if ( style & OI_ASCENDANT ) r.push_back( OASCENDANT );
		if ( style & OI_MERIDIAN ) r.push_back( OMERIDIAN );
		if ( style & OI_DESCENDANT ) r.push_back( ODESCENDANT );
		if ( style & OI_IMUMCOELI ) r.push_back( OIMUMCOELI );
		addHouses( style, r );
	}
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getDefaultVedicObjectList
**
******************************************************/
ObjectArray PlanetList::getDefaultVedicObjectList()
{
	ObjectArray r = getVedicPlanetOnlyList();
	if ( config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE )
	{
		r.push_back( OTRUENODE );
		r.push_back( OTRUEDESCNODE );
	}
	else
	{
		r.push_back( OMEANNODE );
		r.push_back( OMEANDESCNODE );
	}
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getVedicPlanetOnlyList
**
******************************************************/
ObjectArray PlanetList::getVedicPlanetOnlyList()
{
	ObjectArray r;
	r.push_back( OSUN );
	r.push_back( OMOON );
	r.push_back( OMARS );
	r.push_back( OMERCURY );
	r.push_back( OJUPITER );
	r.push_back( OVENUS );
	r.push_back( OSATURN );
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getVedicObjectListInternal
**
******************************************************/
ObjectArray PlanetList::getVedicObjectListInternal( const OBJECT_INCLUDES &style, const bool &withLagna )
{
	ObjectArray r;
	const bool istruenode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE;

	if (( style & OI_ASCENDANT ) && config->vedic->orderLagna == 1 && withLagna ) r.push_back( OASCENDANT );
	if ( style & OI_INNER )
	{
		r.push_back( OSUN );
		r.push_back( OMOON );
		r.push_back( OMARS );
		r.push_back( OMERCURY );
		r.push_back( OJUPITER );
		r.push_back( OVENUS );
		r.push_back( OSATURN );
	}
	if ( style & OI_OUTER )
	{
		r.push_back( OURANUS );
		r.push_back( ONEPTUNE );
		r.push_back( OPLUTO );
	}
	if ( style & OI_DRAGONHEAD )
	{
		r.push_back( istruenode ? OTRUENODE : OMEANNODE );
	}
	if ( style & OI_DRAGONTAIL )
	{
		r.push_back( istruenode ? OTRUEDESCNODE: OMEANDESCNODE );
	}

	addOuterObjectList( style, true, r );
	if (( style & OI_ASCENDANT ) && config->vedic->orderLagna == 0 && withLagna ) r.push_back( OASCENDANT );

	if ( style & OI_MERIDIAN ) r.push_back( OMERIDIAN );
	if ( style & OI_DESCENDANT ) r.push_back( ODESCENDANT );
	if ( style & OI_IMUMCOELI ) r.push_back( OIMUMCOELI );
	addHouses( style, r );
	return r;
}

/*****************************************************
**
**   PlanetList   ---   getVedicObjectListWithoutLagna
**
******************************************************/
ObjectArray PlanetList::getVedicObjectListWithoutLagna( const OBJECT_INCLUDES &style )
{
	return getVedicObjectListInternal( style, false );
}

/*****************************************************
**
**   PlanetList   ---   getVedicObjectList
**
******************************************************/
ObjectArray PlanetList::getVedicObjectList( const OBJECT_INCLUDES &style )
{
	return getVedicObjectListInternal( style, true );
}

/*****************************************************
**
**   PlanetList   ---   dumpObjectStyle
**
******************************************************/
void PlanetList::dumpObjectStyle( const OBJECT_INCLUDES &style )
{
	wxLogMessage( wxT( "PlanetList::dumpObjectStyle style %d " ), (int)style );

	wxLogMessage( wxT( " OI_INNER         %d " ), (bool)(style &  OI_INNER));
	wxLogMessage( wxT( " OI_OUTER         %d " ), (bool)(style &  OI_OUTER));
	wxLogMessage( wxT( " OI_DRAGONTAIL    %d " ), (bool)(style &  OI_DRAGONTAIL));
	wxLogMessage( wxT( " OI_DRAGONHEAD    %d " ), (bool)(style &  OI_DRAGONHEAD));
	wxLogMessage( wxT( " OI_ASCENDANT     %d " ), (bool)(style &  OI_ASCENDANT));
	wxLogMessage( wxT( " OI_MERIDIAN      %d " ), (bool)(style &  OI_MERIDIAN));
	wxLogMessage( wxT( " OI_DESCENDANT    %d " ), (bool)(style &  OI_DESCENDANT));
	wxLogMessage( wxT( " OI_IMUMCOELI     %d " ), (bool)(style &  OI_IMUMCOELI));
	wxLogMessage( wxT( " OI_URANIAN_INNER %d " ), (bool)(style &  OI_URANIAN_INNER));
	wxLogMessage( wxT( " OI_URANIAN_OUTER %d " ), (bool)(style &  OI_URANIAN_OUTER));
	wxLogMessage( wxT( " OI_CHIRON        %d " ), (bool)(style &  OI_CHIRON));
	wxLogMessage( wxT( " OI_PHOLUS        %d " ), (bool)(style &  OI_PHOLUS));
	wxLogMessage( wxT( " OI_PLANETOIDS    %d " ), (bool)(style &  OI_PLANETOIDS));
	wxLogMessage( wxT( " OI_LILITH        %d " ), (bool)(style &  OI_LILITH));
	wxLogMessage( wxT( " OI_ARIES         %d " ), (bool)(style &  OI_ARIES));
	wxLogMessage( wxT( " OI_KALAVELAS     %d " ), (bool)(style &  OI_KALAVELAS));
	wxLogMessage( wxT( " OI_UPAGRAHAS     %d " ), (bool)(style &  OI_UPAGRAHAS));
	wxLogMessage( wxT( " OI_SPECIALLAGNAS %d " ), (bool)(style &  OI_SPECIALLAGNAS));
	wxLogMessage( wxT( " OI_D9_LAGNA      %d " ), (bool)(style &  OI_D9_LAGNA));
	wxLogMessage( wxT( " OI_ARABICPARTS   %d " ), (bool)(style &  OI_ARABICPARTS));
	wxLogMessage( wxT( " OI_ALL_HOUSES    %d " ), (bool)(style &  OI_ALL_HOUSES));
	wxLogMessage( wxT( " OI_4_HOUSES      %d " ), (bool)(style &  OI_4_HOUSES));
}

