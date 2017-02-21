/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/KalachakraDasa.cpp
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

#include "KalachakraDasa.h"

#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "SymbolProvider.h"
#include "Sheet.h"

#include <wx/log.h>
#include <wx/stopwatch.h>

extern Config *config;

// group per birth Nakshatra
const KalachakraGroup KalachakraDasaExpert::K_NAKSHATRA_GROUP[27] =
{
	KG_ASWINI, KG_BHARANI, KG_ASWINI, KG_ROHINI, KG_MRIGASIRA, KG_MRIGASIRA, KG_ASWINI, KG_BHARANI, KG_ASWINI,
	KG_ROHINI, KG_MRIGASIRA, KG_MRIGASIRA, KG_ASWINI, KG_BHARANI, KG_ASWINI, KG_ROHINI, KG_MRIGASIRA, KG_MRIGASIRA,
	KG_ASWINI, KG_BHARANI, KG_ASWINI, KG_ROHINI, KG_MRIGASIRA, KG_MRIGASIRA, KG_ASWINI, KG_BHARANI, KG_ASWINI
};

// Lord per group/pada/column
const Rasi KalachakraDasaExpert::K_KALA[4][4][9] =
{
	{
		{ /*?*/ R_ARIES, R_TAURUS, R_GEMINI, R_CANCER, R_LEO, R_VIRGO, R_LIBRA, R_SCORPIO, R_SAGITTARIUS },
		{ R_CAPRICORN, R_AQUARIUS, R_PISCES, /*S*/ R_SCORPIO, R_LIBRA, R_VIRGO, /*MAND*/ R_CANCER, /*MARK*/ R_LEO, /*MAND*/ R_GEMINI },
		{ R_TAURUS, R_ARIES, R_PISCES, R_AQUARIUS, R_CAPRICORN, R_SAGITTARIUS, /*S*/ R_ARIES, R_TAURUS, R_GEMINI },
		{ R_CANCER, R_LEO, R_VIRGO, R_LIBRA, R_SCORPIO, R_SAGITTARIUS, R_CAPRICORN, R_AQUARIUS, R_PISCES },
	},
	{
		{ /*?*/ R_SCORPIO, R_LIBRA, R_VIRGO, /*MAND*/ R_CANCER, /*MARK*/ R_LEO, /*MAND*/ R_GEMINI, R_TAURUS, R_ARIES, R_PISCES },
		{ R_AQUARIUS, R_CAPRICORN, R_SAGITTARIUS, /*S*/ R_ARIES, R_TAURUS, R_GEMINI, R_CANCER, R_LEO, R_VIRGO },
		{ R_LIBRA, R_SCORPIO, R_SAGITTARIUS, R_CAPRICORN, R_AQUARIUS, R_PISCES, /*S*/ R_SCORPIO, R_LIBRA, R_VIRGO },
		{ /*MAND*/ R_CANCER, /*MARK*/ R_LEO, /*MAND*/ R_GEMINI, R_TAURUS, R_ARIES, R_PISCES, R_AQUARIUS, R_CAPRICORN, R_SAGITTARIUS },
	},
	{
		{ /*?*/ R_SAGITTARIUS, R_CAPRICORN, R_AQUARIUS, R_PISCES, R_ARIES, R_TAURUS, R_GEMINI, /*MAND*/ R_LEO, /*MARK*/ R_CANCER },
		{ /*MAND*/ R_VIRGO, R_LIBRA, R_SCORPIO, /*S*/ R_PISCES, R_AQUARIUS, R_CAPRICORN, R_SAGITTARIUS, R_SCORPIO, R_LIBRA },
		{ R_VIRGO, R_LEO, R_CANCER, R_GEMINI, R_TAURUS, R_ARIES, /*S*/ R_SAGITTARIUS, R_CAPRICORN, R_AQUARIUS },
		{ R_PISCES, R_ARIES, R_TAURUS, R_GEMINI, /*MAND*/ R_LEO, /*MARK*/ R_CANCER, /*MAND*/ R_VIRGO, R_LIBRA, R_SCORPIO }
	},
	{
		{ /*?*/ R_PISCES, R_AQUARIUS, R_CAPRICORN, R_SAGITTARIUS, R_SCORPIO, R_LIBRA, R_VIRGO, /*MAND*/ R_LEO, /*MARK*/ R_CANCER },
		{ /*MAND*/ R_GEMINI, R_TAURUS, R_ARIES, /*S*/ R_SAGITTARIUS, R_CAPRICORN, R_AQUARIUS, R_PISCES, R_ARIES, R_TAURUS },
		{ R_GEMINI, /*MAND*/ R_LEO, /*MARK*/ R_CANCER, /*MAND*/ R_VIRGO, R_LIBRA, R_SCORPIO, /*S*/ R_PISCES, R_AQUARIUS, R_CAPRICORN },
		{ R_SAGITTARIUS, R_SCORPIO, R_LIBRA, R_VIRGO, R_LEO, R_CANCER, R_GEMINI, R_TAURUS, R_ARIES }
	}
};

