/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicChart.cpp
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

#include "VedicChart.h"

#include "Aspect.h"
#include "Ashtakavarga.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Jaimini.h"
#include "mathbase.h"
#include "VargaBase.h"
#include "VargaHoroscope.h"

extern Config *config;

/*****************************************************
**
**   AshtakaVargaChart   ---   Constructor
**
******************************************************/
AshtakaVargaChart::AshtakaVargaChart( ChartProperties *chartprops )
		: VedicRasiChart( CT_RADIX, chartprops )
{
	extrazoom *= 1.5;
	h1set = true;
}

/*****************************************************
**
**   AshtakaVargaChart   ---   getPlanetField
**
******************************************************/
int AshtakaVargaChart::getPlanetField( const ObjectId &ob, const int &i )
{
	//printf( "AshtakaVargaChart::getPlanetField ob %d i %d ascendant %d\n", ob, ascendant );
	// is only used once for position of ascendant in north indian chart
	// in all other cases rasi_values will be used
	// -> exclude all other calls
	assert( i == 0 );
	assert( ob == OASCENDANT );
	return ascendant;
}

/*****************************************************
**
**   AshtakaVargaChart   ---   setRasiValue
**
******************************************************/
void AshtakaVargaChart::setRasiValue( const Rasi &r, const int &value )
{
	ASSERT_VALID_RASI( r );
	rasi_values[r] = value;
}

/*****************************************************
**
**   AshtakaVargaChart   ---   setRasiValues
**
******************************************************/
void AshtakaVargaChart::setRasiValues( const Rasi r[] )
{
	for ( Rasi i = R_ARIES; i <= R_PISCES; i++ ) rasi_values[i] = r[i];
}

/*****************************************************
**
**   AshtakaVargaChart   ---   getCenterString
**
******************************************************/
vector<wxString> AshtakaVargaChart::getCenterString()
{
	vector<wxString> v;
	v.push_back( centerString );
	return v;
}

/*****************************************************
**
**   AshtakaVargaChart   ---   writeChartContents
**
******************************************************/
void AshtakaVargaChart::writeChartContents( const int& /* chart_id */, const bool /* applyFilter */ )
{
	//printf( "AshtakaVargaChart::writeChartContents positionOffset %d\n", positionOffset );
	wxString s;
	int v;
	const wxChar avitem = config->vedicCalculation->ashtakavargaNumberMode == 1 ? 'I' : 'o';

	for ( Rasi i = R_ARIES; i <= R_PISCES; i++ )
	{
		v = rasi_values[red12( i + positionOffset )];
		//printf( "AshtakaVargaChart::writeChartContents Rasi %d value %d\n", i, v );

		fields[i].contents.clear();
		s.Clear();
		if ( config->vedicCalculation->ashtakavargaNumberMode != 0 )
		{
			if ( graphicSupport )
			{
				for ( int j = 0; j < v; j++ )
				{
					if ( j > 0 && ! ( j % 5 ))
					{
						fields[i].contents.textitems.push_back( ChartTextItem( s, s ));
						s.Clear();
					}
					s << avitem;
				}
			}
			else s.Printf( wxT( "%d" ), v );
		}
		else
		{
			s.Printf( wxT( "%d" ), v );
		}
		fields[i].contents.textitems.push_back( ChartTextItem( s, s ));
	}
}

/*****************************************************
**
**   ChartFactory   ---   createAshtakavargaChart
**
******************************************************/
GraphicalChart *ChartFactory::createAshtakavargaChart( ChartProperties *chartprops )
{
	return new AshtakaVargaChart( chartprops );
}

/*****************************************************
**
**   VedicVargaChart   ---   Constructor
**
******************************************************/
VedicVargaChart::VedicVargaChart( const ChartType &charttype, ChartProperties *chartprops, const Varga &varga,
	Horoscope *h1, Horoscope *h2 )
	: VedicRasiChart( charttype, chartprops, h1, h2 ),
	varga( varga )
{
	ashtakavargaExpert = new AshtakavargaExpert( h1, varga );
	ascendant = R_ARIES;
	if ( h1 ) ascendant = getPlanetField( OASCENDANT, 0 );
}

/*****************************************************
**
**   VedicVargaChart   ---   Destructor
**
******************************************************/
VedicVargaChart::~VedicVargaChart()
{
	delete ashtakavargaExpert;
}

