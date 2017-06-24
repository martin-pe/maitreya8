/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicChart.h
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

	virtual vector<wxString> getCenterString();

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

	virtual vector<wxString> getCenterString();
	virtual void writeChartContents( const int &chart_id, const bool applyFilter = false );

	virtual int getPlanetField( const ObjectId&, const int&);

	wxString centerString;
	int rasi_values[12];
	bool graphicSupport;

private:

};

#endif

