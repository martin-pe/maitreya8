/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/TransitView.cpp
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

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/log.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/toolbar.h>

#include "Aspect.h"
#include "astrobase.h"
#include "BasicView.h"
#include "ColorConfig.h"
#include "Commands.h"
#include "Conf.h"
#include "DialogElements.h"
#include "Document.h"
#include "guibase.h"
#include "GraphicGrid.h"
#include "Lang.h"
#include "maitreya.h"
#include "mathbase.h"
#include "mspin.h"
#include "mvalidator.h"
#include "Painter.h"
#include "SplitterWidget.h"
#include "Transit.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "ToolPanel.h"
#include "Uranian.h"
#include "UranianHelper.h"
#include "UranianParamPanel.h"
#include "Varga.h"

extern Config *config;

enum { TRANSIT_NOTEBOOK = wxID_HIGHEST + 3000, TRANSIT_PROGRRESS_TIMER, TRANSIT_UPANEL };

#define TIMER_PERIOD  100

#define MAX_TRANSIT_PAGES 6
enum { TNB_ERROR = -1, TNB_CHART, TNB_SBC, TNB_SHADVARGA, TNB_VARGA2, TNB_VARGA3, TNB_URANIAN };
#define URANIAN_VIEW_TEXT_WO WO_SUPPORTS_EW_TOGGLE | WO_MENU_FULL_OBJECT | WO_EXPORT_PDF | WO_EXPORT_PLAINTEXT | WO_EXPORT_HTMLTEXT | WO_EXPORT_CSVTEXT

/*************************************************//**
*
* \brief view container for transits
*
******************************************************/
class TransitView : public BasicView
{
	DECLARE_CLASS( TransitView )
public:

	/*****************************************************
	**
	**   TransitView   ---   Constructor
	**
	******************************************************/
	TransitView( wxWindow *parent, ChildWindow *frame )
		: BasicView( parent, frame, VIEW_TRANSIT, true /*showToolbar*/ )
	{
		transitmode = config->viewprefs->transitmode;
		expert = new TransitExpert( doc, props );

		// required because addWesternChart will cause a crash on AspectExpert update
		expert->update();

		uexpert = new UranianExpert( doc, props );
		UranianConfig &uconfig = props->getUranianConfig();

		tzchoice = config->viewprefs->transitTimezone;

		if ( config->viewprefs->transitJD > 0 )
		{
			transitdate = config->viewprefs->transitJD;
		}
		else
		{
			transitdate = MDate::getCurrentJD();
		}
		//printf( "transitdate %f stored %f\n", transitdate, config->viewprefs->transitJD );
		calcTzOffset();

		thedate = (int)(transitdate + .5 );
		thetime = ( transitdate - (long)transitdate ) + .5;
		//printf( "TIME IS %f\n", thetime );

		notebook = new wxNotebook( this, TRANSIT_NOTEBOOK );

		// setup notebook tabs, start with TNB_CHART
		tsplitter = new SplitterWidget( notebook );
		gwidget =  new ChartGridWidget( tsplitter, CT_TRANSIT, props );
		gwidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );
		gwidget->addVedicChart( V_RASI, doc, expert->getTransitHoroscope());
		gwidget->addWesternChart( doc, expert->getTransitHoroscope());
		//gwidget->getAspectExpert()->setSortOrder( uconfig.sortOrder );

		twidget = new TextWidget( tsplitter, props );
		twidget->setWidgetOptions( URANIAN_VIEW_TEXT_WO );
		tsplitter->SplitVertically( twidget, gwidget );
		notebook->AddPage( tsplitter, _( "Chart" ));

		// TNB_SBC
		swidget = new ChartGridWidget( notebook, CT_TRANSIT, props );
		swidget->addSarvatobhadraChart( doc, expert->getTransitHoroscope() );
		notebook->AddPage( swidget, _( "Sarvatobhadra" ));

		// TNB_SHADVARGA
		shadvargawidget = new ShadVargaChartGridWidget( notebook, CT_TRANSIT, props, doc, expert->getTransitHoroscope());
		notebook->AddPage( shadvargawidget, _( "Shadvarga" ));

