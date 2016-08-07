/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/SheetWriter.cpp
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
#define Y_EXTRA_HEADER 1.2

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

	// general config
	doFloatingLayout = true;
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
	wxLogMessage( wxT( "START GenericSheetWriter::preformat xrightmax %f" ), xrm );
	xrightmax = xrm;

	//printf( "GenericSheetWriter::preformat xrightmax %f item count %d\n", xrightmax, sheet->items.size() );
	FontProvider *fc = FontProvider::get();
	FONT_ID fontid;
	double h, lineheight;

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
	* Schritte;
	*  - Nur die Breiten formatieren
	*  - Die Breiten der Widget kommen nach den Tables und TextItems (mit Höhe), dann sind all Rects komplett bis auf y
	*  - Die Rects umfassen nur y=0
	*  - Höhen im zweiten Schritt
	*  - Seitenümbrüche nicht vergessen
	*  
	*  
	*  
	*/

	xSizeContents = 0;
	//xSizeContents = xrightmax;
	contentRect.width = xrightmax;
	numcols = 1;

	// 1st step: determine width of tables and text items
	for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
	{
		SheetItem *item = *iter;
		switch( item->type )
		{
			case WiTable:
				preformatTable( painter, (Table*)item );
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
				item->rect = MRect( contentRect.x, 0, ti->tf.size.real(), h );
			}
			break;
			case WiWidget:
			{
				SheetWidgetItem *w = (SheetWidgetItem*)(*iter);
				//printf( "WIDGET contentRect.width %f contentRect.height %f xSizeContents %f\n",
					//contentRect.width, contentRect.height, xSizeContents );
				w->preformat();

				// check if xmin is set. widget must have this minimal width
				double ww = w->xmin ? w->xmin : contentRect.width;
				xSizeContents = Max( ww, xSizeContents );

				// y may be fixed if yfixed is set. otherwise it will be calculated from x and ratio
				double yy = w->yfixed ? w->yfixed : ww * w->ratio;
				w->rect = wxRect( contentRect.x, 0, ww, yy );
			}
			break;
			default:
			break;
		}
	}

	// 3rd step format items in floating layout mode
	if ( sheet->items.size() > 1 && doFloatingLayout )
	{
		if ( xSizeContents > 0 && contentRect.width >= 2 * xSizeContents )
		{
			numcols = (int)( contentRect.width / xSizeContents );
		}
		if ( numcols > 1 )
		{
			const double colwidth = contentRect.width / numcols;
			//printf( "COLS: %d xSizeContents %f contentRect.width %f xrightmax %fcolwidth %f\n",
				//numcols, xSizeContents, contentRect.width, xrightmax, colwidth );

			ycursor = contentRect.y;
			int currentcol = 0;
			double groupMaxHeight = 0;
			for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
			{
				SheetItem *item = *iter;
				item->rect.y = ycursor;
				item->rect.x = contentRect.x + currentcol * colwidth;
				//printf( "COLS %f\n", item->rect.x );

				// some widget items can be enlarged
				if ( item->type == WiWidget )
				{
					if ( item->rect.width < colwidth )
					{
						SheetWidgetItem *wi = (SheetWidgetItem*)item;
						item->rect.width = Min( wi->xmax, colwidth );
						item->rect.height = wi->ratio * item->rect.width;
					}
				}
				// tables must reformat subitem rectangles
				else if ( item->type == WiTable )
				{
					Table *table = (Table*)item;
					table->calculateSubitemRectangles();
				}

				groupMaxHeight = Max( groupMaxHeight, item->rect.height );

				currentcol++;
				if ( currentcol >= numcols )
				{
					ycursor += groupMaxHeight;
					ycursor += table_widget_extra_y;
					groupMaxHeight = 0;
					currentcol = 0;
				}
			}

			// add y space if a non empty row still exists
			if ( groupMaxHeight > 0 )
			{
				ycursor += groupMaxHeight;
				ycursor += table_widget_extra_y;
			}
		}
	}

	// final formatting if floating layout is disabled or multi column mot possible
	if ( numcols == 1 )
	{
		// 4th step: height and center, if required
		for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
		{
			SheetItem *item = *iter;
			ycursor += item->rect.height;

			//if ( doCenterAll )
			if ( doCenterAll && ( item->type == WiTable || item->type == WiWidget ) )
			{
				item->rect.x += .5 * xrightmax - .5 * item->rect.width;
			}
		}

		// 5th step: set rect.y values
		ycursor = contentRect.y;
		for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
		{
			SheetItem *item = *iter;
			item->rect.y = ycursor;
			if ( item->type == WiTable )
			{
				ycursor += item->rect.height + table_widget_extra_y;
				Table *table = (Table*)item;
				table->calculateSubitemRectangles();
			}
			else if ( item->type == WiText )
			{
				ycursor += item->rect.height; // + table_widget_extra_y;
			}
			else if ( item->type == WiWidget )
			{
				ycursor += item->rect.height;
			}
			else assert( false );
		}
	}

