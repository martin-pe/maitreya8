/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/EphemView.h
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

#ifndef _EPHEMVIEW_H_
#define _EPHEMVIEW_H_

#include "SheetView.h"

class ChartProperties;
class EphemExpert;
class EphemWidget;

/*************************************************//**
*
* \brief ephemeris view containing various text tabs and graphical ephemeris
*
******************************************************/
class EphemView : public SheetView
{
	DECLARE_CLASS( EphemView )

public:
	EphemView( wxWindow*, ChildWindow* );
	~EphemView();

	virtual wxString getWindowLabel( const bool shortname = false );

	virtual bool supportsGraphicExport() const;
	virtual bool supportsTextExport() const;

protected:
	EphemExpert *expert;
	int month, lastmonth, year, planets, mode, circleType;
	DasaId dasaId;
	bool isLocaltime;
	double max_deg;

	virtual void write();

	virtual void OnToolbarCommand();
	void OnYearSpin( wxSpinEvent& );
	void OnMonthSpin( wxSpinEvent& );
	void OnNow( wxCommandEvent& );
	void OnChoiceTZ( wxCommandEvent& );
	void setMaxDeg( const int& );
	virtual void initToolItems();
};

/*************************************************//**
*
* \brief widget for graphical ephemeris
*
******************************************************/
class GraphicalEphemWidgetItem :  public SheetWidgetItem
{
	DECLARE_CLASS( GraphicalEphemWidgetItem )

public:
	GraphicalEphemWidgetItem( ChartProperties*, EphemExpert*, const double &max_deg );
	~GraphicalEphemWidgetItem();

  virtual SheetItem *cloneClean();

	virtual bool mouseHasMoved( const wxPoint &p, const bool &outside );

	virtual void doPaint( Painter *painter, const MRect &refreshRect );

	void setMaxDeg( const double &m ) { max_deg = m; }

	void setFilter( const int &f ) { filter = f; }

protected:

	EphemExpert *expert;
	double max_deg;

	int zoom;
	int filter;
	double xleft, xright, ytop, ybottom;

	void paintRuler( Painter* );
	void paintPlanets( Painter* );
	void setLineStyle( Painter *painter, const ObjectId &planet );
};

#endif

