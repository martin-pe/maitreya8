/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AboutDialog.h
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
