/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/EphemView.cpp
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
#include "SymbolProvider.h"

extern Config *config;

IMPLEMENT_CLASS( EphemView, BasicView )
IMPLEMENT_CLASS( GraphicalEphemWidgetItem, SheetWidgetItem )

/*****************************************************
**
**   EphemView   ---   Constructor
**
******************************************************/
EphemView::EphemView( wxWindow *parent, ChildWindow *frame )
		: SheetView( parent, frame, VIEW_EPHEM, true )
{
	int dummy;
	DateTimeFormatter::get()->calculateDateIntegers( MDate::getCurrentJD(), dummy, month, year );
	expert = new EphemExpert( props );

	lastmonth = month;
	dasaId = D_VIMSOTTARI;
	planets = 1;
	mode = config->viewprefs->ephemMode;
	circleType = config->viewprefs->ephemCircleType;

	twidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );

	isLocaltime = config->viewprefs->ephemTimezone;

	expert->prepareMonth( month, year, isLocaltime );

	initToolItems();

	Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EphemView::OnNow ));

	Connect( TBS_YEAR, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( EphemView::OnYearSpin ));
	Connect( TBS_MONTH, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( EphemView::OnMonthSpin ));

	Connect( TBS_EPHEMPLANETS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
	Connect( TBS_TZ, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceTZ ));
	Connect( TBS_DASA, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
	Connect( TBS_EPHEMMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
	Connect( TBS_EPHEMDEG, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( EphemView::OnChoiceCommand ));
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
**   EphemView   ---   OnChoiceTZ
**
******************************************************/
void EphemView::OnChoiceTZ( wxCommandEvent& )
{
	OnDataChanged();
}

/*****************************************************
**
**   EphemView   ---   OnYearSpin
**
******************************************************/
void EphemView::OnYearSpin( wxSpinEvent& )
{
	OnDataChanged();
}

/*****************************************************
**
**   EphemView   ---   OnMonthSpin
**
******************************************************/
void EphemView::OnMonthSpin( wxSpinEvent& )
{
	OnDataChanged();
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
		updateView = true;
	}
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
	updateView = true;
}

/**************************************************************
***
**   EphemView   ---   write
***
***************************************************************/
void EphemView::write()
{
	int ret = 0;
	Sheet *sheet = twidget->getSheet();
	twidget->clearSheet();

	expert->prepareMonth( month, year, isLocaltime );
	switch ( mode )
	{
	case 1:
	{
		ret = expert->calcMonth();
		//writer->addLine( wxT( "HALLO" ));
		SheetWidgetItem *w = new GraphicalEphemWidgetItem( props, expert, max_deg );
		sheet->addItem( w );
	}
	break;
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

	if ( ret )
	{
		wxCommandEvent event( SHOW_EPHEM_FILE_WARNING );
		event.SetEventObject( (wxWindow*)this );
		wxPostEvent( GetParent(), event );
	}
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
GraphicalEphemWidgetItem::GraphicalEphemWidgetItem( ChartProperties *props, EphemExpert *expert, const double &max_deg )
 : SheetWidgetItem( props ),
 expert( expert ),
 max_deg( max_deg )
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
	SheetItem *item = new GraphicalEphemWidgetItem( props, expert, max_deg );
	return item;
}

/*****************************************************
**
**   GraphicalEphemWidgetItem   ---   mouseHasMoved
**
******************************************************/
bool GraphicalEphemWidgetItem::mouseHasMoved( const wxPoint &p, const bool &outside )
{
  printf( "GraphicalEphemWidgetItem::mouseHasMoved x %d y %d outside %d\n", p.x, p.y, outside );
	return false;
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

	xleft = rect.x;
	xright = xleft + rect.width;
	ytop = rect.y;
	ybottom = ytop + rect.height;

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
	int xtext = 50;
	int ytext = 10;
	int ysizey = 20;
	int y = (int)ybottom-30;
	int x = (int)xleft;
	double ystep;

	painter->setTransparentBrush();

	painter->setPenColor( config->colors->fgColor );
	painter->drawRectangle( (int)xleft, (int)ybottom, (int)(xright - xleft), (int)(ytop - ybottom) );
	int mlen = expert->getNumberOfDays();
	double dstep = ( xright - xleft ) / ( mlen - 1 );
	for ( i = 0; i < mlen; i++ )
	{
		painter->setPenColor( config->colors->fgColor );
		x =  (int)(xleft+i*dstep);
		painter->drawLine( x, (int)ybottom, x, (int)ybottom+5 );
		s.Printf( wxT( "%d" ), i+1 );
		painter->drawTextFormatted( wxRect( x-xtext, y + ysizey + ytext, 2*xtext, 2*ytext ), s, Align::Center );
		if ( ! ( ( i + 1 ) % 5 ) && i > 0 )
		{
			painter->setPenColor( *wxLIGHT_GREY ); // TODO DASH
			painter->drawLine( (int)x, (int)ybottom, (int)x, (int)ytop );
		}
	}

	ystep = (ybottom - ytop) / max_deg;
	for ( i = 1; i < max_deg; i++ )
	{
		painter->setPenColor( config->colors->fgColor );

		if ( max_deg >= 180 && ( i % 30 ) ) continue;
		if ( max_deg >= 90 && ( i % 10 ) ) continue;
		if ( max_deg >= 22 && ( i % 5 ) ) continue;
		y = (int)(ybottom - i * ystep);
		painter->drawLine( (int)xleft-5, (int)y, (int)xleft, (int)y );
		s.Printf( wxT( "%d" ), i );
		painter->drawTextFormatted( wxRect( (int)xleft-xtext-5, y - ysizey, xtext, 2 * ysizey ), s, Align::Right+Align::VCenter );

		painter->setPenColor( *wxLIGHT_GREY ); // TODO DASH
		painter->drawLine( (int)xleft, (int)y, (int)xright, (int)y );
	}

	// Finally mark current date if possible
	if ( expert->isCurrentMonth())
	{
		MDate d;
		double actjd = d.getJD();
		d.setDate( 1, expert->getMonth(), expert->getYear(), 0 );
		double portion = ( actjd - d.getJD() ) / expert->getNumberOfDays();
		x = xleft + portion * ( xright - xleft );
		painter->setPenColor( *wxBLUE );
		painter->drawLine( (int)x, (int)ybottom, (int)x, (int)ytop );
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
	int x1, x2, y1, y2;  // daily positions in y and y dimension
	int xp;              // x position for jumps
	//int i;
	double l1, l2;       // length of planets
	double yp, yp2, ydiff;      // length values for jumps
	const int sshift = 60;
	int lsymbolshift[sshift];
	int rsymbolshift[sshift];
	int ylshift, yrshift;
	const int xshiftunit = 15;

	const int ytotal =  (int)(ybottom - ytop);   // total height of interior
	const int mlen = expert->getNumberOfDays();  // length of month
	const double dstep = ( xright - xleft ) / mlen;  // daily step in x dimension
	//const int lstep = 20;                 // step height for legend entries

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
			x1 =  (int)(xleft + day * dstep);
			x2 =  (int)(x1 + dstep);
			l1 = a_red( expert->getPlanetLongitude( i1, day ), max_deg );
			y1 = (int)(ybottom - l1 * ytotal / max_deg);
			l2 = a_red( expert->getPlanetLongitude( i1, day+1 ), max_deg );
			y2 = (int)(ybottom - l2 * ytotal / max_deg);

			if ( expert->getPlanetRetro( i1, day ))
			{
				if (( l1 < l2 ) && expert->getPlanetRetro( i1, day+1 )) // handle jumps
				{
					yp = l1;
					yp2 = max_deg - l2;
					ydiff = yp / ( yp + yp2 );
					xp = (int)( x1 + ydiff * dstep );
					painter->drawLine( x1, y1, xp, (int)ybottom );
					painter->drawLine( xp, (int)ytop, x2, y2 );
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
					xp = (int)( x1 + ydiff * dstep );
					//printf( "2 planet %d yp %f yp2 %f x1 %d x2 %d xp %d l1 %f l2 %f ydiff %f\n", p, yp, yp2, x1, x2, xp, l1, l2, ydiff );
					painter->drawLine( x1, y1, xp, (int)ytop );
					painter->drawLine( xp, (int)ybottom, x2, y2 );
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
		y1 = (int)(ybottom - a_red( expert->getPlanetLongitude( i1, 0 ), max_deg ) * ytotal / max_deg);
		ylshift = a_red( sshift * y1 / ytotal, 60 );

		painter->drawTextFormatted(
			wxRect( Max( (int)xleft - xshiftunit * lsymbolshift[ylshift] - 10, 0 ), y1 - 5, 10, 10 ),
			s, Align::Right+Align::VCenter );
		lsymbolshift[ylshift]++;

		// Planet name on right side
		y1 = (int)(ybottom - a_red( expert->getPlanetLongitude( i1, mlen ), max_deg ) * ytotal / max_deg);
		yrshift = a_red( sshift * y1 / ytotal, 60 );
		painter->drawTextFormatted( wxRect( (int)xright + xshiftunit * rsymbolshift[yrshift], y1 - 5, 20, 10 ), s, Align::Left+Align::VCenter );
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

