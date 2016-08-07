/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/SheetConfig.cpp
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

#include "SheetConfig.h"

#include "Conf.h"
#include "ColorConfig.h"
#include "FileConfig.h"
#include "JSonTool.h"
#include "maitreya.h"

#include <wx/filename.h>
#include <wx/jsonwriter.h>
#include <wx/log.h>
#include <wx/string.h>
#include <wx/wfstream.h>
#include <math.h>

extern Config *config;

/*****************************************************
**
**   SheetConfig   ---   Constructor
**
******************************************************/
SheetConfig::SheetConfig()
{
  name =  _( "Default" );

	textColor = config->colors->fgColor;
	brush = MBrush( config->colors->bgColor );

	selectionMode = 3;
	selectedItemBrush = MBrush( *wxLIGHT_GREY );
}

/*****************************************************
**
**   SheetConfig   ---   Constructor
**
******************************************************/
SheetConfig::SheetConfig( wxString n )
 : name( n )
{
	textColor = config->colors->fgColor;
	brush = MBrush( config->colors->bgColor );

	selectionMode = 2;
	selectedItemBrush = MBrush( *wxLIGHT_GREY );
}

/*****************************************************
**
**   SheetConfig   ---   Constructor
**
******************************************************/
/*
SheetConfig::SheetConfig( const SheetConfig &s )
{
	printf( "SheetConfig::SheetConfig COPY\n" );
	name = s.name;

	textColor = s.textColor;
	brush = s.brush;
	selectionMode = s.selectionMode;
	selectedItemBrush = s.selectedItemBrush;

	tablestyle = s.tablestyle;
}
*/

/*****************************************************
**
**   SheetConfig   ---   operator=
**
******************************************************/
/*
void SheetConfig::operator=( const SheetConfig &s )
{
	printf( "SheetConfig::operator=\n" );
	name = s.name;

	textColor = s.textColor;
	brush = s.brush;
	selectionMode = s.selectionMode;
	selectedItemBrush = s.selectedItemBrush;

	//tablestyle = new TableStyle( *s.tablestyle );
	*tablestyle = *s.tablestyle;
}
*/

/*****************************************************
**
**   SheetConfig   ---   Destructor
**
******************************************************/
SheetConfig::~SheetConfig()
{
#ifdef DEB_VIEWMODE_DESTRUCTORS
	cout << "Destructor SheetConfig" << Endl;
#endif
	//delete tablestyle;
}

/*****************************************************
**
**   TableStyle   ---   Constructor
**
******************************************************/
TableStyle::TableStyle()
{
	useHeaderColors = true;
	headerTextColor = config->colors->fgColor;
	headerBrush = MBrush( wxColour( wxT( "#92BCD5" )));

	useCellColors = true;
	cellTextColor = config->colors->fgColor;

	cellBgMode = 0;
	cellBrush = MBrush( wxColour( wxT( "#EFEFD4" )));
	oddCellBrush = MBrush( wxColour( wxT( "#E9E1BB" )));
	evenCellBrush = MBrush( wxColour( wxT( "#F1EBD3" )));

	gridStyle = 1;
	outerBorderStyle = 3;
	outerBorderPen = *wxBLACK_PEN;

	cellBorderStyle = 1;
	cellBorderPen = *wxBLACK_PEN;

	headerBorderStyle = 2;
	headerBorderPen = *wxBLACK_PEN;

	shadowStyle = 1;
	shadowWidth = 5;
	shadowBrush = MBrush( *wxLIGHT_GREY );
}

IMPLEMENT_SINGLETON( SheetConfigLoader )

/*****************************************************
**
**   SheetConfigLoader   ---   Constructor
**
******************************************************/
SheetConfigLoader::SheetConfigLoader()
: ConfigListLoader<SheetConfig>( CFG_SHEET, CrtPrivateFile )	
{
}

/*****************************************************
**
**   SheetConfigLoader   ---   loadSingleConfig
**
******************************************************/
void SheetConfigLoader::loadSingleConfig( wxJSONValue &v )
{
	SheetConfig *cfg = new SheetConfig;
	cfg->load( v );
	l.push_back( cfg );
}

/*****************************************************
**
**   SheetConfigLoader   ---   saveConfigs
**
******************************************************/
void SheetConfigLoader::saveConfigs()
{
	wxJSONValue root;
	FileConfig *fc = FileConfig::get();

	wxJSONValue &cfgroot = root[ CFG_CONFIGS ];
	for ( uint i = 0; i < l.size(); i++ )
	{
		l[i]->save( cfgroot );
	}

	wxJSONWriter writer;
	fc->backupFile( privateResourcename );
	wxFileOutputStream stream( privateResourcename );
	writer.Write( root, stream );

	wxStreamError err = stream.GetLastError();
	if ( err != wxSTREAM_NO_ERROR )
	{
		wxLogError( wxString::Format( wxT ( "cannot write to output to file %s" ), privateResourcename.c_str() ));
	}

}


