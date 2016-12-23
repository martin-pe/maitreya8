/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/SbcView.cpp
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

#include "BasicView.h"
#include "Document.h"
#include "GraphicGrid.h"

#define WO_BUNDLE_SBCVIEW WO_MENU_SBC_STYLE | WO_MENU_FULL_OBJECT | WO_MENU_SKIN | WO_EXPORT_GRAPHIC

/*************************************************//**
*
* \brief View class for Sarvatobhadra chart
*
******************************************************/
class SbcView : public BasicView
{
	DECLARE_CLASS( SbcView )

public:

	/*****************************************************
	**
	**   SbcView   ---   Constructor
	**
	******************************************************/
	SbcView( wxWindow *parent, ChildWindow *frame )
			: BasicView( parent, frame, VIEW_SBC, false, wxFULL_REPAINT_ON_RESIZE )
	{
		props->setFixedVedic();

		ChartGridWidget *cwidget = new ChartGridWidget( this, CT_RADIX, props );
		cwidget->setWidgetOptions( WO_BUNDLE_SBCVIEW );
		cwidget->addSarvatobhadraChart( doc, (Horoscope*)NULL );
		widget = cwidget;

		OnDataChanged();
	}

protected:

	/**************************************************************
	***
	**   SbcView   ---   getWindowLabel
	***
	***************************************************************/
	virtual wxString getWindowLabel( const bool shortname = false )
	{
		return shortname ? _( "Sbc" ) : _( "Sarvatobhadra" );
	}

};

IMPLEMENT_CLASS( SbcView, BasicView )

/**************************************************************
***
**   ViewFactory   ---   createSbcView
***
***************************************************************/
BasicView *ViewFactory::createSbcView( wxWindow *parent, ChildWindow *frame )
{
	return new SbcView( parent, frame );
}

