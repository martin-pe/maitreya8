/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Horoscope.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
************************************************************************/

#include "Horoscope.h"

#include "ArabicParts.h"
#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "Dasa.h"
#include "Lang.h"
#include "VedicPlanet.h"
#include "mathbase.h"

extern Config *config;

IMPLEMENT_CLASS( Horoscope, BasicHoroscope )

/*****************************************************
**
**   Horoscope   ---   Constructor
**
******************************************************/
Horoscope::Horoscope() : BasicHoroscope()
{
	int i;
	ObjectId p;

	for ( i = 0; i < NUM_LAGNA; i++ ) lagna_len[i] = 0;
	for ( i = 0; i < NUM_UPA; i++ ) upagraha_len[i] = 0;
	for ( p = FIRST_EPHEM_OBJECT; p <= LAST_EPHEM_OBJECT; p++ ) ephem_planets[p] = new VedicPlanet( this, p );

	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ )
	{
		evpos[i] = new ObjectPosition;
		ewpos[i] = new ObjectPosition;
		extended_planets[i] = new VedicPlanet( this, (ObjectId)(FIRST_EXTENDED_OBJECT + i ));
	}
	for ( i = 0; i < 12; i++ )
	{
		house_planet_wrapper[i] = new VedicPlanet( this, (ObjectId)(OHOUSE1 + i ));
	}
	arabic_expert = new ArabicPartsExpert( this );
	arieslen = 0;
}

/*****************************************************
**
**   Horoscope   ---   Destructor
**
******************************************************/
Horoscope::~Horoscope()
{
	int i;
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ ) delete ephem_planets[i];
	for ( i = 0; i < MAX_EXTENDED_OBJECTS; i++ )
	{
		delete evpos[i];
		delete ewpos[i];
		delete extended_planets[i];
	}
	for ( i = 0; i < 12; i++ ) delete house_planet_wrapper[i];
	delete arabic_expert;
}

