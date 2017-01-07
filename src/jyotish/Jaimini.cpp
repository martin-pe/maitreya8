/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Jaimini.cpp
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

#include "Jaimini.h"

#include <wx/string.h>

#include "astrobase.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "maitreya.h"
#include "mathbase.h"
#include "PlanetList.h"
#include "Sheet.h"
#include "Table.h"
#include "VargaBase.h"
#include "VargaHoroscope.h"

extern Config *config;

/*****************************************************
**
**   JaiminiExpert   ---   update
**
******************************************************/
void JaiminiExpert::update()
{
	const ObjectId inode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const ObjectId descnode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;

	// number of planets in the 12 Rasis
	// based on the seven planets Sun..Saturn
	for ( int i = 0; i < 12; i++ ) nbplanets[i] = 0;

	const ObjectArray obs = PlanetList().getDefaultVedicObjectList();

	for ( uint p = 0; p < obs.size(); p++ ) { nbplanets[this->getRasi(obs[p])]++; }

	// number of aspscts on Saturn, Rahu, Mars and Ketu
	aspSat = calcPlanetAspects( OSATURN );
	aspRah = calcPlanetAspects( inode );
	aspMar = calcPlanetAspects( OMARS );
	aspKet = calcPlanetAspects( descnode );

	calcNodeStrength();

	for ( Rasi i = R_ARIES; i <= R_PISCES; i++ )
	{
		nbAspectsOnRasi[i] = calcSignAspects( i );
	}
	calcYears();
	calcCharaKarakas();

}

/*****************************************************
**
**   JaiminiExpert   ---   calcPada
**
******************************************************/
int JaiminiExpert::calcPada( const int &bhava )
{
	assert(( bhava < 12 ) && ( bhava >= 0 ));

	Rasi rasi = (Rasi)red12( this->getRasi( OASCENDANT ) + bhava );
	ObjectId lrd = getJaiminiLord( rasi );
	int diff = red12( this->getRasi( lrd ) - rasi );
	int pada = red12( rasi + 2 * diff );
	if ( ! config->vedicCalculation->arudhaMode && ! ( diff % 3 )) pada = red12( pada + 9 );
	return pada;
}


/*****************************************************
**
**   JaiminiExpert   ---   getStrongerRasi
**
******************************************************/
int JaiminiExpert::getStrongerRasi( const Rasi &rasi1, const Rasi &rasi2 )
{
	ASSERT_VALID_RASI( rasi1 );
	ASSERT_VALID_RASI( rasi2 );

	// Rule 1a
	if ( nbplanets[rasi1] > nbplanets[rasi2] )
	{
		return rasi1;
	}
	// Rule 1b
	else if ( nbplanets[rasi1] < nbplanets[rasi2] )
	{
		return rasi2;
	}
	// Rule 2a
	else if ( nbAspectsOnRasi[rasi1] > nbAspectsOnRasi[rasi2] )
	{
		return rasi1;
	}
	// Rule 2b
	else if ( nbAspectsOnRasi[rasi1] < nbAspectsOnRasi[rasi2] )
	{
		return rasi2;
	}
	// **** TODO RULE3
	// Rule 4a
	else if ( this->getRasiLongitude(getJaiminiLord( rasi1 )) > this->getRasiLongitude(getJaiminiLord(rasi2 )) )
	{
		return rasi1;
	}
	// Rule 4b
	else if ( this->getRasiLongitude(getJaiminiLord( rasi1 )) < this->getRasiLongitude(getJaiminiLord(rasi2 )) )
	{
		return rasi2;
	}
	else assert( false );
	return 0;
}

/*****************************************************
**
**   JaiminiExpert   ---   getJaiminiLord
**
******************************************************/
ObjectId JaiminiExpert::getJaiminiLord( const Rasi &rasi )
{
	const ObjectId inode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const ObjectId descnode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;
	ObjectId lrd = ::getLord( rasi );
	if (( rasi == R_SCORPIO ) && ( ketuStrongerThanMars )) lrd = descnode;
	if (( rasi == R_AQUARIUS ) && ( rahuStrongerThanSaturn )) lrd = inode;

	return lrd;
}

/*****************************************************
**
**   JaiminiExpert   ---   isKetuStrongerThanMars
**
******************************************************/
bool JaiminiExpert::isKetuStrongerThanMars()
{
	return ketuStrongerThanMars;
}

