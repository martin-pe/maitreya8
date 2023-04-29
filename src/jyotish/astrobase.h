/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/astrobase.h
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
#ifndef _ASTROBASE_H_
#define _ASTROBASE_H_

#include <wx/string.h>
#include "maitreya.h"

/**************************************************************
***
**  Variuos mathematical funtions
***
***************************************************************/
double calcYearLength( const int sel, const bool vedic );

// whether len is between to deg values
int between( const double &len, const double &min, const double &max );

// Calculate the distance between 2 points
double planetDistance( const double &len1, const double &len2 );

double getMidpoint( const double &len1, const double &len2 );

// return the number of days for a given month and an given year, including leap years
int calcMonthLength( int monat, int jahr );

// year - month - day
void getTimeSpanInts( int &year, int &month, int &day, const double &length, const bool &vedic );
wxString getTimeSpanFormatted( const double &length, const bool &vedic, const TEXT_FORMAT = TF_LONG  );
wxString getDasaDuration( const double &length );

double getYearLength( const bool vedic );
void getDegMinSecInts2( const double &len, int &deg, int &min, double &sec );
void getDegMinSecInts( const double &len, int &deg, int &min, int &sec );

/**************************************************************
***
**  Basic astrological funtions
***
***************************************************************/
// get the rasi for a given length (0..11)
Rasi getRasi( const double &len );

// get the longitude in a specific rasi - reverse will be counted from the end (0..30)
double getRasiLen( const double &len, const bool &reverse = false );

// get the opposite point
double getOpposition( const double& );

double getAntiscium( const double& );

// whether len is in odd, even or odd footed rasi
int isOddRasi( const double &len );
int isEvenRasi( const double &len );
int isOddFootedRasi( const Rasi &rasi );

// get the varna of a longitude
int getVarna( const double &len );

// whether len is in ascendant, kona, kendra resp. to ascendant length
int inAscendant( const double &len, const double &asc );
int inTrikona( const double &len, const double &asc );
int inKendra( const double &len, const double &asc );

// whether len is in movable, fixed or dual sign
int inMovableSign( const double &len );
int inFixedSign( const double &len );
int inDualSign( const double &len );

// get the lord of a rasi (without nodes, otherwise use JaiminiExpert)
ObjectId getLord( const Rasi&n, const bool vedic = true );

// Get the rasi where planet is exalted
Rasi getExaltationRasi( const ObjectId& );

// whether planet is exalted or debilated in rasi
bool hasExaltationRasi( const ObjectId&, const Rasi& );
bool hasDebilationRasi( const ObjectId&, const Rasi& );

// return the mulatrikona rasi for planet
int getMulatrikonaRasi( const ObjectId& );

// whether planet with len is in mulatrikona (only for valid deg longitude)
bool isInMulatrikona( const ObjectId&, const double &len );

// The same if only the rasi is known
bool hasMulatrikonaRasi( const ObjectId&, const Rasi& );

// naisargika mitra
FRIENDSHIP getPlanetaryFriendship( const ObjectId&, const ObjectId& );
bool isPlanetaryFriend( const ObjectId&, const ObjectId& );
bool isPlanetaryEnemy( const ObjectId&, const ObjectId& );

// tatkalika mitra
FRIENDSHIP getRasiTemporaryFriendship( const Rasi&, const Rasi& );
bool isRasiTempFriend( const Rasi&, const Rasi& ); 

// Calculate sputa drishti of 2 planets with a given longitude
//double getAspectValue( const int &planet1, const double &len1, const int &planet2, const double &len2 );
double getAspectValue( const ObjectId&aspectingPlanet, const ObjectId &aspectedPlanet, const double &len  );

// Name of the avastha ( Sayana, Upavesana, etc.)
const wxChar *getAvasthaName( const int &i , const int avastha = 0 );

bool getGrahaDrishti( const ObjectId &aspectingPlanet, const int &rasidiff );
double getGrahaDrishtiValue( const ObjectId &aspectingPlanet, const int &rasidiff );

/**************************************************************
***
**  Jaimini and Nakshatras
***
***************************************************************/
// get the longitude of a planet in his nakshatra
double getNakshatraLongitude27( const double& );
double getNakshatraLongitude28( const double& );

// get the nakshatra for a given length
Nakshatra getNakshatra27( const double &len );
NakshatraId_28 getNakshatra28( const double &len );

// number of pada for a given length
int getPada( const double &len );

// whether 2 rasi positions have a rasi drishti
bool hasJaiminiAspect( const int &pos1, const int &pos2 );

// whether 2 rasis are vishama of sama pada
bool isVishamapadaRasi( const Rasi &rasi );
bool isSampadaRasi( const Rasi &rasi );

double getNavamsaPortion( const double &len, const ObjectId &pindex, const bool &retro = false );

int getVashya( const double &len );

/**************************************************************
***
**  Western astrology
***
***************************************************************/
double calcGeometricalMidpoint( const double &len1, const double &len2 );



#endif


