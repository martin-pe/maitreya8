/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Nakshatra.h
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

#ifndef _Nakshatra_H_
#define _Nakshatra_H_

#include "Expert.h"
#include "maitreya.h"
#include <vector>

using namespace std;

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

	void getSbcNakshatraProperties( const int &birthStar, vector<NakshatraProperties>& );

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

	vector<NakshatraId_28> getVedhaNakshatras( const NakshatraId_28& );
	void writeSbcText( Sheet*, const ChartProperties* );
	void writeSbcAfflictions( Sheet*, const Horoscope*, const ChartProperties* );

protected:

};

#endif


