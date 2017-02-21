/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/ShadBala.cpp
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

#include "ShadBala.h"

#include <wx/string.h>
#include <math.h>

#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "Hora.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Sheet.h"
#include "Table.h"
#include "Varga.h"
#include "VargaHoroscope.h"

extern Config *config;

// required shadbala in virupas
// su=390 mo=360 ma=300 me=420 ju=390 ve=330 sa=300
const int ShadBalaExpert::required_shadbala[7] = { 390, 360, 420, 330, 300, 390, 300 };

// circulation time of the objects in years, used by cheshta bala calculation
//su=1, mo=.082, ma=1.88, me=.24, ju=11.86, ve=.62, sa=29.46
const double ShadBalaExpert::circulation_time[7] = { 1, .082, .24, .62, 1.88, 11.86, 29.46 };

// types of cheshta bala
enum { CHESHTA_NULL, CHESHTA_VAKRA, CHESHTA_ANUVAKRA, CHESHTA_VIKALA, CHESHTA_MANDA, CHESHTA_MANDATARA, CHESHTA_SAMA, CHESHTA_CHARA,
	CHESHTA_ATICHARA }; 

/*****************************************************
**
**   ShadBalaExpert   ---   Constructor
**
******************************************************/
ShadBalaExpert::ShadBalaExpert( Horoscope *horoscope )
		: Expert( horoscope )
{
}

/*****************************************************
**
**   ShadBalaExpert   ---   clear
**
******************************************************/
void ShadBalaExpert::clear()
{
	for ( int i = OSUN; i <= OSATURN; i++ )
	{
		sthaanabala[i] = uchchabala[i] = samasaptavargajabala[i] = ojhajugmabala[i]
		                                 = kendradibala[i] = drekkanabala[i] = 0;
		digbala[i] = 0;
		kalabala[i] = nathonathabala[i] = pakshabala[i] = tribhagabala[i]
			= varshamasadinahorabala[i] = ayanabala[i] = yudhdhabala[i] = 0;
		percentual_speed[i] = speed[i] = cheshtabala[i] = naisargikabala[i] = drigbala[i] = shadbala[i] = 0;
		varshalord = masalord = dinalord = horalord = ONONE;
		cheshtabalatype[i] = CHESHTA_NULL;
		for ( int j = OSUN; j <= OSATURN; j++ )
		{
			drigbala_pair[i][j] = 0;
			saptavargajabala[i][j] = 0;
		}
	}
	max_saptavargajabala = 0;
	sunriseStatusOk = true;
}

/*****************************************************
**
**   ShadBalaExpert   ---   updateAllBalas
**
******************************************************/
void ShadBalaExpert::updateAllBalas()
{
	clear();

	updateSthaanaBala();
	updateDigBala();
	updateKalaBala();
	updateNaisargikaBala();
	updateCheshtaBala();
	updateDrigBala();

	for ( int i = OSUN; i <= OSATURN; i++ )
	{
		sthaanabala[i] = uchchabala[i] + samasaptavargajabala[i] + ojhajugmabala[i] + kendradibala[i] + drekkanabala[i];
		kalabala[i] = nathonathabala[i] + pakshabala[i] + tribhagabala[i] + varshamasadinahorabala[i] + ayanabala[i] + yudhdhabala[i];
		shadbala[i] = sthaanabala[i] + digbala[i] + kalabala[i] + cheshtabala[i] + naisargikabala[i] + drigbala[i];
	}
}

