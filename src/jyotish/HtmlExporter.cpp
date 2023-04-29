/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/HtmlExporter.cpp
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

#include "Exporter.h"

#include "Conf.h"
#include "ColorConfig.h"
#include "Lang.h"
#include "SheetConfig.h"
#include "Table.h"

extern Config *config;

/*************************************************//**
*
* 
*
******************************************************/
class PlainHtmlExporter : public Exporter
{
public:

	PlainHtmlExporter( SheetConfig *scfg = (SheetConfig*)NULL, WriterConfig *wcfg = (WriterConfig*)NULL )
	 : Exporter( scfg, wcfg )
	{
	}

	virtual wxString exportSheet( Sheet *sheet )
	{
		s.Clear();

		writeHtmlHead();

		for( std::list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
		{
			SheetItem *item = *iter;
			if ( item->type == WiTable )
			{
				generateTable( (Table*)item );
			}
			else if ( item->type == WiWidget )
			{
				s << wxT( "<h1>WARN: widgets not supported in Html views</h1>" ) << Endl;
			}
			else if ( item->type == WiText )
			{
				SheetTextItem *ti = (SheetTextItem*)item;
				wxString t = textFragment2Html( ti->tf );

				switch( ti->subtype )
				{
					case WitHeader:
						s << wxT( "<h1>" ) << t << wxT( "</h1>" ) << Endl;
					break;
					case WitParagraph:
						s << t << wxT( "<p>" ) << Endl;
					break;
					case WitLine:
						s << t << wxT( "<br>" ) << Endl;
					break;
					default:
						assert( false );
					break;
				}
			}
			else assert( false );
		}
		s << wxT( "</body>" ) << Endl;
		s << wxT( "</html>" ) << Endl;
		return s;
	}

protected:

	/*****************************************************
	**
	**   PlainHtmlExporter   ---   writeHtmlHead
	**
	******************************************************/
	virtual void writeHtmlHead()
	{
		s << wxT( "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//en\">" ) << Endl
			<< wxT( "<html>" ) << Endl
			<< wxT( "<head>" ) << Endl
			<< wxT( "<meta charset=\"utf-8\"/>" ) << Endl
			<< wxT( "</head>" ) << Endl
			<< wxT( "<body>" ) << Endl;
	}

	/*****************************************************
	**
	**   PlainHtmlExporter   ---   writeEntry
	**
	******************************************************/
	void writeEntry( const wxString &entry, const int &len )
	{
		wxString t = entry;
		for ( int i = 0; i < len; i++ ) t.Append( wxT( " " ));
		s << t.Left( len );
	}

	/*****************************************************
	**
	**   PlainHtmlExporter   ---   textFragment2Html
	**
	******************************************************/
	wxString textFragment2Html( const MString f )
	{
		wxString s;
		SheetFormatter formatter( writercfg );
		for( std::list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
		{
			switch( iter->fontFormat )
			{
				case TTFF_SUBSCRPTUM:
					s << wxT( "<sub>" ) << formatter.token2Html( *iter ) << wxT( "</sub>" );
				break;
				case TTFF_SUPERSCRPTUM:
					s << wxT( "<sup>" ) << formatter.token2Html( *iter ) << wxT( "</sup>" );
				break;
				default:
					s << formatter.token2Html( *iter );
				break;
			}
		}
		return s;
	}

	/*****************************************************
	**
	**   PlainHtmlExporter   ---   generateTable
	**
	******************************************************/
	virtual void generateTable( Table *table )
	{
		if ( ! table->header.isEmpty())
		{
			s << wxT( "<h1>" ) << textFragment2Html( table->header.tf ) << wxT( "</h1>" ) << Endl;
		}

		uint c, r;
		TableEntry *entry;
		Row row( table, table->getNbCols() );

		s << wxT( "	<table>" ) << Endl;

		for ( r = 0; r < (unsigned)table->getNbRows(); r++ )
		{
			row = table->contents[r];
			if ( row.isEmpty() ) continue;

			s << wxT( "		<tr>" ) << Endl;
			for ( c = 0; c < table->getNbCols(); c++ )
			{
				entry = &row.value[c];

				if ( entry->isHeader )
				{
					s << wxT( "		<th>" ) << textFragment2Html( entry->text ) << wxT ( "</th>" ) << Endl;
				}
				else if ( table->isEmptyCol( c ))
				{
					s << wxT( "			<td width=10></td>" ) << Endl;
				}
				else
				{
					s << wxT( "			<td>" ) << textFragment2Html( entry->text )  << wxT ( "</td>" ) << Endl;
				}
			}
			s << wxT( "		</tr>" ) << Endl;
		}
		s << wxT( "	</table><p>" ) << Endl;
	}

};


/*****************************************************
**
**   ExporterFactory   ---   getPlainHtmlExporter
**
******************************************************/
Exporter *ExporterFactory::getPlainHtmlExporter( SheetConfig *sheetcfg, WriterConfig *writercfg )
{
	return new PlainHtmlExporter( sheetcfg, writercfg );
}

/*************************************************//**
*
* 
*
******************************************************/
class HtmlExporter : public PlainHtmlExporter
{
public:

	HtmlExporter( SheetConfig *scfg = (SheetConfig*)NULL, WriterConfig *wcfg = (WriterConfig*)NULL )
	 : PlainHtmlExporter( scfg, wcfg )
	{
	}

protected:

	/*****************************************************
	**
	**   HtmlExporter   ---   writeHtmlHead
	**
	******************************************************/
	virtual void writeHtmlHead()
	{
		s << wxT( "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//en\">" ) << Endl
			<< wxT( "<html>" ) << Endl
			<< wxT( "<head/>" ) << Endl
			<< wxT( "<meta charset=\"utf-8\"/>" ) << Endl
			<< wxT( "<body " )
			<< wxT( " bgcolor=" ) << config->colors->bgColor.GetAsString( wxC2S_HTML_SYNTAX )
			<< wxT( " text=" ) << config->colors->fgColor.GetAsString( wxC2S_HTML_SYNTAX )
			<< wxT( ">" ) << Endl;
	}

	/*****************************************************
	**
	**   HtmlExporter   ---   generateTable
	**
	******************************************************/
	virtual void generateTable( Table *table )
	{
		if ( ! table->header.isEmpty())
		{
			s << wxT( "<h1>" ) << textFragment2Html( table->header.tf ) << wxT( "</h1>" ) << Endl;
		}

		uint c, r;
		TableEntry *entry;
		Row row( table, table->getNbCols() );
		wxString cellColor;

		if ( sheetcfg->tablestyle.useGrid ) s << wxT( " <table border=1 cellspacing=0>" ) << Endl;
		else s << wxT( " <table>" ) << Endl;
		s << wxT( "		<tr>" ) << Endl;

		for ( r = 0; r < (unsigned)table->getNbRows(); r++ )
		{
			row = table->contents[r];
			switch( sheetcfg->tablestyle.cellBgMode )
			{
				case 1:
					cellColor = sheetcfg->tablestyle.allCellBgColor.GetAsString( wxC2S_HTML_SYNTAX );
				break;
				case 2:
					if ( r % 2 )
						cellColor = sheetcfg->tablestyle.oddCellBgColor.GetAsString( wxC2S_HTML_SYNTAX );
					else
						cellColor = sheetcfg->tablestyle.evenCellBgColor.GetAsString( wxC2S_HTML_SYNTAX );
				break;
				default:
				break;
			}

			if ( row.isEmpty() ) continue;

			s << wxT( "   <tr>" ) << Endl;
			for ( c = 0; c < table->getNbCols(); c++ )
			{
				entry = &row.value[c];

				if ( entry->isHeader )
				{
					s << wxT( "<th nowrap" );

					if ( sheetcfg->tablestyle.useHeaderColors )
					{
						s << wxT( " bgcolor=" ) << sheetcfg->tablestyle.headerBgColor.GetAsString( wxC2S_HTML_SYNTAX );
						s << wxT( "><font color=\"") << sheetcfg->tablestyle.headerTextColor.GetAsString( wxC2S_HTML_SYNTAX ) << wxT("\">" );
						s << textFragment2Html( entry->text ) << wxT ( "</font>" ) << Endl;
					}
					else
					{
						s << wxT("\">" ) << textFragment2Html( entry->text );
					}
					s << wxT ( "</th>" ) << Endl;
				}
				else if ( table->isEmptyCol( c ))
				{
					s << wxT( "<td width=10></td>" ) << Endl;
				}
				else
				{
					wxString align = wxT( "center" );
					if ( table->col_alignment[c] & Align::Right ) align = wxT( "right" );
					else if ( table->col_alignment[c] & Align::Left ) align = wxT( "left" );

					s << wxT( "<td" );
					if ( ! table->col_break[c] ) s << wxT( " nowrap" );
					if ( sheetcfg->tablestyle.useCellColors )
					{
						if ( ! cellColor.IsEmpty()) s << wxT( "  bgcolor=" ) << cellColor;
						s << wxT( " align=" ) << align
							<< wxT( "><font color=\"") << sheetcfg->tablestyle.cellTextColor.GetAsString( wxC2S_HTML_SYNTAX ) << wxT("\">" )
							<< textFragment2Html( entry->text )  << wxT ( "</font></td>" ) << Endl;
					}
					else
					{
						s << wxT( " align=" ) << align << wxT( ">" )
							<< textFragment2Html( entry->text )  << wxT ( "</td>" ) << Endl;
					}
				}
			}
			s << wxT( "   </tr>" ) << Endl;
		}
		s << wxT( "	</table><p>" ) << Endl;
	}

};

/*****************************************************
**
**   ExporterFactory   ---   getHtmlExporter
**
******************************************************/
Exporter *ExporterFactory::getHtmlExporter( SheetConfig *sheetcfg, WriterConfig *writercfg )
{
	return new HtmlExporter( sheetcfg, writercfg );
}



