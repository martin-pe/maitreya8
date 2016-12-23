/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/Statusbar.h
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

