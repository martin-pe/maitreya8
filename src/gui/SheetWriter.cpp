/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWriter.cpp
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

#include "SheetWriter.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "Painter.h"
#include "SheetConfig.h"
#include "Table.h"

#include <wx/log.h>
#include <wx/stopwatch.h>
#include <wx/tokenzr.h>

extern Config *config;

// logging of times for preformant paint
#define SHOW_STOP_WATCH

// extra y factors for line breaks
#define Y_EXTRA_LINE 0
#define Y_EXTRA_PARAGRAPH .5
#define Y_EXTRA_HEADER 1.5

IMPLEMENT_CLASS( GenericSheetWriter, wxObject )
IMPLEMENT_CLASS( DcSheetWriter, GenericSheetWriter )

/*****************************************************
**
**   GenericSheetWriter   ---   Constructor
**
******************************************************/
GenericSheetWriter::GenericSheetWriter( Sheet *sheet, SheetConfig *sheetcfg, WriterConfig *writercfg, ColorConfig *colorcfg )
	:	sheet( sheet ),
	sheetcfg( sheetcfg ),
	writercfg( writercfg ),
	colorcfg( colorcfg )
{
	cornerRadius = 0;
	pageSize = MPoint( 100, 100 );

	// general config
	doCenterAll = false;
	doUniformScaling = false;
}

/*****************************************************
**
**   GenericSheetWriter   ---   Destructor
**
******************************************************/
GenericSheetWriter::~GenericSheetWriter()
{
}

/*****************************************************
**
**   GenericSheetWriter   ---   preformat
**
******************************************************/
void GenericSheetWriter::preformat( Painter *painter, const double &xrm  )
{
	//wxLogMessage( wxT( "START GenericSheetWriter::preformat xrightmax %f" ), xrm );
	xrightmax = xrm;

	//printf( "GenericSheetWriter::preformat xrightmax %f item count %d\n", xrightmax, sheet->items.size() );

#ifdef SHOW_STOP_WATCH
	const wxLongLong starttime = wxGetLocalTimeMillis();
#endif

	/*
	*  Variables for coordinates
	*  item->rect: item coordinates relative to the paint origin. y = 0 in the first step. 
	*
	*  xSizeContents: maximum of the width of the items. starts with 0
	*
	*  xrightmax: hard limit for maximum on right side (depends on paper, resp maximum apropriate screen res)
	*
	*  contentRect: coordinates for painting
	*
	*  sheetRect: not used here
	*
	*/

	xSizeContents = 0;
	//xSizeContents = xrightmax;
	contentRect.width = xrightmax;
	ycursor = contentRect.y;
	for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
	{
		preformatItem( painter, *iter );
		(*iter)->moveTo( contentRect.x, ycursor );
		ycursor += (*iter)->rect.height;
		ycursor += table_widget_extra_y;
	}

	if ( doCenterAll ) sheet->centerItems();

#ifdef SHOW_STOP_WATCH
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "GenericSheetWriter::preformat in %ld millisec" ), totaltime.ToLong() ));
#endif
}

/*****************************************************
**
**   GenericSheetWriter   ---   preformatItem
**
******************************************************/
void GenericSheetWriter::preformatItem( Painter *painter, SheetItem *item )
{
	//wxLogMessage( wxT( "START GenericSheetWriter::preformatItem type %d" ), item->type );
	FontProvider *fc = FontProvider::get();
	FONT_ID fontid;
	double h, lineheight;

	switch( item->type )
	{
		case WiTable:
		{
			Table *table = wxDynamicCast( item, Table );
			assert( table );
			preformatTable( painter, table );
		}
		break;
		case WiWidgetGrid:
		{
			SheetWidgetGrid *grid = wxDynamicCast( item, SheetWidgetGrid );
			assert( grid );
			preformatWidgetGrid( painter, grid );
		}
		break;
		case WiColumnSet:
		{
			SheetColumnSet *colset = wxDynamicCast( item, SheetColumnSet );
			assert( colset );
			preformatColset( painter, colset );
		}
		break;
		case WiRowSet:
		{
			SheetRowSet *rowset = wxDynamicCast( item, SheetRowSet );
			assert( rowset );
			preformatRowset( painter, rowset );
		}
		break;
		case WiText:
		{
			SheetTextItem *ti = (SheetTextItem*)item;
			fontid = getFontIdForItem( ti->subtype );
			painter->setFont( *fc->getFont( fontid ));

			SheetFormatter ffmt;
			formatMString( painter, ti->tf, xrightmax );
			const uint linecount = ti->tf.formattedLines.size();

			xSizeContents = Max( ti->tf.size.real(), xSizeContents );
			h = ti->tf.size.imag();

			lineheight = h;
			if ( linecount > 1 )
			{
				lineheight /= linecount;
			}

			//printf( "h %f Linehight %f linecount %d\n", h, lineheight, linecount );

			switch( ti->subtype )
			{
				case WitHeader:
					h += lineheight * Y_EXTRA_HEADER;
				break;
				case WitParagraph:
					h += lineheight * Y_EXTRA_PARAGRAPH;
				break;
				case WitLine:
					h += lineheight * Y_EXTRA_LINE;
				break;
				default:
					printf( "invalid text item subtype %d\n", ti->subtype );
					assert( false );
				break;
			}
			//printf( "Item type %d xsize now %f\n", (int)ti->type, xSizeContents );
			item->rect = MRect( contentRect.x, ycursor, ti->tf.size.real(), h );
		}
		break;
		case WiWidget:
			printf( "GenericSheetWriter::preformat WiWidget not allowed here. Use container\n" );
			//assert( false );
		break;
		default:
			printf( "GenericSheetWriter::preformat invalid type %d\n", item->type );
			assert( false );
		break;
	}
	//printf( "ENDE GenericSheetWriter::preformatItem type %d\n", item->type );
}

