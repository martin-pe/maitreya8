/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/FileConfig.h
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
#ifndef _FILECONFIG_H_
#define _FILECONFIG_H_

#include <wx/string.h>
#include "maitreya.h"

/*************************************************//**
*
* \brief Provides paths and file names
*
******************************************************/
class FileConfig
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( FileConfig )
public:

	~FileConfig();

	void init( wxString datadir );

	wxString getPicDir() { return picdir; }
	wxString getResourceDir() { return resourcedir; }
	wxString getDasaDir() { return dasadir; }
	wxString getYogaDir() { return yogadir; }
	wxString getFontDir() { return fontdir; }
	const wxString getEphemDir() const;

	wxString getBinDir() { return bindir; }
	wxString getConfigDir() { return configdir; }
	wxString getDataDir() { return datadir; }
	wxString getWorkingDir() { return workingdir; }
	wxString getPathSeparator() { return separator; }

	wxString getLicenseFile() const { return licensefile; }
	wxString getConfigFilename() const { return configfile; }
	wxString getUranianTextFile( const int& ) const;

	bool backupFile( wxString filename, const bool doDelete = false );

	bool copyResourceFile2ConfigDir( wxString basename, const bool force = false );

	wxString getAtlasSqlFile();
	wxString getAtlasDbFile();

	// find file on path
	wxString locateFile( const wxString &file );

	wxString getPdfViewerDefaultFilename();

private:

	FileConfig();

	void prepareConfigDir();

	wxString datadir, configdir, bindir, resourcedir, yogadir, dasadir, picdir, fontdir, locationdir, workingdir;
	wxString licensefile, configfile, separator;
};

#endif

