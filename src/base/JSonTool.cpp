/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/JSonTool.cpp
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

#include "JSonTool.h"
#include "mathbase.h"
#include "ColorConfig.h"

#include <wx/gdicmn.h>
#include <wx/jsonval.h>
#include <wx/log.h>
#include <wx/string.h>

/*****************************************************
**
**   JSonTool   ---   Constructor
**
******************************************************/
JSonTool::JSonTool( wxString filename )
 : filename( filename )
{
}

/*****************************************************
**
**   JSonTool   ---   writeSize
**
******************************************************/
wxJSONValue JSonTool::writeSize( const wxSize &size )
{
	wxJSONValue v;
	v[ wxT( "x" ) ] = size.x;
	v[ wxT( "y" ) ] = size.y;
	return v;
}

/*****************************************************
**
**   JSonTool   ---   readSize
**
******************************************************/
void JSonTool::readSize( wxJSONValue &v, wxSize &size )
{
	if ( ! v[ wxT( "x" ) ].IsNull()) size.x = v[ wxT( "x" ) ].AsInt();
	if ( ! v[ wxT( "y" ) ].IsNull()) size.y = v[ wxT( "y" ) ].AsInt();
}

/*****************************************************
**
**   JSonTool   ---   writeObjectArray
**
******************************************************/
wxJSONValue JSonTool::writeObjectArray( const ObjectArray &v )
{
	wxJSONValue j;
	for( uint i = 0; i < v.size(); i++ )
	{
		j.Append( wxJSONValue( v[i] ));
	}
	return j;
}

/*****************************************************
**
**   JSonTool   ---   readObjectArray
**
******************************************************/
void JSonTool::readObjectArray( wxJSONValue &j, ObjectArray &v )
{
	if ( j.IsArray())
	{
		v.clear();
		for ( int i = 0; i < j.Size(); i++ )
		{
			v.push_back( (ObjectId)j[i].AsInt() );
		}
	}
	else wxLogError( wxT( "JSON value is not an array" ));
}

/*****************************************************
**
**   JSonTool   ---   writeObjectFilter
**
******************************************************/
wxJSONValue JSonTool::writeObjectFilter( const ObjectFilter &v )
{
	wxJSONValue j;
	for( set<ObjectId>::iterator iter = v.begin(); iter != v.end(); iter++ )
	{
		j.Append( wxJSONValue( *iter ));
	}
	return j;
}

/*****************************************************
**
**   JSonTool   ---   readObjectFilter
**
******************************************************/
void JSonTool::readObjectFilter( wxJSONValue &j, ObjectFilter &v )
{
	if ( j.IsArray())
	{
		v.clear();
		for ( int i = 0; i < j.Size(); i++ )
		{
			v.insert( (ObjectId)j[i].AsInt() );
		}
	}
	else wxLogError( wxT( "JSON value is not an array" ));
}

/*****************************************************
**
**   JSonTool   ---   writeIntVector
**
******************************************************/
wxJSONValue JSonTool::writeIntVector( const vector<int> &v )
{
	wxJSONValue j;
	for( uint i = 0; i < v.size(); i++ )
	{
		j.Append( wxJSONValue( v[i] ));
	}
	return j;
}

/*****************************************************
**
**   JSonTool   ---   readIntVector
**
******************************************************/
void JSonTool::readIntVector( wxJSONValue &j, vector<int> &v )
{
	if ( j.IsArray())
	{
		v.clear();
		for ( int i = 0; i < j.Size(); i++ )
		{
			v.push_back( j[i].AsInt() );
		}
	}
	else wxLogError( wxT( "JSON value is not an array" ));
}

/*****************************************************
**
**   JSonTool   ---   writeStringVector
**
******************************************************/
wxJSONValue JSonTool::writeStringVector( const vector<wxString> &v )
{
	wxJSONValue j;
	for( uint i = 0; i < v.size(); i++ )
	{
		j.Append( wxJSONValue( v[i] ));
	}
	return j;
}

