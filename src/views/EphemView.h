/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/EphemView.h
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

#ifndef _EPHEMVIEW_H_
#define _EPHEMVIEW_H_

#include "BasicView.h"
#include "Sheet.h"

class ChartProperties;
class EphemExpert;
class EphemWidget;
class SheetWidget;
class TextWidget;

#define MAX_EPHEM_VIEWTYPES 6

/*************************************************//**
*
* \brief ephemeris view containing various text tabs and graphical ephemeris
*
******************************************************/
class EphemView : public BasicView
{
	DECLARE_CLASS( EphemView )

public:
	EphemView( wxWindow*, ChildWindow* );
	~EphemView();

	virtual wxString getWindowLabel( const bool shortname = false );

	virtual bool supportsGraphicExport() const;
	virtual bool supportsTextExport() const;

	virtual void OnDataChanged();

protected:
	EphemExpert *expert;
	int month, lastmonth, year, planets, mode, circleType;
	DasaId dasaId;
	bool isLocaltime;
	double max_deg;
	bool dirty[MAX_EPHEM_VIEWTYPES];

	SheetWidget *swidget;
	TextWidget *twidget;

	virtual void write();
	void initClientView();
	void setDirty( const bool = true );
	//virtual void OnDataChanged();

	void OnIdle( wxIdleEvent& );
	virtual void OnToolbarCommand();
	void OnNow( wxCommandEvent& );
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

