/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Nakshatra.cpp
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

#include "Nakshatra.h"

#include "astrobase.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Table.h"

#include <wx/string.h>
#include <math.h>
#include <assert.h>

extern Config *config;

/*****************************************************
**
**   NakshatraExpert   ---   getYoni
**
******************************************************/
Yoni NakshatraExpert::getYoni( const double& len ) const
{
	assert( len >= 0 && len <= 360 );
	const wxString k_yoni_name_sk[14] = { wxT( "Ashwa"), wxT( "Mahish" ), wxT( "Simha" ), wxT( "Gaja" ),
		wxT( "Mesha" ), wxT( "Vanar" ), wxT( "Nakul" ), wxT( "Sarpa" ), wxT( "Mriga" ), wxT( "Swan" ),
		wxT( "Marjar" ), wxT( "Mushak" ), wxT( "Vyaghra" ), wxT( "Gow" ) };
	const wxString k_yoni_name[14] = { _( "Horse" ), _( "Buffalo" ), _( "Lion" ), _( "Elephant" ),
		_( "Goat" ), _( "Monkey" ), _( "Mongoose" ), _( "Serpent" ), _( "Deer" ), _( "Dog" ), _( "Cat" ),
		_( "Rat" ), _( "Tiger" ), _( "Cow"  ) };

	const NakshatraId_28 nakshatra = getNakshatra28( len );
	const static int yoni_map[28] =
	{
		0, 3, 4, 7, 7, 9, 10, 4, 10,
		11, 11, 13, 1, 12, 1, 12, 8, 8,
		9, 5, 6, 6, 5, 2, 0, 2, 13, 3
	};
	const int id = yoni_map[nakshatra];
	const static int yoni_male[28] =
	{
		true, true, false, true, false, false, false, true, true,
		true, false, true, false, false, true, true, false, true,
		true, true, true,
		false, // Abhijit
		false, false, false, true, false, false
	};
	wxString name = config->writer->vedicPlanetNames ? k_yoni_name_sk[id] : k_yoni_name[id];

	name << wxT( " (" ) << ( yoni_male[nakshatra] ? wxT( "m" ) : wxT( "f" )) << wxT( ")" );

	return Yoni( id, yoni_male[nakshatra], name );
}

/*****************************************************
**
**   NakshatraExpert   ---   getNadi
**
******************************************************/
Nadi NakshatraExpert::getNadi( const double& len ) const
{
	assert( len >= 0 && len <= 360 );
	Nakshatra nakshatra = getNakshatra27( len );

	const int nadi_map[27] =
	{
		0, 1, 2, 2, 1, 0, 0, 1, 2,
		2, 1, 0, 0, 1, 2, 2, 1, 0,
		0, 1, 2, 2, 1, 0, 0, 1, 2
	};
	return (Nadi)nadi_map[(int)nakshatra];
}

/*****************************************************
**
**   NakshatraExpert   ---   getGana
**
******************************************************/
Gana NakshatraExpert::getGana( const double& len ) const
{
	assert( len >= 0 && len <= 360 );
	Nakshatra nakshatra = getNakshatra27( len );

	const int gana_map[27] =
	{
		0, 1, 2, 1, 0, 1, 0, 0, 2,
		2, 1, 1, 0, 2, 0, 2, 0, 2,
		2, 1, 1, 0, 2, 2, 1, 1, 0
	};
	return (Gana)gana_map[nakshatra];
}

/*****************************************************
**
**   NakshatraExpert   ---   getTara
**
******************************************************/
int NakshatraExpert::getTara( const int& nak ) const
{
	int n = nak;
	if ( n < 0 ) n += 27;
	assert( n >= 0 && n < 27 );
	return n % 9;
}

