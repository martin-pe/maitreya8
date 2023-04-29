/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/AtlasImporter.cpp
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

#include "AtlasImporter.h"

#include <wx/filename.h>
#include <wx/stopwatch.h>
#include <wx/string.h>
#include <wx/textfile.h>
#include <wx/txtstrm.h>
#include <wx/thread.h>
#include <wx/wfstream.h>
#include <wx/zipstrm.h>

#include "AtlasDao.h"
#include "Conf.h"
#include "FileConfig.h"
#include "mathbase.h"

extern Config *config;

#define IMPORT_THREAD_SLEEP_MILLISEC 100
#define LIFECYCLE_THREAD_SLEEP_MILLISEC 5

#define QUERY_BUNDLESIZE 100

#define THREE_DOTS wxT( " ..." )

static wxMutex theMutex;

/*************************************************//**
*
*
*
******************************************************/
class AtlasImportSharedSection
{
public:
	AtlasImportSharedSection()
	{
		init();
	}

	void init()
	{
		threadOrder = THREADORDER_NONE;
		threadStatus = THREADSTATUS_WAITING;

		progress = totalSize = readSize = currentLine = 0;
		errorCount = 0;
		hasNews = false;

		sqlfile.Clear();
		dbfile.Clear();
		errorMessage.Clear();
	}

	long progress;
	long totalSize;
	long readSize;
	long currentLine;
	int errorCount;
	wxString errorMessage;

	int threadOrder;
	int threadStatus;
	bool hasNews;

	wxString sqlfile;
	wxString dbfile;
};

/*************************************************//**
*
* 
*
******************************************************/
class AtlasImportLifeCycleWorker : public wxThread
{
public:
	AtlasImportLifeCycleWorker( AtlasImportSharedSection *sharedSection )
			: wxThread( wxTHREAD_JOINABLE ),
			sharedSection( sharedSection )
	{
		//SetPriority( WXTHREAD_DEFAULT_PRIORITY );
		sharedSection->threadStatus = THREADSTATUS_WAITING;
	}

	/*****************************************************
	**
	**   AtlasImportLifeCycleWorker   ---   Entry
	**
	******************************************************/
	ExitCode Entry()
	{
		while ( true )
		{
			Sleep( LIFECYCLE_THREAD_SLEEP_MILLISEC );
			if ( sharedSection->threadOrder == THREADORDER_EXIT )
			{
				printf( "AtlasImportLifeCycleWorker received interrupt order\n" );
				dao = new AtlasDao( sharedSection->dbfile );
				dao->interruptAllQueries();
				printf( "AtlasImportLifeCycleWorker: DB operations canceled\n" );
				delete dao;
				break;
			}
		}
		return 0;
	}

private:
	AtlasImportSharedSection *sharedSection;
	AtlasDao *dao;
};

/*************************************************//**
*
*
*
******************************************************/
class AtlasImportWorker : public wxThread
{
public:

	AtlasImportWorker( AtlasImportSharedSection *sharedSection )
			: wxThread( wxTHREAD_JOINABLE ),
			sharedSection( sharedSection )
	{
		//SetPriority( WXTHREAD_DEFAULT_PRIORITY );
		sharedSection->threadStatus = THREADSTATUS_WAITING;
	}

	~AtlasImportWorker()
	{
		sharedSection->threadStatus = THREADSTATUS_FINISHED;
	}

	/*****************************************************
	**
	**   AtlasImportWorker   ---   Entry
	**
	******************************************************/
	ExitCode Entry()
	{
		sharedSection->threadStatus = THREADSTATUS_WAITING;

		while ( sharedSection->threadOrder != THREADORDER_EXIT )
		{
			Sleep( IMPORT_THREAD_SLEEP_MILLISEC );
			if ( sharedSection->threadOrder == THREADORDER_WORK )
			{
				TestDestroy();
				sharedSection->progress = 0;
				sharedSection->threadOrder = THREADORDER_NONE;
				sharedSection->threadStatus = THREADSTATUS_WORKING;
				sharedSection->errorCount = 0;
				totalsize = wxFileName::GetSize( sharedSection->sqlfile ).ToULong();
				doit();

				// set status to finished if not canceled or error
				if (  sharedSection->threadStatus == THREADSTATUS_WORKING )
				{
					sharedSection->threadStatus = THREADSTATUS_FINISHED;
				}
				sharedSection->hasNews = true;
			}
		}
		return 0;
	}

private:

	/*****************************************************
	**
	**   AtlasImportWorker   ---   doit
	**
	******************************************************/
	int doit()
	{
		FileConfig::get()->backupFile( sharedSection->dbfile, true );

		dao = new AtlasDao( sharedSection->dbfile );
		sharedSection->progress = 0;
		sharedSection->hasNews = true;
		currentQuery.Clear();
		qb.clear();

		int ret;

		// TODO 
		if ( sharedSection->sqlfile.EndsWith( wxT( ".zip" ))) ret = readZipFile();
		else ret = readSqlFile();

		delete dao;
		return ret;
	}

