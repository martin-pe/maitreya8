/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Yoga.h
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
	std::set<wxString> getSources() const { return sources; }

	/**
	 * get list of possible groups (for choice in YogaView)
	 */
	std::set<wxString> getGroups() const { return groups; }

	/**
	 *  cleanup groups, soources and Yoga definitions
	 */
	void clear();

private:

	YogaExpert();

	std::set<wxString> sources;
	std::set<wxString> groups;
	wxString groupFilter;
	int artificialLagna;
	MdlInterpreter *interpreter;
	bool initialized;

};

#endif


