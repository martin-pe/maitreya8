/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ChartField.h
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

#ifndef _CHARTFIELD_H_
#define _CHARTFIELD_H_

#include <wx/dynarray.h>
#include <wx/string.h>
#include <vector>

#include "maitreya.h"
#include "Painter.h"

class ChartProperties;
class Painter;
class VedicChartConfig;

/*************************************************//**
*
* \brief Holds the text items for a field of a chart
*
******************************************************/
class ChartTextItem
{
public:
	ChartTextItem( const wxString lname, const wxString sname, const bool r = false )
	{
		longname = lname;
		shortname = sname;
		retro = r;
	}
	wxString shortname, longname;
	bool retro;
};

/*************************************************//**
*
* \brief Holds graphical items for a field of a chart
*
******************************************************/
class ChartGraphicItem
{
public:
	ChartGraphicItem( const wxString n, const int &index, const bool r = false ) {
		name = n;
		pindex = index;
		retro = r;
	}
	ChartGraphicItem( const ChartGraphicItem &a ) {
		name = a.name;
		pindex = a.pindex;
		retro = a.retro;
	}
	wxString name;
	int pindex;
	bool retro;
};

/*************************************************//**
*
* \brief Contains the graphical items and text items for one field (1..12 or 1..28) of a chart.
* Instances will associates twice (per chart), one for radix, the other for transit items.
*
******************************************************/
class ChartContents
{
public:
	std::vector<ChartTextItem> textitems;
	std::vector<ChartGraphicItem> graphicitems;
	ObjectArray planets;
	void clear();
};

/*************************************************//**
*
* \brief fields are interpreted as parts of rectangles.
* the type is either full (fulle rectangle) or an triangle.
* Triangle types are named according to the corner determinig the triangle.
*
******************************************************/
enum FIELD_TYPE {
	FIELD_TYPE_FULL = 0, // all fields in south indian chart, sbc, fixed rasis in east indian chart
	FIELD_TYPE_NE, // east: taurus and sag
	FIELD_TYPE_SW, // east: gemini and scorpio
	FIELD_TYPE_NW, // east: leo and pisces
	FIELD_TYPE_SE, // east: virgo and aquarius

	FIELD_TYPE_N, // north: houses 2 and 12
	FIELD_TYPE_W, // north: houses 3 and 5
	FIELD_TYPE_S, // north: houses 6 and 8
	FIELD_TYPE_E, // north: houses 9 and 11

	FIELD_TYPE_DIAMOND // north: houses 1, 4, 7, 10
};

// Chart field status
#define CHART_FIELD_STATUS_MARKUP      0x001
#define CHART_FIELD_STATUS_ZOOM        0x002
#define CHART_FIELD_STATUS_ASPECTING   0x004
#define CHART_FIELD_STATUS_ASPECTED    0x008
#define CHART_FIELD_STATUS_POS_FULL    0x010
#define CHART_FIELD_STATUS_POS_INNER   0x020
#define CHART_FIELD_STATUS_POS_OUTER   0x040
#define CHART_FIELD_STATUS_ALL_ASPECTS 0x080

/*************************************************//**
*
* \brief describes the status of a chart field in graphical chart (varga or SBC).
* This class contains the field geometric, alignement of items and the contents in radix vs. transit mode.
*
******************************************************/
class ChartField
{
public:

	/**
	 * \brief creates a field according to rectangle geometry, field type and chart center position (graviation)
	 */
	ChartField( const MRect&, const GRAVITATION&, const FIELD_TYPE = FIELD_TYPE_FULL );

	/**
	 * \brief gets the field contents (planets and text items) according to the mode (rasix/transit)
	 */
	ChartContents &getContents( const int &hid );

	/**
	 * \brief resets all field values, clears the associated lists
	 */
	void clear();

	/**
	 * \brief calculates the text alignment for ths field in radix/transit mode.
	 The align depends on the geometric form and the position of the chart center (graviation)
	 */
	void calculateAlign();

	/**
	 * \brief center of the rect for arrow drawing. This may be distinct from the center of the rectangle,
	 * for triangular field geometric
	 */
	MPoint getCenter();

	bool isTriangle();

	/**
	 * \brief calculates the rectangle for text output in Rasi/transit mode, according to field type
	 */
	MRect calculateModifiedRect( const FIELD_PART&, const MPoint& );

	/**
	 * \brief underlying rectangle. Some field types (triangle, diamond) exlcude some parts of the rectangle
	 */
	MRect rect;

	/**
	 * \brief geometric type of the field (rectangle, diamond, triangle)
	 */
	FIELD_TYPE fieldtype;

	/**
	 * \brief text alignemnt for output. This value is calculated depending upon the geometric type
	 */
	int align;

	/**
	 * \brief direction of the chart center
	 */
	GRAVITATION gravitation;

	/**
	 * \brief text contents and planet indices in radix/transit mode
	 */
	ChartContents contents, tcontents;
};

#endif

