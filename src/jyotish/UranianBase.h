/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/UranianBase.h
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
#ifndef _URANIANBASE_H_
#define _URANIANBASE_H_

#include "maitreya.h"
#include "Sheet.h"

#include <list>
#include <wx/dynarray.h>
#include <wx/string.h>

class Horoscope;
class UranianExpert;
class UEvent;

enum UWRITE_FORMAT { UF_DEFAULT, UF_CONTEXT };
enum LONGITUDE_TYPE { LT_DEFAULT, LT_ANTISCIA };
enum UENTITY_TYPE { UET_OBJECT, UET_MIDPOINT, UET_SUM, UET_DIFFERENCE, UET_REFLECTION_POINT, UET_SENSITIVE_POINT };

/*************************************************//**
*
* 
*
******************************************************/
class UEntity : public wxObject
{
	DECLARE_CLASS( UEntity )

public:
	UEntity( const double &longitude, const PlanetContext& context, const LONGITUDE_TYPE lttype = LT_DEFAULT )
	 : longitude( longitude ),
	  context( context ), lttype( lttype )
	{}

	virtual ~UEntity() {}
	virtual UEntity *clone() = 0;

	virtual MString write( const UWRITE_FORMAT = UF_DEFAULT ) const = 0;
	virtual UENTITY_TYPE getUeType() const = 0;
	virtual bool applyFilter( const ObjectFilter& ) = 0;

	virtual bool isObject() const { return false; }
	virtual bool isPair() const { return false; }
	virtual bool isTriple() const { return false; }

	virtual ObjectId getFirstObject() const = 0;

	const double longitude;
	const PlanetContext context;
	const LONGITUDE_TYPE lttype;
};

/*************************************************//**
*
* 
*
******************************************************/
class UObject : public UEntity
{
	DECLARE_CLASS( UObject )

public:
	UObject( const double &longitude, const PlanetContext& ctx, const ObjectId& p, const LONGITUDE_TYPE lttype = LT_DEFAULT );
	virtual UEntity *clone();

	virtual bool isObject() const { return true; }

	virtual MString write( const UWRITE_FORMAT = UF_DEFAULT ) const;

	virtual UENTITY_TYPE getUeType() const { return UET_OBJECT; }

	virtual bool applyFilter( const ObjectFilter &filter )
	{
		return ( filter.size() == 0 || filter.find( p ) != filter.end() );
	}

	virtual ObjectId getFirstObject() const { return p; }

	const ObjectId p;
};

/*************************************************//**
*
* 
*
******************************************************/
class UObjectPair : public UEntity
{
	DECLARE_CLASS( UObjectPair )

public:
	UObjectPair( const double &longitude, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2,
		const wxChar operatorSymbol, const LONGITUDE_TYPE lttype = LT_DEFAULT );

	virtual bool isPair() const { return true; }

	virtual bool applyFilter( const ObjectFilter &filter )
	{
		return ( filter.size() == 0 || filter.find( p1 ) != filter.end() || filter.find( p2 ) != filter.end() );
	}
	virtual MString write( const UWRITE_FORMAT = UF_DEFAULT ) const;

	virtual ObjectId getFirstObject() const { return p1; }

	const ObjectId p1, p2;
	const wxChar operatorSymbol;
};

/*************************************************//**
*
* 
*
******************************************************/
class UMidpoint : public UObjectPair
{
public:
	UMidpoint( const double &longitude, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2, const LONGITUDE_TYPE lttype = LT_DEFAULT )
	: UObjectPair( longitude, ctx, p1, p2, '/', lttype ) {}

	virtual UEntity *clone();

	virtual UENTITY_TYPE getUeType() const { return UET_MIDPOINT; }
};

/*************************************************//**
*
* 
*
******************************************************/
class USum : public UObjectPair
{
public:
	USum( const double &len, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2, const LONGITUDE_TYPE lttype = LT_DEFAULT )
	: UObjectPair( len, ctx, p1, p2, '+', lttype ) {}

	virtual UEntity *clone();

	virtual UENTITY_TYPE getUeType() const { return UET_SUM; }
};

/*************************************************//**
*
* 
*
******************************************************/
class UDifference : public UObjectPair
{
public:
	UDifference( const double &len, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2, const LONGITUDE_TYPE lttype = LT_DEFAULT )
	: UObjectPair( len, ctx, p1, p2, '-', lttype ) {}

	virtual UEntity *clone();

	virtual UENTITY_TYPE getUeType() const { return UET_DIFFERENCE; }
};

/*************************************************//**
*
* 
*
******************************************************/
class UReflectionPoint : public UObjectPair
{
public:
	UReflectionPoint( const double &len, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2, const LONGITUDE_TYPE lttype = LT_DEFAULT )
	: UObjectPair( len, ctx, p1, p2, '*', lttype ) {}

	virtual UEntity *clone();

	virtual MString write( const UWRITE_FORMAT = UF_DEFAULT ) const;

	virtual UENTITY_TYPE getUeType() const { return UET_REFLECTION_POINT; }
};

