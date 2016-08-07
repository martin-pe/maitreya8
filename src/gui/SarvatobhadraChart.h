/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SarvatobhadraChart.h
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

