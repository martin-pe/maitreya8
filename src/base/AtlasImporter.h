/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AtlasImporter.h
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

