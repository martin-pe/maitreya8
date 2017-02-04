/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/PenDialog.cpp
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

#include "PenDialog.h"

#include "ColorConfig.h"
#include "Conf.h"
#include "guibase.h"
#include "mathbase.h"
#include "Painter.h"

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/panel.h>
#include <wx/pen.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

extern Config *config;

// begin wxGlade: ::extracode
// end wxGlade

enum { CD_PICKER_FGCOLOR = wxID_HIGHEST + 1, CD_SPIN_FGWIDTH, CD_CHOICE_FGSTYLE, CD_PREVIEW_PANEL };

IMPLEMENT_CLASS( PenDialog, wxDialog )

/*************************************************//**
*
* 
*
******************************************************/
class PenDialogPreviewWidget: public wxPanel
{
	DECLARE_CLASS( PenDialogPreviewWidget )

public:

	/*****************************************************
	**
	**   PenDialogPreviewWidget   ---   Constructor
	**
	******************************************************/
	PenDialogPreviewWidget( wxWindow* parent, wxWindowID id, wxPen *pen, MBrush *b )
		: wxPanel( parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE ),
		pen( pen ),
		brush( b )
	{
		Connect( wxEVT_PAINT, wxPaintEventHandler( PenDialogPreviewWidget::OnPaint ));
	}

private:

	/*****************************************************
	**
	**   PenDialogPreviewWidget   ---   OnPaint
	**
	******************************************************/
	void OnPaint( wxPaintEvent& )
	{
		wxCoord h, w;
		wxPaintDC context( this );
		PrepareDC( context );
		const int border = 10;

		GetSize( &w, &h );

		Painter *painter = new DcPainter( &context );

		// background
		painter->setPenColor( config->colors->bgColor );
		if ( brush ) painter->setBrush( *brush );
		else painter->setDefaultBrush();
		painter->drawRectangle( 0, 0, w, h );

		painter->setPen( *pen );
		painter->drawRectangle( MRect( border, border, w - 2 * border, h - 2 * border ));

		delete painter;
	}

	wxPen *pen;
	MBrush *brush;

};

IMPLEMENT_CLASS( PenDialogPreviewWidget, wxPanel )

/*****************************************************
**
**   PenDialog   ---   Constructor
**
******************************************************/
PenDialog::PenDialog( wxWindow* parent, const wxPen &p, MBrush *brush )
	: wxDialog(parent, -1, wxT( "--"  ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE),
	bgbrush( brush )
{
	pen = new wxPen( p );

    // begin wxGlade: PenDialog::PenDialog
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    label_fgcolor = new wxStaticText(this, wxID_ANY, _("Color"));
    button_fgcolor = new wxColourPickerCtrl(this, CD_PICKER_FGCOLOR);
    label_fgwidth = new wxStaticText(this, wxID_ANY, _("Width"));
    spin_fgwidth = new wxSpinCtrl(this, CD_SPIN_FGWIDTH, wxT("1"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 30);
    label_fgstyle = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_fgstyle_choices[] = {
        _("Solid"),
        _("Dot"),
        _("Long Dash"),
        _("Short Dash"),
        _("Dot Dash"),
    };
    choice_fgstyle = new wxChoice(this, CD_CHOICE_FGSTYLE, wxDefaultPosition, wxDefaultSize, 5, choice_fgstyle_choices);
    preview = new PenDialogPreviewWidget(panel_preview, wxID_ANY, pen, bgbrush);
    static_line = new wxStaticLine(this, wxID_ANY);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	button_fgcolor->SetColour( pen->GetColour());
	spin_fgwidth->SetValue( pen->GetWidth());
	choice_fgstyle->SetSelection( convertWxPenStyle2Int( pen->GetStyle()));
	SetFocus();

	Connect( CD_SPIN_FGWIDTH, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( PenDialog::OnSpin ));
	Connect( CD_PICKER_FGCOLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( PenDialog::OnColorPicker ));
	Connect( CD_CHOICE_FGSTYLE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( PenDialog::OnChoice ));
}

/*****************************************************
**
**   PenDialog   ---   Destructor
**
******************************************************/
PenDialog::~PenDialog()
{
	delete pen;
}

/*****************************************************
**
**   PenDialog   ---   updateAttributes
**
******************************************************/
void PenDialog::updateAttributes()
{
	printf( "UPDATE ATTRI\n" );
	const wxColour color = button_fgcolor->GetColour();
	const int width = spin_fgwidth->GetValue();
	const int style = convertInt2WxPenStyle( choice_fgstyle->GetSelection());
	*pen = wxPen( color, width, style );
	preview->Refresh();
}

/*****************************************************
**
**   PenDialog   ---   set_properties
**
******************************************************/
void PenDialog::set_properties()
{
	button_fgcolor->SetColour( pen->GetColour());
	spin_fgwidth->SetValue( pen->GetWidth());
	choice_fgstyle->SetSelection( convertWxPenStyle2Int( pen->GetStyle()));

    // begin wxGlade: PenDialog::set_properties
    SetTitle(_("Select Pen"));
    choice_fgstyle->SetSelection(0);
    preview->SetMinSize(wxSize(-1, 100));
    button_ok->SetDefault();
    // end wxGlade
}

/*****************************************************
**
**   PenDialog   ---   do_layout
**
******************************************************/
void PenDialog::do_layout()
{
    // begin wxGlade: PenDialog::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_bottom = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_fg = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Attributes")), wxVERTICAL);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Pen Preview")), wxHORIZONTAL);
    wxBoxSizer* sizer_previewpanel = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* grid_fg = new wxFlexGridSizer(3, 2, 3, 3);
    grid_fg->Add(label_fgcolor, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_fg->Add(button_fgcolor, 1, wxALL, 3);
    grid_fg->Add(label_fgwidth, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_fg->Add(spin_fgwidth, 0, wxALL, 3);
    grid_fg->Add(label_fgstyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_fg->Add(choice_fgstyle, 0, wxALL, 3);
    grid_fg->AddGrowableCol(1);
    sizer_fg->Add(grid_fg, 0, wxALIGN_RIGHT, 0);
    sizer_previewpanel->Add(preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(sizer_previewpanel);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_fg->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_fg, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(static_line, 0, wxEXPAND, 0);
    sizer_bottom->Add(button_ok, 0, wxALL, 3);
    sizer_bottom->Add(button_cancel, 0, wxALL, 3);
    sizer_main->Add(sizer_bottom, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