		//  TNB_VARGA2
		varga2widget = new VargaTab2ChartGridWidget( notebook, CT_TRANSIT, props, doc, expert->getTransitHoroscope());
		notebook->AddPage( varga2widget, _( "Varga (2)" ));

		//  TNB_VARGA3
		varga3widget = new VargaTab3ChartGridWidget( notebook, CT_TRANSIT, props, doc, expert->getTransitHoroscope());
		notebook->AddPage( varga3widget, _( "Varga (3)" ));

		// TNB_URANIAN
		wxPanel *panel = new wxPanel( notebook );
		uwidget = new TextWidget( panel, props, URANIAN_VIEW_TEXT_WO );
		upanel = new UranianParamPanel( panel, TRANSIT_UPANEL, props, &uconfig.orbisTransit );
		//printf( "ORBIS 1 %f\n", uconfig.orbisTransit );
		wxBoxSizer* usizer = new wxBoxSizer( wxHORIZONTAL );
		usizer->Add( upanel, 0, wxALL, 3);
		usizer->Add( uwidget, 1, wxEXPAND | wxALL, 3);
		panel->SetSizer( usizer );
		usizer->Fit( panel );
		notebook->AddPage( panel, _( "Uranian" ));

		if ( config->viewprefs->activePageTransit >= 0 && config->viewprefs->activePageTransit < (int)notebook->GetPageCount() )
		{
			notebook->SetSelection( config->viewprefs->activePageTransit );
		}
		widget = notebook;
		initToolItems();
		timer = new wxTimer( this, TRANSIT_PROGRRESS_TIMER );

		Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TransitView::OnSetToCurrent ));
		Connect( TBS_DATE, COMMAND_SPIN_CHANGED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
		Connect( TBS_DATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
		Connect( TBS_TIME, COMMAND_SPIN_CHANGED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
		Connect( TBS_TIME, COMMAND_SPIN_WRAP, wxCommandEventHandler( TransitView::OnTimeCtrlWrap ));

		Connect( TBS_TZ, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceTimezone ));
		Connect( TBS_TRANSITMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));

/*
		Connect( TBS_ORBIS, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TransitView::OnSpinCommand ));
		Connect( TBS_ORBIS, wxEVT_COMMAND_TEXT_ENTER, wxTextEventHandler( TransitView::OnGeneralCommand ));
		Connect( TBS_SORT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
		Connect( TBS_GRADKREIS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
		*/
		Connect( wxEVT_IDLE, wxIdleEventHandler( TransitView::OnIdle ));

		Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TransitView::OnDateProgress ));
		Connect( TRANSIT_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( TransitView::OnNotebook ));

		Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TransitView::OnDateProgress ));
		Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TransitView::OnDateProgress ));

		Connect( TRANSIT_UPANEL, COMMAND_UPANEL_CHANGED, wxCommandEventHandler( TransitView::OnGeneralCommand ));

		Connect( TRANSIT_PROGRRESS_TIMER, wxEVT_TIMER, wxTimerEventHandler( TransitView::OnTimer ));
	}

	/*****************************************************
	**
	**   TransitView   ---   Destructor
	**
	******************************************************/
	~TransitView()
	{
		if ( timer->IsRunning() ) timer->Stop();

		config->viewprefs->sashTransit = tsplitter->GetSashPosition();
		config->viewprefs->transitTimezone = tzchoice;
		config->viewprefs->activePageTransit = notebook->GetSelection();
		config->viewprefs->transitmode = transitmode;
		config->viewprefs->transitJD = transitdate;

		//printf( "ORBIS 2 %f\n", props->getUranianConfig().orbisTransit );
		*config->uranian = props->getUranianConfig();

		delete expert;
		delete uexpert;
	}

	/*****************************************************
	**
	**   TransitView   ---   setDirty
	**
	******************************************************/
	void setDirty( const bool b = true )
	{
		//printf( "TransitView   ---   setDirty %d\n", b );
		dirty = b;
		udirty = b;
	}

	/*****************************************************
	**
	**   TransitView   ---   getWindowLabel
	**
	******************************************************/
	virtual wxString getWindowLabel( const bool = false ) { return _( "Transit" ); }

	/*****************************************************
	**
	**   TransitView   ---   postCreate
	**
	******************************************************/
	virtual void postCreate()
	{
		assert( tsplitter );
		tsplitter->SetSashPosition( config->viewprefs->sashTransit );
		setToCurrent();
		setDirty();
	}

	/*****************************************************
	**
	**   TransitView   ---   isVedic
	**
	******************************************************/
	virtual bool isVedic() const
	{
		if ( props->isFixedVedic()) return true;
		if ( props->isFixedWestern()) return false;

		switch ( notebook->GetSelection() )
		{
			case TNB_CHART:
				return props->isVedic();
			break;
			case TNB_SBC:
			case TNB_SHADVARGA:
			case TNB_VARGA2:
			case TNB_VARGA3:
				return true;
			break;
			case TNB_URANIAN:
				return false;
			break;
			default:
			 assert( false );
			break;
		}
	}