/*****************************************************
**
**   GenericSheetWriter   ---   preformatRowset
**
******************************************************/
void GenericSheetWriter::preformatRowset( Painter *painter, SheetRowSet *rowset )
{
	//printf( "GenericSheetWriter::preformatRowset\n" );
	double xmax = pageSize.real() - 2 * contentRect.x;
	double ymax = pageSize.imag() - 2 * contentRect.y;
	double y0 = ycursor;

	rowset->rect = MRect( contentRect.x, ycursor, xmax, ymax + 1 );
	int shrinkrate = 0;

	while ( shrinkrate < 10 && rowset->rect.height > ymax )
	{
		y0 = ycursor;
		for( list<SheetItem*>::iterator iter = rowset->sheet->items.begin(); iter != rowset->sheet->items.end(); iter++ )
		{
			SheetItem *item = (SheetItem*)(*iter);
			//printf( "GenericSheetWriter::preformatRowset GOTO item, type is %d\n", item->type );
			item->shrinkrate = shrinkrate;
			preformatItem( painter, item );
			item->moveTo( contentRect.x, y0 );
			y0 += item->rect.height;
		}

		rowset->rect.height = y0 - ycursor;
		shrinkrate++;
		//printf( "GenericSheetWriter::preformatRowset after loop rect height %f ymax %f\n", rowset->rect.height, ymax );
	}

	// arrange vertically
	y0 = rowset->rect.y + .5 * ( ymax - rowset->rect.height );
	//printf( "GenericSheetWriter::preformatRowset arrange rect height %f ymax %f y0 %f\n", rowset->rect.height, ymax, y0 );
	for( list<SheetItem*>::iterator iter = rowset->sheet->items.begin(); iter != rowset->sheet->items.end(); iter++ )
	{
		(*iter)->moveTo( (*iter)->rect.x, y0 );
		y0 += (*iter)->rect.height;
		//printf( "GenericSheetWriter::preformatRowset after arrange ITEM rect x %f y %f\n", (*iter)->rect.x, (*iter)->rect.y );
	}

	if ( doCenterAll ) rowset->sheet->centerItems();
}

