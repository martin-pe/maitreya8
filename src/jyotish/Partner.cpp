/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Partner.cpp
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

#include "Partner.h"

#include "astrobase.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "VedicPlanet.h"
#include "Lang.h"
#include "mathbase.h"
#include "Nakshatra.h"
#include "Sheet.h"
#include "Table.h"
#include "Varga.h"

#include <wx/log.h>

extern Config *config;

IMPLEMENT_CLASS( CompositHoroscope, Horoscope )

/*****************************************************
**
**   AshtakootaExpert   ---   Constructor
**
******************************************************/
AshtakootaExpert::AshtakootaExpert()
 :  Expert( (Horoscope*)NULL )
{
	horoscope = 0;
	h2 = 0;
	init();
}

/*****************************************************
**
**   AshtakootaExpert   ---   Constructor
**
******************************************************/
AshtakootaExpert::AshtakootaExpert( Horoscope *horoscope, Horoscope *h2 )
{
	this->horoscope = horoscope;
	this->h2 = h2;
	init();
}

/*****************************************************
**
**   AshtakootaExpert   ---   init()
**
******************************************************/
void AshtakootaExpert::init()
{
	pvarna = pyoni = pmaitri = prasi = pgana = pnadi = prajju = 0;
	pvashya = ptara = ptotal = 0.0;
	rasi1 = rasi2 = R_NONE;
}

/*****************************************************
**
**   AshtakootaExpert   ---   calcYoniValue
**
******************************************************/
int AshtakootaExpert::calcYoniValue( const Yoni &y1, const Yoni &y2 )
{
	assert( y1.id >= 0 && y1.id < 14 );
	assert( y2.id >= 0 && y2.id < 14 );
	const static int yoni_map[14][14] =
	{
		{ 4, 0, 1, 2, 3, 2, 2, 1, 3, 2, 3, 3, 1, 3 },
		{ 0, 4, 1, 2, 3, 2, 2, 2, 3, 2, 2, 2, 1, 3 },
		{ 1, 1, 4, 0, 1, 2, 2, 2, 1, 1, 2, 1, 2, 1 },
		{ 2, 3, 0, 4, 2, 2, 2, 2, 2, 2, 3, 2, 1, 2 },
		{ 3, 3, 1, 2, 4, 0, 3, 1, 3, 2, 3, 2, 1, 3 },
		{ 2, 2, 2, 2, 0, 4, 2, 1, 2, 2, 2, 2, 1, 2 },
		{ 2, 2, 2, 2, 3, 2, 4, 0, 2, 1, 2, 2, 2, 2 },
		{ 1, 2, 2, 2, 1, 1, 0, 4, 1, 1, 1, 1, 2, 1 },
		{ 3, 2, 1, 2, 3, 2, 2, 1, 4, 0, 2, 2, 1, 2 },
		{ 2, 2, 1, 2, 2, 2, 1, 1, 0, 4, 1, 1, 1, 1 },
		{ 3, 2, 2, 3, 3, 2, 2, 1, 2, 1, 4, 0, 2, 2 },
		{ 3, 2, 1, 2, 2, 2, 2, 1, 2, 1, 0, 4, 2, 2 },
		{ 1, 1, 2, 1, 1, 1, 2, 2, 1, 1, 2, 2, 4, 0 },
		{ 3, 3, 1, 2, 3, 2, 2, 1, 2, 1, 2, 2, 0, 4 }
	};
	return yoni_map[y1.id][y2.id];
}