protected:

	/*****************************************************
	**
	**   TransitView   ---   initToolItems
	**
	******************************************************/
	virtual void initToolItems()
	{
		if ( toolbar )
		{
			{
				MDateSpin *ds = wxDynamicCast( toolbar->FindControl( TBS_DATE ), MDateSpin );
				if( ds )
				{
					ds->SetValidator( MDateValidator( &thedate ));
				}
				else
				{
					wxLogError( wxT( "Date spin not found in toolbar. View will not work properly." ));
				}
			}

			{
				MDegSpin *ts = wxDynamicCast( toolbar->FindControl( TBS_TIME ), MDegSpin );
				if( ts )
				{
					ts->SetValidator( MDegreeValidator( &thetime, 24 ));
				}
				else
				{
					wxLogError( wxT( "Time spin not found in toolbar. View will not work properly." ));
				}
			}

			TransitModeChoice *choice_mode = wxDynamicCast( toolbar->FindControl( TBS_TRANSITMODE ), TransitModeChoice );
			if ( choice_mode ) choice_mode->setTransitMode( transitmode );

			wxChoice *choice_tz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_tz ) choice_tz->SetSelection( tzchoice );

			// transfer data to date an time spin
			toolbar->InitDialog();
		}
		setDateTimeSpinsFromTransitdate();
	}


	/*****************************************************
	**
	**   TransitView   ---   OnIdle
	**
	******************************************************/
	void OnIdle( wxIdleEvent& ) { if ( dirty ) OnDataChanged(); }

	/*****************************************************
	**
	**   TransitView   ---   OnTimer
	**
	******************************************************/
	void OnTimer( class wxTimerEvent& )
	{
		int delta = getSliderValue();
		if ( delta == 0 )
		{
			timer->Stop();
		}
		else
		{
			transitdate += getDeltaValue( delta );
			setDateTimeSpinsFromTransitdate();
			OnDataChanged();
		}
		setDirty();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnDateCtrlChanged
	**
	******************************************************/
	void OnDateCtrlChanged( wxCommandEvent &event )
	{
		setTransitdateFromDateTimeSpins();
		event.Skip();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnTimeCtrlWrap
	**
	******************************************************/
	void OnTimeCtrlWrap( wxCommandEvent &event )
	{
		setTransitdateFromDateTimeSpins();
		transitdate += event.GetInt();
		setDateTimeSpinsFromTransitdate();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnSetToCurrent
	**
	******************************************************/
	void OnSetToCurrent( wxCommandEvent& )
	{
		resetSlider();
		setToCurrent();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnChoiceTimezone
	**
	******************************************************/
	void OnChoiceTimezone( wxCommandEvent &event )
	{
		if ( event.GetInt())
		{
			// local time switched on: set tzoffset, add tzoffset later
			tzchoice = 1;
			calcTzOffset();
			transitdate += tzoffset / 24;
		}
		else
		{
			// local time switched off: adjust transitdate, set tzoffset later
			transitdate -= tzoffset / 24;
			tzchoice = 0;
			calcTzOffset();
		}
		setDateTimeSpinsFromTransitdate();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnNotebook
	**
	******************************************************/
	virtual void OnNotebook( wxNotebookEvent& )
	{
		updateCurrentPage();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnDataChanged
	**
	******************************************************/
	virtual void OnDataChanged()
	{
		//printf( "TransitView::OnDataChanged\n" );
		BasicView::OnDataChanged();

		// must be called each time because dst may have changed
		calcTzOffset();

		// year length may have changed
		expert->init();

		if ( toolbar )
		{
			// Date and Time
			setTransitdateFromDateTimeSpins();
			expert->setTransitDate( transitdate, tzoffset );

			// Mode
			TransitModeChoice *choice_mode = wxDynamicCast( toolbar->FindControl( TBS_TRANSITMODE ), TransitModeChoice );
			if ( choice_mode )
			{
				transitmode = choice_mode->getTransitMode();
				expert->setTransitMode( transitmode );
			}
		}

		gwidget->getAspectExpert()->setSortOrder( props->getUranianConfig().sortOrder );

		expert->update();
		udirty = true;
		updateCurrentPage();
	}

	/*****************************************************
	**
	**   TransitView   ---   updateCurrentPage
	**
	******************************************************/
	void updateCurrentPage()
	{
		const int sel = notebook->GetSelection();
		//printf( "TransitView   ---   updateCurrentPage sel %d\n", sel );

		switch ( sel )
		{
			case TNB_CHART:
				gwidget->OnDataChanged();
				gwidget->Refresh();
				writeTransitPlanetList();
			break;
			case TNB_SBC:
				swidget->OnDataChanged();
			break;
			case TNB_SHADVARGA:
				shadvargawidget->OnDataChanged();
			break;
			case TNB_VARGA2:
				varga2widget->OnDataChanged();
			break;
			case TNB_VARGA3:
				varga3widget->OnDataChanged();
			break;
			case TNB_URANIAN:
				//printf( "updateCurrentPage uranian\n" );
				if ( udirty ) updateUranian();
			break;
		default:
				assert( false );
			break;
		}
		dirty = false;
	}

	/*****************************************************
	**
	**   TransitView   ---   OnToolbarCommand
	**
	******************************************************/
	virtual void OnToolbarCommand() { setDirty(); }

	/*****************************************************
	**
	**   TransitView   ---   setToCurrent
	**
	******************************************************/
	void setToCurrent()
	{
		transitdate = MDate::getCurrentJD() + tzoffset / 24.0;
		setDateTimeSpinsFromTransitdate();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnDateProgress
	**
	******************************************************/
	// Slider stuff
	void OnDateProgress( wxScrollEvent &event )
	{
		int type = event.GetEventType();
		if ( type == wxEVT_SCROLL_THUMBRELEASE )
		{
			resetSlider();
		}
		else if ( type == wxEVT_SCROLL_THUMBTRACK )
		{
			if ( ! timer->IsRunning() )
			{
				timer->Start( TIMER_PERIOD );
			}
		}
		setDirty();
	}

	/*****************************************************
	**
	**   TransitView   ---   getDeltaValue
	**
	******************************************************/
	double getDeltaValue( const int &v )
	{
		double ret = 0;
		int val = v > 0 ? v : -v;
		switch ( val )
		{
		case 1: // second
			ret = 1/ 86400.0;
			break;
		case 2: // Minute
			ret = 1/ 1440.0;
			break;
		case 3: // hour
			ret = 1/ 24.0;
			break;
		case 4: // day
			ret = 1.0;
			break;
		case 5: // week
			ret = 7.0;
			break;
		case 6: // month
			ret = 30.0;
			break;
		case 7: //year
			ret = 365.25;
			break;
		}
		if ( v < 0 ) ret *= -1;
		return ret;
	}

	/*****************************************************
	**
	**   TransitView   ---   resetSlider
	**
	******************************************************/
	void resetSlider()
	{
		if ( toolbar )
		{
			wxSlider *slider = (wxSlider*)toolbar->FindControl( TBS_DATE_PROGRESS );
			if ( slider ) slider->SetValue( 0 );
		}
		if ( timer->IsRunning() )
		{
			timer->Stop();
		}
		setDirty();
	}

	/*****************************************************
	**
	**   TransitView   ---   getSliderValue
	**
	******************************************************/
	int getSliderValue()
	{
		int delta = 0;
		wxSlider *slider = (wxSlider*)toolbar->FindControl( TBS_DATE_PROGRESS );
		if ( slider ) delta = slider->GetValue();
		return delta;
	}


	/*****************************************************
	**
	**   TransitView   ---   writeTransitPlanetList
	**
	******************************************************/
	void writeTransitPlanetList()
	{
		//Formatter *formatter = Formatter::get();
		//DateTimeFormatter *df = DateTimeFormatter::get();
		wxString s;

		Sheet *sheet = twidget->getSheet();
		assert( sheet );
		sheet->clear();
		expert->writeTransitHeader( sheet );

		TextHelper th( expert->getTransitHoroscope(), props, sheet );
		//th.writeTransitReport( expert->getTransitHoroscope(), gwidget->getAspectExpert() );
		th.writeTransitReport( transitmode );
		twidget->OnDataChanged();
	}

	/*****************************************************
	**
	**   TransitView   ---   updateUranian
	**
	******************************************************/
	void updateUranian()
	{
		uexpert->OnDataChanged();
		uexpert->calculateTransit( expert->getTransitHoroscope(), transitmode );

		UranianHelper helper( uexpert );
		uwidget->clearSheet();
		helper.writeTransits( uwidget->getSheet(), expert );
		uwidget->OnDataChanged();
		udirty = false;
	}

	/*****************************************************
	**
	**   TransitView   ---   setTransitdateFromDateTimeSpins
	**
	******************************************************/
	// update tzoffset and transitdate
	void setTransitdateFromDateTimeSpins()
	{
		transitdate = thedate - .5 + thetime / 24.0;
		if ( toolbar )
		{
			/*
			// Date and Time
			DateSpin *spin_date = (DateSpin*)toolbar->FindControl( TBS_DATE );
			TimeSpin *spin_time = (TimeSpin*)toolbar->FindControl( TBS_TIME );
			if ( spin_date && spin_time )
			{
				double dd = spin_date->getDoubleValue();
				double tt = spin_time->getDoubleValue();
				transitdate = dd + tt / 24.0;
			}
			*/
		}
		setDirty();
	}

	/*****************************************************
	**
	**   TransitView   ---   setDateTimeSpinsFromTransitdate
	**
	******************************************************/
	void setDateTimeSpinsFromTransitdate()
	{
		thedate = (int)(transitdate + .5 );
		thetime = ( transitdate - (long)transitdate );
		thetime += .5;
		if ( thetime > 1 ) thetime--;
		thetime *= 24;
		toolbar->TransferDataToWindow();

		if ( toolbar ) toolbar->TransferDataToWindow();
		setDirty();
	}

	/*****************************************************
	**
	**   TransitView   ---   calcTzOffset
	**
	******************************************************/
	void calcTzOffset()
	{
		if ( tzchoice == 1 )
		{
			TzUtil tzu;
			TzInfo tzinfo = tzu.calculateTzInfoForJD( transitdate );
			tzoffset = tzinfo.tzhours + tzinfo.dsthours;
		}
		else tzoffset = 0;
	}

	/*****************************************************
	**
	**   TransitView   ---   OnEWToggle
	**
	******************************************************/
  virtual void OnEWToggle( const bool &vedic )
	{
		//printf( "TransitView::OnEWToggle %d\n", vedic );
		props->setVedic( vedic );
		gwidget->OnEWToggle( vedic );
	}

	TransitExpert *expert;

	wxTimer *timer;

	// model for date and time spin
	double thedate, thetime;

	// jd including tz offset
	double transitdate;

	// hours
	double tzoffset;

	// value of choice box
	int tzchoice;

	PlanetContext transitmode;

	// update on next idle event
	bool dirty;
	bool udirty;

	// charts and text views in notebook tabs
	ShadVargaChartGridWidget *shadvargawidget;
	VargaTab2ChartGridWidget *varga2widget;
	VargaTab3ChartGridWidget *varga3widget;
	TextWidget *twidget;
	ChartGridWidget *gwidget, *swidget;

	UranianExpert *uexpert;
	TextWidget *uwidget;
	UranianParamPanel *upanel;

	SplitterWidget *tsplitter;
	wxNotebook *notebook;
};


IMPLEMENT_CLASS( TransitView, BasicView )

/**************************************************************
***
**   ViewFactory   ---   createTransitView
***
***************************************************************/
BasicView *ViewFactory::createTransitView( wxWindow* parent, ChildWindow *frame )
{
	return new TransitView( parent, frame );
}