/*****************************************************
**
**   ShadBalaExpert   ---   updateSthaanaBala
**
******************************************************/
void ShadBalaExpert::updateSthaanaBala()
{
	int q, drekkana;
	ObjectId p;
	double a;

	// Exaltation points
	//su=10 mo=33 ma=298 me=165 ju=95 ve=357 sa=200 ra=285 ke=225
	const static int k_exaltation[9] = { 10, 33, 165, 357, 298, 95, 200, 285, 225 };

// Friends and Enemies

	VargaHoroscope rasichart( horoscope, V_RASI );
	VargaHoroscope navamsachart( horoscope, V_NAVAMSA );

	// Uchcha Bala
	for ( p = OSUN; p <= OSATURN; p++ )
	{
		a = red_deg( k_exaltation[p] - horoscope->getVedicLongitude( p ) - 180 );
		if ( a > 180 ) a = 360 - a;
		uchchabala[p] = a / 3;
		//printf( "Uchcha: p %d a %f uchcha %f\n", p, a, uchchabala[p] );
	}

	// Sapta Vargaja Bala
	for ( p = OSUN; p <= OSATURN; p++ )
	{
		saptavargajabala[p][0] = horoscope->getVargaData( p, V_RASI )->getSaptaVargajaBala();
		saptavargajabala[p][1] = horoscope->getVargaData( p, V_NAVAMSA )->getSaptaVargajaBala();
		saptavargajabala[p][2] = horoscope->getVargaData( p, V_HORA )->getSaptaVargajaBala();
		saptavargajabala[p][3] = horoscope->getVargaData( p, V_DREKKANA )->getSaptaVargajaBala();
		saptavargajabala[p][4] = horoscope->getVargaData( p, V_SAPTAMAMSA )->getSaptaVargajaBala();
		saptavargajabala[p][5] = horoscope->getVargaData( p, V_DVADASAMSA )->getSaptaVargajaBala();
		saptavargajabala[p][6] = horoscope->getVargaData( p, V_TRIMSAMSA )->getSaptaVargajaBala();
	}
	for ( p = OSUN; p <= OSATURN; p++ )
	{
		for ( q = 0; q < 7; q++ )
		{
			samasaptavargajabala[p] += saptavargajabala[p][q];
			max_saptavargajabala = Max( max_saptavargajabala, saptavargajabala[p][q] );
		}
	}

	// Ojhajugmariamsa Bala
	for ( p = OSUN; p <= OSATURN; p++ )
	{
		//double q = chart.getLongitude( p );
		if ( p == OVENUS || p == OMOON )
		{
			if ( rasichart.getRasi( p ) % 2 ) ojhajugmabala[p] += 15;
			if ( navamsachart.getRasi( p ) % 2 ) ojhajugmabala[p] += 15;
		}
		else
		{
			if ( rasichart.getRasi( p ) % 2 > 0 ) ojhajugmabala[p] += 15;
			if ( navamsachart.getRasi( p ) % 2 > 0 ) ojhajugmabala[p] += 15;
		}
	}

	// Kendradi Bala
	for ( p = OSUN; p <= OSATURN; p++ )
	{
		switch ( rasichart.getBhava( p ) % 3 )
		{
		case 0: // Kendra
			kendradibala[p] = 60;
			break;
		case 1:
			kendradibala[p] = 30;
			break;
		default:
			kendradibala[p] = 15;
			break;
		}
	}

	// Drekkana Bala
	for ( p = OSUN; p <= OSATURN; p++ )
	{
		a = rasichart.getRasiLongitude( p ) / 10.0;
		drekkana = (int)a;
		//printf( "Planet %d rasilen %f drekkana %d\n", p, a, drekkana );
		if ( p == OSUN || p == OMARS || p == OJUPITER )
		{
			if ( drekkana == 0 ) drekkanabala[p] = 15;
		}
		else if ( p == OVENUS || p == OMOON )
		{
			if ( drekkana == 1 ) drekkanabala[p] = 15;
		}
		else if ( p == OSATURN || p == OMERCURY )
		{
			if ( drekkana == 2 ) drekkanabala[p] = 15;
		}
		else assert( false );
	}
}

/*****************************************************
**
**   ShadBalaExpert   ---   updateDigBala
**
******************************************************/
void ShadBalaExpert::updateDigBala()
{
	double weakestHouse, housepos, bala;

	for ( ObjectId p = OSUN; p <= OSATURN; p++ )
	{
		switch ( p )
		{
		case OSUN:
		case OMARS:
			weakestHouse = HOUSE4;
			break;
		case OJUPITER:
		case OMERCURY:
			weakestHouse = HOUSE7;
			break;
		case OMOON:
		case OVENUS:
			weakestHouse = HOUSE10;
			break;
		case OSATURN:
			weakestHouse = HOUSE1;
			break;
		default:
			break;
		}
		housepos = horoscope->getHouse4Longitude( horoscope->getVedicLongitude( p ), true );

		// distance of planet from weakest house, range 0..6
		bala = a_red( housepos - weakestHouse, 12 );
		if ( bala > 6 ) bala = 12 - bala;

		digbala[p] = bala * 10;
		//printf( "ShadBalaExpert::updateDigBala planet %d housepos %f weakest %f bala %f\n", p, housepos, weakestHouse, bala );
	}
}

