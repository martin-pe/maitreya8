/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/DataSet.h
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
#ifndef _DATA_SET_
#define _DATA_SET_

#include <wx/string.h>
#include <wx/textfile.h>

#include "ConfigBase.h"
#include "maitreya.h"

/*************************************************//**
*
*
*
******************************************************/
struct TzFormattedDate
{
public:
	wxString dateFormatted;
	wxString timeFormatted;
	wxString timezoneFormatted;;
	wxString fullDateTimeTzFormatted;
	wxString weekDay;
	int dayOfMonth;

};

/*************************************************//**
*
*
*
******************************************************/
struct TzInfo
{
	wxString localTimeFormatted;
	wxString utcTimeFormatted;

	wxString tzOffsetFormatted;
	wxString dstOffsetFormatted;

	int isdst;
	double tzhours;
	double dsthours;

};

/*************************************************//**
*
*
*
******************************************************/
class TzUtil
{
public:
	TzInfo getCurrentTzInfo();

	TzInfo calculateTzInfo( const time_t &tt );

	TzInfo calculateTzInfoForJD( const double &jd );

	TzFormattedDate getDateFormatted( const double &jd, const bool &isLocal );

};

/*************************************************//**
*
* \brief simple data class
*
******************************************************/
class MDate
{
public:

	MDate();
	MDate( const double& );
	MDate( const MDate & );

	static double getCurrentJD();

	void setCurrentDate();
	double getJD() const { return jd; }
	void setDate( const double& );
	void setDate( const int &day, const int &month, const int &year, const double &time );
	void operator=( const MDate& date );
	void dump( wxString& );

private:
	double jd;
	void updateJD( const int &day, const int &month, const int &year, const double &time );
};

/*************************************************//**
*
* \brief contains data of the location of a chart
*
******************************************************/
class Location : public ConfigBase
{
public:

	Location();
	Location( const Location & );

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	void operator=( const Location& );
	void setLocation( const wxString &locName, const double &longitude, const double &latitude,
	                  const double &timeZone, const double &dst );

	double getLongitude() const { return longitude; }
	double getLatitude() const { return latitude; }
	const wxString getLocName() const { return locName; }

	double getTimeZone() const { return tz; }
	void setTimeZone( const double &ttz ) { tz = ttz; }

	double getDST() const { return dst; }
	void setDST( const double &ddst ) { dst = ddst; }

	void dump(wxString&);

private:
	double latitude;
	double longitude;
	double tz;
	double dst;
	wxString locName;
};

/*************************************************//**
*
* 
*
******************************************************/
struct LocationModel
{
	LocationModel()
	{
		longitude = latitude = tz = dst = 0.0;
		c_longitude = c_latitude = 0;
	}
	LocationModel( const Location& );

	double getLongitude() { return( c_longitude == 1 ? - longitude : longitude ); }
	double getLatitude() { return( c_latitude == 1 ? - latitude : latitude ); }
	double getTimezone() { return( c_tz == 1 ? - tz : tz ); }

	wxString locname;
	double longitude, latitude, tz, dst;
	int c_longitude, c_latitude, c_tz;
};


/*************************************************//**
*
* \brief data for a chart (date, location, name etc.)
*
******************************************************/
class DataSet
{
public:

	DataSet();
	DataSet( const Location& );
	DataSet( const DataSet & );
	~DataSet();

	void operator=( const DataSet& );

	void setDate( const double& );
	MDate *getDate() const { return date; }
	void setDate( const int &day, const int &month, const int &year, const double &time );
	void setCurrentDate();

	double getJD() const { return date->getJD(); }
	int getWeekDay();
	double getLocalMeanTime();

	void setLocation( const wxString &locName, const double &longitude, const double &latitute,
	                  const double &timeZone, const double &dst );
	void setLocation( const Location& );
	Location *getLocation() const { return location; }

	const wxString getName() const { return name; }
	void setName( const wxString& );

	wxString getRemark() const { return remark; }
	void setRemark( const wxString& );

	void dump(wxString&);

protected:
	wxString remark;

private:
	wxString name;
	MDate *date;
	Location *location;
};

/*************************************************//**
*
* \brief DataSet with file IO methods
*
******************************************************/
class FileDataSet : public DataSet
{
public:

	FileDataSet();
	FileDataSet( const Location& );
	~FileDataSet();

	bool openFile( const wxString&, bool isAtxFile = true );
	bool saveAs( const wxString& );
	bool save();
	const wxString getFilename() const {
		return filename;
	}

private:
	bool readJhdFile();
	bool readAtxFile();
	bool readMtxFile();
	wxString filename;
	wxTextFile theFile;
};

/*************************************************//**
*
* \brief location in LocationDialog
*
******************************************************/
class LocationEntry
{
public:
	LocationEntry();
	~LocationEntry();
	LocationEntry( wxString name, wxString country, wxString state,
	               const double &lon, const double &lat, const double &tz );
	LocationEntry( const wxString& );
	void operator=( const LocationEntry& );
	bool operator==( const LocationEntry& );

	wxString name, state, country;
	double longitude, latitude, timeZone;
	int row;
};

#endif


