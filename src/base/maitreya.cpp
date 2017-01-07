/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/maitreya.cpp
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

#include "maitreya.h"
#include "mathbase.h"
#include "config.h"
#include "ImageProvider.h"

#include <wx/brush.h>
#include <wx/gdicmn.h>
#include <wx/image.h>
#include <wx/log.h>

/*****************************************************
**
**   pointInRect
**
******************************************************/
bool pointInRect( const MPoint &p, const MRect &r )
{
	if ( r.x <= p.real() && p.real() <= r.x + r.width && r.y <= p.imag() && p.imag() <= r.y + r.height ) return true;
	else return false;
}

/*****************************************************
**
**   pointInRect
**
******************************************************/
bool pointInRect( const wxPoint &p, const MRect &r )
{
	if ( r.x <= p.x && p.x <= r.x + r.width && r.y <= p.y && p.y <= r.y + r.height ) return true;
	else return false;
}

/*****************************************************
**
**   findOrthogonalPoint
**
******************************************************/
MPoint findOrthogonalPoint( const MPoint &p, const double length )
{
	if ( p == MPoint( 0, 0 )) return MPoint();
	if ( p.imag() == 0 ) return MPoint( 0, length );

	// p * i is orthogonal
	return ( length / abs( p ))  * p * MPoint( 0, 1 );
}

/*****************************************************
**
**   pointToWxPoint
**
******************************************************/
wxPoint pointToWxPoint( const MPoint &p )
{
  return wxPoint( a_rund( p.real() ), a_rund( p.imag() ));
}

/*****************************************************
**
**   MRect   ---   toString
**
******************************************************/
wxString MRect::toString()
{
	return wxString::Format( wxT( "x %f y %f w %f h %f" ), x, y, width, height );
}

/*****************************************************
**
**   MRect   ---   grow
**
******************************************************/
MRect MRect::grow( const double &d )
{
	x -= d;
	y -= d;
	width += 2 * d;
	height += 2 * d;
	return *this;
}

/*****************************************************
**
**   MRect   ---   intersects
**
******************************************************/
bool MRect::intersects( const MRect &rect ) const
{
	MRect r = intersect( rect );
	return r.width != 0;
}

/*****************************************************
**
**   MRect   ---   intersect
**
******************************************************/
MRect MRect::intersect( const MRect& rect ) const
{
	MRect r = *this;
	r.intersect( rect );
	return r;
}

/*****************************************************
**
**   MRect   ---   intersect
**
******************************************************/
MRect& MRect::intersect( const MRect& rect )
{
	int x2 = getRight();
	int y2 = getBottom();

	if ( x < rect.x ) x = rect.x;
	if ( y < rect.y ) y = rect.y;
	if ( x2 > rect.getRight() ) x2 = rect.getRight();
	if ( y2 > rect.getBottom() ) y2 = rect.getBottom();

	width = x2 - x + 1;
	height = y2 - y + 1;

	if ( width <= 0 || height <= 0 )
	{
		width = 0;
		height = 0;
	}
	return *this;
}

/*****************************************************
**
**   MBrush   ---   Constructor
**
******************************************************/
MBrush::MBrush()
{
	style = wxSOLID;
	rotateHue = 0;
	//printf( "MBrush::MBrush STANDARD ok %d\n", IsOk() );
}

/*****************************************************
**
**   MBrush   ---   Constructor
**
******************************************************/
MBrush::MBrush( const wxColour &c, const int st )
{
	style = st;
	color = c;
	rotateHue = 0;

	//wxString colorname = color.GetAsString();
	//static int xx = 0;
	//printf( "MBrush::MBrush WITH PARAMS %d color %s\n", xx++, str2char( colorname ));
}

