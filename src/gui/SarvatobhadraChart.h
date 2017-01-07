/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SarvatobhadraChart.h
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

#ifndef SARVATOBHADRACHART_H_
#define SARVATOBHADRACHART_H_

#include <wx/string.h>

#include "Nakshatra.h"
#include "BasicVedicChart.h"

class ChartProperties;

/*************************************************//**
*
* \brief creates GUI represenation of SBC
*
******************************************************/
class SarvatobhadraChart : public BasicVedicChart
{
public:

	SarvatobhadraChart( const ChartType&, ChartProperties*, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	~SarvatobhadraChart();

	virtual void setupChart();
	virtual void drawGrid();
	virtual void finishChart();

	virtual bool handleMouseLeftEvent( int x, int y, wxRect& );

private:
	double xstep, ystep;
	int mark[9][9];
	vector<NakshatraProperties> nakshatraProps;

	virtual void updateAspects();

	// Coordinate methods
	bool getFieldIndex( int *x, int *y );
	MRect getFieldCoord( const int &x, const int &y );
	MRect getWeekdayCoord( const int& );
	MRect getRasiCoord( const Rasi& );
	void getNakshatraIndex( const NakshatraId_28 &i, int &x, int &y );
	void init();

	virtual void getRasiIndex( const Rasi &r, int &x, int &y );

	virtual bool isChartFieldMarked( const int& );

	void getWeekdayIndex( const int &w, int &x, int &y );
	MRect getNakshatraCoord( const NakshatraId_28& );
	void markField( int x, int y );

	virtual int getPlanetField( const ObjectId&, const int &chart_id );

};

#endif