/*****************************************************
**
**   ShadBalaExpert   ---   updateKalaBala
**
******************************************************/
void ShadBalaExpert::updateKalaBala()
{
	double paskha_value, natabala, sunrise, sunset, d;
	double eps, tlen, lat, kranti;
	int daypart, nightpart;
	bool daybirth;

	Calculator *calculator = CalculatorFactory().getCalculator();

	// Nathonahta Bala
	nathonathabala[OMERCURY] = 60;

	natabala = horoscope->getDataSet()->getLocalMeanTime();
	if ( natabala > 12 ) natabala = 24 - natabala;
	natabala *= 5;

	nathonathabala[OMOON] = nathonathabala[OMARS] = nathonathabala[OSATURN] = natabala;
	nathonathabala[OSUN] = nathonathabala[OJUPITER] = nathonathabala[OVENUS] = 60 - natabala;

	// pakshabala
	paskha_value = planetDistance( horoscope->getVedicLongitude( OSUN ), horoscope->getVedicLongitude( OMOON ) ) / 3;

	pakshabala[OSUN]= 60 - paskha_value;
	pakshabala[OMOON]= ( horoscope->isBenefic( OMOON ) ? paskha_value : 60 - paskha_value );
	pakshabala[OMARS]= 60 - paskha_value;
	pakshabala[OMERCURY]= ( horoscope->isBenefic( OMERCURY ) ? paskha_value : 60 - paskha_value );;
	pakshabala[OJUPITER]= paskha_value;
	pakshabala[OVENUS]= paskha_value;
	pakshabala[OSATURN]= 60 - paskha_value;

	// tribhagabala
	sunrise = horoscope->getSunrise();
	sunset = horoscope->getSunset();
	sunriseStatusOk = ( sunrise != 0 && sunset != 0 );

	if ( sunriseStatusOk )
	{
		tribhagabala[OJUPITER] = 60; // Jupiter has always Tribhaga Bala of 1 Rupa
		daybirth = horoscope->getDayBirth();
		//printf( "Day %d sunrise %9.9f sunset %9.9f JD %9.9f\n", daybirth, sunrise, sunset, horoscope->getJD());
		if ( daybirth )
		{
			assert( sunrise < horoscope->getJD() && sunset > horoscope->getJD());
			d =  ( horoscope->getJD() - sunrise ) / ( sunset - sunrise );
			daypart = (int)( d * 3 );
			//printf( "Daypart: %d\n", daypart );
			if ( daypart == 0 ) {
				tribhagabala[OMERCURY] = 60;
			}
			else if ( daypart == 1 ) {
				tribhagabala[OSUN] = 60;
			}
			else if ( daypart == 2 ) {
				tribhagabala[OSATURN] = 60;
			}
		}
		else
		{
			assert( sunrise > horoscope->getJD() && sunset < horoscope->getJD());
			d =  ( horoscope->getJD() - sunset ) / ( sunrise - sunset );
			nightpart = (int)( d * 3 );
			//printf( "Nightpart: %d\n", nightpart );
			if ( nightpart == 0 ) {
				tribhagabala[OMOON] = 60;
			}
			else if ( nightpart == 1 ) {
				tribhagabala[OVENUS] = 60;
			}
			else if ( nightpart == 2 ) {
				tribhagabala[OMARS] = 60;
			}
		}
	}

	// Lords of year, month, day and hour
	HoraExpert hexpert;
	hexpert.setLocation( horoscope->getLocation() );
	hexpert.update( horoscope->getJD() );

	varshalord = hexpert.getVarshaLord();
	varshamasadinahorabala[varshalord] += 15;

	masalord = hexpert.getMasaLord();
	varshamasadinahorabala[masalord] += 30;

	dinalord = hexpert.getDinaLord();
	varshamasadinahorabala[dinalord] += 45;

	horalord = hexpert.getHoraLord();
	varshamasadinahorabala[horalord] += 60;

	// Ayana Bala
	calculator->calcEps( eps, horoscope->getJD());
	for ( ObjectId p = OSUN; p <= OSATURN; p++ )
	{
		ayanabala[p] = eps;

		tlen = horoscope->getTropicalLongitude( p );
		lat = horoscope->getLatitude( p );
		kranti = lat + eps * sin( DEG2RAD * tlen );

		switch ( p )
		{
		case OMERCURY:
			ayanabala[p] += fabs( kranti );
			break;
		case OMOON:
		case OSATURN:
			ayanabala[p] -= kranti;
			break;
		default:
			ayanabala[p] += kranti;
			break;
		}

		ayanabala[p] *= 1.2793;
		ayanabala[p] = Max( ayanabala[p], 0 );
	}

	// yudhdhabala 
	{
		// start with kalabala aquired so far
		// in case of planetary war the difference of both values must be added to the winner, substractcted from the looser
		double kb[7];
		for ( int i = OSUN; i <= OSATURN; i++ )
		{
			kb[i] = nathonathabala[i] + pakshabala[i] + tribhagabala[i] + varshamasadinahorabala[i] + ayanabala[i];
		}
		double dist, shadbaladiff;
		ObjectId pwinner, ploser;
		double len1, len2;

		for ( ObjectId p1 = OMERCURY; p1 <= OSATURN; p1++ )
		{
			for ( ObjectId p2 = p1; p2 <= OSATURN; p2++ )
			{
				if ( p1 == p2 ) continue;

				len1 = horoscope->getVedicLongitude( p1 );
				len2 = horoscope->getVedicLongitude( p2 );
				dist = planetDistance( len1, len2 );
				//printf( "Yuddha Bala %d %d dist %f\n", p1, p2, dist );

				if ( dist < 1 )
				{
					shadbaladiff = fabs( kb[p1] - kb[p2] );

					// winner is the planet with higer longitude
					if (( len1 > len2 ) | ( len1 < 1 && len2 >= 359 ))
					{
						pwinner = p1;
						ploser = p2;
					}
					else
					{
						pwinner = p2;
						ploser = p1;
					}
					yudhdhabala[ pwinner ] = shadbaladiff;
					yudhdhabala[ ploser ] = -shadbaladiff;
					
					//printf( "MATCH %d %d dist %f winner %d loser %d\n", p1, p2, dist, pwinner, ploser );
				}
				
			}
		}
	}
}

