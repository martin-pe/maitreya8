/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/GraphicGrid.cpp
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

#include "GraphicGrid.h"

#include "BasicWesternChart.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "GraphicalChart.h"
#include "Lang.h"
#include "Painter.h"
#include "PlanetList.h"
#include "Varga.h"

#include <wx/log.h>

extern Config *config;

#define NO_CHART_ID UINT_FOR_NOT_FOUND

IMPLEMENT_CLASS( ChartGridWidget, BasicWidget )
IMPLEMENT_CLASS( DefaultEwChartGridWidget, ChartGridWidget )
IMPLEMENT_CLASS( ShadVargaChartGridWidget, ChartGridWidget )
IMPLEMENT_CLASS( VargaTab2ChartGridWidget, ChartGridWidget )
IMPLEMENT_CLASS( VargaTab3ChartGridWidget, ChartGridWidget )

#define WO_BUNDLE_GRAPHICGRID WO_MENU_FULL_GRAPHIC_STYLE | WO_MENU_FULL_OBJECT | WO_MENU_SKIN | WO_EXPORT_GRAPHIC

/*****************************************************
**
**   ChartGridWidget   ---   Constructor
**
******************************************************/
ChartGridWidget::ChartGridWidget( wxWindow *parent, const ChartType &charttype, ChartProperties *chartprops,
	const int rows, const int cols, const wxWindowID id )
		: BasicWidget( parent, chartprops, id ),
		charttype( charttype ),
		rows( rows ),
		cols( cols )
{
	printf( "ChartGridWidget::ChartGridWidget VEDIC %d\n", chartprops->isVedic());
	lastMouseOverChart = NO_CHART_ID;
	wchart = (BasicWesternChart*)NULL;
	setWidgetOptions( WO_BUNDLE_GRAPHICGRID );
	Connect( wxEVT_SIZE, wxSizeEventHandler( ChartGridWidget::OnSize ));
	Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( ChartGridWidget::OnMouseLeftDown ));
}

/*****************************************************
**
**   ChartGridWidget   ---   Destructor
**
******************************************************/
ChartGridWidget::~ChartGridWidget()
{
	deleteCharts();
}

/**************************************************************
***
**   ChartGridWidget   ---   OnMouseLeftDown
***
***************************************************************/
void ChartGridWidget::OnMouseLeftDown( wxMouseEvent &event )
{
	BasicWidget::OnMouseLeftDown( event );

	//printf( "ChartGridWidget::OnMouseLeftDown size %d\n", (int)charts.size() );
	wxRect rect;
	const uint thechart = findChartForCurrentScreenPos();
	if ( thechart != NO_CHART_ID )
	{
		if ( isVedic() )
		{
			if ( thechart < charts.size())
			{
				if ( charts[thechart]->handleMouseLeftEvent( event.m_x, event.m_y, rect )) RefreshRect( rect );
			}
			else wxLogError( wxString::Format( wxT( "Cannot access chart #%u" ), thechart ));
		}
		else
		{
			if ( wchart )
			{
				if ( wchart->handleMouseLeftEvent( event.m_x, event.m_y, rect )) RefreshRect( rect );
			}
			else wxLogError( wxT( "Cannot access western chart" ));
		}
	}
}

/*****************************************************
**
**   ChartGridWidget   ---   setHoroscopes
**
******************************************************/
void ChartGridWidget::setHoroscopes( Horoscope *h1, Horoscope *h2 )
{
	for( uint i = 0; i < charts.size(); i++ )
	{
		charts[i]->setHoroscopes( h1, h2 );
	}
	if ( wchart ) wchart->setHoroscopes( h1, h2 );
}

