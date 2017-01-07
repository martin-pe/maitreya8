/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ChartProperties.cpp
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

#include "ChartProperties.h"

#include "Commands.h"
#include "Conf.h"
#include "maitreya.h"
#include "PlanetList.h"

extern Config *config;

/*****************************************************
**
**   ChartProperties   ---   Constructor
**
******************************************************/
ChartProperties::ChartProperties( const bool readFromConfig )
{
	fixedVedic = fixedWestern = false;
	animated = false;
	isDocumentProp = false;

	if ( readFromConfig ) init();
}

/*****************************************************
**
**   ChartProperties   ---   init
**
******************************************************/
void ChartProperties::init()
{
	vedic = config->preferVedic;

	vobjectstyle = config->vedic->objects;
	wobjectstyle = config->western->objects;

	vgraphicstyle = *config->vedicChart;
	wgraphicstyle = *config->westernChart;

	vcolumnstyle = config->vedic->columnStyle;
	wcolumnstyle = config->western->columnStyle;

	vobjects = PlanetList().getVedicObjectList( vobjectstyle );
	wobjects = PlanetList().getWesternObjectList( wobjectstyle );

	vchartconfig = *config->vedicChartBehavior;
	wchartconfig = *config->westernChartBehavior;

	uconfig = *config->uranian;
}

/*****************************************************
**
**   ChartProperties   ---   setVedic
**
******************************************************/
void ChartProperties::setVedic( const bool b  )
{
	vedic = b;
}

/*****************************************************
**
**   ChartProperties   ---   setFixedVedic
**
******************************************************/
void ChartProperties::setFixedVedic()
{
	vedic = fixedVedic = true;
	fixedWestern = false;
}

/*****************************************************
**
**   ChartProperties   ---   setFixedWestern
**
******************************************************/
void ChartProperties::setFixedWestern()
{
	vedic = fixedVedic = false;
	fixedWestern =  true;
}

/*****************************************************
**
**   ChartProperties   ---   freezeEwStatus
**
******************************************************/
void ChartProperties::freezeEwStatus() 
{
	if ( vedic ) setFixedVedic();
	else setFixedWestern();
}

/*****************************************************
**
**   ChartProperties   ---   isVedic
**
******************************************************/
bool ChartProperties::isVedic() const
{
	//static int count = 0;
	//printf( "ChartProperties::isVedic %d vedic %d fixedVedic %d fixedWestern %d\n", count++, vedic, fixedVedic, fixedWestern );
	if ( fixedVedic ) return true;
	if ( fixedWestern ) return false;
	return vedic;
}

/*****************************************************
**
**   ChartProperties   ---   getPlanetList
**
******************************************************/
const ObjectArray ChartProperties::getPlanetList( const OBJECT_INCLUDES excludes ) const
{
	return isVedic() ? getVedicPlanetList( excludes ) : getWesternPlanetList( excludes );
}

/*****************************************************
**
**   ChartProperties   ---   getPlanetList
**
******************************************************/
const ObjectArray ChartProperties::getPlanetList( const ObjectFilter &filter ) const
{
	return isVedic() ? getVedicPlanetList( filter ) : getWesternPlanetList( filter );
}

/*****************************************************
**
**   ChartProperties   ---   getVedicPlanetList
**
******************************************************/
const ObjectArray ChartProperties::getVedicPlanetList( const OBJECT_INCLUDES excludes ) const
{
	if ( excludes ) return PlanetList().getVedicObjectList( vobjectstyle & ~excludes );
	else return vobjects;
}

/*****************************************************
**
**   ChartProperties   ---   getVedicPlanetList
**
******************************************************/
const ObjectArray ChartProperties::getVedicPlanetList( const ObjectFilter &filter ) const
{
	if ( filter.size() == 0 ) return vobjects;

	ObjectArray a;
	for ( uint i = 0; i <= vobjects.size(); i++ )
	{
		if ( filter.find( vobjects[i] ) != filter.end() ) a.push_back( vobjects[i] );
	}
	return a;
}

/*****************************************************
**
**   ChartProperties   ---   getWesternPlanetList
**
******************************************************/
const ObjectArray ChartProperties::getWesternPlanetList( const OBJECT_INCLUDES excludes ) const
{
	if ( excludes ) return PlanetList().getWesternObjectList( wobjectstyle & ~excludes );
	else return wobjects;
}

/*****************************************************
**
**   ChartProperties   ---   getWesternPlanetList
**
******************************************************/
const ObjectArray ChartProperties::getWesternPlanetList( const ObjectFilter &filter ) const
{
	if ( filter.size() == 0 ) return wobjects;

	ObjectArray a;
	for ( uint i = 0; i <= wobjects.size(); i++ )
	{
		if ( filter.find( wobjects[i] ) != filter.end() ) a.push_back( wobjects[i] );
	}
	return a;
}

