/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/mvalidator.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License,
 * or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
************************************************************************/

#ifndef _MVALIDATOR_H_
#define _MVALIDATOR_H_

#include <wx/validate.h>
#include <list>

class wxCheckBox;
class wxChoice;
class wxColour;
class wxColourPickerCtrl;
class wxSpinCtrl;
class wxTextCtrl;

using namespace std;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE(COMMAND_SPIN_CHANGED, wxID_HIGHEST + 5001 )
DECLARE_EVENT_TYPE(COMMAND_SPIN_WRAP, wxID_HIGHEST + 5002 )
END_DECLARE_EVENT_TYPES()

/*************************************************//**
*
* 
*
******************************************************/
class MBaseValidator : public wxValidator
{
	DECLARE_CLASS( MBaseValidator )

public:

	MBaseValidator();

protected:

	virtual bool doValidate() { return true; }
	virtual void doKillFocus() { TransferFromWindow(); }

	void OnKillFocus( wxFocusEvent& );
	void OnControlChanged( wxCommandEvent& );

	virtual bool Validate( wxWindow* );

	void setWindowBackground( const bool& );
};

/*************************************************//**
*
* 
*
******************************************************/
class MBaseTextValidator : public MBaseValidator
{
	DECLARE_CLASS( MBaseTextValidator )

public:
	MBaseTextValidator();

protected:

	void setIncludes( const wxString& );
	void setIncludes( const list<wxChar> l ) { allowedChars = l; }

	void OnChar( wxKeyEvent& );

	wxTextCtrl *getTextCtrl() const;
	wxString getTextValue();

	list<wxChar> allowedChars;
};

/*************************************************//**
*
* 
*
******************************************************/
class MBaseDoubleValidator : public MBaseTextValidator
{
	DECLARE_CLASS( MBaseDoubleValidator )

public:
	MBaseDoubleValidator( double *v );
	MBaseDoubleValidator( const MBaseDoubleValidator& );

	double *getValue() const { return value; }

protected:

	virtual bool readValueFromControl( double &v ) = 0;
	virtual void doKillFocus();

	double *value;
};

/*************************************************//**
*
* 
*
******************************************************/
class MDoubleValidator : public MBaseDoubleValidator
{
public:
	MDoubleValidator( double *v, const double &minvalue, const double &maxvalue );
	MDoubleValidator( const MDoubleValidator& );

	virtual wxObject *Clone() const { return new MDoubleValidator( *this ); }

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

protected:

	virtual bool readValueFromControl( double &v );
	virtual bool doValidate();

	const double minvalue;
	const double maxvalue;
};

/*************************************************//**
*
* 
*
******************************************************/
class MIntegerValidator : public MBaseTextValidator
{
public:
	MIntegerValidator( int *v );
	MIntegerValidator( const MIntegerValidator& );

	virtual wxObject *Clone() const { return new MIntegerValidator( *this ); }

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

protected:

	virtual bool doValidate();
	virtual void doKillFocus() { TransferFromWindow(); }

	int *value;
};

/*************************************************//**
*
* 
*
******************************************************/
class MDegreeValidator : public MBaseDoubleValidator
{
public:
	MDegreeValidator( double *v, const double &maxvalue );
	MDegreeValidator( const MDegreeValidator& );

	virtual wxObject *Clone() const { return new MDegreeValidator( *this ); }

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

	double getMaxValue() const { return maxvalue; }

protected:

	virtual bool readValueFromControl( double &v );
	virtual bool doValidate();

	const double maxvalue;
};

/*************************************************//**
*
* 
*
******************************************************/
class MDateValidator : public MBaseDoubleValidator
{
	DECLARE_CLASS( MDateValidator )

public:
	MDateValidator( double *v );
	MDateValidator( const MDateValidator& );

	virtual wxObject *Clone() const { return new MDateValidator( *this ); }

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

protected:

	virtual bool readValueFromControl( double &v );
	virtual bool doValidate();
};

/*************************************************//**
*
* 
*
******************************************************/
class MChoiceValidator : public MBaseValidator
{
public:

	MChoiceValidator( int *c ) : MBaseValidator(), choice( c ) { assert( c ); }
	MChoiceValidator( const MChoiceValidator &tocopy ) : MBaseValidator(), choice( tocopy.choice ) { assert( choice ); }