/*****************************************************
**
**   JSonTool   ---   readStringVector
**
******************************************************/
void JSonTool::readStringVector( wxJSONValue &j, vector<wxString> &v )
{
	if ( j.IsArray())
	{
		v.clear();
		for ( int i = 0; i < j.Size(); i++ )
		{
			v.push_back( j[i].AsString() );
		}
	}
	else wxLogError( wxT( "JSON value is not an array" ));
}

/*****************************************************
**
**   JSonTool   ---   writePen
**
******************************************************/
wxJSONValue JSonTool::writePen( const wxPen &pen )
{
	wxJSONValue v;
	v[ CFG_COLOR ] = pen.GetColour().GetAsString( wxC2S_HTML_SYNTAX );
	v[ CFG_WIDTH ] = pen.GetWidth();
	v[ CFG_STYLE ] = convertWxPenStyle2String( pen.GetStyle() );
	return v;
}

/*****************************************************
**
**   JSonTool   ---   readPen
**
******************************************************/
void JSonTool::readPen( wxJSONValue &v, wxPen &pen )
{
	// TODO der default hat hier nichts zu suchen
	pen.SetColour( *wxBLACK );
	pen.SetStyle( wxSOLID );
	pen.SetWidth( 1 );

	if ( ! v[ CFG_COLOR ].IsNull()) pen.SetColour( wxColour( v[ CFG_COLOR ].AsString() ));
	if ( ! v[ CFG_WIDTH ].IsNull()) pen.SetWidth( v[ CFG_WIDTH ].AsInt() );
	if ( ! v[ CFG_STYLE ].IsNull())
	{
		wxString s = v[ CFG_STYLE ].AsString();
		if ( s.IsNumber())
		{
			pen.SetStyle( v[ CFG_STYLE ].AsInt() );
		}
		else
		{
			pen.SetStyle( convertString2WxPenStyle( v[ CFG_STYLE ].AsString()));
		}
		//pen.SetStyle( v[ CFG_STYLE ].AsInt() );
	}
}

/*****************************************************
**
**   JSonTool   ---   writeBrush
**
******************************************************/
wxJSONValue JSonTool::writeBrush( const MBrush &brush )
{
	wxJSONValue v;
	v[ CFG_STYLE ] = convertMBrushStyle2String( brush.style );
	if ( brush.style == wxSTIPPLE )
	{
		v[ CFG_ROTATEHUE ] = brush.rotateHue;
		if ( brush.filename.IsEmpty()) wxLogError( wxT( "cannot save stipple brush without filename" ));
		else v[ CFG_FILENAME ] = brush.filename;
	}
	else if ( brush.style != wxTRANSPARENT )
	{
		v[ CFG_COLOR ] = brush.color.GetAsString( wxC2S_HTML_SYNTAX );
	}

	return v;
}

/*****************************************************
**
**   JSonTool   ---   readBrush
**
******************************************************/
void JSonTool::readBrush( wxJSONValue &v, MBrush &brush )
{
	// 1st step: read style. defaults to wxSOLID (that's okay). can be number or text representation
	if ( ! v[ CFG_STYLE ].IsNull())
	{
		brush.style = convertString2MBrushStyle( v[ CFG_STYLE ].AsString());
	}
	else brush.style = wxSOLID;

	if ( brush.style == wxSTIPPLE )
	{
		if ( ! v[ CFG_FILENAME ].IsNull())
		{
			brush.filename = v[ CFG_FILENAME ].AsString();
			if ( ! v[ CFG_ROTATEHUE ].IsNull() ) brush.rotateHue = v[ CFG_ROTATEHUE ].AsInt();
		}
		else
		{
			reportWrongValue( v[ CFG_FILENAME ], wxT( "filename must not be empty for brushes with stipple style" ));
			brush = MBrush( *wxRED );
		}
	}
	else if ( brush.style != wxTRANSPARENT )
	{
		if ( ! v[ CFG_COLOR ].IsNull())
		{
			brush.color = v[ CFG_COLOR ].AsString();
		}
		else
		{
			wxLogError( wxT( "JSonTool::readBrush falling back to red brush (%s)" ), brush.toString().c_str());
			brush = MBrush( *wxRED );
		}
	}
}