/*****************************************************
**
**   VedicVargaChart   ---   getCenterString
**
******************************************************/
vector<wxString> VedicVargaChart::getCenterString()
{
	vector<wxString> v;
	VargaConfigLoader *loader = VargaConfigLoader::get();
	wxString signification;
	
	const int type = chartprops->getVedicChartDisplayConfig().centerInfoType;
	if ( type == VCC_NOTHING ) return v;

	if ( varga ==  V_BHAVA )
	{
		v.push_back( _( "Bhava" ));
	}
	else
	{
		switch( type )
		{
			case VCC_NAME:
				v.push_back( loader->getVargaName( varga ));
			break;
			case VCC_NAME_SIGNIFICATION:
				v.push_back( loader->getVargaName( varga ));
				signification = loader->getVargaSignification( varga );
				if ( ! signification.IsEmpty() ) v.push_back( signification );
			break;
			case VCC_DIVISION:
				v.push_back( wxString::Format( wxT( "D-%d" ), loader->getVargaDivision( varga )));
			break;
			case VCC_DIVISION_SIGNIFICATION:
				v.push_back( wxString::Format( wxT( "D-%d" ), loader->getVargaDivision( varga )));
				signification = loader->getVargaSignification( varga );
				if ( ! signification.IsEmpty() ) v.push_back( signification );
			break;
			case VCC_NOTHING:
			default:
			break;
		}
	}
	return v;
}

/*****************************************************
**
**   VedicVargaChart   ---   updateAspects
**
******************************************************/
void VedicVargaChart::updateAspects()
{
	if ( h1 )
	{
		assert( aexpert );
		aexpert->updateVedicRasiChartAspects( chartprops, varga, false );
		if ( charttype == CT_PARTNER )
		{
			aexpert->updateVedicRasiChartAspects( chartprops, varga, true );
		}
	}
}

/*****************************************************
**
**   VedicVargaChart   ---   getPlanetField
**
******************************************************/
int VedicVargaChart::getPlanetField( const ObjectId &planet, const int &chart_id )
{
	const Horoscope *h = ( chart_id == 0 ? h1 : h2 );
	if ( ! h )
	{
		printf( "WARN: horoscope in VedicVargaChart::getPlanetField is NULL, returning Aries\n" );
		return false;
	}
	return (int)h->getVargaData( planet, varga )->getRasi();
}

/*****************************************************
**
**   VedicVargaChart   ---   getAshtakavargaPoints
**
******************************************************/
int VedicVargaChart::getAshtakavargaPoints( const ObjectId &planet, const int &sign )
{
	return ashtakavargaExpert->getItem( REKHA, planet, sign );
}

/*****************************************************
**
**   VedicVargaChart   ---   writeChartContents
**
******************************************************/
void VedicVargaChart::writeChartContents( const int &chart_id, const bool applyFilter )
{
	int rasi;
	wxString lname, sname;

	Horoscope *h = getHoroscope( chart_id );
	if ( chart_id == 1 && field_count == 12 && chartprops->getVedicChartDisplayConfig().showAshtakavarga )
	{
		ashtakavargaExpert->setHoroscope( h );
		ashtakavargaExpert->update();
	}
	BasicVedicChart::writeChartContents( chart_id, applyFilter );
	
	// print house numbers along with planets in KP Chart
	if ( chart_id == 0 && chartprops->isKpChart() )
	{
		for ( ObjectId p = OHOUSE1; p <= OHOUSE12; p++ )
		{
			rasi = h->getVargaData( p, V_BHAVA )->getRasi();
			sname = getHouseNumberFormatted( (int) p - OHOUSE1, 2 );
			lname = sname;
			fields[rasi].getContents( chart_id ).textitems.push_back( ChartTextItem( lname, sname, false ));
		}
	}

	if ( chart_id == 0 && chartprops->getVedicChartDisplayConfig().showArudhas )
	{
		JaiminiExpert expert( h, varga );

		for ( int i = 0; i < 12; i++ )
		{
			rasi = red12( expert.calcPada( i ) - positionOffset );

			if ( i == 0 )
			{
				sname = _( "AL" );
				lname = _( "Arudha L" );
			}
			else if ( i == 11 )
			{
				sname = _( "UL" );
				lname = _( "Upapada L" );
			}
			else
			{
				sname.Printf( wxT( "A%d" ), i+1 );
				lname = sname;
			}
			fields[rasi].getContents( chart_id ).textitems.push_back( ChartTextItem( lname, sname, false ));
		}
	}
}

/*****************************************************
**
**   ChartFactory   ---   createVedicChart
**
******************************************************/
GraphicalChart *ChartFactory::createVedicChart( const ChartType &charttype, ChartProperties *chartprops,
	Horoscope *h1, Horoscope *h2, const Varga &varga )
{
	return new VedicVargaChart( charttype, chartprops, varga, h1, h2 );
}


