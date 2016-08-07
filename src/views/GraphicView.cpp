/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/GraphicView.cpp
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
#include "ChildWindow.h"
#include "Commands.h"
#include "Conf.h"
#include "Document.h"
#include "GraphicGrid.h"
#ifdef USE_URANIAN_CHART
#include "Uranian.h"
#endif
#include "VargaBase.h"

extern Config *config;

/*************************************************//**
*
* \brief Simple graphical chart view with one chart in western or Vedic mode
*
******************************************************/
class GraphicView : public BasicView
{
public:

	/*****************************************************
	**
	**   GraphicView   ---   Constructor
	**
	******************************************************/
	GraphicView( wxWindow *parent, ChildWindow *frame, const Varga &varga, const bool &uranian )
			: BasicView( parent, frame, VIEW_GRAPHIC, false ),
			varga( varga )
	{
		printf( "GraphicView   ---   Constructor vedic %d uranian %d\n", props->isVedic(), uranian );
		props->freezeEwStatus();
#ifdef USE_URANIAN_CHART
		uexpert = (UranianExpert*)NULL;
#endif

		gwidget = new ChartGridWidget( this, CT_RADIX, props, 1, 1 );
		if ( props->isVedic() ) gwidget->addVedicChart( varga, doc );
		else
		{
#ifdef USE_URANIAN_CHART
			if ( uranian )
			{
				uexpert = new UranianExpert( doc, props, config->uranian );
				gwidget->addUranianChart( uexpert, doc );
			}
			else gwidget->addWesternChart( doc, 0 );
#else
			gwidget->addWesternChart( doc, 0 );
#endif
		}
		widget = gwidget;
		OnDataChanged();
	}

	/*****************************************************
	**
	**   GraphicView   ---   Destructor
	**
	******************************************************/
	~GraphicView()
	{
#ifdef USE_URANIAN_CHART
		if ( uexpert ) delete uexpert;
#endif
	}

	/**************************************************************
	***
	**   GraphicView   ---   getWindowLabel
	***
	***************************************************************/
	virtual wxString getWindowLabel( const bool shortname = false )
	{
		wxString s;
		VargaConfigLoader *loader = VargaConfigLoader::get();

		if ( isVedic() )
		{
			if ( shortname )
			{
				if ( varga < NB_VARGAS-1 )
					s.Printf( wxT( "D%d" ), loader->getVargaDivision( varga ) );
				else
					s.Printf( wxT( "%s" ), loader->getVargaName( varga ).c_str());
			}
			else
			{
				if ( varga < NB_VARGAS-1 )
				{
					s.Printf( wxT( "%s (D%d)" ), loader->getVargaName( varga ).c_str(),
						loader->getVargaDivision( varga ) );
				}
				else
				{
					s.Printf( wxT( "%s" ), loader->getVargaName( varga ).c_str());
				}
			}
		}
		else
		{
			return shortname ? _( "Chart" ) : _( "Western Chart" );
		}
		return s;
	}

	/*****************************************************
	**
	**   GraphicView   ---   dispatchCommand
	**
	******************************************************/
	virtual bool dispatchCommand( const int &command )
	{
		int ret;
		if ( command == CMD_WESTERNMODE || command == CMD_VEDICMODE )
		{
			ret = BasicView::dispatchCommand( command );
			frame->setTitle();
			return ret;
		}
		else return BasicView::dispatchCommand( command );
	}

	/*****************************************************
	**
	**   GraphicView   ---   OnDataChanged
	**
	******************************************************/
	virtual void OnDataChanged()
	{
		//printf( "GraphicView::OnDataChanged\n" );
#ifdef USE_URANIAN_CHART
		if ( uexpert ) uexpert->OnDataChanged();
#endif
		BasicView::OnDataChanged();
		gwidget->OnDataChanged();
	}


private:

	ChartGridWidget *gwidget;
#ifdef USE_URANIAN_CHART
	UranianExpert *uexpert;
#endif
	Varga varga;

	DECLARE_CLASS( GraphicView )
};

IMPLEMENT_CLASS( GraphicView, BasicView )

/*****************************************************
**
**   ViewFactory   ---   createGraphicView
**
******************************************************/
BasicView *ViewFactory::createGraphicView( wxWindow *parent, ChildWindow *frame, const Varga &varga, const bool &uranian )
{
	return new GraphicView( parent, frame, varga, uranian );
}