/*****************************************************
**
**   GenericSheetWriter   ---   preformatColset
**
******************************************************/
void GenericSheetWriter::preformatColset( Painter *painter, SheetColumnSet *colset )
{
	//printf( "GenericSheetWriter::preformatColset\n" );

	double xmax = pageSize.real() - 2 * contentRect.x;
	//double ymax = pageSize.imag() - 2 * contentRect.y;

	colset->rect = MRect( contentRect.x, ycursor, xmax, 0 );

	double maxh = 0;
	double sumx = xrightmax + 1;
	int shrinkrate = colset->shrinkrate;

	while( sumx > xrightmax && shrinkrate < 10 )
	{
		maxh = 0;
		sumx = 0;
		for( list<SheetItem*>::iterator iter = colset->sheet->items.begin(); iter != colset->sheet->items.end(); iter++ )
		{
			SheetItem *item = (SheetItem*)(*iter);
			assert( item );
			item->shrinkrate = shrinkrate;
			preformatItem( painter, item );
			maxh = Max( maxh, item->rect.height );
			sumx += item->rect.width;

			//item->moveTo( MPoint( currentx, colset->rect.y ));
			//currentx += xrightmax / colset->getSize();
		}
		shrinkrate++;
		if ( sumx > xrightmax )
		{
			printf( "WARN: table ist too large sumx %f contentRect.x %f xrightmax %f, shrinkrate %d\n", sumx, contentRect.x, xrightmax, shrinkrate );
		}
	}

	colset->rect.height = maxh + table_widget_extra_y;
	//double diff = 0;
	//if ( colset->getSize() > 1 ) diff = ( colset->rect.width - sumx ) / ( colset->getSize() - 1 );

	//double newx = contentRect.x + .5 * diff;
	double newx = contentRect.x + .5 * ( colset->rect.width - sumx - ( colset->getSize() - 1 ) * table_widget_extra_y );

	for( list<SheetItem*>::iterator iter = colset->sheet->items.begin(); iter != colset->sheet->items.end(); iter++ )
	{
		//printf( "Adjust table old x %f diff %f newx %f\n", (*iter)->rect.x, diff, (*iter)->rect.x + diff );
		(*iter)->moveTo( newx, colset->rect.y );
		newx += (*iter)->rect.width + table_widget_extra_y;
	}

	// debug output
	/*
  for( list<SheetItem*>::iterator iter = colset->sheet->items.begin(); iter != colset->sheet->items.end(); iter++ )
	{
		printf( "finale colset rect x %f y %f w %f h %f\n", 
			(*iter)->rect.x, (*iter)->rect.y, (*iter)->rect.width, (*iter)->rect.height );
	}
	*/
	//ycursor += colset->rect.height;
}

/*****************************************************
**
**   GenericSheetWriter   ---   preformatWidgetGrid
**
******************************************************/
void GenericSheetWriter::preformatWidgetGrid( Painter* /*painter*/, SheetWidgetGrid *grid )
{
	//printf( "GenericSheetWriter::preformatWidgetGrid\n" );

	double width = pageSize.real() - 2 * contentRect.x;
	width = Max( grid->wmin, width );
	if ( grid->wmax > 0 ) width = Min( grid->wmax, width );

	double height = grid->x2yratio ? width * grid->x2yratio : pageSize.imag() - 2 * contentRect.y;
	height = Max( grid->hmin, height );
	if ( grid->hmax > 0 ) height = Min( grid->hmax, height );

	grid->rect = MRect( contentRect.x, ycursor, width, height );
	//printf( "widget grid rect x %f y %f w %f h %f\n", grid->rect.x, grid->rect.y, grid->rect.width, grid->rect.height );

	double x0 = grid->rect.x;
  double y0 = grid->rect.y;

	assert( grid->nb_cols > 0 );
	const double c_width = width / grid->nb_cols;
	int rows = (int)(grid->sheet->items.size() / grid->nb_cols);
	if ( grid->sheet->items.size() %  grid->nb_cols ) rows++;


	const double r_height = height / rows;
	//printf( "GenericSheetWriter::preformatWidgetGrid grid size %ld rows %d r_height %f\n", grid->sheet->items.size(), rows, r_height );
	//const double r_height = height / grid->nb_rows;
  uint c = 0;
  uint r = 0;

  for( list<SheetItem*>::iterator iter = grid->sheet->items.begin(); iter != grid->sheet->items.end(); iter++ )
	{
		SheetWidgetItem *item = (SheetWidgetItem*)*iter;
    item->rect.x = x0;
		item->rect.width = c_width;
		x0 += c_width;

    item->rect.y = y0;
    if ( ++c == grid->nb_cols )
    {
      c = 0;
      x0 = grid->rect.x;
      y0 += r_height;
      r++;
    }
		item->rect.height = r_height;
	}

	/*
  for( list<SheetItem*>::iterator iter = grid->sheet->items.begin(); iter != grid->sheet->items.end(); iter++ )
	{
		printf( "finale widget grid rect x %f y %f w %f h %f\n", 
			(*iter)->rect.x, (*iter)->rect.y, (*iter)->rect.width, (*iter)->rect.height );
	}
	*/
	//ycursor += grid->rect.height;
}

