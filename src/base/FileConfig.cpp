/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/FileConfig.cpp
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

#include "FileConfig.h"

#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>
#include <wx/utils.h>

#include "Conf.h"
#include "Lang.h"
#include "LanguageConfig.h"
#include "mathbase.h"

extern Config *config;

IMPLEMENT_SINGLETON( FileConfig )

/*****************************************************
**
**   FileConfig   ---   Constructor
**
******************************************************/
FileConfig::FileConfig()
{
	separator = wxFileName::GetPathSeparator();
}

/*****************************************************
**
**   FileConfig   ---   Destructor
**
******************************************************/
FileConfig::~FileConfig()
{
	if ( ego ) delete ego;
	delete config;
}

/*****************************************************
**
**   FileConfig   ---   init
**
******************************************************/
void FileConfig::init( wxString s )
{
	datadir = s;
	//wxLogMessage( wxString::Format( wxT( "Datadir %s" ), datadir.c_str()));
	//printf( "Datadir %s\n" , str2char( datadir ));

	if ( ! datadir.EndsWith( separator )) datadir += separator;

	if ( ! wxDir::Exists( datadir ))
	{
		wxLogMessage( wxT( "WARN: datadir does not exist\n" ));
	}
	workingdir = wxGetCwd();

	picdir = datadir + PIC_DIR_NAME;
	if ( ! wxDir::Exists( picdir )) picdir = datadir + wxT( ".." ) + separator + PIC_DIR_NAME;

#if defined(__WXMAC__)
	resourcedir = datadir;
#else
	resourcedir = datadir + RESOURCE_DIR_NAME;
	if ( ! wxDir::Exists( resourcedir )) resourcedir = datadir + wxT( ".." ) + separator + RESOURCE_DIR_NAME;
	if ( ! wxDir::Exists( resourcedir )) resourcedir = datadir;
#endif

	fontdir = datadir + FONT_DIR_NAME;
	if ( ! wxDir::Exists( fontdir )) fontdir = datadir + wxT( ".." ) + separator + FONT_DIR_NAME;

	locationdir = datadir + ATLAS_DIR_NAME;
	if ( ! wxDir::Exists( locationdir )) locationdir = datadir;

	if ( ! picdir.EndsWith( separator )) picdir += separator;
	if ( ! resourcedir.EndsWith( separator )) resourcedir += separator;
	if ( ! fontdir.EndsWith( separator )) fontdir += separator;
	if ( ! locationdir.EndsWith( separator )) locationdir += separator;

	dasadir = resourcedir + DASA_DIR_NAME;
	if ( ! dasadir.EndsWith( separator )) dasadir += separator;

	prepareConfigDir(); // make dir ~/.maitreya8 and populate it with files (if not present)
	licensefile = datadir + LICENSE_FILE_NAME;

	// Try and error
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../" ) + LICENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../../" ) + LICENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + WIN_LICENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../" ) + WIN_LICENSE_FILE_NAME;
	if ( ! wxFile::Access( licensefile, wxFile::read )) licensefile = datadir + wxT( "../../" ) + WIN_LICENSE_FILE_NAME;

	config = new Config();
	config->readAll();  // depends on config dir/Linux

	LanguageConfig::get(); // depends on path variables
}

/*****************************************************
**
**   FileConfig   ---   getEphemDir
**
******************************************************/
const wxString FileConfig::getEphemDir() const
{
/*
	wxString ephemdir;

#if ! defined __WXMSW__ && ! defined __WXMAC__
	ephemdir = datadir + wxT( ".." ) + separator + EPHEM_DIR_NAME;
	if ( ! wxDir::Exists( ephemdir )) ephemdir = datadir + wxT( ".." ) + separator + wxT( ".." ) + separator + EPHEM_DIR_NAME;
	if ( wxDir::Exists( ephemdir )) return ephemdir;
#endif
*/

// standard path for swe data files on Debian
#if ! defined __WXMSW__ && ! defined __WXMAC__
	if ( wxDir::Exists( DEBIAN_SWE_STANDARD_DATA_PATH )) return DEBIAN_SWE_STANDARD_DATA_PATH;
#endif
	return wxEmptyString;
}

