/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/MdlInterpreter.h
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

#ifndef _MDLINTERPRETER_H_
#define _MDLINTERPRETER_H_

#include <wx/string.h>

#include "Expression.h"

/*************************************************//**
*
* \brief interpreter for MDL rule trees (used for Yoga calculation)
*
******************************************************/
class MdlInterpreter : public ExpressionErrorHandler
{
public:
	MdlInterpreter();

	void setChart( const class AbstractVargaHoroscope *chart );
	void setLagna( const int &artificialLagna );

	int evaluateYogaRule( class Rule *rule );

	Rasi getRasi( const double& ) const;
	int getLagna() const { return artificialLagna; }
	int getNakshatra( const double& ) const;
	int getBhava( const double& ) const;
	bool isBenefic( const double& ) const;
	bool isMalefic( const double& ) const;
	int getNumberOfOccupiedSigns();
	int getNumberOfObjectsInSign( const double& ) const;

	void setReturnStatus( const int &i ) { return_status = i; }

protected:

	const AbstractVargaHoroscope *chart;
	int artificialLagna;
	int return_status;
	int nb_planets_in_sign[13];
	wxString errorText;

};


#endif


