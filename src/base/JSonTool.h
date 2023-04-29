/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/JSonTool.h
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

#ifndef _JSONTOOL_H_
#define _JSONTOOL_H_

#include "GraphConfig.h"
#include "maitreya.h"

#include <wx/brush.h>
#include <wx/pen.h>

#include<list>

class wxJSONValue;
class wxSize;
class BgPalette;
class ChartConfig;

#define CFG_CONFIGS wxT( "configs" )
#define CFG_NAME wxT( "name" )
#define CFG_DESCRIPTION wxT( "description" )
#define CFG_EXTENDS wxT( "extends" )
#define CFG_SELECTED wxT( "selected" )
#define CFG_ITEMS wxT( "items" )
#define CFG_PAGES wxT( "pages" )
#define CFG_STYLE wxT( "style" )
#define CFG_ROTATEHUE wxT( "rotatehue" )
#define CFG_MODE wxT( "mode" )
#define CFG_WIDTH wxT( "width" )
#define CFG_COLOR wxT( "color" )
#define CFG_FILENAME wxT( "filename" )
#define CFG_BRUSH wxT( "brush" )
#define CFG_BRUSHES wxT( "brushes" )
#define CFG_PEN wxT( "pen" )
#define CFG_PENS wxT( "pens" )
#define CFG_PAGEFRAME wxT( "pageframe" )
#define CFG_IMAGE wxT( "image" )
#define CFG_ZODIACSTART wxT( "zodiacstart" )

#define CFG_HOUSES wxT( "houses" )
#define CFG_SIGNS wxT( "signs" )
#define CFG_NAKSHATRAS wxT( "nakshatras" )
#define CFG_USESTOCKBRUSHLIST wxT( "usestockbrushlist" )

#define CFG_RPLANETS wxT( "rplanets" )
#define CFG_RINNERTRANSITPLANETS wxT( "rinnertransitplanets" )
#define CFG_ROUTERTRANSITPLANETS wxT( "routertransitplanets" )
#define CFG_ASPECTS wxT( "aspects" )
#define CFG_ANGLES wxT( "angles" )

#define CFG_SHOWINNERFRAME wxT( "showinnerframe" )
#define CFG_SHOWOUTERFRAME wxT( "showouterframe" )
#define CFG_SHOWLABELS wxT( "showlabels" )
#define CFG_EASTINDIANCENTER wxT( "eastindiancenter" )
#define CFG_SHOW_1DEGREEFRAME wxT( "show1degreeframe" )
#define CFG_SHOW_5DEGREEFRAME wxT( "show5degreeframe" )
#define CFG_SHOW_10DEGREEFRAME wxT( "show10degreeframe" )
#define CFG_WDEGREE1 wxT( "wdegree1" )
#define CFG_WDEGREE5 wxT( "wdegree1" )
#define CFG_WDEGREE10 wxT( "wdegree1" )

#define CFG_RADIUS wxT( "radius" )
#define CFG_WLABELS wxT( "wlabels" )
#define CFG_LABELSTYLE wxT( "labelstyle" )
#define CFG_LABELSIZE wxT( "labelsize" )
#define CFG_ROTATELABELS wxT( "rotatelabels" )
#define CFG_CORNERRADIUS wxT( "cornerradius" )
#define CFG_SHOW wxT( "show" )
#define CFG_FRAME_TYPE wxT( "frametype" )
#define CFG_CIRCLE wxT( "circle" )
#define CFG_ELLIPSE wxT( "ellipse" )
#define CFG_SQUARE wxT( "square" )
#define CFG_RECTANGLE wxT( "rectangle" )
#define CFG_TRIANGLE wxT( "triangle" )
#define CFG_OUTER_RECTANGLE wxT( "outerrectangle" )
#define CFG_DOUBLE_OUTER_LINE wxT( "doubleouterline" )
#define CFG_MARKED_FIELDS wxT( "markedfields" )
#define CFG_TEXTCOLOR wxT( "textcolor" )
#define CFG_GLOBALTEXTCOLOR wxT( "globaltextcolor" )
#define CFG_RELWIDTH wxT( "relwidth" )
#define CFG_RATIO wxT( "ratio" )

