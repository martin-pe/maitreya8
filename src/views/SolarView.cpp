/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/views/SolarView.cpp
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

#include <wx/spinctrl.h>
#include <wx/toolbar.h>

#include "BasicView.h"
#include "Calculator.h"
#include "Commands.h"
#include "Conf.h"
#include "DialogElements.h"
#include "Document.h"
#include "GraphicGrid.h"
#include "Lang.h"
#include "SolarChart.h"
#include "SplitterWidget.h"
#include "TextHelper.h"
#include "TextWidget.h"
#include "ToolPanel.h"

extern Config *config;

/*************************************************//**
*
*  view for Solar (annual or Tajaka) chart
*
******************************************************/
class SolarView : public BasicView
{
	DECLARE_CLASS( SolarView )
public:

	SolarView( wxWindow *parent, ChildWindow *frame )
		: BasicView( parent, frame, VIEW_SOLAR, true )
	{
		expert = new SolarChartExpert( doc, props );
		setCurrrentYear();
		expert->update();

		splitter = new SplitterWidget( this );
		textWidget = new TextWidget( splitter, props );
		textWidget->addWidgetOption( WO_SUPPORTS_EW_TOGGLE );

		gwidget = new DefaultEwChartGridWidget( splitter, CT_RADIX, props, expert->getSolarChart() );
		gwidget->addWesternChart( doc, expert->getSolarChart());
		splitter->SplitVertically( textWidget, gwidget );
		widget = splitter;

		initToolItems();
		OnDataChanged();

		Connect( TBS_YEAR, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( SolarView::OnYearSpin ));
		Connect( CMD_NOW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SolarView::OnNow ));
		Connect( CMD_NEW_ANNUAL_CHART, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SolarView::OnCreateChart ));
	}

	~SolarView()
	{
		config->viewprefs->sashSolar = splitter->GetSashPosition();
		delete expert;
	}

	virtual wxString getWindowLabel( const bool = false ) { return _( "Solar" ); }

	virtual void postCreate()
	{
		wxSplitterWindow *splitter = (wxSplitterWindow*)widget;
		splitter->SetSashPosition( config->viewprefs->sashSolar );
	}

protected:

	void OnDataChanged()
	{
		wxString s;
		DateTimeFormatter *formatter = DateTimeFormatter::get();

		BasicView::OnDataChanged();

		expert->setSolarReturnYear( year );
		expert->update();

		Horoscope *solarchart = expert->getSolarChart();
		Location *location = solarchart->getLocation();
		if ( toolbar )
		{
			ToolbarLabel *text_return = (ToolbarLabel*)toolbar->FindControl( TB_TEXT );
			if ( text_return )
			{
				//s.Printf( wxT( "%s: %s" ), _( "Return" ),
								//formatter->formatFullDateString( solarchart->getJD() + ( location->getTimeZone() + location->getDST())/24 ).c_str() );
				double dummy1;
				int dummy2, dummy3, birthyear;
				CalculatorFactory().getCalculator()->getDateIntsFromJD( doc->getJD(), birthyear, dummy2, dummy3, dummy1 );

				s.Clear();
				s << _( "Return" ) << wxT( ": " )
					<< formatter->formatFullDateString( solarchart->getJD() + ( location->getTimeZone() + location->getDST())/24 )
					<< wxT( ". " );
					if ( year >= birthyear )
					{
						s << wxString::Format( _( "Native is %d years old." ), year - birthyear );
					}
					else
					{
						s << _( "Year is before birth." );
					}
				text_return->SetLabel( s );
			}
			if ( text_return ) text_return->SetLabel( s );
		}

		textWidget->clearSheet();
		expert->writeSolarReturnData( textWidget->getSheet() );
		textWidget->OnDataChanged();

		gwidget->OnDataChanged();
		gwidget->Refresh();
	}


	void setCurrrentYear()
	{
		int bday, aday, bmonth, amonth, byear;
		DateTimeFormatter *formatter = DateTimeFormatter::get();

		formatter->calculateDateIntegers( MDate::getCurrentJD(), aday, amonth, year );
		formatter->calculateDateIntegers( doc->getJD(), bday, bmonth, byear );
		if ( amonth < bmonth || ( amonth == bmonth && aday < bday )) year--;
	}

	void OnNow( wxCommandEvent& )
	{
		setCurrrentYear();
		OnDataChanged();
		if ( toolbar )
		{
			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) spin_year->SetValue( year );
		}
	}

	void OnCreateChart( wxCommandEvent& )
	{
		Document *d = new Document;
		d->setLocation( *doc->getLocation() );
		d->setDate( expert->getSolarChart()->getJD() );
		d->setHName( wxString::Format( wxT( "%s: %s (%d)" ), doc->getHName().c_str(), _( "Annual Chart" ), year ));

		wxCommandEvent event( CREATE_ENTRY_CHART, GetParent()->GetId() );
		event.SetEventObject( (wxObject*)d );
		wxPostEvent( GetParent(), event );
	}

	void OnYearSpin( wxSpinEvent& )
	{
		if ( toolbar )
		{
			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) year = spin_year->GetValue();
		}
		OnDataChanged();
	}

	virtual void initToolItems()
	{
		if ( toolbar )
		{
			wxSpinCtrl *spin_year = (wxSpinCtrl*)toolbar->FindControl( TBS_YEAR );
			if ( spin_year ) spin_year->SetValue( year );
		}
	}

	DefaultEwChartGridWidget *gwidget;
	SolarChartExpert *expert;
	SplitterWidget *splitter;
	TextWidget *textWidget;
	int year;

};


IMPLEMENT_CLASS( SolarView, BasicView )

/**************************************************************
***
**   ViewFactory   ---   createSolarView
***
***************************************************************/
BasicView *ViewFactory::createSolarView( wxWindow *parent, ChildWindow *frame )
{
	return new SolarView( parent, frame );
}


