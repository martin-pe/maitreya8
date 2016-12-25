/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/PrintoutSheetCreator.cpp
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
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
#include "Uranian.h"
#include "UranianHelper.h"
#include "VargaBase.h"
#include "WesternChart.h"
#include "WesternChartConfig.h"

extern Config *config;

/*************************************************//**
*
*  
*
******************************************************/
class EmptySheetItem : public SheetWidgetItem
{ 
public:
	EmptySheetItem( ChartProperties *props, wxString m ) : SheetWidgetItem( props ), message( m ) {}

	virtual SheetItem *cloneClean() { return new EmptySheetItem( props, message ); }

	virtual void doPaint( Painter *painter, const MRect& /*refreshRect*/ )
	{ 
		//painter->setDefaultBrush();
		painter->setTransparentBrush();
		painter->setPen( *wxRED );
		painter->drawRectangle( rect );
		painter->drawLine( rect.x, rect.y, rect.x + rect.width, rect.y + rect.height );
		painter->drawLine( rect.x, rect.y + rect.height, rect.x + rect.width, rect.y );
		painter->setTextColor( *wxRED );
		painter->drawTextFormatted( rect, message, Align::Center );
		painter->setDefaults();
	}

private:
	wxString message;
};

/*************************************************//**
*
*  
*
******************************************************/
class PrintoutChart : public SheetWidgetItem
{
public:
	PrintoutChart( ChartProperties *props, Horoscope *h, PrintoutItem *item )
	 : SheetWidgetItem( props ), horoscope( h ), item( item ) {}

	virtual SheetWidgetItem *cloneClean() { return new PrintoutChart( props, horoscope, item ); }

	virtual void doPaint( Painter *painter, const MRect& /*refreshRect*/ )
	{
		//printf( "PrintoutChart::doPaint type %d RECT %f %f %f %f\n", item->type, refreshRect.x, refreshRect.y, refreshRect.width, refreshRect.height );
		GraphicalChart *chart = (GraphicalChart*)NULL;
		switch( item->type )
		{
			case PD_CHART:
			{
				PrintoutItemChart *pi = (PrintoutItemChart*)item;
				if ( pi->vedic )
				{
					chart = ChartFactory().createVedicChart( CT_RADIX, props, horoscope, 0, pi->varga );
				}
				else
				{
					//props->setWesternSkin( pi->skin != UINT_FOR_NOT_SET ? pi->skin : props->wGraphicSkin );
					if ( pi->skin != UINT_FOR_NOT_SET ) props->setWesternSkin( pi->skin );
					chart = ChartFactory().createWesternChart( CT_RADIX, props, horoscope, 0 );
				}
			}
			break;
			case PD_SBC:
				chart = ChartFactory().createSarvatobhadraChart( CT_RADIX, props, horoscope, 0 );
			break;
			default:
				assert( false );
			break;
		}
		assert( chart );
		chart->OnDataChanged();
		//printf( "PrintoutChart::doPaint type %d RECT %f %f %f %f\n", item.typeId, rect.x, rect.y, rect.width, rect.height );
		chart->paint( painter, rect );
		delete chart;
	}

private:
	Horoscope *horoscope;
	PrintoutItem *item;
};

/*****************************************************
**
**   PrintoutSheetCreator   ---   Constructor
**
******************************************************/
PrintoutSheetCreator::PrintoutSheetCreator( Horoscope *h, ChartProperties *chartprops )
 : horoscope( h ),
  chartprops( chartprops )
{
}

