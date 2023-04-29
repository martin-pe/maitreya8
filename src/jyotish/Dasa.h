/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Dasa.h
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
#ifndef _DASA_H_
#define _DASA_H_

#include "Expert.h"
// kpdata
#include "Horoscope.h"
#include "Sheet.h"

#include <vector>
#include <wx/string.h>

class DasaExpert;
class Horoscope;
class WriterConfig;

/*************************************************//**
*
* \brief this class encapsulates a Dasa with its Lord, start, end and parent Dasa
*
******************************************************/
class Dasa
{
public:

	Dasa( const DasaId &dasaId, const int &lord, const double &startjd, const double &endjd, Dasa *parent )
		: dasaId( dasaId ),
		lord( lord ),
		start_jd( startjd ),
		end_jd( endjd ),
		parent( parent )
	{
		level = parent ? parent->getLevel() + 1 : 0;
	}

	virtual ~Dasa()
	{
		//printf( "Destructor Dasa\n" );
	}

	DasaId getDasaId() const { return dasaId; }

	int getLevel() const { return level; }
	void setLevel( const int &l ) { level = l; }

	double getStartJD() const { return start_jd; }
	double getEndJD() const { return end_jd; }

	int getDasaLord() const { return lord; }

	Dasa *getParent() const { return parent; }
	void setParent( Dasa *d ) { parent = d; }

protected:

	const DasaId dasaId;
	const int lord;
	const double start_jd;
	const double end_jd;
	Dasa *parent;
	int level;
};

/*************************************************//**
*
* \brief event in KP calculation (Ephemeris view)
*
******************************************************/
class KPEvent
{
public:
	KPEvent( const int &l, const int &sl, const double &ll, const double &jjd, const int &index )
		: lord( l ),
		sublord( sl ),
		len( ll ),
		jd( jjd ),
		dasaindex( index )
	{}

	int lord, sublord;
	double len;
	double jd;
	int dasaindex;
};

/*************************************************//**
*
* \brief abstract class encapsulating the calculation of Dasas
*
******************************************************/
class DasaExpert : public Expert
{
public:

	DasaExpert( const DasaId &dasaId, Horoscope *h ) : Expert( h ), dasaId( dasaId ) {}
	virtual ~DasaExpert() {}

	DasaId getDasaId() const { return dasaId; }

	virtual std::vector<Dasa*> getFirstLevel() = 0;
	virtual std::vector<Dasa*> getNextLevel( Dasa* ) = 0;

	virtual bool isGrahaDasaExpert() const = 0;
	bool isRasiDasaExpert() const  { return ! isGrahaDasaExpert(); }

	virtual int getParamayus() const = 0;

	int getBirthMahaDasaLord();

	virtual void writeDasaHeader( Sheet*, const DasaId&, Dasa *dasa );

	wxString getDasaLordName( Dasa *dasa, const TEXT_FORMAT&, WriterConfig* = (WriterConfig*)NULL );
	virtual wxString getDasaLordName( const int&, const TEXT_FORMAT&, WriterConfig* = (WriterConfig*)NULL );

	MString getDasaLordNameF( Dasa *dasa, const TEXT_FORMAT &format, WriterConfig* = (WriterConfig*)NULL );
	virtual MString getDasaLordNameF( const int&, const TEXT_FORMAT&, WriterConfig* = (WriterConfig*)NULL );

	virtual wxString getDasaDescription( Dasa*, const TEXT_FORMAT, WriterConfig* = (WriterConfig*)NULL );
	virtual MString getDasaDescriptionF( Dasa*, const TEXT_FORMAT, WriterConfig* = (WriterConfig*)NULL );

	virtual KpData getKPLords( const double& ) = 0;
	virtual bool hasKpFeatures() const { return false; }
	virtual std::vector<KPEvent> getKPEventList( const double &startlen, const double &Endlen, const double &startjd ) = 0;

protected:
	const DasaId dasaId;
};

/*************************************************//**
*
* \brief encapsulates the creation of Dasa Experts
*
******************************************************/
class DasaExpertFactory
{
public:

	DasaExpert *getDasaExpert( const DasaId&, Horoscope* = (Horoscope*)NULL );

private:

	DasaExpert *getVimsottariDasaExpert( Horoscope* );
	DasaExpert *getYoginiDasaExpert( Horoscope* );
	DasaExpert *getKalachakraDasaExpert( Horoscope* );
	DasaExpert *getAshtottariDasaExpert( Horoscope* );
	DasaExpert *getShodshottariDasaExpert( Horoscope* );
	DasaExpert *getDvadashottariDasaExpert( Horoscope* );
	DasaExpert *getPanchottariDasaExpert( Horoscope* );
	DasaExpert *getShatabdikaDasaExpert( Horoscope* );
	DasaExpert *getChaturashitiSamaDasaExpert( Horoscope* );
	DasaExpert *getDvisaptatiSamaDasaExpert( Horoscope* );
	DasaExpert *getShatTrimshatSamaDasaExpert( Horoscope* );
	DasaExpert *getLagnaVimsottariDasaExpert( Horoscope* );

};

#endif

