/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/HtmlExporter.cpp
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
class HtmlExporter : public Exporter
{
public:

	HtmlExporter()
	{
		SheetConfigLoader *loader = SheetConfigLoader::get();
		sheetcfg = loader->getConfig( config->view->sheetStyle );
	}

	virtual wxString exportSheet( Sheet *sheet )
	{
		s.Clear();

		s << wxT( "<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.0 Transitional//en\">" ) << Endl;
		s << wxT( "<html>" ) << Endl;
		s << wxT( "<head/>" ) << Endl;
		s << wxT( "<meta charset=\"utf-8\"/>" ) << Endl;
		s << wxT( "<body " );
		s << wxT( " bgcolor=" ) << config->colors->bgColor.GetAsString( wxC2S_HTML_SYNTAX );
		s << wxT( " text=" ) << config->colors->fgColor.GetAsString( wxC2S_HTML_SYNTAX );
		s << wxT( ">" ) << Endl;

		for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
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
				SheetFormatter fmt;
				wxString t = fmt.fragment2PlainText( ti->tf );

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

	/*****************************************************
	**
	**   HtmlExporter   ---   writeEntry
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
	**   HtmlExporter   ---   generateTable
	**
	******************************************************/
	void generateTable( Table *table )
	{
		/*
		TableEntry *e;
		for ( uint r = 0; r < table->getNbRows(); r++ )
		{
			for ( uint c = 0; c < table->getNbCols(); c++ )
			{
				e = &table->contents[r].value[c];
			}
		}
		*/
		generateTableIntern( table, sheetcfg->tablestyle.gridStyle );
	}

	/*****************************************************
	**
	**   HtmlExporter   ---   textFragment2Html
	**
	******************************************************/
	wxString textFragment2Html( const MString f )
	{
		wxString s;
		SheetFormatter formatter;
		for( list<MToken>::const_iterator iter = f.tokens.begin(); iter != f.tokens.end(); iter++ )
		{
			s << formatter.token2Html( *iter );
		}
		return s;
	}

	/*****************************************************
	**
	**   HtmlExporter   ---   generateTableIntern
	**
	******************************************************/
	void generateTableIntern( Table *table, const bool &frame )
	{
		uint c, r;
		TableEntry *entry;
		Row row( table, table->getNbCols() );
		wxString cellColor;

		if ( frame ) s << wxT( " <table border=1 cellspacing=0>" ) << Endl;
		else s << wxT( " <table>" ) << Endl;
		s << wxT( "		<tr>" ) << Endl;

		for ( r = 0; r < (unsigned)table->getNbRows(); r++ )
		{
			row = table->contents[r];
			switch( sheetcfg->tablestyle.cellBgMode )
			{
				case 1:
					cellColor = sheetcfg->tablestyle.cellBrush.color.GetAsString( wxC2S_HTML_SYNTAX );
				break;
				case 2:
					if ( r % 2 )
						cellColor = sheetcfg->tablestyle.oddCellBrush.color.GetAsString( wxC2S_HTML_SYNTAX );
					else
						cellColor = sheetcfg->tablestyle.evenCellBrush.color.GetAsString( wxC2S_HTML_SYNTAX );
				break;
				default:
				break;
			}

			// look if it's empty
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
						s << wxT( " bgcolor=" ) << sheetcfg->tablestyle.headerBrush.color.GetAsString( wxC2S_HTML_SYNTAX );
						s << wxT( "><font color=\"") << sheetcfg->tablestyle.headerTextColor.GetAsString( wxC2S_HTML_SYNTAX ) << wxT("\">" );
						//s << entry.value << wxT ( "</font>" ) << Endl;
						s << textFragment2Html( entry->text ) << wxT ( "</font>" ) << Endl;
					}
					else
					{
						//s << wxT("\">" ) << entry.value;
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

					if ( sheetcfg->tablestyle.cellBgMode )
					{
						s << wxT( "<td" )
							<< wxT( "  bgcolor=" ) << cellColor
							<< wxT( " align=" ) << align
							<< wxT( "><font color=\"") << sheetcfg->tablestyle.cellTextColor.GetAsString( wxC2S_HTML_SYNTAX ) << wxT("\">" )
							//<< entry.value  << wxT ( "</font></td>" ) << Endl;
							<< textFragment2Html( entry->text )  << wxT ( "</font></td>" ) << Endl;
					}
					else
					{
						//s << wxT( "<td " ) << wrap << wxT( " align=" ) << align << wxT( ">" ) << entry.value  << wxT ( "</td>" ) << Endl;
						s << wxT( "<td align=" ) << align << wxT( ">" )
							<< textFragment2Html( entry->text )  << wxT ( "</td>" ) << Endl;
					}
				}
			}
			s << wxT( "   </tr>" ) << Endl;
		}
		s << wxT( "	</table><p>" ) << Endl;
	}

private:
	SheetConfig *sheetcfg;
};


/*****************************************************
**
**   ExporterFactory   ---   getHtmlExporter
**
******************************************************/
Exporter *ExporterFactory::getHtmlExporter()
{
	return new HtmlExporter;
}