/*****************************************************
**
**   PrintoutSheetCreator   ---   addItem
**
******************************************************/
void PrintoutSheetCreator::addItem( Sheet *sheet, PrintoutItem *item )
{
	assert( item );
	//printf( "PrintoutSheetCreator::addItem type %d\n", item->type );
	PrintoutTextHelper helper( horoscope, chartprops, sheet );

	switch( item->type )
	{
		case PD_TITLE:
			helper.writeTitle();
		break;
		case PD_HEADER:
		{
			PrintoutItemHeader *pi = (PrintoutItemHeader*)item;
			helper.writePrintoutHeader( pi->headerType, pi->vedic );
		}
		break;
		case PD_CHART:
		case PD_SBC:
			sheet->addItem( new PrintoutChart( chartprops, horoscope, item ));
		break;
		case PD_EMPTY:
			sheet->addItem( new EmptySheetItem( chartprops, wxT( "Empty" )));
		break;
		case PD_DASA_SUMMARY:
		{
			DasaTool *tool = DasaTool::get();
			PrintoutItemDasaSummary *ditem = (PrintoutItemDasaSummary*)item;
			switch( ditem->tableType )
			{
				case 1:
					tool->writeShortReport( sheet, horoscope, ditem->dasaId, true );
				break;
				case 2:
					tool->writeComprehensiveReport( sheet, horoscope, ditem->dasaId, true );
				break;
				case 0:
				default:
					tool->writeCompactReport( sheet, horoscope, ditem->dasaId, true );
				break;
			}
		}
		break;
		case PD_COLUMN_SET:
		{
			PrintoutItemColumnSet *cs = (PrintoutItemColumnSet*)item;
			//printf( "COLUMN SET size %ld ratio %f\n", cs->children.size(), cs->ratio );
			double ratio = cs->ratio;
			if ( ratio == 0 )
			{
				if ( cs->children.size() > 1 ) ratio = .4;
				else ratio = 1;
			}
			SheetWidgetGrid *grid = new SheetWidgetGrid( cs->children.size(), ratio );
			for( list<PrintoutItem*>::iterator iter = cs->children.begin(); iter != cs->children.end(); iter++ )
			{
				addItem( grid->sheet, *iter );
			}
			sheet->addItem( grid );
		}
		break;
		case PD_ROW_SET:
		{
			PrintoutItemRowSet *cs = (PrintoutItemRowSet*)item;
			//printf( "ROW SET size %ld\n", cs->children.size() );
			SheetRowSet *rowset = new SheetRowSet();
			for( list<PrintoutItem*>::iterator iter = cs->children.begin(); iter != cs->children.end(); iter++ )
			{
				addItem( rowset->sheet, *iter );
			}
			sheet->addItem( rowset );
		}
		break;
		case PD_GRID:
		{
			PrintoutItemGrid *g = (PrintoutItemGrid*)item;
			//printf( "GRID size %ld\n", g->children.size() );
			SheetWidgetGrid *grid = new SheetWidgetGrid( g->nb_cols );
			for( list<PrintoutItem*>::iterator iter = g->children.begin(); iter != g->children.end(); iter++ )
			{
				addItem( grid->sheet, *iter );
			}
			sheet->addItem( grid );
		}
		break;
		case PD_URANIAN:
		{
			UranianExpert expert( horoscope, chartprops );
			expert.OnDataChanged();
			UranianHelper helper( &expert );

			SheetColumnSet *sc = new SheetColumnSet;
			SheetItem *t = (SheetItem*)helper.createTupleTable( UTT_MIDPOINTS, PcRadix );
			sc->addItem( t );
			sheet->addItem( sc );
		}
		break;
		case PD_ERROR:
		{
			PrintoutItemError *eitem = (PrintoutItemError*)item;
			wxString s = wxT( "Error: " );
			s << eitem->message;
			sheet->addHeader( s );
		}
		break;
		default:
			printf( "PrintoutSheetCreator::addItem wrong item with id %d\n", item->type );
			sheet->addHeader( wxString::Format( wxT( "Error: unsupported item id %d" ), item->type ));
		break;
	}
}

/*****************************************************
**
**   PrintoutSheetCreator   ---   write
**
******************************************************/
void PrintoutSheetCreator::write( Sheet *sheet, const int &id )
{
	PrintoutConfigLoader *loader = PrintoutConfigLoader::get();
	PrintoutConfig *cfg = loader->getConfig( id );
	PrintoutTextHelper helper( horoscope, chartprops, sheet );

	for( uint i = 0; i < cfg->items.size(); i++ )
	{
		if ( ! cfg->items[i] ) continue;
		addItem( sheet, cfg->items[i] );
	}

/*
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
*/
}




