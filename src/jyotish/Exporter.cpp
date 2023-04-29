/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Exporter.cpp
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
#include "Sheet.h"
#include "SheetConfig.h"
#include "Table.h"

extern Config *config;

/*****************************************************
**
**   Exporter   ---   Constructor
**
******************************************************/
Exporter::Exporter( SheetConfig *scfg, WriterConfig *wcfg )
{
	if ( scfg ) { sheetcfg = scfg; }
	else
	{
		SheetConfigLoader *loader = SheetConfigLoader::get();
		sheetcfg = loader->getConfig( config->view->sheetStyle );
	}
	writercfg = wcfg ? wcfg : config->writer;
}

/*************************************************//**
*
*    CLASS    ExporterText
*
******************************************************/
class ExporterText : public Exporter
{
public:

	ExporterText( SheetConfig *scfg, WriterConfig *wcfg ) : Exporter( scfg, wcfg ) {}

	virtual wxString exportSheet( Sheet *sheet )
	{
		s.Clear();

		for( std::list<SheetItem*>::iterator iter = sheet->items.begin(); iter != sheet->items.end(); iter++ )
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
		std::vector<uint> col_len;

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
public:

	ExporterCsv( SheetConfig *scfg, WriterConfig *wcfg ) : ExporterText( scfg, wcfg ) {}

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
Exporter *ExporterFactory::getExporter( const WidgetExportType &type, SheetConfig *scfg, WriterConfig *wcfg )
{
	Exporter *e = (Exporter*)NULL;
	switch( type )
	{
		case WeText:
			e = new ExporterText( scfg, wcfg );
		break;
		case WeHtml:
			e = getHtmlExporter( scfg, wcfg );
		break;
		case WePlainHtml:
			e = getPlainHtmlExporter( scfg, wcfg );
		break;
		case WeCsv:
			e = new ExporterCsv( scfg, wcfg );
		break;
		default:
			assert( false );
		break;
	}

	assert( e );
	return e;
}



