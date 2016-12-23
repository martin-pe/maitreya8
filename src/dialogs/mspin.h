/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/mspin.h
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


#ifndef _MSPIN_H_
#define _MSPIN_H_

#include <wx/control.h>

class wxSpinButton;
class wxSpinEvent;
class wxStaticText;
class wxTextCtrl;

/*************************************************//**
*
* 
*
******************************************************/
class MBaseSpin : public wxControl
{
	DECLARE_CLASS( MBaseSpin )

public:
	MBaseSpin( wxWindow *parent, int id, const wxPoint pos = wxDefaultPosition,
		const wxSize size = wxDefaultSize, bool showLabel = false );
	~MBaseSpin();

	//void update();
	void SetValidator( const wxValidator& );

protected:

	virtual void add( const double &value ) = 0;

	//virtual bool TransferDataToWindow();

	void OnSpinUp( wxSpinEvent& ) { add( 1 ); }
	void OnSpinDown( wxSpinEvent& ) { add( -1 ); }

	void OnMouseWheelEvent( wxMouseEvent& );
	void OnChar(wxKeyEvent& );
	void OnTextEnter( wxCommandEvent& );

	void OnSetFocus( wxFocusEvent& );
	void OnKillFocus( wxFocusEvent& );

	virtual void sendChangeEvent();
	void sendWrapEvent( const bool forward = true );

	int findToken4add();
	virtual void updateLabel() {}

	double *value;
	double maxvalue;

	wxChar token;

	wxTextCtrl *text;
	wxStaticText *label;
	wxSpinButton *spin;

private:
	bool doSkipSetFocus;
};

/*************************************************//**
*
* 
*
******************************************************/
class MDateSpin : public MBaseSpin
{
	DECLARE_CLASS( MDateSpin )
public:

	MDateSpin( wxWindow *parent, int id, const wxPoint pos = wxDefaultPosition, const wxSize size = wxDefaultSize );

	//void setValue( double* );

protected:

	virtual void updateLabel();
	virtual void add( const double &value );
};

/*************************************************//**
*
* 
*
******************************************************/
class MDegSpin : public MBaseSpin
{
	DECLARE_CLASS( MDegSpin )
public:

	MDegSpin( wxWindow *parent, int id, const double maxvalue, const wxPoint pos = wxDefaultPosition, const wxSize size = wxDefaultSize )
		: MBaseSpin( parent, id, pos, size ),
		maxvalue( maxvalue )
	{
		token = ':';
	}

	//void setValue( double* );

protected:

	const double maxvalue;

	virtual void add( const double &value );
};

#endif