/*****************************************************
**
**   AshtakootaExpert   ---   calcRajjuValue
**
******************************************************/
int AshtakootaExpert::calcRajjuValue( const Rajju &rajju1, const Rajju &rajju2 )
{
	 /*
	  const wxString k_aroha_name[3] = { _( "Aroha" ), wxEmptyString, _( "Avaroha" ) };
		const wxString k_rajjutype_name[5] = { _( "Pada (foot)" ), _("Kati (waist)" ), _( "Nabhi (navel)" ), _( "Kantha (neck)" ), _( "Siro (head)" ) };
	 */
	 int pr = 0;
	 // different Rajjus in Aroha
	 if ( rajju1.aroha == 0 && rajju2.aroha == 0 && rajju1.type != rajju2.type )
	 {
	 	pr = 4;
	 }

	 // different Rajjus, not both in Avaroha
	 else if ( rajju1.type != rajju2.type && ( rajju1.aroha != 2 || rajju2.aroha != 2 ) )
	 {
	 	pr = 3;
	 }

	 // same Rajju, one in Aroha, other in Avaroha
	 else if ( rajju1.type != rajju2.type && rajju1.aroha != rajju2.aroha )
	 {
	 	pr = 2;
	 }

	 // different Rajjus in Avaroha
	 else if ( rajju1.type != rajju2.type && rajju1.aroha == 2 && rajju2.aroha == 2 )
	 {
	 	pr = 1;
	 }

	 // same Rajju 
	 //else if ( rajju1.type == rajju2.type && rajju1.aroha == rajju2.aroha )
	 else if ( rajju1.type == rajju2.type )
	 {
	 	pr = 0;
	 }

	 else
	 {
			wxLogError( wxString::Format( wxT ( "RAJJU1 aroha %d type %d Rajju2 aroha %d type %d\n" ),
				rajju1.aroha, rajju1.type, rajju2.aroha, rajju2.type ));
			assert( false );
	 }

	 //printf( "RAJJU1 aroha %d type %d Rajju2 aroha %d type %d\n", rajju1.aroha, rajju1.type, rajju2.aroha, rajju2.type );
	return pr;
}

