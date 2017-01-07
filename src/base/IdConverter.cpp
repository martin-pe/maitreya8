/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/IdConverter.cpp
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

#include "IdConverter.h"

#include "Commands.h"
#include "Conf.h"
#include "maitreya.h"
#include "ImageProvider.h"

#include <wx/log.h>

extern Config *config;

IMPLEMENT_SINGLETON( IdConverter )

/*****************************************************
**
**   IdConverter   ---   logMode2WxLogLevel
**
******************************************************/
int IdConverter::logMode2WxLogLevel( const int &mode )
{
	int level = wxLOG_Warning;
  switch( mode )
  {
    case 0:
      level = wxLOG_FatalError;
    break;
    case 1:
      level = wxLOG_Error;
    break;
    case 2:
      level = wxLOG_Warning;
    break;
    case 3:
      level = wxLOG_Message;
    break;
    case 4:
      level = wxLOG_Status;
    break;
    case 5:
      level = wxLOG_Info;
    break;
    case 6:
      level = wxLOG_Debug;
    break;
    default:
      level = wxLOG_Warning;
    break;
  }
	return level;
}

/*****************************************************
**
**   IdConverter   ---   viewId2BitmapId
**
******************************************************/
BitmapId IdConverter::viewId2BitmapId( const VIEW_ID &viewId, const bool vedic, const Varga varga )
{
	switch ( viewId )
	{
	case VIEW_TEXT:
		return BM_TEXT;
		break;
	case VIEW_TRANSIT:
		return BM_TRANSIT;
		break;
	case VIEW_SBC:
		return BM_SBC;
		break;
	case VIEW_PRINT:
		return BM_PRINT;
		break;
	case VIEW_SOLAR:
		return BM_SOLAR;
		break;
	case VIEW_VIMSOPAKABALA:
		return BM_SHADBALA;
		break;
	case VIEW_DASA_TREE:
	case VIEW_DASA_COMPOSITE:
		return BM_DASA;
		break;
	case VIEW_GRAPHICALDASA:
		return BM_GRAPHICALDASA;
		break;
	case VIEW_URANIAN:
		return BM_URANIAN;
		break;
	case VIEW_URANIAN_CHART:
		return BM_WCHART;
		break;
	case VIEW_GRAPHIC:
	{
		if ( vedic )
		{
			//printf( "VARGA in idconverter %d\n", (int)varga );
			//if ( (int)varga == -1 ) sleep( 10 );
			//ASSERT_VALID_VARGA( varga );
			if ( varga >= V_FIRST && varga <= V_LAST )
			{
				const int index =  (int)BM_CHART + (int)varga;
				return (BitmapId)index;
			}
			else return BM_CHART;
		}
		else
		{
			return BM_WCHART;
		}
	}
	break;
	case VIEW_PLANETLIST:
		return BM_SVIEW;
		break;
	case VIEW_HORA:
		return BM_HORA;
		break;
	case VIEW_ECLIPSE:
		return BM_ECLIPSE;
		break;
	case VIEW_PARTNER:
		return BM_PARTNER;
		break;
	case VIEW_EPHEM:
		return BM_EPHEM;
		break;
	case VIEW_YOGA:
		return BM_YOGA;
		break;
	case VIEW_GRAPHICGRID:
	case VIEW_VARGA:
		return BM_VARGA;
		break;
	case VIEW_ASHTAKAVARGA:
		return BM_ASHTAKAVARGA;
		break;
	case VIEW_SHADBALA:
		return BM_SHADBALA;
		break;
	case VIEW_YOGA_EDITOR:
		return BM_YOGAEDITOR;
		break;
	case VIEW_MULTIPLE:
		return BM_MVIEW;
		break;
	case VIEW_EMPTY:
		return BM_ERROR;
		break;
	default:
		printf( "ERROR IdConverter::viewId2BitmapId: wrong view id %d\n", (int)viewId );
		//assert( false );
		break;
	}

	return BM_ERROR;
}

