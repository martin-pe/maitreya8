/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/UranianHelper.cpp
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

#include "UranianHelper.h"

#include "astrobase.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "FileConfig.h"
#include "Horoscope.h"
#include "mathbase.h"
#include "Lang.h"
#include "Sheet.h"
#include "Table.h"
#include "Transit.h"
#include "Uranian.h"

#include <set>
#include <wx/textfile.h>

extern Config *config;

IMPLEMENT_SINGLETON( UranianTextLoader )

/*****************************************************
**
**   UranianTextLoader   ---   Constructor
**
******************************************************/
UranianTextLoader::UranianTextLoader()
{
	wobjects[ wxT( "mc" ) ] = OMERIDIAN;
	wobjects[ wxT( "ar" ) ] = OARIES;
	wobjects[ wxT( "so" ) ] = OSUN;
	wobjects[ wxT( "as" ) ] = OASCENDANT;
	wobjects[ wxT( "mo" ) ] = OMOON;
	wobjects[ wxT( "kn" ) ] = OTRUENODE;
	wobjects[ wxT( "me" ) ] = OMERCURY;
	wobjects[ wxT( "ve" ) ] = OVENUS;
	wobjects[ wxT( "ma" ) ] = OMARS;
	wobjects[ wxT( "ju" ) ] = OJUPITER;
	wobjects[ wxT( "sa" ) ] = OSATURN;
	wobjects[ wxT( "ur" ) ] = OURANUS;
	wobjects[ wxT( "ne" ) ] = ONEPTUNE;
	wobjects[ wxT( "pl" ) ] = OPLUTO;
	wobjects[ wxT( "cu" ) ] = OCUPIDO;
	wobjects[ wxT( "ha" ) ] = OHADES;
	wobjects[ wxT( "ze" ) ] = OZEUS;
	wobjects[ wxT( "kr" ) ] = OKRONOS;
	wobjects[ wxT( "ap" ) ] = OAPOLLON;
	wobjects[ wxT( "ad" ) ] = OADMETOS;
	wobjects[ wxT( "vu" ) ] = OVULKANUS;
	wobjects[ wxT( "po" ) ] = OPOSEIDON;
}

/*****************************************************
**
**   UranianTextLoader   ---   Destructor
**
******************************************************/
UranianTextLoader::~UranianTextLoader()
{
	cleanup();
	ego = 0;
}

/*****************************************************
**
**   UranianTextLoader   ---   cleanup
**
******************************************************/
void UranianTextLoader::cleanup()
{
	uint i;

	for ( i = 0; i < midpoint_e.size(); i++ ) delete midpoint_e[i];
	midpoint_e.clear();
	for ( i = 0; i < midpoint_w.size(); i++ ) delete midpoint_w[i];
	midpoint_w.clear();
	for ( i = 0; i < conjunction_w.size(); i++ ) delete conjunction_w[i];
	conjunction_w.clear();
}

/*****************************************************
**
**   UranianTextLoader   ---   getMidpointTextItem
**
******************************************************/
MidpointTextItem *UranianTextLoader::getMidpointTextItem( const int &pp, const int &pp1, const int &pp2, const int &mode )
{
	//printf( "UranianTextLoader::getMidpointTextItem size w %d\n", (int)midpoint_w.size());
	//printf( "UranianTextLoader::getMidpointTextItem size e %d\n", (int)midpoint_e.size());


	int p = pp == OMEANNODE ? OTRUENODE : pp;
	int p1 = pp1 == OMEANNODE ? OTRUENODE : pp1;
	int p2 = pp2 == OMEANNODE ? OTRUENODE : pp2;
	if ( mode )
	{
		// Witte
		if ( midpoint_w.empty()) loadItems( mode );
		for ( uint i = 0; i < midpoint_w.size(); i++ )
		{
			if ( midpoint_w[i]->p == p &&
			        (( midpoint_w[i]->p1 == p1 && midpoint_w[i]->p2 == p2 ) || ( midpoint_w[i]->p1 == p2 && midpoint_w[i]->p2 == p1 ))
			   ) return midpoint_w[i];
		}
	}
	else
	{
		// Ebertin
		if ( midpoint_e.empty()) loadItems( mode );
		for ( uint i = 0; i < midpoint_e.size(); i++ )
		{
			if ( midpoint_e[i]->p == p &&
			        (( midpoint_e[i]->p1 == p1 && midpoint_e[i]->p2 == p2 ) || ( midpoint_e[i]->p1 == p2 && midpoint_e[i]->p2 == p1 ))
			   ) return midpoint_e[i];
		}
	}
	return 0;
}