/*****************************************************
**
**   JaiminiExpert   ---   isRahuStrongerThanSaturn
**
******************************************************/
bool JaiminiExpert::isRahuStrongerThanSaturn()
{
	return rahuStrongerThanSaturn;
}

/*****************************************************
**
**   JaiminiExpert   ---   write
**
******************************************************/
void JaiminiExpert::write( Sheet *sheet, const bool show_header )
{
	int i;
	ObjectId k_id;
	Lang lang;
	VargaConfigLoader *loader = VargaConfigLoader::get();
	wxString s;
	SheetFormatter fmt;

	if ( show_header )
	{
		sheet->addHeader ( _( "Jaimini" ));
		s.Printf( wxT( "%s: %s"), _( "Varga"), loader->getVargaName( varga ).c_str());
		sheet->addLine( s );
	}

	// 7 planets in Raman mode, Rahu included otherwise
	const int max_karakas = config->vedicCalculation->charaKarakaMode ? GNATI_KARAKA : DARA_KARAKA;

	Table *t1 = new Table( 2, 9 );
	t1->setHeader( _( "Chara Karakas" ));
	t1->setHeader( 0, _( "Karaka" ));
	t1->setHeader( 1, _( "Planet" ));
	int line = 1;
	for ( i = ATMA_KARAKA; i <= max_karakas; i++ )
	{
		t1->setEntry( 0, line, lang.getKarakaName( i ) );
		k_id = getCharaKaraka(i);
		if ( k_id > OSATURN ) k_id = OMEANNODE;
		t1->setEntry( 1, line, fmt.getObjectName( k_id, TF_LONG, true ));
		line++;
	}
	sheet->addItem( t1 );

	Table *t2 = new Table( 2, 3 );
	t2->setHeader( _( "Stronger Planets" ));
	t2->setHeader( 0, _( "Pair" ));
	t2->setHeader( 1, _( "Stronger" ));
	t2->setEntry( 0, 1, _( "Rahu/Saturn" ));
	t2->setEntry( 0, 2, _( "Ketu/Mars" ));

	//t2->setEntry( 1, 1, ( isRahuStrongerThanSaturn() ? writer->getObjectName( OMEANNODE, TF_LONG, true ) : writer->getObjectName( OSATURN, TF_LONG )) );
	if ( isRahuStrongerThanSaturn()) t2->setEntry( 1, 1, fmt.getObjectName( OMEANNODE, TF_LONG, true ));
	else t2->setEntry( 1, 1, fmt.getObjectName( OSATURN, TF_LONG ) );

	//t2->setEntry( 1, 2, ( isKetuStrongerThanMars() ? writer->getObjectName( OMEANDESCNODE, TF_LONG, true ) : writer->getObjectName( OMARS, TF_LONG )));
	if ( isKetuStrongerThanMars()) t2->setEntry( 1, 2, fmt.getObjectName( OMEANDESCNODE, TF_LONG, true ));
	else t2->setEntry( 1, 2, fmt.getObjectName( OMARS, TF_LONG ));
	sheet->addItem( t2 );

	Table *t3 = new Table( 2, 13 );
	t3->setHeader( _( "Arudha Padas" ));
	t3->setHeader( 0, _( "House" ));
	t3->setHeader( 1, _( "Rasi" ));
	for ( i = 0; i < 12; i++ )
	{
		s.Printf( wxT( "%02d " ), i+1 );
		t3->setEntry( 0, i+1, s );
		t3->setEntry( 1, i+1, fmt.getSignName( (Rasi)calcPada( i ), TF_LONG ) );
	}
	sheet->addItem( t3 );

	Table *t4 = new Table( 2, 13 );
	t4->setHeader( _( "Years" ));
	t4->setHeader( 0, _( "Rasi" ));
	t4->setHeader( 1, _( "Years" ));
	for ( i = 0; i < 12; i++ )
	{
		t4->setEntry( 0, i+1, fmt.getSignName( (Rasi)i, TF_LONG ));
		s.Printf( wxT( "%02d" ), getRasiYears( (Rasi)i ) );
		t4->setEntry( 1, i+1, s );
	}
	sheet->addItem( t4 );
}