/*****************************************************
**
**   GenericSheetWriter   ---   preformatTable
**
******************************************************/
void GenericSheetWriter::preformatTable( Painter *painter, Table *table )
{
	//wxLogMessage( wxT( "START GenericSheetWriter::preformatTable" ));
	assert( table );
	SheetFormatter formatter;

	MPoint p;
	FontProvider *fc = FontProvider::get();
	TableEntry *e;
	double maxcolwidth = 0;
	double maxrowheight = 0;

	table->resetRectangles();

	if ( ! table->header.isEmpty())
	{
		painter->setFont( *fc->getFont( getFontIdForItem( WitHeader ), -table->shrinkrate ));
		p = painter->getTextExtent( table->header.tf );

		xSizeContents = Max( p.real(), xSizeContents );
		table->header.rect.height = Y_EXTRA_HEADER * p.imag();

		//wxFont fff = *fc->getFont( getFontIdForItem( WitHeader ));
		//printf( "HEADER size %f size %d\n", p.imag(), fff.GetPointSize());
	}
	table->rect = MRect( contentRect.x, 0, 0, table->header.rect.height );
	double singleLineTotalWidth = 0;

	// text amount per columns. will be used for weighted alignment of column width if total width is too large
	vector<double> textAmountPerCol;
	for ( uint col = 0; col < table->getNbCols(); col++ ) textAmountPerCol.push_back( 0 );

	for ( uint col = 0; col < table->getNbCols(); col++ )
	{
		if ( table->isEmptyCol( col ))
		{
			table->col_width[col] = table_empty_col_width;
		}
		else
		{
			for ( uint row = 0; row < table->getNbRows(); row++ )
			{
				e = &table->contents[row].value[col];
				painter->setFont( *fc->getFont( getFontIdForItem( e->isHeader ? WitTableHeader : WitTableCell ), -table->shrinkrate ));
				formatMString( painter, e->text );

				textAmountPerCol[col] += e->text.size.real();
				table->col_width[col] = Max( table->col_width[col], e->text.size.real() + table_cell_delta_x );

				table->contents[row].rect.height = Max( table->contents[row].rect.height, e->text.size.imag() + table_cell_delta_y );
				maxrowheight = Max( maxrowheight, table->contents[row].rect.height );
			}
		}
		maxcolwidth = Max( maxcolwidth, table->col_width[col] );
		singleLineTotalWidth += table->col_width[col];
	}

	// reformat table if evailable space is not sufficient.
	// large lines will get line breaks if possible
	if ( singleLineTotalWidth > xrightmax )
	{
		printf( "Table to large singleLineTotalWidth %f xrightmax %f\n", singleLineTotalWidth, xrightmax );

		// all colums will be scaled proportionally
		if ( doUniformScaling )
		{
			double reservedColwidth = 0;
			uint nbTooLarge = 0;
			double totalTextAmountOfTooLargeCols = 0;

			const double avgWidth = xrightmax / table->getNbCols();
			for ( uint col = 0; col < table->getNbCols(); col++ )
			{
				if ( table->col_width[col] > avgWidth )
				{
					totalTextAmountOfTooLargeCols += textAmountPerCol[col];
					nbTooLarge++;
				}
				else
				{
					//printf( "OK %u real width %f\n", col, table->col_width[col] );
					reservedColwidth += table->col_width[col];
				}
			}

			const double availableColWidth = xrightmax - reservedColwidth;
			assert( nbTooLarge > 0 && nbTooLarge <= table->getNbCols());

			double totalTextAmount = 0;
			for ( uint col = 0; col < table->getNbCols(); col++ )
			{
				totalTextAmount += textAmountPerCol[col];
			}

			for ( uint col = 0; col < table->getNbCols(); col++ )
			{
				if ( table->col_width[col] > avgWidth )
				{
					table->col_width[col] = textAmountPerCol[col] / totalTextAmountOfTooLargeCols * availableColWidth;
					for( uint row = 0; row < table->getNbRows(); row++ )
					{
						e = &table->contents[row].value[col];
						formatMString( painter, e->text, table->col_width[col] );
						table->contents[row].rect.height = Max( table->contents[row].rect.height, e->text.size.imag() + table_cell_delta_y );
					}
				}
			}
		}
		else // only scale according to col_break params (must be set programmatically );
		{
			// count number of lines with break property
			int nb_breaklines = 0;

			// volume of stretchable space
			double yvol = 0;

			// fixed space (i.e. without col_break property)
			double yfixed = 0;
			for ( uint col = 0; col < table->getNbCols(); col++ )
			{
				if ( table->col_break[col] )
				{
					nb_breaklines++;
					//printf( "Break line %d width %f\n", col,  table->col_width[col] );
					yvol += table->col_width[col];
				}
				else
				{
					//printf( "NO Break line %d width %f\n", col,  table->col_width[col] );
					yfixed += table->col_width[col];
				}
			}
			//printf( "Break lines %d volume %f fixed %f\n", nb_breaklines, yvol, yfixed );
			if ( nb_breaklines > 0 )
			{
				assert( yvol > 0 );
				double shrinktarget = singleLineTotalWidth - xrightmax;
				const double aa = 1 + shrinktarget / yvol;
				//printf( "Shrink target %f shrinkrate %f\n", shrinktarget, shrinkrate );

				for ( uint col = 0; col < table->getNbCols(); col++ )
				{
					if ( table->col_break[col] )
					{
						//printf( "COL %d YES target value calculated %f\n", col, table->col_width[col] * aa );
						table->col_width[col] = Max( table->col_width[col] * aa, 200 ); 
						for( uint row = 0; row < table->getNbRows(); row++ )
						{
							e = &table->contents[row].value[col];
							formatMString( painter, e->text, table->col_width[col] );
							table->contents[row].rect.height = Max( table->contents[row].rect.height, e->text.size.imag() + table_cell_delta_y );
						}
					}
					else
					{
						//printf( "COL %d NO\n", col );
					}
				}
			}
		}
	}

	// uniformLayout only supported if table is not too large
	else if ( table->uniformLayout )
	{
		for ( uint col = 0; col < table->getNbCols(); col++ )
		{
			table->col_width[col] = maxcolwidth;
		}
		for( uint row = 0; row < table->getNbRows(); row++ )
		{
			table->contents[row].rect.height = maxrowheight;
		}
	}

	// finally calculate rectangle size of table item
	for ( uint col = 0; col < table->getNbCols(); col++ )
	{
		//printf( "FINAL col %d width %f\n", col, table->col_width[col] );
		table->rect.width += table->col_width[col];
	}
	table->header.rect.width = table->rect.width;

	for ( uint row = 0; row < table->getNbRows(); row++ )
	{
		table->rect.height += table->contents[row].rect.height;
		table->contents[row].rect.width = table->rect.width;
	}

	xSizeContents = Max( xSizeContents, table->rect.width );

	//printf( "FINISHED GenericSheetWriter::preformatTable tableWidth %f tableHeight %f\n", table->rect.width, table->rect.height );
	//wxLogMessage( wxT( "ENDE GenericSheetWriter::preformatTable" ));
}

