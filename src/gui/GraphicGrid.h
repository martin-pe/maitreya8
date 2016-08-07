/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/GraphicGrid.h
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

#ifndef _GRAPHICGRID_H_
#define _GRAPHICGRID_H_

#include <vector>
#include "BasicWidget.h"

using namespace std;

class AspectExpert;
class BasicWesternChart;
class ChartProperties;
class GraphicalChart;
class Horoscope;
class Painter;
class UranianExpert;

/*************************************************//**
*
* \brief Complex graphical chart grid with n:m rows and columns of Vedic Varga charts/alternativly one Western chart
*
******************************************************/
class ChartGridWidget : public BasicWidget
{
	DECLARE_CLASS( ChartGridWidget )
public:

	ChartGridWidget( wxWindow *parent, const ChartType&, ChartProperties*, const int rows = 1, const int cols = 1, const wxWindowID id = -1 );
	~ChartGridWidget();

	void addVedicChart( Varga = V_RASI, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	void addSarvatobhadraChart( Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	void addWesternChart( Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	AspectExpert *getAspectExpert() const;

#ifdef USE_URANIAN_CHART
	void addUranianChart( UranianExpert*, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );
#endif

	void setHoroscopes( Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );
	void deleteCharts();

	virtual void OnDataChanged();
	void OnEWToggle( const bool &vedic );

	ChartType getChartType() const { return charttype; }
	void changeChartType( const ChartType& );

	virtual int getWidgetOptions();

protected:

	ChartType charttype;
	const uint rows, cols;

	vector <GraphicalChart*> charts;
	GraphicalChart *wchart;

	UranianExpert *uexpert;

	virtual void mouseHasMoved( const bool& );
	virtual void doPaint( const wxRect&, const bool eraseBackground = true );
	uint findChartForCurrentScreenPos();
	wxRect getRectForChartIndex( const uint& id );

private:
	uint lastMouseOverChart;
	void OnSize( wxSizeEvent& );
	void OnMouseLeftDown( wxMouseEvent& );
};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 6 Shadvarga charts (D-1, D-2, D-3, D-9, D-12, D-30)
*
******************************************************/
class ShadVargaChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( ShadVargaChartGridWidget )

public:
	ShadVargaChartGridWidget( wxWindow *parent, const ChartType&, ChartProperties*,
		Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 6 Varga charts for 2nd tab of VargaView (D-4, D-7, D-10, D-16, D-20, D-24)
*
******************************************************/
class VargaTab2ChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( VargaTab2ChartGridWidget )

public:
	VargaTab2ChartGridWidget( wxWindow *parent, const ChartType&, ChartProperties*,
		Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 6 Varga charts for 3rd tab of VargaView (D-6, D-8, D-27, D-40, D-45, D-60)
*
******************************************************/
class VargaTab3ChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( VargaTab3ChartGridWidget )

public:
	VargaTab3ChartGridWidget( wxWindow *parent, const ChartType&, ChartProperties*,
		Horoscope* = (Horoscope*)NULL, Horoscope* =  (Horoscope*)NULL );

};

/*************************************************//**
*
* \brief Convenience class for ChartGridWidget with 4 Vedic charts (D-1, Bhava, D-9, D-30)
*
******************************************************/
class DefaultEwChartGridWidget : public ChartGridWidget
{
	DECLARE_CLASS( DefaultEwChartGridWidget )
public:
	DefaultEwChartGridWidget( wxWindow *parent, const ChartType&, ChartProperties*, Horoscope* = (Horoscope*)NULL );
};

#endif

