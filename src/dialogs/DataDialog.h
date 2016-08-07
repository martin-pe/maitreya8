/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/DataDialog.h
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

#ifndef DATADIALOG_H
#define DATADIALOG_H

#include <wx/dialog.h>

// begin wxGlade: dependencies
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade

class Document;
class MDateSpin;
class MDegSpin;

class wxButton;
class wxEvent;
class wxCheckBox;
class wxChoice;
class wxPanel;
class wxStaticBox;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;

struct DataDialogModel;

/*************************************************//**
*
* \brief dialog for birth data
*
******************************************************/
class DataDialog: public wxDialog
{
	DECLARE_CLASS( DataDialog )

public:
    // begin wxGlade: DataDialog::ids
    // end wxGlade

	DataDialog(wxWindow* parent, Document* );
	~DataDialog();

private:
    // begin wxGlade: DataDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

	Document *doc;
	DataDialogModel *model;

	void model2doc();
	void doc2model();
	void model2gui();
	void gui2model();

	void updateDateDetailFields( const bool isvalid = true );
	bool doApply();

	void OnSetCurrentDate( wxCommandEvent& );
	void OnDefaultLocation( wxCommandEvent& );
	void OnSearchLocation( wxCommandEvent& );
	void OnOK( wxCommandEvent& );
	void OnCancel( wxCommandEvent& );
	void OnApply( wxCommandEvent& ) { doApply(); }
	void OnLocalTime( wxCommandEvent& );
	void OnInputControlChanged( wxCommandEvent& );

protected:
    // begin wxGlade: DataDialog::attributes
    wxStaticBox* sizer_remark_staticbox;
    wxStaticBox* sizer_tz_staticbox;
    wxStaticBox* sizer_location_staticbox;
    wxStaticBox* sizer_date_staticbox;
    wxStaticBox* sizer_name_staticbox;
    wxTextCtrl* text_name;
    wxStaticText* label_date;
    MDateSpin* text_date;
    wxStaticText* label_datefmt;
    wxStaticText* label_time;
    MDegSpin* text_time;
    wxStaticText* label_timefmt;
    wxStaticText* label_date_formatted;
    wxStaticText* label_calendar;
    wxPanel* panel_date_formatted;
    wxButton* button_now;
    wxStaticText* label_locname;
    wxTextCtrl* text_locname;
    wxStaticText* label_long;
    MDegSpin* text_longitude;
    wxStaticText* label_long_fmt;
    wxChoice* choice_ew;
    wxStaticText* label_lat;
    MDegSpin* text_latitude;
    wxStaticText* label_lat_fmt;
    wxChoice* choice_ns;
    wxButton* button_defloc;
    wxButton* button_searchloc;
    wxStaticText* label_tz;
    MDegSpin* text_tz;
    wxStaticText* label_tzfmt;
    wxChoice* choice_tzew;
    wxStaticText* label_dst;
    MDegSpin* text_dst;
    wxStaticText* label_dstfmt;
    wxButton* button_localtime;
    wxTextCtrl* text_remark;
    wxButton* button_ok;
    wxButton* button_apply;
    wxButton* button_cancel;
    // end wxGlade
};


#endif // DATADIALOG_H
