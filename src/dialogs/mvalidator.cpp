/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/mvalidator.cpp
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

#include "mvalidator.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "Lang.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/settings.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>

extern Config *config;

//#define DEBUG_MVALIDATOR

IMPLEMENT_CLASS( MBaseValidator, wxValidator )
IMPLEMENT_CLASS( MBaseTextValidator, MBaseValidator )
IMPLEMENT_CLASS( MBaseDoubleValidator, MBaseTextValidator )
IMPLEMENT_CLASS( MDateValidator, MBaseDoubleValidator )

/*****************************************************
**
**   MBaseValidator   ---   Constructor
**
******************************************************/
MBaseValidator::MBaseValidator()
 : wxValidator()
{
	Connect( wxEVT_KILL_FOCUS, wxFocusEventHandler( MBaseValidator::OnKillFocus ));
	Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( MBaseValidator::OnControlChanged ));
	Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( MBaseValidator::OnControlChanged ));
	Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler( MBaseValidator::OnControlChanged ));
	Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( MBaseValidator::OnControlChanged ));
}

/*****************************************************
**
**   MBaseValidator   ---   OnKillFocus
**
******************************************************/
void MBaseValidator::OnKillFocus( wxFocusEvent &event )
{
	//printf( "MBaseValidator::OnKillFocusd\n" );
	doKillFocus();
	event.Skip();
}

/*****************************************************
**
**   MBaseValidator   ---   OnControlChanged
**
******************************************************/
void MBaseValidator::OnControlChanged( wxCommandEvent &event )
{
	//printf( "MBaseValidator::OnInputControlChanged\n" );
	doValidate();
	TransferFromWindow();
	event.Skip();
}

/*****************************************************
**
**   MBaseValidator   ---   doValidate
**
******************************************************/
bool MBaseValidator::Validate( wxWindow* )
{
	return doValidate();
}

/*****************************************************
**
**   MBaseValidator   ---   setWindowBackground
**
******************************************************/
void MBaseValidator::setWindowBackground( const bool &b )
{
	//printf( "MBaseValidator::setWindowBackground valid %d\n", b );
	wxWindow *w = GetWindow();
	//wxWindow *w = getTextCtrl();
	assert( w );

	w->SetBackgroundColour( b ? wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOW ) : config->colors->errorBgColor );

	// must  be set, otherwise forground will be white (GTK, wx 2.8.12)
	w->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_WINDOWTEXT ));
	w->Refresh();
}

/*****************************************************
**
**   MBaseTextValidator   ---   Constructor
**
******************************************************/
MBaseTextValidator::MBaseTextValidator()
	: MBaseValidator()
{
	Connect( wxEVT_CHAR, wxKeyEventHandler( MBaseTextValidator::OnChar ));
}

/*****************************************************
**
**   MBaseTextValidator   ---   getTextCtrl
**
******************************************************/
wxTextCtrl *MBaseTextValidator::getTextCtrl() const
{
	wxTextCtrl *c = wxDynamicCast( GetWindow(), wxTextCtrl );
	assert( c );
	return c;
}

/*****************************************************
**
**   MBaseTextValidator   ---   getTextValue
**
******************************************************/
wxString MBaseTextValidator::getTextValue()
{
	wxTextCtrl *ctrl = getTextCtrl();
	return ctrl->GetValue();
}

/*****************************************************
**
**   MBaseTextValidator   ---   setIncludes
**
******************************************************/
void MBaseTextValidator::setIncludes( const wxString &val )
{
	allowedChars.clear();
	for ( wxString::const_iterator i = val.begin(); i != val.end(); ++i )
	{
		allowedChars.push_back( *i );
	}
}