/*****************************************************
**
**   ShadBalaExpert   ---   updateNaisargikaBala
**
******************************************************/
void ShadBalaExpert::updateNaisargikaBala()
{
	// Order: sun, moon, venus, jupiter, mercury, mars, saturn
	const int nbala_base[7] = { 0, 1, 4, 2, 5, 3, 6 };
	int p;
	for ( p = OSUN; p <= OSATURN; p++ )
	{
		naisargikabala[p] = (double)( 7 - nbala_base[p] ) * 60.0 / 7.0;
	}
}

/*****************************************************
**
**   ShadBalaExpert   ---   updateCheshtaBala
**
******************************************************/
void ShadBalaExpert::updateCheshtaBala()
{
	int count, rasi, baserasi;
	double len, lat, medium_speed, jd, speed1;
	Calculator *calculator = CalculatorFactory().getCalculator();

	DataSet ds( *horoscope->getDataSet());

	cheshtabala[OSUN] = ayanabala[OSUN];
	cheshtabala[OMOON] = pakshabala[OMOON];
	for ( ObjectId p = OMERCURY; p <= OSATURN; p++ )
	{
		speed[p] = horoscope->getSpeed( p );
		medium_speed = 1 / circulation_time[p];
		percentual_speed[p] = 100 * speed[p] / medium_speed;
		if ( speed[p] > 0 ) // direct motion
		{
			//printf( "DIRECT Speed %d: is %f -- medium: %f -- percentual_speed = %f\n", p, speed[p], medium_speed, percentual_speed[p] );
			if ( fabs( percentual_speed[p] ) < 10 ) // Vikala, devoid of motion
			{
				cheshtabalatype[p] = CHESHTA_VIKALA;
				cheshtabala[p] = 15;
			}
			else if ( percentual_speed[p] < 50 ) // Mandatara
			{
				cheshtabalatype[p] = CHESHTA_MANDATARA;
				cheshtabala[p] = 15;
			}
			else if ( percentual_speed[p] < 100 ) // Manda
			{
				cheshtabalatype[p] = CHESHTA_MANDA;
				cheshtabala[p] = 30;
			}
			else if ( percentual_speed[p] < 150 ) // Sama
			{
				cheshtabalatype[p] = CHESHTA_SAMA;
				cheshtabala[p] = 7.5;
			}
			else if ( percentual_speed[p] >= 150 ) // Chara
			{
				cheshtabalatype[p] = CHESHTA_CHARA;
				cheshtabala[p] = 45;

				// Atichara
				baserasi = ::getRasi( horoscope->getVedicLongitude( p ));
				count = 0;
				jd = horoscope->getJD();
				speed1 = speed[p];
				while ( speed1 > 0 && count++ < 90 )
				{
					jd += 3.0;
					ds.setDate( jd );
					calculator->calcPositionSpeed( &ds, p, len, lat, speed1, true, true );
					rasi = getRasi( len );
					//printf( "  -> In Atichara Calc #%d Speed %f len %f rasi %d / baserasi %d\n", count, speed1, len, rasi, baserasi );
					if ( rasi != baserasi )
					{
						//printf ( "Rasi CHANGED IN DIRECT MOTION ->IN ATICHARA\n" );
						cheshtabalatype[p] = CHESHTA_ATICHARA;
						cheshtabala[p] = 30;
						break;
					}
				}
			}
		}
		else
		{
			cheshtabalatype[p] = CHESHTA_VAKRA;
			cheshtabala[p] = 60;
			//printf( "RETRO Speed %d: is %f -- medium: %f -- percentual_speed = %f\n", p, speed[p], medium_speed, percentual_speed[p] );

			// anuvakra
			baserasi = ::getRasi( horoscope->getVedicLongitude( p ));
			count = 0;
			jd = horoscope->getJD();
			speed1 = speed[p];
			while ( speed1 < 0 && count++ < 90 )
			{
				jd += 3.0;
				ds.setDate( jd );
				calculator->calcPositionSpeed( &ds, p, len, lat, speed1, true, true );
				rasi = getRasi( len );
				//printf( "  -> In Retro Calc #%d Speed %f len %f rasi %d / baserasi %d\n", count, speed1, len, rasi, baserasi );
				if ( rasi != baserasi )
				{
					//printf ( "Rasi CHANGED IN RETRO -> ANUVAKRA\n" );
					cheshtabalatype[p] = CHESHTA_ANUVAKRA;
					cheshtabala[p] = 30;
					break;
				}
			}
		}
	}
}