	/*****************************************************
	**
	**   AtlasImportWorker   ---   readZipFile
	**
	******************************************************/
	int readZipFile()
	{
		wxString buf;

		wxFFileInputStream instream( sharedSection->sqlfile );
		wxZipInputStream zipstream( instream );
		wxTextInputStream textstream( zipstream );
		wxZipEntry *zipentry = zipstream.GetNextEntry();

		sharedSection->totalSize = (long)zipstream.GetSize();

		if ( zipentry )
		{
			printf( "Entry: %s\n", str2char( zipentry->GetName()));
			bool b = zipstream.OpenEntry( *zipentry );
			printf( "Open flag_ %d\n", b );

			while( zipstream.GetLastError() == wxSTREAM_NO_ERROR )
			{
				sharedSection->currentLine++;
				buf = textstream.ReadLine();
				if ( checkCancelOrExit() ) break;
				addQueryPart( buf );
				if ( checkCancelOrExit() ) break;
			}
			zipstream.CloseEntry();
		}
		else
		{
			printf( "Entry NULL\n" );
		}
		if ( qb.size() > 0 ) execQueryBundle();
		return 0;
	}

	/*****************************************************
	**
	**   AtlasImportWorker   ---   readSqlFile
	**
	******************************************************/
	int readSqlFile()
	{
		wxString buf;
		wxTextFile textfile;

		if ( ! textfile.Open( sharedSection->sqlfile, wxConvUTF8 ))
		{
			printf( "ERROR: cannot open file filename %s\n", str2char( sharedSection->sqlfile ));
			sharedSection->errorCount++;
			return 1;
		}

		sharedSection->totalSize = (long)wxFileName::GetSize( sharedSection->sqlfile ).ToULong();

		for ( buf = textfile.GetFirstLine(); ! textfile.Eof(); buf = textfile.GetNextLine())
		{
			sharedSection->currentLine++;
			if ( checkCancelOrExit() ) break;
			addQueryPart( buf );
			if ( checkCancelOrExit() ) break;
		}
		textfile.Close();
		if ( qb.size() > 0 ) execQueryBundle();
		return 0;
	}


	/*****************************************************
	**
	**   AtlasImportWorker   ---   addQueryPart
	**
	******************************************************/
	void addQueryPart( wxString buf )
	{
		buf = AllTrim( buf );

		// Comment, skip
		if ( buf.StartsWith( wxT( "--" ))) return;

		if ( ! currentQuery.IsEmpty()) currentQuery << wxT( " " );
		currentQuery << buf;

		if ( buf.Find( ';' ) != wxNOT_FOUND )
		{
			sharedSection->readSize += buf.Length();
			sharedSection->readSize += 2;
			sharedSection->progress = (long)(100.0 * (double)sharedSection->readSize / (double)sharedSection->totalSize);
			qb.push_back( currentQuery );
			currentQuery.Clear();
			if ( qb.size() > 0 &&  ( qb.size() % QUERY_BUNDLESIZE ) == 0 )
			{
				execQueryBundle();
			}
		}
	}

	/*****************************************************
	**
	**   AtlasImportWorker   ---   execQueryBundle
	**
	******************************************************/
	int execQueryBundle()
	{
		int ret = dao->executeQueryBundle( qb, false );
		if( ret != (int)qb.size())
		{
			theMutex.Lock();
			sharedSection->errorCount++;
			sharedSection->errorMessage = dao->getLastErrorMessage();
			sharedSection->threadStatus = THREADSTATUS_ERROR;
			theMutex.Unlock();
		}
		qb.clear();
		return ret;
	}

	/*****************************************************
	**
	**   AtlasImportWorker   ---   checkCancelOrExit
	**
	******************************************************/
	bool checkCancelOrExit()
	{
		if ( sharedSection->threadOrder == THREADORDER_EXIT )
		{
			wxRemoveFile( sharedSection->dbfile );
			sharedSection->threadStatus = THREADSTATUS_FINISHED;
			return true;
		}
		else if ( sharedSection->threadOrder == THREADORDER_CANCEL )
		{
			wxRemoveFile( sharedSection->dbfile );
			sharedSection->threadStatus = THREADSTATUS_CANCELED;
			sharedSection->threadOrder = THREADORDER_NONE;
			return true;
		}
		else if ( sharedSection->errorCount > 0 )
		{
			wxRemoveFile( sharedSection->dbfile );
			sharedSection->threadStatus = THREADSTATUS_ERROR;
			sharedSection->threadOrder = THREADORDER_NONE;
			return true;
		}
		return false;
	}