/*****************************************************
**
**   NakshatraExpert   ---   getRajju
**
******************************************************/
Rajju NakshatraExpert::getRajju( const Nakshatra &nak )
{
	ASSERT_VALID_NAKSHATRA( nak );
	const int n1 = (int)nak % 9;
	Rajju rajju;

	if ( n1 < 4 )  // Aroha
	{
		rajju.aroha = 0;
		rajju.type = n1;
	}
	else if ( n1 == 4 )  // Siro
	{
		rajju.aroha = 1;
		rajju.type = 4;
	}
	else if ( n1 > 4 && n1 < 9 )  // Avaroha
	{
		rajju.aroha = 2;  
		rajju.type = 8 - n1;
	}
	else assert( false );

	return rajju;
}

/*****************************************************
**
**   NakshatraExpert   ---   getSbcNakshatraProperties
**
******************************************************/
void NakshatraExpert::getSbcNakshatraProperties( const int &birthStar, vector<NakshatraProperties> &props )
{
	props.clear();
	int nak27 = 0;  // Aswini

	for( int nak28 = 0; nak28 < 28; nak28++ )
	{
		NakshatraProperties p;
		if ( nak28 == ABHIJIT_INDEX )
		{
			p.tara = -1;
			p.taraQuality = TARA_PROP_NEUTRAL;
		}
		else
		{
			p.tara = getTara( nak27 - birthStar );
			//printf( "Birth %d nak28 %d nak27 %d tara %d\n", birthStar, nak28, nak27, p.tara );
			assert( p.tara >= 0 && p.tara < 9 );
			switch( p.tara )
			{
				case 1:
				case 3:
				case 5:
				case 7:
					p.taraQuality = TARA_PROP_BENEFIC;
				break;
				case 0:
				case 2:
				case 4:
				case 6:
				case 8:
					p.taraQuality = TARA_PROP_MALEFIC;
				break;
				default:
					assert( 0 );
				break;
			}
			nak27++;
		}
		props.push_back( p );
	}
}


/*****************************************************
**
**   NakshatraExpert   ---   write
**
******************************************************/
void NakshatraExpert::write( Sheet *sheet, const ChartProperties *chartprops, const bool show_header )
{
	assert( chartprops );
	assert( horoscope );
	assert( sheet );

	wxString s;
	ObjectId i;
	double len;
	Rajju rajju;
	SheetFormatter fmt;

	Lang lang;
	ObjectArray obs = chartprops->getVedicPlanetList();

	Table *table = new Table( 8, obs.size() + 1 );
	if ( show_header ) table->setHeader( _( "Nakshatras" ));
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  _( "Longitude" ));
	table->setHeader( 2,  _( "Nakshatra" ));
	table->setHeader( 3,  _( "Yoni" ));
	table->setHeader( 4,  _( "Gana" ));
	table->setHeader( 5,  _( "Nadi" ));
	table->setHeader( 6,  _( "Tara" ));
	table->setHeader( 7,  _( "Rajju" ));

	int line = 1;
	for ( uint p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		len = horoscope->getObjectPosition( i, true ).longitude;
		table->setEntry( 0, line, fmt.getObjectName( i, TF_LONG, true ));
		table->setEntry( 1, line, fmt.getPosFormatted( len ));
		table->setEntry( 2, line, lang.getNakshatra27Name( ::getNakshatra27( len )));

		// This is not defined for 28 nakshatra mode, so leave it out
		Yoni yoni = getYoni( len );
		table->setEntry( 3, line, yoni.name );
		table->setEntry( 4, line, lang.getGanaName( getGana( len )) );
		table->setEntry( 5, line, lang.getNadiName( getNadi( len )) );
		table->setEntry( 6, line, lang.getTaraName(
			getTara(  ::getNakshatra27( len ) - ::getNakshatra27( horoscope->getObjectPosition( OMOON, true ).longitude ))));

		rajju = getRajju( ::getNakshatra27( len ));
		table->setEntry( 7, line, lang.getRajjuName( rajju.aroha, rajju.type ));
		line++;
	}
	sheet->addItem( table );
}

