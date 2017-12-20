/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AboutDialog.cpp
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

#include "AboutDialog.h"

#include "Calculator.h"
#include "config.h"
#include "maitreya.h"
#include "FileConfig.h"
#include "FontProvider.h"
#include "guibase.h"
#include "ImageProvider.h"

#include <wx/button.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/filename.h>
#include <wx/image.h>
#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>
#include <wx/timer.h>


enum { ABOUT_TIMER = wxID_HIGHEST + 1 };

IMPLEMENT_CLASS( LogoWidget, wxWindow )
IMPLEMENT_CLASS( AboutDialog, wxDialog )

#define PIC_X 546
#define PIC_Y 205

/*****************************************************
**
**   LogoWidget   ---   Constructor
**
******************************************************/
LogoWidget::LogoWidget( wxWindow *parent, int id )
		: wxWindow( parent, id )
{
	wxString s;
	s = FileConfig::get()->getPicDir();
	if ( s.Last() != wxFileName::GetPathSeparator()) s << wxFileName::GetPathSeparator();

	image = new wxImage( wxBitmap( s + LOGO_FILE, wxBITMAP_TYPE_JPEG ).ConvertToImage());

	xzoom = yzoom = 100;
	ani_mode = 0;
	ani_direction = 0;

	timer = new wxTimer( this, ABOUT_TIMER );
	timer->Start( 6000 );

	Connect( wxEVT_PAINT, wxPaintEventHandler( LogoWidget::OnPaint ));
	Connect( ABOUT_TIMER, wxEVT_TIMER, wxTimerEventHandler( LogoWidget::OnTimer ));
}

/*****************************************************
**
**   LogoWidget   ---   Destructor
**
******************************************************/
LogoWidget::~LogoWidget()
{
	timer->Stop();
	delete timer;
	//delete bitmap;
	delete image;
}

/*****************************************************
**
**   LogoWidget   ---   doAnimation
**
******************************************************/
void LogoWidget::doAnimation()
{
	if ( ! timer->IsRunning() ) timer->Start( 100 );
	else timer->Stop();
}

/*****************************************************
**
**   LogoWidget   ---   OnTimer
**
******************************************************/
void LogoWidget::OnTimer( wxTimerEvent& )
{
	if ( xzoom == 100 && yzoom == 100 ) timer->Start( 100 );
	if ( ani_direction == 0 )
	{
		if ( ani_mode == 0 ) xzoom -= 10;
		else yzoom -= 10;
		if ( xzoom <= -100 || yzoom <= -100 ) ani_direction = 1;
	}
	else
	{
		if ( ani_mode == 0 ) xzoom += 10;
		else yzoom += 10;
		if ( xzoom >= 100 && yzoom >= 100 )
		{
			ani_direction = 0;
			ani_mode = ! ani_mode;
			timer->Start( 12000 );
		}
	}
	Refresh( false );
}

/*****************************************************
**
**   LogoWidget   ---   OnPaint
**
******************************************************/
void LogoWidget::OnPaint(wxPaintEvent& )
{
	double axzoom = ( xzoom > 0 ? xzoom : -xzoom );
	double ayzoom = ( yzoom > 0 ? yzoom : -yzoom );
	int x = (int)(PIC_X * axzoom / 100);
	int xborder = (int)(( PIC_X - x ) / 2);
	int y = (int)(PIC_Y * ayzoom / 100);
	int yborder = (int)(( PIC_Y - y ) / 2);
	bool mirror = ( xzoom == 100 ? false : true );

	wxPaintDC dc(this);
	wxColour bgcolor = wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND );
	dc.SetBrush( wxBrush( GetBackgroundColour(), wxSOLID ));
	dc.SetPen( wxPen( bgcolor, 1, wxSOLID ));
	dc.DrawRectangle( -1, -1, xborder, PIC_Y+2 ); // left
	dc.DrawRectangle( PIC_X - xborder, -1, PIC_X/2+2, PIC_Y+2 ); // right
	dc.DrawRectangle( -1, -1, PIC_X+2, yborder ); // upper
	dc.DrawRectangle( -1, PIC_Y - yborder, PIC_X+2, PIC_Y/2+2 ); // lower

	static double angle = 0;
	angle += .02;
	if ( xzoom == 100 && yzoom == 100 )
	{
		dc.DrawBitmap( wxBitmap( *image ), xborder, yborder, true );
	}
	else if ( xzoom >= 0 && yzoom >= 0 )
	{
		if ( x > 0 && y > 0 )
		{
			dc.DrawBitmap( wxBitmap( image->Scale( x, y )), xborder, yborder, true );
		}
	}
	else
	{
		dc.DrawBitmap( wxBitmap( image->Mirror( mirror ).Scale( x, y )), xborder, yborder, true );
	}
}

