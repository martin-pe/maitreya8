/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Exporter.cpp
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

#include "Sheet.h"
#include "Table.h"

/*************************************************//**
*
*    CLASS    ExporterText
*
******************************************************/
class ExporterText : public Exporter
{
public:

	virtual wxString exportSheet( Sheet *sheet )
	{
		s.Clear();

		for( list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
		{
			SheetItem *item = *iter;
			SheetFormatter fmt;

			if ( item->type == WiTable )
			{
				generateTable( (Table*)item );
			}
			else if ( item->type == WiWidget )
			{
				s << wxT( "WARN: widgets not supported in text views" ) << Endl;
			}
			else if ( item->type == WiText )
			{
				SheetTextItem *ti = (SheetTextItem*)item;
				switch( ti->subtype )
					{
					case WitHeader:
					{
						wxString t = fmt.fragment2PlainText( ti->tf );
						s << t << Endl;
						for ( uint i = 0; i < t.Len(); i++ ) s << wxT( "-" );
						s << Endl << Endl;
					}
					break;
					case WitParagraph:
					case WitLine:
						s << fmt.fragment2PlainText( ti->tf ) << Endl;
					break;
					default:
						assert( false );
					break;
				}
			}
			else assert( false );
		}
		return s;
	}

protected:

	/*****************************************************
	**
	**   ExporterText   ---   writeEntry
	**
	******************************************************/
	void writeEntry( const wxString &t, const int &len )
	{
		s << t;
		for ( int i = t.Length() + 1; i < len; i++ ) s << wxT( " " );
	}

	/*****************************************************
	**
	**   ExporterText   ---   generateTable
	**
	******************************************************/
	virtual void generateTable( Table *table )
	{
		uint i, c, r;
		uint total_len = 0;
		vector<uint> col_len;

		TableEntry *entry;
		SheetFormatter formatter;
		wxString tt;

		// write table header if necessary
		if ( ! table->header.isEmpty() )
		{
			tt = formatter.fragment2PlainText( table->header.tf );
			s << tt << Endl;
			for ( i = 0; i < tt.Len(); i++ ) s << wxT( "-" );
			s << Endl;
		}

		for ( c = 0; c < table->getNbCols(); c++ ) col_len.push_back( 0 );

		for ( r = 0; r < table->getNbRows(); r++ )
		{
			for ( c = 0; c < table->getNbCols(); c++ )
			{
				entry = &table->contents[r].value[c];
				entry->plainText = formatter.fragment2PlainText( entry->text );
				col_len[c] = Max( col_len[c], entry->plainText.Length());
			}
		}

		for ( c = 0; c < table->getNbCols(); c++ )
		{
			col_len[c] += 2;
			total_len += col_len[c];
		}

		// top line with column headers
		for ( c = 0; c < table->getNbCols(); c++ )
		{
			entry = &table->contents[0].value[c];
			writeEntry( entry->plainText, col_len[c] );
		}
		s << Endl;

		// separate first line
		for ( i = 0; i < total_len; i++ ) s << wxT( "-" );
		s << Endl;

		for ( r = 1; r < table->getNbRows(); r++ )
		{
			for ( c = 0; c < table->getNbCols(); c++ )
			{
				entry = &table->contents[r].value[c];
				writeEntry( entry->plainText, col_len[c] );
			}
			s << Endl;
		}
		s << Endl;
	}
};

/*************************************************//**
*
*    CLASS    ExporterCsv
*
******************************************************/
class ExporterCsv : public ExporterText
{
protected:

	/*****************************************************
	**
	**   ExporterCsv   ---   generateTable
	**
	******************************************************/
	virtual void generateTable( Table *table )
	{
		uint c, r;

		TableEntry *entry;
		SheetFormatter formatter;

		// write table header if necessary
		if ( ! table->header.isEmpty() )
		{
			s << formatter.fragment2PlainText( table->header.tf ) << Endl;
		}

		for ( r = 0; r < table->getNbRows(); r++ )
		{
			for ( c = 0; c < table->getNbCols(); c++ )
			{
				entry = &table->contents[r].value[c];
				entry->plainText = formatter.fragment2PlainText( entry->text );
				s << entry->plainText;
				if ( c < table->getNbCols() - 1 ) s << CSV_DELIMITER;
			}
			s << Endl;
		}
		s << Endl;
	}
};

/*****************************************************
**
**   ExporterFactory   ---   getExporter
**
******************************************************/
Exporter *ExporterFactory::getExporter( const WidgetExportType &type )
{
	Exporter *e = (Exporter*)NULL;
	switch( type )
	{
		case WeText:
			e = new ExporterText;
		break;
		case WeHtml:
			e = getHtmlExporter();
		break;
		case WeCsv:
			e = new ExporterCsv;
		break;
		default:
			assert( false );
		break;
	}

	assert( e );
	return e;
}