/*****************************************************
**
**   AshtakootaExpert   ---   update
**
******************************************************/
void AshtakootaExpert::update()
{
	double len1, len2;
	init();

	NakshatraExpert expert;

	len1 = horoscope->getVedicLongitude( OMOON );
	len2 = h2->getVedicLongitude( OMOON );
	rasi1 = getRasi( len1 );
	nak1 = getNakshatra27( len1 );
	rasi2 = getRasi( len2 );
	nak2 = getNakshatra27( len2 );

	/*
	 * Varna
	 */
	varna1 = getVarna( len1 );
	varna2 = getVarna( len2 );
	if ( varna1 >= varna2 )
	{
		pvarna = 1;
	}

	/*
	 * Vashya
	 */
	vashya1 = getVashya( len1 );
	vashya2 = getVashya( len2 );
	assert( vashya1 >= 0 && vashya1 < 5 );
	assert( vashya2 >= 0 && vashya2 < 5 );

	const double vashya_map[5][5] =
	{
		{ 2, 1, .5, 0, 1 },
		{ 0, 2, 1, 0, 1 },
		{ 0, 1, 2, 0, 1 },
		{ .5, .5, 1, 2, 0 },
		{ 0, 1, 1, 0, 2 }
	};
	pvashya = vashya_map[vashya1][vashya2];

	/*
	 * Dina or Tara
	 */
	int dnak1 = (int)nak2 - (int)nak1;
	if ( dnak1 < 0 ) dnak1 += 27;
	dnak1++;
	tara1 = dnak1 % 9;
	if (( tara1 == 3 ) || ( tara1 == 5 ) || ( tara1 == 7 ))
	{
		ptara  = 0;
	}
	else ptara = 1.5;

	// TODO
	int dnak2 = (int)nak1 - (int)nak2;
	if ( dnak2 < 0 ) dnak2 += 27;
	dnak2++;
	tara2 = dnak2 % 9;
	if (( tara2 == 3 ) || ( tara2 == 5 ) || ( tara2 == 7 ))
	{
	}
	else ptara += 1.5;

	/*
	 * Yoni
	 */
	yoni1 = expert.getYoni( len1 );
	yoni2 = expert.getYoni( len2 );
	pyoni = calcYoniValue( yoni1, yoni2 );

	/*
	 * Graha Maitri
	 */
	mitra1 = getPlanetaryFriendship( getLord( rasi1 ), getLord( rasi2 ) );
	mitra2 = getPlanetaryFriendship( getLord( rasi2 ), getLord( rasi1 ) );
	//printf( "Graha maitri rasi1 %d lord1 %d rasi2 %d lord2 %d mitra1 %d mitra2 %d\n", rasi1, getLord(rasi1),rasi2,getLord(rasi2),mitra1,mitra2 );

	if ( mitra1 == PF_FRIEND && mitra2 == PF_FRIEND ) pmaitri = 5;
	else if (( mitra1 == PF_FRIEND && mitra2 == PF_NEUTRAL ) || ( mitra2 == PF_FRIEND && mitra1 == PF_NEUTRAL )) pmaitri = 4;
	else if (( mitra1 == PF_NEUTRAL && mitra2 == PF_NEUTRAL )
	         || ( mitra1 == PF_FRIEND && mitra2 == PF_ENEMY ) || ( mitra1 == PF_ENEMY && mitra2 == PF_FRIEND )) pmaitri = 2;
	else if (( mitra1 == PF_ENEMY && mitra2 == PF_NEUTRAL ) || ( mitra2 == PF_ENEMY && mitra1 == PF_NEUTRAL )) pmaitri = 1;
	else if ( mitra1 == PF_ENEMY && mitra2 == PF_ENEMY ) pmaitri = 0;
	else assert( 0 );

	/*
	 * Gana
	 */
	const int gana_map[3][3] =
	{
		{ 6, 5, 1 },
		{ 6, 6, 0 },
		{ 0, 0, 6 }
	};

	gana1 = expert.getGana( len1 );
	assert( gana1 >= 0 && gana1 < 3 );
	gana2 = expert.getGana( len2 );
	assert( gana2 >= 0 && gana2 < 3 );
	pgana = gana_map[gana1][gana2];

	/*
	 * Rasi Koota (or Bhakoota)
	 */
	int rasidiff = red12( rasi1 - rasi2 );
	if (( rasidiff == HOUSE1 )
	        || ( rasidiff == HOUSE3 )
	        || ( rasidiff == HOUSE4 )
	        || ( rasidiff == HOUSE7 )
	        || ( rasidiff == HOUSE10 )
	        || ( rasidiff == HOUSE11 ))
		prasi = 7;

	/*
	 * Nadi
	 */
	nadi1 = expert.getNadi( len1 );
	nadi2 = expert.getNadi( len2 );
	if ( nadi1 != nadi2 ) pnadi = 8;

	/*
	 * Rajju
	 */
	 rajju1 = expert.getRajju( nak1 );
	 rajju2 = expert.getRajju( nak2 );
	 prajju = calcRajjuValue( rajju1, rajju2 );

	/*
	 * Final
	 */
	ptotal = pvarna + pvashya + ptara + pyoni + pmaitri + prasi + pgana + pnadi + prajju;
}

