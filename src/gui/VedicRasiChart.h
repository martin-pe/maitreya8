/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/VedicRasiChart.h
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

#ifndef VEDICRasiCHART_H_
#define VEDICRasiCHART_H_

#include "BasicVedicChart.h"

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
	virtual wxString getCenterString() = 0;

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
	
	void setupSouth();

	void setupNorth();

	void setupEast();

};

#endif

