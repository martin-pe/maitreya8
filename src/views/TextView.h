/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/TextView.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#ifndef _TEXTVIEW_H_
#define _TEXTVIEW_H_

#include "BasicWidget.h"
#include "BasicView.h"

class TextWidget;

/*************************************************//**
*
* \brief 
*
******************************************************/
class TextView : public BasicView
{
	DECLARE_CLASS( TextView )

public:
	TextView( wxWindow*, ChildWindow*, const VIEW_ID &viewId, const bool &showToolbar );

protected:
	TextWidget *twidget;
	bool updateView;

	virtual void write() = 0;

	virtual void OnDataChanged();
	void OnIdle( wxIdleEvent& );
};

#endif