/*****************************************************
**
**   MBaseTextValidator   ---   OnChar
**
******************************************************/
void MBaseTextValidator::OnChar( wxKeyEvent &event )
{
	const int keycode = event.GetKeyCode();
#ifdef DEBUG_MVALIDATOR
	printf( "MBaseTextValidator::OnChar %d\n", keycode );
#endif
	if( allowedChars.size() == 0 )
	{
		// should be okay, no filter set
		event.Skip();
		return;
	}

	// special handling for OS x and maybe windows
	//if ( keycode == WXK_RETURN || keycode == WXK_TAB )
	if ( keycode == WXK_RETURN )
	{
		//printf( "RETURN\n" );
		wxCommandEvent e( wxEVT_COMMAND_TEXT_ENTER, GetWindow()->GetId());
		wxPostEvent( GetWindow(), e );
		TransferToWindow();
		return;
	}
	else if ( keycode == WXK_TAB )
	{
#ifdef DEBUG_MVALIDATOR
		printf( "TAB\n" );
#endif
		event.Skip();
		return;
	}
	else if ( keycode < WXK_SPACE || keycode == WXK_DELETE || keycode >= WXK_START )
	{
		//printf( "SKIP MBaseTextValidator::OnChar %d\n", keycode );
		event.ResumePropagation( wxEVENT_PROPAGATE_MAX );
		event.Skip();
		return;
	}

	for( list<wxChar>::const_iterator iter = allowedChars.begin(); iter != allowedChars.end(); iter++ )
	{
		if ( *iter == keycode )
		{
			event.Skip();
			return;
		}
	}
	//printf( "MBaseTextValidator::OnChar Wrong char %d\n", keycode );
}

/*****************************************************
**
**   MBaseDoubleValidator   ---   Constructor
**
******************************************************/
MBaseDoubleValidator::MBaseDoubleValidator( double *v )
	: MBaseTextValidator(),
	value( v )
{
	//printf( "MBaseDoubleValidator::MBaseDoubleValidator value %f\n", *value );
	assert( value );
}

/*****************************************************
**
**   MBaseDoubleValidator   ---   Constructor
**
******************************************************/
MBaseDoubleValidator::MBaseDoubleValidator( const MBaseDoubleValidator &tocopy )
	: MBaseTextValidator(),
	value ( tocopy.value )
{
	//printf( "MBaseDoubleValidator::MBaseDoubleValidator COPY value %f\n", *value );
	allowedChars = tocopy.allowedChars;
	Connect( wxEVT_CHAR, wxKeyEventHandler( MBaseDoubleValidator::OnChar ));
}

/*****************************************************
**
**   MBaseDoubleValidator   ---   doKillFocus
**
******************************************************/
void MBaseDoubleValidator::doKillFocus()
{
	//printf( "MBaseDoubleValidator::doKillFocus\n" );
	double v;
	const bool isvalid = readValueFromControl( v );

	//enum  { IFV_NONE = 0, IFV_RESTORE, IFV_ADJUST };
	switch ( config->view->inputFieldValidation )
	{
		case IFV_NONE:
			if ( isvalid )
			{
				*value = v;
				TransferToWindow();
			}
		break;
		case IFV_RESTORE:
			if ( isvalid )
			{
				*value = v;
			}
			TransferToWindow();
			setWindowBackground( true );
		break;
		case IFV_ADJUST:
			*value = v;
			TransferToWindow();
			setWindowBackground( true );
		break;
		default:
			assert( false );
		break;
	}
}

/*****************************************************
**
**   MDoubleValidator   ---   Constructor
**
******************************************************/
MDoubleValidator::MDoubleValidator( double *v, const double &minvalue, const double &maxvalue )
	: MBaseDoubleValidator( v ),
	minvalue( minvalue ),
	maxvalue( maxvalue )
{
	//printf( "MDoubleValidator::MDegreeValidator value %f maxvalue %f\n", *value, maxvalue );
	setIncludes( wxT( "0123456789,." ));
}

/*****************************************************
**
**   MDoubleValidator   ---   Constructor
**
******************************************************/
MDoubleValidator::MDoubleValidator( const MDoubleValidator &tocopy )
	: MBaseDoubleValidator( tocopy ),
	minvalue( tocopy.minvalue ),
	maxvalue( tocopy.maxvalue )
{
	//printf( "COPY MDoubleValidator::MDoubleValidator value %f maxvalue %f\n", *value, maxvalue );
}