/*****************************************************
**
**   JaiminiExpert   ---   calcPlanetAspects
**
******************************************************/
int JaiminiExpert::calcPlanetAspects( const ObjectId &planet )
{
	int ret = 0;

	if ( hasJaiminiAspect( this->getRasi(OJUPITER), this->getRasi(planet) )) ret++;
	if ( hasJaiminiAspect( this->getRasi(OMERCURY), this->getRasi(planet) )) ret++;

	Rasi rasi1 = this->getRasi(planet);
	ObjectId lord1 = getJaiminiLord( rasi1 );
	if ( hasJaiminiAspect( this->getRasi(lord1), this->getRasi(planet) )) ret++;

	return ret;
}


/*****************************************************
**
**   JaiminiExpert   ---   calcNodeStrength
**
******************************************************/
void JaiminiExpert::calcNodeStrength()
{
	ketuStrongerThanMars = false;
	rahuStrongerThanSaturn = false;
	const ObjectId inode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;
	const ObjectId descnode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE;

	/**********************************************
	** RAHU -- SATURN
	**********************************************/
	// ** RULE 1: Jupiter aspects rahu -> rahu in fall
	if ( hasJaiminiAspect( this->getRasi(OJUPITER), this->getRasi(inode) ))
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 2a
	else if (( this->getRasi(OSATURN) == R_AQUARIUS ) && ( this->getRasi(inode) != R_AQUARIUS ))
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 2b
	else if (( this->getRasi(OSATURN) != R_AQUARIUS ) && ( this->getRasi(inode) == R_AQUARIUS ))
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 3a
	else if ( nbplanets[this->getRasi(OSATURN)] > nbplanets[this->getRasi(inode)] )
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 3b
	else if ( nbplanets[this->getRasi(OSATURN)] < nbplanets[this->getRasi(inode)])
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 4a
	else if ( aspSat > aspRah )
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 4b
	else if ( aspSat < aspRah )
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 5a
	else if ( hasExaltationRasi( OSATURN, this->getRasi(OSATURN) ) && ! hasExaltationRasi( inode, this->getRasi(inode) ))
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 5b
	else if ( ! hasExaltationRasi( OSATURN, this->getRasi(OSATURN) ) && hasExaltationRasi( inode, this->getRasi(inode) ))
	{
		rahuStrongerThanSaturn = true;
	}
	// RULE 8a
	else if ( this->getRasiLongitude(OSATURN) > this->getRasiLongitude(inode) )
	{
		rahuStrongerThanSaturn = false;
	}
	// RULE 8b
	else if ( this->getRasiLongitude(OSATURN) < this->getRasiLongitude(inode) )
	{
		rahuStrongerThanSaturn = true;
	}
	else
	{
		rahuStrongerThanSaturn = false;
		//exit(1);
	}

	/**********************************************
	** KETU -- MARS
	**********************************************/
	//** RULE 1: Jupiter aspects ketu -> ketu in fall
	if ( hasJaiminiAspect( this->getRasi(OJUPITER), this->getRasi(descnode) ))
	{
		ketuStrongerThanMars = false;
	}
	// RULE 2a
	else if (( this->getRasi(OMARS) == R_SCORPIO ) && ( this->getRasi(descnode) != R_SCORPIO ))
	{
		ketuStrongerThanMars = true;
	}
	// RULE 2b
	else if (( this->getRasi(OMARS) != R_SCORPIO ) && ( this->getRasi(descnode) == R_SCORPIO ))
	{
		ketuStrongerThanMars = false;
	}
	// RULE 3a
	else if ( nbplanets[this->getRasi(OMARS)] > nbplanets[this->getRasi(descnode)] )
	{
		ketuStrongerThanMars = false;
	}
	// RULE 3b
	else if ( nbplanets[this->getRasi(OMARS)] < nbplanets[this->getRasi(descnode)] )
	{
		ketuStrongerThanMars = true;
	}
	// RULE 4a
	else if ( aspMar > aspKet )
	{
		ketuStrongerThanMars = false;
	}
	// RULE 4b
	else if ( aspMar < aspKet )
	{
		ketuStrongerThanMars = true;
	}
	// RULE 5a
	else if ( hasExaltationRasi( OMARS, this->getRasi(OMARS) ) && ! hasExaltationRasi( descnode, this->getRasi(descnode) ))
	{
		ketuStrongerThanMars = false;
	}
	// RULE 5b
	else if ( ! hasExaltationRasi( OMARS, this->getRasi(OMARS) ) && hasExaltationRasi( descnode, this->getRasi(descnode) ))
	{
		ketuStrongerThanMars = true;
	}
	// **** TODO: RULE 6 + 7
	// RULE 8a
	else if ( this->getRasiLongitude(OMARS) > this->getRasiLongitude(descnode) )
	{
		ketuStrongerThanMars = false;
	}
	// RULE 8b
	else if ( this->getRasiLongitude(OMARS) < this->getRasiLongitude(descnode) )
	{
		ketuStrongerThanMars = true;
	}
	else
	{
		ketuStrongerThanMars = false;
		//exit(1);
	}
}

