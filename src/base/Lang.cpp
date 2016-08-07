/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Lang.cpp
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

#include "Lang.h"

#include <math.h>

#include <wx/log.h>
#include <wx/string.h>

#include "Calculator.h"
#include "Conf.h"
#include "maitreya.h"
#include "mathbase.h"

extern Config *config;

IMPLEMENT_SINGLETON( Formatter )
IMPLEMENT_SINGLETON( DateTimeFormatter )

/**************************************************************
***
**   Lang   ---   Constructor
***
***************************************************************/
Lang::Lang( WriterConfig *c )
{
	writercfg = c ? c : config->writer;
}

/**************************************************************
***
**   Lang   ---   getNakshatraName
***
***************************************************************/
wxString Lang::getNakshatraName( const int &nak, const bool &use28, const TEXT_FORMAT format ) const
{
	const static wxString k_nakshatra_name_large[28]  = { _( "Aswini" ), _( "Bharani" ), _( "Krittika" ), _( "Rohini" ),
		_( "Mrigasira" ), _( "Ardra" ), _( "Punarvasu" ), _( "Pushyami" ), _( "Aslesha" ), _( "Magha" ),
		_( "Poorvaphalguni" ), _( "Uttaraphalguni" ), _( "Hasta" ), _( "Chitra" ), _( "Swati" ), _( "Visakha" ),
		_( "Anuradha" ), _( "Jyeshta" ), _( "Moola" ), _( "Poorvashadha" ), _( "Uttarashadha" ), _( "Abhijit" ),
		_( "Sravana" ), _( "Dhanista" ), _( "Satabhisha" ), _( "Poorvabhadra" ), _( "Uttarabhadra" ), _( "Revati" )
	};
	const static wxString k_nakshatra_name_medium[28]  = {
		_( "Aswini" ), _( "Bharani" ), _( "Krittika" ), _( "Rohini" ), _( "Mrigasira" ), _( "Ardra" ),
		_( "Punarvasu" ), _( "Pushyami" ), _( "Aslesha" ), _( "Magha" ), _( "P.Phalguni" ), _( "U.Phalguni" ),
		_( "Hasta" ), _( "Chitra" ), _( "Swati" ), _( "Visakha" ), _( "Anuradha" ), _( "Jyeshta" ),
		_( "Moola" ), _( "P.Shadha" ), _( "U.Shadha" ), _( "Abhijit" ), _( "Sravana" ), _( "Dhanista" ),
		_( "Satabisha" ), _( "P.Bhadra" ), _( "U.Bhadra" ), _( "Revati" )
	};
	const static wxString k_nakshatra_name_short[28]  = { _( "Asw" ), _( "Bhr" ), _( "Kri" ), _( "Roh" ),
		_( "Mri" ), _( "Ard" ), _( "Pun" ), _( "Pus" ), _( "Asl" ), _( "Mak" ), _( "PPl" ),
		_( "UPl" ), _( "Hst" ), _( "Cit" ), _( "Sva" ), _( "Vis" ), _( "Anu" ), _( "Jye" ),
		_( "Mul" ), _( "PSa" ), _( "USa" ), _( "Abh" ), _( "Sra" ), _( "Dha" ), _( "Sat" ),
		_( "PBa" ), _( "UBa" ), _( "Rev" )
	};
	// shift index after  U. shadha for system of 27 nakshatras
	const int index = ( ! use28 && ( nak > 20 )) ? nak+1 : nak;

	if ( format == TF_SHORT ) return k_nakshatra_name_short[index];
	if ( format == TF_MEDIUM ) return k_nakshatra_name_medium[index];
	else return k_nakshatra_name_large[index];
}

/**************************************************************
***
**   Lang   ---   getNakshatra27Name
***
***************************************************************/
wxString Lang::getNakshatra27Name( const Nakshatra &n, const TEXT_FORMAT format ) const
{
	ASSERT_VALID_NAKSHATRA( n );
	return getNakshatraName( (int)n, false, format );
}

/**************************************************************
***
**   Lang   ---   getNakshatra28Name
***
***************************************************************/
wxString Lang::getNakshatra28Name( const NakshatraId_28 &n, const TEXT_FORMAT format ) const
{
	ASSERT_VALID_NAKSHATRA_28( n );
	return getNakshatraName( (int)n, true, format );
}