/*****************************************************
**
**   ChartGridWidget   ---   addVedicChart
**
******************************************************/
void ChartGridWidget::addVedicChart( Varga varga, Horoscope *h1, Horoscope *h2 )
{
	//printf( "ChartGridWidget::addVedicChart Varga %d rows %d cols %d size %d\n", (int)varga, rows, cols, (int)charts.size() );
	//assert( charts.size() < rows * cols );

	if ( ! IS_VALID_VARGA( varga ))
	{
		wxLogError( wxT( "invalid varga %d in ChartGridWidget" ), varga );
		varga = V_RASI;
	}

	if( charts.size() < rows * cols )
	{
		GraphicalChart *chart = ChartFactory().createVedicChart( charttype, chartprops, h1, h2, varga );
		charts.push_back( chart );
	}
	else 
	printf( "ERROR ChartGridWidget::addVedicChart Varga %d rows %d cols %d size %d\n", (int)varga, rows, cols, (int)charts.size() );
}

/*****************************************************
**
**   ChartGridWidget   ---   addSarvatobhadraChart
**
******************************************************/
void ChartGridWidget::addSarvatobhadraChart( Horoscope *h1, Horoscope *h2 )
{
	charts.push_back( ChartFactory().createSarvatobhadraChart( charttype, chartprops, h1, h2 ));
	setFixedVedic();
}

/*****************************************************
**
**   ChartGridWidget   ---   addWesternChart
**
******************************************************/
void ChartGridWidget::addWesternChart( Horoscope *h1, Horoscope *h2 )
{
	wchart = ChartFactory().createWesternChart( charttype, chartprops, h1, h2 );
	wchart->OnDataChanged();
}

/*****************************************************
**
*   ChartGridWidget   ---   getAspectExpert
**
******************************************************/
AspectExpert *ChartGridWidget::getAspectExpert() const
{
	if ( wchart ) return wchart->getAspectExpert();
	else if ( charts.size() > 0 )
	{
		return charts[0]->getAspectExpert();
	}
	else return (AspectExpert*)NULL;
}

/*****************************************************
**
**   ChartGridWidget   ---   changeChartType
**
******************************************************/
void ChartGridWidget::changeChartType( const ChartType &ct )
{
	//printf( "ChartGridWidget::changeChartType NEW type %d OLD was %d\n", (int)ct, (int)charttype );
	ASSERT_VALID_CHARTTYPE( ct );

	if ( ct != charttype )
	{
		charttype = ct;
		for( uint i = 0; i < charts.size(); i++ )
		{
			charts[i]->changeChartType( charttype );
		}
		if ( wchart ) wchart->changeChartType( charttype );
	}
}

/*****************************************************
**
**   ChartGridWidget   ---   deleteCharts
**
******************************************************/
void ChartGridWidget::deleteCharts()
{
	for ( uint i = 0; i < charts.size(); i++ ) delete charts[i];
	charts.clear();
	if ( wchart != 0 ) delete wchart;
	wchart = 0;
}

/*****************************************************
**
**   ChartGridWidget   ---   OnDataChanged
**
******************************************************/
void ChartGridWidget::OnDataChanged()
{
	/*

	TODO was macht man wenn western(vedic umgeschaltet wird?
	- alte lösung: Liste mit vedic charts und parallel western
	- besser virtuelle Lösung, allerdings muss dann die Liste durchiteriert werden.
	Und: die Vargas gehen verloren, wenn man w-v macht.

	Was passiert übrigens mit AV? das darf nicht ersetzt werden durch westernchart. Dann weiß man nicht mehr weiter bei w-v

	Arten von charts
	- western
	- vedic mit varga
	- AV
	- SBC

	1, 2 und 4 können im Transitmodus kommen.
	alle habe irgendwo ein chart und evtl. ein chart2.
	aber: das kann auch ein AV sein oder ein solarchart
	und bei vedic varga > 0 ist das horoskop nur unterhalb enthalten.

	weitere Frage: wenn supportewtoggle eingeschaltet wird, wirkt das nur auf die Menus.
	das ist falsch. muss im event handler geskipt werden.

	*/

	//static int i = 0;
	//printf( "ChartGridWidget::OnDataChanged no. %d\n", i++ );
	for ( uint i = 0; i < charts.size(); i++ )
	{
		charts[i]->OnDataChanged();
	}
	if ( wchart ) wchart->OnDataChanged();
	Refresh();
}