/*****************************************************
**
**   FileConfig   ---   prepareConfigDir
**
******************************************************/
void FileConfig::prepareConfigDir()
{
	wxString yogabasedir, fn, sourcefile, targetfile;

	wxGetHomeDir( &configdir );
	if ( configdir.Last() != separator ) configdir << separator;
	configdir << wxT( "." ) << CONFIG_DIR_NAME << separator;
	configfile = configdir + CONFIG_FILE_NAME;

	if ( wxFileName::DirExists( configdir ))
	{
		if ( ! wxFileName::IsDirWritable( configdir ))
		{
			wxLogError( wxString::Format( wxT( "Fatal error: config directory \"%s\" not writable" ), configdir.c_str()));
			//exit( 1 );
		}
	}
	else
	{
		//wxLogInfo( wxString::Format( wxT( "Creating config directory \"%s\" for the first time ..." ), configdir.c_str()));
		bool error = wxFileName::Mkdir( configdir, 0775 );
		if ( error != true )
		{
			wxLogError( wxString::Format( wxT( "Fatal error: cannot create config directory %s" ), configdir.c_str()));
			//exit( 1 );
		}
	}

	// Copy Sheetconfig file to config dir
	wxString sheetcfg;
	sheetcfg << CFG_SHEET << CFG_JSON_EXTENSION;
	copyResourceFile2ConfigDir( sheetcfg );

	// Create Yoga directory
	yogadir = configdir + separator + YOGA_DIR_NAME + separator;
	yogabasedir = resourcedir + YOGA_DIR_NAME;
	//PrintLn( yogabasedir );
	if ( wxFileName::DirExists( yogadir ))
	{
		if ( ! wxFileName::IsDirWritable( yogadir ))
		{
			wxLogError( wxString::Format( wxT( "Fatal error: yoga config directory \"%s\" not writable" ), yogadir.c_str()));
			//exit( 1 );
		}
	}
	else
	{
		//wxLogMessage( wxString::Format( wxT( "Creating yoga config directory \"%s\" for the first time ..." ), yogadir.c_str()));
		bool error = wxFileName::Mkdir( yogadir, 0775 );
		if ( error != true )
		{
			wxLogError( wxString::Format( wxT( "Fatal error: cannot create Yoga config directory %s" ), yogadir.c_str()));
			//exit( 1 );
		}
	}

	// Copy Yoga definition files
	wxDir dir( yogabasedir );
	if ( !dir.IsOpened() )
	{
		wxLogError( wxString::Format( wxT( "Fatal error: cannot open Yoga config directory %s" ), yogabasedir.c_str()));
		//exit( 1 );
	}
	else
	{
		bool cont = dir.GetFirst( &fn, wxT( "*.json" ), wxDIR_FILES );
		while ( cont )
		{
			// Windows implementation doesn't filter extension
			if ( fn.Right( 5 ) == wxT( ".json" ))
			{
				targetfile = yogadir + fn;
				if ( ! wxFile::Exists( targetfile ))
				{
					sourcefile = yogabasedir + separator + fn;
					//wxString::Format( wxT( "Copy sourcefile %s to %s" ), sourcefile.c_str(), targetfile.c_str());
					wxCopyFile( sourcefile, targetfile, false );
				}
			}
			cont = dir.GetNext(&fn);
		}
	}
}

/*****************************************************
**
**   FileConfig   ---   copyResourceFile2ConfigDir
**
******************************************************/
bool FileConfig::copyResourceFile2ConfigDir( wxString basename, const bool force )
{
	const wxString targetfile = configdir + basename;
	if ( force || ! wxFileName::FileExists( targetfile ))
	{
		const wxString sourcefile = resourcedir + basename;
		if ( ! wxFileName::FileExists( sourcefile ))
		{
			wxLogError( wxT( "File %s does not exist, using defaults" ), sourcefile.c_str());
			return false;
		}
		else
		{
			wxLogMessage( wxT( "copy %s to %s" ), sourcefile.c_str(), targetfile.c_str());
			if ( ! wxCopyFile( sourcefile, targetfile, force ))
			{
				wxLogError( wxT( "could not copy %s to %s" ), sourcefile.c_str(), targetfile.c_str());
				return false;
			}
		}
	}
	return true;
}