/**************************************************************
***
**    Lang   ---   getObjectName
***
***************************************************************/
const wxString Lang::getObjectName( const ObjectId &num, const TEXT_FORMAT &format, const bool vedic ) const
{
	if ( num == OERROR ) return wxT( "[Error]" );
	if ( num == ONONE ) return wxT( "[None]" );

	assert( num >= 0 );
	wxString ret;
	const static wxString k_object_large[MAX_EPHEM_OBJECTS] =
	{
		_("Sun "), _("Moon"), _("Mercury"), _( "Venus" ), _( "Mars" ), _("Jupiter" ), _("Saturn"),
		_("Uranus"), _("Neptune"), _("Pluto"),
		_("Dragon Head"), _("Dragon Head"),
		//_("Node"), _("Node"),
		_( "Dragon Tail" ), _( "Dragon Tail" ),
		_( "Ascendant" ), _("Meridian"), _( "Descendant" ), _("Imum Coeli"),
		_("Cupido"), _("Hades"), _("Zeus"), _("Kronos"), _("Apollon"), _("Admetos"), _("Vulkanus"), _("Poseidon"),
		_( "Chiron" ), _( "Pholus" ), _( "Ceres" ), _( "Pallas" ), _( "Juno" ), _( "Vesta" ), _( "Lilith" )
	};
	const static wxString k_object_large_vedic[8] =
	{
		_("Surya"), _("Chandra"), _("Budha"), _("Shukra"), _("Kuja"), _("Guru"), _("Shani"), _("Lagna")
	};
	const static wxString k_special_object[MAX_EXTENDED_OBJECTS] =
	{
		_("Aries"),
		_("Dhuma"), _("Vyatipata"), _("Parivesha"), _("Chapa"), _("Upaketu"),
		_("Kala"), _("Mrityu"), _("Ardhaprahara"), _("Yamaghantaka"), _("Gulika"), _( "Mandi" ),
		_("Bhava Lagna"), _("Hora Lagna"), _("Ghatika Lagna"), _( "D9Lagna" )
	};

	if ( num < MAX_EPHEM_OBJECTS )
	{
		ret = k_object_large[num];
		if ( vedic ) //|| writercfg->vedicSignNames )
		{
			if ( num == OTRUENODE || num == OMEANNODE ) ret = _( "Rahu" );
			if ( num == OTRUEDESCNODE || num == OMEANDESCNODE ) ret = _( "Ketu" );
		}
		if ( writercfg->vedicPlanetNames  )
		{
			if ( num <= OSATURN ) ret = k_object_large_vedic[num];
			else if ( num == OASCENDANT ) ret = k_object_large_vedic[7];
		}
	}
	else if ( num >= OARIES && num <= LAST_EXTENDED_OBJECT )
	{
		ret = k_special_object[num - OARIES];
	}
	else if ( num >= OHOUSE1 && num <= OHOUSE12 )
	{
		ret = vedic ? getBhavaName( num - OHOUSE1 ) : wxString::Format( wxT( "%d" ), num - OHOUSE1 + 1 );
	}
	else assert( false );

	if ( format == TF_LONG )
	{
		return ret;
	}

	// Abbreviation for Meridian is MC
	if ( num == OMERIDIAN ) return _( "MC" );
	if ( num == OIMUMCOELI ) return _( "IC" );

	if ( format == TF_MEDIUM )
	{
		if ( ! vedic )
		{
			if ( num == OTRUENODE || num == OMEANNODE ) ret = _( "Nod" );
			if ( num == OTRUEDESCNODE || num == OMEANDESCNODE ) ret = _( "SNo" );
		}
		return ret.Left( 3 );
	}
	else
	{
		if ( ! vedic )
		{
			if ( num == OTRUEDESCNODE || num == OMEANDESCNODE ) ret = _( "SN" );
		}
		return ret.Left( 2 );
	}
}

/**************************************************************
***
**    Lang   ---   getGanaName
***
***************************************************************/
const wxString Lang::getGanaName( const int &index ) const
{
	assert( index >= 0 && index < 3 );
	const wxString k_gana_name[3] = { _( "Deva" ), _( "Manuj" ), _( "Rakshas" ) };
	return k_gana_name[index];
}

/**************************************************************
***
**    Lang   ---   getTaraName
***
***************************************************************/
const wxString Lang::getTaraName( const int &index ) const
{
	assert( index >= -1 && index < 9 );

	if ( index == -1 ) return wxT( "none" );

	const wxString k_tara_name[9] = { _( "Janma" ), _( "Sampat" ), _( "Vipat" ), _( "Kshema" ),
		_( "Pratyak" ), _( "Sadhaka" ), _( "Vadha" ), _( "Mitra" ), _( "Parma Mitra" ) };

	return k_tara_name[index];
}

/**************************************************************
***
**    Lang   ---   getNadiName
***
***************************************************************/
const wxString Lang::getNadiName( const int &index ) const
{
	assert( index >= 0 && index < 3 );
	const wxString k_nadi_name[3] = { _( "Adi (Vata)" ), _( "Madhya (Pitta)" ), _( "Antya (Kapha)" ) };
	return k_nadi_name[index];
}

/**************************************************************
***
**    Lang   ---   getVarnaName
***
***************************************************************/
const wxString Lang::getVarnaName( const int &index ) const
{
	assert( index >= 0 && index < 4 );
	const wxString k_varna_name[4] = { _( "Shudra" ), _("Vaishya" ), _( "Kshattriya" ), _( "Brahmin" ) };
	return k_varna_name[index];
}

/**************************************************************
***
**    Lang   ---   getVashyaName
***
***************************************************************/
const wxString Lang::getVashyaName( const int &index ) const
{
	assert( index >= 0 && index < 5 );
	switch( index )
	{
		case VASHYA_QUADRUPED:
			return _( "Quadruped" );
		break;
		case VASHYA_HUMAN:
			return _( "Human" );
		break;
		case VASHYA_JALACHARA:
			return _( "Jalachara" );
		break;
		case VASHYA_LEO:
			return _( "Leo" );
		break;
		case VASHYA_SCORPIO:
			return _( "Scorpio" );
		break;
		default:
			assert( false );
		break;
	}
}

