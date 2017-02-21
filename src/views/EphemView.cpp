/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/EphemView.cpp
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

#include "EphemView.h"

#include <wx/choice.h>
#include <wx/log.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Commands.h"
#include "Conf.h"
#include "Ephemeris.h"
#include "guibase.h"
#include "Lang.h"
#include "mathbase.h"
#include "Painter.h"
#include "ToolPanel.h"
#include "SheetWidget.h"
#include "SymbolProvider.h"
#include "TextWidget.h"

extern Config *config;

IMPLEMENT_CLASS( EphemView, BasicView )
IMPLEMENT_CLASS( GraphicalEphemWidgetItem, SheetWidgetItem )

/*****************************************************
**
**   EphemView   ---   Constructor
**
******************************************************/
EphemView::EphemView( wxWindow *parent, ChildWindow *frame )
	: BasicView( parent, frame, VIEW_EPHEM, true )
{
	int dummy;
	DateTimeFormatter::get()->calculateDateIntegers( MDate::getCurrentJD(), dummy, month, year );
	expert = new EphemExpert( props );

	lastmonth = month;
	dasaId = D_VIMSOTTARI;
	planets = 1;
	mode = config->viewprefs->ephemMode;
	circleType = config->viewprefs->ephemCircleType;

	swidget = (SheetWidget*)NULL;
	twidget = (TextWidget*)NULL;

	isLocaltime = config->viewprefs->ephemTimezone;

	expert->prepareMonth( month, year, isLocaltime );

	setDirty();
	initToolItems();
	initClientView();

	Connect( wxEVT_IDLE, wxIdleEventHandler( EphemView::OnIdle ));
	Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EphemView::OnNow ));
}

/*****************************************************
**
**   EphemView   ---   Destructor
**
******************************************************/
EphemView::~EphemView()
{
	delete expert;
}

/*****************************************************
**
**   EphemView   ---   initToolItems
**
******************************************************/
void EphemView::initToolItems()
{
	if ( toolbar )
	{
		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) spin_year->SetValue( year );

		wxSpinCtrl *spin_month = (wxSpinCtrl*)toolbar->FindControl( TBS_MONTH );
		if ( spin_month ) spin_month->SetValue( month );

		wxChoice *choice_ephem_mode = (wxChoice*)toolbar->FindControl( TBS_EPHEMMODE );
		if ( choice_ephem_mode ) choice_ephem_mode->SetSelection( mode );

		wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
		if ( choice_ephemtz ) choice_ephemtz->SetSelection( isLocaltime );

		wxChoice *choice_ephemdeg = (wxChoice*)toolbar->FindControl( TBS_EPHEMDEG );
		if ( choice_ephemdeg )
		{
			choice_ephemdeg->SetSelection( circleType );
			choice_ephemdeg->Enable( mode == 1 );
		}
		setMaxDeg( 0 );

		wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
		if ( choice_dasa )
		{
			choice_dasa->Enable( mode == 5 );
		}
		//toolbar->enableControl( TBS_DASA, mode == 5 );
	}
}

/*****************************************************
**
**   EphemView   ---   initClientView
**
******************************************************/
void EphemView::initClientView()
{
	//printf( "EphemView::initClientView mode %d widget %ld swidget %ld twidget %ld\n", mode, (long)widget, (long)swidget, (long)twidget ); 
	//if ( widget ) delete widget;
	if ( mode == 1 )
	{
		if ( ! swidget )
		{
			swidget = new SheetWidget( this, props );
			setMaxDeg( circleType );
			swidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );
		}
		widget = swidget;
		if ( twidget ) delete twidget;
		twidget = (TextWidget*)NULL;
	}
	else
	{
		if ( ! twidget )
		{
			twidget = new TextWidget( this, props );
		}
		twidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );
		if ( mode == 0 || mode == 3 )
		{
			// object toggle enabled for longitudes and ingress
			twidget->addWidgetOption( WO_MENU_RESTRICTED_OBJECT );
		}
		widget = twidget;
		if ( swidget ) delete swidget;
		swidget = (SheetWidget*)NULL;
	}
	doLayout();
	//printf( "ENDE EphemView::initClientView mode %d widget %ld swidget %ld twidget %ld\n", mode, (long)widget, (long)swidget, (long)twidget ); 
}

