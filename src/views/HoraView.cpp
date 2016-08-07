/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/HoraView.cpp
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

#include <wx/choice.h>
#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "SheetView.h"
#include "Conf.h"
#include "DataSet.h"
#include "Hora.h"
#include "Lang.h"
#include "mvalidator.h"
#include "TextHelper.h"
#include "ToolPanel.h"

extern Config *config;

/*************************************************//**
*
* \brief view containing text output for Hora
*
******************************************************/
class HoraView : public SheetView
{
	DECLARE_CLASS( HoraView )

public:

	HoraView( wxWindow *parent, ChildWindow *frame )
	: SheetView( parent, frame, VIEW_HORA, true )
	{
		props->setFixedVedic();
		expert = new HoraExpert();
		twidget->enableFloatingLayout( false );

		jd = floor( MDate::getCurrentJD() + .5 );
		isLocaltime = config->viewprefs->ephemTimezone;

		initToolItems();
		Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( HoraView::OnNow ));
		Connect( TBS_DATE, COMMAND_SPIN_CHANGED, wxCommandEventHandler( HoraView::OnDateCtrlChanged ));
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
		expert->update( jd - .5 );
		twidget->clearSheet();
		expert->write( twidget->getSheet(), isLocaltime );
	}

	void OnDateCtrlChanged( wxCommandEvent& )
	{
		/*
		if ( toolbar )
		{
			DateSpin *spin_date = (DateSpin*)toolbar->FindControl( TBS_DATE );
			if ( spin_date )
			{
				jd = spin_date->getDoubleValue();
			}
		}
		*/
		OnDataChanged();
	}

	virtual void initToolItems()
	{
		if ( toolbar )
		{
			wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_ephemtz ) choice_ephemtz->SetSelection( isLocaltime );

			wxControl *ds = toolbar->FindControl( TBS_DATE );
			if ( ds )
			{
				printf( "DATE SPIN found\n" );
				ds->SetValidator( MDateValidator( &jd ));
			}
			toolbar->InitDialog();
		}
	}

	void OnNow( wxCommandEvent& )
	{
		/*
		if ( toolbar )
		{
			DateSpin *ds = (DateSpin*)toolbar->FindControl( TBS_DATE );
			if ( ds )
			{
				jd = MDate::getCurrentJD() + .5;
				ds->setDoubleValue( jd );
			}
		}
		*/
		jd = MDate::getCurrentJD() + .5;
		if ( toolbar )
		{
			toolbar->TransferDataToWindow();
		}
		updateView = true;
	}

	void OnToolbarCommand()
	{
		if ( toolbar )
		{
			wxChoice *choice_ephemtz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_ephemtz ) isLocaltime = choice_ephemtz->GetSelection();
		}
		updateView = true;
	}


	HoraExpert *expert;
	bool isLocaltime;
	double jd;

};

IMPLEMENT_CLASS( HoraView, SheetView )

/**************************************************************
***
**   ViewFactory   ---   createHoraView
***
***************************************************************/
BasicView *ViewFactory::createHoraView( wxWindow *parent, ChildWindow *frame )
{
	return new HoraView( parent, frame );
}

