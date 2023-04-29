/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/KalachakraDasa.h
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
#ifndef _KALACHAKRADASA_H_
#define _KALACHAKRADASA_H_

#include "Dasa.h"
#include "maitreya.h"

class Horoscope;

enum KalachakraGroup { KG_ASWINI = 0, KG_BHARANI, KG_ROHINI, KG_MRIGASIRA };
enum KalachakraGati { KdgDefault = 0, KdgMandooka, KdgMarkati, KdgSimhavalokan };

#define ASSERT_VALID_KALACHAKRA_GROUP( g ) assert( g >= KG_ASWINI && g <= KG_MRIGASIRA );
#define IS_SAVYA_GROUP( g ) ( g == KG_ASWINI || g == KG_BHARANI )

/*************************************************//**
*
* \brief implementation of Dasa class for Kalachakra
*
******************************************************/
class KalachakraDasa : public Dasa
{
public:

	KalachakraDasa( const int &lord, const double &startjd, const double &endjd, const int &pada,
		const KalachakraGroup &group, const KalachakraGati &gati, Dasa* = (Dasa*)NULL );

	KalachakraGroup getGroup() const { return group; }
	int getPada() const { return pada; }
	bool isWrap() const { return gati != KdgDefault; }
	KalachakraGati getGati() const { return gati; }

protected:
	const int pada;
	const KalachakraGroup group;
	const KalachakraGati gati;
};

/*************************************************//**
*
* \brief expert class for Kalachakra encapsulates calculation
*
******************************************************/
class KalachakraDasaExpert : public DasaExpert
{
public:

	KalachakraDasaExpert( Horoscope* );
	virtual std::vector<Dasa*> getFirstLevel();
	virtual std::vector<Dasa*> getNextLevel( Dasa* );

	virtual KpData getKPLords( const double& );
	virtual bool hasKpFeatures() const { return true; }
	virtual bool isGrahaDasaExpert() const { return false; }

	virtual int getParamayus() const { return paramayus; }

	virtual wxString getDasaLordName( const int &lord, const TEXT_FORMAT&, WriterConfig* = (WriterConfig*)NULL );
	virtual MString getDasaLordNameF( const int &lord, const TEXT_FORMAT&, WriterConfig* = (WriterConfig*)NULL );

	virtual wxString getDasaDescription( Dasa*, const TEXT_FORMAT, WriterConfig* = (WriterConfig*)NULL );
	virtual MString getDasaDescriptionF( Dasa*, const TEXT_FORMAT, WriterConfig* = (WriterConfig*)NULL );

	virtual void writeDasaHeader( Sheet*, const DasaId&, Dasa *dasa );

	bool isSavya() const { return savya; }
	Rasi getAmsa() const { return amsa; }

	Rasi getDeha() const { return deha; }
	Rasi getJeeva() const { return jeeva; }

	virtual std::vector<KPEvent> getKPEventList( const double &startlen, const double &endlen, const double &startjd );

private:
	Rasi deha, jeeva;
	bool savya;
	Rasi amsa;
	int paramayus;

	bool isMarkatiField( const int &group, const int &pada, const int &column );
	KalachakraGati calculateGati( const Rasi &ad_lord, const Rasi &oldlord, const bool &markatiField );

	int getKPLordRecursive( const int &group, const int &pada, int &lord_index, double &elapsed, double &total_len );

	static const Rasi K_KALA[4][4][9];
	static const int K_RASIYEARS[12];
	static const int K_TOTALYEARS[4][4];
	static const KalachakraGroup K_NAKSHATRA_GROUP[27];
	static const Rasi K_APSAVYA_AMSA[12];
};

#endif

