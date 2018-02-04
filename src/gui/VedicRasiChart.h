/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicRasiChart.h
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

#ifndef VEDICRasiCHART_H_
#define VEDICRasiCHART_H_

#include "BasicVedicChart.h"
#include <vector>

using namespace std;

/*************************************************//**
*
* \brief Basic abstract class for Vedic charts with 12 Rasi fields
*
******************************************************/
class VedicRasiChart : public BasicVedicChart
{
public:

	/**
	 *  \brief Constructor. Chartcount is = 1 for single chart, 2 for souble charts (transit or partner chart)
	 */
	VedicRasiChart( const ChartType&, ChartProperties*, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	/**
	 *  \brief set graphic or text in the middle of the chart
	 */
	void setCenterGraphicFormat( const bool &b ) { centerstring_graphic = b; }

protected:

	virtual void setupChart();

	virtual void  drawGrid();
	
	virtual void finishChart();


	/**
	 *  \brief Varga name or division in chart center (according to config)
	 */
	void paintCenterString();

	/**
	 *  \brief Provides text in the middle of the chart, must be implemented by subclasses.
	 */
	virtual vector<wxString> getCenterString() = 0;

	/**
	 *  \brief Used by North Indian chart
	 */
	int getAscendant() { return ascendant; }

	/**
	 *  \brief Ascendant of the chart (used by North Indian chart)
	 */
	int ascendant;

	/**
	 *  \brief determines if text or graphic should be shown in the middle of the chart
	 */
	bool centerstring_graphic;

private:

	void calculateCenterRect();

	wxString getIndianSignLabel( const Rasi&, const int& );

	MRect centerRect;
	
	void setupSouth();

	void setupNorth();

	void setupEast();

};

#endif

