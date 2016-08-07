/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/SheetView.h
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

#ifndef _SHEETVIEW_H_
#define _SHEETVIEW_H_

#include "BasicWidget.h"
#include "BasicView.h"
#include "SheetWidget.h"

class SheetWidget;

/*************************************************//**
*
* \brief 
*
******************************************************/
class SheetView : public BasicView
{
	DECLARE_CLASS( SheetView )

public:
	SheetView( wxWindow*, ChildWindow*, const VIEW_ID &viewId, const bool &showToolbar );
	~SheetView();

	//virtual bool dispatchCommand( const int &command );

protected:
	SheetWidget *twidget;
	bool updateView;

	virtual void write() = 0;

	virtual void OnDataChanged();
	void OnIdle( wxIdleEvent& );
	virtual void exportAs( const WidgetExportType& );
};

#endif