/*****************************************************
**
**   MDoubleValidator   ---   TransferFromWindow
**
******************************************************/
bool MDoubleValidator::TransferFromWindow()
{
	wxString s = getTextValue();
	double v;

	Formatter *formatter = Formatter::get();
	const bool isvalid = formatter->parseDoubleString( s, v, minvalue, maxvalue );
	//printf( "MDoubleValidator::MDoubleValidator %s minvalue %f maxvalud %f isvalid %d\n", str2char( s ), minvalue, maxvalue, isvalid );

	if ( isvalid )
	{
		*value = v;
	}
	return isvalid;
}

/*****************************************************
**
**   MDoubleValidator   ---   TransferToWindow
**
******************************************************/
bool MDoubleValidator::TransferToWindow()
{
	Formatter *formatter = Formatter::get();
	wxString s = formatter->formatDoubleString( *value, DEG_PRECISION_FLEXIBLE );
	//printf( "MDoubleValidator::MDoubleValidator value %f string value %s\n", *value, str2char( s ) );
	wxTextCtrl *ctrl = getTextCtrl();
	ctrl->ChangeValue( formatter->formatDoubleString( *value, DEG_PRECISION_FLEXIBLE ));
	return true;
}

/*****************************************************
**
**   MDoubleValidator   ---   doValidate
**
******************************************************/
bool MDoubleValidator::doValidate()
{
	wxString s = getTextValue();
	double v;
	Formatter *formatter = Formatter::get();
	const bool isvalid = formatter->parseDoubleString( s, v, minvalue, maxvalue );

	//printf( "MDoubleValidator::doValidate text %s v %f valid %d\n", str2char( s ), v, isvalid );
	setWindowBackground( isvalid );
	return isvalid;
}

/*****************************************************
**
**   MDoubleValidator   ---   readValueFromControl
**
******************************************************/
bool MDoubleValidator::readValueFromControl( double &v )
{
	Formatter *formatter = Formatter::get();
	wxString s = getTextValue();

	return formatter->parseDoubleString( s, v, minvalue, maxvalue );
}

/*****************************************************
**
**   MIntegerValidator   ---   Constructor
**
******************************************************/
MIntegerValidator::MIntegerValidator( int *v )
	: MBaseTextValidator(),
	value( v )
{
	setIncludes( wxT( "0123456789" ));
}

/*****************************************************
**
**   MIntegerValidator   ---   Constructor
**
******************************************************/
MIntegerValidator::MIntegerValidator( const MIntegerValidator &tocopy )
	: MBaseTextValidator(),
	value( tocopy.value )
{
	allowedChars = tocopy.allowedChars;
}

/*****************************************************
**
**   MIntegerValidator   ---   TransferFromWindow
**
******************************************************/
bool MIntegerValidator::TransferFromWindow()
{
	wxString s = getTextValue();
	long l;
	const bool b = s.ToLong( &l );
	*value = (int)l;
	return b;
}

/*****************************************************
**
**   MIntegerValidator   ---   TransferToWindow
**
******************************************************/
bool MIntegerValidator::TransferToWindow()
{
	wxTextCtrl *ctrl = getTextCtrl();
	ctrl->ChangeValue( wxString::Format( wxT( "%d" ), *value ));
	return true;
}

/*****************************************************
**
**   MIntegerValidator   ---   doValidate
**
******************************************************/
bool MIntegerValidator::doValidate()
{
	wxString s = getTextValue();
	long l;
	const bool b = s.ToLong( &l );
	return b;
}

/*****************************************************
**
**   MDegreeValidator   ---   Constructor
**
******************************************************/
MDegreeValidator::MDegreeValidator( double *v, const double &maxvalue )
	: MBaseDoubleValidator( v ),
	maxvalue( maxvalue )
{
	//printf( "MDegreeValidator::MDegreeValidator value %f maxvalue %f\n", *value, maxvalue );
	setIncludes( wxT( "0123456789:" ));
}

