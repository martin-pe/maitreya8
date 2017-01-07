/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DasaPanel.h
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

#ifndef DASAPANEL_H
#define DASAPANEL_H

#include <wx/collpane.h>

class wxBitmapButton;
class wxStaticText;

enum { DASA_CREATE_ENTRY_CHART = wxID_HIGHEST + 2000 };

/*************************************************//**
*
*  \brief bottom panel in Dasa tree view
*
******************************************************/
class DasaPanel: public wxCollapsiblePane
{
	friend class DasaView;
public:

	DasaPanel(wxWindow* parent, int id = -1 );
	void clear();

private:

protected:
	wxBitmapButton* button_entry_chart;
	wxStaticText* label_type;
	wxStaticText* label_startdate;
	wxStaticText* label_enddate;
	wxStaticText* label_duration;

private:
};


#endif // DASAPANEL_H