/*****************************************************
**
**   ChartProperties   ---   setObjectStyle
**
******************************************************/
void ChartProperties::setObjectStyle( const OBJECT_INCLUDES &style, const bool &vedic )
{
	vedic ? vobjectstyle = style : wobjectstyle = style;
	updatePlanetList( vedic );
}

/*****************************************************
**
**   ChartProperties   ---   updatePlanetList
**
******************************************************/
void ChartProperties::updatePlanetList( const bool &v )
{
	v	?
	vobjects = PlanetList().getVedicObjectList( vobjectstyle )
	           : wobjects = PlanetList().getWesternObjectList( wobjectstyle );
}

/*****************************************************
**
**   ChartProperties   ---   changeSkin
**
******************************************************/
void ChartProperties::changeSkin( const bool &increment, const bool &v )
{
	//printf( "BEFORE vgraphicstyle.graphicSkin %d vedic %d\n", vgraphicstyle.graphicSkin, v );
	int skin = v ? vgraphicstyle.graphicSkin : wgraphicstyle.graphicSkin;
	if ( increment ) skin--;
	else skin++;
	const int size = v ? (int)VedicChartConfigLoader::get()->getConfigs().size()
		: (int)WesternChartConfigLoader::get()->getConfigs().size();
	if ( skin < 0 ) skin = size - 1;
	if ( skin >= size ) skin = 0;
	v ? vgraphicstyle.graphicSkin = skin : wgraphicstyle.graphicSkin = skin;
	//printf( "AFTER vgraphicstyle.graphicSkin %d\n", vgraphicstyle.graphicSkin );
}

/*****************************************************
**
**   ChartProperties   ---   processNavigationKey
**
******************************************************/
void ChartProperties::processNavigationKey( const int &code )
{
	//printf( "ChartProperties::processNavigationKey code %d\n", code );
	switch( code )
	{
		case WXK_UP:
		case WXK_DOWN:
			changeSkin( code == WXK_UP, isVedic() );
			break;
		case WXK_LEFT:
		case WXK_RIGHT:
			if ( isVedic() )
			{
				if ( code == WXK_RIGHT ) vgraphicstyle.indianChartType++;
				else vgraphicstyle.indianChartType--;
				if ( vgraphicstyle.indianChartType < 0 ) vgraphicstyle.indianChartType = VCT_EAST;
				if ( vgraphicstyle.indianChartType > 2 ) vgraphicstyle.indianChartType = VCT_SOUTH;
			}
			else
			{
				if ( code == WXK_RIGHT ) wgraphicstyle.chartOrientation++;
				else wgraphicstyle.chartOrientation--;
				if ( wgraphicstyle.chartOrientation < 0 ) wgraphicstyle.chartOrientation = 10;
				if ( wgraphicstyle.chartOrientation > 10 ) wgraphicstyle.chartOrientation = 0;
			}
			break;
		case WXK_PAGEDOWN:
		case WXK_PAGEUP:
			break;
		default:
			assert( false );
			break;
	}
}