/**************************************************************
***
**   Lang   ---   getRajjuName
***
***************************************************************/
const wxString Lang::getRajjuName( const int &aroha, const int &type ) const
{
	assert( aroha >= 0 && aroha <= 2 );
	assert( type >= 0 && type <= 4 );
	const wxString k_aroha_name[3] = { _( "Aroha" ), wxEmptyString, _( "Avaroha" ) };
	const wxString k_rajjutype_name[5] = { _( "Pada (foot)" ), _("Kati (waist)" ), _( "Nabhi (navel)" ), _( "Kantha (neck)" ), _( "Siro (head)" ) };

	if ( aroha == 1 ) return k_rajjutype_name[4];
	wxString s;
	s << k_aroha_name[aroha] << wxT( "/" ) << k_rajjutype_name[type];
	return s;
}

/**************************************************************
***
**    Lang   ---   getFriendshipName
***
***************************************************************/
const wxString Lang::getFriendshipName( const FRIENDSHIP &f, const TEXT_FORMAT format ) const
{
	ASSERT_VALID_FRIENDSHIP( f )
  //PF_NONE = -1, PF_ENEMY = 0, PF_NEUTRAL, PF_FRIEND, PF_SWORN_ENEMY, PF_BEST_FRIEND };
	const static wxString k_friendship_sk[5] = { _( "Adhimitra" ), _( "Mitra" ), _( "Sama" ), _( "Satru" ), _( "Adhisatru" ) }; 
	const static wxString k_friendship[5] = { _( "Best friend" ), _( "Friend" ), _( "Neutral" ), _( "Enemy" ), _( "Sworn enemy" ) }; 
	const static wxString k_friendship_short[5] = { _( "BstFr" ), _( "Frnd" ), _( "Neutr" ), _( "Enem" ), _( "SwrEn" ) };

	if ( format == TF_SHORT )	return k_friendship_short[f];
	else return writercfg->vedicPlanetNames ? k_friendship_sk[(int)f] : k_friendship[(int)f];
}

/**************************************************************
***
**    Lang   ---   getDignityName
***
***************************************************************/
const wxString Lang::getDignityName( const DIGNITY &d, const TEXT_FORMAT format ) const
{
	assert( d >= DG_EXALTED && d <= DG_ADHISATRU );

	const static wxString k_dignity_name_sk[4] = { _( "Uchcha" ), _( "Nicha" ), _( "Moolatrikona" ), _( "Svakshetra" ) };
	const static wxString k_dignity_name[4] = { _( "Exalted" ), _( "Debilated" ), _( "Moolatrikona" ), _( "Own" ) };
	const static wxString k_dignity_short[4] = { _( "Exal" ), _( "Deb" ), _( "Mool" ), _( "Own" ) };
	int index = 0;

	switch( d )
	{
		case DG_EXALTED:
			index = 0;
		break;
		case DG_DEBILATED:
			index = 1;
		break;
		case DG_MOOLATRIKONA:
			index = 2;
		break;
		case DG_SVAKSHETRA:
			index = 3;
		break;
		case DG_ADHIMITRA:
			return getFriendshipName( PF_BEST_FRIEND, format );
		break;
		case DG_MITRA:
			return getFriendshipName( PF_FRIEND, format );
		break;
   	case DG_SAMA:
			return getFriendshipName( PF_NEUTRAL, format );
		break;
		case DG_SATRU:
			return getFriendshipName( PF_ENEMY, format );
		break;
		case DG_ADHISATRU:
			return getFriendshipName( PF_SWORN_ENEMY, format );
		default:
		break;
	}
	assert( index >= 0 && index < 4 );

	if ( format == TF_SHORT )	return k_dignity_short[index];
	else return writercfg->vedicPlanetNames ? k_dignity_name_sk[(int)index] : k_dignity_name[(int)index];
}

