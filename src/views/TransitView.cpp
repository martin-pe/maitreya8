/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/TransitView.cpp
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

#include <wx/choice.h>
#include <wx/notebook.h>
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
#include "maitreya.h"
#include "DialogElements.h"
#include "Document.h"
#include "SheetWidget.h"
#include "guibase.h"
#include "GraphicGrid.h"
#include "mathbase.h"
#include "mvalidator.h"
#include "ObjectFilterDialog.h"
#include "Painter.h"
#include "SplitterWidget.h"
#include "Transit.h"
#include "TextHelper.h"
#include "ToolPanel.h"
#include "Varga.h"

#ifdef USE_URANIAN_CHART
#include "Uranian.h"
#include "UranianHelper.h"
#endif

extern Config *config;

enum { TRANSIT_NOTEBOOK = wxID_HIGHEST + 3000, TRANSIT_PROGRRESS_TIMER };

#define TIMER_PERIOD  100

#define MAX_TRANSIT_PAGES 7
enum { TNB_ERROR = -1, TNB_DOUBLE, TNB_SBC, TNB_SHADVARGA, TNB_VARGA2, TNB_VARGA3, TNB_URANIAN, TNB_URANIAN_CHART };

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
		props->setTransitmode( true );
		activewidget = (BasicWidget*)NULL;
		activetext = (SheetWidget*)NULL;

		transitmode = config->viewprefs->transitmode;
		expert = new TransitExpert( doc, props );

		// required because addWesternChart will cause a crash on AspectExpert update
		expert->update();

#ifdef USE_URANIAN_CHART
		uconfig = new UranianConfig( *config->uranian );
		uexpert = new UranianExpert( doc, props, uconfig );
#endif

		tzchoice = config->viewprefs->transitTimezone;
		transitdate = MDate::getCurrentJD();
		calcTzOffset();

		thedate = (int)(transitdate + .5 );
		thetime = ( transitdate - (long)transitdate ) + .5;

		notebook = new wxNotebook( this, TRANSIT_NOTEBOOK );
		initToolItems();

		// setup notebook tabs, start with TNB_DOUBLE
		tsplitter = new SplitterWidget( notebook );
		gwidget =  new ChartGridWidget( tsplitter, CT_TRANSIT, props );
		gwidget->addVedicChart( V_RASI, doc, expert->getTransitHoroscope());
		gwidget->addWesternChart( doc, expert->getTransitHoroscope());
#ifdef USE_URANIAN_CHART
		gwidget->getAspectExpert()->setSortOrder( uconfig->sortOrder );
#endif
		twidget = new SheetWidget( tsplitter, props );
		twidget->enableFloatingLayout( false );
		tsplitter->SplitVertically( twidget, gwidget );
		notebook->AddPage( tsplitter, _( "Double Chart" ));

		if ( ! props->isFixedWestern())
		{
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
		}

#ifdef USE_URANIAN_CHART
		if ( ! props->isFixedVedic())
		{
			// TNB_URANIAN
			uwidget = new SheetWidget( notebook, props );
			notebook->AddPage( uwidget, _( "Uranian" ));

			// TNB_URANIAN_CHART
			uranianChart =  new ChartGridWidget( notebook, CT_TRANSIT, props );
			uranianChart->addUranianChart( uexpert, doc, expert->getTransitHoroscope());
			notebook->AddPage( uranianChart, _( "Uranian Chart" ));
		}
#endif

		if ( config->viewprefs->activePageTransit >= 0 && config->viewprefs->activePageTransit < (int)notebook->GetPageCount() )
		{
			notebook->SetSelection( config->viewprefs->activePageTransit );
		}
		widget = notebook;
		timer = new wxTimer( this, TRANSIT_PROGRRESS_TIMER );


		Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TransitView::OnSetToCurrent ));
		Connect( TBS_DATE, COMMAND_SPIN_CHANGED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
		Connect( TBS_DATE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
		Connect( TBS_TIME, COMMAND_SPIN_CHANGED, wxCommandEventHandler( TransitView::OnDateCtrlChanged ));
		Connect( TBS_TIME, COMMAND_SPIN_WRAP, wxCommandEventHandler( TransitView::OnTimeCtrlWrap ));

		Connect( TBS_TZ, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceTimezone ));
		Connect( TBS_TRANSITMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));

#ifdef USE_URANIAN_CHART
		Connect( CMD_FILTER, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( TransitView::OnFilter ));
		Connect( TBS_ORBIS, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( TransitView::OnSpinCommand ));
		Connect( TBS_ORBIS, wxEVT_COMMAND_TEXT_ENTER, wxTextEventHandler( TransitView::OnGeneralCommand ));
		Connect( TBS_SORT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
		Connect( TBS_GRADKREIS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( TransitView::OnChoiceCommand ));
#endif
		Connect( wxEVT_IDLE, wxIdleEventHandler( TransitView::OnIdle ));

		Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( TransitView::OnDateProgress ));
		Connect( TRANSIT_NOTEBOOK, wxEVT_COMMAND_NOTEBOOK_PAGE_CHANGED, wxNotebookEventHandler( TransitView::OnNotebook ));

		Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( TransitView::OnDateProgress ));
		Connect( TBS_DATE_PROGRESS, wxEVT_SCROLL_CHANGED, wxScrollEventHandler( TransitView::OnDateProgress ));

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

