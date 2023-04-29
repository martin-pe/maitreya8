/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/DasaTool.cpp
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

#include "DasaTool.h"
#include "astrobase.h"
#include "Conf.h"
#include "Dasa.h"
#include "DasaConfig.h"
#include "Lang.h"
#include "maitreya.h"
#include "Sheet.h"
#include "Table.h"

#include <wx/tokenzr.h>

extern Config *config;

IMPLEMENT_SINGLETON( DasaTool )

/*****************************************************
**
**   DasaTool   ---   Constructor
**
******************************************************/
DasaTool::DasaTool()
{
  // repeat object name because names in Lang.cpp are translatable
	objects[ wxT( "Sun" ) ] = OSUN;
	objects[ wxT( "Moon" ) ] = OMOON;
	objects[ wxT( "Mars" ) ] = OMARS;
	objects[ wxT( "Mercury" ) ] = OMERCURY;
	objects[ wxT( "Jupiter" ) ] = OJUPITER;
	objects[ wxT( "Venus" ) ] = OVENUS;
	objects[ wxT( "Saturn" ) ] = OSATURN;
	objects[ wxT( "Rahu" ) ] = OMEANNODE;
	objects[ wxT( "Ketu" ) ] = OMEANDESCNODE;
}

/*****************************************************
**
**   DasaTool   ---   getDasaName
**
******************************************************/
wxString DasaTool::getDasaName( const DasaId &dasa )
{
	ASSERT_VALID_DASA_ID( dasa );
	static wxString k_dasa_name[MAX_DASAEXPERTS]  = { _( "Vimsottari" ), _( "Yogini" ), _( "Kalachakra" ),
		_( "Ashtottari" ), _( "Shodshottari" ), _( "Dvadashottari" ), _( "Panchottari" ), _( "Shatabdika" ),
		_( "Chaturashiti Sama" ), _( "Dvisaptati Sama" ), _( "Shat Trimsat Sama" ), _( "Lagna Vimsottari" ) };

	return k_dasa_name[dasa];
}

/*****************************************************
**
**   DasaTool   ---   getDasaLevelName
**
******************************************************/
wxString DasaTool::getDasaLevelName( const int &level )
{
	//printf( "DasaTool::getLevelName level %d\n", level );
	static wxString k_levelname[5] = { _( "Mahadasa" ), _( "Antardasa" ),
		_( "Pratyantardasa" ), _( "Sooksmantardasa" ), _( "Pranadasa" ) };

	if ( level >= 0 && level < 5 ) return k_levelname[level];
	else return wxString::Format( _( "Antardasa Level %d" ), level + 1 );
}

/*****************************************************
**
**   DasaTool   ---   getDasaLordChain
**
******************************************************/
wxString DasaTool::getDasaLordChain( Dasa *dasa, const bool addStartAndEndDate )
{
	if ( dasa == (Dasa*)NULL ) return wxEmptyString;
	WriterConfig *cfg = new WriterConfig( *config->writer );
	cfg->signSymbols = cfg->planetSymbols = false;

	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasa->getDasaId() );
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	wxString s;
	Dasa *d = dasa;
	int count = 0;
	while( d )
	{
		if ( count ) s.Prepend( wxT( "-" ));
		//s.Prepend( AllTrim( expert->getDasaLordName( d, TF_LONG, cfg )));
		//PrintLn( s );
		s.Prepend( AllTrim( expert->getDasaDescription( d, TF_LONG, cfg )));
		d = d->getParent();
		count++;
		assert( count < 1000 );
	}
	if ( addStartAndEndDate )
	{
		s << wxT( " " ) << formatter->formatDateString( dasa->getStartJD() );
		s << wxT( " - " ) << formatter->formatDateString( dasa->getEndJD() );
	}

	delete cfg;
	delete expert;
	return s;
}

/*****************************************************
**
**   DasaTool   ---   calculateMahaAntarLords
**
******************************************************/
void DasaTool::calculateMahaAntarLords( Dasa *dasa, int &lord, int &sublord )
{
	lord = sublord = -1;
	if ( dasa )
	{
		lord = dasa->getDasaLord();
		while( dasa->getParent() )
		{
			sublord = lord;
			dasa = dasa->getParent();
			lord = dasa->getDasaLord();
		}
	}
}

/*****************************************************
**
**   DasaTool   ---   getDasaSources
**
******************************************************/
std::set<wxString> &DasaTool::getDasaSources()
{
	DasaConfigLoader::get()->getConfigs();
	return DasaConfigLoader::get()->getSources();
}