/*****************************************************
**
**   MBrush   ---   Constructor
**
******************************************************/
MBrush::MBrush( const MBrush &tocopy )
{
	//printf( "MBrush::MBrush COPY rotate %d\n", tocopy.rotateHue );

	this->color = tocopy.color;
	this->style = tocopy.style;
	this->filename = tocopy.filename;
	this->rotateHue = tocopy.rotateHue;
}

/*****************************************************
**
**   MBrush   ---   Constructor
**
******************************************************/
MBrush::MBrush( const wxString fn, const int rotateHue )
 : filename( fn ),
	rotateHue( rotateHue )
{
	style = wxSTIPPLE;
	color = wxNullColour;
	//printf( "MBrush::MBrush WITH STIPPLE PARAMS filename %s rotate %d\n", str2char( filename ), rotateHue );
}

/*****************************************************
**
**   MBrush   ---   toString
**
******************************************************/
wxString MBrush::toString() const
{
	wxString s;
	s << wxT( "color: " ) << color.GetAsString()
		<< wxT( " style: " ) << style
		<< wxT( " filename: '" ) << filename << wxT( "'" )
		<< wxT( " rotateHue: " ) << rotateHue;
	return s;
}

/*****************************************************
**
**   MBrush   ---   IsOk
**
******************************************************/
bool MBrush::IsOk() const
{
	if ( style == wxTRANSPARENT )
	{
		return true;
	}
	else if ( style == wxSTIPPLE )
	{
		return filename.IsEmpty() ? false : true;
	}
	else
	{
		return color.IsOk() ? true : false;
	}
	assert( false );
}

OBJECT_INCLUDES operator~ ( const OBJECT_INCLUDES& a ) { return (OBJECT_INCLUDES) ~((int)a); }
OBJECT_INCLUDES operator& ( const OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b ) { return (OBJECT_INCLUDES)( (int)a & (int)b ); }
OBJECT_INCLUDES operator| ( const OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b ) { return (OBJECT_INCLUDES)( (int)a | (int)b ); }
void operator&= ( OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b ) { a = a & b; }
void operator|= ( OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b ) { a = a | b; }

TAB_COLUMN_TYPE operator~ ( const TAB_COLUMN_TYPE& a ) { return (TAB_COLUMN_TYPE) ~((int)a); }
TAB_COLUMN_TYPE operator& ( const TAB_COLUMN_TYPE& a, const TAB_COLUMN_TYPE& b ) { return (TAB_COLUMN_TYPE)( (int)a & (int)b ); }
TAB_COLUMN_TYPE operator| ( const TAB_COLUMN_TYPE& a, const TAB_COLUMN_TYPE& b ) { return (TAB_COLUMN_TYPE)( (int)a | (int)b ); }
void operator&= ( TAB_COLUMN_TYPE& a, const TAB_COLUMN_TYPE& b ) { a = a & b; }
void operator|= ( TAB_COLUMN_TYPE& a, const TAB_COLUMN_TYPE& b ) { a = a | b; }

/*****************************************************
**
**   ObjectId operators
**
******************************************************/
ObjectId operator++( ObjectId& g ) // prefix
{
	g = (ObjectId)( (int)g + 1 );
	return g;
}

ObjectId operator++( ObjectId &g, int ) // postfix
{
	ObjectId result = g;
	g = (ObjectId)( (int)g + 1 );
	return result;
}

ObjectId operator+ ( const ObjectId &o1, const ObjectId &o2 )
{
	return (ObjectId)((int)o1 + (int)o2);
}

/*****************************************************
**
**   Rasi operators
**
******************************************************/
Rasi operator++( Rasi& g ) // prefix
{
	g = (Rasi)( (int)g + 1 );
	return g;
}

Rasi operator++( Rasi &g, int ) // postfix
{
	Rasi result = g;
	g = (Rasi)( (int)g + 1 );
	return result;
}

Rasi operator+ ( const Rasi &o1, const Rasi &o2 )
{
	return (Rasi)((int)o1 + (int)o2);
}

