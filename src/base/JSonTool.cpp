/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/JSonTool.cpp
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

#include "JSonTool.h"
#include "mathbase.h"
#include "ChartConfig.h"
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
**   JSonTool   ---   readPoint
**
******************************************************/
void JSonTool::readPoint( wxJSONValue &v, wxPoint &p )
{
	if ( ! v[ wxT( "x" ) ].IsNull()) p.x = v[ wxT( "x" ) ].AsInt();
	if ( ! v[ wxT( "y" ) ].IsNull()) p.y = v[ wxT( "y" ) ].AsInt();
}

/*****************************************************
**
**   JSonTool   ---   writePoint
**
******************************************************/
wxJSONValue JSonTool::writePoint( const wxPoint &p )
{
	wxJSONValue v;
	v[ wxT( "x" ) ] = p.x;
	v[ wxT( "y" ) ] = p.y;
	return v;
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
	for( std::set<ObjectId>::iterator iter = v.begin(); iter != v.end(); iter++ )
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
wxJSONValue JSonTool::writeIntVector( const std::vector<int> &v )
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
void JSonTool::readIntVector( wxJSONValue &j, std::vector<int> &v )
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
wxJSONValue JSonTool::writeStringVector( const std::vector<wxString> &v )
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
void JSonTool::readStringVector( wxJSONValue &j, std::vector<wxString> &v )
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
void JSonTool::readPatterns( wxJSONValue &v, std::list<GcChartFrameBgPattern> &l )
{
	for ( int i = 0; i < v.Size(); i++ )
	{
		wxJSONValue p = v[i];
		GcChartFrameBgPattern pattern;
		readChartFrame( p[ CFG_INNERFRAME ], pattern.innerFrame );
		readChartFrame( p[ CFG_OUTERFRAME ], pattern.outerFrame );
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
void JSonTool::readColorList( wxJSONValue&, std::vector<wxColour>& )
{
	assert( false );
}

/*****************************************************
**
**   JSonTool   ---   readFrameType
**
******************************************************/
void JSonTool::readFrameType( wxJSONValue &v, CHART_FRAME &frame )
{
	if ( v.IsNull()) return;
	wxString ftype = v.AsString();

	if ( ftype == CFG_CIRCLE ) frame = CF_CIRCLE;
	else if ( ftype == CFG_ELLIPSE ) frame = CF_ELLIPSE;
	else if ( ftype == CFG_SQUARE ) frame = CF_SQUARE;
	else if ( ftype == CFG_RECTANGLE ) frame = CF_RECTANGLE;
	else if ( ftype == CFG_TRIANGLE ) frame = CF_TRIANGLE;
	else wxLogError( wxString::Format( wxT( "Invalid frame type %s" ), ftype.c_str()));

	//printf( "JSonTool::readFrameType '%s' %d\n", str2char( ftype ), frame );
}

/*****************************************************
**
**   JSonTool   ---   readChartFrame
**
******************************************************/
void JSonTool::readChartFrame( wxJSONValue &v, GcChartFrame &frame )
{
	//printf( "JSonTool::readChartFrame frame type on entry is %d\n", frame.frameType );

	if ( ! v[ CFG_PEN ].IsNull()) readPen( v[ CFG_PEN ], frame.pen );
	if ( ! v[ CFG_BRUSH ].IsNull()) readBrush( v[ CFG_BRUSH ], frame.brush );
	if ( ! v[ CFG_RADIUS ].IsNull()) frame.radius = v[ CFG_RADIUS ].AsInt();
	if ( ! v[ CFG_SHOW ].IsNull()) frame.show = v[ CFG_SHOW ].AsBool();

	if ( ! v[ CFG_FRAME_TYPE ].IsNull()) readFrameType( v[ CFG_FRAME_TYPE ], frame.frameType );

	assert( frame.frameType >= CF_INHERITED && frame.frameType <= CF_TRIANGLE );
}

/*****************************************************
**
**   JSonTool   ---   readChartRing
**
******************************************************/
void JSonTool::readChartRing( wxJSONValue &v, GcChartRing &r )
{
	readChartFrame( v, r );
	if ( ! v[ CFG_WIDTH ].IsNull()) r.width = v[ CFG_WIDTH ].AsInt();
	if ( ! v[ CFG_WLABELS ].IsNull()) r.wLabels = v[ CFG_WLABELS ].AsInt();
	if ( ! v[ CFG_LABELSIZE ].IsNull()) r.labelSize = v[ CFG_LABELSIZE ].AsInt();
	if ( ! v[ CFG_LABELSTYLE ].IsNull()) r.labelStyle = v[ CFG_LABELSTYLE ].AsInt();
	if ( ! v[ CFG_ROTATELABELS ].IsNull()) r.rotateLabels = v[ CFG_ROTATELABELS ].AsInt();

	if ( ! v[ CFG_SHOWINNERFRAME ].IsNull()) r.showInnerFrame = v[ CFG_SHOWINNERFRAME ].AsBool();
	if ( ! v[ CFG_SHOWOUTERFRAME ].IsNull()) r.showOuterFrame = v[ CFG_SHOWOUTERFRAME ].AsBool();
	if ( ! v[ CFG_SHOWLABELS ].IsNull()) r.showLabels = v[ CFG_SHOWLABELS ].AsBool();

	if ( ! v[ CFG_PALETTE ].IsNull()) r.paletteName = v[ CFG_PALETTE ].AsString();
	if ( ! v[ CFG_GLOBALTEXTCOLOR ].IsNull() ) r.globalTextColor = wxColour( v[ CFG_GLOBALTEXTCOLOR ].AsString());
	//printf( "JSonTool::readChartRing %d\n", r.frameType );
}

/*****************************************************
**
**   JSonTool   ---   readObjectRing
**
******************************************************/
void JSonTool::readObjectRing( wxJSONValue &v, GcObjectRing &r )
{
	//printf( "JSonTool::readObjectRing frame type on entry is %d\n", r.frameType );
	readChartRing( v, r );
	if ( ! v[ CFG_WDEGREE1 ].IsNull()) r.wdegree1 = v[ CFG_WDEGREE1 ].AsInt();
	if ( ! v[ CFG_WDEGREE5 ].IsNull()) r.wdegree5 = v[ CFG_WDEGREE5 ].AsInt();
	if ( ! v[ CFG_WDEGREE10 ].IsNull()) r.wdegree10 = v[ CFG_WDEGREE10 ].AsInt();
	if ( ! v[ CFG_SHOW_1DEGREEFRAME ].IsNull()) r.show1DegreeFrame = v[ CFG_SHOW_1DEGREEFRAME ].AsBool();
	if ( ! v[ CFG_SHOW_5DEGREEFRAME ].IsNull()) r.show5DegreeFrame = v[ CFG_SHOW_5DEGREEFRAME ].AsBool();
	if ( ! v[ CFG_SHOW_10DEGREEFRAME ].IsNull()) r.show10DegreeFrame = v[ CFG_SHOW_10DEGREEFRAME ].AsBool();
	//printf( "JSonTool::readObjectRing frametype is %d\n", r.frameType );
}

/*****************************************************
**
**   JSonTool   ---   readInlayImage
**
******************************************************/
void  JSonTool::readInlayImage( wxJSONValue &v, GcInlayImage &img )
{
	readChartFrame( v, img );
	if ( ! v[ CFG_FILENAME ].IsNull()) img.filename = v[ CFG_FILENAME ].AsString();
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
**   JSonTool   ---   readRegion
**
******************************************************/
void JSonTool::readRegion( wxJSONValue &v, GcRegion &r )
{
	//printf( "JSonTool::readRegion\n" );
	if ( ! v[ CFG_PEN ].IsNull() ) readPen( v[ CFG_PEN ], r.pen );
	if ( ! v[ CFG_BRUSH ].IsNull() ) readBrush( v[ CFG_BRUSH ], r.brush );
	if ( ! v[ CFG_TEXTCOLOR ].IsNull() ) r.pen.SetColour( wxColour( v[ CFG_TEXTCOLOR ].AsString() ));
}

/*****************************************************
**
**   JSonTool   ---   readChartConfig
**
******************************************************/
void JSonTool::readChartConfig( wxJSONValue &v, ChartConfig *cfg )
{
	if ( ! v[ CFG_NAME ].IsNull() ) cfg->name = v[ CFG_NAME ].AsString();
	else cfg->name = wxT( "unknown" );

	if ( ! v[ CFG_PEN ].IsNull())
	{
		readPen( v[CFG_PEN ], cfg->pen );
	}
	if ( ! v[ CFG_BRUSH ].IsNull()) readBrush( v[ CFG_BRUSH ], cfg->brush );

	if ( ! v[ CFG_TEXTCOLOR ].IsNull() ) cfg->textColor = v[ CFG_TEXTCOLOR ].AsString();
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


