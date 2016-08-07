/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Hora.h
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

#ifndef _HORA_H_
#define _HORA_H_

class Location;
class Sheet;

#include "maitreya.h"

/*************************************************//**
*
* \brief Calculates the Lords of year, month, day and hour
*
******************************************************/
class HoraExpert
{
public:
	HoraExpert();

	void setLocation( Location* );
	void update( const double &jd );

	ObjectId getDinaLord() const { return dinaLord; }
	ObjectId getMasaLord() const { return masaLord; }
	ObjectId getVarshaLord() const { return varshaLord; }
	ObjectId getHoraLord() const { return currentHoraLord; }

	int getHoraLordByIndex( const int& ) const;
	double getHoraStartByIndex( const int& ) const;

	void write( Sheet*, const bool localtime = true );

private:

	Location *location;
	ObjectId horaLord[24];
	double horaStart[25];
	ObjectId dinaLord, varshaLord, masaLord, currentHoraLord;
};

#endif


