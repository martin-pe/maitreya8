/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AboutDialog.h
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


#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

// begin wxGlade: dependencies
// end wxGlade

#include <wx/dialog.h>

class TextWidget;

class wxButton;
class wxNotebook;
class wxPanel;
class wxStaticBox;
class wxStaticBitmap;
class wxStaticLine;
class wxStaticText;
class wxTextCtrl;
class wxTimer;
class wxTimerEvent;

/*************************************************//**
*
*  \brief animated logo inside About dialog
*
******************************************************/
class LogoWidget : public wxWindow
{
	DECLARE_CLASS( LogoWidget )

public:

	LogoWidget( wxWindow *parent, int id );
	~LogoWidget();

	void doAnimation();

protected:

	void OnPaint(wxPaintEvent& event);
	void OnTimer( wxTimerEvent& );

	wxImage *image;
	int xzoom, yzoom, ani_mode, ani_direction;
	wxTimer *timer;
};

/*************************************************//**
*
* \brief About dialog (shown with F1)
*
* About dialog holding application information, license and credits.
**
******************************************************/
class AboutDialog: public wxDialog
{
	DECLARE_CLASS( AboutDialog )

public:
    // begin wxGlade: AboutDialog::ids
    // end wxGlade

	AboutDialog(wxWindow* parent );

private:
    // begin wxGlade: AboutDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade
	//virtual bool ProcessEvent(wxEvent& );

protected:
    // begin wxGlade: AboutDialog::attributes
    wxStaticBox* sizer_software_staticbox;
    wxStaticBox* sizer_versions_staticbox;
    wxStaticBox* sizer_author_staticbox;
    LogoWidget* widget;
    wxPanel* panel_logo;
    wxStaticText* label_author;
    wxStaticText* label_url;
    wxStaticText* label_dedicate;
    wxStaticText* label_title_version;
    wxStaticText* label_version;
    wxStaticText* label_title_wx;
    wxStaticText* label_version_wx;
    wxStaticText* label_title_swe;
    wxStaticText* label_version_swe;
    wxStaticText* label_title_wxsqlite;
    wxStaticText* label_version_wxsqlite3;
    wxStaticText* label_title_wxpdfdoc;
    wxStaticText* label_version_wxpdfdoc;
    wxStaticText* label_title_builddate;
    wxStaticText* label_builddate;
    wxStaticText* label_title_compiler;
    wxStaticText* label_compiler;
    wxStaticText* label_title_unicode;
    wxStaticText* label_unicode;
    wxStaticText* label_title_os;
    wxStaticText* label_os;
    wxPanel* pane_about;
    wxTextCtrl* text_license;
    wxPanel* pane_license;
    wxNotebook* notebook;
    wxButton* button_ok;
    // end wxGlade
};


#endif // ABOUTDIALOG_H