#ifdef USE_URANIAN_CHART
		config->uranian->orbisTransit = uconfig->orbisTransit;
		config->uranian->gradkreis = uconfig->gradkreis;
		config->uranian->sortOrder = uconfig->sortOrder;
#endif

		delete expert;

#ifdef USE_URANIAN_CHART
		delete uexpert;
		delete uconfig;
#endif
	}

	/*****************************************************
	**
	**   TransitView   ---   setDirty
	**
	******************************************************/
	void setDirty( const bool b = true )
	{
		printf( "TransitView   ---   setDirty %d\n", b );
		dirty = b;
		udirty = b;
		uwritten = false;
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
			case TNB_DOUBLE:
				return props->isVedic();
			break;
			case TNB_SBC:
			case TNB_SHADVARGA:
			case TNB_VARGA2:
			case TNB_VARGA3:
				return true;
			break;
#ifdef USE_URANIAN_CHART
			case TNB_URANIAN:
			case TNB_URANIAN_CHART:
				return false;
			break;
#endif
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
				wxControl *ds = toolbar->FindControl( TBS_DATE );
				if ( ds )
				{
					ds->SetValidator( MDateValidator( &thedate ));
				}
				else assert( false );
			}

			{
				wxControl *ts = toolbar->FindControl( TBS_TIME );
				if ( ts )
				{
					ts->SetValidator( MDegreeValidator( &thetime, 24 ));
				}
				else assert( false );
			}

			wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_TRANSITMODE );
			if ( choice_mode ) choice_mode->SetSelection( transitmode );

			wxChoice *choice_tz = (wxChoice*)toolbar->FindControl( TBS_TZ );
			if ( choice_tz ) choice_tz->SetSelection( tzchoice );

#ifdef USE_URANIAN_CHART
			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis ) spin_orbis->SetValue( 60 * uconfig->orbisTransit );

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk ) choice_gk->SetSelection( (int)uconfig->gradkreis );

			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort ) choice_sort->SetSelection( uconfig->sortOrder );
