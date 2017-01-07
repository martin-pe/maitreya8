/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SplitterWidget.h
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


#ifndef _SPLITTERWIDGET_H_
#define _SPLITTERWIDGET_H_

#include <wx/splitter.h>
#include "Commands.h"

/*************************************************//**
*
* \brief base splitter class adding support for several events
*
******************************************************/
class SplitterWidget : public wxSplitterWindow, public KeyEventObserver
{
	DECLARE_CLASS( SplitterWidget )
public:
	SplitterWidget( wxWindow *parent, const wxWindowID = -1, const wxPoint &pos = wxDefaultPosition,
		const wxSize size = wxDefaultSize, long style = wxSP_3D  );

	~SplitterWidget();

protected:
	void onMouseRight( wxMouseEvent& );
	void OnSize( wxSizeEvent& );
	void OnPositionChanged( wxSplitterEvent& );
	void OnSplitterDoubleClick(wxSplitterEvent& );
	void OnSplitterSashPosChanged(wxSplitterEvent& );
	void OnKeyDown( wxKeyEvent& );
	void OnSetFocus( wxFocusEvent& );

};

#endif