/*****************************************************
**
**   GenericSheetWriter   ---   drawSheet
**
******************************************************/
void GenericSheetWriter::drawSheet( Painter *painter, const MRect &refreshRect, const bool )
{
	assert( painter );
#ifdef SHOW_STOP_WATCH
	const wxLongLong starttime = wxGetLocalTimeMillis();
#endif

	//printf( "GenericSheetWriter::paintPage contentRect.x %f contentRect.y %f wirter has %lu items\n",
		//contentRect.x, contentRect.y, sheet->items.size() );

	for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
	{
		SheetItem *item = (SheetItem*)*iter;
		assert( item );

		handlePageBreak( item );
		drawItem( painter, item, refreshRect );
	}
#ifdef SHOW_STOP_WATCH
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "GenericSheetWriter::drawSheet in %ld millisec" ), totaltime.ToLong() ));
#endif
}

/*****************************************************
**
**   GenericSheetWriter   ---   drawItem
**
******************************************************/
void GenericSheetWriter::drawItem( Painter *painter, SheetItem *item, const MRect &refreshRect )
{
	assert( painter );
	assert( item );

	if  ( ! refreshRect.intersects( item->rect )) return;

	FontProvider *fc = FontProvider::get();
	FONT_ID fontid;

	switch( item->type )
	{
		case WiTable:
			drawTable( painter, (Table*)item, refreshRect );
		break;
		case WiWidget:
		{
			SheetWidgetItem *wi = (SheetWidgetItem*)item;
			wi->doPaint( painter, refreshRect );
		}
		break;
		case WiColumnSet:
		case WiWidgetGrid:
		case WiRowSet:
		{
			//printf( "CONTAINER ITEM TYPE %d row set %d\n", item->type, WiRowSet );
			SheetItemContainer *container = wxDynamicCast( item, SheetItemContainer );
			assert( container );
			for( list<SheetItem*>::iterator iter = container->sheet->items.begin(); iter != container->sheet->items.end(); iter++ )
			{
				//painter->setPen( *wxGREEN_PEN );
				//painter->drawRectangle( (*iter)->rect );
				drawItem( painter, *iter, refreshRect );
			}
		}
		break;
		case WiText:
		{
			SheetTextItem *ti = (SheetTextItem*)item;

			// draw text item
			fontid = getFontIdForItem( ti->subtype );
			painter->setFont( *fc->getFont( fontid ));
			painter->setTextColor( config->colors->fgColor );
			painter->drawMString( ti->rect, ti->tf, Align::Left | Align::Top );
		}
		break;
		default:
			printf( "Wrong writer item type %d\n", (int)item->type );
			assert( false );
		break;
	}
}