/*****************************************************
**
**   SarvatobhadraExpert   ---   getVedhaNakshatras
**
******************************************************/
vector<NakshatraId_28> SarvatobhadraExpert::getVedhaNakshatras( const NakshatraId_28 &n )
{
	ASSERT_VALID_NAKSHATRA_28( n );
	const NakshatraId_28 nak_aspects[28][4] = {
		{ N28_ASWINI, N28_ROHINI, N28_PPHALGUNI, N28_JYESHTA },
		{ N28_BHARANI, N28_KRITTIKA, N28_MAKHA, N28_ANURADHA },
		{ N28_KRITTIKA, N28_VISAKHA, N28_SRAVANA, N28_BHARANI },
		{ N28_ROHINI, N28_SWATI, N28_ABHIJIT, N28_ASWINI },
		{ N28_MRIGASIRA, N28_CHITTA, N28_USHADHA, N28_REVATI },
		{ N28_ARUDRA, N28_HASTHA, N28_PSHADHA, N28_UBHADRA },
		{ N28_PUNARVASU, N28_UPHALGUNI, N28_MULA, N28_PBHADRA },
		{ N28_PUSHYAMI, N28_PPHALGUNI, N28_JYESHTA, N28_SATHABISHAK },
		{ N28_ASLESHA, N28_MAKHA, N28_ANURADHA, N28_DHANISHTA },
		{ N28_MAKHA, N28_SRAVANA, N28_BHARANI, N28_ASLESHA },
		{ N28_PPHALGUNI, N28_ABHIJIT, N28_ASWINI, N28_PUSHYAMI },
		{ N28_UPHALGUNI, N28_USHADHA, N28_REVATI, N28_PUNARVASU },
		{ N28_HASTHA, N28_PSHADHA, N28_UBHADRA, N28_ARUDRA },
		{ N28_CHITTA, N28_MULA, N28_PBHADRA, N28_MRIGASIRA },
		{ N28_SWATI, N28_JYESHTA, N28_SATHABISHAK, N28_ROHINI },
		{ N28_VISAKHA, N28_ANURADHA, N28_DHANISHTA, N28_KRITTIKA },
		{ N28_ANURADHA, N28_BHARANI, N28_ASLESHA, N28_VISAKHA },
		{ N28_JYESHTA, N28_ASWINI, N28_PUSHYAMI, N28_SWATI },
		{ N28_MULA, N28_REVATI, N28_PUNARVASU, N28_CHITTA },
		{ N28_PSHADHA, N28_UBHADRA, N28_ARUDRA, N28_HASTHA },
		{ N28_USHADHA, N28_PBHADRA, N28_MRIGASIRA, N28_UPHALGUNI },
		{ N28_ABHIJIT, N28_SATHABISHAK, N28_ROHINI, N28_PPHALGUNI },
		{ N28_SRAVANA, N28_MAKHA, N28_KRITTIKA, N28_DHANISHTA },
		{ N28_DHANISHTA, N28_ASLESHA, N28_VISAKHA, N28_SRAVANA },
		{ N28_SATHABISHAK, N28_PUSHYAMI, N28_SWATI, N28_ABHIJIT },
		{ N28_PBHADRA, N28_PUNARVASU, N28_CHITTA, N28_USHADHA },
		{ N28_UBHADRA, N28_ARUDRA, N28_HASTHA, N28_PSHADHA },
		{ N28_REVATI, N28_MRIGASIRA, N28_UPHALGUNI, N28_MULA }
	};

	vector<NakshatraId_28> v;
	for( int i = 0; i < 4; i++ )
	{
		v.push_back( nak_aspects[n][i] );
	}
	return v;
}