/*****************************************************
**
**   UranianTextLoader   ---   getConjunctionTextItem
**
******************************************************/
ConjunctionTextItem *UranianTextLoader::getConjunctionTextItem( const int &pp1, const int &pp2, const int &mode )
{
	int p1 = pp1 == OMEANNODE ? OTRUENODE : pp1;
	int p2 = pp2 == OMEANNODE ? OTRUENODE : pp2;
	if ( mode )
	{
		if ( conjunction_w.empty()) loadItems( mode );
		for ( uint i = 0; i < conjunction_w.size(); i++ )
		{
			if (( conjunction_w[i]->p1 == p1 && conjunction_w[i]->p2 == p2 )
			        || ( conjunction_w[i]->p1 == p2 && conjunction_w[i]->p2 == p1 )) return conjunction_w[i];
		}
	}
	return 0;
}

/*****************************************************
**
**   UranianTextLoader   ---   getText4UEvent
**
******************************************************/
wxString UranianTextLoader::getText4UEvent( const UEvent &event, const bool witteMode )
{
	UranianTextLoader *loader = UranianTextLoader::get();
	if ( event.isDoubleObjectEvent())
	{
		UObject *uo1 = (UObject*)event.e1;	
		UObject *uo2 = (UObject*)event.e2;	

		ConjunctionTextItem *citem = loader->getConjunctionTextItem( uo1->p, uo2->p, witteMode );
		if ( citem ) return citem->text;
	}
	else if ( event.isObjectAndPairEvent())
	{
		UObject *uo = event.getObjectEntity();
		UObjectPair *up = event.getObjectPairEntity();

		MidpointTextItem *mpitem = loader->getMidpointTextItem( uo->p, up->p1, up->p2, witteMode );
		if ( mpitem ) return mpitem->text;
	}
	else if ( event.isObjectAndTripleEvent())
	{
		UObjectTriple *ut = event.getObjectTripleEntity();

		//return wxT( "triples not supported" );

		// can't afford this
		MidpointTextItem *mpitem = loader->getMidpointTextItem( ut->p1, ut->p2, ut->p3, witteMode );
		//if ( mpitem ) mpitem->text;
		wxString s = wxT( "Root: " ) + mpitem->text;
		return s;
		//if ( mpitem ) return mpitem->text;
	}
	else if ( event.isDoublePairEvent())
	{
		printf( "ERROR in UranianHelper::writeUEvents: double pairs not supported\n" );
	}
	else assert( false );
	return wxEmptyString;
}

/*****************************************************
**
**   UranianTextLoader   ---  hasItemsForMode
**
******************************************************/
bool UranianTextLoader::hasItemsForMode( const int &mode )
{
	wxString filename = FileConfig::get()->getUranianTextFile( mode );
	return wxFile::Access( filename, wxFile::read );
}

