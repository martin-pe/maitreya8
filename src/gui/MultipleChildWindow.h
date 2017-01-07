/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/MultipleChildWindow.h
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

#ifndef _MULTIPLECHILDWINDOW_H_
#define _MULTIPLECHILDWINDOW_H_

#include <vector>

#include "ChartProperties.h"
#include "ChildWindow.h"

class Document;
class BasicView;
class MultipleViewConfig;
struct NotebookNode;

class wxBookCtrlBase;

using namespace std;

/*************************************************//**
*
* \brief complex child Window containing notebooks, splitters and views
*
******************************************************/
class MultipleChildWindow : public ChildWindow
{
	DECLARE_CLASS( MultipleChildWindow )

public:
	MultipleChildWindow( wxFrame *parent, Document *doc, const BitmapId&, const wxSize& size = wxDefaultSize, const bool ismain = false );
	~MultipleChildWindow();

	void initViews( MultipleViewConfig* );

	virtual bool isMultiple() const { return true; }

	virtual void OnDataChanged();
	virtual void postCreate();

	//MultipleViewConfig* getMultipleViewConfig() { return viewconfig; }

	virtual void setTitle();

protected:

	vector<BasicView*> viewlist;
	MultipleViewConfig *viewconfig;

	// Life cycle methods
	wxWindow *insertNode( wxWindow *parent, ViewNode* );
	void postCreateNode( ViewNode*, wxWindow* = 0 );
	wxBookCtrlBase *createNotebook( wxWindow *parent, NotebookNode *node );
	void saveViewStatus( ViewNode* );

	// view handling
	BasicView *searchNextBasicView( ViewNode* );
	bool isViewVisible( BasicView* );

	void OnSize( wxSizeEvent& );

};

#endif

