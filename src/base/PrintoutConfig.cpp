/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PrintoutConfig.cpp
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

#include "PrintoutConfig.h"

#include "Conf.h"
#include "maitreya.h"
#include "FileConfig.h"
#include "JSonTool.h"
#include "mathbase.h"
#include "VargaBase.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

#include <math.h>
#include <wx/jsonval.h>
#include <wx/log.h>

extern Config *config;

/*****************************************************
**
**   PdfDocumentConfig   ---   Constructor
**
******************************************************/
PdfDocumentConfig::PdfDocumentConfig()
{
	saveFileOption = 0;
	showSelectDialog = true;
	defaultPrintout = 0;
	paperFormat = 0;
	launchPdfViewer = true;
	askOverwrite = true;
	compressPdf = true;

	sheetStyle = 0;
	vGraphicSkin = 0;
	wGraphicSkin = 0;
	pdfViewerCommand = FileConfig::get()->getPdfViewerDefaultFilename();

	customHeader.enabled = true;
	customHeader.text = wxT( "$name/$fulldate/$location" );
	customHeader.showSeparator = true;
	customHeader.orientation = 0;
	customHeader.showOnFirstPage = false;

	customFooter.enabled = true;
	customFooter.text = _( "Printed by Maitreya $version on $creationdate/page $page" );
	customFooter.showSeparator = true;
	customFooter.orientation = 0;
	customFooter.showOnFirstPage = true;
}

/*****************************************************
**
**   PdfDocumentConfig   ---   Constructor
**
******************************************************/
int PdfDocumentConfig::getWxPaperSize()
{
 int format = wxPAPER_A4;
  switch( paperFormat )
  {
    case 1: 
      format = wxPAPER_LETTER;
    break;
    case 2:
      format = wxPAPER_LEGAL;
    break;
    default:
     //format = wxPAPER_A4;
    break;
  } 
	return format;
}

IMPLEMENT_SINGLETON( PrintoutConfigLoader )

/*****************************************************
**
**   PrintoutConfig   ---   Constructor
**
******************************************************/
PrintoutConfig::PrintoutConfig( wxString n  )
{
	name = n;
	vedic = config->vedic;
}

#define CFG_TITLE wxT( "title" )
#define CFG_HEADER wxT( "header" )
#define CFG_DASA_SUMMARY wxT( "dasasummary" )
#define CFG_CHART wxT( "chart" )
//#define CFG_WIDGET_GRID wxT( "widgetgrid" )
#define CFG_GRID wxT( "grid" )
#define CFG_COLUMN_SET wxT( "columnset" )
#define CFG_ROW_SET wxT( "rowset" )
#define CFG_SBC wxT( "sbc" )
#define CFG_EMPTY wxT( "empty" )
#define CFG_WESTERN_CHART wxT( "westernchart" )
#define CFG_VEDIC_CHART_PAIR wxT( "vedicchartpair" )
#define CFG_URANIAN wxT( "uranian" )

#define CFG_TYPE wxT( "type" )
#define CFG_DIVISION wxT( "division" )
#define CFG_DIVISION1 wxT( "division1" )
#define CFG_DIVISION2 wxT( "division2" )
#define CFG_HEADERTYPE wxT( "headertype" )
#define CFG_CHILDREN wxT( "children" )
#define CFG_HEADERTYPE wxT( "headertype" )
#define CFG_COLUMNS wxT( "columns" )
#define CFG_SKIN wxT( "skin" )
#define CFG_DASAID wxT( "dasaid" )
#define CFG_RATIO wxT( "ratio" )

