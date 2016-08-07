/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/Statusbar.cpp
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

#include "Statusbar.h"

#include <wx/menu.h>
#include <wx/timer.h>

#include "astrobase.h"
#include "Calculator.h"
#include "Conf.h"
#include "DataSet.h"
#include "Lang.h"
#include "MenuProvider.h"
#include "Sheet.h"

extern Config *config;

#define STATUS_MESSAGE_TIMEOUT 3000

enum { STATUSBAR_PTIMER = wxID_HIGHEST + 2500, STATUSBAR_MSGTIMER };

IMPLEMENT_CLASS( MyStatusbar, wxStatusBar )
IMPLEMENT_CLASS( MainWindowStatusbar, MyStatusbar )

/*****************************************************
**
**   MyStatusbar   ---   Constructor
**
******************************************************/
MyStatusbar::MyStatusbar( wxWindow *parent )
		: wxStatusBar( parent, -1, wxST_SIZEGRIP )
{
	msgtimer = new wxTimer( this, STATUSBAR_MSGTIMER );

	Connect( STATUSBAR_MSGTIMER, wxEVT_TIMER, wxTimerEventHandler( MyStatusbar::OnMessageTimer ));
}

/*****************************************************
**
**   MyStatusbar   ---   Destructor
**
******************************************************/
MyStatusbar::~MyStatusbar()
{
	if ( msgtimer->IsRunning() ) msgtimer->Stop();
	delete msgtimer;
}

/*****************************************************
**
**   MyStatusbar   ---   echo
**
******************************************************/
void MyStatusbar::echo( wxString message )
{
	if ( msgtimer->IsRunning()) msgtimer->Stop();

	SetStatusText( message, 0 );

	if ( ! message.IsEmpty())
	{
		msgtimer->Start( STATUS_MESSAGE_TIMEOUT, true );
	}
}

/*****************************************************
**
**   MyStatusbar   ---   OnMessageTimer
**
******************************************************/
void MyStatusbar::OnMessageTimer( wxTimerEvent& )
{
	if ( msgtimer->IsRunning()) msgtimer->Stop();
	SetStatusText( wxEmptyString );
}

/*****************************************************
**
**   MainWindowStatusbar   ---   Constructor
**
******************************************************/
MainWindowStatusbar::MainWindowStatusbar( wxWindow *parent )
		: MyStatusbar( parent )
{
	SetFieldsCount( 2 );
	planettimer = new wxTimer( this, STATUSBAR_PTIMER );

	Connect( STATUSBAR_PTIMER, wxEVT_TIMER, wxTimerEventHandler( MainWindowStatusbar::OnPlanetTimer ));
	Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( MainWindowStatusbar::OnRightDown ));
}

/*****************************************************
**
**   MainWindowStatusbar   ---   Destructor
**
******************************************************/
MainWindowStatusbar::~MainWindowStatusbar()
{
	if ( planettimer->IsRunning() ) planettimer->Stop();
	delete planettimer;
}

/*****************************************************
**
**   MainWindowStatusbar   ---   StartPlanetTimer
**
******************************************************/
void MainWindowStatusbar::StartPlanetTimer( const bool &b )
{
	if ( b )
	{
		if ( ! planettimer->IsRunning() ) planettimer->Start( 1000 );
	}
	else
	{
		if ( planettimer->IsRunning() ) planettimer->Stop();
		SetStatusText( wxEmptyString, 0 );
		SetStatusText( wxEmptyString, 1 );
	}
}

/*****************************************************
**
**   MainWindowStatusbar   ---   OnPlanetTimer
**
******************************************************/
void MainWindowStatusbar::OnPlanetTimer( wxTimerEvent& )
{
	wxString t0, t1;
	double mlen, asclen, dummy, nakportion;
	Lang lang;
	Calculator *calculator = CalculatorFactory().getCalculator();
	DataSet d;
	SheetFormatter fmt;

	d.setCurrentDate();
	d.setLocation( *config->defaultLocation );
	asclen = calculator->calcAscendantAya( d.getJD(), d.getLocation()->getLatitude(),
		d.getLocation()->getLongitude(), config->preferVedic );

	calculator->calcPosition( &d, OMOON, mlen, dummy, true, config->preferVedic );

	t0 << fmt.fragment2PlainText( fmt.getObjectName( OASCENDANT, TF_MEDIUM ))
	 << SPACE
	 << fmt.fragment2PlainText( fmt.getPosFormatted( asclen ));
	SetStatusText( t0, 0 );

	t1 << fmt.fragment2PlainText( fmt.getObjectName( OMOON, TF_MEDIUM ))
	 << SPACE
	 << fmt.fragment2PlainText( fmt.getPosFormatted( mlen ));
	if ( config->preferVedic )
	{
		nakportion = (int)(getNakshatraLongitude27( mlen ) * 10000 /NAKSHATRA_LEN );
		nakportion /= 100;

		t1 << SPACE << nakportion << wxT( "%" ) << SPACE
			<< lang.getNakshatra27Name( ::getNakshatra27( mlen ), TF_MEDIUM );
	}
	SetStatusText( t1, 1 );
}

/*****************************************************
**
**   MainWindowStatusbar   ---   OnRightDown
**
******************************************************/
void MainWindowStatusbar::OnRightDown( wxMouseEvent &event )
{
	wxMenu *menu = ContextMenuProvider().getAppWindowContextMenu();
	PopupMenu( menu, event.GetX(), event.GetY() );
	delete menu;
}