/*************************************************//**
*
* 
*
******************************************************/
class UObjectTriple : public UObjectPair
{
public:
	UObjectTriple( const double &longitude, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2, const ObjectId& p3, const LONGITUDE_TYPE lttype = LT_DEFAULT );

	virtual bool isPair() const { return false; }
	virtual bool isTriple() const { return true; }

	virtual bool applyFilter( const ObjectFilter &filter )
	{
		return ( filter.size() == 0 || filter.find( p1 ) != filter.end() || filter.find( p2 ) != filter.end() || filter.find( p3 ) != filter.end() );
	}

	const ObjectId p3;
};

/*************************************************//**
*
* 
*
******************************************************/
class USensitivePoint : public UObjectTriple
{
public:
	USensitivePoint( const double &len, const PlanetContext& ctx, const ObjectId& p1, const ObjectId& p2, const ObjectId& p3, const LONGITUDE_TYPE lttype = LT_DEFAULT )
	: UObjectTriple( len, ctx, p1, p2, p3, lttype ) {}

	virtual UEntity *clone();

	virtual UENTITY_TYPE getUeType() const { return UET_SENSITIVE_POINT; }

	virtual MString write( const UWRITE_FORMAT = UF_DEFAULT ) const;
};

/*************************************************//**
*
* 
*
******************************************************/
struct UranianMatch
{
	UranianMatch( const GRADKREIS gradkreis = GK_NONE, const double orbis = 0, const double residuum = 0 )
	 : gradkreis( gradkreis ),
	 	orbis( orbis ),
		residuum( residuum )
	{}

	GRADKREIS gradkreis;
	double orbis;
	double residuum;
};

/*************************************************//**
*
* 
*
******************************************************/
class UEvent
{
public:
	UEvent( UEntity *e1, UEntity *e2, const GRADKREIS &gradkreis, const double &orbis, const double &residuum, const double jd = 0 );
	UEvent( UEntity *e1, UEntity *e2, const UranianMatch&, const double jd = 0 );
	UEvent( const UEvent& );
	~UEvent();

	virtual MString write( const UWRITE_FORMAT = UF_DEFAULT ) const;

	virtual bool applyFilter( const ObjectFilter &filter )
	 { return e1->applyFilter( filter ) || e2->applyFilter( filter ); }

	bool isDoubleObjectEvent() const { return e1->isObject() && e2->isObject(); }
	bool isObjectAndPairEvent() const { return ( e1->isObject() && e2->isPair() ) || ( e2->isObject() && e1->isPair() ); }
	bool isObjectAndTripleEvent() const { return ( e1->isObject() && e2->isTriple() ) || ( e2->isObject() && e1->isTriple() ); }
	bool isDoublePairEvent() const { return  e1->isPair() && e2->isPair(); }

	UObject *getObjectEntity() const
	{
		if ( e1->getUeType() == UET_OBJECT ) return (UObject*)e1;
		else if ( e2->getUeType() == UET_OBJECT ) return (UObject*)e2;
		else assert( false );
	}

	UObjectPair *getObjectPairEntity() const
	{
		if ( e1->isPair()) return (UObjectPair*)e1;
		else if ( e2->isPair()) return (UObjectPair*)e2;
		else assert( false );
	}

	UObjectTriple *getObjectTripleEntity() const
	{
		if ( e1->isTriple()) return (UObjectTriple*)e1;
		else if ( e2->isTriple()) return (UObjectTriple*)e2;
		else assert( false );
	}

	UEntity *e1, *e2;
	const GRADKREIS gradkreis;
	const double orbis;
	const double residuum;
	double duedate;
};

/*************************************************//**
*
* 
*
******************************************************/
class ClusterMatchingList
{
public:

	ClusterMatchingList( const PlanetContext ctx = PcRadix )
	 : rootContext( ctx )
	{
		oneContext = true;
	}

	PlanetContext rootContext;
	std::vector<UEntity*> matchings;
	bool oneContext;

};

/*************************************************//**
*
* 
*
******************************************************/
class UranianTool
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( UranianTool );
public:

	double getDegrees4Gradkreis( const GRADKREIS& );

	double getMultiplicator4Gradkreis( const GRADKREIS& );

	wxString gradkreis2String( const GRADKREIS &gradkreis );

	ASPECT_TYPE gradkreis2AspectCode( const GRADKREIS &gk ) const;

	wxChar getUranianAspectCode( const GRADKREIS &gradkreis ) const;

private:
	UranianTool();
};

/*************************************************//**
*
* 
*
******************************************************/
class UranianCalculationResult
{
public:
	UranianCalculationResult( UranianExpert *expert, Horoscope *horoscope );

	~UranianCalculationResult()
	{
	}

	std::list<UEvent> uevents;
	void clear();

	UranianExpert *expert;
	Horoscope *horoscope;

private:
};

/*************************************************//**
*
* 
*
******************************************************/
class UEventSorter
{
public:
	UEventSorter( const ASPECT_SORTORDER &order, const bool &vedic ) : order( order ), vedic( vedic ) {}

	bool operator()( const UEvent&, const UEvent& ) const;

	const ASPECT_SORTORDER order;

	// unused
	const bool vedic;

private:
};

#endif

