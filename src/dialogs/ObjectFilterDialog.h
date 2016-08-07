/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ObjectFilterDialog.h
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
    wxStaticBox* sizer_right_staticbox;
    wxStaticBox* sizer_list_staticbox;
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
