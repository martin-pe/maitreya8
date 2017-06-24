/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ObjectFilterDialog.h
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

#ifndef OBJECTFILTERDIALOG_H
#define OBJECTFILTERDIALOG_H

#include <wx/dialog.h>

#ifndef APP_CATALOG
#define APP_CATALOG "app"  // replace with the appropriate catalog name
#endif

// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

#include "maitreya.h"

class wxButton;
class wxCheckListBox;
class wxStaticBox;

/*************************************************//**
*
* 
*
******************************************************/
class ObjectFilterDialog: public wxDialog
{
public:
    // begin wxGlade: ObjectFilterDialog::ids
    // end wxGlade

	ObjectFilterDialog(wxWindow* parent, const ObjectArray&, const ObjectFilter& );
	~ObjectFilterDialog();

	ObjectFilter getFilter() const { return filter; }

private:
    // begin wxGlade: ObjectFilterDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:

	void OnAll( wxCommandEvent& );
	void OnFromMars( wxCommandEvent& );
	void OnUptoMars( wxCommandEvent& );
	void OnNone( wxCommandEvent& );
	void OnOK( wxCommandEvent& );

	ObjectArray planets;
	ObjectFilter filter;

    // begin wxGlade: ObjectFilterDialog::attributes
    wxCheckListBox* list;
    wxButton* button_all;
    wxButton* button_upto_mars;
    wxButton* button_from_mars;
    wxButton* button_none;
    wxButton* button_ok;
    wxButton* button_cancel;
    // end wxGlade
}; // wxGlade: end class


#endif // OBJECTFILTERDIALOG_H