/*****************************************************
**
**   Horoscope   ---   update
**
******************************************************/
void Horoscope::update()
{
	int i, j, lord;
	double step_len, lsun, thejd, asc;
	Calculator *calculator = CalculatorFactory().getCalculator();
	ObjectPosition pos;
	double startjdrise, startjdset;

	// BasicHoroscope
	updatePlanets();

	// daybirth, sunrise and sunset
	double diff_ascsun = red_deg( getTropicalLongitude( OASCENDANT ) -  getTropicalLongitude( OSUN ));
	isDayBirth = ( diff_ascsun < 180 );

	startjdrise = startjdset = getJD();
	if ( isDayBirth ) startjdrise--;
	else startjdset--;

	sunrise = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNRISE, startjdrise, getLocation()->getLatitude(), getLocation()->getLongitude());
	sunset = calculator->calcNextSolarEvent( SOLAR_EVENT_SUNSET, startjdset, getLocation()->getLatitude(), getLocation()->getLongitude());

	// Upagraha Update
	lsun = object_len[OSUN];
	// Dhuma
	upagraha_len[CDHUMA] =  red_deg( lsun + 133.333333333 );
	// Vyatipata unused, ref Dhuma
	upagraha_len[CVYATIPATA] = 0;
	// Parivesha unsused, ref Dhuma
	upagraha_len[CPARIVESHA] = 0;
	// Chapa
	upagraha_len[CCHAPA] = red_deg( lsun + 313.333333333 );
	// Upaketu
	upagraha_len[CUPAKETU] = red_deg( lsun + 330 );

	// Kalavelas
	for ( i = 0; i <= 8; i++ )
	{
		if ( isDayBirth )
		{
			lord = ( getDataSet()->getWeekDay() + i ) % 8;
			step_len = .125 * fabs( sunset - sunrise );
			thejd = sunrise + i * step_len;
		}
		else
		{
			lord = ( getDataSet()->getWeekDay() + i + 4 ) % 8;
			step_len = .125 * fabs( sunrise - sunset );
			// FIXME
			if ( step_len < 0 ) step_len *= -1;
			thejd = sunset + i * step_len;
		}
		for ( j = 0; j < 6; j++ )
		{
			if ( config->vedicCalculation->kalavela.lord[j] == lord )
			{
				// Default: config->kalaVelaPortion[j] == KALAVELA_PORTION_START, do nothing
				if ( config->vedicCalculation->kalavela.portion[j] == KALAVELA_PORTION_MIDDLE ) thejd += .5* step_len;
				else if ( config->vedicCalculation->kalavela.portion[j] == KALAVELA_PORTION_END ) thejd += step_len;

				asc = red_deg( CalculatorFactory().getCalculator()->calcAscendant( thejd, getLocation()->getLatitude(), getLocation()->getLongitude() ));
				upagraha_len[CKALA+j] = asc;
			}
		}
	}

	// Special Lagnas
	lagna_len[CBHAVA_LAGNA] = red_deg( 360 * ( getJD() - sunrise ) + lsun);
	lagna_len[CHORA_LAGNA] = red_deg( 720 * ( getJD() - sunrise ) + lsun);
	lagna_len[CGHATIKA_LAGNA] = red_deg( 1800 * ( getJD() - sunrise ) + lsun);

	updateObjectPositions();

	double angle = red_deg( object_len[ OMOON ] - object_len[ OSUN ] );
	switch ( config->vedicCalculation->moonBenefic )
	{
	case 1: // angle < 180
		moonBenefic = ( angle < 180 );
		break;
	case 2: // 120 < angle < 240
		moonBenefic = ( angle > 120 && angle < 240 );
		break;
	case 3: // always
		moonBenefic =  true;
		break;
	case 0: // never
	default:
		moonBenefic = false;
		break;
	}

	mercuryBenefic = true;
	switch ( config->vedicCalculation->mercuryBenefic )
	{
	case 1: // no malefic conjunction
	{
		const int mpos = getRasi( getVedicLongitude( OMERCURY ));
		if ( getRasi( getVedicLongitude( OSUN )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( ! moonBenefic && getRasi( getVedicLongitude( OMOON )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLongitude( OMARS )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLongitude( OSATURN )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLongitude(
		                  config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUENODE : OMEANNODE )) == mpos ) {
			mercuryBenefic = false;
			break;
		}
		if ( getRasi( getVedicLongitude( config->vedicCalculation->lunarNodeMode == LUNAR_NODE_TRUE ? OTRUEDESCNODE : OMEANDESCNODE )) == mpos )
		{
			mercuryBenefic = false;
			break;
		}
	}
	case 2: // always
		mercuryBenefic = true;
		break;
	case 0: // never
	default:
		mercuryBenefic = false;
		break;
	}

	// Arabic parts
	arabic_expert->update();
}

/**************************************************************
***
**    Horoscope   ---   updateObjectPositions
***
***************************************************************/
void Horoscope::updateObjectPositions()
{
	ObjectId p, pid;

	for ( p = OSUN; p < MAX_EPHEM_OBJECTS; p++ ) ephem_planets[p]->update( getVedicLongitude( p ));

	// Attention: must be separated from updateVargaData because Vargas reference each other
	for ( p = OSUN; p <= OSATURN; p++ ) ephem_planets[p]->calculateDignities();

	for ( p = (ObjectId)0; p < MAX_EXTENDED_OBJECTS; p++ )
	{
		pid = p + FIRST_EXTENDED_OBJECT;
		switch ( pid )
		{
			case ODHUMA:
			case OVYATIPATA:
			case OPARIVESHA:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CDHUMA];
				break;
			case OCHAPA:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CCHAPA];
				break;
			case OUPAKETU:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CUPAKETU];
				break;
			case OKALA:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CKALA];
				break;
			case OMRITYU:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CMRITYU];
				break;
			case OARDHAPRAHARA:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CARDHAPRAHARA];
				break;
			case OYAMAGHANTAKA:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CYAMAGHANTAKA];
				break;
			case OGULIKA:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CGULIKA];
				break;
			case OMANDI:
				evpos[p]->longitude = ewpos[p]->longitude = upagraha_len[CMANDI];
				break;
			case OBHAVA_LAGNA:
				evpos[p]->longitude = ewpos[p]->longitude = lagna_len[CBHAVA_LAGNA];
			break;
			case OHORA_LAGNA:
				evpos[p]->longitude = ewpos[p]->longitude = lagna_len[CHORA_LAGNA];
				break;
			case OGHATIKA_LAGNA:
				evpos[p]->longitude = ewpos[p]->longitude = lagna_len[CGHATIKA_LAGNA];
				break;
			case OARIES:
				evpos[p]->longitude = ewpos[p]->longitude = arieslen;
				break;

			// Calculate D9 Lagna separately because Ayanamsa must be multiplied, too
			case OD9LAGNA:
				evpos[p]->longitude = red_deg( 9 * red_deg( object_len[OASCENDANT] - iayanamsa ));
				ewpos[p]->longitude = red_deg( 9 * ( object_len[OASCENDANT] - wayanamsa ));
			break;
			default:
				assert( false );
			break;
		}

		if ( pid != OD9LAGNA )
		{
			evpos[p]->longitude = red_deg( evpos[p]->longitude - iayanamsa );
			ewpos[p]->longitude = red_deg( ewpos[p]->longitude - wayanamsa );
		}
		if ( pid == OVYATIPATA )
		{
			evpos[p]->longitude = red_deg( 360 - evpos[p]->longitude );
			ewpos[p]->longitude = red_deg( 360 - ewpos[p]->longitude );
		}
		else if ( pid == OPARIVESHA )
		{
			evpos[p]->longitude = red_deg( 180 - evpos[p]->longitude );
			ewpos[p]->longitude = red_deg( 180 - ewpos[p]->longitude );
		}
		extended_planets[p]->update( evpos[p]->longitude );
	}

	for ( int i = 0; i < 12; i++ )
	{
		house_planet_wrapper[i]->update( getHouse( i, true, ! config->vedicCalculation->houseUseCusps ));
	}
}