/*****************************************************
**
**   MDegreeValidator   ---   Constructor
**
******************************************************/
MDegreeValidator::MDegreeValidator( const MDegreeValidator &tocopy )
	: MBaseDoubleValidator( tocopy ),
	maxvalue( tocopy.maxvalue )
{
	//printf( "COPY MDegreeValidator::MDegreeValidator value %f maxvalue %f\n", *value, maxvalue );
}

/*****************************************************
**
**   MDegreeValidator   ---   TransferFromWindow
**
******************************************************/
bool MDegreeValidator::TransferFromWindow()
{
	wxString s = getTextValue();
	//printf( "MDegreeValidator::TransferFromWindow %s\n", str2char( s ) );
	double v;
	Formatter *formatter = Formatter::get();
	const bool isvalid = formatter->parseDegreeString( s, v, maxvalue );
	if ( isvalid )
	{
		*value = v;
	}
	return isvalid;
}

/*****************************************************
**
**   MDegreeValidator   ---   TransferToWindow
**
******************************************************/
bool MDegreeValidator::TransferToWindow()
{
	Formatter *formatter = Formatter::get();
	wxString s = formatter->formatDegreeString( *value, DEG_PRECISION_FLEXIBLE );
	//printf( "MDegreeValidator::TransferToWindow value %f string value %s\n", *value, str2char( s ) );
	wxTextCtrl *ctrl = getTextCtrl();

	ctrl->ChangeValue( formatter->formatDegreeString( *value, DEG_PRECISION_FLEXIBLE ));
	return true;
}

/*****************************************************
**
**   MDegreeValidator   ---   doValidate
**
******************************************************/
bool MDegreeValidator::doValidate()
{
	wxString s = getTextValue();
	double v;
	Formatter *formatter = Formatter::get();
	const bool isvalid = formatter->parseDegreeString( s, v, maxvalue );

	//printf( "MDegreeValidator::doValidate text %s v %f valid %d\n", str2char( s ), v, isvalid );
	setWindowBackground( isvalid );
	return isvalid;
}

/*****************************************************
**
**   MDegreeValidator   ---   readValueFromControl
**
******************************************************/
bool MDegreeValidator::readValueFromControl( double &v )
{
	Formatter *formatter = Formatter::get();
	wxString s = getTextValue();

	return formatter->parseDegreeString( s, v, maxvalue );
}

/*****************************************************
**
**   MDateValidator   ---   Constructor
**
******************************************************/
MDateValidator::MDateValidator( double *v )
	: MBaseDoubleValidator( v )
{
	//printf( "MDateValidator::MDateValidator value %f\n", *value );
	setIncludes( wxT( "0123456789-" ));
}

/*****************************************************
**
**   MDateValidator   ---   Constructor
**
******************************************************/
MDateValidator::MDateValidator( const MDateValidator &tocopy )
	: MBaseDoubleValidator( tocopy )
{
}

/*****************************************************
**
**   MDateValidator   ---   TransferFromWindow
**
******************************************************/
bool MDateValidator::TransferFromWindow()
{
	//printf( "MDateValidator::TransferFromWindow\n" );
	wxString s = getTextValue();
	double v;
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	const bool isvalid =  formatter->parseDateString( s, v );
	if ( isvalid )
	{
		*value = v;
	}
	return isvalid;
}

/*****************************************************
**
**   MDateValidator   ---   TransferToWindow
**
******************************************************/
bool MDateValidator::TransferToWindow()
{
	//printf( "MDateValidator::iTransferToWindow\n" );
	wxTextCtrl *ctrl = getTextCtrl();

	DateTimeFormatter *formatter = DateTimeFormatter::get();
	ctrl->ChangeValue( formatter->formatDateString( *value ));
	return true;
}

/*****************************************************
**
**   MDateValidator   ---   doValidate
**
******************************************************/
bool MDateValidator::doValidate()
{
	wxString s = getTextValue();
	double v;
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	const bool isvalid =  formatter->parseDateString( s, v );

	//printf( "MDateValidator::doValidate text %s v %f valid %d\n", str2char( s ), v, isvalid );
	setWindowBackground( isvalid );
	return isvalid;
}