/*****************************************************
**
**   ChartGridWidget   ---   OnSize
**
******************************************************/
void ChartGridWidget::OnSize( wxSizeEvent &event )
{
	//printf( "ChartGridWidget::OnSize %ld\n", vediccharts.size() );
	OnDataChanged();
	SetSize( event.GetSize().x, event.GetSize().y );
}

/*****************************************************
**
**   ChartGridWidget   ---   OnEWToggle
**
******************************************************/
void ChartGridWidget::OnEWToggle( const bool &vedic )
{
  printf( "ChartGridWidget::OnEWToggle %d charts.size %ld wchart %ld\n", vedic, charts.size(), (long)wchart );
	// TODO

}

/**************************************************************
***
**   ChartGridWidget   ---   mouseHasMoved
***
***************************************************************/
void ChartGridWidget::mouseHasMoved( const bool& )
{
	//printf( "ChartGridWidget::mouseHasMoved vedic %d\n", chartprops->isVedic() );
	if ( isVedic() )
	{
		const uint thechart = findChartForCurrentScreenPos();
		//printf( "ChartGridWidget::mouseHasMoved %d thechart %u\n", b, thechart );

		// reset old chart if mouse has moved out without motion event on the chart itself
		if ( lastMouseOverChart != NO_CHART_ID && thechart != lastMouseOverChart )
		{
			assert( lastMouseOverChart < rows * cols );
			if ( charts[lastMouseOverChart]->OnResponsiveChartEvent()) RefreshRect( getRectForChartIndex( lastMouseOverChart ));
		}

		if ( thechart != NO_CHART_ID && thechart < charts.size())
		{
			if ( charts[thechart]->OnResponsiveChartEvent( MPoint( mousePosition.x, mousePosition.y ), keyMod )) RefreshRect( getRectForChartIndex( thechart ));
		}
		lastMouseOverChart = thechart;
	}
	else
	{
		if ( wchart )
		{
			if ( wchart->OnResponsiveChartEvent( MPoint( mousePosition.x, mousePosition.y ))) Refresh();
		}
		else wxLogError( wxT( "ChartGridWidget::mouseHasMoved western chart is not set!" ));
	}
}

/**************************************************************
***
**   ChartGridWidget   ---   getRectForChartIndex
***
***************************************************************/
wxRect ChartGridWidget::getRectForChartIndex( const uint& id )
{
	const wxSize size = GetVirtualSize();
	const int xstep = size.x / cols;
	const int ystep = size.y / rows;

	const int col = id % cols;
	const int row = id / cols;

	//printf( "ChartGridWidget::getRectForChartIndex x %d y %d w %d h %d col %d row %d\n", xstep * col, ystep * row, xstep, ystep, col, row );
	return wxRect( xstep * col, ystep * row, xstep, ystep );
}

/**************************************************************
***
**   ChartGridWidget   ---   findChartForCurrentScreenPos
***
***************************************************************/
uint ChartGridWidget::findChartForCurrentScreenPos()
{
	// single chart
	if ( rows == 1 && cols == 1 ) return 0;

	// more complex grid
	if ( chartprops->isVedic() )
	{
		const wxSize size = GetVirtualSize();
		const uint xstep = size.x / cols;
		const uint ystep = size.y / rows;

		//printf( "xstep %d ystep %d\n", xstep, ystep );

		uint thecol = -1;
		for ( uint col = 0; col < cols; col++ )
		{
			if ( col * xstep <= (unsigned)mousePosition.x && ( col + 1 ) * xstep >= (unsigned)mousePosition.x )
			{
				thecol = col;
				break;
			}
		}

		uint therow = -1;
		for ( uint row = 0; row < rows; row++ )
		{
			if ( row * ystep <= (unsigned)mousePosition.y && ( row + 1 ) * ystep >= (unsigned)mousePosition.y )
			{
				therow = row;
				break;
			}
		}
		//printf( "therow ist %d thecol %d nb %u\n", therow, thecol, therow * cols + thecol );
		if ( therow < rows && thecol < cols ) return therow * cols + thecol;
	}
	return NO_CHART_ID;
}