/**************************************************************
***
**   EphemView   ---   write
***
***************************************************************/
void EphemView::write()
{
	//printf( "EphemView::write mode %d widget %ld swidget %ld twidget %ld\n", mode, (long)widget, (long)swidget, (long)twidget ); 
	//printf( "EphemView::write mode %d month %d year %d\n", mode, month, year );
	assert( mode >= 0 && mode < MAX_EPHEM_VIEWTYPES );
	int ret = 0;

	expert->prepareMonth( month, year, isLocaltime );
	if ( mode == 1 )
	{
		assert( swidget );
		Sheet *sheet = swidget->getSheet();
		assert( sheet );
		sheet->clear();
		ret = expert->calcMonth();

		SheetWidgetGrid *grid = new SheetWidgetGrid( 1, .5, 100, 100, 1600, 800 );
		SheetWidgetItem *w = new GraphicalEphemWidgetItem( props, expert, max_deg, isLocaltime );
		grid->addItem( w );
		sheet->addItem( grid );
		swidget->OnDataChanged();
	}
	else
	{
		assert( twidget );
		Sheet *sheet = twidget->getSheet();
		sheet->clear();
		switch ( mode )
		{
			case 2:
				expert->writeDetails( sheet );
				break;
			case 3:
				ret = expert->writeIngress( sheet );
				break;
			case 4:
				expert->writeLunar( sheet );
				break;
			case 5:
				expert->writeKp( sheet, dasaId );
				break;
			default:
				ret = expert->writeLongitudes( sheet );
			break;
		}
		twidget->OnDataChanged();
	}
	Refresh();

	if ( ret )
	{
		wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
		event.SetEventObject( (wxWindow*)this );
		wxPostEvent( GetParent(), event );
	}
}

/*************************************************//**
*
*     EphemView   ---   OnDataChanged
*
******************************************************/
void EphemView::OnDataChanged()
{
	//printf( "EphemView::OnDataChanged\n" );
	setDirty();
	BasicView::OnDataChanged();
}

/*************************************************//**
*
*     EphemView   ---   OnIdle
*
******************************************************/
void EphemView::OnIdle( wxIdleEvent& )
{
	assert( mode >= 0 && mode < MAX_EPHEM_VIEWTYPES );
	//static int count = 0;
	//printf( "EphemView::OnIdle %d mode %d dirty %d\n", count++, mode, dirty[mode] );
	if ( ! dirty[mode] ) return;


	write();
	dirty[mode] = false;
}

/*****************************************************
**
**   EphemView   ---   setDirty
**
******************************************************/
void EphemView::setDirty( const bool b )
{
	for( int i = 0; i < MAX_EPHEM_VIEWTYPES; i++ )
	{
		dirty[i] = b;
	}
}

/*****************************************************
**
**   EphemView   ---   supportsGraphicExport
**
******************************************************/
bool EphemView::supportsGraphicExport() const
{
	return ( mode == 1 );
}

/*****************************************************
**
**   EphemView   ---   supportsTextExport
**
******************************************************/
bool EphemView::supportsTextExport() const
{
	return ( mode != 1 );
}

