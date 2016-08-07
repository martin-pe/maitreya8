/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicChart.h
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

#ifndef VEDICCHART_H_
#define VEDICCHART_H_

#include "VedicRasiChart.h"

class AshtakavargaExpert;
class ChartProperties;
class Horoscope;

/*************************************************//**
*
* \brief Vedic chart graphic based upon a Varga chart
*
******************************************************/
class VedicVargaChart : public VedicRasiChart
{
public:

	VedicVargaChart( const ChartType&, ChartProperties*, const Varga&, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	~VedicVargaChart();

protected:
	const Varga varga;

	void updateAspects();

	virtual wxString getCenterString();

	virtual int getPlanetField( const ObjectId &planet, const int &chart_id );

	virtual void writeChartContents( const int &chart_id, const bool applyFilter = false );
	virtual int getAshtakavargaPoints( const ObjectId &planet, const int &sign );

	AshtakavargaExpert *ashtakavargaExpert;
};

/*************************************************//**
*
* \brief Ashtakavarga chart based on abstract Vedic Rasi chart
*
******************************************************/
class AshtakaVargaChart : public VedicRasiChart
{
public:
	AshtakaVargaChart( ChartProperties* );
	void setAscendant( const int &a ) { ascendant = a; }
	void setRasiValue( const Rasi &r, const int &value );
	void setRasiValues( const Rasi r[] );
	void setCenterString( wxString cs ) { centerString = cs; }
	void setGraphicSupport( const bool &b ) { graphicSupport = b; }

protected:
	void updateAspects() {}

	virtual wxString getCenterString() { return centerString; }
	virtual void writeChartContents( const int &chart_id, const bool applyFilter = false );

	virtual int getPlanetField( const ObjectId&, const int&) { return 0; }

	wxString centerString;
	int rasi_values[12];
	bool graphicSupport;

private:

};

#endif