/*****************************************************
**
**   PrintoutConfigLoader   ---   Constructor
**
******************************************************/
PrintoutConfigLoader::PrintoutConfigLoader()
 : ConfigListLoader<PrintoutConfig>( CFG_PRINTOUT, CrtCombinedDir )	
{
	pd_map[ CFG_TITLE ] = PD_TITLE;
	pd_map[ CFG_HEADER ] = PD_HEADER;
	pd_map[ CFG_DASA_SUMMARY ] = PD_DASA_SUMMARY;
	pd_map[ CFG_CHART ] = PD_CHART;
	//pd_map[ CFG_WIDGET_GRID ] = PD_WIDGET_GRID;
	pd_map[ CFG_GRID ] = PD_GRID;
	pd_map[ CFG_COLUMN_SET ] = PD_COLUMN_SET;
	pd_map[ CFG_ROW_SET ] = PD_ROW_SET;
	pd_map[ CFG_SBC ] = PD_SBC;
	pd_map[ CFG_WESTERN_CHART ] = PD_WESTERN_CHART;
	pd_map[ CFG_VEDIC_CHART_PAIR ] = PD_VEDIC_CHART_PAIR;
	pd_map[ CFG_URANIAN ] = PD_URANIAN;
	pd_map[ CFG_EMPTY ] = PD_EMPTY;
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   loadSingleConfig
**
******************************************************/
void PrintoutConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	PrintoutConfig *cfg = (PrintoutConfig*)NULL;
	if ( ! v[ CFG_EXTENDS ].IsNull())
	{
		wxString baseconfig = v[ CFG_EXTENDS ].AsString();
		PrintoutConfig *basecfg = getByName( baseconfig );
		if ( basecfg ) cfg = new PrintoutConfig( *basecfg );
		else wxLogError(
			wxString::Format( wxT( "could not find base config for extension %s - using default configuration for that entry" ),
			baseconfig.c_str()));
		//printf( "Found Extend relation: %s pointer is %ld\n", str2char( baseconfig ), (long)basecfg);
	}
	if ( ! cfg ) cfg = new PrintoutConfig;
	cfg->load( v );

	wxJSONValue &jconfigs = v[ CFG_ITEMS ];
	const bool isArray = jconfigs.IsArray();

	if ( isArray )
	{
		for ( int i = 0; i < jconfigs.Size(); i++ )
		{
			cfg->items.push_back( loadItem( jconfigs[i] ));
		}
	}
	else // no array
	{
		//wxLogError( wxT( "No valid item array found" ));
	}
	l.push_back( cfg );
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   loadContainer
**
******************************************************/
void PrintoutConfigLoader::loadContainer( wxJSONValue &v, PrintoutItemContainer *container )
{
	wxJSONValue &children = v[ CFG_CHILDREN ];
	const bool isArray = children.IsArray();

	if ( isArray )
	{
		for ( int i = 0; i < children.Size(); i++ )
		{
			//printf( "CHILD in loadContainer\n" );
			container->children.push_back( loadItem( children[i] ));
		}
	}
	else
	{
		// TODO error handling
	}
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   loadItem
**
******************************************************/
PrintoutItem *PrintoutConfigLoader::loadItem( wxJSONValue &v )
{
	wxString s;
	PrintoutItem *item = (PrintoutItem*)NULL;
	if ( ! v[ CFG_TYPE ].IsNull())
	{
		s = v[ CFG_TYPE ].AsString();
		if ( pd_map.find( s ) != pd_map.end() )
		{
			const PD_ITEM_TYPE id = pd_map[ s ];
			switch( id )
			{
				case PD_TITLE:
					item = new PrintoutItem( PD_TITLE );
				break;
				case PD_HEADER:
				{
					int headerType = 0;
					if ( ! v[ CFG_HEADERTYPE ].IsNull()) headerType = v[ CFG_HEADERTYPE ].AsInt();
					bool vedic = config->vedic;
					if ( ! v[ CFG_VEDIC ].IsNull()) vedic = v[ CFG_VEDIC ].AsBool();
					item = new PrintoutItemHeader( headerType, vedic );
				}
				break;
				case PD_VEDIC_CHART_PAIR:
				{
					VargaConfigLoader *vl = VargaConfigLoader::get();

					const bool vedic = true;
					Varga varga1 = V_RASI;
					if ( ! v[ CFG_DIVISION1 ].IsNull()) varga1 = vl->getVargaIndexByDivisionString( v[ CFG_DIVISION1 ].AsString() );

					Varga varga2 = V_NAVAMSA;
					if ( ! v[ CFG_DIVISION2 ].IsNull()) varga2 = vl->getVargaIndexByDivisionString( v[ CFG_DIVISION2 ].AsString() );
					int skin1 = 0;
					int skin2 = 0;

					// TODO auf valid varga prüfen, ggf. error

					PrintoutItemColumnSet *colset = new PrintoutItemColumnSet;
					colset->children.push_back( new PrintoutItemChart( vedic, skin1, varga1 ));
					colset->children.push_back( new PrintoutItemChart( vedic, skin2, varga2 ));
					item = colset;
				}
				break;
				case PD_WESTERN_CHART:
				{
					PrintoutItemColumnSet *colset = new PrintoutItemColumnSet;
					uint skin1 = UINT_FOR_NOT_SET;
					if ( ! v[ CFG_SKIN ].IsNull())
					{
						skin1 = WesternChartConfigLoader::get()->getIndexByName( v[ CFG_SKIN ].AsString());
						wxString a = v[ CFG_SKIN ].AsString();
						if ( skin1 == UINT_FOR_NOT_FOUND )
						{
							printf( "ERROR SKIN %u name %s\n", skin1, str2char( a ));
							skin1 = UINT_FOR_NOT_SET;
						}
						//else
						//printf( "SKIN %u name %s\n", skin1, str2char( a ));
					}
					//else printf( "SKIN not set %u\n", skin1 );
					
					colset->children.push_back( new PrintoutItemChart( false, skin1 ));
					item = colset;
				}
				break;
				case PD_CHART:
				{
					VargaConfigLoader *vl = VargaConfigLoader::get();
					int skin1 = 0;
					bool vedic = true;
					if ( ! v[ CFG_VEDIC ].IsNull()) vedic = v[ CFG_VEDIC ].AsBool();
					Varga varga = V_RASI;
					if ( ! v[ CFG_DIVISION ].IsNull()) varga = vl->getVargaIndexByDivisionString( v[ CFG_DIVISION ].AsString() );
					item = new PrintoutItemChart( vedic, skin1, varga );
				}
				break;
				case PD_SBC:
				{
					int skin = 0;
					item = new PrintoutItemSbc( skin );
				}
				break;
				case PD_COLUMN_SET:
				{
					//printf( "NEW COLSET\n" );
					PrintoutItemColumnSet *colset = new PrintoutItemColumnSet;
					loadContainer( v, colset );
					if ( ! v[ CFG_RATIO ].IsNull()) colset->ratio = myatof( v[ CFG_RATIO ].AsString());
					item = colset;
				}
				break;
				case PD_ROW_SET:
				{
					//printf( "NEW ROWSET\n" );
					PrintoutItemRowSet *rowset = new PrintoutItemRowSet;
					loadContainer( v, rowset );
					item = rowset;
				}
				break;
				case PD_GRID:
				{
					//printf( "NEW GRID\n" );
					uint nb_cols = 2;
					if ( ! v[ CFG_COLUMNS ].IsNull()) nb_cols = v[ CFG_COLUMNS ].AsUInt();
					PrintoutItemGrid *grid = new PrintoutItemGrid( nb_cols );
					loadContainer( v, grid );
					item = grid;
				}
				break;
				case PD_DASA_SUMMARY:
				{
					DasaId dasaId = D_VIMSOTTARI;
					int tableType = 0;
					if ( ! v[ CFG_DASAID ].IsNull()) dasaId = (DasaId)v[ CFG_DASAID ].AsInt();
					item = new PrintoutItemDasaSummary( dasaId, tableType );
				}
				break;
				case PD_URANIAN:
					item = new PrintoutItem( PD_URANIAN );
				break;
				case PD_EMPTY:
					item = new PrintoutItem( PD_EMPTY );
				break;
				default:
					printf( "wrong item %d\n", id );
					//assert( false );
				break;
			}
		}
		else
		{
			printf( "Key not found '%s'\n", str2char( s ));
			item = new PrintoutItemError( wxString::Format( wxT( "unsupported printout item %s" ), s.c_str()));
		}
	}
	return item;
}

