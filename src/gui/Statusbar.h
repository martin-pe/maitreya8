/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/Statusbar.h
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

#ifndef _STATUSBAR_H_
#define _STATUSBAR_H_

#include <wx/statusbr.h>

class wxMouseEvent;
class wxTimer;
class wxTimerEvent;

/*************************************************//**
*
* \brief status bar with logging feature
*
******************************************************/
class MyStatusbar : public wxStatusBar
{
	DECLARE_CLASS( Statusbar )
public:
	MyStatusbar( wxWindow *parent );

	~MyStatusbar();

	void echo( wxString );

private:

	wxTimer *msgtimer;

	void OnMessageTimer( wxTimerEvent& );

};

/*************************************************//**
*
* \brief status bar with planet view feature
*
******************************************************/
class MainWindowStatusbar : public MyStatusbar
{
	DECLARE_CLASS( MainWindowStatusbar )
public:
	MainWindowStatusbar( wxWindow *parent );
	~MainWindowStatusbar();

	void StartPlanetTimer( const bool& );
	void OnRightDown( wxMouseEvent& );

private:
	wxTimer *planettimer;
	void OnPlanetTimer( wxTimerEvent& );
};

#endif