// number of years per rasi lord
const int KalachakraDasaExpert::K_RASIYEARS[12] =
{
	7, 16, 9, 21, 5, 9,
	16, 7, 10, 4, 4, 10
};

// Paramayus per Group/Pada
const int KalachakraDasaExpert::K_TOTALYEARS[4][4] =
{
	{ 100, 85, 83,  86 },
	{ 100, 85, 83,  86 },
	{  86, 83, 85, 100 },
	{  86, 83, 85, 100 }
};

// mapping from savya to apsavya amsa lord
const Rasi KalachakraDasaExpert::K_APSAVYA_AMSA[12] =
{
	R_SCORPIO, R_LIBRA, R_GEMINI, R_LEO, R_CANCER, R_GEMINI,
	R_TAURUS, R_ARIES, R_PISCES, R_AQUARIUS, R_CAPRICORN, R_SAGITTARIUS
};

/*****************************************************
**
**   KalachakraDasa   ---   Constructor
**
******************************************************/
KalachakraDasa::KalachakraDasa( const int &lord, const double &startjd, const double &endjd,
	const int &pada, const KalachakraGroup &group, const KalachakraGati &gati, Dasa *parent )
		: Dasa( D_KALACHAKRA, lord, startjd, endjd, parent ),
		pada( pada ),
		group( group ),
		gati( gati )
{
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   Constructor
**
******************************************************/
KalachakraDasaExpert::KalachakraDasaExpert( Horoscope * h )
 : DasaExpert( D_KALACHAKRA, h )
{
	deha = jeeva = amsa = R_ERROR;
	paramayus = 0;
	savya = true;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   isMarkatiField
**
******************************************************/
bool KalachakraDasaExpert::isMarkatiField( const int &group, const int &pada, const int &column )
{
	assert( group >= 0 && group < 4 );
	assert( pada >= 0 && pada < 4 );
	assert( column >= 0 && column < 9 );

	const int MARKATI_LOCATIONS[7][3] =
	{
		{ 0, 1, 7 },
		{ 1, 0, 4 },
		{ 1, 3, 1 },
		{ 2, 0, 8 },
		{ 2, 3, 5 },
		{ 3, 0, 8 },
		{ 3, 2, 2 }
	};
	for( int i = 0; i < 7; i++ )
	{
		if (
			MARKATI_LOCATIONS[i][group] == group
			&& MARKATI_LOCATIONS[i][pada] == pada
			&& MARKATI_LOCATIONS[i][column] == column
		)
		return true;
	}
	return false;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   calculateGati
**
******************************************************/
KalachakraGati KalachakraDasaExpert::calculateGati( const Rasi &ad_lord, const Rasi &oldlord, const bool &markatiField )
{
	KalachakraGati gati = KdgDefault;
	if ( oldlord != R_NONE )
	{
		switch( ad_lord )
		{
			case R_ARIES:
				if ( oldlord == R_SAGITTARIUS ) gati = KdgSimhavalokan;
			break;
			case R_GEMINI:
				if ( oldlord == R_LEO ) gati = KdgMandooka;
			break;
			case R_CANCER:
				if ( oldlord == R_VIRGO ) gati = KdgMandooka;
				else if ( markatiField ) gati = KdgMarkati;
			break;
			case R_LEO:
				if ( oldlord == R_GEMINI ) gati = KdgMandooka;
				else if ( markatiField ) gati = KdgMarkati;
			break;
			case R_VIRGO:
				if ( oldlord == R_CANCER ) gati = KdgMandooka;
			break;
			case R_SCORPIO:
				if ( oldlord == R_PISCES ) gati = KdgSimhavalokan;
			break;
			case R_SAGITTARIUS:
				if ( oldlord == R_ARIES ) gati = KdgSimhavalokan;
			break;
			case R_PISCES:
				if ( oldlord == R_SCORPIO ) gati = KdgSimhavalokan;
			break;
			default:
			break;
		}
	}
	return gati;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getFirstLevel
**
******************************************************/
vector<Dasa*> KalachakraDasaExpert::getFirstLevel()
{
	Calculator *calculator = CalculatorFactory().getCalculator();
	vector<Dasa*> ret;
	int pada;
	KalachakraGroup group;
	KalachakraGati gati = KdgDefault;
	Rasi lord, oldlord = R_NONE;

	const double mlen = horoscope->getVedicLongitude( OMOON );
	const double portion = ( config->vedicCalculation->kalachakraMode == 3 ? 0
		: calculator->calcNakshatraPortion( horoscope->getDataSet(), mlen, true ));

	pada = (int)( mlen / PADA_LEN );
	pada %= 4;

	int nakshatra = (int)::getNakshatra27( mlen );
	group = K_NAKSHATRA_GROUP[nakshatra];
	savya = ( IS_SAVYA_GROUP ( group ));

	amsa = (Rasi)(( nakshatra % 3  ) * 4 + pada );
	if ( ! savya ) amsa = K_APSAVYA_AMSA[amsa];

	paramayus = K_TOTALYEARS[group][pada];

	deha = savya ? K_KALA[group][pada][0] : K_KALA[group][pada][8];
	jeeva = savya ? K_KALA[group][pada][8] : K_KALA[group][pada][0];

	double elapsed = portion * paramayus;
	int column = 0;
	while ( elapsed - K_RASIYEARS[(int)K_KALA[group][pada][column]] > 0 )
	{
		elapsed -= K_RASIYEARS[(int)K_KALA[group][pada][column]];
		assert( column++ < 8 );
	}
	double start_jd =  horoscope->getJD() - elapsed  * getYearLength( true );
	double end_jd;

	for ( int i = 0; i <= 9; i++ )
	{
		end_jd = start_jd + K_RASIYEARS[K_KALA[group][pada][column]] * getYearLength( true );

		lord = K_KALA[group][pada][column];
		gati = calculateGati( lord, oldlord, isMarkatiField( group, pada, column ));
		ret.push_back( new KalachakraDasa( lord, start_jd, end_jd, pada, group, gati ));
		//ret.push_back( new KalachakraDasa( K_KALA[group][pada][column], start_jd, end_jd, pada, group, gati ));
		column++;
		if ( column > 8 )
		{
			column = 0;

			// iKalachakraMode == 1 doesn's jump to next row
			// mode 0 goes to next row or next nakshatra if last pada.
			// Last pada of savya or apsavya goes to first nakshatra of same group
			if ( config->vedicCalculation->kalachakraMode == 0 )
			{
				if ( pada < 3 ) pada++;
				else
				{
					pada  = 0;
					nakshatra++;
					if ( nakshatra > 27 ) nakshatra = 0;

					// On change of savya/apsava -> don't add one nakshatra, but go back to first nakshatra of same cycle
					if (( savya && K_NAKSHATRA_GROUP[nakshatra] > 1 ) || ( ! savya && K_NAKSHATRA_GROUP[nakshatra] <= 1 )) nakshatra -= 3;
					if ( nakshatra < 0 ) nakshatra += 27;
					group = K_NAKSHATRA_GROUP[nakshatra];
				}
			}
			// Mode 2 stays in the same nakshatra
			else if ( config->vedicCalculation->kalachakraMode == 2 )
			{
				if ( pada < 3 ) pada++;
				else pada  = 0;
			}
		}
		start_jd = end_jd;
		oldlord = lord;
	}
	return ret;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getNextLevel
**
******************************************************/
vector<Dasa*> KalachakraDasaExpert::getNextLevel( Dasa *d )
{
	vector<Dasa*> ret;
	Rasi ad_lord, oldlord = R_NONE;
	double ad_dur;
	int currentcol;
	KalachakraGati gati = KdgDefault;

	assert( d );
	assert( d->getDasaId() == D_KALACHAKRA );
	KalachakraDasa *dasa = (KalachakraDasa*)d;

	const KalachakraGroup group = dasa->getGroup();
	const int pada = dasa->getPada();
	const int lord = dasa->getDasaLord();

	// get correct column
	int column = 0;
	int count = 0;
	while ( K_KALA[group][pada][column] != lord )
	{
		if ( ++column > 9 ) column = 0;
		assert( ++count < 10 );
	}
	const double total_years = K_TOTALYEARS[group][pada];

	double start_jd = dasa->getStartJD();
	double end_jd = dasa->getEndJD();
	const double dasa_dur = end_jd - start_jd;

	for ( int i = 0; i < 9; i++ )
	{
		currentcol = (column + i) % 9;
		ad_lord = K_KALA[group][pada][currentcol];
		ad_dur = dasa_dur * (double)K_RASIYEARS[ad_lord] / total_years;
		end_jd = start_jd + ad_dur;

		gati = calculateGati( ad_lord, oldlord, isMarkatiField( group, pada, currentcol ));
		ret.push_back( new KalachakraDasa( ad_lord, start_jd, end_jd, pada, group, gati, d ));

		start_jd = end_jd;
		oldlord = ad_lord;
	}
	return ret;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getDasaLordNameF
**
******************************************************/
MString KalachakraDasaExpert::getDasaLordNameF( const int &lord, const TEXT_FORMAT &format, WriterConfig *cfg )
{
	SheetFormatter fmt( cfg );
	return fmt.getSignName( (Rasi)lord, format );
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getDasaLordName
**
******************************************************/
wxString KalachakraDasaExpert::getDasaLordName( const int &lord, const TEXT_FORMAT &format, WriterConfig *c )
{
	Lang lang;

	WriterConfig *cfg = c ? c : config->writer;
	if ( cfg->signSymbols )
	{
		return SymbolProvider( cfg ).getSignCode( (Rasi)lord );
	}
	else return lang.getSignName( (Rasi)lord, format );
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getDasaDescriptionF
**
******************************************************/
MString KalachakraDasaExpert::getDasaDescriptionF( Dasa *dasa, const TEXT_FORMAT format, WriterConfig *cfg )
{
	assert( dasa );
	wxString suffix;
	MString f;
	wxString s;

	KalachakraDasa *kdasa = (KalachakraDasa*)dasa;
	f.add( getDasaLordNameF( kdasa->getDasaLord(), format, cfg ));


	switch( kdasa->getGati())
	{
		case KdgDefault:
		break;
		case KdgMandooka:
			suffix = _( "Mandooka" );
		break;
		case KdgMarkati:
			suffix = _( "Markati" );
		break;
		case KdgSimhavalokan:
			suffix = _( "Simha" );
		break;
		default:
			assert( false );
		break;
		
	}
	if ( kdasa->getDasaLord() == jeeva ) suffix = _( "Jeeva" );
	if ( kdasa->getDasaLord() == deha ) suffix = _( "Deha" );
	if ( kdasa->getDasaLord() == amsa ) suffix = _( "Amsa" );

	if ( ! suffix.IsEmpty())
	{
		switch( format )
		{
			case TF_SHORT:
				s << wxT( "/" ) << suffix.Left( 2 );
			break;
			case TF_MEDIUM:
				s << SPACE << wxT( "(" ) << suffix.Left( 3 ) << wxT( ")" );
			break;
			case TF_LONG:
				s << SPACE << wxT( "(" ) << suffix << wxT( ")" );
			break;
			default:
				assert( false );
			break;
		}
	}
	f.add( s );
	return f;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getDasaDescription
**
******************************************************/
wxString KalachakraDasaExpert::getDasaDescription( Dasa *dasa, const TEXT_FORMAT format, WriterConfig *cfg )
{
	assert( dasa );
	wxString suffix;

	KalachakraDasa *kdasa = (KalachakraDasa*)dasa;
	wxString s = getDasaLordName( kdasa->getDasaLord(), format, cfg );

	if ( kdasa->isWrap()) suffix = _( "wrap" );
	if ( kdasa->getDasaLord() == jeeva ) suffix = _( "Jeeva" );
	if ( kdasa->getDasaLord() == deha ) suffix = _( "Deha" );
	if ( kdasa->getDasaLord() == amsa ) suffix = _( "Amsa" );

	switch( kdasa->getGati() )
	{
		case KdgMandooka:
			suffix = _( "Mandooka Gati" );
		break;
		case KdgMarkati:
			suffix = _( "Markati Gati" );
		break;
		case KdgSimhavalokan:
			suffix = _( "Simhavalokan Gati" );
		break;
		default:
		break;
	}

	if ( ! suffix.IsEmpty())
	{
		switch( format )
		{
			case TF_SHORT:
				s << wxT( "/" ) << suffix.Left( 2 );
			break;
			case TF_MEDIUM:
				s << SPACE << wxT( "(" ) << suffix.Left( 3 ) << wxT( ")" );
			break;
			case TF_LONG:
				s << SPACE << wxT( "(" ) << suffix << wxT( ")" );
			break;
			default:
				assert( false );
			break;
		}
	}
	return s;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   writeDasaHeader
**
******************************************************/
void KalachakraDasaExpert::writeDasaHeader( Sheet *sheet, const DasaId &dasaId, Dasa *dasa )
{
	DasaExpert::writeDasaHeader( sheet, dasaId, dasa );
	wxString s;
	Lang lang;

	// test indirectly if getFirstLevel has been calculated before
	if ( paramayus != 0 )
	{
		s.Clear();
		s << _( "Amsa" ) << COLON_SPACE << lang.getSignName( getAmsa(), TF_LONG );
		sheet->addParagraph( s );
		s.Clear();
		s << _( "Deha" ) << COLON_SPACE << lang.getSignName( getDeha(), TF_LONG );
		sheet->addParagraph( s );
		s.Clear();
		s << _( "Jeeva" ) << COLON_SPACE << lang.getSignName( getJeeva(), TF_LONG );
		sheet->addParagraph( s );
	}
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getKPLords
**
******************************************************/
KpData KalachakraDasaExpert::getKPLords( const double &len )
{
	KpData kp;
	int mnak = (int)( len / NAKSHATRA_LEN );
	int group = K_NAKSHATRA_GROUP[mnak];
	savya = ( group <= 1 );

	int pada = (int)( len / PADA_LEN );
	pada %= 4;
	double portion = ( len - ( mnak * NAKSHATRA_LEN + pada * PADA_LEN ) ) / PADA_LEN;

	const int totalyears = K_TOTALYEARS[group][pada];
	//double total_len = totalyears;

	double elapsed = portion * totalyears;
	int spalte = 0;

	//printf( "portion %f totalyears %d elapsed %f\n", portion, totalyears, elapsed );
	//kp.lord = getKPLordRecursive( group, pada, spalte, elapsed, total_len );

	// Lord of Dasa
	while ( elapsed - K_RASIYEARS[(int)K_KALA[group][pada][spalte]] > 0 )
	{
		elapsed -= K_RASIYEARS[(int)K_KALA[group][pada][spalte]];
		spalte++;
		if ( spalte > 8 ) { assert( 0 ); }
	}
	kp.lord = K_KALA[group][pada][spalte]; // that's it

	double maha_dur = K_RASIYEARS[(int)K_KALA[group][pada][spalte]];
	//printf( "getKP len %f nak %d pada %d portion %f lord %d ela %f maha %f\n", len, mnak, pada, portion, lord, elapsed, maha_dur );

	double aloop = K_RASIYEARS[(int)K_KALA[group][pada][spalte]] * maha_dur / totalyears;
	int count = 0;
	while ( elapsed - aloop > 0 )
	{
		//printf( "LOOP %f spalte %d elapsed %f aa %d %f \n", aloop, spalte, elapsed, K_RASIYEARS[(int)K_KALA[group][pada][spalte]], maha_dur  );
		elapsed -= aloop;
		spalte++;
		if ( spalte > 8 ) spalte = 0;
		aloop = K_RASIYEARS[(int)K_KALA[group][pada][spalte]] * maha_dur / totalyears;
		assert( ++count <= 8 );
	}

	assert( spalte >= 0 && spalte < 9 );
	kp.sublord = K_KALA[group][pada][spalte];

	//printf( "elapased %f allop %f\n", elapsed, aloop );
	return kp;
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getKPLordRecursive
**
******************************************************/
int KalachakraDasaExpert::getKPLordRecursive( const int &group, const int &pada, int &spalte, double &elapsed, double &total_len )
{
	//printf( "getKPLordRecursive g %d p %d s %d el %f tl %f\n", group, pada, spalte, elapsed, total_len );
	assert( group >= 0 && group < 4 );
	assert( pada >= 0 && pada < 4 );
	assert( spalte >= 0 && group < 9 );

	const int totalyears = K_TOTALYEARS[group][pada];
	double maha_dur = K_RASIYEARS[(int)K_KALA[group][pada][spalte]];

	double aloop = K_RASIYEARS[(int)K_KALA[group][pada][spalte]] * maha_dur / totalyears;
	int count = 0;
	while ( elapsed - aloop > 0 )
	{
		//printf( "LOOP %f spalte %d elapsed %f aa %d %f \n", aloop, spalte, elapsed, K_RASIYEARS[(int)K_KALA[group][pada][spalte]], maha_dur  );
		elapsed -= aloop;
		spalte++;
		if ( spalte > 8 ) spalte = 0;
		aloop = K_RASIYEARS[(int)K_KALA[group][pada][spalte]] * maha_dur / totalyears;
		assert( ++count <= 8 );
	}

	total_len /= aloop;
	return K_KALA[group][pada][spalte];
}

/*****************************************************
**
**   KalachakraDasaExpert   ---   getKPEventList
**
******************************************************/
vector<KPEvent> KalachakraDasaExpert::getKPEventList( const double &startlen, const double &endlen, const double &startjd )
{
	vector<KPEvent> events;
	int i, j, pada, column, param_ayus, antarcolumn;
	int currentnak;
	KalachakraGroup group;
	Rasi lord, sublord;
	double mahayears, antaryears;
	double len, mahalen, approxjd, inc_antarlen, dist;
	bool append;

	const wxLongLong starttime = wxGetLocalTimeMillis();

	int index = 0;
	int startnak = (int)::getNakshatra27( startlen );
	for ( i = 0; i < 32; i++ )
	{
		currentnak = (startnak + i ) % 27;
		for ( pada = 0; pada < 4; pada++ )
		{
			group = K_NAKSHATRA_GROUP[currentnak];
			len = currentnak * NAKSHATRA_LEN + pada * PADA_LEN;
			param_ayus = K_TOTALYEARS[group][pada];
			for ( column = 0; column < 9; column++ )
			{
				index++;
				lord = K_KALA[group][pada][column];
				mahayears = K_RASIYEARS[lord];
				mahalen = mahayears * PADA_LEN / (double)param_ayus;
				inc_antarlen = 0;
				for ( j = 0; j < 9; j++ )
				{
					append = true;
					antarcolumn = column + j;
					if ( antarcolumn >= 9 ) antarcolumn -= 9;
					sublord = K_KALA[group][pada][antarcolumn];
					antaryears = K_RASIYEARS[sublord];
					approxjd = startjd + i + pada/4;
					if ( i == 0 ) // cut off at the beginning (antardasas before starting point)
					{
						dist = len + inc_antarlen - startlen;
						if ( dist < -300 ) dist += 360;  // correct if 360-deg shift
						if ( dist < 0 ) append = false;
					}
					if ( i >= 28 )  // cut off at the end (if event belongs to next time interval)
					{
						dist = len + inc_antarlen - endlen;
						if ( dist > 300 ) dist -= 360;
						if ( dist < -300 ) dist += 360;
						if ( dist > 0 ) append = false;     // could also be return ?
					}
					if ( append == true )
					{
						events.push_back( KPEvent( lord, sublord, len + inc_antarlen, approxjd, index ));
					}

					inc_antarlen += (double)antaryears * mahalen / (double)param_ayus;
				}
				len += mahalen;
			}

		}
	}
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "KalachakraDasaExpert::getKPEventList in %ld millisec" ), totaltime.ToLong()));
	return events;
}

/*****************************************************
**
**   DasaExpertFactory   ---   create
**
******************************************************/
DasaExpert *DasaExpertFactory::getKalachakraDasaExpert( Horoscope *h )
{
	return new KalachakraDasaExpert( h );
}