	virtual wxObject *Clone() const { return new MChoiceValidator( *this ); }

protected:

	wxChoice *getChoiceControl();

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

	int *choice;
};

/*************************************************//**
*
* 
*
******************************************************/
class MSpinValidator : public MBaseValidator
{
public:

	MSpinValidator( int *v ) : MBaseValidator(), value( v ) { assert( value ); }
	MSpinValidator( const MSpinValidator &tocopy ) : MBaseValidator(), value( tocopy.value ) {}

	virtual wxObject *Clone() const { return new MSpinValidator( *this ); }

protected:

	wxSpinCtrl *getSpinControl();

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

	int *value;
};

/*************************************************//**
*
* 
*
******************************************************/
class MBaseCheckValidator : public MBaseValidator
{
public:
	MBaseCheckValidator() : MBaseValidator() {}

protected:
	wxCheckBox *getCheckControl();

};

/*************************************************//**
*
* 
*
******************************************************/
class MCheckValidator : public MBaseCheckValidator
{
public:

	MCheckValidator( bool *v ) : MBaseCheckValidator(), value( v ) { assert( value ); }
	MCheckValidator( const MCheckValidator &tocopy ) : MBaseCheckValidator(), value( tocopy.value ) {}

	virtual wxObject *Clone() const { return new MCheckValidator( *this ); }

protected:

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

	bool *value;
};

/*************************************************//**
*
* 
*
******************************************************/
class MBooleanFlagValidator : public MBaseCheckValidator
{
public:

	MBooleanFlagValidator( int *v, const int &sv ) : MBaseCheckValidator(), value( v ), shiftvalue( sv ) { assert( value ); assert( shiftvalue ); }
	MBooleanFlagValidator( const MBooleanFlagValidator &tocopy ) : MBaseCheckValidator(), value( tocopy.value ), shiftvalue( tocopy.shiftvalue ) {}

	virtual wxObject *Clone() const { return new MBooleanFlagValidator( *this ); }

protected:

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

	int *value;
	const int shiftvalue;
};

/*************************************************//**
*
* 
*
******************************************************/
class MColourPickerValidator : public MBaseValidator
{
public:

	MColourPickerValidator( wxColour *v ) : MBaseValidator(), value( v ) { assert( value ); }
	MColourPickerValidator( const MColourPickerValidator &tocopy ) : MBaseValidator(), value( tocopy.value ) {}

	virtual wxObject *Clone() const { return new MColourPickerValidator( *this ); }

protected:

	wxColourPickerCtrl *getColourPickerControl();

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();
	virtual bool doValidate();

	wxColour *value;
};

/*************************************************//**
*
* 
*
******************************************************/
class MTextValidator : public MBaseTextValidator
{
public:

	MTextValidator( wxString *s ) : MBaseTextValidator(), value( s ) { assert( value ); }
	MTextValidator( const MTextValidator &tocopy ) : MBaseTextValidator(), value( tocopy.value ) {}

	virtual wxObject *Clone() const { return new MTextValidator( *this ); }

protected:

	virtual bool TransferFromWindow();
	virtual bool TransferToWindow();

	wxString *value;
};

/*************************************************//**
*
* 
*
******************************************************/
class MFilenameValidator : public MTextValidator
{
public:

	MFilenameValidator( wxString *s ) : MTextValidator( s ) {}
	MFilenameValidator( const MFilenameValidator &tocopy ) : MTextValidator( tocopy ) {}

	virtual wxObject *Clone() const { return new MFilenameValidator( *this ); }

protected:

	virtual bool TransferFromWindow();
	virtual bool doValidate();

};

/*************************************************//**
*
* 
*
******************************************************/
class MDirectoryValidator : public MTextValidator
{
public:

	MDirectoryValidator( wxString *s ) : MTextValidator( s ) {}
	MDirectoryValidator( const MDirectoryValidator &tocopy ) : MTextValidator( tocopy ) {}

	virtual wxObject *Clone() const { return new MDirectoryValidator( *this ); }

protected:

	virtual bool TransferFromWindow();
	virtual bool doValidate();

};


#endif

