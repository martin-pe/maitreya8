/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/ChartField.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#include "ChartField.h"

#include "astrobase.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "Varga.h"
#include "VedicChartConfig.h"

#include <math.h>

extern Config *config;

/*****************************************************
**
**   ChartField   ---   Constructor
**
******************************************************/
ChartField::ChartField( const MRect &r, const GRAVITATION  &grav, const FIELD_TYPE type )
{
	rect = r;
	gravitation = grav;
	fieldtype = type;
	
	align = Align::Center;
	calculateAlign();
}

/*****************************************************
**
**   ChartField   ---   clear
**
******************************************************/
void ChartField::clear()
{
	contents.clear();
	tcontents.clear();
}

/*****************************************************
**
**   ChartField   ---   getContents
**
******************************************************/
ChartContents &ChartField::getContents( const int &hid )
{
	assert( hid == 0 || hid == 1 );
	return( hid == 0 ? contents : tcontents );
}

/*****************************************************
**
**   ChartContents   ---   clear
**
******************************************************/
void ChartContents::clear()
{
	textitems.clear();
	graphicitems.clear();
	planets.clear();
}

/*****************************************************
**
**   ChartContents   ---   isTriangle
**
******************************************************/
bool ChartField::isTriangle()
{
	return fieldtype == FIELD_TYPE_NE || fieldtype == FIELD_TYPE_SW || fieldtype == FIELD_TYPE_NW || fieldtype == FIELD_TYPE_SE;
}

/*****************************************************
**
**   ChartField   ---   getCenter
**
******************************************************/
MPoint ChartField::getCenter()
{
	switch( fieldtype )
	{
		case FIELD_TYPE_FULL:
		case FIELD_TYPE_DIAMOND:
			return MPoint( rect.x + 0.5 * rect.width, rect.y + 0.5 * rect.height );
		break;

		case FIELD_TYPE_NE:
			return MPoint( rect.x + 0.7 * rect.width, rect.y + 0.3 * rect.height );
		break;
		case FIELD_TYPE_SW:
			return MPoint( rect.x + 0.3 * rect.width, rect.y + 0.7 * rect.height );
		break;
		case FIELD_TYPE_NW:
			return MPoint( rect.x + 0.3 * rect.width, rect.y + 0.3 * rect.height );
		break;
		case FIELD_TYPE_SE:
			return MPoint( rect.x + 0.7 * rect.width, rect.y + 0.7 * rect.height );
		break;

		case FIELD_TYPE_N:
		break;
		case FIELD_TYPE_W:
		break;
		case FIELD_TYPE_S:
		break;
		case FIELD_TYPE_E: 
		break;
		default:
		break;
	}


	return MPoint( rect.x + 0.5 * rect.width, rect.y + 0.5 * rect.height );
}