/*****************************************************
**
**   UranianTextLoader   ---   loadItems
**
******************************************************/
bool UranianTextLoader::loadItems( const int &mode )
{
	int tabpos, p, p1, p2;
	wxString s, cp, cp1, cp2, theText;
	wxString filename = FileConfig::get()->getUranianTextFile( mode );

	if ( ! wxFile::Access( filename, wxFile::read ))
	{
		//printf( "WARN: textfile %s not found or not readable\n", str2char( filename ));
		return false;
	}
	//else printf( "SUCCESS: textfile %s found and redable\n", str2char( filename ));

	wxTextFile file( filename );
	file.Open();
	for ( s = file.GetFirstLine(); !file.Eof(); s = file.GetNextLine() )
	{
		// get tab position
		tabpos = s.Find( '\t' );
		if ( tabpos == 4 )
		{
			if ( mode == 1 )
			{
				// Conjunction event
				cp1 = s.Mid( 0, 2 );
				p1 = getTextItem( cp1 );
				cp2 = s.Mid( 2, 2 );
				p2 = getTextItem( cp2 );
				theText = s.Mid( 5 );
				if ( p1 != -1 && p2 != -1 && ! theText.IsEmpty() ) conjunction_w.push_back( new ConjunctionTextItem( p1, p2, theText ));
			}
		}
		else if ( tabpos == 6 )
		{
			// midpoint event
			cp = s.Mid( 0, 2 );
			p = getTextItem( cp );
			cp1 = s.Mid( 2, 2 );
			p1 = getTextItem( cp1 );
			cp2 = s.Mid( 4, 2 );
			p2 = getTextItem( cp2 );
			theText = s.Mid( 7 );
			if ( p != -1 && p1 != -1 && p2 != -1 && ! theText.IsEmpty() )
			{
				if ( mode ) midpoint_w.push_back( new MidpointTextItem( p, p1, p2, theText ));
				else midpoint_e.push_back( new MidpointTextItem( p, p1, p2, theText ));
			}
		}
		else
		{
			printf( "Wrong tabpos %d in Uranian filei: %s\n", tabpos, str2char( s ));
		}
	}
	return true;
}

/*****************************************************
**
**   UranianTextLoader   ---   getTextItem
**
******************************************************/
int UranianTextLoader::getTextItem( wxString s )
{
	int ret = wobjects[s];
	if ( ret == 0 && s != wxT ( "so" ) ) return -1;
	else return ret;
}

/*****************************************************
**
**   UranianHelper   ---   Constructor
**
******************************************************/
UranianHelper::UranianHelper( UranianExpert *expert )
		: expert( expert )
{
}

/*****************************************************
**
**   UranianHelper   ---   isOneContext
**
******************************************************/
bool UranianHelper::isOneContext( std::list<UEvent> &events, const PlanetContext &ctx )
{
	std::list<UEvent>::iterator iter;
	for ( iter = events.begin(); iter != events.end(); iter++ )
	{
		assert( (*iter).e1 );
		assert( (*iter).e2 );
		if ( iter->e1->context != ctx || iter->e2->context != ctx )
		{
			return false;
		}
	}
	return true;
}

/*****************************************************
**
**   UranianHelper   ---   writeRadixAnalysis
**
******************************************************/
void UranianHelper::writeRadixAnalysis( Sheet *sheet )
{
	writeUEvents( sheet, PcRadix );
	writeMatchingClusters( sheet, PcRadix );
}

/*****************************************************
**
**   UranianHelper   ---   writeTransits
**
******************************************************/
void UranianHelper::writeTransits( Sheet *sheet, TransitExpert *texpert )
{
	wxString s;
	UranianTool *tool = UranianTool::get();
	PlanetContext ctx = texpert->getTransitMode();
	UranianConfig &cfg = expert->getChartProperties()->getUranianConfig();

	sheet->addLine( wxString::Format( wxT( "%s: %d %s" ),
		_( "Orbis" ), (int)(60 * cfg.orbisTransit), wxT( "min" )));

	s << _( "Gradkreis" ) << wxT( ": " )
		<< tool->gradkreis2String( cfg.gradkreis );
	sheet->addLine( s );

	writeUEvents( sheet, ctx ); // 3rd param: show date
	writeMatchingClusters( sheet, ctx );
}

/*****************************************************
**
**   UranianHelper   ---   writePartner
**
******************************************************/
void UranianHelper::writePartner( Sheet *sheet )
{
	wxString s;

	//if ( show_header ) writer->addHeader( _( "Uranian Partner Analysis" ));

	s.Clear();

	s << _( "Partner1" ) << wxT( "/" ) << _( "Partner2" );
	sheet->addHeader( s );
	writeUEvents( sheet, PcPartner1 );
	writeMatchingClusters( sheet, PcPartner1 );

	s.Clear();
	s << _( "Partner2" ) << wxT( "/" ) << _( "Partner1" );
	sheet->addHeader( s );
	writeUEvents( sheet, PcPartner2 );
	writeMatchingClusters( sheet, PcPartner2 );
}

/*****************************************************
**
**   UranianHelper   ---   writeYearlyPreview
**
******************************************************/
void UranianHelper::writeYearlyPreview( Sheet *sheet )
{
	sheet->addHeader( _( "Yearly Preview for Solar Arc" ));
	writeUEvents( sheet, PcPeriod, true ); //, true );
}

