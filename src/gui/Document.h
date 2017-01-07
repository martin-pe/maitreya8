/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/Document.h
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

#ifndef _DOCUMENT_H_
#define _DOCUMENT_H_

#include "Horoscope.h"
#include "guibase.h"

#include <wx/object.h>
#include <wx/timer.h>
#include <vector>


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
* \brief listener that can subscribe to events from document lists (used in ApplicationWindow and PartnerView)
*
******************************************************/
class DocumentListener 
{
public:

	virtual void notifyDocumentListChanged() = 0;
	virtual void notifyDocumentChanged( Document* ) = 0;
	virtual void notifyDocumentSaved( Document* ) = 0;
};

/*************************************************//**
*
* \brief holds dynamic lists of documents for PartnerView
*
******************************************************/
class DocumentManager
{
  DECLARE_SINGLETON( DocumentManager )

public:
	void addDoc( Document* );
	void deleteDoc( Document* );

	void subscribe( DocumentListener* );
	void unsubscribe( DocumentListener* );

	bool queryClose();
	void updateAllDocs();
	void updateDocsAndChildren();

	void documentChanged( Document* );
	void documentSaved( Document* );

	int getDocumentCount() { return docs.size(); }
	Document *getDocument( const uint &i );
	wxArrayString getNamesArray();

protected:

	vector<Document*> docs;
	vector<DocumentListener*> clients;

};


#endif

