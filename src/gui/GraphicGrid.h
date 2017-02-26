/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/GraphicGrid.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2017 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
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

	void addAshtakavargaChart();

	void addWesternChart( Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );

	AspectExpert *getAspectExpert() const;

	void setHoroscopes( Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );
	void deleteCharts();

	virtual void OnDataChanged();
	void OnEWToggle( const bool &vedic );

	ChartType getChartType() const { return charttype; }
	void changeChartType( const ChartType& );

protected:

	ChartType charttype;
	const uint rows, cols;

	vector <GraphicalChart*> charts;
	GraphicalChart *wchart;

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