/*****************************************************
**
**   DasaTool   ---   planet2ObjectId
**
******************************************************/
ObjectId DasaTool::planet2ObjectId( const wxString s )
{
  /*
	* ugly wxHashmap: operatori[] inserts default value (0=OSUN) if value not found (and returns it)
	* find method works but doesn't look nice
	*/
	std::map<wxString, ObjectId>::iterator iter = objects.find( s );
	if ( iter == objects.end()) return OERROR;
	else return iter->second;
}

/*****************************************************
**
**   DasaTool   ---   dasa2DasaId
**
******************************************************/
DasaId DasaTool::dasa2DasaId( const wxString s )
{
	DasaId dasaId = D_NONE; 
	if ( ! s.CmpNoCase( wxT( "Vimsottari" ))) dasaId = D_VIMSOTTARI;

	// TODO other dasas
	return dasaId;
} 

/*****************************************************
**
**   DasaTool   ---   writeDasaEffect
**
******************************************************/
void DasaTool::writeDasaEffect( Sheet *sheet, Dasa *dasa, const wxString sourcefilter  )
{
	//printf( "getDasaEffect filter \"%s\"\n", str2char( sourcefilter ));

	std::vector<DasaConfig*> &defs = DasaConfigLoader::get()->getConfigs();
	for ( uint i = 0; i < defs.size(); i++ )
	{
		DasaConfig *cfg = (DasaConfig*)defs[i];
		//printf( "i %d dasaId %d lord %d sublordi cfgdasa %d %d cfglord %d cfgsublord %d\n", i, (int)dasaId, lord, sublord, (int)cfg->dasaId, cfg->lord, cfg->sublord );
		if ( cfg->matches( dasa ) && ( sourcefilter.IsEmpty() || sourcefilter == cfg->source ))
		{
			sheet->addHeader( cfg->source );

			wxStringTokenizer t( cfg->effect, wxT( '\n' ), wxTOKEN_RET_EMPTY );
			while ( t.HasMoreTokens())
			{
				sheet->addParagraph( t.GetNextToken());
			}
			//writer->addParagraph( cfg->effect );
		}
	
	}
}

/*****************************************************
**
**   DasaTool   ---   writeDasaDocumentation
**
******************************************************/
void DasaTool::writeDasaDocumentation( Sheet *sheet, DasaExpert *expert, Dasa *dasa, const wxString sourcefilter  )
{
	wxString s;
	int lord = -1, sublord = -1;

	if ( expert )
	{
		const DasaId dasaId = expert->getDasaId();
		ASSERT_VALID_DASA_ID( dasaId );
		//printf( "Dasa id %d\n", dasaId );
		expert->writeDasaHeader( sheet, dasaId, dasa );

		if ( dasa )
		{
			calculateMahaAntarLords( dasa, lord, sublord );
			//printf( "Lords: %d %d\n", lord, sublord );
			writeDasaEffect( sheet, dasa, sourcefilter );
		}
	}
}

/*****************************************************
**
**   DasaTool   ---   dumpDasa
**
******************************************************/
void DasaTool::dumpDasa( Dasa *dasa, wxString &o )
{
	assert( dasa );

	DateTimeFormatter *f = DateTimeFormatter::get();
	Lang lang;
	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasa->getDasaId() );

	o << expert->getDasaLordName( dasa, TF_MEDIUM );
	o	<< wxT( " " ) << f->formatDateString( dasa->getStartJD() ) << wxT( " - " ) << f->formatDateString( dasa->getEndJD() ) << Endl;

	delete expert;
}

/*****************************************************
**
**   DasaTool   ---   writeShortReport
**
******************************************************/
void DasaTool::writeShortReport( Sheet *sheet, Horoscope *horoscope, const DasaId &dasa, const bool show_header )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	SheetFormatter fmt;
	double startjd;
	uint i;
	wxString s;
	MString t;
	Lang lang;

	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasa, horoscope );
	const bool isgraha = expert->isGrahaDasaExpert();

	if ( show_header )
	{
		s.Clear();
		s << this->getDasaName( dasa ) << SPACE << _( "Dasa" );
		sheet->addHeader( s );
	}

	std::vector<Dasa*> v = expert->getFirstLevel();
	std::vector<Dasa*> w;

	for ( i = 0; i < v.size(); i++ )
	{
		startjd = Max( v[i]->getStartJD(), horoscope->getJD() );

		t = MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, v[i]->getDasaLord(), TF_LONG, true );
		s.Clear();
		s << COLON_SPACE << formatter->formatDateString( startjd )
			<< wxT( " - " ) << formatter->formatDateString( v[i]->getEndJD());
		t.add( s );
		sheet->addHeader( t );

		w = expert->getNextLevel( v[i] );
		if ( w.size() == 0 ) continue;

		startjd = Max( w[0]->getStartJD(), horoscope->getJD() );
		s.Clear();
		s << wxT( " * " ) << formatter->formatDateString( startjd ) << wxT( " " );
		t = MToken( s );

		for ( uint j = 0; j < w.size(); j++ )
		{
			if ( w[j]->getEndJD() >= horoscope->getJD())
			{
				t.add( MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, w[j]->getDasaLord(), TF_LONG, true ));
				t.add( SPACE );
				t.add( formatter->formatDateString( w[j]->getEndJD()));
				t.add( SPACE );
			}
			delete w[j];
		}
		sheet->addParagraph( t );
	}
	for ( i = 0; i < v.size(); i++ ) delete v[i];
	
	delete expert;
}