/**************************************************************
***
**   Lang   ---   getVimsopakaDignityName
***
***************************************************************/
const wxString Lang::getVimsopakaDignityName( const int &index, const int &dignity ) const
{
	assert( index >= 0 && index < 4 && dignity >= 0 );

	const static wxString shadvarga_dignity[5] = { _( "Kimsuka" ), _( "Vyanjana" ), _( "Chamara" ), _( "Chatra" ), _( "Kundala" ) };
	const static wxString saptavarga_dignity[6] = { _( "Kimsuka" ), _( "Vyanjana" ), _( "Chamara" ), _( "Chatra" ),
		_( "Kundala" ), _( "Mukuta" ) };
	const static wxString dasavarga_dignity[9] = { _( "Parijata" ), _( "Uttama" ),	_( "Gopura" ), _( "Simhasana" ),
		_( "Paravata" ), _( "Devaloka" ), _( "Brahmaloka" ), _( "Sakravahana" ), _( "Sridhama"  ) };
	const static wxString shodasavarga_dignity[15] = { _( "Bhedaka" ), _( "Kusumia"), _( "Nagapushpa" ), _( "Kanduka" ),
		_( "Kerala" ), _( "Kalpa Vriksha" ), _( "Chandana Vana" ), _( "PoornaChandra" ), _( "Uchchaisrava" ),
		_( "Dhanvantari" ), _( "Suryakanta" ), _( "Vidruma" ), _( "Chakra Simhasana" ), _( "Goloki" ), _( "Sri Vallabha" ) };

	if ( dignity < 2 ) return wxEmptyString;

	switch ( index )
	{
	case 0:
		assert( dignity <= 6 );
		return shadvarga_dignity[dignity-2];
		break;
	case 1:
		assert( dignity <= 7 );
		return saptavarga_dignity[dignity-2];
		break;
	case 2:
		assert( dignity <= 10 );
		return dasavarga_dignity[dignity-2];
		break;
	case 3:
		assert( dignity <= 16 );
		return shodasavarga_dignity[dignity-2];
		break;
	default:
		assert( false );
		break;
	}
	return wxEmptyString;
}

/**************************************************************
***
**     Lang   ---   getTithiName
***
***************************************************************/
const wxString Lang::getTithiName( const int &index ) const
{
	assert( index >= 0 && index < 30 );
	const static wxString k_tithi_name[30] = {
		_( "Sukla - Pratipad" ),
		_( "Sukla - Dviteeya" ),
		_( "Sukla - Triteeya" ),
		_( "Sukla - Chaturthi" ),
		_( "Sukla - Panchami" ),
		_( "Sukla - Shasti" ),
		_( "Sukla - Saptami" ),
		_( "Sukla - Asthami" ),
		_( "Sukla - Navami" ),
		_( "Sukla - Dasami" ),
		_( "Sukla - Ekadasi" ),
		_( "Sukla - Dvadasi" ),
		_( "Sukla - Trayodasi" ),
		_( "Sukla - Chaturdasi" ),
		_( "Sukla - Purnima" ),
		_( "Krishna - Pratipad" ),
		_( "Krishna - Dviteeya" ),
		_( "Krishna - Triteeya" ),
		_( "Krishna - Chaturthi" ),
		_( "Krishna - Panchami" ),
		_( "Krishna - Shasti" ),
		_( "Krishna - Saptami" ),
		_( "Krishna - Asthami" ),
		_( "Krishna - Navami" ),
		_( "Krishna - Dasami" ),
		_( "Krishna - Ekadasi" ),
		_( "Krishna - Dvadasi" ),
		_( "Krishna - Trayodasi" ),
		_( "Krishna - Chaturdasi" ),
		_( "Krishna - Amavasya" ),
	};
	return k_tithi_name[index];
}

/**************************************************************
***
**     Lang   ---   getWeekdayName
***
***************************************************************/
const wxString Lang::getWeekdayName( const int &day ) const
{
	assert(( day >= 0 ) || ( day < 7 ));
	const static wxString wd[7] = { _("Sunday"), _("Monday"), _("Tuesday"), _("Wednesday"),
		_("Thursday"), _("Friday"), _("Saturday") };
	return wd[day];
}

/**************************************************************
***
**     Lang   ---   getSignName
***
***************************************************************/
const wxString Lang::getSignName( const Rasi &i, const TEXT_FORMAT format ) const
{
	ASSERT_VALID_EXTENDED_RASI( i );
	wxString s;

	if ( i == R_ERROR ) return wxT( "[Error]" );
	if ( i == R_NONE ) return wxT( "[None]" );

	const static wxString k_sign[12]={ _( "Ar" ), _( "Ta" ), _( "Ge" ), _( "Cn" ), _( "Le" ), _( "Vi" ),
		_( "Li" ), _( "Sc" ), _( "Sa" ), _( "Cp" ), _( "Aq" ), _( "Pi" ) };

	const static wxString k_sign_full[12]={ _( "Aries" ), _( "Taurus" ), _( "Gemini" ), _( "Cancer" ),
		_( "Leo" ), _( "Virgo" ), _( "Libra" ), _( "Scorpio" ), _( "Sagittarius" ), _( "Capricorn" ),
		_( "Aquarius" ), _( "Pisces" ) };

	const static wxString k_sign_full_vedic[12]={ wxT( "Mesha" ), wxT( "Vrishabha" ), wxT( "Mithuna" ),
		wxT( "Kataka" ), wxT( "Simha" ), wxT( "Kanya" ), wxT( "Thula" ), wxT( "Vrischika" ),
		wxT( "Dhanus" ), wxT( "Makara" ), wxT( "Kumbha" ), wxT( "Meena" ) };

	const static wxString k_sign_medium_vedic[12]={ wxT( "Mes" ), wxT( "Vrs" ), wxT( "Mit" ),
		wxT( "Kat" ), wxT( "Sim" ), wxT( "Kan" ), wxT( "Thu" ), wxT( "Vrc" ), wxT( "Dha" ),
		wxT( "Mak" ), wxT( "Kum" ), wxT( "Mee" ) };

	const static wxString k_sign_vedic[12]={ wxT( "Ms" ), wxT( "Vs" ), wxT( "Mi" ), wxT( "Kt" ),
	wxT( "Si" ), wxT( "Kn" ), wxT( "Th" ), wxT( "Vc" ), wxT( "Dh" ), wxT( "Mk" ), wxT( "Ku" ), wxT( "Mn" ) };

	if ( format == TF_LONG )
	{
		if ( writercfg->vedicSignNames ) s = k_sign_full_vedic[i];
		else s = k_sign_full[i];
	}
	else if ( format == TF_MEDIUM )
	{
		if ( writercfg->vedicSignNames ) s = k_sign_medium_vedic[i];
		else s = k_sign_full[i] ;
		s = s.Left( 3 );
	}
	else {
		if ( writercfg->vedicSignNames ) s = k_sign_vedic[i];
		else s = k_sign[i];
	}
	return s;
}