/*****************************************************
**
**   UranianHelper   ---   writeUEvents
**
******************************************************/
void UranianHelper::writeUEvents( Sheet *sheet, const PlanetContext &ctx, const bool showdate )
{
	DateTimeFormatter *dtf= DateTimeFormatter::get();
	Formatter *formatter = Formatter::get();
	UranianTextLoader *loader = UranianTextLoader::get();
	const ObjectFilter filter = expert->getChartProperties()->getObjectFilter();
	UranianTool *tool = UranianTool::get();

	//OBJECT_INCLUDES oi = expert->getChartProperties()->getObjectStyle();
	//const bool witteMode = oi & OI_URANIAN;
	const bool witteMode = true;

	sheet->addHeader( _( "Matchings" ));

	UranianCalculationResult *r = expert->planetContext2Result( ctx );
	assert( r );

	if ( r->uevents.size() == 0 )
	{
		sheet->addLine( _( "No Matchings" ));
		return;
	}

	const bool hasText = loader->hasItemsForMode( witteMode );
	//const bool hasText = false;

	int nbcols = 4;
	if ( hasText ) nbcols++;
	
	Table *table = new Table( nbcols, r->uevents.size() + 1 );
	table->setHeader( 0, wxT( "No." ));
	table->col_alignment[0] = Align::Right;

	table->setHeader( 1, wxT( "Planets" ));
	table->col_break[1] = false;

	if ( showdate ) table->setHeader( 2, wxT( "Due Date" ));
	else table->setHeader( 2, wxT( "Orbis" ));

	table->setHeader( 3, wxT( "Type" ));
	if ( hasText )
	{
		table->setHeader( 4, wxT( "Effect" ));
		table->col_break[4] = true;
	}

	const UWRITE_FORMAT format = isOneContext( r->uevents, ctx ) ? UF_DEFAULT : UF_CONTEXT;

	int count = 1;
	for ( std::list<UEvent>::iterator iter = r->uevents.begin(); iter != r->uevents.end(); iter++ )
	{
		if ( filter.size() > 0 && ! (*iter).applyFilter( filter )) continue;

		table->setEntry( 0, count, wxString::Format( wxT( "%d" ), count ));
		table->setEntry( 1, count, iter->write( format ));

		if ( showdate ) table->setEntry( 2, count, dtf->formatDateString( (*iter).duedate ));
		else table->setEntry( 2, count, formatter->getLenFormatted( (*iter).orbis ));

		table->setEntry( 3, count, tool->gradkreis2String( (*iter).gradkreis ));
		if ( hasText ) table->setEntry( 4, count, loader->getText4UEvent( (*iter), witteMode ));

		count++;
	}
	if ( table->getNbRows() <= 1 )
	{
		sheet->addParagraph( _( "No events found" ));
		delete table;
	}
	else sheet->addItem( table );
}

/*****************************************************
**
**   UranianHelper   ---   writeSpecialPoints
**
******************************************************/
void UranianHelper::writeSpecialPoints( Sheet *sheet, const PlanetContext &ctx )
{
	sheet->addHeader( wxT( "Special Points" ));
	UranianCalculationResult *r = expert->planetContext2Result( ctx );
	assert( r );
	sheet->addHeader( wxT( "Special Points" ));
}

/*****************************************************
**
**   UranianHelper   ---   writeSensitivePoints
**
******************************************************/
void UranianHelper::writeSensitivePoints( Sheet *sheet, const PlanetContext &ctx )
{
	UranianCalculationResult *r = expert->planetContext2Result( ctx );
	assert( r );

	sheet->addHeader( wxT( "Sensitive Points" ));
}

/*****************************************************
**
**   UranianHelper   ---   writeTextAnalysis
**
******************************************************/
void UranianHelper::writeTextAnalysis( Sheet *sheet, const PlanetContext &ctx )
{
	UranianCalculationResult *r = expert->planetContext2Result( ctx );
	assert( r );
	const Horoscope *h = r->horoscope;
	assert( h );

	sheet->addHeader( wxT( "TextAnalysis" ));
}

