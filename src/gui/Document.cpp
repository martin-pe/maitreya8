/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/Document.cpp
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

#include "Document.h"

#include <wx/choice.h>
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/icon.h>

#include "ChildWindow.h"
#include "Conf.h"
#include "maitreya.h"
#include "DataDialog.h"
#include "FileConfig.h"
#include "guibase.h"
#include "ImageProvider.h"
#include "PdfTools.h"
#include "PrintoutConfigBase.h"

extern Config *config;

enum { DOCUMENT_TIMER = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( Document, Horoscope )
IMPLEMENT_SINGLETON( DocumentManager )

/*****************************************************
**
**   Document   ---   Constructor
**
******************************************************/
Document::Document() : Horoscope()
{
	dirty = false;
	mainwindow = 0;
	timer = new wxTimer( this, DOCUMENT_TIMER );
	StartTimer( false );

	Connect( DOCUMENT_TIMER, wxEVT_TIMER, wxTimerEventHandler( Document::OnTimer ));
}

/*****************************************************
**
**   Document   ---   Destructor
**
******************************************************/
Document::~Document()
{
	if ( timer->IsRunning() ) timer->Stop();
	delete timer;
}

/*****************************************************
**
**   Document   ---   addChildWindow
**
******************************************************/
void Document::addChildWindow( ChildWindow *child )
{
	if ( child != mainwindow ) childlist.push_back( child );
}

/*****************************************************
**
**   Document   ---   releaseChildWindow
**
******************************************************/
void Document::releaseChildWindow( ChildWindow *child )
{
	//printf( "Document::releaseChildWindow\n" );
	if ( child == mainwindow ) return;
	for ( vector<ChildWindow*>::iterator iter = childlist.begin(); iter != childlist.end(); iter++ )
	{
		if ( *iter  == child )
		{
			childlist.erase( iter );
			return;
		}
	}
	// TODO: can happen with wxPostEvent stead ProcessEvent on Close (Application close)
	//assert( 0 ); // must not happen (child not found/wrong child)
}

/*****************************************************
**
**   Document   ---   setMainWindow
**
******************************************************/
void Document::setMainWindow( ChildWindow *child )
{
	mainwindow = child;
}

/*****************************************************
**
**   Document   ---   queryClose
**
******************************************************/
bool Document::queryClose()
{
	if ( ! dirty ) return true;
	int i = doMessageBox( mainwindow,
		wxString::Format( wxT( "%s: %s ?" ), _( "Do you want to save the chart" ), getHName().c_str()),
		wxYES_NO | wxCANCEL | wxCENTRE );
	if ( i == wxID_CANCEL ) return false;
	else if ( i == wxID_YES )
	{
		return save();
	}
	else
	{
		dirty = false;
		return true;
	}
}

/*****************************************************
**
**   Document   ---   closeDependentChildWindows
**
******************************************************/
void Document::closeDependentChildWindows()
{
	vector<ChildWindow*>::iterator iter = childlist.begin();
	while ( iter != childlist.end())
	{
		((ChildWindow*)*iter)->Close();
		iter = childlist.begin();
	}
}

/*****************************************************
**
**   Document   ---   updateAllChildWindows
**
******************************************************/
void Document::updateAllChildWindows()
{
	mainwindow->OnDataChanged();
	for ( vector<ChildWindow*>::iterator iter = childlist.begin(); iter != childlist.end(); iter++ )
	{
		((ChildWindow*)*iter)->OnDataChanged();
	}
}

/*****************************************************
**
**   Document   ---   save
**
******************************************************/
bool Document::save()
{
	if ( ! wxStrcmp( dataSet->getFilename(), wxT( "" ))) return saveAs();

	FileConfig::get()->backupFile( dataSet->getFilename() );
	if ( ! Horoscope::save() )
	{
		doMessageBox( mainwindow,
			wxString::Format( wxT( "%s %s" ), _( "Cannot save file" ), dataSet->getFilename().c_str()),
			wxOK | wxICON_ERROR | wxCENTRE );
		return false;
	}
	dirty = false;

	DocumentManager::get()->documentSaved( this );
	return true;
}

/*****************************************************
**
**   Document   ---   saveAs
**
******************************************************/
bool Document::saveAs()
{
	wxString filename = dataSet->getName();
	if ( filename == wxT( "" ))
	{
		filename = wxString::Format( wxT( "%s.mtx" ), getHName().c_str() );
	}
	wxString filetypes = _( "Maitreya text (*.mtx)|*.mtx| All files (*.*)|*.*" );

	wxFileDialog saveFileDialog( mainwindow, _("Save file as" ), config->viewprefs->defSavePath, filename, filetypes, wxFD_SAVE, wxDefaultPosition );

	if ( saveFileDialog.ShowModal() == wxID_OK )
	{
		// Bugfix 7.0.3 GetDirectory returns home directory instead of selected dir
		//filename.Clear();
		//filename << saveFileDialog.GetDirectory() << wxFileName::GetPathSeparator()
		//<< saveFileDialog.GetFilename();
		filename = saveFileDialog.GetPath();
		if ( ! Horoscope::saveAs( filename ))
		{
			doMessageBox( mainwindow,
				wxString::Format( wxT( "%s %s" ), _( "Cannot save file" ), filename.c_str() ),
				wxOK | wxICON_ERROR | wxCENTRE );
			return false;
		}
		dirty = false;

		DocumentManager::get()->documentSaved( this );
		config->viewprefs->defSavePath = saveFileDialog.GetDirectory();
	}
	else
	{
		return false;
	}
	return true;
}

/*****************************************************
**
**   Document   ---   editData
**
******************************************************/
void Document::editData()
{
	DataDialog dialog( mainwindow, this );
	dialog.ShowModal();
	DocumentManager::get()->documentChanged( this );
}

/*****************************************************
**
**   Document   ---   doQuickPrint
**
******************************************************/
void Document::doQuickPrint( PdfDocumentConfig* /*pcfg*/, const wxString )
{
	PdfTool( config->print ).printHoroscope( this );
}

/*****************************************************
**
**   Document   ---   doPrint
**
******************************************************/
void Document::doPrint()
{
	PdfTool( config->print ).printHoroscope( this, true );
}

#define MINIMUM_ANIMATION_FREQUENCY_MILLISEC 300

/*****************************************************
**
**   Document   ---   StartTimer
**
******************************************************/
void Document::StartTimer( bool b )
{
	if ( b ) // Intervall 0 will be 100 msec, eat up myself otherwise. What about slow machines
	{
		if ( ! timer->IsRunning() || 	myAniInterval != config->animation->interval )
		{
			timer->Start( Max( config->animation->interval * 1000, MINIMUM_ANIMATION_FREQUENCY_MILLISEC ) );
			myAniInterval = config->animation->interval;
		}
	}
	else
	{
		if ( timer->IsRunning() ) timer->Stop();
	}
}

/*****************************************************
**
**   Document   ---   OnTimer
**
******************************************************/
void Document::OnTimer( wxTimerEvent& )
{
	double sw;
	if ( config->animation->mode )
	{
		sw = config->animation->stepLength;
		switch ( config->animation->stepDimension )
		{
		case 0:  // seconds
			sw /= 86400;
			break;
		case 1:  // minutes
			sw /= 1400;
			break;
		case 2:  // hours
			sw /= 24;
			break;
		case 3:  // days
			break;
		case 4:  // months
			sw *= 30.6;
			break;
		case 5:  // years
			sw *= 365.25;
			break;
		default:  // seconds
			sw /= 86400;
			break;
		}
		setDate( getJD() + sw );
	}
	else
	{
		setCurrentDate();
	}
	update();
	updateAllChildWindows();

	// see if interval param has changed, so restart timer
	if ( config->animation->interval != myAniInterval )	StartTimer( true );
}


/*****************************************************
**
**   DocumentManager   ---   addDoc
**
******************************************************/
void DocumentManager::addDoc( Document *doc )
{
	docs.push_back( doc );
	for( vector<DocumentListener*>::iterator iter = clients.begin(); iter != clients.end(); iter++ )
	{
		(*iter)->notifyDocumentListChanged();
	}
}

/*****************************************************
**
**   DocumentManager   ---   deleteDoc
**
******************************************************/
void DocumentManager::deleteDoc( Document *doc )
{
	for ( vector<Document*>::iterator iter = docs.begin(); iter != docs.end(); iter++ )
	{
		if ( *iter == doc )
		{
			docs.erase( iter );
			break;
		}
	}
	for( vector<DocumentListener*>::iterator iter = clients.begin(); iter != clients.end(); iter++ )
	{
		(*iter)->notifyDocumentListChanged();
	}
}

/*****************************************************
**
**   DocumentManager   ---   subscribe
**
******************************************************/
void DocumentManager::subscribe( DocumentListener *l )
{
	clients.push_back( l );
}

/*****************************************************
**
**   DocumentManager   ---   unsubscribe
**
******************************************************/
void DocumentManager::unsubscribe( DocumentListener *l )
{
	for( vector<DocumentListener*>::iterator iter = clients.begin(); iter != clients.end(); iter++ )
	{
		if ( (*iter) == l )
		{
			clients.erase( iter );
			return;
		}
	}
	//assert( 0 );
}

/*****************************************************
**
**   DocumentManager   ---   queryClose
**
******************************************************/
bool DocumentManager::queryClose()
{
	for( vector<Document*>::iterator iter = docs.begin(); iter != docs.end(); iter++ )
	{
		if ( ! (*iter)->queryClose() ) return false;
	}
	return true;
}

/*****************************************************
**
**   DocumentManager   ---   updateAllDocs
**
******************************************************/
void DocumentManager::updateAllDocs()
{
	for ( vector<Document*>::iterator iter = docs.begin(); iter != docs.end(); iter++ )
		(*iter)->update();
}

/*****************************************************
**
**   DocumentManager   ---   updateDocsAndChildren
**
******************************************************/
void DocumentManager::updateDocsAndChildren()
{
	for( vector<Document*>::iterator iter = docs.begin(); iter != docs.end(); iter++ )
	{
		(*iter)->update();
		(*iter)->updateAllChildWindows();
	}
}

/*****************************************************
**
**   DocumentManager   ---   documentChanged
**
******************************************************/
void DocumentManager::documentChanged( Document *d )
{
	for( vector<DocumentListener*>::iterator iter = clients.begin(); iter != clients.end(); iter++ )
	{
		(*iter)->notifyDocumentChanged( d );
	}
}

/*****************************************************
**
**   DocumentManager   ---   documentSaved
**
******************************************************/
void DocumentManager::documentSaved( Document *d )
{
	for( vector<DocumentListener*>::iterator iter = clients.begin(); iter != clients.end(); iter++ )
	{
		(*iter)->notifyDocumentSaved( d );
	}
}

/**************************************************************
***
**  DocumentManager   ---   getDocument
***
***************************************************************/
Document *DocumentManager::getDocument( const uint &i )
{
	assert( i < docs.size());
	return docs[i];
}

/**************************************************************
***
**  DocumentManager   ---   getNamesArray
***
***************************************************************/
wxArrayString DocumentManager::getNamesArray()
{
	wxArrayString a;
	for( vector<Document*>::iterator iter = docs.begin(); iter != docs.end(); iter++ )
	{
		a.Add( (*iter)->getHName());
	}
	return a;
}


