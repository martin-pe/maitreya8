/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicVedicChart.h
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

#ifndef _BASICVEDICCHART_H_
#define _BASICVEDICCHART_H_

#include <wx/dynarray.h>
#include <wx/string.h>
#include <vector>

#include "ChartField.h"
#include "GraphicalChart.h"

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
	std::vector<ChartField> fields;

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
	void drawGraphicItemLine( MRect rect, const std::vector<ChartGraphicItem> g, const int &align, const FIELD_PART& );

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

