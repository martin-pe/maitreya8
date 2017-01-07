/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/BaseDao.h
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

