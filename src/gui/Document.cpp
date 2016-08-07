/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/Document.cpp
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

#include "Document.h"

#include <wx/app.h>
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
IMPLEMENT_CLASS( DocumentListener, wxObject )

DEFINE_EVENT_TYPE( DOC_SAVED )
DEFINE_EVENT_TYPE( DOC_UPDATED )
DEFINE_EVENT_TYPE( CREATE_ENTRY_CHART )

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
	wxCommandEvent event( DOC_SAVED, mainwindow->GetId() );
	event.SetEventObject( this );
	wxPostEvent( mainwindow->GetParent(), event );
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
		wxCommandEvent event( DOC_SAVED, mainwindow->GetId() );
		event.SetEventObject( this );
		wxPostEvent( mainwindow->GetParent(), event );
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
	emitDocUpdatedEvent();
}

/*****************************************************
**
**   Document   ---   editData
**
******************************************************/
void Document::emitDocUpdatedEvent()
{
	printf( "SEND EVENT DOC UPDATED\n" );
	wxCommandEvent event( DOC_UPDATED, mainwindow->GetId() );
	event.SetEventObject( this );
	wxPostEvent( mainwindow->GetParent(), event );
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
	for ( uint i = 0; i < clients.size(); i++ ) clients[i]->documentListChanged();
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
	for ( uint i = 0; i < clients.size(); i++ ) clients[i]->documentListChanged();
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
**   DocumentManager   ---   addClient
**
******************************************************/
void DocumentManager::addClient( DocumentListener *l )
{
	clients.push_back( l );
}

/*****************************************************
**
**   DocumentManager   ---   deleteClient
**
******************************************************/
void DocumentManager::deleteClient( DocumentListener *l )
{
	vector<DocumentListener*>::iterator iter;
	for ( iter = clients.begin(); iter != clients.end(); iter++ )
	{
		if ( (*iter) == l )
		{
			clients.erase( iter );
			return;
		}
	}
	// FIXME: What's that
	//delete l;
	//assert( 0 );
}

/*****************************************************
**
**   DocumentManager   ---   getNbDocuments
**
******************************************************/
int DocumentManager::getNbDocuments()
{
	return docs.size();
}

/**************************************************************
***
**  DocumentManager   ---   getDocument
***
***************************************************************/
Document *DocumentManager::getDocument( const int &i )
{
	return docs[i];
}