/*****************************************************
**
**   MDateValidator   ---   readValueFromControl
**
******************************************************/
bool MDateValidator::readValueFromControl( double &v )
{
	DateTimeFormatter *formatter = DateTimeFormatter::get();
	wxString s = getTextValue();

	return formatter->parseDateString( s, v );
}

/*****************************************************
**
**   MChoiceValidator   ---   getChoiceControl
**
******************************************************/
wxChoice *MChoiceValidator::getChoiceControl()
{
	wxChoice *c = wxDynamicCast( GetWindow(), wxChoice );
	assert( c );
	return c;
}

/*****************************************************
**
**   MChoiceValidator   ---   TransferFromWindow
**
******************************************************/
bool MChoiceValidator::TransferFromWindow()
{
	*choice = getChoiceControl()->GetSelection();
	//printf( "MChoiceValidator::TransferFromWindow choice %d\n", *choice );
	return true;
}

/*****************************************************
**
**   MChoiceValidator   ---   TransferToWindow
**
******************************************************/
bool MChoiceValidator::TransferToWindow()
{
	//printf( "MChoiceValidator::TransferToWindow choice %d\n", *choice );
	getChoiceControl()->SetSelection( *choice );
	return true;
}

/*****************************************************
**
**   MSpinValidator   ---   getSpinControl
**
******************************************************/
wxSpinCtrl *MSpinValidator::getSpinControl()
{
	wxSpinCtrl *c = wxDynamicCast( GetWindow(), wxSpinCtrl );
	assert( c );
	return c;
}

/*****************************************************
**
**   MSpinValidator   ---   TransferFromWindow
**
******************************************************/
bool MSpinValidator::TransferFromWindow()
{
	*value = getSpinControl()->GetValue();
	//printf( "MSpinValidator::TransferFromWindow value %d\n", *value );
	return true;
}

/*****************************************************
**
**   MSpinValidator   ---   TransferToWindow
**
******************************************************/
bool MSpinValidator::TransferToWindow()
{
	//printf( "MSpinValidator::TransferToWindow value %d\n", *value );
	getSpinControl()->SetValue( *value );
	return true;
}

/*****************************************************
**
**   MBaseCheckValidator   ---   getCheckControl
**
******************************************************/
wxCheckBox *MBaseCheckValidator::getCheckControl()
{
	wxCheckBox *c = wxDynamicCast( GetWindow(), wxCheckBox );
	assert( c );
	return c;
}

/*****************************************************
**
**   MCheckValidator   ---   TransferFromWindow
**
******************************************************/
bool MCheckValidator::TransferFromWindow()
{
	*value = getCheckControl()->GetValue();
	//printf( "MCheckValidator::TransferFromWindow value %d\n", *value );
	return true;
}

/*****************************************************
**
**   MCheckValidator   ---   TransferToWindow
**
******************************************************/
bool MCheckValidator::TransferToWindow()
{
	//printf( "MCheckValidator::TransferToWindow value %d\n", *value );
	getCheckControl()->SetValue( *value );
	return true;
}

/*****************************************************
**
**   MBooleanFlagValidator   ---   TransferFromWindow
**
******************************************************/
bool MBooleanFlagValidator::TransferFromWindow()
{
	//printf( "MBooleanFlagValidator::TransferFromWindow value %d shiftvalue %d\n", *value, shiftvalue );
	if ( getCheckControl()->GetValue() )
	{
		*value |= shiftvalue;
	}
	else
	{
		*value &= ~shiftvalue;
	}
	//printf( "AFTER MBooleanFlagValidator::TransferFromWindow value %d shiftvalue %d\n", *value, shiftvalue );
	return true;
}

/*****************************************************
**
**   MBooleanFlagValidator   ---   TransferToWindow
**
******************************************************/
bool MBooleanFlagValidator::TransferToWindow()
{
	//printf( "MBooleanFlagValidator::TransferToWindow value %d\n", *value );
	getCheckControl()->SetValue( *value & shiftvalue );
	return true;
}