/*****************************************************
**
**   ShadBalaExpert   ---   updateDrigBala
**
******************************************************/
void ShadBalaExpert::updateDrigBala()
{
	ObjectId aspectingPlanet, aspectedPlanet;
	int rasidiff;
	//const char *pp[7] = { "Sun", "Moon", "Mercury", "Venus", "Mars", "Jupiter", "Saturn" };

	for ( aspectingPlanet = OSUN; aspectingPlanet <= OSATURN; aspectingPlanet++ )
	{
		for ( aspectedPlanet = OSUN; aspectedPlanet <= OSATURN; aspectedPlanet++ )
		{
			if ( aspectingPlanet == aspectedPlanet ) continue;

			rasidiff = red12( getRasi( horoscope->getVedicLongitude( aspectedPlanet )) - getRasi( horoscope->getVedicLongitude( aspectingPlanet )));

			//printf( "Drig Bala: %s - %s DiFF %d aspectvalue %d\n", pp[aspectingPlanet], pp[aspectedPlanet], rasidiff, (int)60.0 * getGrahaDrishtiValue( aspectingPlanet, rasidiff ) );

			drigbala_pair[aspectedPlanet][aspectingPlanet] = 60.0 * getGrahaDrishtiValue( aspectingPlanet, rasidiff );


			if ( drigbala_pair[aspectedPlanet][aspectingPlanet] > 0 )
			{
				const bool maleficAspect = ( aspectingPlanet == OSUN
			        || ( aspectingPlanet == OMOON && ! horoscope->isBenefic( OMOON ))
			        || aspectingPlanet == OMARS
			        || ( aspectingPlanet == OMERCURY && ! horoscope->isBenefic( OMERCURY ))
			        || aspectingPlanet == OSATURN );

				if ( maleficAspect ) drigbala_pair[aspectedPlanet][aspectingPlanet] -= 15;
				else drigbala_pair[aspectedPlanet][aspectingPlanet] += 15;
			}

/*
			// Negative values for malefic aspects
			if ( aspectingPlanet == OSUN
			        || ( aspectingPlanet == OMOON && ! horoscope->isBenefic( OMOON ))
			        || aspectingPlanet == OMARS
			        || ( aspectingPlanet == OMERCURY && ! horoscope->isBenefic( OMERCURY ))
			        || aspectingPlanet == OSATURN
			   )
			{
				drigbala_pair[aspectedPlanet][aspectingPlanet] *= -1;
			}
*/

			drigbala[aspectedPlanet] += drigbala_pair[aspectedPlanet][aspectingPlanet];
		}
	}
}