/*****************************************************
**
**   AshtakootaExpert   ---   writeComprehensive
**
******************************************************/
void AshtakootaExpert::writeComprehensive( Sheet *sheet )
{
	wxString s;
	Lang lang;
	NakshatraExpert nexpert;

	Table *table = new Table( 7, 11 );
	table->setHeader( 0, _( "No." ));
	table->setHeader( 1, _( "Koota" ));
	table->setHeader( 2, _( "Description" ));
	table->setHeader( 3, _( "Criterium" ));
	table->setHeader( 4, _( "Groom" ));
	table->setHeader( 5, _( "Bride" ));
	table->setHeader( 6, _( "Result" ));

	for( int i = 1; i <= 9; i++ ) table->setEntry( 0, i, wxString::Format( wxT( "%d." ), i ));

	//varna
	table->setEntry( 1, 1, _( "Varna" ));
	table->setEntry( 2, 1, _( "Social matching" ));
	table->setEntry( 3, 1, _( "Caste associated with the Moon's sign" ));
	s.Clear();
	s << lang.getSignName( rasi1 ) << wxT( " / ")
		<< lang.getVarnaName( varna1 );
	table->setEntry( 4, 1, s );

	s.Clear();
	s << lang.getSignName( rasi2 ) << wxT( " / ")
		<< lang.getVarnaName( varna2 );
	table->setEntry( 5, 1, s );
	table->setEntry( 6, 1, wxString::Format( wxT( "%d / 1" ), pvarna ));

	// vashya
	table->setEntry( 1, 2, _( "Vashya" ));
	table->setEntry( 2, 2, _( "control, amenability" ));
	table->setEntry( 3, 2, _( "Vashya of Moon's sign" ));
	s.Clear();
	s << lang.getSignName( rasi1 ) << wxT( " / ")
		<< lang.getVashyaName( vashya1 );
	table->setEntry( 4, 2, s );

	s.Clear();
	s << lang.getSignName( rasi2 ) << wxT( " / ")
		<< lang.getVashyaName( vashya2 );
	table->setEntry( 5,	2, s );
	s.Clear();
	s << printfDouble( pvashya ) << wxT( " / 2" ); 
	table->setEntry( 6, 2, s );

	// Dina
	table->setEntry( 1, 3, wxT( "Dina (Tara)" ));
	table->setEntry( 2, 3, wxT( "health, family happiness" ));
	table->setEntry( 3, 3, wxT( "Tara of difference between Moon's Nakshatras" ));
	table->setEntry( 4, 3, lang.getTaraName( tara1 ) );
	table->setEntry( 5, 3, lang.getTaraName( tara2 ) );
	table->setEntry( 6, 3, wxString::Format( wxT( "%.1f / 3" ), ptara ));

	// yoni
	table->setEntry( 1, 4, wxT( "Yoni" ));
	table->setEntry( 2, 4, wxT( "sexual attraction" ));
	table->setEntry( 3, 4, wxT( "Animal symbol of the Moon's Nakshatra" ));
	s.Clear();
	s << lang.getNakshatra27Name( nak1 ) << wxT( " - " ) << yoni1.name;
	table->setEntry( 4, 4, s );
	s.Clear();
	s << lang.getNakshatra27Name( nak2 ) << wxT( " - " ) << yoni2.name;
	table->setEntry( 5, 4, s );
	table->setEntry( 6, 4, wxString::Format( wxT( "%d / 4" ), pyoni ));

	// graha
	table->setEntry( 1, 5, _( "Graha" ));
	table->setEntry( 2, 5, _( "mental affection" ));
	table->setEntry( 3, 5, _( "Mutual friendship of the Moon's sign lords" ));
	table->setEntry( 4, 5, lang.getObjectName( ::getLord( rasi1 ), TF_LONG ) );
	table->setEntry( 5, 5, lang.getObjectName( ::getLord( rasi2 ), TF_LONG ) );
	table->setEntry( 6, 5, wxString::Format( wxT( "%d / 5" ), pmaitri ));

	// gana
	table->setEntry( 1, 6, _( "Gana" ));
	table->setEntry( 2, 6, _( "character matching" ));
	table->setEntry( 3, 6, _( "Moon's Nakshatra" ));
	s.Clear();
	s << lang.getNakshatra27Name( nak1 ) << wxT( " - " ) << lang.getGanaName( gana1 );
	table->setEntry( 4, 6, s );
	s.Clear();
	s << lang.getNakshatra27Name( nak2 ) << wxT( " - " ) << lang.getGanaName( gana2 );
	table->setEntry( 5, 6, s );
	table->setEntry( 6, 6, wxString::Format( wxT( "%d / 6" ), pgana ));

	// rasi / bha
	table->setEntry( 1, 7, _( "Rasi (Bha)" ));
	table->setEntry( 2, 7, _( "general indications" ));
	table->setEntry( 3, 7, _( "Difference between the Moon's signs" ));
	s.Clear();
	table->setEntry( 4, 7, lang.getSignName( rasi1 ) );
	table->setEntry( 5, 7, lang.getSignName( rasi2 ) );
	table->setEntry( 6, 7, wxString::Format( wxT( "%d / 7" ), prasi ));

	// nadi
	table->setEntry( 1, 8, _( "Nadi" ));
	table->setEntry( 2, 8, _( "physiological matching, offspring" ));
	table->setEntry( 3, 8, _( "Moon's sign" ));
	s.Clear();
	s << lang.getSignName( rasi1 ) << wxT( " - ") << lang.getNadiName( nadi1 );
	table->setEntry( 4, 8, s );
	s.Clear();
	s << lang.getSignName( rasi2 ) << wxT( " - ") << lang.getNadiName( nadi2 );
	table->setEntry( 5, 8, s );
	table->setEntry( 6, 8, wxString::Format( wxT( "%d / 8" ), pnadi ));

	// rajju
	table->setEntry( 1, 9, _( "Rajju" ));
	table->setEntry( 2, 9, _( "general indications" ));
	table->setEntry( 3, 9, _( "Moon's Nakshatra" ));
	s.Clear();
	s << lang.getNakshatra27Name( nak1 ) << wxT( " - " )
		<< lang.getRajjuName( rajju1.aroha, rajju1.type );
	table->setEntry( 4, 9, s );
	s.Clear();
	s << lang.getNakshatra27Name( nak2 ) << wxT( " - " )
		<< lang.getRajjuName( rajju2.aroha, rajju2.type );
	table->setEntry( 5, 9, s );
	table->setEntry( 6, 9, wxString::Format( wxT( "%d / 4" ), prajju ));

	table->setHeaderEntry( 0, 10, wxT( "" ));
	table->setHeaderEntry( 1, 10, wxT( "" ));
	table->setHeaderEntry( 2, 10, wxT( "" ));
	table->setHeaderEntry( 3, 10, wxT( "" ));
	table->setHeaderEntry( 4, 10, wxT( "Total" ));
	double ptotal2 = .5 + 100.0 * ptotal / 40.0;
		table->setHeaderEntry( 5, 10, wxString::Format( wxT( "%d%%" ), (int)ptotal2 ));
		table->setHeaderEntry( 6, 10, wxString::Format( wxT( "%.1f/40" ), ptotal ));

	sheet->addItem( table );
}

