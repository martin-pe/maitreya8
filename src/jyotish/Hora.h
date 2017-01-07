/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Hora.h
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


