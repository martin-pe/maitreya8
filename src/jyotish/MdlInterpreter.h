/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/MdlInterpreter.h
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

#ifndef _MDLINTERPRETER_H_
#define _MDLINTERPRETER_H_

#include <wx/string.h>
#include <vector>

#include "Expression.h"

using namespace std;

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