/*****************************************************
**
**   IdConverter   ---   commandId2ViewId
**
******************************************************/
VIEW_ID IdConverter::commandId2ViewId( const int &command )
{
	VIEW_ID viewId = VIEW_EMPTY;

	switch ( command )
	{
	case CMD_NEW_TEXT:
		viewId = VIEW_TEXT;
		break;
	case CMD_NEW_TRANSIT:
		viewId = VIEW_TRANSIT;
		break;
	case CMD_NEW_SBC:
		viewId = VIEW_SBC;
		break;
	case CMD_NEW_SOLAR:
		viewId = VIEW_SOLAR;
		break;
	case CMD_NEW_PRINTPREVIEW:
		viewId = VIEW_PRINT;
		break;
	case CMD_NEW_DASA_TREE:
		viewId = VIEW_DASA_TREE;
		break;
	case CMD_NEW_DASA_COMPOSITE:
		viewId = VIEW_DASA_COMPOSITE;
		break;
	case CMD_NEW_GRAPHICALDASA:
		viewId = VIEW_GRAPHICALDASA;
		break;
	case CMD_NEW_MAINVIEW:
		viewId = VIEW_PLANETLIST;
		break;
	case CMD_NEW_YOGA:
		viewId = VIEW_YOGA;
		break;
	case CMD_NEW_GRAPHICGRID:
		viewId = VIEW_GRAPHICGRID;
		break;
	case CMD_NEW_VARGA:
		viewId = VIEW_VARGA;
		break;
	case CMD_NEW_SHADBALA:
		viewId = VIEW_SHADBALA;
		break;
	case CMD_NEW_ASHTAKAVARGA:
		viewId = VIEW_ASHTAKAVARGA;
		break;
	case CMD_NEW_VIMSOPAKABALA:
		viewId = VIEW_VIMSOPAKABALA;
		break;
	case CMD_NEW_EMPTY:
		viewId = VIEW_EMPTY;
		break;
	case APP_HORA:
		viewId = VIEW_HORA;
		break;
	case APP_EPHEMERIS:
		viewId = VIEW_EPHEM;
		break;
	case APP_ECLIPSE:
		viewId = VIEW_ECLIPSE;
		break;
	case APP_PARTNER:
		viewId = VIEW_PARTNER;
		break;
	case APP_YOGAEDITOR:
		viewId = VIEW_YOGA_EDITOR;
		break;
	case CMD_NEW_WCHART:
		viewId = VIEW_GRAPHIC;
		break;
	case CMD_NEW_URANIAN:
		viewId = VIEW_URANIAN;
		break;
	case CMD_NEW_URANIAN_CHART:
		viewId = VIEW_URANIAN_CHART;
		break;
	default:
		if ( command >= CMD_NEW_RASI && command <= CMD_NEW_RASI+30 )
		{
			viewId = VIEW_GRAPHIC;
		}
	}
	return viewId;
}

/**************************************************************
***
**    IdConverter   ---   getSizeForViewId
***
***************************************************************/
wxSize IdConverter::getSizeForViewId( const VIEW_ID &viewId, const bool &vedic )
{
	switch ( viewId )
	{
	case VIEW_PLANETLIST:
		return config->viewprefs->sizes.sMainWindow;
		break;
	case VIEW_GRAPHIC:
		return ( vedic ? config->viewprefs->sizes.sVedicGraphicWindow : config->viewprefs->sizes.sWesternGraphicWindow );
		break;
	case VIEW_TEXT:
		return config->viewprefs->sizes.sTextWindow;
		break;
	case VIEW_TRANSIT:
		return config->viewprefs->sizes.sTransitWindow;
		break;
	case VIEW_SBC:
		return config->viewprefs->sizes.sSbcWindow;
		break;
	case VIEW_SOLAR:
		return config->viewprefs->sizes.sSolarWindow;
		break;
	case VIEW_PRINT:
		return config->viewprefs->sizes.sPrintPreviewWindow;
		break;
	case VIEW_GRAPHICALDASA:
		return config->viewprefs->sizes.sGraphicalDasaWindow;
		break;
	case VIEW_ASHTAKAVARGA:
		return config->viewprefs->sizes.sAshtakaVargaWindow;
		break;
	case VIEW_DASA_TREE:
		return config->viewprefs->sizes.sDasaTreeWindow;
		break;
	case VIEW_DASA_COMPOSITE:
		return config->viewprefs->sizes.sDasaCompositeWindow;
		break;
	case VIEW_URANIAN_CHART:
		return config->viewprefs->sizes.sUranianChartWindow;
		break;
	case VIEW_URANIAN:
		return config->viewprefs->sizes.sUranianWindow;
		break;
	case VIEW_HORA:
		return config->viewprefs->sizes.sHoraWindow;
		break;
	case VIEW_ECLIPSE:
		return config->viewprefs->sizes.sEclipseWindow;
		break;
	case VIEW_EPHEM:
		return config->viewprefs->sizes.sEphemWindow;
		break;
	case VIEW_PARTNER:
		return config->viewprefs->sizes.sPartnerWindow;
		break;
	case VIEW_YOGA:
		return config->viewprefs->sizes.sYogaWindow;
		break;
	case VIEW_VARGA:
		return config->viewprefs->sizes.sVargaSummaryWindow;
		break;
	case VIEW_SHADBALA:
		return config->viewprefs->sizes.sShadbalaWindow;
		break;
	case VIEW_YOGA_EDITOR:
		return config->viewprefs->sizes.sYogaEditor;
		break;
	case VIEW_MULTIPLE:
		return config->viewprefs->sizes.sMultipleView;
		break;
	case VIEW_EMPTY:
		return wxSize( 100, 100 );
		break;
	default:
		printf( "Error: wrong view id %d in getSizeForViewId\n", viewId );
		assert( false );
		break;
	}
}

