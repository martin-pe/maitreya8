/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/mspin.cpp
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

#include "mspin.h"

#include "Calculator.h"
#include "Conf.h"
#include "DataSet.h"
#include "Lang.h"
#include "mvalidator.h"

#include <wx/app.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

extern Config *config;

DEFINE_EVENT_TYPE( COMMAND_SPIN_CHANGED )
DEFINE_EVENT_TYPE( COMMAND_SPIN_WRAP )

//#define DEBUG_MSPIN

enum { BDT_SPIN = wxID_HIGHEST + 650, BDT_TEXT, BDT_LABEL };

IMPLEMENT_CLASS( MBaseSpin, wxControl )
IMPLEMENT_CLASS( MDateSpin, MBaseSpin )
IMPLEMENT_CLASS( MDegSpin, MBaseSpin )

/*************************************************//**
*
*  delegates transfer* and validate stuff to text control
*
******************************************************/
class DelegatingSpinValidator : public wxValidator
{
public:

	DelegatingSpinValidator( wxTextCtrl *c ) : wxValidator(), ctrl( c ) { assert( c ); }
	DelegatingSpinValidator( const DelegatingSpinValidator &tocopy ) : wxValidator(), ctrl( tocopy.ctrl ) {}
	virtual wxObject *Clone() const { return new DelegatingSpinValidator( *this ); }

	virtual bool Validate( wxWindow* )
	{
		assert( ctrl );
		return 	ctrl->Validate();
	}

	virtual bool TransferToWindow()
	{
#ifdef DEBUG_MSPIN
		printf( "DelegatingSpinValidator::TransferToWindow\n" );
#endif
		MBaseSpin *bspin = wxDynamicCast( GetWindow(), MBaseSpin );
		assert( bspin );

		assert( ctrl );
		wxValidator *val = ctrl->GetValidator();
		assert( val );
		bool b = val->TransferToWindow();
		bspin->updateLabel();
		return b;
	}

	virtual bool TransferFromWindow()
	{
#ifdef DEBUG_MSPIN
		printf( "DelegatingSpinValidator::TransferFromWindow\n" );
#endif
		assert( ctrl );
		wxValidator *val = ctrl->GetValidator();
		assert( val );
		return 	val->TransferFromWindow();
	}

private:

	wxTextCtrl *ctrl;
};


/*****************************************************
**
**   MSpinCtrlTextGeneric   ---   Constructor
**
**   code is copied and modified from wx3.0 generic/spinctrl sources
**
******************************************************/
class MSpinCtrlTextGeneric : public wxTextCtrl
{
public:
	MSpinCtrlTextGeneric(MBaseSpin *spin, const wxWindowID id = -1 )
		: wxTextCtrl( spin, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 )
	{
		m_spin = spin;
		processDeletegatedEvents = true;
		SetSizeHints(wxDefaultCoord, wxDefaultCoord);
	}

	virtual ~MSpinCtrlTextGeneric()
	{
#ifdef DEBUG_MSPIN
	printf( "MSpinCtrlTextGeneric::~MSpinCtrlTextGeneric\n" );
#endif

		//printf( "Contents %s\n",str2char( GetValue()));
		//SetValidator( wxDefaultValidator );
		// MSW sends extra kill focus event on destroy
#ifdef __WIN32__
		if (m_spin) m_spin->text = NULL;
#endif
		m_spin = NULL;
	}

	void OnChar( wxKeyEvent &event )
	{
#ifdef DEBUG_MSPIN
		printf( "OnChar\n" );
#endif
		if ( processDeletegatedEvents )
		{
			if ( !m_spin->ProcessEvent(event) ) { event.Skip(); }
		}
	}

	void OnTextEvent(wxCommandEvent& event)
	{
#ifdef DEBUG_MSPIN
		printf( "OnTextEvent processDeletegatedEvents %d\n", processDeletegatedEvents );
#endif

		if ( processDeletegatedEvents )
		{
			wxCommandEvent eventCopy(event);
			eventCopy.SetEventObject(m_spin);
			eventCopy.SetId(m_spin->GetId());
			m_spin->ProcessEvent(eventCopy);
		}
	}

	void OnKillFocus(wxFocusEvent& event)
	{
#ifdef DEBUG_MSPIN
		printf( "MSpinCtrlTextGeneric::OnKillFocus\n" );
#endif
		if ( processDeletegatedEvents )
		{
			assert( m_spin );
			m_spin->doSkipSetFocus = true;
			m_spin->ProcessEvent(event);
			event.Skip();
		}
	}