/**************************************************************
***
**    Lang   ---   getKarakaName
***
***************************************************************/
const wxString Lang::getKarakaName( const int &i, const TEXT_FORMAT ) const
{
	const static wxString chara_karaka_name_long[8] = { _( "Atma" ), _( "Amatya" ), _( "Bhratru" ),
	        _( "Matru" ), _( "Pitru" ), _( "Putra" ), _( "Gnati" ), _( "Dhanya"  )
	                                          };
	assert( i >= NO_KARAKA && i < 8 );
	if ( i == NO_KARAKA ) return wxEmptyString;
	else return chara_karaka_name_long[i];
}

/**************************************************************
***
**    Lang   ---   getShastiamsaName
***
***************************************************************/
const wxString Lang::getShastiamsaName( const int &i ) const
{
	const static wxString k_shastiamsa_name[60] = {
		_( "Ghora" ), _( "Rakshasa" ), _( "Deva" ), _( "Kubera" ), _( "Yaksha" ),
		_( "Kinnara" ), _( "Bhrashta" ), _( "Kulaghna" ), _( "Garala" ), _( "Vahni" ),
		_( "Maya" ), _( "Purishaka" ), _( "Apampathi" ), _( "Marut" ), _( "Kaala" ),
		_( "Sarpa" ), _( "Amrita" ), _( "Indu" ), _( "Mridu" ), _( "Komala" ),
		_( "Heramba" ), _( "Brahma" ), _( "Vishnu" ), _( "Maheswara" ), _( "Deva" ),
		_( "Ardra" ), _( "Kalinasa" ), _( "Kshiteesa" ), _( "Kamalakara" ), _( "Gulika" ),
		_( "Mrithyu" ), _( "Kaala" ), _( "Davagani" ), _( "Ghora" ), _( "Yama" ),
		_( "Kantaka" ), _( "Sudha" ), _( "Amrita" ), _( "Poornachandra" ), _( "Vishadagdha" ),
		_( "Kulanasa" ), _( "Vamsakshaya" ), _( "Utpata" ), _( "Kaala" ), _( "Saumya" ),
		_( "Komala" ), _( "Seetala" ), _( "Karaladamshtra" ), _( "Chandramukhi" ), _( "Praveena" ),
		_( "Kaala Pavaka" ), _( "Dandayudha" ), _( "Nirmala" ), _( "Saumya" ), _( "Kroora" ),
		_( "Atiseetala" ), _( "Amrita" ), _( "Payodhi" ), _( "Bhramana" ), _( "Chandra Rekha" )
	};
	assert( i >= 0 && i < 60 );
	return k_shastiamsa_name[i];
}

/**************************************************************
***
**    Lang   ---   getBhavaName
***
***************************************************************/
const wxString Lang::getBhavaName( const int &i ) const
{
	const static wxString k_bhava_name[12] = { _( "Lagna" ), _( "Dhana" ), _( "Sahaja" ), _( "Bandhu" ), _( "Putra" ),
		_( "Satru" ), _( "Yuvati" ), _( "Randhra" ), _( "Dharma" ), _( "Karma" ), _( "Labha" ), _( "Vyaya" ) }; 
	assert( i >= 0 && i < 12 );
	return k_bhava_name[i];
}

