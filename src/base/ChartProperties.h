/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ChartProperties.h
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

#ifndef _CHARTPROPERTIES_H_
#define _CHARTPROPERTIES_H_

#include <vector>

#include "Conf.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

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
	void setObjectStyle( const OBJECT_INCLUDES &style )        { setObjectStyle( style, isVedic() ); }

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
	int getSkin() const { return isVedic() ? vgraphicstyle.graphicSkin : wgraphicstyle.graphicSkin; }

	void setVedicSkin( const int &skin )   { vgraphicstyle.graphicSkin = skin ; }
	void setWesternSkin( const int &skin )   { wgraphicstyle.graphicSkin = skin ; }

	VedicChartBehaviorConfig &getVedicChartBehaviorConfig() { return vchartconfig; }
	void setVedicChartBehaviorConfig( const VedicChartBehaviorConfig &cfg ) { vchartconfig = cfg; }

	WesternChartBehaviorConfig &getWesternChartBehaviorConfig() { return wchartconfig; }
	void setWesternChartBehaviorConfig( const WesternChartBehaviorConfig &cfg ) { wchartconfig = cfg; }

	void changeSkin( const bool &increment, const bool &vedic );
	void changeSkin( const bool &increment ) { changeSkin( increment, isVedic() ); }
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

	UranianConfig &getUranianConfig() { return uconfig; }
	void setUranianConfig( const UranianConfig &ucfg ) { uconfig = ucfg; }

	bool isAnimated() const { return animated; }
	void setAnimated( const bool &b ) { animated = b; }

	bool hasDocument() const { return isDocumentProp; }
	void setHasDocument( const bool &b ) { isDocumentProp = b; }

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

	OBJECT_INCLUDES vobjectstyle, wobjectstyle;
	VedicChartDisplayConfig vgraphicstyle;
	VedicChartBehaviorConfig vchartconfig;

	WesternChartDisplayConfig wgraphicstyle;
	WesternChartBehaviorConfig wchartconfig;

	TAB_COLUMN_TYPE vcolumnstyle, wcolumnstyle;
	ObjectArray vobjects, wobjects;
	ObjectFilter filter;

	UranianConfig uconfig;
};


#endif