/*****************************************************
**
**   GRADKREIS operators
**
******************************************************/
GRADKREIS operator++( GRADKREIS& g ) // prefix
{
	g = (GRADKREIS)( (int)g + 1 );
	return g;
}

GRADKREIS operator++( GRADKREIS &g, int ) // postfix
{
	GRADKREIS result = g;
	g = (GRADKREIS)( (int)g + 1 );
	return result;
}

/*****************************************************
**
**   ASPECT_TYPE operators
**
******************************************************/
ASPECT_TYPE operator++( ASPECT_TYPE& a ) // prefix
{
	a = (ASPECT_TYPE)( (int)a + 1 );
	return a;
}

ASPECT_TYPE operator++( ASPECT_TYPE &a, int ) // postfix
{
	ASPECT_TYPE result = a;
	a = (ASPECT_TYPE)( (int)a + 1 );
	return result;
}

/*****************************************************
**
**   NakshatraId_27 operators
**
******************************************************/
NakshatraId_27 operator++( NakshatraId_27& a ) // prefix
{
	a = (NakshatraId_27)( (int)a + 1 );
	return a;
}

NakshatraId_27 operator++( NakshatraId_27 &a, int ) // postfix
{
	NakshatraId_27 result = a;
	a = (NakshatraId_27)( (int)a + 1 );
	return result;
}

/*****************************************************
**
**   NakshatraId_28 operators
**
******************************************************/
NakshatraId_28 operator++( NakshatraId_28& a ) // prefix
{
	a = (NakshatraId_28)( (int)a + 1 );
	return a;
}

NakshatraId_28 operator++( NakshatraId_28 &a, int ) // postfix
{
	NakshatraId_28 result = a;
	a = (NakshatraId_28)( (int)a + 1 );
	return result;
}

/*****************************************************
**
**   Varga operators
**
******************************************************/
Varga operator++( Varga& v ) // prefix
{
	v = (Varga)( (int)v + 1 );
	return v;
}

Varga operator++( Varga &v, int ) // postfix
{
	Varga result = v;
	v = (Varga)( (int)v + 1 );
	return result;
}


/**************************************************************
***
**   wxIntArray/ObjectArray conversion
***
***************************************************************/
wxArrayInt objectArray2wxArrayInt( const ObjectArray &oa )
{
	wxArrayInt wxa;
	for( uint i = 0; i < oa.size(); i++ )
	{
		wxa.Add( oa[i] );
	}
	return wxa;
}

/**************************************************************
***
**   wxIntArray/ObjectArray conversion
***
***************************************************************/
ObjectArray wxArrayInt2objectArray( const wxArrayInt &wxa )
{
	ObjectArray oa;
	for( uint i = 0; i < wxa.Count(); i++ )
	{
		oa.push_back( (ObjectId)wxa.Item( i ));
	}
	return oa;
}

/**************************************************************
***
**  Static ids for text alignement
***
***************************************************************/
int Align::VCenter =  0x01;
int Align::Bottom  =  0x02;
int Align::Top     =  0x04;
int Align::HCenter =  0x08;
int Align::Left    = 	0x10;
int Align::Right   = 	0x20;
int Align::Center  =  Align::VCenter | Align::HCenter;

/*****************************************************
**
**   Align   --   getApplicationVersion
**
******************************************************/
const wxString getApplicationVersion()
{
	return wxConvertMB2WX( VERSION );
}

/*****************************************************
**
**   Align   --   toString
**
******************************************************/
wxString Align::toString( const int &a )
{
	wxString s;

	if ( a & Align::Left ) s = wxT( "Left+" );
	else if ( a & Align::Right ) s = wxT( "Right+" );
	else s = wxT( "HCenter+" );

	if ( a & Align::Top ) s += wxT( "Top" );
	else if ( a & Align::Bottom ) s += wxT( "Bottom" );
	else s += wxT( "VCenter" );

	return s;
};