/*****************************************************
**
**   JSonTool   ---   readPatterns
**
******************************************************/
void JSonTool::readPatterns( wxJSONValue &v, list<GcChartFrameBgPattern> &l )
{
// TODO array iterieren und elemente setzen
	for ( int i = 0; i < v.Size(); i++ )
	{
		wxJSONValue p = v[i];
		GcChartFrameBgPattern pattern;
		if ( ! p[ CFG_INNERFRAME ].IsNull()) pattern.innerFrameName = p[ CFG_INNERFRAME ].AsString();
		if ( ! p[ CFG_OUTERFRAME ].IsNull()) pattern.outerFrameName = p[ CFG_OUTERFRAME ].AsString();
		if ( ! p[ CFG_PALETTE ].IsNull()) pattern.paletteName = p[ CFG_PALETTE ].AsString();
		if ( ! p[ CFG_ALIGNTOHOUSES ].IsNull()) pattern.alignToHouses = p[ CFG_ALIGNTOHOUSES ].AsBool();
		l.push_back( pattern );
	}
}

/*****************************************************
**
**   JSonTool   ---   readPalette
**
******************************************************/
void JSonTool::readPalette( wxJSONValue &v, BgPalette &palette )
{
	palette.brushes.clear();
	for ( int i = 0; i < v.Size(); i++ )
	{
		wxJSONValue b = v[i];
		//printf( "JSonTool::readBrushList %d\n", i );
		MBrush brush; //( *wxRED, wxSOLID );
		readBrush( v[i], brush );
		palette.brushes.push_back( brush );
	}
}

/*****************************************************
**
**   JSonTool   ---   writePalette
**
******************************************************/
wxJSONValue JSonTool::writePalette( BgPalette &palette )
{
	wxJSONValue jv;
	//printf( "SIZE %d\n", palette.brushes.size());

	for( uint i = 0; i < palette.brushes.size(); i++ )
	{
		jv.Append( writeBrush( palette.brushes[i] ));
	}
	return jv;
}

/*****************************************************
**
**   JSonTool   ---   readColorList
**
******************************************************/
void JSonTool::readColorList( wxJSONValue&, vector<wxColour>& )
{
	assert( false );
}

/*****************************************************
**
**   JSonTool   ---   readFrameType
**
******************************************************/
CHART_FRAME JSonTool::readFrameType( wxString ftype )
{
	CHART_FRAME frameType = CF_CIRCLE;

	if ( ftype == CFG_CIRCLE ) {}
	else if ( ftype == CFG_ELLIPSE ) frameType = CF_ELLIPSE;
	else if ( ftype == CFG_SQUARE ) frameType = CF_SQUARE;
	else if ( ftype == CFG_RECTANGLE ) frameType = CF_RECTANGLE;
	else if ( ftype == CFG_TRIANGLE ) frameType = CF_TRIANGLE;
	else wxLogError( wxString::Format( wxT( "Invalid frame type %s" ), ftype.c_str()));

	return frameType;
}

/*****************************************************
**
**   JSonTool   ---   readChartFrame
**
******************************************************/
void JSonTool::readChartFrame( wxJSONValue &v, GcChartFrame &frame )
{
	//printf( "JSonTool::readChartFrame\n" );

	if ( ! v[ CFG_PEN ].IsNull()) readPen( v[ CFG_PEN ], frame.pen );
	if ( ! v[ CFG_BRUSH ].IsNull()) readBrush( v[ CFG_BRUSH ], frame.brush );
	if ( ! v[ CFG_RADIUS ].IsNull()) frame.radius = v[ CFG_RADIUS ].AsInt();
	if ( ! v[ CFG_SHOW ].IsNull()) frame.show = v[ CFG_SHOW ].AsBool();

	if ( ! v[ CFG_FRAME_TYPE ].IsNull()) frame.frameType = readFrameType( v[ CFG_FRAME_TYPE ].AsString() );
}

