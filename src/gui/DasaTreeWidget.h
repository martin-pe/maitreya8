/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/DasaTreeWidget.h
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

#ifndef _DASATREEWIDGET_H_
#define _DASATREEWIDGET_H_

#include "Dasa.h"
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
class DasaTreeWidget : public TreeWidget
{
public:
	DasaTreeWidget( wxWindow *parent, wxWindowID id, Horoscope*, ChartProperties* );
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
	void OnContextMenuEvent( wxMouseEvent& );

	Horoscope *horoscope;
	wxTreeItemId rootid, baseid, specialid, conditionalid;
	double currentjd;
	ChartProperties *props;
	DasaExpert *experts[MAX_DASAEXPERTS];

private:

	DECLARE_CLASS( DasaTreeWidget )
};


#endif

