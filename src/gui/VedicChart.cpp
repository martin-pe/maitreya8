/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicChart.cpp
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
**   AshtakaVargaChart   ---   writeChartContents
**
******************************************************/
void AshtakaVargaChart::writeChartContents( const int& /* chart_id */, const bool /* applyFilter */ )
{
	//printf( "AshtakaVargaChart::writeChartContents\n" );
	wxString s;
	wxChar avitem = config->vedicCalculation->ashtakavargaNumberMode == 1 ? 'I' : 'o';

	for ( Rasi i = R_ARIES; i <= R_PISCES; i++ )
	{
		fields[i].contents.clear();
		s.Clear();
		if ( config->vedicCalculation->ashtakavargaNumberMode != 0 )
		{
			if ( graphicSupport )
				for ( int j = 0; j < rasi_values[i]; j++ )
				{
					if ( j == 5 )
					{
						fields[i].contents.textitems.push_back( ChartTextItem( s, s ));
						s.Clear();
					}
					s << avitem;
				}
			else s.Printf( wxT( "%d" ), rasi_values[i] );
		}
		else
		{
			s.Printf( wxT( "%d" ), rasi_values[i] );
		}
		fields[i].contents.textitems.push_back( ChartTextItem( s, s ));
	}
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
wxString VedicVargaChart::getCenterString()
{
	const int type = chartprops->getVedicChartDisplayConfig().centerInfoType;
	if ( type == VCC_NOTHING ) return wxEmptyString;

	if ( varga ==  V_BHAVA ) return _( "Bhava" );

	VargaConfigLoader *loader = VargaConfigLoader::get();
	wxString signification;

	switch( chartprops->getVedicChartDisplayConfig().centerInfoType )
	{
		case VCC_NAME:
			return loader->getVargaName( varga );
		break;
		case VCC_NAME_SIGNIFICATION:
			signification = loader->getVargaSignification( varga );
			if ( signification.IsEmpty() ) return loader->getVargaName( varga );
			else return wxString::Format( wxT( "%s (%s)" ), loader->getVargaName( varga ).c_str(), signification.c_str()); break;
		case VCC_DIVISION:
			return wxString::Format( _( "D-%d" ), loader->getVargaDivision( varga ));
		break;
		case VCC_DIVISION_SIGNIFICATION:
			signification = loader->getVargaSignification( varga );
			if ( signification.IsEmpty() ) return wxString::Format( _( "D-%d" ), loader->getVargaDivision( varga ));
			else return wxString::Format( wxT( "D-%d (%s)" ), loader->getVargaDivision( varga ), signification.c_str() );
		break;
		case VCC_NOTHING:
		default:
		break;
	}
	return wxT( "ERROR VedicVargaChart::getCenterString" );
}

/*****************************************************
**
**   VedicVargaChart   ---   updateAspects
**
******************************************************/
void VedicVargaChart::updateAspects()
{
	assert( aexpert );
	aexpert->updateVedicRasiChartAspects( chartprops, varga, false );
	if ( charttype == CT_PARTNER )
	{
		aexpert->updateVedicRasiChartAspects( chartprops, varga, true );
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
	Horoscope *h = getHoroscope( chart_id );
	if ( chart_id == 1 && field_count == 12 && chartprops->getVedicChartDisplayConfig().showAshtakavarga )
	{
		ashtakavargaExpert->setHoroscope( h );
		ashtakavargaExpert->update();
	}
	BasicVedicChart::writeChartContents( chart_id, applyFilter );

	int i, rasi;
	wxString lname, sname;

	if ( chart_id == 0 ) ascendant = getPlanetField( OASCENDANT, 0 );

	if ( chart_id == 0 && chartprops->getVedicChartDisplayConfig().showArudhas )
	{
		JaiminiExpert expert( h, varga );
		for ( i = 0; i < 12; i++ )
		{
			rasi = expert.calcPada( i );

			//fields[rasi].getContents( chart_id ).planets.push_back( (ObjectId)i );
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
				sname.Printf( _( "A%d" ), i+1 );
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