	AtlasImportSharedSection *sharedSection;
	unsigned long totalsize;
	AtlasDao *dao;
	wxString currentQuery;
	std::list<wxString> qb;
};


/*****************************************************
**
**   AtlasImporter   ---   Constructor
**
******************************************************/
AtlasImporter::AtlasImporter()
{
	sharedSection = new AtlasImportSharedSection;
	lifeCycleWorker = new AtlasImportLifeCycleWorker( sharedSection );
	wxThreadError te1 = lifeCycleWorker->Create();
	if ( te1 != wxTHREAD_NO_ERROR )
	{
		printf( "Error create life cycle thread: %d\n", te1 );
	}
	wxThreadError te2 = lifeCycleWorker->Run();
	if ( te2 != wxTHREAD_NO_ERROR )
	{
		printf( "Error run life cycle thread: %d\n", te2 );
	}
	printf( "Atlas life cycle thread started\n" );

	importWorker = new AtlasImportWorker( sharedSection );
	wxThreadError te3 = importWorker->Create();
	if ( te3 != wxTHREAD_NO_ERROR )
	{
		printf( "Error create import thread: %d\n", te3 );
	}
	wxThreadError te4 = importWorker->Run();
	if ( te4 != wxTHREAD_NO_ERROR )
	{
		printf( "Error run import thread: %d\n", te4 );
	}
	printf( "AtlasImportWorker started\n" );
}

/*****************************************************
**
**   AtlasImporter   ---   Destructor
**
******************************************************/
AtlasImporter::~AtlasImporter()
{
	sharedSection->threadOrder = THREADORDER_EXIT;
	if ( importWorker->IsRunning())
	{
		printf( "AtlasImportDialog destructor waiting for import worker to join ...\n" );
		wxLongLong starttime = wxGetLocalTimeMillis();
		importWorker->Wait();
		wxLongLong duration = wxGetLocalTimeMillis() - starttime;
		printf( "Import worker stopped in %ld milllisec\n", duration.ToLong());
	}
	delete importWorker;

	delete sharedSection;
}
	
/*****************************************************
**
**   AtlasImporter   ---   various
**
******************************************************/
int AtlasImporter::getImportStatus()
{
	assert( sharedSection );
	return sharedSection->threadStatus;
}
bool AtlasImporter::hasNews()
{
	assert( sharedSection );
	return sharedSection->hasNews;
}
void AtlasImporter::aknowledgeNews()
{
	assert( sharedSection );
	sharedSection->hasNews = false;
}
void AtlasImporter::abort()
{
	assert( sharedSection );
	sharedSection->threadOrder = THREADORDER_CANCEL;
}
void AtlasImporter::run()
{
	assert( sharedSection );
	sharedSection->threadOrder = THREADORDER_WORK;
}
void AtlasImporter::setImportFile( wxString s )
{
	assert( sharedSection );
	theMutex.Lock();
	sharedSection->sqlfile = s;
	theMutex.Unlock();
}
void AtlasImporter::setDatabaseFile( wxString s )
{
	assert( sharedSection );
	theMutex.Lock();
	sharedSection->dbfile = s;
	theMutex.Unlock();
}
wxString AtlasImporter::getDefaultDbFilename()
{
	return BaseDao().getDbFileName();
}

/*****************************************************
**
**   AtlasImporter   ---   writeErrorStatus
**
******************************************************/
wxString AtlasImporter::writeErrorStatus()
{
	wxString s;
	if ( sharedSection->errorCount > 0 )
	{
			s << wxT( "Database filename: " ) << sharedSection->dbfile << Endl
			<< wxT( "SQL filename: " ) << sharedSection->sqlfile << Endl
			//<< wxT( "Current line number: " ) << sharedSection->currentLine << Endl
			<< wxT( "Error message: " ) << sharedSection->errorMessage << Endl;
	}
	else
	{
		s = _( "No error" );
	}

	printf( "AtlasImporter::writeErrorStatus: %s\n", str2char( s ));
	return s;
}

/*****************************************************
**
**   AtlasImporter   ---   writeRunStatus
**
******************************************************/
wxString AtlasImporter::writeRunStatus()
{
	wxString label;
	if ( sharedSection->threadStatus == THREADSTATUS_WORKING )
	{
		if ( sharedSection->progress == 0 )
		{
			label << _( "Read" ) << THREE_DOTS;
		}
		else if ( sharedSection->progress < 99 )
		{
			label = wxString::Format( wxT( "%s: %ld%%" ), _( "Insert" ), sharedSection->progress );
		}
		else
		{
			label << _( "Commit" ) << THREE_DOTS;
		}
	}
	return label;
}

