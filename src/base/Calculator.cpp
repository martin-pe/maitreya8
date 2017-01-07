/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Calculator.cpp
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

#include "Calculator.h"

#include <wx/log.h>
#include <wx/string.h>

#ifndef TRUE
#define TRUE 1
#endif
//#define TRUE 1

#ifdef __WXMSW__
#undef TRUE
#endif
#ifndef _cplusplus
extern "C" {
#endif
#include "swephexp.h"
#ifndef _cplusplus
}
#endif

#include "Conf.h"
#include "DataSet.h"
#include "mathbase.h"

extern Config *config;

#define PRINT_PEDANTIC	if ( err[0] != 0 ) { wxString msg = wxConvCurrent->cMB2WX( err ); wxLogMessage( wxString::Format( wxT( "Swe: %s" ), msg.c_str())); }

/*****************************************************
**
**   EclipseData   ---   Constructor
**
******************************************************/
EclipseData::EclipseData()
{
	jd = sunpos = moonpos = 0;
	planet = type = 0;
	description = wxT( "" );
}

/*************************************************//**
*
* \brief Swiss Ephemeris implementation of Calculator
*
******************************************************/
class CalculatorSwe : public Calculator
{
public:

	/*****************************************************
	**
	**   CalculatorSwe   ---   Constructor
	**
	******************************************************/
	CalculatorSwe()
	{
		init();
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   Destructor
	**
	******************************************************/
	virtual ~CalculatorSwe()
	{
		swe_close();
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   init
	**
	******************************************************/
	void init()
	{
		//if ( ! config->ephem->sePath.IsEmpty() )
		if ( true )
		{
			//printf( "SEPATH %s\n", str2char( config->ephem->sePath ));
			swe_set_ephe_path( (char*)str2char( config->ephem->sePath ));
			wxLogMessage( wxString::Format( wxT( "Ephem path: %s\n" ), config->ephem->sePath.c_str()));
		}
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcAyanamsa
	**
	******************************************************/
	virtual wxString getVersion()
	{
		char buf[256];
		wxString s;
		s = wxConvertMB2WX( swe_version( buf ));
		return s;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcAyanamsa
	**
	******************************************************/
	double calcAyanamsa( const double &jd, const AYANAMSA type = AY_NONE )
	{
		struct aya_config
		{
		public:
			double t0, ayan_t0;
		};
		static const aya_config aya_param[3] =
		{
			{ 2415020.0, 360 - 337.53953 },   // 1: Lahiri (Robert Hand) 
			{ 2415020.0, 360 - 338.98556 },   // 3: Raman (Robert Hand) 
			{ 2415020.0, 360 - 337.636111 }  // 5: Krishnamurti (Robert Hand) 
		};

		if ( type == AY_NONE ) return 0;

		double t, t0, ayan_t0;
		if ( type >= AY_LAHIRI && type <= AY_KRISHNAMURTI )
		{
			t0 = aya_param[(int)(type-1)].t0;
			ayan_t0 = aya_param[(int)(type-1)].ayan_t0;
		}
		else if ( type == AY_CUSTOM )
		{
			t0 = config->ephem->custom_t0;
			ayan_t0 = config->ephem->custom_ayan_t0;
		}
		else
		{
			wxLogError( wxString::Format( wxT( "Wrong ayanamsa %d\n" ), (int)type ));
			assert( false );
		}
		if ( config->ephem->custom_aya_constant )
		{
			t = jd - t0;
			double years = t/365.25;
			double portion = years/config->ephem->custom_aya_period;
			double aya = portion * 360;
			//return red_deg( config->custom_ayan_t0 + 360 * ( jd - config->custom_t0 ) / ( config->custom_aya_period * 365.25 ));

			// bugfix 6.0: forgot ayan_t0
			//return red_deg( aya );
			return red_deg( ayan_t0 + aya );
		}
		else
		{
			swe_set_sid_mode( 255, t0, ayan_t0 );
			return swe_get_ayanamsa( calcJd( jd ));
		}
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcSiderealTime
	**
	******************************************************/
	double calcSiderealTime( const double &jd , const double &longitude )
	{
		return swe_sidtime( jd + longitude / 360 );
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcEps
	**
	******************************************************/
	void calcEps( double &eps, const double &jd )
	{
		char err[256];
		double x[6];

		// x[0] enthaelt eps
		if ( swe_calc( calcJd( jd ), SE_ECL_NUT, 0, x, err) == -1 )
		{
			assert( false );
		}
		PRINT_PEDANTIC
		eps = x[0];
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcNextSolarEvent
	**
	******************************************************/
	double calcNextSolarEvent( const int &type, const double &jd, const double &lat, const double &lon )
	{
		char err[256];
		double rsmi[3], tret[3];

		int flag = 0;
		if ( ! config->ephem->sunrise_def ) flag = SE_BIT_DISC_CENTER;
		if ( ! config->ephem->sunrise_refrac ) flag |= SE_BIT_NO_REFRACTION;

		//int event_flag = 0;
		switch ( type )
		{
		case SOLAR_EVENT_SUNRISE:
			flag |= SE_CALC_RISE;
			break;
		case SOLAR_EVENT_SUNSET:
			flag |= SE_CALC_SET;
			break;
		case SOLAR_EVENT_NOON:
			flag |= SE_CALC_ITRANSIT;
			break;
		case SOLAR_EVENT_MIDNIGHT:
			flag |= SE_CALC_MTRANSIT;
			break;
		default:
			assert( false );
			break;
		}

		rsmi[0] = lon;
		rsmi[1] = lat;
		rsmi[2] = 0;

		swe_rise_trans( calcJd( jd ), SE_SUN, 0, 0, flag, rsmi, 0, 0, tret, err );
		PRINT_PEDANTIC
		return tret[0];
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcSunRiseSunSet
	**
	******************************************************/
	bool calcSunRiseSunSet( DataSet *d, double *sunrise = (double*)NULL, double *sunset = (double*)NULL, double *noon = (double*)NULL, double *midnight = (double*)NULL )
	{
		char err[256];
		bool daybirth = true;
		double r[6];
		double cusp[13], ascmc[10];
		double rsmi[3], tret[3];
		double startjdrise, startjdset, startjdnoon, startjdmidnight;
		int flag = 0;

		if ( ! config->ephem->sunrise_def ) flag = SE_BIT_DISC_CENTER;
		if ( ! config->ephem->sunrise_refrac ) flag |= SE_BIT_NO_REFRACTION;
		double jd = calcJd( d->getJD() );

		swe_calc( jd, OSUN, 0, r,err );
		PRINT_PEDANTIC
		swe_houses( jd, d->getLocation()->getLatitude(), d->getLocation()->getLongitude(), 'E', cusp, ascmc );

		double diff_ascsun = red_deg( ascmc[0] -  r[0] );  // Sun and AC
		if ( diff_ascsun > 180 ) daybirth = false;

		double diff_icsun = red_deg( ascmc[1] + 180 - r[0] );  // Sun and IC

		startjdrise = startjdset = startjdnoon = startjdmidnight = jd;

		if ( daybirth )
		{
			if ( diff_icsun < 180 )
			{
				// forenoon
				startjdrise--;
			}
			else
			{
				// afternoon
				startjdrise--;
				startjdnoon--;
			}
		}
		else
		{
			if ( diff_icsun < 180 )
			{
				// morning before sunrise
				startjdrise--;
				startjdset--;
				startjdnoon--;
				startjdmidnight--;
			}
			else
			{
				// evening after sunset
				startjdrise--;
				startjdset--;
				startjdnoon--;
			}
		}

		rsmi[0] = d->getLocation()->getLongitude();
		rsmi[1] = d->getLocation()->getLatitude();
		rsmi[2] = 0;

		if ( sunrise )
		{
			swe_rise_trans( startjdrise, SE_SUN, 0, 0, SE_CALC_RISE|flag, rsmi, 0, 0, tret, err );
			PRINT_PEDANTIC
			*sunrise = tret[0];
		}
		if ( sunset )
		{
			swe_rise_trans( startjdset, SE_SUN, 0, 0, SE_CALC_SET|flag, rsmi, 0, 0, tret, err );
			PRINT_PEDANTIC
			*sunset = tret[0];
		}

		if ( noon )
		{
			swe_rise_trans( startjdnoon, SE_SUN, 0, 0, SE_CALC_MTRANSIT|flag, rsmi, 0, 0, tret, err );
			PRINT_PEDANTIC
			*noon = tret[0];
		}

		if ( midnight )
		{
			swe_rise_trans( startjdmidnight, SE_SUN, 0, 0, SE_CALC_ITRANSIT|flag, rsmi, 0, 0, tret, err );
			PRINT_PEDANTIC
			*midnight = tret[0];
		}
		return daybirth;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcAscendantPortion
	**
	******************************************************/
	double calcAscendantPortion( DataSet *d, const double &alen )
	{
		int nakshatra;
		double nak_len, jd1, jd2, len1, len2;

		nakshatra = (int)( alen / NAKSHATRA_LEN );
		nak_len = alen - nakshatra * NAKSHATRA_LEN;
		if ( config->vedicCalculation->nakshatraPortionMode ) // according to entry time
		{
			len1 = nakshatra * NAKSHATRA_LEN;
			len2 = ( nakshatra + 1 ) * NAKSHATRA_LEN;
			jd1 = CalcDateForAscendantPos( d, len1 );
			jd2 = CalcDateForAscendantPos( d, len2 );
			return ( d->getJD() - jd1 ) / ( jd2 - jd1 );
		}
		else // according to length
		{
			return nak_len / NAKSHATRA_LEN;
		}
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcNakshatraPortion
	**
	******************************************************/
	double calcNakshatraPortion( DataSet *d, const double &mlen, const bool usepada = false )
	{
		int nakshatra, pada;
		double nak_len, p_len, len1, len2, jd1, jd2;

		nakshatra = (int)( mlen / NAKSHATRA_LEN );
		nak_len = mlen - nakshatra * NAKSHATRA_LEN;
		pada = (int)(nak_len / PADA_LEN );
		p_len = mlen - ( nakshatra * NAKSHATRA_LEN + pada * PADA_LEN );
		if ( usepada )
		{
			len1 = nakshatra * NAKSHATRA_LEN + pada * PADA_LEN;
			len2 = nakshatra * NAKSHATRA_LEN + ( pada + 1 ) * PADA_LEN;
		}
		else
		{
			len1 = nakshatra * NAKSHATRA_LEN;
			len2 = ( nakshatra + 1 ) * NAKSHATRA_LEN;
		}

		if ( config->vedicCalculation->nakshatraPortionMode ) // according to entry time
		{
			jd1 = CalcDateForMoonPos( d, len1 );
			jd2 = CalcDateForMoonPos( d, len2 );
			return ( d->getJD() - jd1 ) / ( jd2 - jd1 );
		}
		else // according to length
		{
			if ( ! usepada ) return nak_len / NAKSHATRA_LEN;
			else return p_len / PADA_LEN;
		}
	}


	/*****************************************************
	**
	**   CalculatorSwe   ---   calcPositionSpeed
	**
	******************************************************/
	void calcPositionSpeed( DataSet *d, const int &planet, double &slen, double &slat, double &sspeed,
		const bool aya = false, const bool indian = false )
	{
		double r[6];
		char err[256];

		if ( planet == OMEANDESCNODE )
		{
			swe_calc( calcJd( d->getJD() ), mapOToS( OMEANNODE ), calcIFlag( d ) | SEFLG_SPEED, r, err );
			r[0] += 180;
		}
		else if ( planet == OTRUEDESCNODE )
		{
			swe_calc( calcJd( d->getJD() ), mapOToS( OTRUENODE ), calcIFlag( d ) | SEFLG_SPEED, r, err );
			r[0] += 180;
		}
		else swe_calc( calcJd( d->getJD() ), mapOToS( planet ), calcIFlag( d ) | SEFLG_SPEED, r, err );
		PRINT_PEDANTIC

		if ( r[0] != 0.0 )
		{
			slen = red_deg( r[0] - ( aya ? calcAyanamsa( d->getJD(), ( indian ? config->vedicCalculation->ayanamsa : config->westernCalculation->ayanamsa )) : 0 ));
			slat = r[1];
			sspeed = r[3];
			if ( sspeed == 0 )
			{
				wxLogWarning( wxString::Format( wxT( "WARN CalculatorSwe::calcPositionSpeed speed is null planet %d len %.12f\n" ), planet, slen ));
			}
		}
		else
		{
			// happens if data files are missing (planetoids, Chiron)
			slen = 0;
			slat = 0;
			sspeed = 0;
		}
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcPosition
	**
	******************************************************/
	void calcPosition( DataSet *d, const int &planet, double &slen, double &slat,
		const bool aya = false, const bool indian = false )
	{
		double r[6];
		char err[256];
		swe_calc( calcJd( d->getJD() ), mapOToS( planet ), calcIFlag( d ), r, err );
		PRINT_PEDANTIC
		slen = red_deg( r[0] - ( aya ? calcAyanamsa( d->getJD(),
			( indian ? config->vedicCalculation->ayanamsa : config->westernCalculation->ayanamsa )) : 0 ));
		slat = r[1];
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcHouses
	**
	******************************************************/
	void calcHouses( const double &jd, const double &lon, const double &lat, const int &hsys, double *r )
	{
		double cusp[13], ascmc[10];
		swe_houses( jd, lat, lon, getHouseSystemCode( hsys ), cusp, ascmc );
		for ( int i = 0; i < 12; i++ ) r[i] = cusp[1+i];
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calc_jd
	**
	******************************************************/
	double calc_jd( const int &year, const int &month, const int &day, const double &time, const bool gregflag = true )
	{
		return swe_julday( year, month, day, time, gregflag ? 1 : 0 );
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calc_and_check_jd
	**
	** returns 
	**  - true if okay
	**  - false if not okay
	**
	******************************************************/
	bool calc_and_check_jd( const int &year, const int &month, const int &day, const double &time, double &jd )
	{
		const char c = isGregorianCalendarDate( year, month, day ) ? 'g' : 'j';
		const int ret = swe_date_conversion( year, month, day, time, c, &jd );
		if ( ret == ERR ) return true;
		if ( year < MIN_EPHEM_YEAR || year > MAX_EPHEM_YEAR )
		{
			return true;
		}
		else return false;
	}

#define GREG_CALANDAR_START_JD 2299160.5
	/*****************************************************
	**
	**   CalculatorSwe   ---   isGregorianCalendarDate
	**
	******************************************************/
	bool isGregorianCalendarDate( const int &year, const int &month, const int &day )
	{
		// Oct 14 1582
		//if ((int32) jyear * 10000L + (int32) jmon * 100L + (int32) jday < 15821015L)
		return year > 1582 || month > 10 || day > 14;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   isGregorianCalendarDate
	**
	******************************************************/
	bool isGregorianCalendarDate( const double &jd )
	{
		return jd > GREG_CALANDAR_START_JD;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcTajakaJd
	**
	******************************************************/
	double calcTajakaJd( const DataSet *dataset, const int &tyear, const bool &vedic )
	{
		double diff, slen, dummy, speed, birthlen, dummy1;
		int count = 0;
		int dummy2, dummy3, year;
		getDateIntsFromJD( dataset->getJD(), year, dummy2, dummy3, dummy1 );

		DataSet d( *dataset );
		calcPosition( &d, 0, birthlen, dummy, true, vedic );
		d.setDate( dataset->getJD() + ( tyear - year ) * 365.25 );
		do
		{
			calcPositionSpeed( &d, 0, slen, dummy, speed, true, vedic );

			diff = slen - birthlen;
			if ( diff > 200 ) diff -= 360;
			if ( diff < -200 ) diff += 360;
			d.setDate( d.getJD() - diff / speed );
		}
		while (( fabs( diff ) > 0.0000001 ) && ( count++ < 20 ));

		if ( count > 19 ) wxLogError( wxT( "Too many iterations for calculation of Sun length\n" ));
		return d.getJD();
	}


	/***************************************************************
	**
	** CalculatorSwe   ----  getDateIntsFromJD
	**
	***************************************************************/
	bool getDateIntsFromJD( const double &jd, int &year, int &month, int &day, double &ut )
	{
		swe_revjul( jd, jd >= GREG_CALANDAR_START_JD ? 1 : 0, &year, &month, &day, &ut );
		return true;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcAscendant
	**
	******************************************************/
	double calcAscendant( const double &jd, const double &lat, const double &lon )
	{
		double cusp[13], ascmc[10];
		swe_houses( jd, lat, lon, 'P', cusp, ascmc );

		return ascmc[0];
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcAscendantAya
	**
	******************************************************/
	double calcAscendantAya( const double &jd, const double &lat, const double &lon, bool indian )
	{
		double ayanamsa = 0;
		double cusp[13], ascmc[10];
		swe_houses( jd, lat, lon, 'P', cusp, ascmc );

		ayanamsa = calcAyanamsa( calcJd( jd ), ( indian ? config->vedicCalculation->ayanamsa : config->westernCalculation->ayanamsa ));
		return red_deg( ascmc[0] - ayanamsa );
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcAscendantByArmc
	**
	******************************************************/
	double calcAscendantByArmc( const double &armc, const double &eps, const double &lat, double *r, const int hsys = HS_PLACIDUS )
	{
		int i;
		double cusp[13], ascmc[10];
		swe_houses_armc( armc, lat, eps, getHouseSystemCode( hsys ), cusp, ascmc );
		if ( r != 0 )
		{
			for ( i = 0; i < 12; i++ ) r[i] = cusp[1+i];
		}
		return ascmc[0];
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcMC
	**
	******************************************************/
	double calcMC( const double &jd, const double &lat, const double &lon )
	{
		double cusp[13], ascmc[10];
		swe_houses( jd, lat, lon, 'P', cusp, ascmc );

		return ascmc[1];
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcMCAya
	**
	******************************************************/
	double calcMCAya( const double &jd, const double &lat, const double &lon, bool indian )
	{
		double cusp[13], ascmc[10];
		swe_houses( jd, lat, lon, 'P', cusp, ascmc );

		return red_deg( ascmc[1] - calcAyanamsa( jd, ( indian ? config->vedicCalculation->ayanamsa : config->westernCalculation->ayanamsa )  ));
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcNextEclipse
	**
	******************************************************/
	EclipseData calcNextEclipse( const double &jd, const int &planet, const int & /* type */, const bool &vedic )
	{
		EclipseData data;
		double tret[10];
		char err[256];
		int ret;
		double dummy;
		DataSet d( *config->defaultLocation );
		d.setDate( jd );

		if ( planet == OSUN ) ret = swe_sol_eclipse_when_glob( calcJd( jd ), 0, 0, tret, false, err );
		else ret = swe_lun_eclipse_when( calcJd( jd ), 0, 0, tret, false, err );
		PRINT_PEDANTIC
		data.jd = tret[0];
		d.setDate( data.jd );
		data.type = ret;
		data.planet = planet;
		calcPosition( &d, 0, data.sunpos, dummy, true, vedic );
		calcPosition( &d, 1, data.moonpos, dummy, true, vedic );
		if ( ret & SE_ECL_TOTAL ) data.description = _( "Total" );
		else if ( ret & SE_ECL_ANNULAR ) data.description = _( "Annular" );
		else if ( ret & SE_ECL_PARTIAL ) data.description = _( "Partial" );
		else if ( ret & SE_ECL_PENUMBRAL ) data.description = _( "Penumbral" );
		else if ( ret & SE_ECL_ANNULAR_TOTAL ) data.description = _( "Annular Total" );
		else data.description = _( "unknown" );

		return data;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcMcFromAsc
	**
	******************************************************/
	double calcMcFromAsc( const double &ascpos, const double &jd, const double &lon, const double &lat )
	{
		double pos[3], xin[2], xaz[3];
		pos[0] = lon;
		pos[1] = lat;
		pos[2] = 0;

		xin[0] = ascpos;
		xin[1] = 0;
		//xin[0] = 0;

		swe_azalt( jd, 	SE_ECL2HOR, pos, 0, 10, xin, xaz );
		return 0;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcPlanetaryEvent
	**
	******************************************************/
	double calcPlanetaryEvent( const DataSet *d, const double &targetlen, const int &planet,
					const bool &vedic )
	{
		double diff, len, dummy, speed, tlen;
		DataSet dataset( *d );

		int count = 0;
		int pp = planet;

		tlen = targetlen;
		// map both rahu and ketu to the same node (depending on conf for inode)
		if ( planet == OMEANDESCNODE )
		{
			pp = OMEANNODE;
			tlen = red_deg( tlen + 180 );
		}
		else if ( planet == OTRUEDESCNODE )
		{
			pp = OTRUENODE;
			tlen = red_deg( tlen + 180 );
		}

		do
		{
			calcPositionSpeed( &dataset, pp, len, dummy, speed, true, vedic );

			// bugfix 6.0: found ingress event with speed=0 (Month 1/11, planet Jupiter, western ephemeris)
			if ( speed == 0 )
			{
				wxLogWarning( wxString::Format( wxT( "CalculatorSwe::calcPlanetaryEvent WARN: planetary speed is null: planet %d, targetlen %f date %f calculated len %.12f\n" ),
								planet, targetlen, dataset.getJD(), len ));
				speed = .0000000001;
			}

			//if ( planet == OTRUEDESCNODE || planet == OMEANDESCNODE ) len = red_deg( len + 180 ); // ketu gets len of rahu + 180

			diff = len - tlen;
			if ( diff > 200 ) diff -= 360;
			if ( diff < -200 ) diff += 360;
			dataset.setDate( dataset.getJD() - diff / speed );
		}
		while (( fabs( diff ) > 0.0000001 ) && ( count++ < 20 ));

		if ( count > 19 )
		{
			wxLogError( wxT( "Too many iterations for calculation of planetary length\n" ));
		}
		return dataset.getJD();
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcSunMoonEvent
	**
	******************************************************/
	double calcSunMoonEvent( DataSet *d, const double &angle, double &slen, double &mlen )
	{
		double mspeed, sspeed, dummy, a, diff;
		int count = 0;
		const double targettol = .000001;
		do
		{
			calcPositionSpeed( d, OSUN, slen, dummy, sspeed, true, false );
			calcPositionSpeed( d, OMOON, mlen, dummy, mspeed, true, false );
			a = red_deg( mlen - slen );
			diff = angle - a;
			if ( diff > 200 ) diff -= 360;
			if ( diff < -200 ) diff += 360;
			d->setDate( d->getJD() + diff / ( mspeed - sspeed ));

			count++;
		}
		while ( fabs( diff ) > targettol && count < 20 );

		return d->getJD();
	}

private:

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcJd
	**
	******************************************************/
	double calcJd( const double &jd )
	{
		return( config->ephem->useEphemerisTime ? jd : jd + swe_deltat( jd ));
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   mapOToS
	**
	******************************************************/
	int mapOToS( const int &i )
	{
		switch ( i )
		{
		case OSUN      :
			return SE_SUN;
			break;
		case OMOON     :
			return SE_MOON;
			break;
		case OMERCURY  :
			return SE_MERCURY;
			break;
		case OVENUS    :
			return SE_VENUS;
			break;
		case OMARS     :
			return SE_MARS;
			break;
		case OJUPITER  :
			return SE_JUPITER;
			break;
		case OSATURN   :
			return SE_SATURN;
			break;
		case OURANUS   :
			return SE_URANUS;
			break;
		case ONEPTUNE  :
			return SE_NEPTUNE;
			break;
		case OPLUTO    :
			return SE_PLUTO;
			break;
		case OMEANNODE :
			return SE_MEAN_NODE;
			break;
		case OTRUENODE :
			return SE_TRUE_NODE;
			break;
		case OCUPIDO   :
			return SE_CUPIDO;
			break;
		case OHADES    :
			return SE_HADES;
		break;
		case OZEUS     :
			return SE_ZEUS;
			break;
		case OKRONOS   :
			return SE_KRONOS;
			break;
		case OAPOLLON  :
			return SE_APOLLON;
			break;
		case OADMETOS  :
			return SE_ADMETOS;
			break;
		case OVULKANUS :
			return SE_VULKANUS;
			break;
		case OPOSEIDON :
			return SE_POSEIDON;
			break;
		case OCHIRON   :
			return SE_CHIRON;
			break;
		case OPHOLUS   :
			return SE_PHOLUS;
			break;
		case OCERES    :
			return SE_CERES;
			break;
		case OPALLAS   :
			return SE_PALLAS;
			break;
		case OJUNO     :
			return SE_JUNO;
			break;
		case OVESTA    :
			return SE_VESTA;
			break;
		case OLILITH   :
			return SE_MEAN_APOG;
			break;
		default        :
			assert( 0 );
			break;
		}
		return -1;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   getHouseSystemCode
	**
	******************************************************/
	int getHouseSystemCode( const int &hsys )
	{
		int c = 'P';
		switch ( hsys )
		{
		case HS_NONE:
		case HS_PLACIDUS:
			c = 'P';
			break;
		case HS_KOCH:
			c = 'K';
			break;
		case HS_REGIOMONTANUS:
			c = 'R';
			break;
		case HS_CAMPANUS:
			c = 'C';
			break;
		case HS_PORPHYRY:
			c = 'O';
			break;
		case HS_EQUAL:
			c = 'E';
			break;
		case HS_VEHLOW:
			c = 'V';
			break;
		case HS_AXIAL:
			c = 'X';
			break;
		case HS_AZIMUTAL:
			c = 'H';
			break;
		case HS_POLICH:
			c = 'T';
			break;
		case HS_ALCABITUS:
			c = 'B';
			break;
		case HS_MORINUS:
			c = 'M';
			break;
		case HS_KRUSINSKI:
			c = 'U';
			break;
		default:
			assert( false );
			break;
		}
		return c;
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   CalcDateForAscendantPos
	**
	******************************************************/
	double CalcDateForAscendantPos( DataSet *d, const double &dest )
	{
		double diff, alen, speed;
		int count = 0;
		DataSet dataset( *d );
		Location *loc = dataset.getLocation();
		double lat = loc->getLatitude();
		double lon = loc->getLongitude();
		speed = 720;
		do
		{
			//calcPositionSpeed( &dataset, 1, alen, dummy, mspeed, true, true );
			alen = calcAscendantAya( dataset.getJD(), lat, lon, true );

			diff = alen - dest;
			if ( diff > 200 ) diff -= 360;
			if ( diff < -200 ) diff += 360;
			dataset.setDate( dataset.getJD() - diff / speed );
		}
		while (( fabs( diff ) > 0.0001 ) && ( count++ < 40 ));

		if ( count > 39 ) printf("Too many iterations for calculation of ascecndant length\n" );
		return dataset.getJD();
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   CalcDateForMoonPos
	**
	******************************************************/
	double CalcDateForMoonPos( DataSet *d, const double &dest )
	{
		double diff, mlen, dummy, mspeed;
		int count = 0;
		DataSet dataset( *d );
		do
		{
			calcPositionSpeed( &dataset, 1, mlen, dummy, mspeed, true, true );

			diff = mlen - dest;
			if ( diff > 200 ) diff -= 360;
			if ( diff < -200 ) diff += 360;
			dataset.setDate( dataset.getJD() - diff / mspeed );
		}
		while (( fabs( diff ) > 0.0000001 ) && ( count++ < 20 ));

		if ( count > 19 ) printf("Too many iterations for calculation of moon length\n" );
		return dataset.getJD();
	}

	/*****************************************************
	**
	**   CalculatorSwe   ---   calcIFlag
	**
	******************************************************/
	long calcIFlag( DataSet *d )
	{
		long iflag = 0;
		if ( config->ephem->useTruePositions ) iflag |= SEFLG_TRUEPOS;
		switch ( config->ephem->ephemCoordinateSystem )
		{
		case EPHEM_COORDINATES_GEOCENTRIC:
			// do nothing
			break;
		case EPHEM_COORDINATES_TOPOCENTRIC:
			swe_set_topo( d->getLocation()->getLongitude(), d->getLocation()->getLatitude(), 0 );
			iflag |= SEFLG_TOPOCTR;
			break;
		case EPHEM_COORDINATES_EQUATORIAL:
			iflag |= SEFLG_EQUATORIAL;
			break;
		case EPHEM_COORDINATES_BARYCENTRIC:
			iflag |= SEFLG_BARYCTR;
			break;
		case EPHEM_COORDINATES_HELIOCENTRIC:
			iflag |= SEFLG_HELCTR ;
			break;
		default:
			assert( false );
			break;
		}
		return iflag;
	}
};

Calculator *CalculatorFactory::calculator = 0;
/*****************************************************
**
**   CalculatorFactory   ---   getCalculator
**
******************************************************/
Calculator *CalculatorFactory::getCalculator()
{
	if ( calculator == 0 )
	{
		calculator = new CalculatorSwe();
		calculator->init();
	}
	return calculator;
}



