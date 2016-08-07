/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/MultipleChildWindow.h
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

