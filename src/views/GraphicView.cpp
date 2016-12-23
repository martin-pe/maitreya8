/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/GraphicView.cpp
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
#include "ChildWindow.h"
#include "Conf.h"
#include "Document.h"
#include "GraphicGrid.h"
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
	GraphicView( wxWindow *parent, ChildWindow *frame, const Varga &varga )
			: BasicView( parent, frame, VIEW_GRAPHIC, false ),
			varga( varga )
	{
		props->freezeEwStatus();

		gwidget = new ChartGridWidget( this, CT_RADIX, props, 1, 1 );
		if ( props->isVedic())
		{
			gwidget->addVedicChart( varga, doc );
			props->setFixedVedic();
		}
		else
		{
			gwidget->addWesternChart( doc, 0 );
			props->setFixedWestern();
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
		BasicView::OnDataChanged();
		gwidget->OnDataChanged();
	}


private:

	ChartGridWidget *gwidget;
	Varga varga;

	DECLARE_CLASS( GraphicView )
};

IMPLEMENT_CLASS( GraphicView, BasicView )

/*****************************************************
**
**   ViewFactory   ---   createGraphicView
**
******************************************************/
BasicView *ViewFactory::createGraphicView( wxWindow *parent, ChildWindow *frame, const Varga &varga )
{
	return new GraphicView( parent, frame, varga );
}