/*****************************************************
**
**   AshtakootaExpert   ---   write
**
******************************************************/
void AshtakootaExpert::write( Sheet *sheet )
{
	wxString s;
	Lang lang;
	NakshatraExpert nexpert;

	int perc = (int)(100 * (double)ptotal / 40);

	//writer->addHeader( _( "Vedic Partner Chart" ));
	s.Printf( wxT( "%s: %s" ), _( "Groom" ), horoscope->getHName().c_str());
	sheet->addLine( s );
	s.Printf( wxT( "%s: %s" ), _( "Bride" ), h2->getHName().c_str());
	sheet->addParagraph( s );
	s.Clear();

	Table *table = new Table( 2, 11 );
	table->setHeader( 0, _( "Quality" ));
	table->setHeader( 1, _( "Points" ));

	table->setEntry( 0, 1, _( "Varna" ));
	s << pvarna << wxT( " / 1" );
	table->setEntry( 1, 1, s );
	s.Clear();

	table->setEntry( 0, 2, _( "Vashya" ));
	s << pvashya << wxT( " / 2" );
	table->setEntry( 1, 2, s );
	s.Clear();

	table->setEntry( 0, 3, _( "Tara" ));
	s << ptara << wxT( " / 3" );
	table->setEntry( 1, 3, s );
	s.Clear();

	table->setEntry( 0, 4, _( "Yoni" ));
	s << pyoni << wxT( " / 4" );
	table->setEntry( 1, 4, s );
	s.Clear();

	table->setEntry( 0, 5, _( "Graha Maitri" ));
	s << pmaitri << wxT( " / 5" );
	table->setEntry( 1, 5, s );
	s.Clear();

	table->setEntry( 0, 6, _( "Gana" ));
	s << pgana << wxT( " / 6" );
	table->setEntry( 1, 6, s );
	s.Clear();

	table->setEntry( 0, 7, _( "Bhakoota" ));
	s << prasi << wxT( " / 7" );
	table->setEntry( 1, 7, s );
	s.Clear();

	table->setEntry( 0, 8, _( "Nadi" ));
	s << pnadi << wxT( " / 8" );
	table->setEntry( 1, 8, s );
	s.Clear();

	table->setEntry( 0, 9, _( "Rajju" ));
	s << prajju << wxT( " / 4" );
	table->setEntry( 1, 9, s );
	s.Clear();

	table->setHeaderEntry( 0, 10, _( "Total" ));
	s << ptotal << wxT( " / 36 (" ) << perc << wxT( "%)" );
	//s << ptotal << wxT( " / 36" );
	table->setHeaderEntry( 1, 10, s );
	s.Clear();

	sheet->addItem( table );

	// Details about Moon #1
	sheet->addHeader( _( "Groom" ));

	double mlen1 = horoscope->getObjectPosition( OMOON, true ).longitude;
	Table *t1 = new Table( 2, 7 );
	t1->setHeader( 0, _( "Quality" ));
	t1->setHeader( 1, _( "Value" ));

	t1->setEntry( 0, 1, _( "Nakshatra" ));
	t1->setEntry( 1, 1, lang.getNakshatra27Name( ::getNakshatra27( mlen1 ), TF_LONG ));


	t1->setEntry( 0, 2, _( "Varna" ));
	t1->setEntry( 1, 2, lang.getVarnaName( getVarna( mlen1 )));

	Yoni yoni1 = nexpert.getYoni( mlen1 );
	t1->setEntry( 0, 3, _( "Yoni" ));
	t1->setEntry( 1, 3, yoni1.name );

	t1->setEntry( 0, 4, _( "Gana" ));
	t1->setEntry( 1, 4, lang.getGanaName( nexpert.getGana( mlen1 )));

	t1->setEntry( 0, 5, _( "Nadi" ));
	t1->setEntry( 1, 5, lang.getNadiName( nexpert.getNadi( mlen1 )));

	t1->setEntry( 0, 6, _( "Rajju" ));
	t1->setEntry( 1, 6, lang.getRajjuName( rajju1.aroha, rajju1.type ));

	sheet->addItem( t1 );

	// Details about Moon #2
	sheet->addHeader( _( "Bride" ));

	double mlen2 = h2->getObjectPosition( OMOON, true ).longitude;
	Table *t2 = new Table( 2, 7 );
	t2->setHeader( 0, _( "Quality" ));
	t2->setHeader( 1, _( "Value" ));

	t2->setEntry( 0, 1, _( "Nakshatra" ));
	t2->setEntry( 1, 1, lang.getNakshatra27Name( ::getNakshatra27( mlen2 ), TF_LONG ));

	t2->setEntry( 0, 2, _( "Varna" ));
	t2->setEntry( 1, 2, lang.getVarnaName( getVarna( mlen2 )));

	Yoni yoni2 = nexpert.getYoni( mlen2 );
	t2->setEntry( 0, 3, _( "Yoni" ));
	t2->setEntry( 1, 3, yoni2.name );

	t2->setEntry( 0, 4, _( "Gana" ));
	t2->setEntry( 1, 4, lang.getGanaName( nexpert.getGana( mlen2 )));

	t2->setEntry( 0, 5, _( "Nadi" ));
	t2->setEntry( 1, 5, lang.getNadiName( nexpert.getNadi( mlen2 )));

	t2->setEntry( 0, 6, _( "Rajju" ));
	t2->setEntry( 1, 6, lang.getRajjuName( rajju2.aroha, rajju2.type ));

	sheet->addItem( t2 );
}