#define CFG_PALETTE wxT( "palette" )
#define CFG_PATTERNS wxT( "patterns" )
#define CFG_INNERFRAME wxT( "innerFrame" )
#define CFG_OUTERFRAME wxT( "outerFrame" )
#define CFG_ALIGNTOHOUSES wxT( "aligntohouses" )

// YOGA/DASA
#define CFG_GROUP wxT( "group" )
#define CFG_SOURCE wxT( "source" )
#define CFG_EFFECT wxT( "effect" )
#define CFG_RULE wxT( "rule" )
#define CFG_HIGHER_VARGAS wxT( "highervargas" )
#define CFG_DASA wxT( "dasa" )
#define CFG_LORD wxT( "lord" )
#define CFG_SUBLORD wxT( "sublord" )

// multiple view
#define CFG_VIEWTYPE wxT( "viewtype" )
#define CFG_VEDIC wxT( "vedic" )
#define CFG_PAGES wxT( "pages" )
#define CFG_NOTEBOOK wxT( "notebook" )
#define CFG_WIDGET wxT( "widget" )
#define CFG_SPLITTER wxT( "splitter" )
#define CFG_WINDOW1 wxT( "window1" )
#define CFG_WINDOW2 wxT( "window2" )
#define CFG_SPLITDIRECTION wxT( "splitdirection" )
#define CFG_SASHPOS wxT( "sashpos" )
#define CFG_SASHPOSPERCENT wxT( "sashpospercent" )
#define CFG_TEXTMODE wxT( "textmode" )
#define CFG_SHOWTOOLBAR wxT( "showtoolbar" )
#define CFG_ROWCOUNT wxT( "rowcount" )
#define CFG_COLCOUNT wxT( "colcount" )
#define CFG_VARGA wxT( "varga" )
#define CFG_VARGAS wxT( "vargas" )

#define CFG_PLANET_MARKER_LENGTH wxT( "planetmarkerlength" )


/*************************************************//**
*
* 
*
******************************************************/
class JSonTool
{
public:

	JSonTool( wxString filename = wxEmptyString );

	wxJSONValue writeSize( const wxSize& );
	void readSize( wxJSONValue&, wxSize& );

	wxJSONValue writePoint( const wxPoint& );
	void readPoint( wxJSONValue&, wxPoint& );

	wxJSONValue writeIntVector( const std::vector<int>& );
	void readIntVector( wxJSONValue&, std::vector<int>& );

	wxJSONValue writeStringVector( const std::vector<wxString>& );
	void readStringVector( wxJSONValue&, std::vector<wxString>& );

	wxJSONValue writeObjectArray( const ObjectArray& );
	void readObjectArray( wxJSONValue&, ObjectArray& );

	wxJSONValue writeObjectFilter( const ObjectFilter& );
	void readObjectFilter( wxJSONValue&, ObjectFilter& );

	wxJSONValue writePen( const wxPen& );
	void readPen( wxJSONValue&, wxPen& );

	wxJSONValue writeBrush( const MBrush& );
	void readBrush( wxJSONValue&, MBrush& );

	void readPalette( wxJSONValue&, BgPalette& );
	wxJSONValue writePalette( BgPalette& );

	void readColorList( wxJSONValue&, std::vector<wxColour>& );

	void readFrameType( wxJSONValue&, CHART_FRAME& );

	void readChartFrame( wxJSONValue&, GcChartFrame& );

	void readChartRing( wxJSONValue&, GcChartRing& );

	void readObjectRing( wxJSONValue&, GcObjectRing& );

	void readRectangle( wxJSONValue&, GcRectangle& );

	void readRegion( wxJSONValue&, GcRegion& );

	void readPatterns( wxJSONValue&, std::list<GcChartFrameBgPattern>& );

	void readChartConfig( wxJSONValue&, ChartConfig* );

	void readInlayImage( wxJSONValue&, GcInlayImage& );

private:

	void reportWrongValue( wxJSONValue&, wxString message );
	wxString filename;

};


#endif


