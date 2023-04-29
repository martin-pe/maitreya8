/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/GenericDasa.cpp
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

#include "GenericDasa.h"

#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "Dasa.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Sheet.h"

extern Config *config;

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getPortion
**
******************************************************/
double GenericNakshatraDasaExpert::getPortion( Nakshatra &nak )
{
	// Generic Calculation based upon the Moon, overwritten by Lagna Vimsottari Dasa
	Calculator *calculator = CalculatorFactory().getCalculator();
	double mlen = horoscope->getVedicLongitude( OMOON );
	nak = (Nakshatra)( mlen / NAKSHATRA_LEN );
	return calculator->calcNakshatraPortion( horoscope->getDataSet(), mlen, false );
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getNextLevel
**
******************************************************/
std::vector<Dasa*> GenericNakshatraDasaExpert::getNextLevel( Dasa *dasa )
{
	assert( dasa );

	std::vector<Dasa*> ret;
	Dasa *parent;

	double start_jd = dasa->getStartJD();
	double end_jd = dasa->getEndJD();
	double dasa_len = end_jd - start_jd;
	double antar_len = 0;
	int lord = -1;
	for ( int i = 0; i < getNbLords(); i++ )
	{
		if ( getDasaLordByIndex( i ) == dasa->getDasaLord() )
		{
			lord = i;
			break;
		}
	}
	assert( lord != -1 );

	parent = dasa;
	//printf( "GenericNakshatraDasaExpert::getNextLevel level %d parent %ld dasa %ld\n", dasa->getLevel(), (long)parent, (long)dasa );

	for ( int i = 0; i < getNbLords(); i++ )
	{
		antar_len = ( dasa_len * getDasaDuration( lord )) / getParamayus();
		end_jd = start_jd + antar_len;


		ret.push_back( new Dasa( dasaId, getDasaLordByIndex( lord ), start_jd, end_jd, parent ));

		lord = ( lord + 1 ) % getNbLords();
		start_jd = end_jd;
	}
	return ret;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getFirstLevel
**
******************************************************/
std::vector<Dasa*> GenericNakshatraDasaExpert::getFirstLevel()
{
	std::vector<Dasa*> ret, tret;
	double nportion;
	int total_length = 0;
	Nakshatra nak;

	nportion = getPortion( nak );
	int pindex = getDasaIndex4Nakshatra( nak );
	int pl = getDasaLordByIndex( pindex );

	double start_jd = horoscope->getJD() - nportion * getDasaDuration( pindex ) * getYearLength( true );
	double end_jd = start_jd + getParamayus() * getYearLength( true );

	while ( total_length < 100 )
	{
		Dasa d( dasaId, pl, start_jd, end_jd, (Dasa*)NULL );
		d.setLevel( -1 );
		tret = getNextLevel( &d );
		for ( unsigned j = 0; j < tret.size(); j++ )
		{
			tret[j]->setParent( (Dasa*)NULL );
			ret.push_back( tret[j] );
		}
		tret.clear();
		total_length += getParamayus();
		start_jd = end_jd;
		end_jd += getParamayus() * getYearLength( true );
	}
	return ret;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getKPLordRecursive
**
******************************************************/
void GenericNakshatraDasaExpert::getKPLordRecursive( int &lord_index, double &elapsed, double &total_len )
{
	int count = 0;
	const int max_loop = 10; // maximum number of antardasas in cycle
	double antar_len; // in years

	if ( elapsed >= total_len ) printf( "ERROR GenericNakshatraDasaExpert::getKPLordRecursive: elapsed time too big, %f total was %f\n", elapsed, total_len );
	assert ( elapsed < total_len );

	antar_len = getDasaDuration( lord_index ) * total_len;
	antar_len /= getParamayus();
	//printf( "Elapsed 1 %f antar_len %f total_len %f dur1 %d dur2 %d\n", elapsed, antar_len, total_len, getDasaDuration( lord_index ), getDasaDuration( lord_index ) );

	// count so many antardasas that elapsed time is more than antarlen
	while (( elapsed > antar_len ) && ( count++ < max_loop  ))
	{
		lord_index++;
		if ( lord_index >= getNbLords() ) lord_index -= getNbLords();
		elapsed -= antar_len;

		antar_len = getDasaDuration( lord_index ) * total_len;
		antar_len /= getParamayus();
		//printf( "Elapsed 2 %f antar_len %f\n", elapsed, antar_len );
	}
	if ( count >= max_loop ) printf( "ERROR GenericNakshatraDasaExpert::getKPLordRecursive: count %d max_loop %d\n", count, max_loop );
	assert( count < max_loop );

	total_len = antar_len;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getKPLords
**
******************************************************/
KpData GenericNakshatraDasaExpert::getKPLords( const double &len )
{
	KpData kp;

	// 0...27 - ASWINI = 0 etc
	Nakshatra nak = getNakshatra27( len );

	// index of the first lord for that Nakshatra in the sequence of lords, e.g. Vimsottari has Sun, Moon, Mars, Rahu, Jupiter, Saturn, Mercury, Ketu, Venus
	int lord_index = getDasaIndex4Nakshatra( nak );

	// elapsed portion in the Nakshatra (0...1)
	double nakshatraPortion = getNakshatraLongitude27( len ) / NAKSHATRA_LEN;

	// elapsed time in years
	double elapsed = nakshatraPortion * getDasaDuration( lord_index );

	// total length in years (e.g. 120 for Vimsottari)
	double total_len = getParamayus();

	//printf( "START ------  CAll 1 KP len %f lord_index %d, portion %f elapsed %f total_len %f\n", len, lord_index, nakshatraPortion, elapsed, total_len );
	getKPLordRecursive( lord_index, elapsed, total_len );
	kp.lord = getDasaLordByIndex( lord_index );
	//printf( "CAll 2 KP len %f lord_index %d KPLORD %d, elapsed %f total_len %f\n", len, lord_index, kp.lord, elapsed, total_len );

	getKPLordRecursive( lord_index, elapsed, total_len );
	kp.sublord = getDasaLordByIndex( lord_index );
	//printf( "CAll 3 KP len %f lord_index %d KPLORD %d, elapsed %f total_len %f\n\n", len, lord_index,  kp.sublord, elapsed, total_len );

	getKPLordRecursive( lord_index, elapsed, total_len );
	kp.subsublord = getDasaLordByIndex( lord_index );
	//printf( "FINISHED   ------  CAll 4 KP len %f lord_index %d KPLORD %d, elapsed %f total_len %f\n\n", len, lord_index,  kp.subsublord, elapsed, total_len );

	return kp;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getKPEventList
**
******************************************************/
std::vector<KPEvent> GenericNakshatraDasaExpert::getKPEventList( const double &startlen, const double &endlen,
        const double &startjd )
{
	std::vector<KPEvent> events;
	Nakshatra currentnak;
	int currentlord, i, j, antarlord, antardasatime;
	double dasastartlen, antarstartlen, dist;
	bool append;

	Nakshatra startnak = getNakshatra27( startlen );

	for ( i = 0; i < 32; i++ )
	{
		currentnak = (Nakshatra)(((int)startnak + i ) % 27 );
		currentlord = getDasaIndex4Nakshatra( currentnak );
		dasastartlen = red_deg( ( startnak + i ) * 13.33333333 );
		antarstartlen = dasastartlen;

		for ( j = 0; j < getNbLords(); j++ )
		{
			append = true;
			antarlord = ( currentlord  + j ) % getNbLords();
			antardasatime = getDasaDuration( antarlord ); // years

			if ( i == 0 ) // cut off at the beginning (antardasas before starting point)
			{
				dist = antarstartlen - startlen;
				if ( dist < -300 ) dist += 360;  // correct if 360-deg shift
				if ( dist < 0 ) append = false;
			}
			if ( i >= 28 )  // cut off at the end (if event belongs to next time interval)
			{
				dist = antarstartlen - endlen;
				if ( dist > 300 ) dist -= 360;
				if ( dist < -300 ) dist += 360;
				if ( dist > 0 ) append = false;     // could also be return ?
			}
			if ( append )
				events.push_back( KPEvent( getDasaLordByIndex( currentlord ), getDasaLordByIndex( antarlord ),
					antarstartlen + .0000001, startjd + i + (double)j/getNbLords(), i ));
			antarstartlen += 13.333333333 * (double)antardasatime / getParamayus();
		}
	}

	return events;
}

/*****************************************************
**
**   GenericNakshatraDasaExpert   ---   getSolarDasa
**
******************************************************/
std::vector<Dasa*> GenericNakshatraDasaExpert::getSolarDasa( const Horoscope *solarchart, const int &year )
{
	std::vector<Dasa*> ret;
	int currentdasa;
	double start_jd, end_jd;

	// lenght of generic dasas is in days. Solar Dasa shifts from e.g. 120 y to 365.25 days
	//const double duration_factor = 365.25 / (double)getParamayus();
	const double duration_factor = getYearLength( true ) / (double)getParamayus();

	const int birthnaks = getNakshatra27( horoscope->getVedicLongitude( OMOON ));
	const int numyears = year - getYearForJD( horoscope->getJD());
	int basedasa = ( birthnaks + numyears - 2 ) % 9;
	if ( basedasa < 0 ) basedasa += 9;
	assert( basedasa >= 0 && basedasa < 9 );

	start_jd = solarchart->getJD();

	for( int i = 0; i < 9; i++ )
	{
		currentdasa = ( basedasa + i ) % 9;
		//printf( "DASA %d basedasa %d numyears %d\n", currentdasa, basedasa, numyears );
		end_jd = start_jd + duration_factor * getDasaDuration( currentdasa );

		ret.push_back( new Dasa( dasaId, getDasaLordByIndex( currentdasa ), start_jd, end_jd, (Dasa*)NULL ));
		start_jd = end_jd;
	}
	return ret;
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId YoginiDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
		static const ObjectId dasa_lord[8] = { OMOON, OSUN, OJUPITER, OMARS, OMERCURY, OSATURN, OVENUS, OMEANNODE };
	return dasa_lord[index];
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int YoginiDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	return (Nakshatra)a_red( nak + 3, 8 );
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaDuration
**
******************************************************/
int YoginiDasaExpert::getDasaDuration( const int &index ) const
{
	assert( index >= 0 && index < 8 );
	return index+1;
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getExtraLordName
**
******************************************************/
wxString YoginiDasaExpert::getExtraLordName( Dasa *dasa, const TEXT_FORMAT format )
{
	assert( dasa );

	const wxString k_yogini[8] = { _( "Mangala" ), _( "Pingala" ), _( "Dhanya" ), _( "Bhramari" ),
		_( "Bhadrika" ), _( "Ulka" ), _( "Siddha" ), _( "Sankata" ) };

	int i = 0;
	for( i = 0; i < 8; i++ )
	{
		if ( getDasaLordByIndex( i ) == dasa->getDasaLord() )
		{
			break;
		}
	}
	assert( i >= 0 && i < 8 );
	if ( format == TF_LONG ) return k_yogini[i];
	else return wxString( k_yogini[i] ).Left( 3 );
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaDescriptionF
**
******************************************************/
MString YoginiDasaExpert::getDasaDescriptionF( Dasa *dasa, const TEXT_FORMAT format, WriterConfig *cfg )
{
	assert( dasa );
	//printf( "YoginiDasaExpert::getDasaDescription lord %d\n", dasa->getDasaLord() );

	switch ( config->vedicCalculation->yoginiDasaLordDisplayType )
	{
		case 0: // Yogini
			return MString( getExtraLordName( dasa, format ));
		break;
		case 1: // Planet
			return DasaExpert::getDasaLordNameF( dasa, format, cfg );
		break;
		//case 2: // Both
		default:
		{
			MString f;
			f.add( DasaExpert::getDasaLordNameF( dasa, format, cfg ));
			wxString s;
				s << wxT( "(" ) << getExtraLordName( dasa, format ) << wxT( ")" );
			f.add( s );
			return f;
		}
		break;
	}
}

/*****************************************************
**
**   YoginiDasaExpert   ---   getDasaDescription
**
******************************************************/
wxString YoginiDasaExpert::getDasaDescription( Dasa *dasa, const TEXT_FORMAT format, WriterConfig *cfg )
{
	assert( dasa );
	//printf( "YoginiDasaExpert::getDasaDescription lord %d\n", dasa->getDasaLord() );

	switch ( config->vedicCalculation->yoginiDasaLordDisplayType )
	{
		case 0: // Yogini
			return getExtraLordName( dasa, format );
		break;
		case 1: // Planet
			return DasaExpert::getDasaLordName( dasa, format, cfg );
		break;
		//case 2: // Both
		default:
		{
			wxString s;
			s << DasaExpert::getDasaLordName( dasa, format, cfg )
				<< wxT( "(" ) << getExtraLordName( dasa, format ) << wxT( ")" );
			return s;
		}
		break;
	}
}

/*****************************************************
**
**   ShodshottariDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId ShodshottariDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const ObjectId dasa_lord[8] = { OSUN, OMARS, OJUPITER, OSATURN, OMEANDESCNODE, OMOON, OMERCURY, OVENUS };
	return dasa_lord[index];
}

/*****************************************************
**
**   ShodshottariDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ShodshottariDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	int diff = (int)a_red( nak - N27_PUSHYAMI, 27 );
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   ShodshottariDasaExpert   ---   getDasaDuration
**
******************************************************/
int ShodshottariDasaExpert::getDasaDuration( const int &index ) const
{
	assert( index >= 0 && index < 8 );
	static const int dasa_dur[8] = { 11, 12, 13, 14, 15, 16, 17, 18 };
	return dasa_dur[index];
}

/*****************************************************
**
**   DvadashottariDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId DvadashottariDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const ObjectId dasa_lord[8] = { OSUN, OJUPITER, OMEANDESCNODE, OMERCURY, OMEANNODE, OMARS, OSATURN, OMOON };
	return dasa_lord[index];
}

/*****************************************************
**
**   DvadashottariDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int DvadashottariDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	int diff = (int)a_red( 26 - nak, 27 ); // 26 is Revati
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   DvadashottariDasaExpert   ---   getDasaDuration
**
******************************************************/
int DvadashottariDasaExpert::getDasaDuration( const int &index ) const
{
	assert( index >= 0 && index < 8 );
	static const int dasa_dur[8] = { 7, 9, 11, 13, 15, 17, 19, 21 };
	return dasa_dur[index];
}

/*****************************************************
**
**   PanchottariDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId PanchottariDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const ObjectId dasa_lord[7] = { OSUN, OMERCURY, OSATURN, OMARS, OVENUS, OMOON, OJUPITER };
	return dasa_lord[index];
}

/*****************************************************
**
**   PanchottariDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int PanchottariDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	int diff = (int)a_red( nak - N27_ANURADHA, 27 );
	return (int)a_red( diff, 7 );
}

/*****************************************************
**
**   PanchottariDasaExpert   ---   getDasaDuration
**
******************************************************/
int PanchottariDasaExpert::getDasaDuration( const int &index ) const
{
	assert( index >= 0 && index < 7 );
	static const int dasa_dur[7] = { 12, 13, 14, 15, 16, 17, 18 };
	return dasa_dur[index];
}

/*****************************************************
**
**   ShatabdikaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId ShatabdikaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const ObjectId dasa_lord[7] = { OSUN, OMOON, OVENUS, OMERCURY, OJUPITER, OMARS, OSATURN };
	return dasa_lord[index];
}

/*****************************************************
**
**   ShatabdikaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ShatabdikaDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	int diff = (int)a_red( nak - 26, 27 ); // 26 is Revati
	return (int)a_red( diff, 7 );
}

/*****************************************************
**
**   ShatabdikaDasaExpert   ---   getDasaDuration
**
******************************************************/
int ShatabdikaDasaExpert::getDasaDuration( const int &index ) const
{
	assert( index >= 0 && index < 7 );
	static const int dasa_dur[7] = { 5, 5, 10, 10, 20, 20, 30 };
	return dasa_dur[index];
}

/*****************************************************
**
**   ChaturashitiSamaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId ChaturashitiSamaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 7 );
	static const ObjectId dasa_lord[7] = { OSUN, OMOON, OMARS, OMERCURY, OJUPITER, OVENUS, OSATURN };
	return dasa_lord[index];
}

/*****************************************************
**
**   ChaturashitiSamaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ChaturashitiSamaDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	int diff = (int)a_red( nak - N27_SWATI, 27 );
	return (int)a_red( diff, 7 );
}

/*****************************************************
**
**   ChaturashitiSamaDasaExpert   ---   getDasaDuration
**
******************************************************/
int ChaturashitiSamaDasaExpert::getDasaDuration( const int& ) const
{
	return 12;
}

/*****************************************************
**
**   DvisaptatiSamaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId DvisaptatiSamaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const ObjectId dasa_lord[8] = { OSUN, OMOON, OMARS, OMERCURY, OJUPITER, OVENUS, OSATURN, OMEANNODE };
	return dasa_lord[index];
}

/*****************************************************
**
**   DvisaptatiSamaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int DvisaptatiSamaDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	int diff = (int)a_red( nak - 18, 27 );
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   DvisaptatiSamaDasaExpert   ---   getDasaDuration
**
******************************************************/
int DvisaptatiSamaDasaExpert::getDasaDuration( const int &index ) const
{
	assert( index >= 0 && index < 8 );
	return 9;
}

/*****************************************************
**
**   ShatTrimshatSamaDasaExpert   ---   getDasaLordByIndex
**
******************************************************/
ObjectId ShatTrimshatSamaDasaExpert::getDasaLordByIndex( const int &index )
{
	assert( index >= 0 && index < 8 );
	static const ObjectId dasa_lord[8] = { OMOON, OSUN, OJUPITER, OMARS, OMERCURY, OSATURN, OVENUS, OMEANNODE };
	return dasa_lord[index];
}

/*****************************************************
**
**   ShatTrimshatSamaDasaExpert   ---   getDasaIndex4Nakshatra
**
******************************************************/
int ShatTrimshatSamaDasaExpert::getDasaIndex4Nakshatra( const Nakshatra &nak )
{
	int diff = (int)a_red( nak - 21, 27 );
	return (int)a_red( diff, 8 );
}

/*****************************************************
**
**   ShatTrimshatSamaDasaExpert   ---   getDasaDuration
**
******************************************************/
int ShatTrimshatSamaDasaExpert::getDasaDuration( const int &index ) const
{
	assert( index >= 0 && index < 8 );
	return index + 1;
}


/*****************************************************
**
**   DasaExpertFactory   ---   create
**
******************************************************/
DasaExpert *DasaExpertFactory::getYoginiDasaExpert( Horoscope *h ) { return new YoginiDasaExpert( h ); }
DasaExpert *DasaExpertFactory::getShodshottariDasaExpert( Horoscope *h ) { return new ShodshottariDasaExpert( h ); }
DasaExpert *DasaExpertFactory::getDvadashottariDasaExpert( Horoscope *h ) { return new DvadashottariDasaExpert( h ); }
DasaExpert *DasaExpertFactory::getPanchottariDasaExpert( Horoscope *h ) { return new PanchottariDasaExpert( h ); }
DasaExpert *DasaExpertFactory::getShatabdikaDasaExpert( Horoscope *h ) { return new ShatabdikaDasaExpert( h ); }
DasaExpert *DasaExpertFactory::getChaturashitiSamaDasaExpert( Horoscope *h ) { return new ChaturashitiSamaDasaExpert( h ); }
DasaExpert *DasaExpertFactory::getDvisaptatiSamaDasaExpert( Horoscope *h ) { return new DvisaptatiSamaDasaExpert( h ); }
DasaExpert *DasaExpertFactory::getShatTrimshatSamaDasaExpert( Horoscope *h ) { return new ShatTrimshatSamaDasaExpert( h ); }