/*****************************************************
**
**   EphemView   ---   OnToolbarCommand
**
******************************************************/
void EphemView::OnToolbarCommand()
{
	if ( toolbar )
	{
		wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_EPHEMMODE );
		if ( choice_mode )
		{
			if ( mode != choice_mode->GetSelection() )
			{
				mode = choice_mode->GetSelection();
				config->viewprefs->ephemMode = mode;
				initClientView();
			}
		}

		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		if ( spin_year ) year = spin_year->GetValue();

		wxSpinCtrl *spin_month = (wxSpinCtrl*)toolbar->FindControl( TBS_MONTH );
		if ( spin_year && spin_month )
		{
			if ( lastmonth == 12 && spin_month->GetValue() == 1 )
				spin_year->SetValue( spin_year->GetValue() + 1 );
			if ( lastmonth == 1 && spin_month->GetValue() == 12 )
				spin_year->SetValue( spin_year->GetValue() - 1 );

			lastmonth = spin_month->GetValue();
			year = spin_year->GetValue();
			month = spin_month->GetValue();
		}

		wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
		if ( choice_ephemtz )
		{
			isLocaltime = choice_ephemtz->GetSelection();
			config->viewprefs->ephemTimezone = isLocaltime;
		}

		wxChoice *choice_ephemdeg = (wxChoice*)toolbar->FindControl( TBS_EPHEMDEG );
		if ( choice_ephemdeg )
		{
			circleType = choice_ephemdeg->GetSelection();
			config->viewprefs->ephemCircleType = circleType;
			setMaxDeg( circleType );
			choice_ephemdeg->Enable( mode == 1 );
		}

		wxChoice *choice_dasa = (wxChoice*)toolbar->FindControl( TBS_DASA );
		if ( choice_dasa )
		{
			dasaId = (DasaId)choice_dasa->GetSelection();
			choice_dasa->Enable(  mode == 5 );
		}
	}
	OnDataChanged();
}

/*****************************************************
**
**   EphemView   ---   setMaxDeg
**
******************************************************/
void EphemView::setMaxDeg( const int &sel )
{
	switch ( sel )
	{
	case 0 :
		max_deg =   360;
		break;
	case 1 :
		max_deg =   180;
		break;
	case 2 :
		max_deg =    90;
		break;
	case 3 :
		max_deg =    45;
		break;
	case 4 :
		max_deg =    30;
		break;
	case 5 :
		max_deg =  22.5;
		break;
	case 6 :
		max_deg = 13.333333333;
		break;
	case 7 :
		max_deg = 11.25;
		break;
	case 8 :
		max_deg = 5.125;
		break;
	default:
		max_deg =   360;
		break;
	}
}

/*****************************************************
**
**   EphemView   ---   OnNow
**
******************************************************/
void EphemView::OnNow( wxCommandEvent& )
{
	int dummy;
	if ( toolbar )
	{
		wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
		wxSpinCtrl *spin_month = (wxSpinCtrl*)toolbar->FindControl( TBS_MONTH );
		if ( spin_year && spin_month )
		{
			DateTimeFormatter::get()->calculateDateIntegers( MDate::getCurrentJD(), dummy, month, year );
			spin_year->SetValue( year );
			spin_month->SetValue( month );
			lastmonth = month;
		}
		expert->prepareMonth( month, year, isLocaltime );
	}
	setDirty();
}

/**************************************************************
***
**   EphemView   ---   getWindowLabel
***
***************************************************************/
wxString EphemView::getWindowLabel( const bool /* shortname */ )
{
	return _( "Ephemeris" );
}

/**************************************************************
***
**   GraphicalEphemWidgetItem   ---   Constructor
***
***************************************************************/
GraphicalEphemWidgetItem::GraphicalEphemWidgetItem( ChartProperties *props, EphemExpert *expert, const double &max_deg, const bool &localtime )
 : SheetWidgetItem( props ),
 expert( expert ),
 max_deg( max_deg ),
 isLocaltime( localtime )
{
	filter = 1;
}

/**************************************************************
***
**   GraphicalEphemWidgetItem   ---   Destructor
***
***************************************************************/
GraphicalEphemWidgetItem::~GraphicalEphemWidgetItem()
{
}

/**************************************************************
***
**   GraphicalEphemWidgetItem   ---   cloneClean
***
***************************************************************/
SheetItem *GraphicalEphemWidgetItem::cloneClean()
{
	SheetItem *item = new GraphicalEphemWidgetItem( props, expert, max_deg, isLocaltime );
	return item;
}

/*****************************************************
**
**   GraphicalEphemWidgetItem   ---   setLineStyle
**
******************************************************/
void GraphicalEphemWidgetItem::setLineStyle( Painter *painter, const ObjectId &planet )
{
	const int lw = 2;
	painter->setPen( wxPen( config->chartColors->getPlanetColor( planet ), lw, wxSOLID ));
}

