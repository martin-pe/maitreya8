/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Transit.cpp
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

#include "Transit.h"

#include "ArabicParts.h"
#include "astrobase.h"
#include "maitreya.h"
#include "Calculator.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Table.h"
#include "VedicPlanet.h"
#include "Varga.h"

extern Config *config;

IMPLEMENT_CLASS( TransitHoroscope, Horoscope )

/*****************************************************
**
**   TransitHoroscope   ---   Constructor
**
******************************************************/
TransitHoroscope::TransitHoroscope()
		: Horoscope()
{
}

/*****************************************************
**
**   TransitHoroscope   ---   calcTransitPositions
**
******************************************************/
double TransitHoroscope::calcTransitPositions( const Horoscope *hbase, const double &transitJD,
        const bool &vedic, const double &yl, const TRANSIT_MODE &mode )
{
	double posdelta = 0.0;
	double len, dummy;
	int i;
	ObjectId p;
	Calculator *calculator = CalculatorFactory().getCalculator();

	DataSet ds( *getDataSet());
	directionJD = hbase->getJD() + ( transitJD - hbase->getJD() ) / yl;

	switch ( mode )
	{
		case TR_TRANSIT:
			setDate( transitJD );
		break;
		case TR_SOLAR_ARC:
		case TR_CONSTANT_ARC:
		case TR_LUNAR_ARC:
			setDate( hbase->getJD() );
		break;
		case TR_DIRECTION:
			setDate( directionJD );
		break;
		default:
			assert( false );
		break;
	}
	update();

	// TODO: date before birth
	// TODO: big Lunar arc can be > 360
	if ( mode == TR_SOLAR_ARC )
	{
		ds.setDate( directionJD );
		calculator->calcPosition( &ds, OSUN, len, dummy, true, vedic );
		posdelta = red_deg( len - getLongitude( OSUN, vedic ));
	}
	else if ( mode == TR_CONSTANT_ARC )
	{
		posdelta = ( transitJD - hbase->getJD() ) / yl;
	}
	else if ( mode == TR_LUNAR_ARC )
	{
		ds.setDate( directionJD );
		calculator->calcPosition( &ds, OMOON, len, dummy, true, vedic );
		posdelta = red_deg( len - getLongitude( OMOON, vedic ));
	}
	if ( posdelta != 0 )
	{
		for ( i = FIRST_EPHEM_OBJECT; i <= LAST_EPHEM_OBJECT; i++ )
		{
			object_len[i] = red_deg( object_len[i] + posdelta );

			// no retrogression for most modes
			if ( mode != TR_TRANSIT ) object_speed[i] = 1;
		}

		for ( i = 0; i < NUM_LAGNA; i++ ) lagna_len[i] = red_deg( lagna_len[ i ] + posdelta );
		for ( i = 0; i < NUM_UPA; i++ ) upagraha_len[i] = red_deg( upagraha_len[ i ] + posdelta );

		for ( i = HOUSE1; i <= HOUSE12; i++ )
		{
			whousecusp[i] = red_deg( whousecusp[i] + posdelta );
			ihousecusp[i] = red_deg( ihousecusp[i] + posdelta );
			ihousesandhi[i] = red_deg( ihousesandhi[i] + posdelta );
		}
		arieslen = posdelta;
	}
	for ( p = OSUN; p < MAX_EPHEM_OBJECTS; p++ )
	{
		ephem_planets[p]->update( getVedicLongitude( p ));
	}
	arabic_expert->update();

	updatePositionArrays();
	updateObjectPositions();

	setDate( transitJD );
	return posdelta;
}

/*****************************************************
**
**   TransitExpert   ---   Constructor
**
******************************************************/
TransitExpert::TransitExpert( Horoscope *h, const ChartProperties *chartprops )
		: Expert( h ),
		chartprops( chartprops )
{
	transitmode = TR_TRANSIT;
	init();

	htransit = new TransitHoroscope();
	transitJD = MDate::getCurrentJD();
	tzoffset = 0;
}

/*****************************************************
**
**   TransitExpert   ---   Destructor
**
******************************************************/
TransitExpert::~TransitExpert()
{
	delete htransit;
}

/*****************************************************
**
**   TransitExpert   ---   init
**
******************************************************/
void TransitExpert::init()
{
	yearlength = calcYearLength(
		chartprops->isVedic() ? config->vedicCalculation->yearLength : config->westernCalculation->yearLength, chartprops->isVedic()
	);
}

/*****************************************************
**
**   TransitExpert   ---   update
**
******************************************************/
void TransitExpert::update()
{
	posdelta = htransit->calcTransitPositions( horoscope, transitJD, chartprops->isVedic(), yearlength, transitmode );
}

/*****************************************************
**
**   TransitExpert   ---   setTransitDate
**
******************************************************/
void TransitExpert::setTransitDate( const double &jd, const double tzoffset )
{
	transitJD = jd - tzoffset / 24.0;
	this->tzoffset = tzoffset;
}

/*****************************************************
**
**   TransitExpert   ---   writeTransitHeader
**
******************************************************/
void TransitExpert::writeTransitHeader( Sheet *sheet )
{
	ASSERT_VALID_TRANSIT_MODE( transitmode );
	Formatter *formatter = Formatter::get();
	DateTimeFormatter *dfformatter = DateTimeFormatter::get();
	wxString s;

	const static wxString theader[5]  = { _( "Transit" ), _( "Solar Arc" ),
		_( "Progressive Directions" ), _( "Constant Arc" ), _( "Lunar Arc" ) };
	sheet->addHeader( theader[(int)transitmode] );

	/* obsolete
	if ( transitmode == TR_TRANSIT )
	{
		s << _( "Event date" ) << wxT( ": " )
			<< formatter->getFullDateStringFromJD( htransit->getJD(), tzoffset );
		writer->addLine( s );
	}
	*/
	if ( transitmode == TR_DIRECTION )
	{
		s << _( "Event date" ) << wxT( ": " )
			//<< formatter->getFullDateStringFromJD( htransit->getDirectionJD() );
			<< dfformatter->formatDateString( htransit->getDirectionJD(), 0, DF_INCLUDE_YEAR_BC_AD | DF_INCLUDE_TIME );
		sheet->addLine( s );
	}

	// Location
	s.Clear();
	s <<  _( "Location" ) << wxT( ": " ) <<  htransit->getLocation()->getLocName();
	sheet->addLine( s );

	// Year Length
	if ( transitmode != TR_TRANSIT )
	{
		s.Clear();
		s << _( "Year Length" ) << wxT( ": " ) << yearlength;
		sheet->addLine( s );
	}
	if ( posdelta != 0 )
	{
		s.Clear();
		s << _( "Arc" ) << wxT( ": " ) << formatter->getLenFormatted( posdelta );
		sheet->addLine( s );
	}
	sheet->addLine( wxEmptyString );
}