#endif

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
		const int sel = notebook->GetSelection();
		const int tnbId = notebook->GetSelection();
		printf( "TransitView   ---   OnNotebook sel %d pageId %d\n", sel, tnbId );

		switch ( tnbId )
		{
			case TNB_DOUBLE:
				activetext = twidget;
				activewidget = gwidget;
			break;

			case TNB_SBC:
				activetext = (SheetWidget*)NULL;
				activewidget = swidget;
			break;

			case TNB_SHADVARGA:
				activetext = (SheetWidget*)NULL;
				activewidget = shadvargawidget;
			break;

			case TNB_VARGA2:
				activetext = (SheetWidget*)NULL;
				activewidget = varga2widget;
			break;

			case TNB_VARGA3:
				activetext = (SheetWidget*)NULL;
				activewidget = varga3widget;
			break;

#ifdef USE_URANIAN_CHART
			case TNB_URANIAN:
				activetext = (SheetWidget*)NULL;
				activewidget = (BasicWidget*)NULL;
			break;

			case TNB_URANIAN_CHART:
				activetext = (SheetWidget*)NULL;
				activewidget = uranianChart;
			break;
#endif

			default:
				printf( "Error: wrong notebook index %d in TransitView::OnNotebook\n", notebook->GetSelection() );
			break;
		}
		updateCurrentPage();
	}

	/*****************************************************
	**
	**   TransitView   ---   OnDataChanged
	**
	******************************************************/
	virtual void OnDataChanged()
	{
		printf( "TransitView::OnDataChanged\n" );
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
			wxChoice *choice_mode = (wxChoice*)toolbar->FindControl( TBS_TRANSITMODE );
			if ( choice_mode )
			{
				transitmode = (TRANSIT_MODE)choice_mode->GetSelection();
				expert->setTransitMode( transitmode );
			}

#ifdef USE_URANIAN_CHART
			// sort order
			SortChoice *choice_sort = (SortChoice*)toolbar->FindControl( TBS_SORT );
			if ( choice_sort )
			{
				uconfig->sortOrder = (ASPECT_SORTORDER)choice_sort->GetSelection();
				gwidget->getAspectExpert()->setSortOrder( uconfig->sortOrder );
			}

			// Orbis (uranian only)
			wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
			if ( spin_orbis )
			{
				uconfig->orbisTransit = spin_orbis->GetValue() / 60.0;
			}

			GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
			if ( choice_gk )
			{
				uconfig->gradkreis = choice_gk->getGradkreis();
			}
#endif

		}
		expert->update();
		udirty = true;
		uwritten = false;
		updateCurrentPage();
	}

	/*****************************************************
	**
	**   TransitView   ---   activateControls
	**
	******************************************************/
	void activateControls()
	{
		printf( "Activate controls\n" );
#ifdef USE_URANIAN_CHART
		const bool isUranian = ( notebook->GetSelection() == TNB_URANIAN || notebook->GetSelection() == TNB_URANIAN_CHART );
		const bool supportsSort = isUranian || ( notebook->GetSelection() == TNB_DOUBLE && ! isVedic());

		wxSpinCtrl *spin_orbis = (wxSpinCtrl*)toolbar->FindControl( TBS_ORBIS );
		if ( spin_orbis ) spin_orbis->Enable( isUranian );

		wxChoice *choice_sort = (wxChoice*)toolbar->FindControl( TBS_SORT );
		if ( choice_sort ) choice_sort->Enable( supportsSort );

		GradkreisChoice *choice_gk = (GradkreisChoice*)toolbar->FindControl( TBS_GRADKREIS );
		if ( choice_gk ) choice_gk->Enable( isUranian );
/*
		// sort order
		toolbar->enableControl( TBS_SORT, supportsSort );

		// Orbis (uranian only)
		toolbar->enableControl( TBS_ORBIS, isUranian );

		toolbar->enableControl( TBS_GRADKREIS, isUranian );
*/
		toolbar->EnableTool( CMD_FILTER, isUranian  );
#endif
	}

	/*****************************************************
	**
	**   TransitView   ---   updateCurrentPage
	**
	******************************************************/
	void updateCurrentPage()
	{
		const int sel = notebook->GetSelection();
		const int tnbId = notebook->GetSelection();
		printf( "TransitView   ---   updateCurrentPage sel %d pageId %d\n", sel, tnbId );

		activateControls();
		switch ( tnbId )
		{
			case TNB_DOUBLE:
				gwidget->OnDataChanged();
				gwidget->Refresh();
				writeAspectText();
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
#ifdef USE_URANIAN_CHART
			case TNB_URANIAN:
				printf( "updateCurrentPage uranian\n" );
				if ( udirty ) updateUranian();
			break;
			case TNB_URANIAN_CHART:
				printf( "updateCurrentPage TNB_URANIAN_CHART:\n" );
				if ( udirty ) updateUranian();
				uranianChart->OnDataChanged();
				uranianChart->Refresh();
			break;
#endif
		default:
				assert( false );
			break;
		}

		if ( activewidget ) activewidget->Refresh();
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
	**   TransitView   ---   writeAspectText
	**
	******************************************************/
	void writeAspectText()
	{
		twidget->clearSheet();
		TextHelper th( expert->getHoroscope(), props, twidget->getSheet() );
		th.writeTransitReport( expert->getTransitHoroscope(), gwidget->getAspectExpert() );
		twidget->OnDataChanged();
	}

#ifdef USE_URANIAN_CHART
	/*****************************************************
	**
	**   TransitView   ---   OnFilter
	**
	******************************************************/
	void OnFilter( wxCommandEvent& )
	{
		ObjectFilterDialog dialog( this, uexpert->getPlanetList( PC_RADIX ), filter);
		if ( dialog.ShowModal() == wxID_OK )
		{
			filter = dialog.getFilter();
			props->setObjectFilter( filter );
			OnDataChanged();
			setDirty();
		}
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
		helper.writeTransits( uwidget->getSheet(), expert->getTransitMode());
		uwidget->OnDataChanged();
		udirty = false;
	}
#endif

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
		printf( "thetime %f\n", thetime );
		thetime += .5;
		if ( thetime > 1 ) thetime--;
		printf( "thetime %f\n", thetime );
		thetime *= 24;
		printf( "thetime %f\n", thetime );
		toolbar->TransferDataToWindow();

		if ( toolbar )
		{
			/*
			DateSpin *spin_date = (DateSpin*)toolbar->FindControl( TBS_DATE );
			TimeSpin *spin_time = (TimeSpin*)toolbar->FindControl( TBS_TIME );
			if ( spin_date && spin_time )
			{
				double thedate = (int)(transitdate + .5 );
				spin_date->setDoubleValue( thedate -.5 );

				double thetime = ( transitdate - (long)transitdate ) + .5;
				thetime = a_red( 24.0 * thetime, 24 );
				spin_time->setDoubleValue( thetime );
			}
			*/
		}
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
		printf( "TransitView::OnEWToggle %d\n", vedic );
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

	TRANSIT_MODE transitmode;

	// update on next idle event
	bool dirty;
	bool udirty;
	bool uwritten;

	// charts and text views in notebook tabs
	ShadVargaChartGridWidget *shadvargawidget;
	VargaTab2ChartGridWidget *varga2widget;
	VargaTab3ChartGridWidget *varga3widget;
	SheetWidget *twidget;
	ChartGridWidget *gwidget, *swidget;
	ObjectFilter filter;

#ifdef USE_URANIAN_CHART
	UranianExpert *uexpert;
	UranianConfig *uconfig;
	SheetWidget *uwidget;
	ChartGridWidget *uranianChart;
#endif

	SplitterWidget *tsplitter;

	SheetWidget *activetext;
	BasicWidget *activewidget;
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