/*****************************************************
**
**   SarvatobhadraExpert   ---   writeSbcText
**
******************************************************/
void SarvatobhadraExpert::writeSbcText( Sheet *sheet, const ChartProperties *props )
{
	Lang lang;
	ObjectId i;
	double len;
	SheetFormatter fmt;

	ObjectArray obs = props->getVedicPlanetList();
	Table *table = new Table( 3, obs.size() + 1 );
	table->setHeader( 0,  _( "Planet" ));
	table->setHeader( 1,  _( "Longitude" ));
	table->setHeader( 2,  _( "Nakshatra" ));
	int line = 1;

	for ( uint p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		table->setEntry( 0, line, fmt.getObjectName( i, TF_MEDIUM, true ));
		len = horoscope->getVedicLongitude( i );
		table->setEntry( 1, line, fmt.getPosFormatted( len, horoscope->getMovingDirection( i )));
		table->setEntry( 2, line, lang.getNakshatra27Name( ::getNakshatra27( len )));
		line++;
	}
	sheet->addItem( table );
}


/*****************************************************
**
**   SarvatobhadraExpert   ---   writeSbcAfflictions
**
******************************************************/
void SarvatobhadraExpert::writeSbcAfflictions( Sheet*, const Horoscope*, const ChartProperties* )
{
	/*
	Lang lang;
	ObjectId i;
	int j, rnak, nak_index;
	ObjectId k;
	Nakshatra nak;

	const int special_nak[9] = { 0, 9, 15, 17, 18, 22, 24, 25, 26 };
	const wxChar *special_nak_name[9] = { _( "Janma" ), _( "Karmaksha" ), _( "Sanghatika" ), _( "Samudayika" ),
		_( "Adhana" ), _( "Vinasana" ), _( "Jati" ), _( "Desa" ), _( "Abhisheka" ) };

	int vedas[4];
	wxString s1;

	writer->addHeader( _( "Afflictions" ));

	//int star = getNakshatra( horoscope->getVedicLongitude( OMOON ), nakmode );

	ObjectArray obs = props->getVedicPlanetList();
	for ( uint p = 0; p < obs.size(); p++ )
	{
		i = obs[p];
		nak = ::getNakshatra27( h2->getVedicLongitude( i ));
		s1.Printf( wxT( "%s (%s)" ), (const wxChar*)writer->getObjectName(i, TF_LONG, true ),
		           (const wxChar*)lang.getNakshatraName( nak, nakmode, TF_MEDIUM ));
		writer->addHeader( s1 );
		vedas[0] = nak;
		nak_index = nak;
		if ( nakmode == N27 && nak > ABHIJIT ) nak_index++;
		vedas[1] = nak_aspects[nak_index][1];
		vedas[2] = nak_aspects[nak_index][2];
		vedas[3] = nak_aspects[nak_index][3];
		if ( nakmode == N27 )
		{
			for ( j = 1; j < 4; j++ )
			{
				if ( vedas[j] == ABHIJIT ) vedas[j] = -1;
				else if ( vedas[j] > ABHIJIT ) vedas[j]--;
			}
		}
		for ( j = 0; j < 4; j++ )
		{
			if ( vedas[j] == -1 ) continue;

			// affliction of special nakshatras
			//if ( nakmode == N27 )
			// TODO what is this
			if ( false )
			{
				for ( l = 0; l < 9; l++ )
				{
					k = star + special_nak[l];
					if ( k >= ABHIJIT ) k++;
					diff = (int)a_red( k - vedas[j], 28 );
					if ( diff == 0 )
					{
						s1.Printf( wxT( "%6s: %s" ), (const wxChar*)lang.getNakshatraName( vedas[j], nakmode, TF_MEDIUM ), special_nak_name[l] );
						writer->addLine( s1 );
					}
				}
			}

			// Radix Nakshatra afflictions
			for ( uint q = 0; q < obs.size(); q++ )
			{
				k = obs[q];
				rnak = getNakshatra( horoscope->getVedicLongitude( k ), nakmode );
				if ( rnak != vedas[j] ) continue;
				if ( horoscope == h2 && i == k ) continue;
				s1.Printf( wxT( "%6s: %s" ), (const wxChar*)lang.getNakshatraName( vedas[j], nakmode, TF_MEDIUM ),
				           (const wxChar*)writer->getObjectName( k, TF_LONG, true ));
				writer->addLine( s1 );
			}
		}
	}
	*/
}