	MBaseSpin *m_spin;
	bool processDeletegatedEvents;

private:
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MSpinCtrlTextGeneric, wxTextCtrl)
    EVT_CHAR(MSpinCtrlTextGeneric::OnChar)
    EVT_TEXT(wxID_ANY, MSpinCtrlTextGeneric::OnTextEvent)
    EVT_TEXT_ENTER(wxID_ANY, MSpinCtrlTextGeneric::OnTextEvent)
    EVT_KILL_FOCUS(MSpinCtrlTextGeneric::OnKillFocus)
END_EVENT_TABLE()

#define SPIN_MAX_RANGE INT_MAX / 2 - 1

// spin sizes per platform
#ifdef __WIN32__
#define SPIN_SIZE wxSize( 15, -1 )
#elif defined(__WXGTK3__)
#define SPIN_SIZE wxSize( 54, -1 )
#else
#define SPIN_SIZE wxDefaultSize
#endif

/*************************************************//**
*
* 
*
******************************************************/
class MSpinCtrlButtonGeneric : public wxSpinButton
{
public:
	MSpinCtrlButtonGeneric( MBaseSpin *spin, wxWindowID id )
		: wxSpinButton( spin, id, wxDefaultPosition, SPIN_SIZE, wxSP_ARROW_KEYS | wxSP_WRAP)
	{
		m_spin = spin;
		SetRange( - SPIN_MAX_RANGE, SPIN_MAX_RANGE );
	}

	~MSpinCtrlButtonGeneric()
	{
#ifdef DEBUG_MSPIN
	printf( "MSpinCtrlButtonGeneric::~MSpinCtrlButtonGeneric\n" );
#endif
	}

	void OnSetFocus(wxFocusEvent& /*event*/)
	{
#ifdef DEBUG_MSPIN
		printf( "MSpinCtrlButtonGeneric::OnSetFocus\n" );
#endif
	}

protected:
	MBaseSpin *m_spin;

private:
	DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(MSpinCtrlButtonGeneric, wxSpinButton)
    EVT_SET_FOCUS(MSpinCtrlButtonGeneric::OnSetFocus)
END_EVENT_TABLE()

/*****************************************************
**
**   MBaseSpin   ---   Constructor
**
******************************************************/
MBaseSpin::MBaseSpin( wxWindow *parent, int id, const wxPoint pos, const wxSize size, const bool showLabel )
	: wxControl( parent, id, pos, size, wxBORDER_NONE | wxTAB_TRAVERSAL ),
	value( 0 ),
	maxvalue( 0 )
{
	SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );

	//spin = new wxSpinButton( this, BDT_SPIN, wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS | wxSP_WRAP );
	spin = new MSpinCtrlButtonGeneric( this, BDT_SPIN );
	text = new MSpinCtrlTextGeneric( this, BDT_TEXT );

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( text, 1, wxALL|wxEXPAND, 3 );
	sizer->Add( spin, 0, wxALL|wxEXPAND, 3 );

	if ( showLabel )
	{
		label = new wxStaticText( this, BDT_LABEL, wxT( "--" ), wxDefaultPosition, wxSize( 30, -1 ));
		sizer->Add( label, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3 );
	}
	else label = (wxStaticText*)NULL;
	SetSizer( sizer );
	
	sizer->SetSizeHints(this);
	Layout();

	doSkipSetFocus = false;

	Connect( wxEVT_SCROLL_LINEUP, wxSpinEventHandler( MBaseSpin::OnSpinUp ));
	Connect( wxEVT_SCROLL_LINEDOWN, wxSpinEventHandler( MBaseSpin::OnSpinDown ));
	Connect( wxEVT_MOUSEWHEEL, wxMouseEventHandler( MBaseSpin::OnMouseWheelEvent ));
	Connect( wxEVT_CHAR, wxKeyEventHandler( MBaseSpin::OnChar ));
	Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( MBaseSpin::OnSetFocus ));
	Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MBaseSpin::OnKillFocus ));
}

/*****************************************************
**
**   MBaseSpin   ---   Destructor
**
******************************************************/
MBaseSpin::~MBaseSpin()
{
#ifdef DEBUG_MSPIN
	printf( "MBaseSpin::~MBaseSpin\n" );
#endif
	text->processDeletegatedEvents = false;
	//wxWindow::SetValidator( wxDefaultValidator );
	//text->SetValidator( wxDefaultValidator );
}

