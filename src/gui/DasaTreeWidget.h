/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/DasaTreeWidget.h
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

#ifndef _DASATREEWIDGET_H_
#define _DASATREEWIDGET_H_

#include "Dasa.h"
#include "BasicWidget.h"
#include "TreeWidget.h"

class Horoscope;

DECLARE_EVENT_TYPE( DASATREE_CHANGED, wxID_HIGHEST + 1630 )

/*************************************************//**
*
* \brief holds all relevant astrological data in a tree item
*
******************************************************/
class DasaTreeItemClientData : public TreeItemClientData
{
public:

	DasaTreeItemClientData( DasaExpert *expert, Dasa *dasa ) : expert( expert ), dasa( dasa )
	{
	} 

	~DasaTreeItemClientData()
	{
		if ( dasa ) delete dasa;
	}

	bool isRootItem() { return dasa == (Dasa*)NULL; }

	DasaExpert *getExpert() const { return expert; }
	DasaId getDasaId() const
	{
		assert( expert );
		return expert->getDasaId();
	}

	Dasa *getDasa() const { return dasa; }
	void setDasa( Dasa *d ) { dasa = d; }

protected:
	DasaExpert *expert;
	Dasa *dasa;
};


/*************************************************//**
*
*
*
******************************************************/
class DasaTreeWidget : public BasicWidget
{
public:
	DasaTreeWidget( wxWindow *parent, ChartProperties*, wxWindowID id, Horoscope* );
	~DasaTreeWidget();

	void OnDataChanged();

	double getCurrentJD() const { return currentjd; }

protected:

	wxString getItemLabel( DasaTreeItemClientData* );
	void expand( MyTreeEvent& );
	void collapse( MyTreeEvent& );
	void OnSelChanging( MyTreeEvent& );
	void updateDasa( const wxTreeItemId& );
	void emitItemChanged( DasaTreeItemClientData* );

	virtual void HandleMouseWheelEvent( wxMouseEvent& );
	virtual void doPaint( const wxRect&, const bool eraseBackground = true );

	TreeWidget *twidget;
	Horoscope *horoscope;
	wxTreeItemId rootid, baseid, specialid, conditionalid;
	double currentjd;
	ChartProperties *props;
	DasaExpert *experts[MAX_DASAEXPERTS];

private:

	DECLARE_CLASS( DasaTreeWidget )
};


#endif

