/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Yoga.cpp
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

#include "Yoga.h"

#include "Horoscope.h"
#include "MdlInterpreter.h"
#include "Expression.h"
#include "Table.h"
#include "Varga.h"
#include "VargaHoroscope.h"
#include "YogaConfig.h"

#include <wx/log.h>
#include <wx/string.h>
#include <math.h>
#include <assert.h>
#include <stdio.h>

extern struct yy_buffer_state * yy_scan_string (const char *yy_str  );
extern int yyparse( void );
extern Rule *rule;
extern wxString parseErrorMessage;

extern int yylineno;

#define YOGA_PRINT_STACK_TRACE_CONSOLE
#define YOGA_PRINT_STACK_TRACE_GUI

IMPLEMENT_SINGLETON( YogaExpert )

/*****************************************************
**
**   YogaExpert   ---   Constructor
**
******************************************************/
YogaExpert::YogaExpert()
{
	interpreter = new MdlInterpreter();
	initialized = false;
}

/*****************************************************
**
**   YogaExpert   ---   init
**
******************************************************/
void YogaExpert::init()
{
	std::vector<YogaConfig*> &defs = YogaConfigLoader::get()->getConfigs();

	for ( uint i = 0; i < defs.size(); i++ )
	{
		YogaConfig *cfg = defs[i];

		if ( ! cfg->group.IsEmpty()) groups.insert( cfg->group );
		sources.insert( cfg->source );

		// parse rule string if not already done
		if ( cfg->rule == 0 )
		{
			//yy_scan_string( wxConvCurrent->cWX2MB( cfg->rulestr ) );
			yy_scan_string( cfg->rulestr.char_str() );
			int errorcode = yyparse();

			if ( errorcode != 0 )
			{
				wxLogError(
					wxString::Format( wxT( "Parse error, rule for Yoga #%d will be NULL, rule was \"%s\"" ), (int)i, cfg->rulestr.c_str()));
				cfg->rule = (Rule*)NULL;
			}
			else cfg->rule = rule;
			rule = (Rule*)NULL;
		}
	}
	initialized = true;
}

/*****************************************************
**
**   YogaExpert   ---   clear
**
******************************************************/
void YogaExpert::clear()
{
	sources.clear();
	groups.clear();

	clearErrors();

	delete YogaConfigLoader::get();
	initialized = false;
}

/*****************************************************
**
**   YogaExpert   ---   parseRule
**
******************************************************/
Rule *YogaExpert::parseRule( wxString s, wxString &msg )
{
	yylineno = 0;
	//yy_scan_string( wxConvCurrent->cWX2MB( s ));
	yy_scan_string( s.char_str());
	int ret = yyparse();
	if ( ret != 0 )
	{
		msg = parseErrorMessage;
		return 0;
	}
	else assert( rule );
	if ( rule->hasErrors )
	{
		if ( rule->errorStack.size() > 0 ) msg = rule->errorStack[0];
		else msg = wxT( "Unknown error in Rule class: error stack empty" );
#ifdef YOGA_PRINT_STACK_TRACE_CONSOLE
		rule->printStackTrace();
#endif
		return 0;
	}
	msg = _( "Rule successfully parsed" );
	return rule;
}

/*****************************************************
**
**   YogaExpert   ---   evaluateRule
**
******************************************************/
bool YogaExpert::evaluateRule( AbstractVargaHoroscope *evaluationChart, Rule *rule, wxString &msg )
{
	yylineno = 0;
	interpreter->setChart( evaluationChart );
	int ret = interpreter->evaluateYogaRule( rule );

	if ( interpreter->hasErrors )
	{
		if ( interpreter->errorStack.size() > 0 ) msg = interpreter->errorStack[0];
		else msg = wxT( "Unknown interpreter error" );
		return false;
	}
	return (bool)ret;
}

/*****************************************************
**
**   YogaExpert   ---   updateAndWrite
**
******************************************************/
void YogaExpert::updateAndWrite( Sheet *sheet, const class VargaHoroscope *chart, const int artificialLagna,
		const wxString groupFilter, const wxString sourceFilter )
{
	std::list<YogaConfig*> ret;
	uint i;
	YogaConfigLoader *loader = YogaConfigLoader::get();
	wxString msg;

	//if ( ! initialized || loader->needsReload() ) init();

	clearErrors();

	Table *table = new Table( 4, 1 );
	table->setHeader( 0, _( "Group" ));
	table->setHeader( 1, _( "Description" ));
	table->col_break[1] = true;
	table->setHeader( 2, _( "Effect" ));
	table->col_break[2] = true;
	table->setHeader( 3, _( "Source" ));

	interpreter->setChart( chart );
	if ( artificialLagna != -1 ) interpreter->setLagna( artificialLagna );

	std::vector<YogaConfig*> &defs = loader->getConfigs();
	//printf( "YogaExpert::updateAndWrite size of yogas is %ld\n", defs.size());

	for ( i = 0; i < defs.size(); i++ )
	{
		YogaConfig *cfg = defs[i];

		if ( chart->getVarga() > V_RASI && cfg->allowHigherVargas == false ) continue;
		if (  groupFilter != wxEmptyString && cfg->group != groupFilter )  continue;
		if (  sourceFilter != wxEmptyString && cfg->source != sourceFilter )  continue;

		if ( cfg->rule == (Rule*)NULL )
		{
			wxLogWarning( wxT( "Rule is NULL, trying to parse again" ));
			cfg->rule = parseRule( cfg->rulestr, msg );
			if ( cfg->rule == (Rule*)NULL )
			{
				wxLogError( wxT( "Rule is still NULL, error message was %s" ), msg.c_str() );
				continue;
			}
		}

		if ( interpreter->evaluateYogaRule( cfg->rule ))
		{
			Row row( table, 4 );
			row.value[0].text = cfg->group;
			row.value[1].text = cfg->description;
			row.value[2].text = cfg->effect;
			row.value[3].text = cfg->source;
			table->addRow( row );
		}

		if ( interpreter->hasErrors )
		{
			addError( wxT( "Rule: " + cfg->rulestr ));
			appendErrors( interpreter );

#ifdef YOGA_PRINT_STACK_TRACE_CONSOLE
			printf( "Error in Yoga\n" );
			printf( "Group: %s / Source: %s\n", str2char( cfg->group ), str2char( cfg->source ));
			printf( "Description: %s\n", str2char( cfg->description ));
			printf( "Rule: %s\n", str2char( cfg->rulestr ));
			interpreter->printStackTrace();
			printf( "\n" );
		}
#endif

	}

	if ( table->getNbRows() == 1 )
	{
		sheet->addParagraph( _( "No matching Yogas found." ));
		delete table;
		return;
	}
	else
	{
		sheet->addItem( table );
	}

#ifdef YOGA_PRINT_STACK_TRACE_GUI
	if ( errorStack.size() > 0 )
	{
		sheet->addHeader( wxT( "Errors" ));
		for ( uint i = 0; i < errorStack.size(); i++ )
		{
			sheet->addLine( errorStack[i] );
		}
	}
#endif
}


