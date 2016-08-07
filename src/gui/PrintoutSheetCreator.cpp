/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PrintoutSheetCreator.cpp
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

#include "PrintoutSheetCreator.h"

#include "BarDiagram.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "DasaTool.h"
#include "GenericTableWriter.h"
#include "Horoscope.h"
#include "maitreya.h"
#include "PrintoutConfig.h"
#include "PrintoutTextHelper.h"
#include "Sheet.h"
#include "VargaBase.h"
#include "WesternChart.h"

extern Config *config;

enum PrintoutChartType { PcTypeWestern = 0, PcTypeVedicPair, PcTypeSbc };

/*************************************************//**
*
*  
*
******************************************************/
class PrintoutChart : public SheetWidgetItem
{
public:
	PrintoutChart( ChartProperties *props, Horoscope *h, const PrintoutChartType &mode, const Varga v1 = V_RASI, const Varga v2 = V_NAVAMSA )
	 : SheetWidgetItem( props ), horoscope( h ), mode( mode ), varga1( v1 ), varga2( v2 )
	{
		//printf( "skin %d\n", props->getWesternSkin());
		switch( mode )
		{
			case PcTypeWestern:
				ratio = 1;
			break;
			case PcTypeVedicPair:
				ratio = .4;
			break;
			case PcTypeSbc:
				ratio = .4;
			break;
			default:
				assert( false );
			break;
		}
	}

	virtual SheetWidgetItem *cloneClean()
	{
		return new PrintoutChart( props, horoscope, mode, varga1, varga2 );
	}

	virtual void doPaint( Painter *painter, const MRect& /*refreshRect*/ )
	{
		//printf( "PrintoutChart::doPaint RECT %f %f %f %f\n", refreshRect.x, refreshRect.y, refreshRect.width, refreshRect.height );
		GraphicalChart *chart;
		switch( mode )
		{
			case PcTypeWestern:
			{
				chart = ChartFactory().createWesternChart( CT_RADIX, props, horoscope, 0 );
				chart->OnDataChanged();
				chart->paint( painter, rect );
				delete chart;
			}
			break;
			case PcTypeVedicPair:
			{
				GraphicalChart *chart1 = ChartFactory().createVedicChart( CT_RADIX, props, horoscope, 0, varga1 );
				chart1->OnDataChanged();
				chart1->paint( painter, MRect( rect.x, rect.y, .5 * rect.width, rect.height ));
				delete chart1;

				GraphicalChart *chart2 = ChartFactory().createVedicChart( CT_RADIX, props, horoscope, 0, varga2 );
				chart2->OnDataChanged();
				chart2->paint( painter, MRect( rect.x + .5 * rect.width, rect.y, .5 * rect.width, rect.height ));
				delete chart2;
			}
			break;
			case PcTypeSbc:
			{
				chart = ChartFactory().createSarvatobhadraChart( CT_RADIX, props, horoscope, 0 );
				chart->OnDataChanged();
				chart->paint( painter, rect );
				delete chart;
			}
			break;
			default:
				assert( false );
			break;
		}
	}

private:
	Horoscope *horoscope;
	const PrintoutChartType mode;
	const Varga varga1, varga2;
};

/*****************************************************
**
**   PrintoutSheetCreator   ---   Constructor
**
******************************************************/
PrintoutSheetCreator::PrintoutSheetCreator( Horoscope *h, ChartProperties *chartprops, Sheet *sheet )
 : horoscope( h ),
  chartprops( chartprops ),
  sheet( sheet )
{
}

/*****************************************************
**
**   PrintoutSheetCreator   ---   write
**
******************************************************/
void PrintoutSheetCreator::write( const int &id )
{
	PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
	PrintoutConfig *cfg = loader->getConfig( id );

	for( uint i = 0; i < cfg->items.size(); i++ )
	{
		switch( cfg->items[i].typeId )
		{
			case PD_HEADER:
				PrintoutTextHelper( horoscope, chartprops, sheet ).writePrintoutHeader( cfg->items[i].subtype, cfg->items[i].vedic );
			break;
			case PD_VEDIC_CHART_PAIR:
			{
				VargaConfigLoader *loader = VargaConfigLoader::get();
				Varga v1 = V_RASI;
				Varga v2 = V_NAVAMSA;
				if ( cfg->items[i].vargaIds.size() > 0 )
				{
					v1 = loader->getVargaIndexByDivision( cfg->items[i].vargaIds[0]);
					if ( cfg->items[i].vargaIds.size() > 1 )
					{
						v2 = loader->getVargaIndexByDivision( cfg->items[i].vargaIds[1]);
					}
				}
				sheet->addItem( new PrintoutChart( chartprops, horoscope, PcTypeVedicPair, v1, v2 ));
			}
			break;
			case PD_DASA_SUMMARY:
			{
				DasaTool *tool = DasaTool::get();
				switch( cfg->items[i].subtype )
				{
					case 1:
						tool->writeShortReport( sheet, horoscope, (DasaId)cfg->items[i].dasaId, true );
					break;
					case 2:
						tool->writeComprehensiveReport( sheet, horoscope, (DasaId)cfg->items[i].dasaId, true );
					break;
					case 0:
					default:
						tool->writeCompactReport( sheet, horoscope, (DasaId)cfg->items[i].dasaId, true );
					break;
				}
			}
			break;
			case PD_WESTERN_CHART:
				sheet->addItem( new PrintoutChart( chartprops, horoscope, PcTypeWestern ));
			break;
			case PD_SBC:
				sheet->addItem( new PrintoutChart( chartprops, horoscope, PcTypeSbc ));
			break;
			case PD_ASPECTARIUM:
			break;
			case PD_ASHTAKAVARGA:
			break;
			case PD_URANIAN:
			break;
			case PD_YOGAS:
			break;
			case PD_VARGA_DIAGRAMS:
			{
				BarDiagramWriter bdwriter( sheet, chartprops, horoscope );
				bdwriter.writeVargaDiagrams();
			}
			break;
			case PD_ERROR:
				sheet->addHeader( wxString::Format( wxT( "Error: unknown printout item \"%s\"" ), cfg->items[i].type.c_str() ));
			break;
			default:
			break;
		}
	}
}