/*****************************************************
**
**   UranianHelper   ---   createTupleTable
**
******************************************************/
Table *UranianHelper::createTupleTable( const URANIAN_TUPLE_TYPE &utt, const PlanetContext &ctx )
{
	Formatter *formatter = Formatter::get();
	double value;
	//wxString s, s1;
	SheetFormatter fmt;

	UranianCalculationResult *r = expert->planetContext2Result( ctx );
	assert( r );
	const Horoscope *h = r->horoscope;
	assert( h );

	const ObjectArray &planets = expert->getChartProperties()->getPlanetList();
	const int pmax = planets.size();

	UranianConfig &cfg = expert->getChartProperties()->getUranianConfig();
	const bool vedic = expert->getChartProperties()->isVedic();
	UranianTool *tool = UranianTool::get();
	double mod_degrees = tool->getDegrees4Gradkreis( cfg.gradkreis );

	const bool symmetric = ( utt == UTT_DIFFERENCES || utt == UTT_REFLECTION_POINTS );

	Table *table = new Table( pmax + 2, pmax + 2 );

	table->setHeader( 0, wxEmptyString );
	table->setHeader( pmax + 1, wxEmptyString );
	table->setHeaderEntry( 0, pmax + 1,  wxEmptyString );
	table->setHeaderEntry( pmax + 1, pmax + 1, wxEmptyString );
	for ( int i = 0; i < pmax; i++ )
	{
		table->setHeader( i+1, fmt.getObjectName( planets[i] ));
		table->setHeaderEntry( i + 1, pmax + 1, fmt.getObjectName( planets[i] ));

		table->setHeaderEntry( 0, i + 1, fmt.getObjectName( planets[i] ));
		table->setHeaderEntry( pmax + 1, i + 1, fmt.getObjectName( planets[i] ));
	}

	for ( int i = 0; i < pmax; i++ )
	{
		for ( int j = 0; j < pmax; j++ )
		{
			if ( i >= j || symmetric )
			{
				switch( utt )
				{
					case UTT_SUMS:
						value = a_red( h->getLongitude( planets[i], vedic ) +  h->getLongitude( planets[j], vedic ), mod_degrees );
					break;
					case UTT_DIFFERENCES:
						value = a_red( h->getLongitude( planets[i], vedic ) -  h->getLongitude( planets[j], vedic ), mod_degrees );
					break;
					case UTT_MIDPOINTS:
					{
						if ( cfg.gradkreis == GK_360 ) 
							value = getMidpoint( h->getLongitude( planets[i], vedic ), h->getLongitude( planets[j], vedic ));
						else value = a_red( .5 * ( h->getLongitude( planets[i], vedic ) +  h->getLongitude( planets[j], vedic )),
							mod_degrees );
					}
					break;
					case UTT_ANT_MIDPOINTS:
					{
						if ( cfg.gradkreis == GK_360 ) 
							value = getMidpoint(
								getAntiscium ( h->getLongitude( planets[i], vedic )),
								getAntiscium( h->getLongitude( planets[j], vedic )));
						else value = a_red( .5 * (
							getAntiscium( h->getLongitude( planets[i], vedic )) + 
							getAntiscium( h->getLongitude( planets[j], vedic ))),
							mod_degrees );
					}
					break;
					case UTT_REFLECTION_POINTS:
						value = a_red( 2 * h->getLongitude( planets[i], vedic ) -  h->getLongitude( planets[j], vedic ), mod_degrees );
					break;
					default:
						assert( false );
					break;
				}
				if ( cfg.gradkreis == GK_360 )
					table->setEntry( j + 1, i + 1, fmt.getPosFormatted( value, MD_DIRECT, DEG_PRECISION_SECOND, TF_SHORT ));
				else
					table->setEntry( j + 1, i + 1, formatter->getDegreesFormatted( value ));
			}
		}
	}
	return table;
}

