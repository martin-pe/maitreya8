/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/GraphicalChart.h
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

#ifndef GRAPHICALCHART_H_
#define GRAPHICALCHART_H_

#include <wx/brush.h>
#include <wx/gdicmn.h>
#include <wx/pen.h>
#include <vector>

#include "GraphConfig.h"
#include "Painter.h"

class AspectExpert;
class ChartConfig;
class ChartProperties;
class Horoscope;
class UranianExpert;
class Writer;

using namespace std;

/*************************************************//**
*
*  
*
******************************************************/
class GraphicalChart
{
public:

	GraphicalChart( const bool &vedic, const ChartType&, ChartProperties*, Horoscope* = (Horoscope*)NULL, Horoscope* = (Horoscope*)NULL );
	virtual ~GraphicalChart();

	void paint( Painter*, const MRect&, const wxRect* = 0 );

	virtual void OnDataChanged();

	virtual void setHoroscopes( Horoscope*, Horoscope* );

	virtual bool OnResponsiveChartEvent( const MPoint p = MPoint( -1, -1 ), const int keyMod = 0) = 0;

	virtual bool handleMouseLeftEvent( int, int, wxRect& ) { return false; }

	ChartType getChartType() const { return charttype; }
	void changeChartType( const ChartType &ct )
	{
		ASSERT_VALID_CHARTTYPE( ct );
		charttype = ct;
	}

	virtual bool getPlanetRetro( const ObjectId &planet, const int &chart_id );

	virtual bool isPlanetBenefic( const ObjectId &planet, const int &chart_id );

	Horoscope *getHoroscope( const int &chart_id );

	AspectExpert *getAspectExpert() const { return aexpert; }

private:

	const bool vedic;

protected:

	ChartType charttype;
  ChartProperties *chartprops;
	Horoscope *h1, *h2;
	AspectExpert *aexpert;

	virtual void updateAspects() = 0;

	virtual void paintInternal( ChartConfig* ) = 0;

	virtual void setup() = 0;

	virtual ChartConfig *getChartConfig() const = 0;

	void drawChartFrameField( const double &a0, const double &a1, const GcChartFrame&, const GcChartFrame& );

	virtual void calculateObjectList();

	void paintChartFrame( const GcChartFrame& );
	void paintMarkerLine( const double &alfa, const GcChartFrame&, const GcChartFrame& );
	void paintDegreeMarkers( const double &aries,
		const GcChartFrame &zodiacFrame,
		const GcChartFrame &degree1Frame,
		const GcChartFrame &degree5Frame,
		const GcChartFrame &degree10Frame
		);

	MPoint frameBorderPointToXy( const double &alfa, const GcChartFrame& );
	void paintArrow( const double &alfa, GcArrow& );
	void paintArrow( const MPoint&, const MPoint& );

	void setDefaultTextColor( const FIELD_PART = FP_ALL );
	void setGraphicFont( const double zoom = 1.0 );
	void setSymbolFont( const double zoom = 1.0 );

	MRect rect;
	wxRect *refreshRect;
	double rmax;
	double rxmax, rymax;
	double xcenter, ycenter;

	double text_width, text_height;

	uint markedField;
	int markedFieldStatus;

	wxPen defaultPen;
	MBrush defaultBrush;

	ObjectArray obs;
	Painter *painter;
	bool h1set, h2set;
	double extrazoom;
	bool valid;

private:
	

	// used only here. Western/Vedic chart has its own configs
	ChartConfig *cfg;


	double painterzoom;
};

/*************************************************//**
*
*  
*
******************************************************/
class ChartFactory
{
public:
	GraphicalChart *createWesternChart( const ChartType&, ChartProperties*, Horoscope*, Horoscope* );
	GraphicalChart *createUranianChart( const ChartType&, ChartProperties*, Horoscope*, Horoscope*, UranianExpert* );
	GraphicalChart *createSarvatobhadraChart( const ChartType&, ChartProperties*, Horoscope*, Horoscope* );
	GraphicalChart *createVedicChart( const ChartType&, ChartProperties*, Horoscope*, Horoscope*, const Varga& );
};

#endif

