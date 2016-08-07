/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/mspin.h
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


#ifndef _MSPIN_H_
#define _MSPIN_H_

#include <wx/control.h>
#include <wx/valtext.h>

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
	MBaseSpin( wxWindow *parent, int id, const wxPoint pos = wxDefaultPosition, const wxSize size = wxDefaultSize );
	~MBaseSpin();

	virtual void SetValidator( const wxValidator& );

protected:

	virtual void add( const double &value ) = 0;

	void OnSpinUp( wxSpinEvent& ) { add( 1 ); }
	void OnSpinDown( wxSpinEvent& ) { add( -1 ); }

	void OnMouseWheelEvent( wxMouseEvent& );
	void OnChar(wxKeyEvent& );

	void OnSetFocus( wxFocusEvent& );
	void OnKillFocus( wxFocusEvent& );

	void sendChangeEvent();
	void sendWrapEvent( const bool forward = true );

	int findToken4add();

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

	MDateSpin( wxWindow *parent, int id, const wxPoint pos = wxDefaultPosition, const wxSize size = wxDefaultSize )
		: MBaseSpin( parent, id, pos, size )
	{
		token = '-';
	}

protected:

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

	MDegSpin( wxWindow *parent, int id, const wxPoint pos = wxDefaultPosition, const wxSize size = wxDefaultSize )
		: MBaseSpin( parent, id, pos, size )
	{
		token = ':';
	}

protected:

	virtual void add( const double &value );
};

#endif


