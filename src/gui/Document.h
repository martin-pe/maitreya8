/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/Document.h
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

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include "Horoscope.h"
#include "guibase.h"

#include <wx/object.h>
#include <wx/timer.h>
#include <vector>

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( DOC_SAVED, wxID_HIGHEST + 1200 )
DECLARE_EVENT_TYPE( DOC_UPDATED, wxID_HIGHEST + 1201 )
DECLARE_EVENT_TYPE( CREATE_ENTRY_CHART, wxID_HIGHEST + 1203 )
END_DECLARE_EVENT_TYPES()

class ChildWindow;
class DocumentManager;
class PdfDocumentConfig;

using namespace std;

/*************************************************//**
*
* \brief Horoscope implementation with child window management and other GUI features
*
******************************************************/
class Document : public Horoscope
{
	DECLARE_CLASS( Document )

public:

	Document();
	~Document();

	void addChildWindow( ChildWindow* );
	void releaseChildWindow( ChildWindow *child );
	void setMainWindow( ChildWindow* );
	ChildWindow *getMainWindow() { return mainwindow; }
	bool save();
	bool saveAs();
	bool queryClose();
	bool isDirty() { return dirty; }
	void setDirty( const bool &d ) { dirty = d; }
	void updateAllChildWindows();
	void closeDependentChildWindows();
	void editData();
	void doPrint();
	void doQuickPrint( PdfDocumentConfig* = 0, const wxString = wxEmptyString );
	void StartTimer( bool b );
	bool isAnimated() const { return timer->IsRunning(); }

	void emitDocUpdatedEvent();

protected:
	wxTimer *timer;
	void OnTimer( wxTimerEvent &event );
	int myAniInterval;
	bool dirty;
	vector<ChildWindow*> childlist;
	ChildWindow *mainwindow;
};

/*************************************************//**
*
* \brief listener propagating events for document lists (used in PartnerView)
*
******************************************************/
class DocumentListener : public wxObject
{
	DECLARE_CLASS( DocumentListener )
public:
	DocumentListener( DocumentManager *manager ) : wxObject() {
		this->manager = manager;
	}
	virtual ~DocumentListener() {}
	virtual void documentListChanged() = 0;

protected:
	DocumentManager *manager;

};

/*************************************************//**
*
* \brief holds dynamic lists of documents for PartnerView
*
******************************************************/
class DocumentManager
{
public:
	void addDoc( Document* );
	void deleteDoc( Document* );
	void updateAllDocs();

	void addClient( DocumentListener* );
	void deleteClient( DocumentListener* );
	int getNbDocuments();
	Document *getDocument( const int &i );

protected:
	vector<Document*> docs;
	vector<DocumentListener*> clients;

private:

};


#endif

