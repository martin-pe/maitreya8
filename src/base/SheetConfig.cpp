/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/SheetConfig.cpp
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
SheetConfig::SheetConfig( wxString name )
{
  if ( name.IsEmpty() ) this->name =  _( "Default" );
	else this->name = name;
}

/*****************************************************
**
**   SheetConfig   ---   Copy Constructor
**
******************************************************/
SheetConfig::SheetConfig( const SheetConfig &sc )
{
	//printf( "SheetConfig::SheetConfig COPY\n" );
	name = sc.name;
	tablestyle = sc.tablestyle;
}

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
	headerBgColor = wxColour( wxT( "#92BCD5" ));

	useCellColors = true;
	cellTextColor = config->colors->fgColor;

	cellBgMode = 0;
	allCellBgColor = wxColour( wxT( "#EFEFD4" ));
	oddCellBgColor = wxColour( wxT( "#E9E1BB" ));
	evenCellBgColor = wxColour( wxT( "#F1EBD3" ));

	useGrid = true;
}

IMPLEMENT_SINGLETON( SheetConfigLoader )

/*****************************************************
**
**   SheetConfigLoader   ---   Constructor
**
******************************************************/
SheetConfigLoader::SheetConfigLoader()
: ConfigListLoader<SheetConfig>( CFG_SHEET, CrtLocalFile )	
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
	fc->backupFile( localResourcename );
	wxFileOutputStream stream( localResourcename );
	writer.Write( root, stream );

	wxStreamError err = stream.GetLastError();
	if ( err != wxSTREAM_NO_ERROR )
	{
		wxLogError( wxString::Format( wxT ( "cannot write to output to file %s" ), localResourcename.c_str() ));
	}

}