/**************************************************************
***
**   GraphicalEphemWidgetItem   ---   doPaint
***
***************************************************************/
void GraphicalEphemWidgetItem::doPaint( Painter *painter, const MRect& /*refreshRect*/ )
{
	assert( painter );
	//printf( "GraphicalEphemWidgetItem::doPaint rect %f %f %f %f\n", rect.x, rect.y, rect.width, rect.height );

	xright = rect.x + rect.width;
	ybottom = rect.y + rect.height;

	painter->setDefaults();
	paintRuler( painter );
	paintPlanets( painter );
}

/*****************************************************
**
**   GraphicalEphemWidgetItem   ---   paintRuler
**
******************************************************/
void GraphicalEphemWidgetItem::paintRuler( Painter *painter )
{
	wxString s;
	int i;
	int y = (int)ybottom-30;
	double x;

	const double ytext = 10;
	const double tic_len = 5;
	const double xtext = 50;
	const double ysizey = 20;

	painter->setTransparentBrush();

	painter->setPenColor( config->colors->fgColor );

	// outer rectangle
	painter->drawRectangle( rect );
	const int mlen = expert->getNumberOfDays();

	// tics on the horizontal ruler at the bottom (per day) and horizontal reference lines
	const double xstep = rect.width / ( mlen - 1 );
	for ( i = 0; i < mlen; i++ )
	{
		x =  rect.x + i * xstep;

		// tic
		painter->drawLine( x, ybottom, x, ybottom + tic_len );
		s.Printf( wxT( "%d" ), i + 1 );
		painter->drawTextFormatted( wxRect( x - xtext, y + ysizey + ytext, 2 * xtext, 2 * ytext ), s, Align::Center );

		// ref lines
		if ( ! ( ( i + 1 ) % 5 ) && i > 0 )
		{
			painter->setPenColor( *wxLIGHT_GREY ); // DASH ?
			painter->drawLine( x, ybottom, x, rect.y );
			painter->setPenColor( config->colors->fgColor );
		}
	}

	// vertical tics on the left and grey reference lines
	const double ystep = rect.height / max_deg;
	for ( i = 1; i < max_deg; i++ )
	{
		// avoid line for certain gradkreis values
		if ( max_deg >= 180 && ( i % 30 ) ) continue;
		if ( max_deg >= 90 && ( i % 10 ) ) continue;
		if ( max_deg >= 22 && ( i % 5 ) ) continue;

		y = ybottom - i * ystep;

		// tic
		painter->drawLine( rect.x - tic_len, y, rect.x, y );
		s.Printf( wxT( "%d" ), i );
		painter->drawTextFormatted( wxRect( rect.x - xtext - tic_len, y - ysizey, xtext, 2 * ysizey ), s, Align::Right+Align::VCenter );

		// ref line
		painter->setPenColor( *wxLIGHT_GREY ); // DASH ?
		painter->drawLine( rect.x, y, xright, y );
		painter->setPenColor( config->colors->fgColor );
	}

	// mark current date if possible
	if ( expert->isCurrentMonth())
	{
		MDate d;
		double actjd = d.getJD();
		if ( isLocaltime )
		{
			const TzInfo tzi = TzUtil().calculateTzInfoForJD( actjd );
			actjd += ( tzi.tzhours + tzi.dsthours ) / 24.0;
		}

		d.setDate( 1, expert->getMonth(), expert->getYear(), 0 );
		double portion = ( actjd - d.getJD() ) / ( expert->getNumberOfDays() + 1 );
		x = rect.x + xstep + portion * ( rect.width );
		painter->setPenColor( *wxBLUE );
		painter->drawLine( x, ybottom, x, rect.y );
		painter->setPenColor( config->colors->fgColor );
	}
}