/*****************************************************
**
**   ChartProperties   ---   dispatchWidgetPropertyCommand
**
******************************************************/
bool ChartProperties::dispatchWidgetPropertyCommand( const int &command )
{
	if ( command >= CMD_GRAPHIC_STYLE && command < CMD_GRAPHIC_STYLE + 100 )
	{
		if ( isVedic() ) vgraphicstyle.graphicSkin = command - CMD_GRAPHIC_STYLE;
		else wgraphicstyle.graphicSkin = command - CMD_GRAPHIC_STYLE;
	}
	else switch ( command )
		{
		case CMD_WESTERNMODE:
			vedic = false;
			break;
		case CMD_VEDICMODE:
			vedic = true;
			break;

		case CMD_KEY_LEFT:
		case CMD_KEY_RIGHT:
			if ( isVedic() )
			{
				if ( command == CMD_KEY_RIGHT ) vgraphicstyle.indianChartType++;
				else vgraphicstyle.indianChartType--;
				if ( vgraphicstyle.indianChartType < 0 ) vgraphicstyle.indianChartType = VCT_EAST;
				if ( vgraphicstyle.indianChartType > 2 ) vgraphicstyle.indianChartType = VCT_SOUTH;
			}
			else
			{
				if ( command == CMD_KEY_RIGHT ) wgraphicstyle.chartOrientation++;
				else wgraphicstyle.chartOrientation--;
				if ( wgraphicstyle.chartOrientation < 0 ) wgraphicstyle.chartOrientation = 10;
				if ( wgraphicstyle.chartOrientation > 10 ) wgraphicstyle.chartOrientation = 0;
			}
		break;
		case CMD_KEY_PAGE_DOWN:
		case CMD_KEY_PAGE_UP:
			processNavigationKey( command );
		break;

		case CMD_KEY_ARROW_UP:
			changeSkin( true );
		break;
		case CMD_KEY_ARROW_DOWN:
			changeSkin( false );
		break;

		case CMD_WCS_HOUSES:
			wgraphicstyle.showHouses = ! wgraphicstyle.showHouses;
		break;

		case CMD_WCS_ASPECTS:
			wgraphicstyle.showAspects = ! wgraphicstyle.showAspects;
		break;

		case CMD_WCS_RETRO:
			wgraphicstyle.showRetro = ! wgraphicstyle.showRetro;
		break;

		case CMD_WCC_PLANETS:
			wgraphicstyle.showPlanetColors = ! wgraphicstyle.showPlanetColors;
		break;

		case CMD_WCC_SIGNS:
			wgraphicstyle.showSignColors = ! wgraphicstyle.showSignColors;
		break;

		case CMD_WCC_HOUSES:
			wgraphicstyle.showHouseColors = !  wgraphicstyle.showHouseColors;
		break;

		case CMD_WCC_ASPECTS:
			wgraphicstyle.showAspectColors = !  wgraphicstyle.showAspectColors;
		break;

		case CMD_WCS_ASPECTSYMBOLS:
			wgraphicstyle.showAspectSymbols = ! wgraphicstyle.showAspectSymbols;
		break;

		case CMD_WCS_SECONDCHART_INSIDE:
			wgraphicstyle.secondchartStyle = ! wgraphicstyle.secondchartStyle;
		break;

		case CMD_VCT_SOUTH:
			vgraphicstyle.indianChartType = VCT_SOUTH;
		break;
		case CMD_VCT_NORTH:
			vgraphicstyle.indianChartType = VCT_NORTH;
		break;
		case CMD_VCT_EAST:
			vgraphicstyle.indianChartType = VCT_EAST;
		break;

		case CMD_VCC_NOTHING:
			vgraphicstyle.centerInfoType = VCC_NOTHING;
		break;
		case CMD_VCC_NAME:
			vgraphicstyle.centerInfoType = VCC_NAME;
		break;
		case CMD_VCC_NAME_SIGNIFICATION:
			vgraphicstyle.centerInfoType = VCC_NAME_SIGNIFICATION;
		break;
		case CMD_VCC_DIVISION:
			vgraphicstyle.centerInfoType = VCC_DIVISION;
		break;
		case CMD_VCC_DIVISION_SIGNIFICATION:
			vgraphicstyle.centerInfoType = VCC_DIVISION_SIGNIFICATION;
		break;

		case CMD_VCN_ASC:
			vgraphicstyle.northIndianSignDisplayType = VCN_ASC;
		break;
		case CMD_VCN_NUMBER:
			vgraphicstyle.northIndianSignDisplayType = VCN_NUMBER;
		break;
		case CMD_VCN_SHORT:
			vgraphicstyle.northIndianSignDisplayType = VCN_SHORT;
		break;
		case CMD_VCN_SYMBOL:
			vgraphicstyle.northIndianSignDisplayType = VCN_SYMBOL;
		break;

		case CMD_VCS_ARUDHAS:
			vgraphicstyle.showArudhas = !vgraphicstyle.showArudhas;
		break;
		case CMD_VCS_ASHTAKAVARGA:
			vgraphicstyle.showAshtakavarga = !vgraphicstyle.showAshtakavarga;
		break;
		case CMD_VCS_RETRO:
			vgraphicstyle.showRetro = !vgraphicstyle.showRetro;
		break;

		case CMD_VCS_PLANETCOLORS:
			vgraphicstyle.showPlanetColors = !vgraphicstyle.showPlanetColors;
		break;
		case CMD_VCS_SANSKRITSYMBOLS:
			vgraphicstyle.showSbcSanskritSymbols = !vgraphicstyle.showSbcSanskritSymbols;
		break;
		case CMD_VCS_SBC_NAKSHATRA_QUALITY:
			vgraphicstyle.showSbcNakshatraQuality = !vgraphicstyle.showSbcNakshatraQuality;
		break;

#define SETOBJECTFLAG( a, c ) \
			case a: \
				if ( isVedic() ) \
				{ \
					vobjectstyle & c ? vobjectstyle &= ~c : vobjectstyle |= c; \
				} \
				else \
				{ \
					wobjectstyle & c ? wobjectstyle &= ~c : wobjectstyle |= c; \
				} \
			break;
			SETOBJECTFLAG( CMD_SHOWOUTER, OI_OUTER )
			SETOBJECTFLAG( CMD_SHOWDRAGONHEAD, OI_DRAGONHEAD )
			SETOBJECTFLAG( CMD_SHOWDRAGONTAIL, OI_DRAGONTAIL )
			SETOBJECTFLAG( CMD_SHOWASCENDANT, OI_ASCENDANT )
			SETOBJECTFLAG( CMD_SHOWMERIDIAN, OI_MERIDIAN )
			SETOBJECTFLAG( CMD_SHOWDESCENDANT, OI_DESCENDANT )
			SETOBJECTFLAG( CMD_SHOWIMUMCOELI, OI_IMUMCOELI )
			SETOBJECTFLAG( CMD_SHOWURANIAN_INNER, OI_URANIAN_INNER )
			SETOBJECTFLAG( CMD_SHOWURANIAN_OUTER, OI_URANIAN_OUTER )
			SETOBJECTFLAG( CMD_SHOWCHIRON, OI_CHIRON )
			SETOBJECTFLAG( CMD_SHOWPHOLUS, OI_PHOLUS )
			SETOBJECTFLAG( CMD_SHOWPLANETOIDS, OI_PLANETOIDS )
			SETOBJECTFLAG( CMD_SHOWLILITH, OI_LILITH )
			SETOBJECTFLAG( CMD_SHOWUPAGRAHAS, OI_UPAGRAHAS )
			SETOBJECTFLAG( CMD_SHOWKALAVELAS, OI_KALAVELAS )
			SETOBJECTFLAG( CMD_SHOWSPECIALLAGNAS, OI_SPECIALLAGNAS )
			SETOBJECTFLAG( CMD_SHOWARIES, OI_ARIES )
			SETOBJECTFLAG( CMD_SHOWD9LAGNA, OI_D9_LAGNA )
			SETOBJECTFLAG( CMD_SHOWARABICPARTS, OI_ARABICPARTS )
			SETOBJECTFLAG( CMD_SHOW_HOUSES, OI_ALL_HOUSES )
			SETOBJECTFLAG( CMD_SHOW_4HOUSES, OI_4_HOUSES )
#undef SETOBJECTFLAG

#define SETMAINVIEWFLAG( a, c ) \
			case a: \
				if ( isVedic() ) vcolumnstyle & c ? vcolumnstyle &= ~c : vcolumnstyle |= c; \
				else wcolumnstyle & c ? wcolumnstyle &= ~c : wcolumnstyle |= c; \
			break;

			SETMAINVIEWFLAG( CMD_MAIN_SHOW_LORD, TAB_CT_SIGN_LORD );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_DIGNITY, TAB_CT_DIGNITY );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_NAVAMSA, TAB_CT_NAVAMSA );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_KARAKA, TAB_CT_CHARA_KARAKA );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_SHASTIAMSA, TAB_CT_SHASTIAMSA_LORD );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_Nakshatra, TAB_CT_NAKSHATRA );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_DASAVARGA, TAB_CT_DASA_VARGA );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_HOUSEPOS, TAB_CT_HOUSE_POSITION );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_KP, TAB_CT_KP_LORDS );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_ASHTAKA, TAB_CT_AV_REKHAPOINTS );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_PADA, TAB_CT_NAKSHATRA_PADA );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_DECLINATION, TAB_CT_LATITUDE );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_90_DEGREE_LONGITUDE, TAB_CT_90_DEGREE_LONGITUDE );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_45_DEGREE_LONGITUDE, TAB_CT_45_DEGREE_LONGITUDE );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_ANTISCIA, TAB_CT_ANTISCIA );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_ELEMENT, TAB_CT_ELEMENT );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_SIGNIFICATION, TAB_CT_SIGNIFICATION );
			SETMAINVIEWFLAG( CMD_MAIN_SHOW_SIGN_QUALITY, TAB_CT_SIGN_QUALITY );

#undef SETMAINVIEWFLAG

			// dummy
		case 0:
			break;
		default:
			return false;
			break;
		}
	updatePlanetList( isVedic() );
	return true;
}

/*****************************************************
**
**   ChartProperties   ---   dump
**
******************************************************/
void ChartProperties::dump()
{
	uint i;
	printf( "Chart Properties: vedic %d\n", isVedic() );
	printf( " vobjectstyle %d wobjectstyle %d\n",  vobjectstyle, wobjectstyle );
	//printf( " vgraphicstyle %d wgraphicstyle %d\n", vgraphicstyle, wgraphicstyle );
	printf( " vcolumnstyle %d wcolumnstyle %d\n",  vcolumnstyle, wcolumnstyle );
	printf( " vskin %d wskin %d\n", vgraphicstyle.graphicSkin, wgraphicstyle.graphicSkin );

	printf( " Vedic Objects: " );
	for ( i = 0; i < vobjects.size(); i++ ) printf( "%d ", vobjects[i] );
	printf( "\n" );

	printf( " Western Objects: " );
	for ( i = 0; i < wobjects.size(); i++ ) printf( "%d ", wobjects[i] );
	printf( "\n" );
}