#ifdef SHOW_STOP_WATCH
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "GenericSheetWriter::preformat in %ld millisec" ), totaltime.ToLong() ));
#endif
}

/*****************************************************
**
**   GenericSheetWriter   ---   preformatTable
**
******************************************************/
void GenericSheetWriter::preformatTable( Painter *painter, Table *table )
{
	wxLogMessage( wxT( "START GenericSheetWriter::preformatTable" ));
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
		painter->setFont( *fc->getFont( getFontIdForItem( WitHeader )));
		p = painter->getTextExtent( table->header.tf );
		xSizeContents = Max( p.real(), xSizeContents );
		table->header.rect.height = Y_EXTRA_HEADER * p.imag();

		wxFont fff = *fc->getFont( getFontIdForItem( WitHeader ));
		printf( "HEADER size %f size %d\n", p.imag(), fff.GetPointSize());
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
				painter->setFont( *fc->getFont( getFontIdForItem( e->isHeader ? WitTableHeader : WitTableCell )));
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
				const double shrinkrate = 1 - shrinktarget / yvol;
				//printf( "Shrink target %f shrinkrate %f\n", shrinktarget, shrinkrate );

				for ( uint col = 0; col < table->getNbCols(); col++ )
				{
					if ( table->col_break[col] )
					{
						//printf( "COL %d YES target value calculated %f\n", col, table->col_width[col] * shrinkrate );
						table->col_width[col] = Max( table->col_width[col] * shrinkrate, 200 ); 
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

	printf( "FINISHED GenericSheetWriter::preformatTable tableWidth %f tableHeight %f\n", table->rect.width, table->rect.height );
	wxLogMessage( wxT( "ENDE GenericSheetWriter::preformatTable" ));
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

	FontProvider *fc = FontProvider::get();
	FONT_ID fontid;
	//printf( "GenericSheetWriter::paintPage contentRect.x %f contentRect.y %f wirter has %lu items\n",
		//contentRect.x, contentRect.y, sheet->items.size() );

	for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
	{
		SheetItem *item = (SheetItem*)*iter;
		assert( item );

		handlePageBreak( item );

		switch( item->type )
		{
			case WiTable:
				if  ( refreshRect.intersects( item->rect ))
				{
					drawTable( painter, (Table*)item, refreshRect );
				}
			break;
			case WiWidget:
			{
				SheetWidgetItem *wi = (SheetWidgetItem*)item;
				wi->doPaint( painter, refreshRect );
			}
			break;
			case WiText:
			{
				SheetTextItem *ti = (SheetTextItem*)item;
				if ( ti->hasMarkup() )
				{
					painter->setBrush( sheetcfg->selectedItemBrush );
					painter->drawRectangle( ti->rect );
				}

				// draw text item
				fontid = getFontIdForItem( ti->subtype );
				painter->setFont( *fc->getFont( fontid ));
				painter->setTextColor( sheetcfg->textColor );
				painter->drawMString( ti->rect, ti->tf, Align::Left | Align::Top );
			}
			break;
			default:
				printf( "Wrong writer item type %d\n", (int)item->type );
				assert( false );
			break;
		}
	}
#ifdef SHOW_STOP_WATCH
	const wxLongLong totaltime = wxGetLocalTimeMillis() - starttime;
	wxLogMessage( wxString::Format( wxT( "GenericSheetWriter::drawSheet in %ld millisec" ), totaltime.ToLong() ));
#endif
}

/*****************************************************
**
**   GenericSheetWriter   ---   drawTable
**
******************************************************/
void GenericSheetWriter::drawTable( Painter *painter, Table *table, const MRect &refreshRect )
{
	wxLogMessage( wxT( "START GenericSheetWriter::drawTable refresh x %f y %f w %f h %f" ),
		refreshRect.x, refreshRect.y, refreshRect.width, refreshRect.height );
	assert( table );
	MRect drawRect;

	double x0, y0;
	TableEntry *e;
	FontProvider *fc = FontProvider::get();

	//TableStyle *style = sheetcfg->tablestle;
	//assert( style );

	MRect tableRect = table->rect;

	/*
	 * Table header
	*/
	painter->setTextColor( sheetcfg->textColor );
	if ( ! table->header.isEmpty())
	{
		painter->setFont( *fc->getFont( getFontIdForItem( WitHeader )));
		drawRect = table->rect;
		drawRect.height = table->header.rect.height;

		tableRect.y += table->header.rect.height;
		tableRect.height -= table->header.rect.height;
		tableRect.height -= Y_EXTRA_HEADER;

		if ( drawRect.intersects( refreshRect ))
		{
			SheetFormatter fmt;

			if ( sheetcfg->selectionMode != 0 && table->markedItem == &table->header )
			{
				// dummy frame TODO delete or uncomment
				//painter->setPen( wxPen( *wxCYAN, 1 ));

				painter->setBrush( sheetcfg->selectedItemBrush );
				painter->drawRectangle( drawRect );
			}
			painter->drawMString( drawRect, table->header.tf, Align::Center );
		}
		contentRect.y += table->header.rect.height;
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
		while( c < table->getNbCols())
		{
			if ( table->isEmptyCol( c ))
			{
				tableframes.push_back( MRect( xg0, tableRect.y, xgw, tableRect.height ));
				xg0 = xg0 + xgw + table_empty_col_width;
				xgw = 0;
			}
			else { xgw += table->col_width[c]; }
			c++;
		}
		if ( xgw > 0 )
		{
			tableframes.push_back( MRect( xg0, tableRect.y, xgw, tableRect.height ));
		}
	}

	// draw table shadows
	if ( sheetcfg->tablestyle.shadowStyle && sheetcfg->tablestyle.shadowWidth != 0 )
	{
		for( list<MRect>::iterator iter = tableframes.begin(); iter != tableframes.end(); iter++ )
		{
			drawTableShadow( painter, &sheetcfg->tablestyle, *iter );
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
				bool domark = false;
				if ( table->markedItem )
				{
					TableEntry *tme = wxDynamicCast(table->markedItem, TableEntry );
					if ( tme )
					{
						switch( sheetcfg->selectionMode )
						{
							case 1: // cell
								if ( tme->rowid == row && tme->colid == col ) domark = true;
							break;
							case 2: // row
								if ( tme->rowid == row ) domark = true;
							break;
							case 3: // row/col
								if ( tme->isHeader && ( tme->rowid == 0 || tme->rowid == table->getNbRows() - 1 ))
								{
									if ( tme->colid == col ) domark = true;
								}
								else
								{
									if ( tme->rowid == row ) domark = true;
								}
							break;
							case 0: 
							default:
							break;
						}
					}
					else // SheetTextItem = header
					{
						//printf( "ELSE domerk %d\n", domark );
						//domark = true;
					}
					//domark = ( e == me );
				}

				if ( domark )
				{
					painter->setBrush( sheetcfg->selectedItemBrush );
				}
				else 
				{
					if ( e->isHeader )
					{
						if ( sheetcfg->tablestyle.useHeaderColors )
						{
							painter->setBrush( sheetcfg->tablestyle.headerBrush );
						}
						else
						{
							painter->setBrush( sheetcfg->brush );
						}
					}
					else // normal cell
					{
						switch( sheetcfg->tablestyle.cellBgMode )
						{
							case 0:
								//painter->setBrush( config->colors->bgColor );
								painter->setBrush( sheetcfg->brush );
							break;
							case 1:
								painter->setBrush( sheetcfg->tablestyle.cellBrush );
							break;
							case 2:
								if ( row % 2 ) painter->setBrush( sheetcfg->tablestyle.oddCellBrush );
							else painter->setBrush( sheetcfg->tablestyle.evenCellBrush );
							break;
							default:
								wxLogError( wxString::Format( wxT( "wrong table style %d" ), sheetcfg->tablestyle.cellBgMode ));
							break;
						}
					}
				}
				painter->setTransparentPen();
				painter->drawRectangle( x0, y0, table->col_width[col], table->contents[row].rect.height );

				/*
				 *  draw text
				 */
				if ( e->isHeader )
				{
					painter->setFont( *fc->getFont( getFontIdForItem( WitTableHeader )));
					painter->setTextColor( sheetcfg->tablestyle.useHeaderColors ? sheetcfg->tablestyle.headerTextColor : sheetcfg->textColor );
				}
				else
				{
					painter->setFont( *fc->getFont( getFontIdForItem( WitTableCell )));
					painter->setTextColor( sheetcfg->tablestyle.useCellColors ? sheetcfg->tablestyle.cellTextColor : sheetcfg->textColor );
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
	if ( sheetcfg->tablestyle.gridStyle )
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
						//if ( sheetcfg->tablestyle.useHeaderColors )
						painter->setPen( sheetcfg->tablestyle.headerBorderPen );
						//else painter->setDefaultPen();

						// left
						if ( sheetcfg->tablestyle.headerBorderStyle == 1 || sheetcfg->tablestyle.headerBorderStyle ==  3 )
						{
							painter->drawLine( x0, y0, x0, y0 + table->contents[row].rect.height );
							painter->drawLine( x0 + table->col_width[col], y0, x0 + table->col_width[col], y0 + table->contents[row].rect.height );
						}

						// top and bottom
						if ( sheetcfg->tablestyle.headerBorderStyle == 2 || sheetcfg->tablestyle.headerBorderStyle ==  3  )
						{
							painter->drawLine( x0, y0, x0 + table->col_width[col], y0 );
							painter->drawLine( x0, y0 + table->contents[row].rect.height, x0 + table->col_width[col], y0 + table->contents[row].rect.height );
						}
					}
					else
					{
						//if ( sheetcfg->tablestyle.useCellColors )
						painter->setPen( sheetcfg->tablestyle.cellBorderPen );
						//else painter->setDefaultPen();

						// top
						//painter->drawLine( x0, y0, x0 + table->col_width[col], y0 );

						// left
						if (( sheetcfg->tablestyle.cellBorderStyle == 1 || sheetcfg->tablestyle.cellBorderStyle ==  3 ) && col > 0 )
							painter->drawLine( x0, y0, x0, y0 + table->contents[row].rect.height );

						// bottom
						if (( sheetcfg->tablestyle.cellBorderStyle == 2 || sheetcfg->tablestyle.cellBorderStyle ==  3 )) // && row < table->getNbRows() - 1 )
							painter->drawLine( x0, y0, x0 + table->col_width[col], y0 );
					}

				} // end if rect and rect intersects draw rect

				x0 += table->col_width[col];
			}
			y0 += table->contents[row].rect.height;
		}
	}

	// draw outer table frame
	if ( sheetcfg->tablestyle.gridStyle )
	{
		for( list<MRect>::iterator iter = tableframes.begin(); iter != tableframes.end(); iter++ )
		{
			drawTableFrame( painter, &sheetcfg->tablestyle, *iter );
		}
	}
	wxLogMessage( wxT( "ENDE GenericSheetWriter::drawTable" ));
}