/**************************************************************
***
**    Lang   ---   getMonthName
***
***************************************************************/
const wxString Lang::getMonthName( const int &i ) const
{
	assert( i >= 0 && i < 12 );
	const static wxString month_name[12] = { _( "January" ), _( "February" ), _( "March" ), _( "April" ), _( "May" ),
		_( "June" ), _( "July" ), _( "August" ), _( "September" ), _( "October" ), _( "November" ), _( "December" ) };
	return month_name[i];
}
/*****************************************************
**
**   Lang   ---   getElementName
**
******************************************************/
const wxString Lang::getElementName( const int &sign ) const
{
	switch( sign % 4 )
	{
		case 0:
			return _( "Fire" );
		break;
		case 1:
			return _( "Earth" );
		break;
		case 2:
			return _( "Air" );
		break;
		case 3:
			return _( "Water" );
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   Lang   ---   getSignQualityName
**
******************************************************/
const wxString Lang::getSignQualityName( const int &sign ) const
{
	switch( sign % 3 )
	{
		case 0:
			return _( "Movable" );
		break;
		case 1:
			return _( "Fixed" );
		break;
		case 2:
			return _( "Dual" );
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   Lang   ---   getObjectSignification
**
******************************************************/
const wxString Lang::getObjectSignification( const ObjectId &p, const TEXT_FORMAT&, const bool vedic )
{
	const static wxString k_vedic_signification[7] = { _( "Atma" ), _( "Mind" ), _( "Speech" ), _( "Semen" ),
		_( "Strength" ), _( "Guru" ), _( "Grief" ) };
	if ( vedic && p >= OSUN && p <= OSATURN ) return k_vedic_signification[int(p)];
	else return wxT( "-" );
};

/*****************************************************
**
**   Lang   ---   getAyanamsaName
**
******************************************************/
const wxString Lang::getAyanamsaName( const AYANAMSA &aya )
{
	wxString s;
	switch ( aya )
	{
	case AY_NONE:
		s= _( "None" );
		break;
	case AY_LAHIRI:
		s = _( "Lahiri" );
		break;
	case AY_RAMAN:
		s = _( "Raman" );
		break;
	case AY_KRISHNAMURTI:
		s = _( "Krishnamurti" );
		break;
	case AY_CUSTOM:
		s.Printf( wxT( "Custom (t0 %8.1f ayan_t0 %2.8f)" ), config->ephem->custom_t0, config->ephem->custom_ayan_t0 );
		break;
	default:
		s= _( "Unknown" );
		break;
	}
	return s;
}

/*****************************************************
**
**   DateTimeFormatter   ---   parseDateString
**
******************************************************/
bool DateTimeFormatter::parseDateString( wxString value, int &day, int &month, int &year )
{
	int ret = wxSscanf( value, wxT( "%d-%d-%d" ), &year, &month, &day );
	if ( ret != 3 ) return false;

	double dummy;
	Calculator *calculator = CalculatorFactory().getCalculator();
	return calculator->calc_and_check_jd( year, month, day, 0, dummy ) != true;
}

/*****************************************************
**
**   DateTimeFormatter   ---   parseDateString
**
******************************************************/
bool DateTimeFormatter::parseDateString( wxString value, double &jd )
{
	int year, month, day;
	const int nb_tokens = wxSscanf( value, wxT( "%d-%d-%d" ), &year, &month, &day );
	if ( nb_tokens != 3 ) return false;

	Calculator *calculator = CalculatorFactory().getCalculator();
	const bool ret = calculator->calc_and_check_jd( year, month, day, 0, jd ) != true;

	return ret;
}
 
/*****************************************************
**
**   DateTimeFormatter   ---   formatDateString
**
******************************************************/
wxString DateTimeFormatter::formatDateString( const double &jd, const double tzoffset, const int format )
{
	wxString s;
	int year, month, day;
	Formatter *tf = Formatter::get();

	if ( jd == 0 ) return wxT( "-" );
	const double reljd = jd + tzoffset / 24.0;

	calculateDateIntegers( reljd, day, month, year );

	if ( format & DF_INCLUDE_YEAR_SIGN )
	{
		s << wxString::Format( wxT( "%04d-%02d-%02d" ), year, month, day );
	}
	else
	{
		// AD and BC
		s << wxString::Format( wxT( "%04d-%02d-%02d %s" ), year > 0 ? year : 1 - year,
			month, day, year > 0 ? wxT( "AD" ) : wxT( "BC" ) );
	}

	if ( format & DF_INCLUDE_TIME )
	{
		s << SPACE << tf->getTimeFormatted( getTimeFromJD( reljd ));
	}
	if ( format & DF_INCLUDE_TZOFFSET )
	{
		if ( tzoffset >= 0 )
		{
			s << SPACE << wxString::Format( wxT( "(+%1.1f)" ), tzoffset );
		}
		else
		{
			s << SPACE << wxString::Format( wxT( "(%1.1f)" ), tzoffset );
		}
	}
	return s;
}

/*****************************************************
**
**   DateTimeFormatter   ---   calculateDateIntegers
**
******************************************************/
void DateTimeFormatter::calculateDateIntegers( const double &jd, int &day, int &month, int &year )
{
	double ut = 0;
	CalculatorFactory().getCalculator()->getDateIntsFromJD( jd, year, month, day, ut );
}

/*****************************************************
**
**   Formatter   ---   getAyanamsaNameAndValue
**
******************************************************/
const wxString Formatter::getAyanamsaNameAndValue( const AYANAMSA &type, const double &value )
{
	Lang lang;

	wxString s = lang.getAyanamsaName( type );
	if ( type == AY_NONE ) return s;
	s << wxT( " (" ) << getDegreesFormatted( value ) << wxT( ")" );

	return s;
}

/*****************************************************
**
**   Formatter   ---   getDegMinSecInts2
**
******************************************************/
void Formatter::getDegMinSecInts2( const double &len, int &deg, int &min, double &sec )
{
	double mylen = fabs( len );
	deg = (int)mylen;
	double mind = (double)((mylen - (double)deg ) * 60);
	min = (int)mind;
	sec = ( mind - (double)min ) * 60;
	if ( len < 0 ) deg *= -1;
}

/*****************************************************
**
**   Formatter   ---   getDegMinSecInts
**
******************************************************/
void Formatter::getDegMinSecInts( const double &len, int &deg, int &min, int &sec )
{
	double mylen = fabs( len );
	deg = (int)mylen;
	double mind = (double)((mylen - (double)deg ) * 60);
	min = (int)mind;
	sec = (int)(( mind - (double)min ) * 60 );
	if ( len < 0 ) deg *= -1;
}

/*****************************************************
**
**   Formatter   ---   formatDegreeString
**
******************************************************/
const wxString Formatter::formatDegreeString( double t, const int &format )
{
	double sec = 0;
	int deg = 0;
	int min = 0;

	getDegMinSecInts2( t, deg, min, sec );

	if ( format == DEG_PRECISION_MORE )
	{
		return wxString::Format( wxT( "%02d:%02d:%02d.%02d" ), deg, min, (int)sec, (int)(.5 + 100 * ( sec - (int)sec )));
	}
	else
	{
		sec = a_rund( sec );
		if ( sec == 60 )
		{
			sec = 0;
			min++;
			if ( min == 60 )
			{
				min = 0;
				deg++;
			}
		}
	}
	if ( format == DEG_PRECISION_SECOND || ( DEG_PRECISION_FLEXIBLE && sec != 0 ))
	{
		return wxString::Format( wxT( "%02d:%02d:%02d" ), deg, min, (int)sec );
	}
	else
	{
		if ( sec >= 30 ) min++;
		if ( min == 60 )
		{
			min = 0;
			deg++;
		}
		return wxString::Format( wxT( "%02d:%02d" ), deg, min );
	}
}

/*****************************************************
**
**   Formatter   ---   getDegMinSecFormatted
**
******************************************************/
const wxString Formatter::getDegMinSecFormatted( double t, const int &format, const double &minvalue, const double &maxvalue )
{
	double sec = 0;
	int deg = 0;
	int min = 0;

	if ( t < minvalue || t > maxvalue )
	{
		printf( "Formatter::getDegMinSecFormatted WARN invalid value %f, minvalue was %f maxvalue %f\n", t, minvalue, maxvalue );
		t = a_red( t, maxvalue );
	}

	getDegMinSecInts2( t, deg, min, sec );

	if ( format == DEG_PRECISION_MORE )
	{
		return wxString::Format( wxT( "%02d:%02d:%02d.%02d" ), deg, min, (int)sec, (int)(.5 + 100 * ( sec - (int)sec )));
	}
	else
	{
		sec = a_rund( sec );
		if ( sec == 60 )
		{
			sec = 0;
			min++;
			if ( min == 60 )
			{
				min = 0;
				deg++;
			}
		}
	}
	if ( format == DEG_PRECISION_SECOND || ( DEG_PRECISION_FLEXIBLE && sec != 0 ))
	{
		return wxString::Format( wxT( "%02d:%02d:%02d" ), deg, min, (int)sec );
	}
	else
	{
		if ( sec >= 30 ) min++;
		if ( min == 60 )
		{
			min = 0;
			deg++;
		}
		return wxString::Format( wxT( "%02d:%02d" ), deg, min );
	}
}

/*****************************************************
**
**   Formatter   ---   getLenFormatted
**
******************************************************/
const wxString Formatter::getLenFormatted( const double &len, const int format )
{
	wxString s;
	if ( len < 0 ) s = wxT( "-" );
	s <<  getDegMinSecFormatted( fabs( len ), format, 0.0, 360.0 );
	return s;
}

/*****************************************************
**
**   Formatter   ---   getLatitudeFormatted
**
******************************************************/
const wxString Formatter::getLatitudeFormatted( const double &l, const int format )
{
	wxString s;
	s <<  getDegMinSecFormatted( fabs( l ), format, 0.0, 90.0 );

	if ( l > 0 ) s << wxT( "N" );
	else if ( l < 0 ) s << wxT( "S" );
	return s;
}

/*****************************************************
**
**   Formatter   ---   getLongitudeFormatted
**
******************************************************/
const wxString Formatter::getLongitudeFormatted( const double &l, const int format )
{
	wxString s;
	s <<  getDegMinSecFormatted( fabs( l ), format, 0.0, 180.0 );
	if ( l > 0 ) s << wxT( "E" );
	else if ( l < 0 ) s << wxT( "W" );
	return s;
}

#define PARSE_FORMAT_STRICT

/*****************************************************
**
**   Formatter   ---   parseDegMinSecAndFormat
**
******************************************************/
wxString Formatter::parseDegMinSecAndFormat( wxString s, const int format )
{
	int deg, min, sec;

	bool b = getDegreeIntsFromString( s.c_str(), deg, min, sec );
	if ( ! b )
	{
		printf( "Formatter::parseDegreesAndFormat WARN: invalid string %s\n", str2char( s ));
		return s;
	}
	double value = sec + min * 60 + deg * 3600;
	value /= 3600.0;
	wxString ret = getDegreesFormatted( value, format );

#ifdef PARSE_FORMAT_STRICT
	int deg2, min2, sec2;

	bool b2 = getDegreeIntsFromString( ret.c_str(), deg2, min2, sec2 );
	if ( ! b2 )
	{
		printf( "ERROR Formatter::parseDegMinSecAndFormat: output parsing failed for string %s\n", str2char( ret )); 
	}
	if ( deg != deg2 || min != min2 || sec != sec2 )
	{
		printf( "ERROR Formatter::parseDegMinSecAndFormat: wrong values (rounding errors?): old %d %d %d, new %d %d %d,\n", deg, min, sec, deg2, min2, sec2 );
	}
#endif

	return ret;
}

/*****************************************************
**
**   Formatter   ---   getLenString
**
******************************************************/
// unused
const wxString Formatter::getLenString( const double &len, const int format, const int dir )
{
	wxString s;
	Lang lang;

	s << ( dir == 1 ? 'R' : 'D' );

	double mylen = red_deg( len );
	const Rasi sign = (Rasi)( mylen / 30 );
	mylen -= 30 * sign;

	s << getDegMinSecFormatted( mylen, format, 0.0, 30.0 ) << lang.getSignName(sign);
	return s;
}

/*****************************************************
**
**   Formatter   ---   parseDegreeString
**
******************************************************/
bool Formatter::parseDegreeString( const wxString&s, double &value, const int &maxvalue )
{
	int deg, min, sec;
	deg = min = sec = 0;

	const bool b = parseDegreeString( s, deg, min, sec, maxvalue );
	if ( b || config->view->inputFieldValidation == IFV_ADJUST )
	{
		value = sec + min * 60 + deg * 3600;
		value /= 3600.0;
	}
	return b;
}

/*****************************************************
**
**   Formatter   ---   parseDegreeString
**
******************************************************/
bool Formatter::parseDegreeString( const wxString &value, int &deg, int &minute, int &second, const int &maxvalue )
{
	deg = 0;
	minute = 0;
	second = 0;
	bool ok = true;

	int ret = wxSscanf( value, wxT( "%d:%d:%d" ), &deg, &minute, &second );
	//printf( "Formatter::parseDegreeString ret %d\n", ret );

	// value is >= 1 if anything found, otherwise -1
	//if ( ret < 1 )
	if ( false ) // allow empty values
	{
		deg = 0;
		minute = 0;
		second = 0;
		return false;
	}

	//enum  { IFV_NONE = 0, IFV_RESTORE, IFV_ADJUST };

	if (( second < 0 ) || ( second >= 60 ))
	{
		if ( config->view->inputFieldValidation == IFV_ADJUST )
		{
			minute+= (int)( second / 60 );
			second = a_red( second, 60 );
		}
		ok = false;
	}
	if (( minute < 0 ) || ( minute >= 60 ))
	{
		if ( config->view->inputFieldValidation == IFV_ADJUST )
		{
			deg+= (int)( minute / 60 );
			minute = a_red( minute, 60 );
		}
		ok = false;
	}
	if ( deg < 0 || deg > maxvalue || ( deg == maxvalue &&  minute + second > 0 ))
	{
		if ( config->view->inputFieldValidation == IFV_ADJUST )
		{
			deg = a_red( deg, maxvalue );
		}
		ok = false;
	}
	return ok;
}

/*****************************************************
**
**   Formatter   ---   getDegreeIntsFromString
**
******************************************************/
bool Formatter::getDegreeIntsFromString( wxString value, int &deg, int &minute, int &second, int /* format */ )
{
	deg = 0;
	minute = 0;
	second = 0;
	bool ok = true;

	int ret = wxSscanf( value, wxT( "%d:%d:%d" ), &deg, &minute, &second );
	if ( ret < 1 ) return false;

	if (( second < 0 ) || ( second > 59 ))
	{
		minute+= (int)( second / 60 );
		second = a_red( second, 60 );
		ok = false;
	}
	if (( minute < 0 ) || ( minute > 59 ))
	{
		deg+= (int)( minute / 60 );
		minute = a_red( minute, 60 );
		ok = false;
	}
	if (( deg < -180 ) || ( deg > 180 ))
	{
		ok = false;
	}

	return ok;
}

/*****************************************************
**
**   Formatter   ---   parseDoubleString
**
******************************************************/
bool Formatter::parseDoubleString( const wxString &s, double &value, const double &minvalue, const double &maxvalue )
{
	const double v = myatof( s );
	bool isvalid = ( v >= minvalue && v <= maxvalue );
	if ( isvalid || config->view->inputFieldValidation == IFV_ADJUST )
	{
		value = v;
	}
	return isvalid;
}

/*****************************************************
**
**   Formatter   ---   formatDoubleString
**
******************************************************/
wxString Formatter::formatDoubleString( const double &t, const int /*format*/  )
{
	return printfDouble( t );
}

/*****************************************************
**
**   Formatter   ---   getIntOrEmtpy
**
******************************************************/
wxString Formatter::getIntOrEmtpy( const int &a )
{
	if ( a ) return  wxString::Format( wxT( "%d" ), a );
	else return wxEmptyString;
}