/*****************************************************
**
**   GenericSheetWriter   ---   drawTable
**
******************************************************/
void GenericSheetWriter::drawTable( Painter *painter, Table *table, const MRect &refreshRect )
{
	//wxLogMessage( wxT( "START GenericSheetWriter::drawTable refresh x %f y %f w %f h %f" ),
		//refreshRect.x, refreshRect.y, refreshRect.width, refreshRect.height );
	assert( table );
	MRect drawRect;

	double x0, y0;
	TableEntry *e;
	FontProvider *fc = FontProvider::get();
	MRect tableRect = table->rect;

	/*
	 * Table header
	*/
	painter->setTextColor( config->colors->fgColor );
	if ( ! table->header.isEmpty())
	{
		painter->setFont( *fc->getFont( getFontIdForItem( WitHeader ), -table->shrinkrate ));
		drawRect = table->rect;
		drawRect.height = table->header.rect.height;
		//painter->drawRectangle( drawRect );

		tableRect.y += table->header.rect.height;
		tableRect.height -= table->header.rect.height;
		tableRect.height -= Y_EXTRA_HEADER;

		if ( drawRect.intersects( refreshRect ))
		{
			painter->drawMString( drawRect, table->header.tf, Align::Center );
		}
		//contentRect.y += table->header.rect.height;
	}

	// calculate the table frames depending on empty colums
	// first the shadow will be painted, then cells and frames. Finally outer frames
	list<MRect> tableframes;
	{
		// left margin
		double xg0 = tableRect.x;

		// width of individual table frame
		double xgw = 0;
		uint c = 0;
		//double hsegment = 0;
		while( c < table->getNbCols())
		{
			if ( table->isEmptyCol( c ))
			{
				// TODO maximale höhe ausrechnen pro Segment
				tableframes.push_back( MRect( xg0, tableRect.y, xgw, tableRect.height ));
				xg0 = xg0 + xgw + table_empty_col_width;
				xgw = 0;
			}
			else
			{
				xgw += table->col_width[c];
				//for( uint r = 0; r
				//hsegment += table->row_height
			}
			c++;
		}
		if ( xgw > 0 )
		{
			tableframes.push_back( MRect( xg0, tableRect.y, xgw, tableRect.height ));
		}
	}

	y0 = tableRect.y;
	painter->setDefaultPen();

	// first table cell iteration: draw cell background and contents
	for ( uint row = 0; row < table->getNbRows(); row++ )
	{
		x0 = tableRect.x;
		for ( uint col = 0; col < table->getNbCols(); col++ )
		{
			if ( table->isEmptyCol( col ))
			{
				x0 += table->col_width[col];
				continue;
			}

			MRect drawRect( x0, y0, table->col_width[col], table->contents[row].rect.height );
			if ( drawRect.intersects( refreshRect ))
			{
				e = &table->contents[row].value[col];

				/*
				 *  draw cell background
				 *  no specific colors for markup
				 */
				if ( e->isHeader )
				{
					if ( sheetcfg->tablestyle.useHeaderColors )
					{
						painter->setBrush( sheetcfg->tablestyle.headerBgColor );
					}
					else
					{
						painter->setBrush( config->colors->bgColor );
					}
				}
				else // normal cell
				{
					switch( sheetcfg->tablestyle.cellBgMode )
					{
						case 0:
							painter->setBrush( config->colors->bgColor );
						break;
						case 1:
							painter->setBrush( sheetcfg->tablestyle.allCellBgColor );
						break;
						case 2:
							if ( row % 2 ) painter->setBrush( sheetcfg->tablestyle.oddCellBgColor );
						else painter->setBrush( sheetcfg->tablestyle.evenCellBgColor );
						break;
						default:
							wxLogError( wxString::Format( wxT( "wrong table style %d" ), sheetcfg->tablestyle.cellBgMode ));
						break;
					}
				}
				painter->setTransparentPen();
				painter->drawRectangle( x0, y0, table->col_width[col], table->contents[row].rect.height );

				/*
				 *  draw text
				 */
				if ( e->isHeader )
				{
					painter->setFont( *fc->getFont( getFontIdForItem( WitTableHeader ), -table->shrinkrate ));
					painter->setTextColor( sheetcfg->tablestyle.useHeaderColors ?
						sheetcfg->tablestyle.headerTextColor
						: config->colors->fgColor );
				}
				else
				{
					painter->setFont( *fc->getFont( getFontIdForItem( WitTableCell ), -table->shrinkrate ));
					painter->setTextColor( sheetcfg->tablestyle.useCellColors ?
						sheetcfg->tablestyle.cellTextColor
						: config->colors->fgColor );
				}

				// prepare text output
				drawRect = MRect( x0, y0, table->col_width[col], table->contents[row].rect.height );

				const int align = e->isHeader ? Align::Center : table->col_alignment[col];
				if ( align & Align::Left ) drawRect.x += table_cell_align_delta;
				if ( align & Align::Right ) drawRect.width -= table_cell_align_delta;
				painter->drawMString( drawRect, e->text, align );

			} // end if rect and rect intersects draw rect

			x0 += table->col_width[col];

		}
		y0 += table->contents[row].rect.height;
	}

	// second table cell iteration: draw frames
	if ( sheetcfg->tablestyle.useGrid )
	{
		y0 = tableRect.y;
		for ( uint row = 0; row < table->getNbRows(); row++ )
		{
			x0 = tableRect.x;
			for ( uint col = 0; col < table->getNbCols(); col++ )
			{
				if ( table->isEmptyCol( col ))
				{
					x0 += table->col_width[col];
					continue;
				}

				MRect drawRect( x0, y0, table->col_width[col], table->contents[row].rect.height );
				if ( drawRect.intersects( refreshRect ))
				{
					e = &table->contents[row].value[col];
					if ( e->isHeader )
					{
						painter->setDefaultPen();

						// left
						painter->drawLine( x0, y0, x0, y0 + table->contents[row].rect.height );
						painter->drawLine( x0 + table->col_width[col], y0, x0 + table->col_width[col], y0 + table->contents[row].rect.height );

						// top and bottom
						painter->drawLine( x0, y0, x0 + table->col_width[col], y0 );
						painter->drawLine( x0, y0 + table->contents[row].rect.height, x0 + table->col_width[col], y0 + table->contents[row].rect.height );
					}
					else
					{
						painter->setDefaultPen();

						// left
						painter->drawLine( x0, y0, x0, y0 + table->contents[row].rect.height );

						// bottom
						painter->drawLine( x0, y0, x0 + table->col_width[col], y0 );
					}

				} // end if rect and rect intersects draw rect

				x0 += table->col_width[col];
			}
			y0 += table->contents[row].rect.height;
		}
	}

	// table frame
	if ( sheetcfg->tablestyle.useGrid )
	{
		painter->setDefaultPen();
		painter->setTransparentBrush();
		painter->drawRectangle( table->rect );
	}

	//wxLogMessage( wxT( "ENDE GenericSheetWriter::drawTable" ));
}

