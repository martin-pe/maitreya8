/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicView.h
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

#ifndef _BASICVIEW_H_
#define _BASICVIEW_H_

#include <wx/window.h>
#include <vector>
#include "ChartProperties.h"
#include "Commands.h"
#include "maitreya.h"

using namespace std;

class ChildWindow;

class Document;
class DocumentManager;
struct WidgetNode;

class wxBoxSizer;
class wxCommandEvent;
class wxPanel;
class wxSizeEvent;
class wxSpinEvent;
class wxStatusBar;
class wxToolBar;

/*************************************************//**
*
* \brief base class for all views
*
******************************************************/
class BasicView : public wxWindow, public KeyEventObserver
{
public:
	BasicView( wxWindow*, ChildWindow*, const VIEW_ID&, const bool &showToolbar, const long style = 0 );

	Document *getDoc() const { return doc; }
	ChildWindow *getFrame() const { return frame; }
	VIEW_ID getViewId() const { return viewId; }
	ChartProperties *getProps() const { return props; }

	bool isVedic() const { return props->isVedic(); }
	bool isEwFixed() const { return props->isEwFixed(); }

	virtual void OnEWToggle( const bool& ) {}

	virtual void OnDataChanged();
	virtual wxString getWindowLabel( const bool = false ) { return wxT( "---" ); }
	virtual wxString getTitle();
	void doLayout();
	virtual wxSize DoGetBestSize() const;
	virtual void postCreate() {}

	void echo( wxString );
	void setStatusText( wxString message, const int &field );

	wxWindow *getWidget() { return widget; }
	virtual bool dispatchCommand( const int &command );

	virtual bool queryClose() { return true; }

protected:
	mutable ChildWindow *frame;
	const VIEW_ID viewId;
	const bool showtoolbar;
	ChartProperties *props;
	Document *doc;

	wxWindow *widget;
	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent& );
	void OnSetFocus( wxFocusEvent& );

	virtual void handleSetFocusEvent();

	wxToolBar *toolbar;
	wxStatusBar *statusbar;

	wxWindow *detailpanel;
	virtual void initToolItems() {}

	virtual void OnChoiceCommand( wxCommandEvent& );
	virtual void OnSpinCommand( wxSpinEvent& );
	virtual void OnGeneralCommand( wxCommandEvent& );
	virtual void OnCommand( wxCommandEvent& );
	void OnMenuCommand( wxCommandEvent& );
	virtual void OnToolbarCommand() {}
	void OnSize( wxSizeEvent& );
	virtual void OnEnterPressed( wxKeyEvent& );

private:

	wxBoxSizer* sizer;

	DECLARE_CLASS( BasicView )
};

/*************************************************//**
*
* \brief empty view for fallback
*
******************************************************/
class EmptyView : public BasicView
{
public:
	EmptyView( wxWindow *parent, ChildWindow* );

private:
	DECLARE_CLASS( EmptyView )
};

/*************************************************//**
*
* \brief encapsulates the creation of a BasicView. Implementation not here.
*
******************************************************/
class ViewFactory
{
public:

	BasicView *createPartnerView( wxWindow*, ChildWindow*, DocumentManager* );
	BasicView *createView( wxWindow*, ChildWindow*, WidgetNode* );

private:

	BasicView *createDasaTreeView( wxWindow*, ChildWindow* );
	BasicView *createDasaCompositeView( wxWindow*, ChildWindow* );
	BasicView *createGraphicalDasaView( wxWindow*, ChildWindow* ) ;
	BasicView *createSbcView( wxWindow*, ChildWindow* );
	BasicView *createSolarView( wxWindow*, ChildWindow* );
	BasicView *createSelectableTextView( wxWindow*, ChildWindow*, const bool &toolpanel, const int &mode );
	BasicView *createYogaView( wxWindow*, ChildWindow* );
	BasicView *createMainView( wxWindow*, ChildWindow* );
#ifdef USE_URANIAN_CHART
	BasicView *createUranianView( wxWindow*, ChildWindow* );
#endif
	BasicView *createTransitView( wxWindow*, ChildWindow* );
	BasicView *createVargaView( wxWindow*, ChildWindow* );
	BasicView *createGraphicGridView( wxWindow*, ChildWindow*, const int &rows, const int &cols, vector<int> vargas );
	BasicView *createPrintView( wxWindow*, ChildWindow* );

	BasicView *createVimsopakaBalaView( wxWindow*, ChildWindow* );
	BasicView *createAshtakaVargaView( wxWindow*, ChildWindow* );
	BasicView *createGraphicView( wxWindow*, ChildWindow*, const Varga &varga, const bool& uranian );

#ifdef USE_SHADBALA
	BasicView *createShadbalaView( wxWindow*, ChildWindow* );
#endif

	// non doc views
	BasicView *createEclipseView( wxWindow*, ChildWindow* );
	BasicView *createEphemView( wxWindow*, ChildWindow* );
	BasicView *createHoraView( wxWindow*, ChildWindow* );
	BasicView *createYogaEditorView( wxWindow*, ChildWindow* );

	BasicView *createEmptyView( wxWindow*, ChildWindow* );
};

#endif