/**************************************************************
***
**    IdConverter   ---   setConfigSizeForViewId
***
***************************************************************/
void IdConverter::setConfigSizeForViewId( const VIEW_ID &viewId, const wxSize &size, const bool &vedic )
{
	switch ( viewId )
	{
	case VIEW_PLANETLIST:
		config->viewprefs->sizes.sMainWindow = size;
		break;
	case VIEW_GRAPHIC:
		if ( vedic )
		{
			config->viewprefs->sizes.sVedicGraphicWindow = size;
		}
		else
		{
			config->viewprefs->sizes.sWesternGraphicWindow = size;
		}
		break;
	case VIEW_TEXT:
		config->viewprefs->sizes.sTextWindow = size;
		break;
	case VIEW_DASA_TREE:
		config->viewprefs->sizes.sDasaTreeWindow = size;
		break;
	case VIEW_DASA_COMPOSITE:
		config->viewprefs->sizes.sDasaCompositeWindow = size;
		break;
	case VIEW_GRAPHICALDASA:
		config->viewprefs->sizes.sGraphicalDasaWindow = size;
		break;
	case VIEW_TRANSIT:
		config->viewprefs->sizes.sTransitWindow = size;
		break;
	case VIEW_SBC:
		config->viewprefs->sizes.sSbcWindow = size;
		break;
	case VIEW_SOLAR:
		config->viewprefs->sizes.sSolarWindow = size;
		break;
	case VIEW_PRINT:
		config->viewprefs->sizes.sPrintPreviewWindow = size;
		break;
	case VIEW_URANIAN_CHART:
		config->viewprefs->sizes.sUranianChartWindow = size;
		break;
	case VIEW_URANIAN:
		config->viewprefs->sizes.sUranianWindow = size;
		break;
	case VIEW_HORA:
		config->viewprefs->sizes.sHoraWindow = size;
		break;
	case VIEW_ECLIPSE:
		config->viewprefs->sizes.sEclipseWindow = size;
		break;
	case VIEW_PARTNER:
		config->viewprefs->sizes.sPartnerWindow = size;
		break;
	case VIEW_EPHEM:
		config->viewprefs->sizes.sEphemWindow = size;
		break;
	case VIEW_YOGA:
		config->viewprefs->sizes.sYogaWindow = size;
		break;
	case VIEW_VARGA:
		config->viewprefs->sizes.sVargaSummaryWindow = size;
		break;
	case VIEW_SHADBALA:
		config->viewprefs->sizes.sShadbalaWindow = size;
		break;
	case VIEW_ASHTAKAVARGA:
		config->viewprefs->sizes.sAshtakaVargaWindow = size;
		break;
	case VIEW_YOGA_EDITOR:
		config->viewprefs->sizes.sYogaEditor = size;
		break;
	case VIEW_MULTIPLE:
		config->viewprefs->sizes.sMultipleView = size;
		break;
	case VIEW_EMPTY:
		break;
	default:
		printf( "Error: wrong view id %d in setConfigSizeForViewId\n", viewId );
		assert( false );
		break;
	}
}