/*****************************************************
**
**   GenericSheetWriter   ---   formatMString
**
******************************************************/
void GenericSheetWriter::formatMString( Painter *painter, MString &f, const double xmax )
{
	MPoint p;
	MString s;

	f.formattedLines.clear();

#ifdef SHOW_STOP_WATCH
	static wxLongLong totaltime = 0;
	const wxLongLong starttime = wxGetLocalTimeMillis();
#endif

	// x position in current line. one line can contain multiple tokens
	double xcursor = 0;

	// TODO machen und htotal entsorgen
	//double ycursor = 0;

	// hright of current line i.e. maximum height of characters
	double currenth = 0;

	// aummarized width and height of the string. Used for determination of total size
	double xtotal = 0;
	double htotal = 0;

	f.size = painter->getTextExtent( f );


	SheetFormatter ff;
	//printf( "SIZE %f %f contents plain %s\n", f.size.real(), f.size.imag(), str2char( ff.fragment2PlainText( f )));
	if ( xmax == 0 || f.size.real() < xmax )
	{
		//printf( "REturn\n" );
		return;
	}
	
	for( list<MToken>::iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
	{
		// token consists of symbols
		if ( iter->entity != TTSE_NONE )
		{
			p = painter->getTextExtent( *iter );
			currenth = Max( p.imag(), currenth );
			if ( xcursor != 0 && xcursor + p.real() > xmax )
			{
				s.size = MPoint( xcursor, currenth );
				htotal += currenth;
				xtotal = Max( xcursor, xtotal );
				f.formattedLines.push_back( s );
				currenth = 0;
				s.clear();
				s.add( *iter );
				xcursor = p.real();
			}
			else
			{
				s.add( *iter );
				xcursor += p.real();
			}
		}
		else if ( iter->text == SPACE ) // TODO strings with trailing spaces to not work
		{
			//printf( "SINGLE SPACE '%s'\n", str2char( iter->text ));
			s.add( SPACE );
			p = painter->getTextExtent( SPACE );
			xcursor += p.real();
		}
		else // token is a plain string
		{
			wxString token, line, testline;

			// x offset for 1st line only, will be reset on line break
			double xcursor_0 = xcursor;

			//printf( "ITER TEXT '%s'\n", str2char( iter->text ));
			wxStringTokenizer tk( iter->text, SPACE );
			while( tk.HasMoreTokens())
			{
				token = tk.GetNextToken();
				if ( line.IsEmpty()) { testline = token; }
				else
				{
					testline.Clear();
					testline << line << SPACE << token;
				}

				p = painter->getTextExtent( testline );
				currenth = Max( p.imag(), currenth );
				//printf( "Token '%s' extent.x %f xcursor_0 %f xcursor %f\n", str2char( token ), p.real(), xcursor_0, xcursor );

				if ( xcursor_0 + p.real() >= xmax )
				{
					// finish line and start new one
					s.add( line );
					//printf( "BREAK xcursor %f pxpreal %f xmax %f linecount in s %d CHAR %s\n", xcursor, p.real(), xmax, 
						//(int)s.tokens.size(), str2char( line ));
					s.size = MPoint( xcursor, currenth );
					f.formattedLines.push_back( s );
					htotal += currenth;
					xtotal = max( xtotal, xcursor );
					currenth = 0;

					s.clear();
					line = token;
					p = painter->getTextExtent( line );
					currenth = Max( p.imag(), currenth );
					xcursor = p.real();
					xcursor_0 = 0;
				}
				else
				{
					// nein, das ist falsch, die reste aus der alten zeilen werden nicht übernommen
					xcursor = xcursor_0 + p.real();

					// auch falsch, das ist zu viel
					//xcursor += p.real();
					line = testline;
				}
			}
			// tokenizer finished, add rest of line added if enything left
			if ( line.Length() > 0 )
			{
				//printf( "-- end tokenizer, add string %s #### xcursor was %f\n", str2char( line ), xcursor );
				s.size = MPoint( xcursor, currenth );
				s.add( line );
			}

		} // end of otken type query
	} // end of list<token> iteration

	// finally add current string if anything
	if ( ! s.isEmpty())
	{
		//printf( "End Method, add String .... xcursor is %f\n", xcursor );
		s.size = MPoint( xcursor, currenth );
		f.formattedLines.push_back( s );
		xtotal = max( xtotal, xcursor );
		htotal += currenth;
	}
	f.size = MPoint( xtotal, htotal );

#ifdef SHOW_STOP_WATCH
	const wxLongLong duration = wxGetLocalTimeMillis() - starttime;
	totaltime += duration;
	wxLogMessage( wxString::Format( wxT( "GenericSheetWriter::formatMString in %ld millisec, total time %ld" ), duration.ToLong(), totaltime.ToLong() ));
#endif
}

/*****************************************************
**
**   DcSheetWriter   ---   Constructor
**
******************************************************/
DcSheetWriter::DcSheetWriter( Sheet *sheet, SheetConfig *sheetcfg, WriterConfig *writercfg, ColorConfig *colorcfg )
  : GenericSheetWriter( sheet, sheetcfg, writercfg, colorcfg )
{
	table_cell_delta_x = 10;
	table_cell_delta_y = 10;
	table_widget_extra_y = 20;
	table_cell_align_delta = 5;
	table_empty_col_width = 10;
	doCenterAll = false;
	//doCenterAll = true;
}

/*****************************************************
**
**   DcSheetWriter   ---   getFontIdForItem
**
******************************************************/
FONT_ID DcSheetWriter::getFontIdForItem( const SHEET_TEXT_ITEM_SUBTYPE &type )
{
	switch( type )
	{
		case WitHeader:
			return FONT_TEXT_HEADER;
		break;
		case WitParagraph:
		case WitLine:
			return FONT_TEXT_DEFAULT;
		break;
		case WitTableCell:
			return FONT_TEXT_TABLE_CONTENTS;
		break;
		case WitTableHeader:
			return FONT_TEXT_TABLE_HEADER;
		break;
		default:
			assert( false );
		break;
	}
}



