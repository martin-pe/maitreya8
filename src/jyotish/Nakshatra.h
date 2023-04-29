/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Nakshatra.h
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

#ifndef _Nakshatra_H_
#define _Nakshatra_H_

#include "Expert.h"
#include "maitreya.h"
#include <vector>

class ChartProperties;
class Sheet;

enum { TARA_PROP_BENEFIC, TARA_PROP_MALEFIC, TARA_PROP_NEUTRAL }; 

/*************************************************//**
*
* 
*
******************************************************/
class NakshatraProperties
{
public:

	// benefic, malefic, neutral (Abhijit)
	int taraQuality;

	int tara; 

};


/*************************************************//**
*
* 
*
******************************************************/
class Rajju
{
public:

	Rajju() { aroha = type = 0; }

	int aroha;
	int type;
};

/*************************************************//**
*
* 
*
******************************************************/
class Yoni
{
public:

	Yoni() { id = 0; male = true; }
	Yoni( const int &yid, const bool &m, const wxString &s )
	 : id( yid ), male( m ), name( s ) {}

	int id;
	bool male;
	wxString name;
};

/*************************************************//**
*
* \brief Encapsulates calculation of Nakshatra releated details
*
******************************************************/
class NakshatraExpert : public Expert
{
public:

	NakshatraExpert() : Expert() {}
	NakshatraExpert( Horoscope *h ) : Expert( h ) {}

	int getVedhaNakshatra( const int &nak ) const;
	Yoni getYoni( const double& ) const;
	Nadi getNadi( const double& ) const;
	Gana getGana( const double& ) const;
	int getTara( const int& ) const;
	Rajju getRajju( const Nakshatra& );

	void getSbcNakshatraProperties( const int &birthStar, std::vector<NakshatraProperties>& );

	void write( Sheet*, const ChartProperties *chartprops, const bool show_header = true );
};

/*************************************************//**
*
* \brief Encapsulates astrological details for Sarvatobhadra charts
*
******************************************************/
class SarvatobhadraExpert : public Expert
{
public:

	SarvatobhadraExpert() : Expert() {}
	SarvatobhadraExpert( Horoscope *h ) : Expert( h ) {}

	std::vector<NakshatraId_28> getVedhaNakshatras( const NakshatraId_28& );
	void writeSbcText( Sheet*, const ChartProperties* );
	void writeSbcAfflictions( Sheet*, const Horoscope*, const ChartProperties* );

protected:

};

#endif


