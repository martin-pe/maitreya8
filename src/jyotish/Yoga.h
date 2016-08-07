/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Yoga.h
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

#ifndef _YOGA_H_
#define _YOGA_H_

#include <wx/string.h>
#include <list>
#include <set>

#include "maitreya.h"
#include "Expression.h"
#include "YogaConfig.h"

class AbstractVargaHoroscope;
class MdlInterpreter;
class Rule;
class Sheet;
class YogaConfig;

using namespace std;

/*************************************************//**
*
* \brief singleton class encapsulates calculation and text report of Yogas
*
******************************************************/
class YogaExpert : public ExpressionErrorHandler
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( YogaExpert )

public:

	/**
	 * setup sources, groups and parse rules
	 */
	void init();

	/**
	 * update yogas according to VargaHoroscope, filter, group and Lagna
	 */
	//list<YogaConfig*> update( const class VargaHoroscope*, const int artificialLagna = -1,
		//const wxString groupFilter = wxEmptyString, const wxString sourceFilter = wxEmptyString );

	void updateAndWrite( Sheet*, const class VargaHoroscope*, const int artificialLagna = -1,
		const wxString groupFilter = wxEmptyString, const wxString sourceFilter = wxEmptyString );

	/**
	 * write output to YogaView, text console or file
	 */
	//void write( Writer*, list<YogaConfig*> );

	/**
	 * parse a rule with a given string representation
	 */
	Rule *parseRule( wxString s, wxString &msg );

	/**
	 * evaluate a Rule for a given chart 
	 */
	bool evaluateRule( AbstractVargaHoroscope *evaluationChart, Rule*, wxString &msg );

	/**
	 * get list of possible sources (for choice in YogaView)
	 */
	set<wxString> getSources() const { return sources; }

	/**
	 * get list of possible groups (for choice in YogaView)
	 */
	set<wxString> getGroups() const { return groups; }

	/**
	 *  cleanup groups, soources and Yoga definitions
	 */
	void clear();

private:

	YogaExpert();

	set<wxString> sources;
	set<wxString> groups;
	wxString groupFilter;
	int artificialLagna;
	MdlInterpreter *interpreter;
	bool initialized;

};

#endif


