/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DasaPanel.h
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