/*****************************************************
**
**   MBaseSpin   ---   OnMouseWheelEvent
**
******************************************************/
void MBaseSpin::OnMouseWheelEvent( wxMouseEvent &event )
{
	//printf( "MBaseSpin::OnMouseWheelEvent rotation %d\n", event.GetWheelRotation() );
	add( event.GetWheelRotation() > 0 ? 1 : -1 );
}

/*****************************************************
**
**   MBaseSpin   ---   sendChangeEvent
**
******************************************************/
void MBaseSpin::sendChangeEvent()
{
#ifdef DEBUG_MSPIN
	printf( "MBaseSpin::sendChangeEvent\n" );
#endif
	updateLabel();
	wxCommandEvent e( COMMAND_SPIN_CHANGED, GetId());
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   MBaseSpin   ---   sendWrapEvent
**
******************************************************/
void MBaseSpin::sendWrapEvent( const bool forward )
{
	wxCommandEvent e( COMMAND_SPIN_WRAP, GetId());
	e.SetInt( forward ? 1 : -1 );
	wxPostEvent( GetParent(), e );
}

/*****************************************************
**
**   MBaseSpin   ---   SetValidator
**
******************************************************/
void MBaseSpin::SetValidator( const wxValidator &val )
{
	wxString s = val.GetClassInfo()->GetClassName();
	//printf( "MBaseSpin::SetValidator class %s\n", str2char( s ));

	MBaseDoubleValidator *dval = wxDynamicCast( &val, MBaseDoubleValidator );
	assert( dval );
	value = dval->getValue();

	// does not happen if val is MDateValidator
	MDegreeValidator *degval = wxDynamicCast( &val, MDegreeValidator );
	if ( degval )
	{
		maxvalue = degval->getMaxValue();
	}

	text->SetValidator( val );
	wxWindow::SetValidator( DelegatingSpinValidator( text ));
	updateLabel();
}

/*****************************************************
**
**   MBaseSpin   ---   OnTextEnter
**
******************************************************/
void MBaseSpin::OnTextEnter( wxCommandEvent& )
{
	//printf( "TEXT ENTER\n" );
	TransferDataToWindow();
	sendChangeEvent();
}

/*****************************************************
**
**   MBaseSpin   ---   OnChar
**
******************************************************/
void MBaseSpin::OnChar(wxKeyEvent &event )
{
	const int keycode = event.GetKeyCode();
#ifdef DEBUG_MSPIN
	printf( "MBaseSpin::OnChar keycode %d\n", keycode );
#endif

	switch( keycode )
  {
    case WXK_TAB:
    {
      const bool shiftpressed = wxGetKeyState( WXK_SHIFT );
			TransferDataToWindow();
			sendChangeEvent();
			Navigate( ! shiftpressed );
    }
    break;
    case WXK_RETURN:
    {
      //printf( "SPIN RETURN\n" );
			TransferDataToWindow();
			sendChangeEvent();
    }
    break;
    case WXK_DOWN:
      //printf( "SPIN DOWN\n" );
			add( -1 );
    break;
    case WXK_UP:
      //printf( "SPIN UP\n" );
			add( 1 );
    break;
    default:
      //printf( "SPIN DETAULT\n" );
			event.Skip();
    break;
  }
}

/*****************************************************
**
**   MBaseSpin   ---   OnKillFocus
**
******************************************************/
void MBaseSpin::OnKillFocus( wxFocusEvent &event )
{
#ifdef DEBUG_MSPIN
	printf( "MBaseSpin::OnKillFocus\n" );
#endif

	wxWindow *win = event.GetWindow();
	if ( ! win )
	{
#ifdef DEBUG_MSPIN
		printf( "NULL\n" );
#endif
		return;
	}
	wxString s = win->GetClassInfo()->GetClassName();
	if ( win == text )
	{
		// focus goes to the text control, nothing to do
#ifdef DEBUG_MSPIN
		printf( "TEXT %s\n", str2char( s ));
#endif
		event.Skip();
	}
	else
	{
#ifdef DEBUG_MSPIN
		printf( "OTHER %s\n", str2char( s ));
#endif
		event.Skip();
	}
}

/*****************************************************
**
**   MBaseSpin   ---   OnSetFocus
**
******************************************************/
void MBaseSpin::OnSetFocus( wxFocusEvent &event )
{
#ifdef DEBUG_MSPIN
	printf( "MBaseSpin::OnSetFocus doSkipSetFocus %d\n", doSkipSetFocus );
#endif
	if ( ! doSkipSetFocus )
	{
		text->SetFocus();
	}
	else event.Skip();
	doSkipSetFocus = false;
}

/*****************************************************
**
**   MBaseSpin   ---   findToken4add
**
******************************************************/
int MBaseSpin::findToken4add()
{
	assert( text );
	wxString s = text->GetValue();
	//printf( "MBaseSpin::findToken4add %s MAX %f insert point %ld\n", str2char( s ), maxvalue, text->GetInsertionPoint());

	// for datespin potential first '-' does not count, so replace it
	if ( s[0] == '-' ) s[0] = '0';

	const int freq = s.Freq( token );
	
	// field is empty or a plain number
	if ( freq == 0 ) return 0;

	const int t1 = s.Find( token );
	const bool hasfocus = ( wxWindow::FindFocus() == text ) ? true : false;
	long cursorpos = text->GetInsertionPoint();
	//printf( "t1 %d --- FOCUS %d cursor %ld\n", t1, hasfocus, cursorpos );

	if ( hasfocus )
	{
		if ( cursorpos < t1 ) return 0;
		else
		{
			if ( freq == 1 ) { return 1; }
			else
			{
				const int t2 = s.Find( token, true );
				if ( cursorpos > t2 ) return 2;
				else return 1;
			}
		}
	}
	else
	{
		return freq == 1 ? 1 : 2;
	}
	assert( false );
	return 0;
}

/*****************************************************
**
**   MDateSpin   ---   Constructor
**
******************************************************/
MDateSpin::MDateSpin( wxWindow *parent, int id, const wxPoint pos, const wxSize size  )
	: MBaseSpin( parent, id, pos, size, true )
{
	token = '-';
}


/*****************************************************
**
**   MDateSpin   ---   updateLabel
**
******************************************************/
void MDateSpin::updateLabel()
{
	//printf( "MDateSpin::updateLabel\n" );
	Lang lang;
	assert( label );
	const double tmpjd = *value + ( config->defaultLocation->getTimeZone() + config->defaultLocation->getDST() ) / 24.0;

	label->SetLabel( lang.getWeekdayName( ((int)(tmpjd + 1.5 )) % 7).Left( 2 ));
}

/*****************************************************
**
**   MDateSpin   ---   add
**
******************************************************/
void MDateSpin::add( const double &v )
{
	long cursorpos = text->GetInsertionPoint();
	wxString s = text->GetValue();
	int year, month, day;

	Calculator *calculator = CalculatorFactory().getCalculator();
	DateTimeFormatter *formatter = DateTimeFormatter::get();

	formatter->parseDateString( s, day, month, year );
	const int tokenpos = findToken4add();
	//printf( "token pos %d\n", tokenpos );

	switch( tokenpos )
	{
		case 0:
			year += v;
		break;
		case 1:
			month += v;
		break;
		case 2:
		default:
			day += v;
		break;
	}

	assert( value );
	*value = calculator->calc_jd( year, month, day, 0 );
#ifdef DEBUG_MSPIN
		printf( "MDateSpin::add year %d month %d day %d jd %f\n", year, month, day, *value );
#endif

	TransferDataToWindow();
	text->SetInsertionPoint( cursorpos );
	sendChangeEvent();
}

/*****************************************************
**
**   MDegSpin   ---   add
**
******************************************************/
void MDegSpin::add( const double &v )
{
	wxString s = text->GetValue();
	int deg, min, sec;
	deg = min = sec = 0;

	Formatter *formatter = Formatter::get();

	formatter->parseDegreeString( s, deg, min, sec, 360 );

	const long cursorpos = text->GetInsertionPoint();
	const int tokenpos = findToken4add();

	switch( tokenpos )
	{
		case 0:
			deg += v;
		break;
		case 1:
			min += v;
		break;
		case 2:
			sec += v;
		break;
		default:
			assert( false );
		break;
	}

	*value = 3600 * deg + 60 * min + sec;
	*value /= 3600;

	if ( maxvalue )
	{
		if ( *value < 0 )
		{
			*value += maxvalue;
			sendWrapEvent( false );
		}
		else if ( *value >= maxvalue )
		{
			*value -= maxvalue;
			sendWrapEvent( true );
		}
	}

	TransferDataToWindow();
	text->SetInsertionPoint( cursorpos );
	sendChangeEvent();
}