/*****************************************************
**
**   DasaTool   ---   writeCompactReport
**
******************************************************/
void DasaTool::writeCompactReport( Sheet *sheet, Horoscope *horoscope, const DasaId &dasa, const bool show_header )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	wxString s;
	int line, currentAD = -1, currentMD = -1, currentMdLord = -1, currentAdLord = -1;
	MString t;
	const TEXT_FORMAT format = TF_MEDIUM;

	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasa, horoscope );
	const bool isgraha = expert->isGrahaDasaExpert();

	const double currentJD = MDate::getCurrentJD();

	//dasaShowDateMode = 0; // 0 = startdate 1 = end date
	const bool show_enddate = config->vedicCalculation->dasaShowDateMode == 1;

	std::vector<Dasa*> v = expert->getFirstLevel();
	std::vector<Dasa*> w;

	// test for antardasa length
	w = expert->getNextLevel( v[0] );
	//const uint thesize = w.size();
	//printf( "SIZE %d\n", w.size());

	Table *table = new Table( 5, 2 * w.size() + 2 );
	if ( show_header )
	{
		s.Clear();
		s << this->getDasaName( dasa ) << SPACE << _( "Dasa" );
		table->setHeader( s );
	}


	for ( int i = 0; i < 5; i++ )
	{
		table->col_alignment[i] = Align::Right;
		t = MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, v[i]->getDasaLord(), format, true );
		t.add( SPACE );
		t.add( formatter->formatDateString( show_enddate ? v[i]->getEndJD() : Max( v[i]->getStartJD(), horoscope->getJD())));
		table->setHeader( i, t );

		w = expert->getNextLevel( v[i] );
		line = 1;
		for ( uint j = 0; j < w.size(); j++ )
		{
			if( w[j]->getEndJD() < horoscope->getJD() ) continue;
			if( w[j]->getStartJD() < currentJD && w[j]->getEndJD() > currentJD )
			{
				currentMD = i;
				currentMdLord = v[currentMD]->getDasaLord();
				currentAD = j;
				currentAdLord = w[currentAD]->getDasaLord();
			}

			t = MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, w[j]->getDasaLord(), format, true );
			t.add( SPACE );
			t.add( formatter->formatDateString( show_enddate ? w[j]->getEndJD() : Max( w[j]->getStartJD(), horoscope->getJD())));
			table->setEntry( i, line, t );
			line++;
		}
	}

	const int startline = line;
	for ( uint i = 0; i < 4 && i + 5 < v.size(); i++ )
	{
		line = startline;
		//printf( "i ist %d und size %d\n", i, v.size() );
		t = MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, v[i+5]->getDasaLord(), format, true );
		t.add(  wxT( " " ));
		t.add( formatter->formatDateString( show_enddate ? v[i+5]->getEndJD() : Max( v[i+5]->getStartJD(), horoscope->getJD())));
		table->setHeaderEntry( i, line, t );

		w = expert->getNextLevel( v[i+5] );

		line++;
		for ( uint j = 0; j < w.size(); j++ )
		{
			if( w[j]->getEndJD() < horoscope->getJD() ) continue;
			if( w[j]->getStartJD() < currentJD && w[j]->getEndJD() > currentJD )
			{
				// bugfix 7.0.3 not i but i+5
				currentMD = i+5;
				currentMdLord = v[currentMD]->getDasaLord();
				currentAD = j;
				currentAdLord = w[currentAD]->getDasaLord();
			}

			t = MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, w[j]->getDasaLord(), format, true );
			t.add( SPACE );
			t.add( formatter->formatDateString( show_enddate ? w[j]->getEndJD() : Max( w[j]->getStartJD(), horoscope->getJD())));
			//printf( "Line now %d\n", line );
			table->setEntry( i, line, t );
			line++;
		}
	}

	// Current Pratyantardasa
	if( currentMD >= 0 && currentAD >= 0 )
	{
		line = startline;
		t = MToken( _( "Current" ));
		t.add( wxT( ": " ));
		t.add( MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, currentMdLord, format, true ));
		t.add( wxT( "/" ));
		t.add( MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, currentAdLord, format, true ));
		table->setHeaderEntry( 4, line, t );

		w = expert->getNextLevel( v[currentMD] );
		std::vector<Dasa*> w2 = expert->getNextLevel( w[currentAD] );

		line++;
		for ( uint j = 0; j < w2.size(); j++ )
		{
			t = MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, w2[j]->getDasaLord(), format, true );
			t.add( SPACE );
			t.add( formatter->formatDateString( show_enddate ? w2[j]->getEndJD() : Max( w2[j]->getStartJD(), horoscope->getJD())));
			table->setEntry( 4, line, t );
			line++;
		}
	}
	else printf( "WARN DasaTool::writeCompact currentMD %d currentAD %d\n", currentMD, currentAD );

	sheet->addItem( table );
	delete expert;
}