/*****************************************************
**
**   MColourPickerValidator   ---   getColourPickerControl
**
******************************************************/
wxColourPickerCtrl *MColourPickerValidator::getColourPickerControl()
{
	wxColourPickerCtrl *c = wxDynamicCast( GetWindow(), wxColourPickerCtrl );
	assert( c );
	return c;
}

/*****************************************************
**
**   MColourPickerValidator   ---   TransferFromWindow
**
******************************************************/
bool MColourPickerValidator::TransferFromWindow()
{
	wxString color = getColourPickerControl()->GetColour().GetAsString(  wxC2S_CSS_SYNTAX );
	//printf( "MColourPickerValidator::TransferFromWindow color %s\n", str2char( color ));
	*value = getColourPickerControl()->GetColour();
	return true;
}

/*****************************************************
**
**   MColourPickerValidator   ---   TransferToWindow
**
******************************************************/
bool MColourPickerValidator::TransferToWindow()
{
	wxString color = getColourPickerControl()->GetColour().GetAsString(  wxC2S_CSS_SYNTAX );
	//printf( "MColourPickerValidator::TransferToWindow color %s\n", str2char( color ));
	getColourPickerControl()->SetColour( *value );
	return true;
}

/*****************************************************
**
**   MColourPickerValidator   ---   doValidate
**
******************************************************/
bool MColourPickerValidator::doValidate()
{
	return getColourPickerControl()->GetColour().IsOk();
	return true;
}

/*****************************************************
**
**   MTextValidator   ---   TransferFromWindow
**
******************************************************/
bool MTextValidator::TransferFromWindow()
{
	*value = getTextCtrl()->GetValue();
	//printf( "MTextValidator::TransferFromWindow value %s\n", str2char( (*value) ));
	return true;
}

/*****************************************************
**
**   MTextValidator   ---   TransferToWindow
**
******************************************************/
bool MTextValidator::TransferToWindow()
{
	//printf( "MTextValidator::TransferToWindow value %s\n", str2char( (*value) ));
	getTextCtrl()->ChangeValue( *value );
	return true;
}

/*****************************************************
**
**   MFilenameValidator   ---   TransferFromWindow
**
******************************************************/
bool MFilenameValidator::TransferFromWindow()
{
	wxString filename = getTextValue();
	const bool isvalid = doValidate();
	if ( isvalid )
	{
		*value = filename;
	}
	return isvalid;
}

/*****************************************************
**
**   MFilenameValidator   ---   doValidate
**
******************************************************/
bool MFilenameValidator::doValidate()
{
	wxString filename = getTextValue();
	bool isvalid = wxFileName::FileExists( filename );
	if ( isvalid && ! wxFileName::IsFileReadable( filename ))
	{
		isvalid = false;
	}
	setWindowBackground( isvalid );
	return isvalid;
}

/*****************************************************
**
**   MDirectoryValidator   ---   TransferFromWindow
**
******************************************************/
bool MDirectoryValidator::TransferFromWindow()
{
	wxString dirname = getTextValue();
	const bool isvalid = ( dirname.IsEmpty() || wxDir::Exists( dirname ));
	//printf( "MDirectoryValidator::TransferFromWindow isvalid %d config->view->inputFieldValidations %d= IFV_ADJUST %d\n",
		//config->view->inputFieldValidation, IFV_ADJUST, isvalid );
	if ( isvalid || config->view->inputFieldValidation != IFV_RESTORE )
	{
		*value = dirname;
	}
	return isvalid;
}

/*****************************************************
**
**   MDirectoryValidator   ---   doValidate
**
******************************************************/
bool MDirectoryValidator::doValidate()
{
	wxString dirname = getTextValue();
	const bool isvalid = ( dirname.IsEmpty() || wxDir::Exists( dirname ));
	//printf( "MDirectoryValidator::doValidate isvalid %d\n", isvalid );
	setWindowBackground( isvalid );
	return isvalid;
}


