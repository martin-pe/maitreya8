/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/GenericDasa.h
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

#ifndef _GENERICDASA_H_
#define _GENERICDASA_H_

#include "maitreya.h"
#include "Dasa.h"

class Horoscope;
class Sheet;

/*************************************************//**
*
* \brief expert class for generic Dasas (following the pattern of Vimsottari, Yogini etc.)
*
******************************************************/
class GenericNakshatraDasaExpert : public DasaExpert
{
public:

	GenericNakshatraDasaExpert( const DasaId &dasa, Horoscope *h ): DasaExpert( dasa, h ) {}

	virtual vector<Dasa*> getFirstLevel();
	virtual vector<Dasa*> getNextLevel( Dasa* );

	virtual bool hasKpFeatures() const { return true; }

	virtual bool isGrahaDasaExpert() const { return true; }

	virtual KpData getKPLords( const double &len );

	virtual vector<KPEvent> getKPEventList( const double &startlen, const double &endlen, const double &startjd );

	vector<Dasa*> getSolarDasa( const Horoscope *solarchart, const int &years );

protected:

	void getKPLordRecursive( int &lord_index, double &elapsed, double &totallen );

	virtual double getPortion( Nakshatra& );
	virtual int getNbLords() const = 0;
	virtual ObjectId getDasaLordByIndex( const int& ) = 0;
	virtual int getDasaIndex4Nakshatra( const Nakshatra& ) = 0;
	virtual int getDasaDuration( const int& ) const = 0;

};

/*************************************************//**
*
* \brief expert class for Yogini Dasa
*
******************************************************/
class YoginiDasaExpert : public GenericNakshatraDasaExpert
{
public:

	YoginiDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_YOGINI, h ) {}

protected:
	virtual int getNbLords() const { return 8; }
	virtual int getParamayus() const { return 36; } virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
	wxString getExtraLordName( Dasa*, const TEXT_FORMAT = TF_LONG );

	virtual wxString getDasaDescription( Dasa*, const TEXT_FORMAT, WriterConfig* = (WriterConfig*)NULL );
	virtual MString getDasaDescriptionF( Dasa*, const TEXT_FORMAT, WriterConfig* = (WriterConfig*)NULL );
};

/*************************************************//**
*
* \brief expert class for Shodshottari Dasa
*
******************************************************/
class ShodshottariDasaExpert : public GenericNakshatraDasaExpert
{
public:

	ShodshottariDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_SHODSHOTTARI, h ) {}

protected:
	virtual int getNbLords() const { return 8; }
	virtual int getParamayus() const { return 116; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
};

/*************************************************//**
*
* \brief expert class for Dvadashottari Dasa
*
******************************************************/
class DvadashottariDasaExpert : public GenericNakshatraDasaExpert
{
public:

	DvadashottariDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_DVADASHOTTARI, h ) {}

protected:
	virtual int getNbLords() const { return 8; }
	virtual int getParamayus() const { return 112; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
};

/*************************************************//**
*
* \brief expert class for Panchottari Dasa
*
******************************************************/
class PanchottariDasaExpert : public GenericNakshatraDasaExpert
{
public:

	PanchottariDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_PANCHOTTARI, h ) {}

protected:
	virtual int getNbLords() const { return 7; }
	virtual int getParamayus() const { return 105; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
};

/*************************************************//**
*
* \brief expert class for Shatabdika Dasa
*
******************************************************/
class ShatabdikaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	ShatabdikaDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_SHATABDIKA, h ) {}

protected:
	virtual int getNbLords() const { return 7; }
	virtual int getParamayus() const { return 100; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
};

/*************************************************//**
*
* \brief expert class for Chaturashiti Sama Dasa
*
******************************************************/
class ChaturashitiSamaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	ChaturashitiSamaDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_CHATURASHITISAMA, h ) {}

protected:
	virtual int getNbLords() const { return 7; }
	virtual int getParamayus() const { return 84; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
};

/*************************************************//**
*
* \brief expert class for Dvisaptati Sama Dasa
*
******************************************************/
class DvisaptatiSamaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	DvisaptatiSamaDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_DVISAPTATISAMA, h ) {}

protected:
	virtual int getNbLords() const { return 8; }
	virtual int getParamayus() const { return 72; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
};

/*************************************************//**
*
* \brief expert class for ShatTrimshat Sama Dasa
*
******************************************************/
class ShatTrimshatSamaDasaExpert : public GenericNakshatraDasaExpert
{
public:

	ShatTrimshatSamaDasaExpert( Horoscope *h ) : GenericNakshatraDasaExpert( D_SHATTRIMSATSAMA, h ) {}

protected:
	virtual int getNbLords() const { return 8; }
	virtual int getParamayus() const { return 36; }
	virtual ObjectId getDasaLordByIndex( const int& );
	virtual int getDasaIndex4Nakshatra( const Nakshatra& );
	virtual int getDasaDuration( const int& ) const;
};

#endif

