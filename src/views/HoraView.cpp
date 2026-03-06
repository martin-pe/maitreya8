/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/HoraView.cpp
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

#include <wx/choice.h>
#include <wx/log.h>
#include <wx/toolbar.h>

#include "Conf.h"
#include "DataSet.h"
#include "Hora.h"
#include "mspin.h"
#include "mvalidator.h"
#include "TextView.h"
#include "TextWidget.h"
#include "ToolPanel.h"

extern Config *config;

/*************************************************//**
*
* \brief view containing text output for Hora
*
******************************************************/
class HoraView : public TextView
{
	DECLARE_CLASS( HoraView )

public:

	HoraView( wxWindow *parent, ChildWindow *frame )
	: TextView( parent, frame, VIEW_HORA, true )
	{
		props->setFixedVedic();
		expert = new HoraExpert();

		isLocaltime = config->viewprefs->ephemTimezone;
		setCurrentDate();

		initToolItems();
		Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( HoraView::OnNow ));
		Connect( TBS_DATE, COMMAND_SPIN_CHANGED, wxCommandEventHandler( HoraView::OnDateCtrlChanged ));
		Connect( TBS_DATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( HoraView::OnDateCtrlChanged ));
	}

	~HoraView()
	{
		config->viewprefs->ephemTimezone = isLocaltime;
		delete expert;
	}

	virtual wxString getWindowLabel( const bool = false ) { return _( "Hora" ); }

protected:

	virtual void write()
	{
		expert->update( jd + 1 );
		twidget->clearSheet();
		expert->write( twidget->getSheet(), isLocaltime );
	}

	void OnDateCtrlChanged( wxCommandEvent& )
	{
		//printf( "HoraView::OnDateCtrlChanged\n" );
		OnDataChanged();
	}

	virtual void initToolItems()
	{
		if ( toolbar )
		{
			wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_ephemtz ) choice_ephemtz->SetSelection( isLocaltime );

			MDateSpin *ds = wxDynamicCast( toolbar->FindControl( TBS_DATE ), MDateSpin );
			if( ds )
			{
				ds->SetValidator( MDateValidator( &jd ));
			}
			else
			{
				wxLogError( wxT( "Date spin not found in toolbar. View will not work properly." ));
			}
			toolbar->TransferDataToWindow();
		}
	}

	void setCurrentDate()
	{
		double currentJD = MDate::getCurrentJD();
    if ( isLocaltime )
    {
      currentJD += ( config->defaultLocation->getTimeZone() + config->defaultLocation->getDST() ) / 24.0;
    }

    jd = floor( currentJD - .5 ) + .5;
		if ( toolbar ) toolbar->TransferDataToWindow();
		updateView = true;
	}

	void OnNow( wxCommandEvent& ) { setCurrentDate(); }

	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_ephemtz ) isLocaltime = choice_ephemtz->GetSelection();
		}
		updateView = true;
	}


	HoraExpert *expert = nullptr;

	bool isLocaltime = true;

	double jd = 0.0;

};

IMPLEMENT_CLASS( HoraView, TextView )

/**************************************************************
***
**   ViewFactory   ---   createHoraView
***
***************************************************************/
BasicView *ViewFactory::createHoraView( wxWindow *parent, ChildWindow *frame )
{
	return new HoraView( parent, frame );
}

