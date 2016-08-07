/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/EclipseView.cpp
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

#include "SheetView.h"
#include "Conf.h"
#include "DataSet.h"
#include "Eclipse.h"
#include "mathbase.h"
#include "ToolPanel.h"

extern Config *config;

/*************************************************//**
*
* \brief view containing text output for eclipses
*
******************************************************/
class EclipseView : public SheetView
{
	DECLARE_CLASS( EclipseView )

public:

	EclipseView( wxWindow *parent, ChildWindow *frame )
		: SheetView( parent, frame, VIEW_ECLIPSE, true )
	{
		startyear = getCurrentYear();
		years = 3;
		type = 0;
		isLocaltime = config->viewprefs->ephemTimezone;
		expert = new EclipseExpert;
		twidget->enableFloatingLayout( false );

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
		//expert->write( writer, isLocaltime );
		twidget->clearSheet();
		expert->write( twidget->getSheet(), isLocaltime );
	}

	int startyear, years, type;
	int isLocaltime;
	EclipseExpert *expert;
};

IMPLEMENT_CLASS( EclipseView, SheetView )

/**************************************************************
***
**   ViewFactory   ---   createEclipseView
***
***************************************************************/
BasicView *ViewFactory::createEclipseView( wxWindow *parent, ChildWindow *frame )
{
	return new EclipseView( parent, frame );
}