/*****************************************************
**
**   UranianHelper   ---   writeTupleTable
**
******************************************************/
void UranianHelper::writeTupleTable( Sheet *sheet, const URANIAN_TUPLE_TYPE &utt, const PlanetContext &ctx )
{
	wxString s, s1;

	switch( utt )
	{
		case UTT_SUMS:
			s = _( "Sums" );
		break;
		case UTT_DIFFERENCES:
			s = _( "Differences" );
			s1 = _( "Row - column" );
		break;
		case UTT_MIDPOINTS:
			s = _( "Midpoints" );
			//if ( cfg.gradkreis == GK_360 ) mod_degrees = 180.0;
		break;
		case UTT_ANT_MIDPOINTS:
			s = _( "Midpoints of Antiscia" );
		break;
		case UTT_REFLECTION_POINTS:
			s = _( "Reflection Points" );
			s1 = _( "Row + row - column" );
		break;
		default:
			assert( false );
		break;
	}
	sheet->addHeader( s );
	if ( ! s.IsEmpty()) sheet->addLine( s1 );
	sheet->addItem( createTupleTable( utt, ctx ));
}

/*****************************************************
**
**   UranianHelper   ---   writeMatchingClusterPart
**
******************************************************/
void UranianHelper::writeMatchingClusterPart( Sheet *sheet, const PlanetContext &ctx,
	std::map<ObjectId, ClusterMatchingList> &m, const uint &istart, const uint &iend )
{
	Lang lang;
	ObjectId p;
	const ObjectArray &planets = expert->getChartProperties()->getPlanetList();
	UWRITE_FORMAT format = UF_DEFAULT;
	MString t;
	int col = 0;
	uint maxrow = 0;
	std::set<ObjectId> keys;

	assert( istart < planets.size());
	assert( iend <= planets.size());
	assert( istart < iend );

	for ( uint i = istart; i < iend; i++ )
	{
		p = planets[i];
		for( std::map<ObjectId,ClusterMatchingList>::iterator iter = m.begin(); iter != m.end(); iter++ )
		{
			if ( iter->first == p ) keys.insert( p );
		}
	}
	for( std::set<ObjectId>::iterator iter = keys.begin(); iter != keys.end(); iter++ )
	{
		maxrow = Max( m[*iter].matchings.size(), maxrow );
	}

	Table *table = new Table( keys.size(), maxrow + 1 );
	for ( uint i = istart; i < iend ; i++ )
	{
		p = planets[i];
		if ( m[p].matchings.size() == 0 ) continue;

		format = m[p].oneContext ? UF_DEFAULT : UF_CONTEXT;

		t = MToken( TTSE_PLANET, (int)p, TF_MEDIUM );
		if ( format == UF_CONTEXT ) t.add( MToken( lang.getPlanetContextSubscriptum( ctx ), TTFF_SUBSCRPTUM ));
		table->setHeader( col, t );

		for( uint r = 0; r < m[p].matchings.size(); r++ )
		{
			table->setEntry( col, r + 1, m[p].matchings[r]->write( format ));
		}
		col++;
	}
	sheet->addItem( table );
}

/*****************************************************
**
**   UranianHelper   ---   writeMatchingClusters
**
******************************************************/
void UranianHelper::writeMatchingClusters( Sheet *sheet, const PlanetContext &ctx )
{
	std::map<ObjectId, ClusterMatchingList> m = expert->createClusterMatchingList( ctx );
	std::set<ObjectId> keys;

	//printf( "UranianHelper::writeMatchingClusters context %d\n", (int)ctx );

	const bool skipAries = ( ctx == PcTransit || ctx == PcDirection );

	// collect keys. only planets with matchings need to be shown
	for( std::map<ObjectId,ClusterMatchingList>::iterator iter = m.begin(); iter != m.end(); iter++ )
	{
		if ( iter->first == OARIES && skipAries ) continue;
		keys.insert( iter->first );
	}

	sheet->addHeader( _( "Clusters" ));
	if ( keys.size() == 0 )
	{
		sheet->addLine( _( "No Matchings" ));
	}
	else
	{
		const ObjectArray &planets = expert->getChartProperties()->getPlanetList();
		if ( keys.size() > 10 )
		{
			uint pmid = (uint)keys.size() / 2;
			writeMatchingClusterPart( sheet, ctx, m, 0, pmid );
			writeMatchingClusterPart( sheet, ctx, m, pmid, planets.size());
		}
		else
		{
			writeMatchingClusterPart( sheet, ctx, m, 0, planets.size());
		}
	}
}


