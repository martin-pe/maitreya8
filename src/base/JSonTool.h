/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/JSonTool.h
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

#define CFG_CONFIGS wxT( "configs" )
#define CFG_NAME wxT( "name" )
#define CFG_DESCRIPTION wxT( "description" )
#define CFG_EXTENDS wxT( "extends" )
#define CFG_SELECTED wxT( "selected" )
#define CFG_ITEMS wxT( "items" )
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

#define CFG_HOUSES wxT( "houses" )
#define CFG_ZODIACALSIGNS wxT( "zodiacalsigns" )
#define CFG_USESTOCKBRUSHLIST wxT( "usestockbrushlist" )
#define CFG_ZODIAC wxT( "zodiac" )
#define CFG_ZODIAC_1_DEGREE wxT( "zodiac1degree" )
#define CFG_ZODIAC_5_DEGREE wxT( "zodiac5degree" )
#define CFG_ZODIAC_10_DEGREE wxT( "zodiac10degree" )
#define CFG_ZODIAC_SYMBOLS wxT( "zodiacsymbols" )
#define CFG_OUTER_ZODIAC wxT( "outerzodiac" )
#define CFG_INNER_HOUSE wxT( "innerhouse" )
#define CFG_OUTER_HOUSE wxT( "outerhouse" )
#define CFG_HOUSE_NUMBERS wxT( "housenumbers" )
#define CFG_PLANETS wxT( "planets" )
#define CFG_ASPECTS wxT( "aspects" )
#define CFG_INNER_TRANSIT_ZODIAC wxT( "innertransitzodiac" )
#define CFG_INNER_TRANSIT_ZODIAC_1_DEGREE wxT( "innertransitzodiac1degree" )
#define CFG_INNER_TRANSIT_ZODIAC_5_DEGREE wxT( "innertransitzodiac5degree" )
#define CFG_INNER_TRANSIT_ZODIAC_10_DEGREE wxT( "innertransitzodiac10degree" )
#define CFG_INNER_TRANSIT_PLANETS wxT( "innertransitplanets" )
#define CFG_OUTER_TRANSIT_ZODIAC wxT( "outertransitzodiac" )
#define CFG_OUTER_TRANSIT_ZODIAC_1_DEGREE wxT( "outertransitzodiac1degree" )
#define CFG_OUTER_TRANSIT_ZODIAC_5_DEGREE wxT( "outertransitzodiac5degree" )
#define CFG_OUTER_TRANSIT_ZODIAC_10_DEGREE wxT( "outertransitzodiac10degree" )
#define CFG_OUTER_TRANSIT_PLANETS wxT( "outertransitplanets" )
//#define CFG_WIDGET_BACKGROUND wxT( "widgetbackground" )

#define CFG_RADIUS wxT( "radius" )
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
#define CFG_SYMBOL_ZOOM_FACTOR wxT( "symbolzoomfactor" )
#define CFG_ROTATE_SYMBOLS wxT( "rotatesymbols" )
#define CFG_SYMBOL_TYPE wxT( "symboltype" )

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


// TODO
/*
#define CFG_OUTER_ANGLE_MARKER wxT( "outeranglemarker" )
#define CFG_INNER_ANGLE_MARKER wxT( "inneranglemarker" )
*/

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

	wxJSONValue writeIntVector( const vector<int>& );
	void readIntVector( wxJSONValue&, vector<int>& );

	wxJSONValue writeStringVector( const vector<wxString>& );
	void readStringVector( wxJSONValue&, vector<wxString>& );

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

	void readColorList( wxJSONValue&, vector<wxColour>& );

	CHART_FRAME readFrameType( wxString ftype );

	void readChartFrame( wxJSONValue&, GcChartFrame& );

	void readRectangle( wxJSONValue&, GcRectangle& );

	void readArrow( wxJSONValue&, GcArrow& );

	void readRegion( wxJSONValue&, GcRegion& );

	void readPatterns( wxJSONValue&, list<GcChartFrameBgPattern>& );

	void readZodiacalSigns( wxJSONValue&, GcZodiacalSigns& );

	void readHouses( wxJSONValue&, GcHouses& );

private:

	void reportWrongValue( wxJSONValue&, wxString message );
	wxString filename;

};


#endif