/*****************************************************
**
**   AboutDialog   ---   Constructor
**
******************************************************/
AboutDialog::AboutDialog(wxWindow* parent )
		: wxDialog( parent, -1, wxT( "" ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE )
{
    // begin wxGlade: AboutDialog::AboutDialog
    notebook = new wxNotebook(this, wxID_ANY);
    pane_license = new wxPanel(notebook, wxID_ANY);
    pane_about = new wxPanel(notebook, wxID_ANY);
    panel_logo = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    widget = new LogoWidget(panel_logo, wxID_ANY);
    label_author = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_url = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_dedicate = new wxStaticText(pane_about, wxID_ANY, _("Dedicated to Paramapadma Dhirananda"));
    label_title_version = new wxStaticText(pane_about, wxID_ANY, _("Maitreya"));
    label_version = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_wx = new wxStaticText(pane_about, wxID_ANY, _("wxWidgets"));
    label_version_wx = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_swe = new wxStaticText(pane_about, wxID_ANY, _("Swiss Ephemeris"));
    label_version_swe = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_wxsqlite = new wxStaticText(pane_about, wxID_ANY, _("wxsqlite"));
    label_version_wxsqlite3 = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_wxpdfdoc = new wxStaticText(pane_about, wxID_ANY, _("wxpdfdoc"));
    label_version_wxpdfdoc = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_builddate = new wxStaticText(pane_about, wxID_ANY, _("Build Date"));
    label_builddate = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_compiler = new wxStaticText(pane_about, wxID_ANY, _("Compiled by"));
    label_compiler = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_unicode = new wxStaticText(pane_about, wxID_ANY, _("Unicode Support"));
    label_unicode = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    label_title_os = new wxStaticText(pane_about, wxID_ANY, _("Currently running on"));
    label_os = new wxStaticText(pane_about, wxID_ANY, _("dummy"));
    text_license = new wxTextCtrl(pane_license, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP);
    button_ok = new wxButton(this, wxID_OK, _("OK"));

    set_properties();
    do_layout();
    // end wxGlade

	wxString s, s1;
	wxString licensefile = FileConfig::get()->getLicenseFile();
	if ( ! wxFile::Exists( licensefile ))
	{
		printf( "ERROR: license file %s not found\n", str2char( licensefile ));
		s = wxT( "GPL v2" );
	}
	else
	{
		wxTextFile file( licensefile );
		file.Open();

		for ( s1 = file.GetFirstLine(); ! file.Eof(); s1 = file.GetNextLine() )
		{
			s << s1 << Endl;
		}
	}
	text_license->SetValue( s );

	SetIcon( ImageProvider::get()->getIcon( BM_ABOUT ));
	button_ok->SetFocus();
}

/*****************************************************
**
**   AboutDialog   ---   ProcessEvent
**
******************************************************/
/*
bool AboutDialog::ProcessEvent(wxEvent& event)
{
	if ( event.GetClassInfo()->IsKindOf( CLASSINFO( wxKeyEvent )))
	{
		if ( ((wxKeyEvent*)&event)->GetKeyCode() == WXK_ESCAPE )
		{
			EndModal( wxID_CANCEL );
			return true;
		}
		else return wxDialog::ProcessEvent( event );
	}
	else return wxDialog::ProcessEvent( event );
}
*/

/*****************************************************
**
**   AboutDialog   ---   set_properties
**
******************************************************/
void AboutDialog::set_properties()
{
	wxString s;

	label_author->SetLabel( AUTHOR );
	label_url->SetLabel( SARAVALI_URL );

	// Version of the software
	label_version->SetLabel( wxConvertMB2WX( PACKAGE_VERSION ) );

	s << wxString::Format( wxT( "%d.%d.%d/" ), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER );

	text_license->SetFont( *FontProvider::get()->getFont( FONT_PLAIN_DEFAULT ));

#ifdef __WXOSX_CARBON__
	s << wxT( "Carbon" );
#elif __WXOSX_COCOA__
	s << wxT( "Cocoa" );
#elif defined  __MINGW32__
	s << wxT( "MinGW32" );
#elif defined  __VISUALC__
	s << wxT( "VisualC" );
#elif defined __WXMOTIF__ 
	s << wxT( "Motif" );
//#elif defined __WXGTK20__
	//s << wxT( "GTK2" );
#elif defined __WXGTK__
	s << wxT( "GTK" );
#elif defined __WXX11__
	s << wxT( "X11" );
#else
	s << wxT( "Unknown" );
#endif

	label_version_wx->SetLabel( s );
	//label_version_wx->SetLabel( wxString::Format( wxT( "%d.%d.%d" ), wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER ));

	s.Clear();
	if ( USE_WXPDFDOC_BUILTIN == 1 )
	{
		s << WXPDFDOC_BUILTIN_VERSION << wxT( "/builtin" );
	}
	else
	{
		s << wxConvertMB2WX( WXPDFDOC_SYSTEM_VERSION ) << wxT( "/system" );
	}
	label_version_wxpdfdoc->SetLabel( s );

	s.Clear();
	if ( USE_WXSQLITE_BUILTIN == 1 )
	{
		s << WXSQLITE3_BUILTIN_VERSION << wxT( "/builtin" );
	}
	else
	{
		s << wxConvertMB2WX( WXSQLITE_SYSTEM_VERSION ) << wxT( "/system" );
	}
	label_version_wxsqlite3->SetLabel( s );

	s.Clear();
	s << CalculatorFactory().getCalculator()->getVersion();
	if ( USE_SWE_BUILTIN == 1 )
	{
		s << wxT( "/builtin" );
	}
	else
	{
		s << wxT( "/system" );
	}
	label_version_swe->SetLabel( s );

	// Build date
	label_builddate->SetLabel( wxT( __DATE__ ) );

	s.Clear();
	// Compiler
#ifdef __GNUC__
	s << wxT( "GNUC " ) << wxConvertMB2WX( __VERSION__ );
#elif defined(__VISUALC__)
	s << wxT( "Visual C " ) << wxString::Format( wxT( "%d" ), _MSC_VER );
#else
	s << wxT( "Unknown Compiler" );
#endif
	label_compiler->SetLabel( s );

#if wxUSE_UNICODE
	label_unicode->SetLabel( wxT( "yes" ));
#else
	label_unicode->SetLabel( wxT( "no" ));
#endif

	label_os->SetLabel( wxGetOsDescription());

    // begin wxGlade: AboutDialog::set_properties
    SetTitle(_("About"));
    widget->SetMinSize(wxSize(546,205));
    button_ok->SetDefault();
    // end wxGlade
}

/*****************************************************
**
**   AboutDialog   ---   do_layout
**
******************************************************/
void AboutDialog::do_layout()
{
    // begin wxGlade: AboutDialog::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(3, 1, 0, 0);
    wxBoxSizer* sizer_license = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* sizer_about = new wxFlexGridSizer(3, 1, 0, 0);
    wxStaticBoxSizer* sizer_software = new wxStaticBoxSizer(new wxStaticBox(pane_about, wxID_ANY, _("Software")), wxVERTICAL);
    wxFlexGridSizer* grid_software = new wxFlexGridSizer(4, 2, 0, 5);
    wxStaticBoxSizer* sizer_versions = new wxStaticBoxSizer(new wxStaticBox(pane_about, wxID_ANY, _("Versions")), wxVERTICAL);
    wxFlexGridSizer* grid_versions = new wxFlexGridSizer(5, 2, 0, 5);
    wxStaticBoxSizer* sizer_author = new wxStaticBoxSizer(new wxStaticBox(pane_about, wxID_ANY, _("Author")), wxVERTICAL);
    wxBoxSizer* sizer_logo = new wxBoxSizer(wxHORIZONTAL);
    sizer_logo->Add(widget, 1, wxALL|wxEXPAND, 3);
    panel_logo->SetSizer(sizer_logo);
    sizer_main->Add(panel_logo, 1, 0, 3);
    sizer_author->Add(label_author, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    sizer_author->Add(label_url, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    sizer_author->Add(label_dedicate, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    sizer_about->Add(sizer_author, 1, wxEXPAND, 3);
    grid_versions->Add(label_title_version, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_versions->Add(label_version, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_versions->Add(label_title_wx, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_versions->Add(label_version_wx, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_versions->Add(label_title_swe, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_versions->Add(label_version_swe, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_versions->Add(label_title_wxsqlite, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_versions->Add(label_version_wxsqlite3, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_versions->Add(label_title_wxpdfdoc, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_versions->Add(label_version_wxpdfdoc, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_versions->AddGrowableCol(1);
    sizer_versions->Add(grid_versions, 1, wxALL|wxEXPAND, 3);
    sizer_about->Add(sizer_versions, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_title_builddate, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_builddate, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_title_compiler, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_compiler, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_title_unicode, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_unicode, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_title_os, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->Add(label_os, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_software->AddGrowableCol(1);
    sizer_software->Add(grid_software, 1, wxALL|wxEXPAND, 3);
    sizer_about->Add(sizer_software, 1, wxALL|wxEXPAND, 3);
    pane_about->SetSizer(sizer_about);
    sizer_about->AddGrowableCol(0);
    sizer_license->Add(text_license, 1, wxALL|wxEXPAND, 3);
    pane_license->SetSizer(sizer_license);
    notebook->AddPage(pane_about, _("About"));
    notebook->AddPage(pane_license, _("License"));
    sizer_main->Add(notebook, 1, wxEXPAND, 0);
    sizer_main->Add(button_ok, 0, wxALIGN_CENTER, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(1);
    sizer_main->AddGrowableCol(0);
    Layout();
    // end wxGlade
}