/*****************************************************
**
**   DasaTool   ---   writeComprehensiveReport
**
******************************************************/
void DasaTool::writeComprehensiveReport( Sheet *sheet, Horoscope *horoscope, const DasaId &dasa, const bool show_header )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	double startjd;
	uint i;
	wxString s;
	MString t;
	const TEXT_FORMAT format = TF_LONG;

	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasa, horoscope );
	const bool isgraha = expert->isGrahaDasaExpert();

	if ( show_header )
	{
		s.Clear();
		s << this->getDasaName( dasa ) << SPACE << _( "Dasa" );
		sheet->addHeader( s );
	}

	std::vector<Dasa*> v = expert->getFirstLevel();
	std::vector<Dasa*> w;

	for ( i = 0; i < v.size(); i++ )
	{
		t = MToken( wxString::Format( wxT( "%d. Mahadasa " ), i+1 ));
		t.add( MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, v[i]->getDasaLord(), format, true ));
		sheet->addHeader( t );

		startjd = Max( v[i]->getStartJD(), horoscope->getJD() );
		s.Printf( wxT( "%s: %s" ), _( "Start Date" ), formatter->formatDateString( startjd ).c_str() );
		sheet->addLine( s );
		s.Printf( wxT( "%s: %s" ), _( "End Date" ), formatter->formatDateString( v[i]->getEndJD() ).c_str() );
		sheet->addLine( s );

		s.Clear();
		//s << _( "Duration" ) << wxT( ": " ) << getTimeSpanFormatted( v[i]->getEndJD() - v[i]->getStartJD(), format );
		s << _( "Duration" ) << wxT( ": " ) << getDasaDuration( v[i]->getEndJD() - v[i]->getStartJD() );
		sheet->addLine( s );

		w = expert->getNextLevel( v[i] );
		if ( w.size() == 0 ) continue;

		sheet->addHeader( _( "Antardasas" ) );

		// Get the number of lines
		int lines = 1;
		for ( uint j = 0; j < w.size(); j++ )
		{
			if ( w[j]->getEndJD() >= horoscope->getJD()) lines++;
		}

		Table *table = new Table( 4, lines );
		table->setHeader( 0, _( "Lord" ));
		table->setHeader( 1,  _( "Start Date" ));
		table->setHeader( 2,  _( "Final Date" ));
		table->setHeader( 3,  _( "Duration" ));

		int line = 1;
		for ( uint j = 0; j < w.size(); j++ )
		{
			if ( w[j]->getEndJD() >= horoscope->getJD())
			{
				table->setEntry( 0, line, MToken( isgraha ? TTSE_PLANET : TTSE_SIGN, w[j]->getDasaLord(), format, true ));

				startjd = Max( w[j]->getStartJD(), horoscope->getJD() );
				table->setEntry( 1, line, formatter->formatDateString( startjd ) );

				table->setEntry( 2, line, formatter->formatDateString( w[j]->getEndJD() ) );

				//table->setEntry( 3, line, getTimeSpanFormatted( w[j]->getEndJD() - w[j]->getStartJD(), TF_MEDIUM ));
				table->setEntry( 3, line, getDasaDuration( w[j]->getEndJD() - w[j]->getStartJD() ));
				line++;
			}
			delete w[j];
		}
		sheet->addItem( table );
	}
	for ( i = 0; i < v.size(); i++ ) delete v[i];
	delete expert;
}