/*****************************************************
**
**   CompositHoroscope   ---   Constructor
**
******************************************************/
CompositHoroscope::CompositHoroscope()
		: Horoscope()
{
}

/*****************************************************
**
**   CompositHoroscope   ---   update
**
******************************************************/
void CompositHoroscope::update( Horoscope *horoscope, Horoscope *h2 )
{
	int i;
	ObjectId p;
	double eps, r[13], houselen;
	Calculator *calculator = CalculatorFactory().getCalculator();
	VargaExpert expert( this );

	if ( ! horoscope || ! h2 ) return;

	iayanamsa = .5 * ( horoscope->getAyanamsa( true ) + horoscope->getAyanamsa( true ));
	wayanamsa = .5 * ( horoscope->getAyanamsa( false ) + horoscope->getAyanamsa( false ));
	siderealTime = .5 * ( horoscope->getSiderealTime() + h2->getSiderealTime());
	for ( i = OSUN; i < MAX_EPHEM_OBJECTS; i++ ) object_len[i] = calcGeometricalMidpoint( horoscope->object_len[ i ], h2->object_len[ i ] );

	double arm1 = horoscope->getSiderealTime() * 15;
	double arm2 = h2->getSiderealTime() * 15;
	double arm = calcGeometricalMidpoint( arm1, arm2 );
	calculator->calcEps( eps, .5 * ( horoscope->getJD() + h2->getJD() ));
	calculator->calcAscendantByArmc( arm, eps, .5 * ( horoscope->getLatitude() + h2->getLatitude()), r, config->westernCalculation->houseSystem );

	object_len[OASCENDANT] = r[HOUSE1];
	object_len[OMERIDIAN]  = r[HOUSE10];

	for ( i = 0; i < 12; i++ ) whousecusp[i] = red_deg( r[i] - wayanamsa );
	if ( config->vedicCalculation->houseSystem != config->westernCalculation->houseSystem )
		calculator->calcAscendantByArmc( arm, eps, .5 * ( horoscope->getLatitude() + h2->getLatitude()), r, config->vedicCalculation->houseSystem );
	for ( i = 0; i < 12; i++ ) ihousecusp[i] = red_deg( r[i] - iayanamsa );
	for ( i = 0; i < 12; i++ )
	{
		houselen = red_deg( ihousecusp[i<11 ? i+1 : 0] - ihousecusp[i] );
		ihousesandhi[i] = red_deg( ihousecusp[i] + .5 * houselen );
	}

	updateObjectPositions();
	updatePositionArrays();
	for ( p = OSUN; p < MAX_EPHEM_OBJECTS; p++ ) ephem_planets[p]->update( getObjectPosition( p, true ).longitude );
	for ( p = OSUN; p < MAX_EXTENDED_OBJECTS; p++ ) extended_planets[p]->update( getObjectPosition( p + FIRST_EXTENDED_OBJECT, true ).longitude );

	// Attention: must be separated from updateVargaData because Vargas reference each other
	for ( p = OSUN; p <= OSATURN; p++ ) ephem_planets[p]->calculateDignities();
}

/*****************************************************
**
**   CompositHoroscope   ---   dump
**
******************************************************/
void CompositHoroscope::dump( Sheet *sheet, const ChartProperties* chartprops, bool /*show_header*/ )
{
	vector<int> obs;
	ObjectPosition pos;
	int line = 0;
	SheetFormatter fmt;

	Table *table = new Table( 2, chartprops->getPlanetList().size() + 1 );
	//if ( show_header ) table->setHeader( _( "Composite Chart" ));
	table->setHeader( _( "Composite Planets" ));
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  _( "Longitude" ));

	for ( unsigned i = 0; i < chartprops->getPlanetList().size(); i++ )
	{
		line++;
		pos = getObjectPosition( chartprops->getPlanetList()[i], chartprops->isVedic() );
		table->setEntry( 0, line, fmt.getObjectName( chartprops->getPlanetList()[i], TF_MEDIUM ));
		table->setEntry( 1, line, fmt.getPosFormatted( pos.longitude, pos.direction, DEG_PRECISION_SECOND, TF_MEDIUM ));
	}
	sheet->addItem( table );
}


