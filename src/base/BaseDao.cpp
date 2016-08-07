/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/BaseDao.cpp
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

#include "BaseDao.h"

#include "Conf.h"
#include "FileConfig.h"

#include "wx/filename.h"
#include "wx/wxsqlite3.h"

extern Config *config;

#define MAX_ERROR_STACK_SIZE 10

/*****************************************************
**
**   BaseDao   ---   Constructor
**
******************************************************/
BaseDao::BaseDao( const wxString dbfile )
		: dbfile( dbfile )
{
	dbhandle = 0;
}

/*****************************************************
**
**   BaseDao   ---   Destructor
**
******************************************************/
BaseDao::~BaseDao()
{
	closeDbHandle();
}

/*****************************************************
**
**   BaseDao   ---   executeQueryBundle
**
******************************************************/
int BaseDao::executeQueryBundle( list<wxString> &qb, const bool &dbMustExist )
{
	int count = 0;
	if ( qb.size() == 0 ) return 0;
	wxString queryString;
	try
	{
		wxSQLite3Database *db = getDbHandle( dbMustExist );
		if ( db )
		{
			for ( list<wxString>::iterator iter = qb.begin(); iter != qb.end(); iter++ )
			{
				queryString = *iter;
				db->ExecuteUpdate( (*iter) );
				count++;
			}
		}
	}
	catch (wxSQLite3Exception& e)
	{
		handleException( e );
	}
	return count;
}


/*****************************************************
**
**   BaseDao   ---   getDbFile
**
******************************************************/
wxString BaseDao::getDbFileName()
{
	if ( config->atlas->databaseFile.IsEmpty() )
	{
		config->atlas->databaseFile = FileConfig::get()->getAtlasDbFile();
	}
	return config->atlas->databaseFile;
}

/*****************************************************
**
**   BaseDao   ---   isDbStatusOkay
**
******************************************************/
bool BaseDao::isDbStatusOkay()
{
	errorStack.clear();
	getDbHandle();
	return( errorStack.size() == 0 );
}

/*****************************************************
**
**   BaseDao   ---   getDbHandle
**
******************************************************/
wxSQLite3Database *BaseDao::getDbHandle( const bool dbFileMustExist )
{
	if ( ! dbhandle )
	{
		wxString dbName;

		// dbfile
		if ( ! dbfile.IsEmpty())
		{
			dbName = dbfile;
		}
		else
		{
			dbName = getDbFileName();
		}
		try
		{
			int flags = WXSQLITE_OPEN_READWRITE;
			if ( ! dbFileMustExist ) flags |= WXSQLITE_OPEN_CREATE;
			dbhandle = new wxSQLite3Database();
			dbhandle->Open( dbName, wxEmptyString, flags );
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
	return dbhandle;
}

/*****************************************************
**
**   BaseDao   ---   closeDbHandle
**
******************************************************/
void BaseDao::closeDbHandle()
{
	if ( dbhandle )
	{
		try
		{
			dbhandle->Close();
			delete dbhandle;
		}
		catch (wxSQLite3Exception& e)
		{
			printf( "Close dbhandle failed\n" );
		}
	}
	dbhandle = 0;
}

/*****************************************************
**
**   BaseDao   ---   interruptAllQueries
**
******************************************************/
void BaseDao::interruptAllQueries()
{
	//printf( "BaseDao::interruptAllQueries\n" );
	wxSQLite3Database *db = getDbHandle();
	if ( db )
	{
		try
		{
			db->Interrupt();
		}
		catch (wxSQLite3Exception& e)
		{
			handleException( e );
		}
	}
}

/*****************************************************
**
**   BaseDao   ---   lastDbStatusWasOkay
**
******************************************************/
bool BaseDao::lastDbStatusWasOkay()
{
	return errorStack.size() == 0;
}

/*****************************************************
**
**   BaseDao   ---   getLastErrorMessage
**
******************************************************/
wxString BaseDao::getLastErrorMessage()
{
	if ( errorStack.size() == 0 ) return _( "No error" );
	wxString s;
	s << wxT( "Error code " ) << errorStack[0].errorCode << wxT( ", message: " ) << errorStack[0].message;
	return s;
}

/*****************************************************
**
**   BaseDao   ---   handleException
**
******************************************************/
void BaseDao::handleException( wxSQLite3Exception &e )
{
	errorStack.push_back( DbErrorEntry(e.GetErrorCode(), e.GetMessage()));
	closeDbHandle();

	printf( "Query %s, Exception code %d, message: %s\n", str2char( queryString ), e.GetErrorCode(), str2char( e.GetMessage()));
}