/*****************************************************
**
**   ChartField   ---   calculateModifiedRect
**
******************************************************/
MRect ChartField::calculateModifiedRect( const FIELD_PART &mode, const MPoint &dmargin )
{
	MRect r = rect;
	if ( mode == FP_ALL ) return r;

	switch( gravitation )
	{
		case GRAVITATION_S: // e.g. diamond house 1, aries in east indian chart
			if ( mode == FP_OUTER )
			{
				r.height = dmargin.imag();
			}
			else // transit radix
			{
				r.y += dmargin.imag();
				r.height -= dmargin.imag();
			}
		break;
		case GRAVITATION_SE: // right down, e.g. taurus and gemini in east indian chart
			if ( mode == FP_OUTER )
			{
				//r.width = dmargin.real();
				r.height = dmargin.imag();
				if ( fieldtype == FIELD_TYPE_SW )
				{
					r.y = dmargin.imag();
					r.width = dmargin.real();
				}
			}
			else // transit radix
			{
				r.x += dmargin.real();
				r.width -= dmargin.real();
				r.y += dmargin.imag();
				r.height -= dmargin.imag();
			}
		break;
		case GRAVITATION_SW: // left down, e.g. aq and pisces in east indian chart
			if ( mode == FP_OUTER )
			{
				if ( fieldtype == FIELD_TYPE_SE )
				{
					r.y = dmargin.imag();
					r.height -= dmargin.imag();
					r.x = r.x + r.width - dmargin.real();
					r.width = dmargin.real();
				}
				else
				{
					r.height = dmargin.imag();
				}
			}
			else // transit radix
			{
				r.width -= dmargin.real();
				r.y += dmargin.imag();
				r.height -= dmargin.imag();
			}
		break;
		case GRAVITATION_E: // left
			if ( mode == FP_OUTER )
			{
				r.width = dmargin.real();
			}
			else 
			{
				r.x += dmargin.real();
				r.width -= dmargin.real();
			}
		break;
		case GRAVITATION_N:
			if ( mode == FP_OUTER )
			{
				r.y = r.y + r.height - dmargin.imag();
				r.height = dmargin.imag();
			}
			else // transit radix
			{
				r.height -= dmargin.imag();
			}
		break;
		case GRAVITATION_NE:
			if ( mode == FP_OUTER )
			{
				if ( fieldtype == FIELD_TYPE_NW )
				{
					r.height -= dmargin.imag();
					r.width = dmargin.real();
				}
				else
				{
					r.y += r.height - dmargin.imag();
					r.height = dmargin.imag();
				}
			}
			else // transit radix
			{
				r.x += dmargin.real();
				r.width -= dmargin.real();
				r.height -= dmargin.imag();
			}
		break;
		case GRAVITATION_NW:
			if ( mode == FP_OUTER )
			{
				if ( fieldtype == FIELD_TYPE_NE )
				{
					r.x = r.x + r.width - dmargin.real();
					r.width = dmargin.real();
					r.height -= dmargin.imag();
				}
				else
				{
					r.y += r.height - dmargin.imag();
					r.height = dmargin.imag();
				}
			}
			else // transit radix
			{
				r.width -= dmargin.real();
				r.height -= dmargin.imag();
			}
		break;
		case GRAVITATION_W:
			if ( mode == FP_OUTER )
			{
				r.x = r.x + r.width - dmargin.real();
				r.width = dmargin.real();
			}
			else // transit radix
			{
				r.width -= dmargin.real();
			}
		break;
		default:
			assert( false );
		break;
	}
	return r;
}

/*****************************************************
**
**   ChartField   ---   calculateAlign
**
******************************************************/
void ChartField::calculateAlign()
{
	switch( fieldtype )
	{
		case FIELD_TYPE_FULL: // all fields in south indian chart, sbc, fixed rasis in east indian chart
			align = Align::Center;
		break;

		case FIELD_TYPE_NE: // east: taurus and sag
			align = Align::Right + Align::Top;
		break;

		case FIELD_TYPE_SW: // east: gemini and scorpio
			align = Align::Left + Align::Bottom;
		break;

		case FIELD_TYPE_NW: // east: leo and pisces
			align = Align::Left + Align::Top;
		break;

		case FIELD_TYPE_SE: // east: virgo and aquarius
			align = Align::Right + Align::Bottom;
		break;

		case FIELD_TYPE_N: // north: houses 2 and 12
			align = Align::Top + Align::HCenter;
		break;

		case FIELD_TYPE_W: // north: houses 3 and 5
			align = Align::Left + Align::VCenter;
		break;

		case FIELD_TYPE_S: // north: houses 6 and 8
			align = Align::Bottom + Align::HCenter;
		break;

		case FIELD_TYPE_E: // north: houses 9 and 11
			align = Align::Right + Align::VCenter;
		break;

		case FIELD_TYPE_DIAMOND:// north: houses 1: 4: 7: 10
			align = Align::Center;
		break;

		default:
			printf( "WARN: invalid field type %d in ChartContents::calculateAlign\n", fieldtype );
			align = Align::Center;
		break;
	}
}



