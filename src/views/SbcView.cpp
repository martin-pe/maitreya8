/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/SbcView.cpp
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

#include "BasicView.h"
#include "Document.h"
#include "GraphicGrid.h"

//#define WO_BUNDLE_SBCVIEW WO_MENU_SBC_STYLE + WO_MENU_FULL_OBJECT + WO_MENU_SKIN + WO_EXPORT_GRAFIC

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
		//cwidget->setWidgetOptions( WO_BUNDLE_SBCVIEW );
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

