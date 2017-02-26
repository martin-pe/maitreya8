/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/ImageProvider.h
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

#ifndef _ICONPROVIDER_H_
#define _ICONPROVIDER_H_

#include <wx/bitmap.h>
#include <wx/icon.h>
#include "maitreya.h"

class wxImageList;

enum BitmapId {
	BM_NULL = -1, // do not show any image
	BM_ERROR = 0,
	BM_MAITREYA,

	// global commands
	BM_CLOSE,
	BM_NEW,
	BM_OPEN,
	BM_QUIT,
	BM_SAVE, BM_SAVEAS,

	// view without document
	BM_ECLIPSE,
	BM_EPHEM,
	BM_HORA,
	BM_YOGAEDITOR,

	// view with document
	BM_ASHTAKAVARGA,
	BM_DASA,
	BM_GRAPHICALDASA,
	BM_MVIEW,
	BM_PARTNER,
	BM_SBC,
	BM_SHADBALA,
	BM_SOLAR,
	BM_SVIEW,
	BM_TEXT,
	BM_TRANSIT,
	BM_URANIAN,
	BM_VARGA,
	BM_WCHART,
	BM_YOGA,

	// vedic charts
	BM_CHART, BM_CHART9, BM_CHART2, BM_CHART3, BM_CHART4, BM_CHART6, BM_CHART7, BM_CHART8,
	BM_CHART10, BM_CHART12, BM_CHART16, BM_CHART20, BM_CHART24, BM_CHART27, BM_CHART30, BM_CHART40,
	BM_CHART45, BM_CHART60, BM_CHART108, BM_CHART144, BM_CHARTB,

	// misc modal dialogs
	BM_ABOUT,
	BM_ATLAS,
	BM_CONFIG,

	// commands on document dependend views
	BM_EDIT,
	BM_PRINT,
	BM_QUICKPRINT,

	// dasa tree view
	BM_FOLDER, BM_FOLDERE,
	BM_DASATREE1, BM_DASATREE1E,
	BM_DASATREE2, BM_DASATREE2E,
	BM_DASATREE3, BM_DASATREE3E,
	BM_DASATREE4, BM_DASATREE4E,
	BM_DASATREE5, BM_DASATREE5E,
	BM_MAHADASA, BM_MAHADASAE,

	// general toolbar command
	BM_ARROW_DOWN, BM_ARROW_LEFT, BM_ARROW_RIGHT, BM_ARROW_UP,
	BM_COPY,
	BM_EXPORT_CSV, BM_EXPORT_HTML, BM_EXPORT_IMAGE, BM_EXPORT_PDF, BM_EXPORT_TEXT,
	BM_FILTER,
	BM_GOTO,
	BM_HELP,
	BM_NOW,
	BM_SPLIT,
	BM_TIP,
	BM_ZOOM_IN, BM_ZOOM_ORIGINAL, BM_ZOOM_OUT, 

	BM_SEPARATOR
};

#define BM_FIRST BM_ERROR
#define BM_LAST BM_SEPARATOR
#define IS_VALID_BITMAP_ID( b ) b >= BM_FIRST && b <= BM_LAST

/*************************************************//**
*
* \brief encapsulates access to pictures and icons
*
******************************************************/
class ImageProvider
{
	DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( ImageProvider )
public:

	~ImageProvider();

	int getDefaultBitmapSize() const;

	wxBitmap getFileBasedBitmap( const wxString &s, const int rotateHue = 0, const bool storeInCache = true );
	wxBitmap *getFileBasedBitmapRef( const wxString &s, const int rotateHue = 0, const bool storeInCache = true );
	wxBitmap getFileBasedBitmapConservative( const wxString &s, const int rotateHue );

	wxBitmap getBitmap( const BitmapId&, const int rotateHue = 0 );

	wxImage getImage( const wxString &s );

	wxBitmap getScaledBitmap( const BitmapId&, const int &size );
	wxBitmap getScaledBitmap( const wxString s, const int &size );
	wxBitmap getMenuBitmap( const BitmapId& );

	wxIcon getIcon( const BitmapId& );

	wxImageList *getDasaImageList();

	wxImageList *createViewImageList( const bool /*small*/ = false );
	int getViewImageListIndex4BitmapId( const BitmapId& ) const;

private:

	wxBitmap errorBmp;
	wxImageList *dasaImageList;

	char **bitmapId2CharArray( const BitmapId& );
	ImageProvider();

};

#endif


