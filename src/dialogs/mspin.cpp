/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/mspin.cpp
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

#include "mspin.h"

#include "Calculator.h"
#include "DataSet.h"
#include "Lang.h"
#include "mathbase.h"
#include "mvalidator.h"

#include <wx/app.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/spinbutt.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>

DEFINE_EVENT_TYPE( COMMAND_SPIN_CHANGED )
DEFINE_EVENT_TYPE( COMMAND_SPIN_WRAP )

enum { BDT_SPIN = wxID_HIGHEST + 650, BDT_TEXT, BDT_LABEL };

IMPLEMENT_CLASS( MBaseSpin, wxControl )
IMPLEMENT_CLASS( MDateSpin, MBaseSpin )
IMPLEMENT_CLASS( MDegSpin, MBaseSpin )

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
		printf( "DelegatingSpinValidator::TransferToWindow\n" );
		assert( ctrl );
		wxValidator *val = ctrl->GetValidator();
		assert( val );
		return 	val->TransferToWindow();
	}

	virtual bool TransferFromWindow()
	{
		printf( "DelegatingSpinValidator::TransferFromWindow\n" );
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
**   MBaseSpin   ---   Constructor
**
******************************************************/
MBaseSpin::MBaseSpin( wxWindow *parent, int id, const wxPoint pos, const wxSize size )
	: wxControl( parent, id, pos, size, wxBORDER_NONE ), // | wxTAB_TRAVERSAL )
	value( 0 ),
	maxvalue( 0 )
{
	SetExtraStyle( wxWS_EX_VALIDATE_RECURSIVELY );

	spin = new wxSpinButton( this, BDT_SPIN, wxDefaultPosition, wxSize( 15, -1 ), wxSP_ARROW_KEYS | wxSP_WRAP );
	text = new wxTextCtrl( this, BDT_TEXT, wxEmptyString, wxDefaultPosition, wxSize( 100, -1 ));

	wxBoxSizer* sizer = new wxBoxSizer( wxHORIZONTAL );
	sizer->Add( text, 1, wxALL|wxEXPAND, 3 );
	sizer->Add( spin, 0, wxALL|wxEXPAND, 3 );

	SetAutoLayout( true );
	SetSizer( sizer );
	sizer->Fit( this );
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
	text->SetValidator( wxDefaultValidator );
	wxWindow::SetValidator( wxDefaultValidator );
}

/*****************************************************
**
**   MBaseSpin   ---   SetValidator
**
******************************************************/
void MBaseSpin::SetValidator( const wxValidator &val )
{
	printf( "MBaseSpin::SetValidator\n" );

	MBaseDoubleValidator *dval = wxDynamicCast( &val, MBaseDoubleValidator );
	assert( dval );
	value = dval->getValue();

	MDegreeValidator *degval = wxDynamicCast( &val, MDegreeValidator );
	if ( degval )
	{
		maxvalue = degval->getMaxValue();
	}

	text->SetValidator( val );
	wxWindow::SetValidator( DelegatingSpinValidator( text ));
}

/*****************************************************
**
**   MBaseSpin   ---   OnMouseWheelEvent
**
******************************************************/
void MBaseSpin::OnMouseWheelEvent( wxMouseEvent &event )
{
	printf( "MBaseSpin::OnMouseWheelEvent rotation %d\n", event.GetWheelRotation() );
	add( event.GetWheelRotation() > 0 ? 1 : -1 );
}

/*****************************************************
**
**   MBaseSpin   ---   add
**
******************************************************/
void MBaseSpin::add( const double &v )
{
	long cursorpos = text->GetInsertionPoint();
	printf( "MBaseSpin::add delta %f value %f cursor %ld\n", v, *value, cursorpos );
	*value += v;
	TransferDataToWindow();
	text->SetInsertionPoint( cursorpos );

	sendChangeEvent();
}

/*****************************************************
**
**   MBaseSpin   ---   sendChangeEvent
**
******************************************************/
void MBaseSpin::sendChangeEvent()
{
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
**   MBaseSpin   ---   OnChar
**
******************************************************/
void MBaseSpin::OnChar(wxKeyEvent &event )
{
	const int keycode = event.GetKeyCode();
	wxLogMessage( wxT( "MBaseSpin::OnChar keycode %d" ), keycode );
	switch( keycode )
  {
    case WXK_TAB:
    {
      const bool shiftpressed = wxGetKeyState( WXK_SHIFT );
      printf( "MBaseSpin::OnChar SPIN TAB shift %d\n", shiftpressed );
			//GetParent()->GetParent()->Navigate( ! shiftpressed );
			//text->KillFocus();
			doSkipSetFocus = true;
			Navigate( ! shiftpressed );
    }
    break;
    case WXK_RETURN:
    {
      printf( "SPIN RETURN\n" );
			TransferDataToWindow();
    }
    break;
    case WXK_DOWN:
      printf( "SPIN DOWN\n" );
			add( -1 );
    break;
    case WXK_UP:
      printf( "SPIN UP\n" );
			add( 1 );
    break;
    default:
      printf( "SPIN DETAULT\n" );
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
	printf( "MBaseSpin::OnKillFocus\n" );
	event.Skip();
	doSkipSetFocus = false;
}

/*****************************************************
**
**   MBaseSpin   ---   OnSetFocus
**
******************************************************/
void MBaseSpin::OnSetFocus( wxFocusEvent &event )
{
	printf( "MBaseSpin::OnSetFocus doSkipSetFocus %d\n", doSkipSetFocus );

	if ( ! doSkipSetFocus ) text->SetFocus();
	else event.Skip();
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
	printf( "MBaseSpin::findToken4add %s MAX %f insert point %ld\n", str2char( s ), maxvalue, text->GetInsertionPoint());

	// for datespin potential first '-' does not count, so replace it
	if ( s[0] == '-' ) s[0] = '0';

	const int freq = s.Freq( token );
	
	// field is empty or a plain number
	if ( freq == 0 ) return 0;

	const int t1 = s.Find( token );
	const bool hasfocus = ( wxWindow::FindFocus() == text ) ? true : false;
	long cursorpos = text->GetInsertionPoint();
	printf( "t1 %d --- FOCUS %d cursor %ld\n", t1, hasfocus, cursorpos );

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

	*value = calculator->calc_jd( year, month, day, 12 );

	printf( "MDateSpin::add delta %f value %f cursor %ld\n", v, *value, cursorpos );
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

	//Calculator *calculator = CalculatorFactory().getCalculator();
	Formatter *formatter = Formatter::get();

	formatter->parseDegreeString( s, deg, min, sec, 360 );
	printf( "HALLO 1 s %s deg %d min %d sec %d\n", str2char( s ), deg, min, sec );

	const long cursorpos = text->GetInsertionPoint();
	//int freq = s.Freq( token );
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
	//printf( "HALLO freq %d target pos %d ------  deg %d min %d sec %d\n", freq, findToken4add(), deg, min, sec );

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

	printf( "MDegSpin::add delta %f value %f cursor %ld\n", v, *value, cursorpos );
	TransferDataToWindow();
	text->SetInsertionPoint( cursorpos );
	sendChangeEvent();
}




