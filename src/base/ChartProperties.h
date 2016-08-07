/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ChartProperties.h
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

#ifndef _CHARTPROPERTIES_H_
#define _CHARTPROPERTIES_H_

#include <vector>

#include "Conf.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

using namespace std;

/*************************************************//**
*
* \brief contains all common config properties for a chart
*
******************************************************/
class ChartProperties
{
public:

	ChartProperties( const bool readFromConfig = true );
	virtual ~ChartProperties() {}

	// Vedic - western
	void setVedic( const bool = true );
	void setFixedVedic();
	void setFixedWestern();

	virtual bool isVedic() const;
	bool isFixedVedic()   const { return fixedVedic; }
	bool isFixedWestern() const { return fixedWestern; }
	bool isEwFixed() const { return fixedVedic || fixedWestern; }
	void freezeEwStatus();

	// supported objects
	OBJECT_INCLUDES getVedicObjectStyle() const { return vobjectstyle; }
	OBJECT_INCLUDES getWesternObjectStyle() const { return wobjectstyle; }
	OBJECT_INCLUDES getObjectStyle() const { return isVedic() ? vobjectstyle : wobjectstyle; }

	void setVedicObjectStyle( const OBJECT_INCLUDES &style )   { setObjectStyle( style, true ); }
	void setWesternObjectStyle( const OBJECT_INCLUDES &style ) { setObjectStyle( style, false ); }
	void setObjectStyle( const OBJECT_INCLUDES &style )        { setObjectStyle( style, vedic ); }

	// graphic styles
	VedicChartDisplayConfig &getVedicChartDisplayConfig() { return vgraphicstyle; }
	WesternChartDisplayConfig &getWesternChartDisplayConfig() { return wgraphicstyle; }

	void setVedicChartDisplayConfig( const VedicChartDisplayConfig &style )   { vgraphicstyle = style; }
	void setWesternChartDisplayConfig( const WesternChartDisplayConfig &style ) { wgraphicstyle = style; }

	// main view styles
	TAB_COLUMN_TYPE getVedicColumnStyle() const { return vcolumnstyle; }
	TAB_COLUMN_TYPE getWesternColumnStyle() const { return wcolumnstyle; }
	TAB_COLUMN_TYPE getColumnStyle() const { return isVedic() ? vcolumnstyle : wcolumnstyle; }

	void setVedicColumnStyle( const TAB_COLUMN_TYPE &style )   { vcolumnstyle = style; }
	void setWesternColumnStyle( const TAB_COLUMN_TYPE &style ) { wcolumnstyle = style; }

	// skin for graphical charts
	int getVedicSkin() const { return vgraphicstyle.graphicSkin; }
	int getWesternSkin() const { return wgraphicstyle.graphicSkin; }
	int getSkin() const { return vedic ? vgraphicstyle.graphicSkin : wgraphicstyle.graphicSkin; }

	void setVedicSkin( const int &skin )   { vgraphicstyle.graphicSkin = skin ; }
	void setWesternSkin( const int &skin )   { wgraphicstyle.graphicSkin = skin ; }

	VedicChartBehaviorConfig &getVedicChartBehaviorConfig() { return vchartconfig; }
	void setVedicChartBehaviorConfig( const VedicChartBehaviorConfig &cfg ) { vchartconfig = cfg; }

	WesternChartBehaviorConfig &getWesternChartBehaviorConfig() { return wchartconfig; }
	void setWesternChartBehaviorConfig( const WesternChartBehaviorConfig &cfg ) { wchartconfig = cfg; }

	void changeSkin( const bool &increment, const bool &vedic );
	void changeSkin( const bool &increment ) { changeSkin( increment, vedic ); }
	void processNavigationKey( const int& );

	// planet lists
	const ObjectArray getPlanetList( const OBJECT_INCLUDES excludes = OI_NONE ) const;
	const ObjectArray getVedicPlanetList( const OBJECT_INCLUDES excludes = OI_NONE ) const;
	const ObjectArray getWesternPlanetList( const OBJECT_INCLUDES excludes = OI_NONE) const;

	const ObjectArray getPlanetList( const ObjectFilter& ) const;
	const ObjectArray getVedicPlanetList( const ObjectFilter& ) const;
	const ObjectArray getWesternPlanetList( const ObjectFilter& ) const;

	void setObjectFilter( const ObjectFilter &f ) { filter = f; }
	ObjectFilter getObjectFilter() const { return filter; }
	bool isFiltered() const { return filter.size() > 0; }

	bool isAnimated() const { return animated; }
	void setAnimated( const bool &b ) { animated = b; }

	bool hasDocument() const { return isDocumentProp; }
	void setHasDocument( const bool &b ) { isDocumentProp = b; }

	bool isTransitmode() const { return transitmode; }
	void setTransitmode( const bool &b ) { transitmode = b; }

	bool dispatchWidgetPropertyCommand( const int& );

	void dump();

protected:

private:

	void init();
	void setObjectStyle( const OBJECT_INCLUDES&, const bool &vedic );
	void updatePlanetList( const bool& );

	bool vedic;
	bool fixedVedic;
	bool fixedWestern;

	bool animated;
	bool isDocumentProp;
	bool transitmode;

	OBJECT_INCLUDES vobjectstyle, wobjectstyle;
	VedicChartDisplayConfig vgraphicstyle;
	VedicChartBehaviorConfig vchartconfig;

	WesternChartDisplayConfig wgraphicstyle;
	WesternChartBehaviorConfig wchartconfig;

	TAB_COLUMN_TYPE vcolumnstyle, wcolumnstyle;
	ObjectArray vobjects, wobjects;
	ObjectFilter filter;
};


#endif

