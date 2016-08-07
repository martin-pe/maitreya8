/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/BaseDao.h
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

#ifndef BASEDAO_H
#define BASEDAO_H

#include <list>
#include <vector>
#include <wx/string.h>

using namespace std;

/*************************************************//**
*
*
*
******************************************************/
struct DbErrorEntry
{
	DbErrorEntry( int errorCode, wxString message )
	{
		this->errorCode = errorCode;
		this->message = message;
	}
	int errorCode;
	wxString message;
};

/*************************************************//**
*
*
*
******************************************************/
class BaseDao
{
public:

	BaseDao( const wxString = wxEmptyString);

	~BaseDao();

	int executeQueryBundle( list<wxString>&, const bool &dbMustExist );

	wxString getDbFileName();

	void closeDbHandle();

	wxString getLastQuery() { return queryString; }

	bool lastDbStatusWasOkay();

	bool isDbStatusOkay();

	void interruptAllQueries();

	wxString getLastErrorMessage();

	vector<DbErrorEntry> &getErrorStack() { return errorStack; }

protected:

	class wxSQLite3Database *getDbHandle( const bool dbFileMustExist = true );

	void handleException( class wxSQLite3Exception& );

	wxString queryString;

	vector<DbErrorEntry> errorStack;

private:

	const wxString dbfile;

	class wxSQLite3Database *dbhandle;

};


#endif

