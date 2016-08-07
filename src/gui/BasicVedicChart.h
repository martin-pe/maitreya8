/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicVedicChart.h
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

#ifndef _BASICVEDICCHART_H_
#define _BASICVEDICCHART_H_

#include <wx/dynarray.h>
#include <wx/string.h>
#include <vector>

#include "ChartField.h"
#include "GraphicalChart.h"

using namespace std;

class ChartProperties;
class Painter;
class VedicChartConfig;


/*************************************************//**
*
* \brief Basic class for all Vedic charts including Rasi and Sarvatobhadra
*
*
* This class provides basic functionality for Vedic charts:
* - holding contents for Rasi/Nakshatra fields
* - drawing methods for graphical items and text items
* - styles for Vedic charts
* - Painter reference
* - Paint method, calling abstract method paintChart
*
******************************************************/
class BasicVedicChart : public GraphicalChart
{
public:

	BasicVedicChart( const ChartType&, ChartProperties*, const uint &field_count,
		Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	/**
	 * \brief calculates of the given point is within one of the fields (Aries .. Pisces or Aswini ... Revati) of the chart.
	 * returns -1 if the positiom is outside of a field.
	 */
	uint getFieldForScreenPos( const MPoint& );

	/**
	 * \brief calls the event handling of mouse handling (markup, aspect display)
	 */
	virtual bool OnResponsiveChartEvent( const MPoint p = MPoint( -1, -1 ), const int keyMod = 0 );

protected:

	/**
	 * \brief basic setup. This method calls setupChart containing the specific setup instructions
	 */
	virtual void setup();

	VedicChartConfig *getVChartConfig() const;

	virtual void finishChart() = 0;

	virtual void setupChart() = 0;

	virtual ChartConfig *getChartConfig() const;

	/**
	 * \brief called by generic paint method in GraphicalChart.
	 */
	virtual void paintInternal( ChartConfig* );

	void setAspectPainterStyle( const int &i, const bool isBenefic );

	void paintAspectsForFieldPair( const int &f1, const int &f2 );

	/**
	 * \brief 12 or 28 supported. Must be set in the constructor.
	 */
	const uint field_count;

	/**
	 * \brief max widget extension
	 */
	double xmax, ymax;

	/**
	 * \brief radius from center to end of the chart (not end of widget)
	 */
	double xr, yr;

	/**
	 * \brief core information about the chart fields, their geometry and contents
	 */
	vector<ChartField> fields;

	/**
	 * \brief Write astrological objects into contents arrays
	 */
	virtual void writeChartContents( const int &chart_id, const bool applyFilter = false );

	virtual int getAshtakavargaPoints( const ObjectId&, const int& ) { return -1; };


	/**
	*  \brief Abstract method. Is overwritten by Sarvatobhadra chart (28 fields) and Vedic Rasi chart (12 fields in South/East/North Indian style)
	*/
	virtual void drawGrid() = 0;

	/**
	*  \brief Returns the field index (1..12 or 1..28) for a given planet in radix or transit mode. Must be overwritten.
	*/
	virtual int getPlanetField( const ObjectId&, const int &chart_id ) = 0;

	void drawFieldText( const uint &i, const FIELD_PART& );

	/**
	 * \brief Draw a single graphical item
	 */
	void drawSingleGraphicItem( const MRect &rect, const ChartGraphicItem &g, const FIELD_PART& );

	/**
	 * \brief Draw one line of text in the given rectangle
	 */
	void drawTextItemLine( MRect rect, const wxString &s, const int &align, const FIELD_PART& );

	/**
	 * \brief Draw one line of graphical items in the given rectangle
	 */
	void drawGraphicItemLine( MRect rect, const vector<ChartGraphicItem> g, const int &align, const FIELD_PART& );

	/**
	 * \brief Draw text in the given rectangle. Adds support for text align.
	 */
	void drawTextFormatted( const MRect &rect, const wxString t, const int& align );

	void paintOuterRectangle();

	/**
	 * \brief paint the field including bachkground, markup and aspects (depends upon config and markup status)
	 */
	void paintField( const int &i );

	bool isFieldZoomed( const uint& );

	virtual bool isChartFieldMarked( const int& ) { return false; }

	/**
	 * \brief offset of the first field in the list relative to the first astrological field (Aries or Aswini).
	 * Only used in North Indian chart where ascendant is in the first field instead of Aries.
	 */
	int positionOffset;

	MRect transitrect;

private:

};

#endif