/*****************************************************
**
**   FileConfig   ---   getUranianTextFile
**
******************************************************/
wxString FileConfig::getUranianTextFile( const int &mode ) const
{
	wxString s = datadir;
	if ( s.Last() != separator ) s << separator;
	if ( mode ) s << wxT( "witte.txt" );
	else s << wxT( "ebertin.txt" );
	return s;
}

/*****************************************************
**
**   FileConfig   ---   backupFile
**
******************************************************/
bool FileConfig::backupFile( wxString filename, const bool doDelete )
{
	if ( config->view->backupFileMode == 0 ) return true;
	if ( ! wxFileName::FileExists( filename ))
	{
		wxLogMessage( wxString::Format( wxT( "FileConfig::backupFile file not found %s, return" ), filename.c_str()));
		return true;
	}

	wxFileName fn( filename );
	fn.SetExt( wxT( "bak" ) );

	if ( config->view->backupFileMode == 2 && wxFileName::FileExists( fn.GetFullPath() ))
	{
		for ( int i = 1; i < 1000; i++ )
		{
			fn.SetExt( wxString::Format( wxT( "bak%d" ), i ));
			if ( ! wxFileName::FileExists( fn.GetFullPath() )) break;
		}
	}
	if ( doDelete ) return wxRenameFile( filename, fn.GetFullPath(), true );
	else return wxCopyFile( filename, fn.GetFullPath(), true );
}

/*****************************************************
**
**   FileConfig   ---   locateFile
**
******************************************************/
wxString FileConfig::locateFile( const wxString &file )
{
	wxString path, fn;

	wxGetEnv( wxT( "PATH" ), &path );
	wxStringTokenizer tk( path, wxT( ":" ));
	while ( tk.HasMoreTokens() )
	{
		fn = tk.GetNextToken();
		if ( fn.Last() != wxFileName::GetPathSeparator()) fn << wxFileName::GetPathSeparator();
		fn << file;
		if ( wxFileName::FileExists( fn )) return fn;
	}
	return wxEmptyString;
}

/*****************************************************
**
**   FileConfig   ---   getPdfViewerDefaultFilename
**
******************************************************/
wxString FileConfig::getPdfViewerDefaultFilename()
{
#ifdef __WXMAC__
	return MAC_PDF_START_COMMAND;
#endif

#ifndef __WXMSW__
#define NUM_READERS 9
	const wxChar *pdfviewers[NUM_READERS] = { wxT( "evince" ), wxT( "kpdf" ), wxT( "xpdf" ),
		wxT( "okular" ), wxT( "kghostview" ), wxT( "acroread" ),
		wxT( "epdfview" ), wxT( "atril" ), wxT( "xreader" ) };
	wxString s;

	for ( int i = 0; i < NUM_READERS; i++ )
	{
		s = locateFile( pdfviewers[i] );
		if ( ! s.IsEmpty())
		{
			return s;
		}
	}
#endif
	return wxEmptyString;
}

/*****************************************************
**
**   FileConfig   ---   getAtlasSqlFile
**
******************************************************/
wxString FileConfig::getAtlasSqlFile()
{
	wxString filename = locationdir;
	if ( ! filename.EndsWith( separator )) filename << separator;

	filename << ATLAS_DEFAULT_SQLFILENAME;
	return filename;
}

/*****************************************************
**
**   FileConfig   ---   getAtlasDbFile
**
******************************************************/
wxString FileConfig::getAtlasDbFile()
{
	wxString filename = configdir;
	if ( ! filename.EndsWith( separator )) filename << separator;
	filename << ATLAS_DEFAULT_DBFILENAME;
	return filename;
}