/**************************************************************
***
**    Horoscope   ---   getVargaData
***
***************************************************************/
VargaData *Horoscope::getVargaData( const ObjectId &planet, const Varga &varga ) const
{
	ASSERT_VALID_OBJECT( planet )
	ASSERT_VALID_VARGA( varga )
	return getPlanet( planet )->getVargaData( varga );
}

/**************************************************************
***
**   Horoscope   ---   getPlanet
***
***************************************************************/
VedicPlanet *Horoscope::getPlanet( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	if ( planet <= LAST_EPHEM_OBJECT )
	{
		return ephem_planets[planet];
	}
	else if ( planet >= FIRST_EXTENDED_OBJECT && planet <= LAST_EXTENDED_OBJECT )
	{
		return extended_planets[planet - FIRST_EXTENDED_OBJECT];
	}
	else if ( IS_HOUSE_OBJECT( planet ))
	{
		return house_planet_wrapper[planet - OHOUSE1];
	}
	else
	{
		ASSERT_VALID_ARABIC_OBJECT( planet )
		return arabic_expert->getPlanet( planet );
	}
}

/*****************************************************
**
**   Horoscope   ---   getObjectPosition
**
******************************************************/
ObjectPosition Horoscope::getObjectPosition( const ObjectId &planet, const bool &vedic ) const
{
	ASSERT_VALID_OBJECT( planet )
	if ( IS_EPHEM_OBJECT( planet ) || IS_HOUSE_OBJECT( planet )) return BasicHoroscope::getObjectPosition( planet, vedic );
	else if ( planet <= LAST_EXTENDED_OBJECT ) return( vedic ? *evpos[planet - FIRST_EXTENDED_OBJECT] : *ewpos[planet - FIRST_EXTENDED_OBJECT] );
	else return arabic_expert->getObjectPosition( planet, vedic );
}

/*****************************************************
**
**   Horoscope   ---   isBenefic
**
******************************************************/
bool Horoscope::isBenefic( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet )
	switch ( planet )
	{
	case OMOON:
		return moonBenefic;
		break;
	case OMERCURY:
		return mercuryBenefic;
		break;
	case OVENUS:
	case OJUPITER:
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

/*****************************************************
**
**   Horoscope   ---   updateKP
**
******************************************************/
void Horoscope::updateKP( const DasaId &dasaindex ) const
{
	ObjectId i;
	DasaExpert *expert = DasaExpertFactory().getDasaExpert( dasaindex );

	for ( i = OSUN; i < MAX_EPHEM_OBJECTS; i++ )
		kp_lord[i] = expert->getKPLords( getVedicLongitude( i ));

	for ( i = OSUN; i < MAX_EXTENDED_OBJECTS; i++ )
		ekp_lord[i] = expert->getKPLords( getVedicLongitude( i + FIRST_EXTENDED_OBJECT ));

	for ( i = OSUN; i < (ObjectId)MAX_ARABIC_OBJECTS; i++ )
		akp_lord[i] = expert->getKPLords( getVedicLongitude( i + FIRST_ARABIC_OBJECT ));

	for ( int j = HOUSE1; j <= HOUSE12; j++ )
		hkp_lord[j] = expert->getKPLords( getHouse( j, true, false ));
	delete expert;
}

/*****************************************************
**
**   Horoscope   ---   getHouseKPLords
**
******************************************************/
KpData Horoscope::getHouseKPLords( const int &house ) const
{
	printf( "Horoscope::getHouseKPLords house %d\n", house );
	ASSERT_VALID_HOUSE( house )
	return hkp_lord[house];
}

/*****************************************************
**
**   Horoscope   ---   getKPLords
**
******************************************************/
KpData Horoscope::getKPLords( const ObjectId &planet ) const
{
	ASSERT_VALID_OBJECT( planet );

	if ( IS_EPHEM_OBJECT( planet ))
	{
		return kp_lord[planet];
	}
	else if ( IS_EXTENDED_OBJECT( planet ))
	{
		return ekp_lord[planet - FIRST_EXTENDED_OBJECT];
	}
	else if ( IS_HOUSE_OBJECT( planet ))
	{
		return hkp_lord[planet - OHOUSE1];
	}
	else
	{
		ASSERT_VALID_ARABIC_OBJECT( planet )
		return kp_lord[planet - FIRST_ARABIC_OBJECT];
	}
}