/*****************************************************
**
**   GraphicalEphemWidgetItem   ---   paintPlanets
**
******************************************************/
void GraphicalEphemWidgetItem::paintPlanets( Painter *painter )
{
	ObjectId p;
	double x1, x2, y1, y2;  // daily positions in y and y dimension
	double xp;              // x position for jumps
	double l1, l2;       // length of planets
	double yp, yp2, ydiff;      // length values for jumps
	const int sshift = 60;
	int lsymbolshift[sshift];
	int rsymbolshift[sshift];
	int ylshift, yrshift;
	const int xshiftunit = 15;

	const int mlen = expert->getNumberOfDays();  // length of month
	const double xstep = rect.width / mlen;  // daily step in x dimension

	wxString s;
	Lang lang;

	for ( int i = 0; i < sshift; i++ )
	{
		lsymbolshift[i] = 0;
		rsymbolshift[i] = 0;
	}

	for ( unsigned i1 = 0; i1 < expert->getPlanetdataSize(); i1++ )
	{
		p = expert->getPlanetId( i1 );

		// loop if daily motion of moon is smaller than max_deg
		if ( max_deg < 14 &&  p == OMOON ) continue;

		setLineStyle( painter, p );
		for ( int day = 0; day < mlen; day++ )
		{
			x1 =  rect.x + day * xstep;
			x2 =  x1 + xstep;
			l1 = a_red( expert->getPlanetLongitude( i1, day ), max_deg );
			y1 = ybottom - l1 * rect.height / max_deg;
			l2 = a_red( expert->getPlanetLongitude( i1, day+1 ), max_deg );
			y2 = ybottom - l2 * rect.height / max_deg;

			if ( expert->getPlanetRetro( i1, day ))
			{
				if (( l1 < l2 ) && expert->getPlanetRetro( i1, day+1 )) // handle jumps
				{
					yp = l1;
					yp2 = max_deg - l2;
					ydiff = yp / ( yp + yp2 );
					xp = (int)( x1 + ydiff * xstep );
					painter->drawLine( x1, y1, xp, ybottom );
					painter->drawLine( xp, rect.y, x2, y2 );
				}
				else // that's normal
				{
					painter->drawLine( x1, y1, x2, y2 );
				}
			}
			else // non retrograde
			{
				if (( l1 > l2 ) && ! expert->getPlanetRetro( i1, day+1 )) // handle jumps
				{
					yp = max_deg - l1;
					yp2 = l2;
					ydiff = yp / ( yp + yp2 );
					xp = x1 + ydiff * xstep;
					//printf( "2 planet %d yp %f yp2 %f x1 %d x2 %d xp %d l1 %f l2 %f ydiff %f\n", p, yp, yp2, x1, x2, xp, l1, l2, ydiff );
					painter->drawLine( x1, y1, xp, rect.y );
					painter->drawLine( xp, ybottom, x2, y2 );
				}
				else // that happens normally
				{
					painter->drawLine( x1, y1, x2, y2 );
				}
			}
		}
		if ( config->writer->planetSymbols )
		{
			painter->setSymbolFont();
			s = SymbolProvider().getPlanetCode( p );
		}
		else
		{
			painter->setGraphicFont();
			s = lang.getObjectName( p, TF_MEDIUM, props->isVedic() );
		}
		// Planet name on left side
		y1 = ybottom - a_red( expert->getPlanetLongitude( i1, 0 ), max_deg ) * rect.height / max_deg;

		assert( rect.height != 0 );
		ylshift = a_red( sshift * y1 / rect.height, 60 );

		painter->drawTextFormatted(
			wxRect( Max( rect.x - xshiftunit * lsymbolshift[ylshift] - 10, 0 ), y1 - 5, 10, 10 ), s, Align::Right+Align::VCenter );
		lsymbolshift[ylshift]++;

		// Planet name on right side
		y1 = ybottom - a_red( expert->getPlanetLongitude( i1, mlen ), max_deg ) * rect.height / max_deg;
		yrshift = a_red( sshift * y1 / rect.height, 60 );
		painter->drawTextFormatted( wxRect( xright + xshiftunit * rsymbolshift[yrshift], y1 - 5, 20, 10 ), s, Align::Left+Align::VCenter );
		rsymbolshift[yrshift]++;
	}
}

/**************************************************************
***
**   ViewFactory   ---   createEphemView
***
***************************************************************/
BasicView *ViewFactory::createEphemView( wxWindow *parent, ChildWindow *frame )
{
	return new EphemView( parent, frame );
}

