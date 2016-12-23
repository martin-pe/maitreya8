/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/EclipseView.cpp
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

#include <wx/choice.h>
#include <wx/spinctrl.h>

#include "Conf.h"
#include "DataSet.h"
#include "Eclipse.h"
#include "mathbase.h"
#include "TextView.h"
#include "TextWidget.h"
#include "ToolPanel.h"

extern Config *config;

/*************************************************//**
*
* \brief view containing text output for eclipses
*
******************************************************/
class EclipseView : public TextView
{
	DECLARE_CLASS( EclipseView )

public:

	EclipseView( wxWindow *parent, ChildWindow *frame )
		: TextView( parent, frame, VIEW_ECLIPSE, true )
	{
		startyear = getCurrentYear();
		years = 3;
		type = 0;
		isLocaltime = config->viewprefs->ephemTimezone;
		expert = new EclipseExpert;
		twidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );

		initToolItems();
	}

	~EclipseView() { delete expert; }

	virtual wxString getWindowLabel( const bool = false ) { return _( "Eclipse" ); }

protected:

	virtual void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxSpinCtrl *spin_start = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_start ) startyear = spin_start->GetValue();

			wxSpinCtrl *spin_years = (wxSpinCtrl*)toolbar->FindControl( TBS_NBYEARS );
			if ( spin_years ) years = spin_years->GetValue();

			wxChoice *choice_type = (wxChoice*)toolbar->FindControl( TBS_ECL_TYPE );
			if ( choice_type ) type = choice_type->GetSelection();

			wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_ephemtz ) isLocaltime = choice_ephemtz->GetSelection();
			config->viewprefs->ephemTimezone = isLocaltime;
		}
		updateView = true;
	}


	virtual void initToolItems()
	{
		if ( toolbar )
		{
			wxSpinCtrl *spin_start = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_start ) spin_start->SetValue( startyear );

			wxSpinCtrl *spin_years = (wxSpinCtrl*)toolbar->FindControl( TBS_NBYEARS );
			if ( spin_years ) spin_years->SetValue( years );

			wxChoice *choice_type = (wxChoice*)toolbar->FindControl( TBS_ECL_TYPE );
			if ( choice_type ) choice_type->SetSelection( type );

			wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_ephemtz ) choice_ephemtz->SetSelection( isLocaltime );
		}
	}

	/*****************************************************
	**
	**   EclipseView   ---   write
	**
	******************************************************/
	virtual void write()
	{
		double startjd, endjd;
		MDate d;
		d.setDate( 1, 1, startyear, 0 );
		startjd = d.getJD();
		d.setDate( 1, 1, startyear + years, 0 );
		endjd = d.getJD();

		expert->update( startjd, endjd, type, 0, isVedic() );
		twidget->clearSheet();
		expert->write( twidget->getSheet(), isLocaltime );
	}

	int startyear, years, type;
	int isLocaltime;
	EclipseExpert *expert;
};

IMPLEMENT_CLASS( EclipseView, TextView )

/**************************************************************
***
**   ViewFactory   ---   createEclipseView
***
***************************************************************/
BasicView *ViewFactory::createEclipseView( wxWindow *parent, ChildWindow *frame )
{
	return new EclipseView( parent, frame );
}