/*****************************************************
**
**   GenericSheetWriter   ---   drawTableShadow
**
******************************************************/
void GenericSheetWriter::drawTableShadow( Painter *painter, TableStyle *style, MRect &r )
{
	assert( style );
	if ( style->shadowStyle && style->shadowWidth != 0 )
	{
		painter->setTransparentPen();
		painter->setBrush( style->shadowBrush );

		if ( style->shadowWidth > 0 )
		{
			painter->drawRectangle( MRect( r.x + r.width, r.y + style->shadowWidth, style->shadowWidth, r.height ), cornerRadius );
			painter->drawRectangle( MRect( r.x + style->shadowWidth, r.y + r.height, r.width, style->shadowWidth ), cornerRadius );
		}
		else
		{
			painter->drawRectangle( MRect( r.x + style->shadowWidth, r.y + style->shadowWidth, - style->shadowWidth, r.height ), cornerRadius );
			painter->drawRectangle( MRect( r.x + style->shadowWidth, r.y + style->shadowWidth, r.width, - style->shadowWidth ), cornerRadius );
		}
	}
}

/*****************************************************
**
**   GenericSheetWriter   ---   drawTableFrame
**
******************************************************/
void GenericSheetWriter::drawTableFrame( Painter *painter, TableStyle *style, MRect &r )
{
	assert( style );
	if ( ! style->gridStyle ) return;

	painter->setPen( style->outerBorderPen );
	painter->setTransparentBrush();
	switch( style->outerBorderStyle )
	{
		case 1: // left right
			painter->drawLine( r.x, r.y, r.x, r.y + r.height );
			painter->drawLine( r.x + r.width, r.y, r.x + r.width, r.y + r.height );
		break;
		case 2: // opt bottom
			painter->drawLine( r.x, r.y, r.x + r.width, r.y );
			painter->drawLine( r.x, r.y + r.height, r.x + r.width, r.y + r.height );
		break;
		case 3: // all
			painter->drawRectangle( r ); //, cornerRadius );
			/*
			painter->drawLine( r.x, r.y, r.x, r.y + r.height );
			painter->drawLine( r.x + r.width, r.y, r.x + r.width, r.y + r.height );
			painter->drawLine( r.x, r.y, r.x + r.width, r.y );
			painter->drawLine( r.x, r.y + r.height, r.x + r.width, r.y + r.height );
			*/
		break;
		case 0:
			// nothing
		break;
		default:
			assert( false );
		break;
	}
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
		else // token is a plain string
		{
			wxString token, line, testline;

			// x offset for 1st line only, will be reset on line break
			double xcursor_0 = xcursor;

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
				//printf( "Token %s extent.x %f xcursor_0 %f xcursor %f\n", str2char( token ), p.real(), xcursor_0, xcursor );

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



