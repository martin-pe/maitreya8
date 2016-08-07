/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/PrintoutConfig.cpp
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

#include "PrintoutConfig.h"

#include "Conf.h"
#include "maitreya.h"
#include "FileConfig.h"
#include "JSonTool.h"

#include <math.h>
#include <wx/jsonval.h>
#include <wx/log.h>

extern Config *config;

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
	customHeader.text = _( "$name/$fulldate/$location" );
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
**   PrintoutConfigLoader   ---   Constructor
**
******************************************************/
PrintoutConfigLoader::PrintoutConfigLoader()
 : ConfigListLoader<PrintoutConfig>( CFG_PRINTOUT, CrtGlobalFile )	
{
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   loadSingleConfig
**
******************************************************/
void PrintoutConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	PrintoutConfig *cfg = new PrintoutConfig;
	cfg->load( v );

	printf( "HALLAO ....... ---- name %s\n", str2char( cfg->name ));

	wxJSONValue &jconfigs = v[ CFG_ITEMS ];

	const bool isArray = jconfigs.IsArray();

	if ( isArray )
	{
		for ( int i = 0; i < jconfigs.Size(); i++ )
		{
			printf( "KUNO\n" );
			PrintoutConfigItem item;
			item.load( jconfigs[i] );
			item.typeId = itemType2Int( item.type );
			//if ( item.typeId == PD_ERROR ) item.subtype = 
			printf( "ITEM\n" );
			printf( "type %s ID %d\n", str2char( item.type ), (int)item.typeId);
			printf( "Subtype %d\n", item.subtype );

			if ( item.vargaIds.size() > 0 )
			{
				for( uint i = 0; i < item.vargaIds.size(); i++ ) printf( "VARGA %d\n", item.vargaIds[i] );
			}
			cfg->items.push_back( item );
		}
	}
	else // no array
	{
		wxLogError( wxT( "No valid item array found" ));
	}

	l.push_back( cfg );
}

/*****************************************************
**
**   PrintoutConfigLoader   ---   itemType2Int
**
******************************************************/
PD_ITEM_TYPE PrintoutConfigLoader::itemType2Int( wxString s )
{
	if ( ! s.CmpNoCase( wxT( "header" ))) return PD_HEADER;
	else if ( ! s.CmpNoCase( wxT( "westernchart" ))) return PD_WESTERN_CHART;
	else if ( ! s.CmpNoCase( wxT( "vedicchartpair" ))) return PD_VEDIC_CHART_PAIR;
	else if ( ! s.CmpNoCase( wxT( "dasa" ))) return PD_DASA_SUMMARY;
	else if ( ! s.CmpNoCase( wxT( "yogas" ))) return PD_YOGAS;
	else if ( ! s.CmpNoCase( wxT( "sbc" ))) return PD_SBC;
	else if ( ! s.CmpNoCase( wxT( "vargadiagrams" ))) return PD_VARGA_DIAGRAMS;

	return PD_ERROR;

//enum PD_ITEM_TYPE { PD_HEADER = 0, PD_DASA_SUMMARY, PD_VEDIC_CHART_PAIR, PD_WESTERN_CHART, PD_SBC, PD_ASPECTARIUM, PD_ASHTAKAVARGA, PD_URANIAN }; PD_VARGA_DIAGRAMS,

}