/*****************************************************
**
**   JaiminiExpert   ---   calcSignAspects
**
******************************************************/
int JaiminiExpert::calcSignAspects( const Rasi &rasi )
{
	int ret = 0;

	if ( hasJaiminiAspect( this->getRasi(OJUPITER), rasi )) ret++;
	if ( hasJaiminiAspect( this->getRasi(OMERCURY), rasi )) ret++;

	ObjectId lrd = getJaiminiLord( rasi );

	if ( hasJaiminiAspect( this->getRasi(lrd), rasi )) ret++;

	return ret;
}


/*****************************************************
**
**   JaiminiExpert   ---   calcYears
**
******************************************************/
void JaiminiExpert::calcYears()
{
	for ( Rasi rasi = R_ARIES; rasi <= R_PISCES; rasi++ )
	{

		ObjectId ll = getJaiminiLord( rasi );
		Rasi pos = this->getRasi( ll );

		if ( isOddFootedRasi( rasi ))
		{
			rasiYears[rasi] = red12( pos - rasi );
		}
		else
		{
			rasiYears[rasi] = red12( rasi - pos );
		}

		if ( rasiYears[rasi] == 0 )
		{
			rasiYears[rasi] = 12;
		}
		else
		{
			if (( hasExaltationRasi( ll, pos  )) && ( ll != OMERCURY  ))
			{
				rasiYears[rasi]++;
			}
			else if ( hasDebilationRasi( ll, pos  ))
			{
				rasiYears[rasi]--;
			}
		}
	}
}

/*****************************************************
**
**   JaiminiExpert   ---   getRasiYears
**
******************************************************/
int JaiminiExpert::getRasiYears( const Rasi &rasi )
{
	assert(( rasi >= 0 ) && ( rasi < 12 ));
	return rasiYears[rasi];
}

/*****************************************************
**
**   JaiminiExpert   ---   getCharaKaraka
**
******************************************************/
ObjectId JaiminiExpert::getCharaKaraka( const int &index )
{
	assert( index >= ATMA_KARAKA && index <= DARA_KARAKA );
	return chara_karaka[index];
}

/*****************************************************
**
**   JaiminiExpert   ---   getCharaKarakaProperty
**
******************************************************/
int JaiminiExpert::getCharaKarakaProperty( const ObjectId &planet )
{
	// Planet index is 0..6 (OSUN...OSATURN) or 7 standing for Rahu
	assert(( planet >= OSUN && planet < (ObjectId)8 ));

	// if Chara karaka mode is Raman (seven planets only) Rahu will be no Karaka
	if ( config->vedicCalculation->charaKarakaMode && planet == 8 ) return NO_KARAKA;

	//printf( "getCharaKarakaProperty planet %d value %d\n", planet, p_chara_karaka[planet] );
	return p_chara_karaka[planet];
}

/*****************************************************
**
**   JaiminiExpert   ---   calcCharaKarakas
**
******************************************************/
void JaiminiExpert::calcCharaKarakas()
{
	ObjectId i, p;
	ObjectId j;
	double max_len;

	const int max_karakas = config->vedicCalculation->charaKarakaMode ? 7 : 8;
	double rl[8];

	// set Rahu, in Raman mode no Karaka property
	p_chara_karaka[7] = NO_KARAKA;

	const ObjectId inode = config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE;

	for ( i = OSUN; i < max_karakas; i++ )
	{
		p = ( i <= OSATURN ? i : inode );
		rl[i] = this->getRasiLongitude(p);
		//printf( "RL %d ist %f\n", i, rl[i] );
	}
	for ( i = OSUN; i < max_karakas; i++ )
	{
		max_len = rl[0];
		chara_karaka[i] = OSUN;
		for ( j = OMOON; j < max_karakas; j++ )
		{
			if ( rl[j] > max_len )
			{
				max_len = rl[j];
				chara_karaka[i] = j;
			}
		}
		rl[chara_karaka[i]] = 0;
		p_chara_karaka[chara_karaka[i]] = i;
	}
}

