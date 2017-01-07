/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AtlasImporter.h
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

#ifndef _ATLASIMPORTER_H
#define _ATLASIMPORTER_H

#include <wx/string.h>

class AtlasImportLifeCycleWorker;
class AtlasImportWorker;
class AtlasImportSharedSection;

enum { THREADSTATUS_WAITING, THREADSTATUS_WORKING, THREADSTATUS_CANCELED,
	THREADSTATUS_FINISHED, THREADSTATUS_ERROR };
enum  { THREADORDER_NONE, THREADORDER_WORK, THREADORDER_CANCEL, THREADORDER_EXIT };

/*************************************************//**
*
*  
*
******************************************************/
class AtlasImporter
{
public:

	AtlasImporter();
	~AtlasImporter();

	bool isRunning() { return getImportStatus() == THREADSTATUS_WORKING; }
	bool isCanceled() { return getImportStatus() == THREADSTATUS_CANCELED; }
	bool isFinished() { return getImportStatus() == THREADSTATUS_FINISHED; }
	bool hasErrors() { return getImportStatus() == THREADSTATUS_ERROR; }

	bool hasNews();
	void aknowledgeNews();
	void abort();
	void run();

	int getImportStatus();

	void setImportFile( wxString );
	void setDatabaseFile( wxString );

	wxString getDefaultDbFilename();
	wxString writeRunStatus();
	wxString writeErrorStatus();

protected:
	AtlasImportSharedSection *sharedSection;
	AtlasImportLifeCycleWorker *lifeCycleWorker;
	AtlasImportWorker *importWorker;
};


#endif