/*****************************************************
**
**   ChartGridWidget   ---   doPaint
**
******************************************************/
void ChartGridWidget::doPaint( const wxRect &rect, const bool /* eraseBackground */ )
{
	assert( painter );
	const wxSize size = GetVirtualSize();

	if ( isVedic() )
	{
		if ( charts.size() == 0 ) wxLogError( wxT( "ChartGridWidget::doPaint vedic chart list is empty!" ));

		uint nb = 0;
		for ( uint row = 0; row < rows && nb < charts.size(); row++ )
		{
			for ( uint col = 0; col < cols; col++ )
			{
				assert( nb < charts.size());
				assert( charts[nb] );
				wxRect chartRect = getRectForChartIndex( nb );

				if ( rect.Intersects( chartRect ))
				{
					charts[nb]->paint( painter, chartRect );
				}
				nb++;
			}
		}
	}
	else
	{
		if ( wchart ) wchart->paint( painter, MRect( 0, 0, size.x, size.y ));
		else wxLogError( wxT( "ChartGridWidget::doPaint western chart is not set!" ));
	}
}

/*****************************************************
**
**   ShadVargaChartGridWidget   ---   Constructor
**
******************************************************/
ShadVargaChartGridWidget::ShadVargaChartGridWidget( wxWindow *parent, const ChartType &charttype, ChartProperties *chartprops,
	Horoscope *h1, Horoscope *h2 )
		: ChartGridWidget( parent, charttype, chartprops, 2, 3 )
{
	addVedicChart( V_RASI, h1, h2 );
	addVedicChart( V_HORA, h1, h2 );
	addVedicChart( V_DREKKANA, h1, h2 );
	addVedicChart( V_NAVAMSA, h1, h2 );
	addVedicChart( V_DVADASAMSA, h1, h2 );
	addVedicChart( V_TRIMSAMSA, h1, h2 );
	setFixedVedic();
}

/*****************************************************
**
**   VargaTab2ChartGridWidget   ---   Constructor
**
******************************************************/
VargaTab2ChartGridWidget::VargaTab2ChartGridWidget( wxWindow *parent, const ChartType &charttype, ChartProperties *chartprops,
	Horoscope *h1, Horoscope *h2 )
		: ChartGridWidget( parent, charttype, chartprops, 2, 3 )
{
	addVedicChart( V_CHATURTAMSA, h1, h2 );
	addVedicChart( V_SAPTAMAMSA, h1, h2 );
	addVedicChart( V_DASAMSA, h1, h2 );
	addVedicChart( V_SHODASAMSA, h1, h2 );
	addVedicChart( V_VIMSAMSA, h1, h2 );
	addVedicChart( V_SIDDHAMSA, h1, h2 );
	setFixedVedic();
}

/*****************************************************
**
**   VargaTab3ChartGridWidget   ---   Constructor
**
******************************************************/
VargaTab3ChartGridWidget::VargaTab3ChartGridWidget( wxWindow *parent, const ChartType &charttype, ChartProperties *chartprops,
	Horoscope *h1, Horoscope *h2 )
		: ChartGridWidget( parent, charttype, chartprops, 2, 3 )
{
	addVedicChart( V_BHAMSA, h1, h2 );
	addVedicChart( V_CHATVARIMSAMSA, h1, h2 );
	addVedicChart( V_AKSHAVEDAMSA, h1, h2 );
	addVedicChart( V_SHASTIAMSA, h1, h2 );
	addVedicChart( V_ASHTOTTARAMSA, h1, h2 );
	addVedicChart( V_DVADASDVADASAMSA, h1, h2 );
	setFixedVedic();
}

/*****************************************************
**
**   DefaultEwChartGridWidget   ---   Constructor
**
******************************************************/
DefaultEwChartGridWidget::DefaultEwChartGridWidget( wxWindow *parent, const ChartType &charttype, ChartProperties *chartprops,
	Horoscope *h )
		: ChartGridWidget( parent, charttype, chartprops, 2, 2 )
{
	addVedicChart( V_RASI, h );
	addVedicChart( V_BHAVA, h );
	addVedicChart( V_NAVAMSA, h );
	addVedicChart( V_TRIMSAMSA, h );
}