/*****************************************************
**
**   JSonTool   ---   readRectangle
**
******************************************************/
void  JSonTool::readRectangle( wxJSONValue &v, GcRectangle &rect )
{
	//printf( "JSonTool::readRectangle\n" );

	if ( ! v[ CFG_PEN ].IsNull()) readPen( v[ CFG_PEN ], rect.pen );
	if ( ! v[ CFG_BRUSH ].IsNull()) readBrush( v[ CFG_BRUSH ], rect.brush );
	if ( ! v[ CFG_DOUBLE_OUTER_LINE ].IsNull()) rect.doubleOuterLine = v[ CFG_DOUBLE_OUTER_LINE ].AsBool();
	if ( ! v[ CFG_RADIUS ].IsNull()) rect.radius = v[ CFG_RADIUS ].AsInt();
	if ( ! v[ CFG_CORNERRADIUS ].IsNull()) rect.cornerRadius = v[ CFG_CORNERRADIUS ].AsInt();
	if ( ! v[ CFG_SHOW ].IsNull()) rect.show = v[ CFG_SHOW ].AsBool();
}

/*****************************************************
**
**   JSonTool   ---   readArrow
**
******************************************************/
void  JSonTool::readArrow( wxJSONValue&, GcArrow& )
{
	assert( false );
}

/*****************************************************
**
**   JSonTool   ---   readRegion
**
******************************************************/
void  JSonTool::readRegion( wxJSONValue &v, GcRegion &r )
{
	//printf( "JSonTool::readRegion\n" );
	if ( ! v[ CFG_PEN ].IsNull() ) readPen( v[ CFG_PEN ], r.pen );
	if ( ! v[ CFG_BRUSH ].IsNull() ) readBrush( v[ CFG_BRUSH ], r.brush );
	if ( ! v[ CFG_TEXTCOLOR ].IsNull() ) r.pen.SetColour( wxColour( v[ CFG_TEXTCOLOR ].AsString() ));
}

/*****************************************************
**
**   JSonTool   ---   readZodiacalSigns
**
******************************************************/
void  JSonTool::readZodiacalSigns( wxJSONValue &v, GcZodiacalSigns &signs )
{
	//printf( "JSonTool::readZodiacalSigns\n" );
	if ( ! v[ CFG_PALETTE ].IsNull()) signs.paletteName = v[ CFG_PALETTE ].AsString();
	if ( ! v[ CFG_GLOBALTEXTCOLOR ].IsNull() ) signs.globalTextColor = wxColour( v[ CFG_GLOBALTEXTCOLOR ].AsString());

	if ( ! v[ CFG_SYMBOL_ZOOM_FACTOR ].IsNull()) signs.symbolZoomFactor = v[ CFG_SYMBOL_ZOOM_FACTOR ].AsInt();
	if ( ! v[ CFG_ROTATE_SYMBOLS ].IsNull()) signs.rotateSymbols = v[ CFG_ROTATE_SYMBOLS ].AsBool();
	if ( ! v[ CFG_SYMBOL_TYPE ].IsNull()) signs.symbolType = v[ CFG_SYMBOL_TYPE ].AsInt();
}

/*****************************************************
**
**   JSonTool   ---   readHouses
**
******************************************************/
void  JSonTool::readHouses( wxJSONValue &v, GcHouses &h )
{
	//printf( "JSonTool::readHouses\n" );
	if ( ! v[ CFG_PALETTE ].IsNull()) h.paletteName = v[ CFG_PALETTE ].AsString();
	if ( ! v[ CFG_GLOBALTEXTCOLOR ].IsNull() ) h.globalTextColor = wxColour( v[ CFG_GLOBALTEXTCOLOR ].AsString());
}

/*****************************************************
**
**   JSonTool   ---   reportWrongValue
**
******************************************************/
void  JSonTool::reportWrongValue( wxJSONValue &v, wxString s )
{
	wxString message = s;
	message << wxT( " (" );
	if ( ! filename.IsEmpty()) message << filename << wxT( ", " );
	message <<  wxT( "line " ) <<  v.GetLineNo() << wxT( ")" );
	wxLogError( message );
}


