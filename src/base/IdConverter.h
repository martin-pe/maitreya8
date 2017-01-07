/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/IdConverter.h
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

#ifndef _IDCONVERTER_H_
#define _IDCONVERTER_H_

#include "ImageProvider.h"
#include "maitreya.h"

/*************************************************//**
*
* \brief 
*
******************************************************/
class IdConverter
{
	DECLARE_SINGLETON( IdConverter )

public:
	VIEW_ID commandId2ViewId( const int &command );

	BitmapId viewId2BitmapId( const VIEW_ID&, const bool vedic = true, const Varga varga = V_NONE );

	wxSize getSizeForViewId( const VIEW_ID &viewId, const bool &vedic );

	void setConfigSizeForViewId( const VIEW_ID &viewId, const wxSize &size, const bool &vedic );

	int logMode2WxLogLevel( const int &mode );

};

#endif


