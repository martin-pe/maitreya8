/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/FileConfig.h
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