/*****************************************************
**
**   ShadBalaExpert   ---   getPercentualShadBala
**
******************************************************/
double ShadBalaExpert::getPercentualShadBala( const int &planet ) const
{
	ASSERT_VALID_SHADBALA_INDEX( planet );
	return shadbala[planet] * 100.0 / required_shadbala[planet] ;
}

/*****************************************************
**
**   ShadBalaExpert   ---   write
**
******************************************************/
void ShadBalaExpert::write( Sheet *sheet )
{
	Formatter *formatter = Formatter::get();
	uint i, j;
	wxString s;
	ObjectId p;
	Lang lang;
	SheetFormatter fmt;
	const static wxString cheshtabala_name[9] = { wxEmptyString, _( "Vakra" ), _( "Anuvakra" ),
		_( "Vikala" ), _( "Manda" ), _( "Mandatara" ), _( "Sama" ), _( "Chara" ), _( "Atichara" ) };

	// get correct order for planets
	ObjectArray obs = PlanetList().getVedicPlanetOnlyList();

	Table *t0 = new Table( 13, 8 );
	t0->setHeader( _( "Six Balas" ));
	t0->setHeader( 0, wxEmptyString );
	t0->setHeader( 1, _( "Sthana" ));
	t0->setHeader( 2, _( "Dig" ));
	t0->setHeader( 3, _( "Kala" ));
	t0->setHeader( 4, _( "Cheshta" ));
	t0->setHeader( 5, _( "Naisargika" ));
	t0->setHeader( 6, _( "Drik" ));
	t0->setHeader( 7, wxEmptyString );
	t0->setHeader( 8, _( "Rupas" ));
	t0->setHeader( 9, _( "Virupas" ));
	t0->setHeader( 10, _( "Required" ));
	t0->setHeader( 11, _( "Strength (Percent)" ));
	t0->setHeader( 12, wxEmptyString );
	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		t0->setEntry( 0, i+1, fmt.getObjectName( p, TF_LONG ));

		t0->setEntry( 1, i+1, wxString::Format( wxT( "%.0f" ), getSthaanaBala( p ) ));
		t0->setEntry( 2, i+1, wxString::Format( wxT( "%.0f" ), getDigBala( p )));
		t0->setEntry( 3, i+1, wxString::Format( wxT( "%.0f" ), getKalaBala( p )));
		t0->setEntry( 4, i+1, wxString::Format( wxT( "%.0f" ), getCheshtaBala( p )));
		t0->setEntry( 5, i+1, wxString::Format( wxT( "%.0f" ), getNaisargikaBala( p )));
		t0->setEntry( 6, i+1, wxString::Format( wxT( "%.0f" ), getDrikBala( p )));

		t0->setEntry( 8, i+1, wxString::Format( wxT( "%.1f" ), getShadBala( p ) / 60.0 ));
		t0->setEntry( 9, i+1, wxString::Format( wxT( "%.0f" ), getShadBala( p )));
		t0->setEntry( 10, i+1, wxString::Format( wxT( "%d" ), required_shadbala[p] ));
		t0->setEntry( 11, i+1, wxString::Format( wxT( "%.1f%%" ), getPercentualShadBala( p ) ));
		t0->setEntry( 12, i+1, fmt.getObjectName( p, TF_LONG ));
	}
	sheet->addItem( t0 );

	Table *t1 = new Table( 10, 8 );
	t1->setHeader( _( "Sthana Bala" ));
	t1->setHeader( 0, wxEmptyString );
	t1->setHeader( 1, _( "Uchcha" ));
	t1->setHeader( 2, _( "Saptavargaja" ));
	t1->setHeader( 3, _( "Ojhayugma" ));
	t1->setHeader( 4, _( "Kendradi" ));
	t1->setHeader( 5, _( "Drekkana" ));
	t1->setHeader( 6, wxEmptyString );
	t1->setHeader( 7, _( "Rupas" ));
	t1->setHeader( 8, _( "Virupas" ));
	t1->setHeader( 9, wxEmptyString );
	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		t1->setEntry( 0, i+1, fmt.getObjectName( p, TF_LONG ));

		t1->setEntry( 1, i+1, wxString::Format( wxT( "%.0f" ), getUchchaBala( p )));
		t1->setEntry( 2, i+1, wxString::Format( wxT( "%.0f" ), getSaptavargajaBala( p )));
		t1->setEntry( 3, i+1, wxString::Format( wxT( "%.0f" ), getOjhajugmaBala( p )));
		t1->setEntry( 4, i+1, wxString::Format( wxT( "%.0f" ), getKendradiBala( p ) ));
		t1->setEntry( 5, i+1, wxString::Format( wxT( "%.0f" ), getDrekkanaBala( p ) ));

		t1->setEntry( 7, i+1, wxString::Format( wxT( "%.1f" ), getSthaanaBala( p ) / 60 ));
		t1->setEntry( 8, i+1, wxString::Format( wxT( "%.0f" ), getSthaanaBala( p )));
		t1->setEntry( 9, i+1, fmt.getObjectName( p, TF_LONG ));
	}
	sheet->addItem( t1 );

	Table *t2 = new Table( 11, 8 );
	t2->setHeader( _( "Saptavargaja Bala" ));
	t2->setHeader( 0, wxEmptyString );
	t2->setHeader( 1, wxT( "D-1" ));
	t2->setHeader( 2, wxT( "D-9" ));
	t2->setHeader( 3, wxT( "D-2" ));
	t2->setHeader( 4, wxT( "D-3" ));
	t2->setHeader( 5, wxT( "D-7" ));
	t2->setHeader( 6, wxT( "D-12" ));
	t2->setHeader( 7, wxT( "D-30" ));
	t2->setHeader( 8, wxEmptyString );
	t2->setHeader( 9, _( "Total Virupas" ));
	t2->setHeader( 10, wxEmptyString );

	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		t2->setEntry( 0, i+1, fmt.getObjectName( p, TF_LONG ));

		for ( j = 0; j < 7; j++ ) // 7 Vargas
		{
			t2->setEntry( j + 1, i+1, wxString::Format( wxT( "%.0f" ), getSaptavargajaBala( p, j )));
		}
		t2->setEntry( 9, i+1, wxString::Format( wxT( "%.0f" ), getSaptavargajaBala( p )));
		t2->setEntry( 10, i+1, fmt.getObjectName( p, TF_LONG ));
	}
	sheet->addItem( t2 );

	Table *t3 = new Table( 11, 8 );
	t3->setHeader( _( "Kala Bala" ));
	t3->setHeader( 0, wxEmptyString );
	t3->setHeader( 1, _( "Nathonatha" ));
	t3->setHeader( 2, _( "Paksha" ));
	t3->setHeader( 3, _( "Tribhaga" ));
	t3->setHeader( 4, _( "Varsha-Masa-Dina-Hora" ));
	t3->setHeader( 5, _( "Ayana" ));
	t3->setHeader( 6, _( "Yudhdha" ));
	t3->setHeader( 7, wxEmptyString );
	t3->setHeader( 8, _( "Total Rupas" ));
	t3->setHeader( 9, _( "Virupas" ));
	t3->setHeader( 10, wxEmptyString );
	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		t3->setEntry( 0, i+1, fmt.getObjectName( p, TF_LONG ));

		t3->setEntry( 1, i+1, wxString::Format( wxT( "%.0f" ), getNathonathaBala( p )));
		t3->setEntry( 2, i+1, wxString::Format( wxT( "%.0f" ), getPakshaBala( p )));
		t3->setEntry( 3, i+1, wxString::Format( wxT( "%.0f" ), getTribhagaBala( p )));
		t3->setEntry( 4, i+1, wxString::Format( wxT( "%.0f" ), getVarshaMasaDinaHoraBala( p )));
		t3->setEntry( 5, i+1, wxString::Format( wxT( "%.0f" ), getAyanaBala( p )));
		t3->setEntry( 6, i+1, wxString::Format( wxT( "%.0f" ), getYudhdhaBala( p )));

		t3->setEntry( 8, i+1, wxString::Format( wxT( "%.1f" ), getKalaBala( p ) / 60.0 ));
		t3->setEntry( 9, i+1, wxString::Format( wxT( "%.0f" ), getKalaBala( p )));
		t3->setEntry( 10, i+1, fmt.getObjectName( p, TF_LONG ));
	}
	sheet->addItem( t3 );

	if ( ! sunriseStatusOk ) sheet->addLine( _( "Remark: no Tribhaga Bala, sunrise and sunset not found." ));


	s.Clear();
	s << _( "Lord of year" ) << wxT( ": " ) << lang.getObjectName( varshalord, TF_LONG );
	sheet->addLine( s );

	s.Clear();
	s << _( "Lord of month" ) << wxT( ": " ) << lang.getObjectName( masalord, TF_LONG );
	sheet->addLine( s );

	s.Clear();
	s << _( "Lord of day" ) << wxT( ": " ) << lang.getObjectName( dinalord, TF_LONG );
	sheet->addLine( s );

	s.Clear();
	s << _( "Lord of hour" ) << wxT( ": " ) << lang.getObjectName( horalord, TF_LONG );
	sheet->addLine( s );

	Table *t4 = new Table( 6, 6 );
	t4->setHeader( _( "Cheshta Bala" ));
	t4->setHeader( 0, wxEmptyString );
	t4->setHeader( 1, _( "Speed (deg/day)" ));
	t4->setHeader( 2, _( "Medium Speed" ));
	t4->setHeader( 3, _( "Ratio" ));
	t4->setHeader( 4, _( "Type" ));
	t4->setHeader( 5, _( "Virupas" ));
	int line = 1;

	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		if ( p == OSUN || p == OMOON ) continue; // no such Bala for Sun and Moon
		t4->setEntry( 0, line, fmt.getObjectName( p, TF_LONG ));

		t4->setEntry( 1, line, formatter->getLenFormatted( speed[p], DEG_PRECISION_SECOND ) );
		t4->setEntry( 2, line, formatter->getLenFormatted( 1 / circulation_time[p], DEG_PRECISION_SECOND ) );

		t4->setEntry( 3, line, wxString::Format( wxT( "%.1f%%" ), percentual_speed[p] ));
		t4->setEntry( 4, line, cheshtabala_name[cheshtabalatype[p]] );

		t4->setEntry( 5, line, wxString::Format( wxT( "%.1f" ), cheshtabala[p] ));
		line++;
	}
	sheet->addItem( t4 );

	Table *t5 = new Table( 8, 9 );
	t5->setHeader( _( "Drig Bala" ));
	t5->setHeader( 0,  wxEmptyString );
	t5->setEntry( 0, 8, _( "Sum" ));
	for ( i = OSUN; i <= OSATURN; i++ ) t5->setHeader( i+1, fmt.getObjectName( obs[i], TF_LONG ));
	line = 1;
	for ( i = 0; i < obs.size(); i++ )
	{
		p = obs[i];
		t5->setEntry( 0, line, fmt.getObjectName( p, TF_LONG ) );
		for ( j = 0; j < obs.size(); j++ )
		{
			if ( i == j ) continue;
			t5->setEntry( j+1, line, wxString::Format( wxT( "%02d" ), (int)drigbala_pair[obs[j]][p] ));
		}
		t5->setEntry( i + 1, 8, wxString::Format( wxT( "%02d" ), (int)drigbala[p] ) );
		line++;
	}
	sheet->addItem( t5 );
}

