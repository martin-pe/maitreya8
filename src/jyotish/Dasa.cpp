/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Dasa.cpp
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


#include "Dasa.h"

#include <wx/string.h>

#include "astrobase.h"
#include "Conf.h"
#include "DasaTool.h"
#include "Lang.h"
#include "maitreya.h"
#include "Sheet.h"
#include "SymbolProvider.h"

extern Config *config;

/*****************************************************
**
**   DasaExpert   ---   getDasaLordName
**
******************************************************/
wxString DasaExpert::getDasaLordName( Dasa *dasa, const TEXT_FORMAT &format, WriterConfig *cfg )
{
	assert( dasa );
	return getDasaLordName( dasa->getDasaLord(), format, cfg ? cfg : config->writer );
}

/*****************************************************
**
**   DasaExpert   ---   getDasaLordNameF
**
******************************************************/
MString DasaExpert::getDasaLordNameF( Dasa *dasa, const TEXT_FORMAT &format, WriterConfig *cfg )
{
	assert( dasa );
	return getDasaLordNameF( dasa->getDasaLord(), format, cfg ? cfg : config->writer  );
}

/*****************************************************
**
**   DasaExpert   ---   getDasaLordName
**
******************************************************/
MString DasaExpert::getDasaLordNameF( const int &lord, const TEXT_FORMAT &format, WriterConfig *cfg )
{
	assert( isGrahaDasaExpert() );
	SheetFormatter fmt( cfg );

	return fmt.getObjectName( (ObjectId)lord, format, true );
}

/*****************************************************
**
**   DasaExpert   ---   getDasaLordName
**
******************************************************/
wxString DasaExpert::getDasaLordName( const int &lord, const TEXT_FORMAT &format, WriterConfig *c )
{
	assert( isGrahaDasaExpert() );
	Lang lang;
	WriterConfig *cfg = c ? c : config->writer;

	if ( cfg->planetSymbols && lord <= OARIES )
	{
		return SymbolProvider( cfg ).getPlanetCode( (ObjectId)lord );
	}
	else return lang.getObjectName( (ObjectId)lord, format, true );
}

/*****************************************************
**
**   DasaExpert   ---   getDasaDescriptionF
**
******************************************************/
MString DasaExpert::getDasaDescriptionF( Dasa *dasa, const TEXT_FORMAT format, WriterConfig *cfg )
{
	return getDasaLordNameF( dasa, format, cfg ? cfg : config->writer  );
}

/*****************************************************
**
**   DasaExpert   ---   getDasaDescription
**
******************************************************/
wxString DasaExpert::getDasaDescription( Dasa *dasa, const TEXT_FORMAT format, WriterConfig *cfg )
{
	return getDasaLordName( dasa, format, cfg ? cfg : config->writer  );
}

/*****************************************************
**
**   DasaExpert   ---   getBirthMahaDasaLord
**
******************************************************/
int DasaExpert::getBirthMahaDasaLord()
{
	std::vector<Dasa*> v = getFirstLevel();
	int lord = v[0]->getDasaLord();
	for( uint i = 0; i < v.size(); i++ )
	{
		delete v[i];
	}
	return lord;
}

/*****************************************************
**
**   DasaExpert   ---   writeDasaHeader
**
******************************************************/
void DasaExpert::writeDasaHeader( Sheet *sheet, const DasaId &dasaId, Dasa *dasa )
{
	assert( sheet );
	wxString s;
	DasaTool *tool = DasaTool::get();

	s.Clear();
	s << tool->getDasaName( dasaId );
	if ( dasa )
	{
		s << SPACE << wxT( "(" ) <<  tool->getDasaLevelName( dasa->getLevel() ) << wxT( ")" );
	}
	sheet->addHeader( s );

	if ( getParamayus())
	{
		s.Clear();
		s << _( "Paramayus" ) << COLON_SPACE << getParamayus() << SPACE << _( "years" );;
		sheet->addParagraph( s );
	}

	if ( dasa )
	{
		s.Clear();
		if ( dasa->getLevel() == 0 ) s << _( "Lord" );
		else s << _( "Lords" );
		s << COLON_SPACE << tool->getDasaLordChain( dasa, false );
		sheet->addParagraph( s );

		// Start and end of Dasa
		TzUtil tzu;
		const bool isLocaltime = true;
		TzFormattedDate startdate = tzu.getDateFormatted( dasa->getStartJD(), isLocaltime );
		s.Clear();
		s << _( "Start Date" ) << COLON_SPACE << startdate.fullDateTimeTzFormatted;
		sheet->addParagraph( s );

		TzFormattedDate enddate = tzu.getDateFormatted( dasa->getEndJD(), isLocaltime );
		s.Clear();
		s << _( "End Date" ) << COLON_SPACE << enddate.fullDateTimeTzFormatted;
		sheet->addParagraph( s );

		s.Clear();
		//s << _( "Duration" ) << COLON_SPACE << getTimeSpanFormatted( dasa->getEndJD() - Max( horoscope->getJD(), dasa->getStartJD()), true, TF_LONG );
		s << _( "Duration" ) << COLON_SPACE << getDasaDuration( dasa->getEndJD() - Max( horoscope->getJD(), dasa->getStartJD()) );
		sheet->addParagraph( s );
	}
}

/*****************************************************
**
**   DasaExpertFactory   ---   getDasaExpert
**
******************************************************/
DasaExpert *DasaExpertFactory::getDasaExpert( const DasaId &dasa, Horoscope *h )
{
	switch( dasa )
	{
		case D_VIMSOTTARI:
			return getVimsottariDasaExpert( h );
		break;
		case D_YOGINI:
			return getYoginiDasaExpert( h );
		break;
		case D_KALACHAKRA:
			return getKalachakraDasaExpert( h );
		break;
		case D_ASHTOTTARI:
			return getAshtottariDasaExpert( h );
		break;
		case D_SHODSHOTTARI:
			return getShodshottariDasaExpert( h );
		break;
		case D_DVADASHOTTARI:
			return getDvadashottariDasaExpert( h );
		break;
		case D_PANCHOTTARI:
			return getPanchottariDasaExpert( h );
		break;
		case D_SHATABDIKA:
			return getShatabdikaDasaExpert( h );
		break;
		case D_CHATURASHITISAMA:
			return getChaturashitiSamaDasaExpert( h );
		break;
		case D_DVISAPTATISAMA:
			return getDvisaptatiSamaDasaExpert( h );
		break;
		case D_SHATTRIMSATSAMA:
			return getShatTrimshatSamaDasaExpert( h );
		break;
		case D_LAGNAVIMSOTTARI:
			return getLagnaVimsottariDasaExpert( h );
		break;
		default:
			assert( false );
		break;
	}
}

