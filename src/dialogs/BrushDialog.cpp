/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/BrushDialog.cpp
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

#include "BrushDialog.h"

// begin wxGlade: ::extracode
// end wxGlade

enum { CD_PICKER_BGCOLOR = wxID_HIGHEST + 1, CD_CHOICE_BGSTYLE, CD_CHOICE_BGIMAGE, CD_PREVIEW_PANEL, CD_SLIDER_HUE };

#include "ColorConfig.h"
#include "Conf.h"
#include "guibase.h"
#include "FileConfig.h"
#include "ImageProvider.h"
#include "mathbase.h"
#include "Painter.h"

#include <wx/button.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/dir.h>
#include <wx/image.h>
#include <wx/log.h>
#include <wx/panel.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/slider.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

extern Config *config;

// begin wxGlade: ::extracode
// end wxGlade

IMPLEMENT_CLASS( BrushDialog, wxDialog )

/*************************************************//**
*
* 
*
******************************************************/
class BrushDialogPreviewWidget: public wxPanel
{
	DECLARE_CLASS( BrushDialogPreviewWidget )

public:

	/*****************************************************
	**
	**   BrushDialogPreviewWidget   ---   Constructor
	**
	******************************************************/
	BrushDialogPreviewWidget( wxWindow* parent, wxWindowID id, MBrush &brush )
		: wxPanel( parent, id, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE ),
		brush( brush )
	{
		Connect( wxEVT_PAINT, wxPaintEventHandler( BrushDialogPreviewWidget::OnPaint ));
	}

private:

	/*****************************************************
	**
	**   BrushDialogPreviewWidget   ---   OnPaint
	**
	******************************************************/
	void OnPaint( wxPaintEvent& )
	{
		wxCoord h, w;
		GetSize( &w, &h );

		wxPaintDC dc( this );
		PrepareDC( dc );

		// do not use painter set brush method because cache can be blown up with stipple images and hues (on slider)
		switch( brush.style )
		{
			case wxTRANSPARENT:
				dc.SetBrush( *wxTRANSPARENT_BRUSH );
			break;
			case wxSTIPPLE:
			{
				ImageProvider *ip = ImageProvider::get();
				dc.SetBrush( ip->getFileBasedBitmap( brush.filename, brush.rotateHue, false ));
			}
			break;
			default:
				dc.SetBrush( wxBrush( brush.color, brush.style ));
			break;
		}
		dc.DrawRectangle( 0, 0, w, h );
	}

	const MBrush &brush;

};

IMPLEMENT_CLASS( BrushDialogPreviewWidget, wxPanel )

/*****************************************************
**
**   BrushDialog   ---   Constructor
**
******************************************************/
BrushDialog::BrushDialog( wxWindow* parent, const MBrush &b )
	: wxDialog(parent, -1, wxT( "--"  ), wxDefaultPosition, wxDefaultSize, DEFAULT_DIALOG_STYLE)
{
	brush = b;
	if( brush.style == wxSTIPPLE || brush.style == wxTRANSPARENT )
	{
		brush.color = config->colors->bgColor;
	}
	else if ( ! brush.color.IsOk())
	{
		{
			wxLogError( wxT( "BrushDialog::BrushDialog color not okay %s, using default" ), brush.color.GetAsString().c_str());
			brush.color = config->colors->bgColor;
		}
	}

    // begin wxGlade: BrushDialog::BrushDialog
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    label_bgcolor = new wxStaticText(this, wxID_ANY, _("Color"));
    button_bgcolor = new wxColourPickerCtrl(this, CD_PICKER_BGCOLOR);
    label_bgstyle = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_bgstyle_choices[] = {
        _("Solid"),
        _("Transparent"),
        _("Backward diagonal hatch"),
        _("Cross-diagonal hatch"),
        _("Forward diagonal hatch"),
        _("Cross hatch"),
        _("Horizontal hatch"),
        _("Vertical hatch"),
        _("Stipple"),
    };
    choice_bgstyle = new wxChoice(this, CD_CHOICE_BGSTYLE, wxDefaultPosition, wxDefaultSize, 9, choice_bgstyle_choices);
    label_bgimage = new wxStaticText(this, wxID_ANY, _("Image"));
    const wxString choice_bgimage_choices[] = {};
    choice_bgimage = new wxChoice(this, CD_CHOICE_BGIMAGE, wxDefaultPosition, wxDefaultSize, 0, choice_bgimage_choices);
    label_rotatehue = new wxStaticText(this, wxID_ANY, _("Rotate Hue"));
    slider_rotatehue = new wxSlider(this, CD_SLIDER_HUE, 0, 0, 360, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS);
    preview = new BrushDialogPreviewWidget(panel_preview, wxID_ANY, brush);
    static_line = new wxStaticLine(this, wxID_ANY);
    button_ok = new wxButton(this, wxID_OK, _("OK"));
    button_cancel = new wxButton(this, wxID_CANCEL, _("Cancel"));

    set_properties();
    do_layout();
    // end wxGlade

	choice_bgstyle->SetSelection( convertMBrushStyle2Int( brush.style ));
	slider_rotatehue->SetValue( brush.rotateHue );

	choice_bgimage->Clear();

	wxDir dir;
	wxString picname, ext;
	wxArrayString ar;
	dir.Open( FileConfig::get()->getPicDir() );
	bool nn = dir.GetFirst( &picname );
	while ( nn )
	{
		ext = picname.Right( 4 ).Lower();
		if ( ext == wxT( ".png" ) || ext == wxT( ".jpg" ) || ext == wxT( ".gif" ) || ext == wxT( "jpeg" )) { ar.Add( picname ); }
		nn = dir.GetNext( &picname );
	}
	ar.Sort();

	int sel = 0;
	for( uint i = 0; i < ar.GetCount(); i++ )
	{
		printf( "AR ist %s\n", str2char( ar[i] ));
		choice_bgimage->Append( ar[i] );
		if ( brush.filename == ar[i] ) sel = i;
	}
	choice_bgimage->SetSelection( sel );

	if ( brush.filename.IsEmpty() && ar.size() > 0 )
	{
		brush.filename = ar[0];
	}
	printf( "brush->filename %s\n", str2char( brush.filename ));

	updateUi();
	SetFocus();

	Connect( CD_PICKER_BGCOLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( BrushDialog::OnColorPicker ));
	Connect( CD_CHOICE_BGSTYLE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BrushDialog::OnStyleChoice ));
	Connect( CD_CHOICE_BGIMAGE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( BrushDialog::OnCommand ));
	Connect( CD_SLIDER_HUE, wxEVT_SCROLL_THUMBTRACK, wxCommandEventHandler( BrushDialog::OnCommand ));
	Connect( wxID_OK, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( BrushDialog::OnOK ));
}

/*****************************************************
**
**   BrushDialog   ---   OnColorPicker
**
******************************************************/
void BrushDialog::OnColorPicker( wxColourPickerEvent& )
{
	brush.color = button_bgcolor->GetColour();
	preview->Refresh();
}

/*****************************************************
**
**   BrushDialog   ---   OnStyleChoice
**
******************************************************/
void BrushDialog::OnStyleChoice( wxCommandEvent& )
{
	brush.style = convertInt2MBrushStyle( choice_bgstyle->GetSelection());
	updateUi();
	preview->Refresh();
}

/*****************************************************
**
**   BrushDialog   ---   OnCommand
**
******************************************************/
void BrushDialog::OnCommand( wxCommandEvent& )
{
	brush.filename = choice_bgimage->GetStringSelection();
	brush.rotateHue = slider_rotatehue->GetValue();
	preview->Refresh();
}

/*****************************************************
**
**   BrushDialog   ---   updateUi
**
******************************************************/
void BrushDialog::updateUi()
{
	printf( "BrushDialog::updateUi style is %d\n", brush.style );

	const bool isStipple = ( brush.style == wxSTIPPLE );
	const bool useColor = ( ! isStipple && brush.style != wxTRANSPARENT );

	button_bgcolor->Enable( useColor );
	button_bgcolor->SetColour( useColor ? brush.color : wxSystemSettings::GetColour( wxSYS_COLOUR_BACKGROUND ));

	label_bgimage->Enable( isStipple );
	choice_bgimage->Enable( isStipple );
	label_rotatehue->Enable( isStipple );
	slider_rotatehue->Enable( isStipple );
}

/*****************************************************
**
**   BrushDialog   ---   OnOK
**
******************************************************/
void BrushDialog::OnOK( wxCommandEvent& )
{
	printf( "OK\n" );
	if( brush.style == wxSTIPPLE || brush.style == wxTRANSPARENT )
	{
		brush.color = wxColour();
	}
	if( brush.style != wxSTIPPLE )
	{
		brush.filename.clear();
		brush.rotateHue = 0;
	}
	EndModal( wxID_OK );
}

/*****************************************************
**
**   BrushDialog   ---   set_properties
**
******************************************************/
void BrushDialog::set_properties()
{
    // begin wxGlade: BrushDialog::set_properties
    SetTitle(_("Select Brush"));
    choice_bgstyle->SetToolTip(_("Background images can be used with option 'Stipple'"));
    choice_bgstyle->SetSelection(0);
    button_ok->SetDefault();
    // end wxGlade
}

/*****************************************************
**
**   BrushDialog   ---   do_layout
**
******************************************************/
void BrushDialog::do_layout()
{
    // begin wxGlade: BrushDialog::do_layout
    wxBoxSizer* sizer_main = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* sizer_bottom = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer* sizer_bg = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Attributes")), wxVERTICAL);
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, _("Brush Preview")), wxHORIZONTAL);
    wxBoxSizer* sizer_previewpanel = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* grid_bg = new wxFlexGridSizer(4, 2, 3, 3);
    grid_bg->Add(label_bgcolor, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_bg->Add(button_bgcolor, 1, wxALL, 3);
    grid_bg->Add(label_bgstyle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_bg->Add(choice_bgstyle, 0, wxALL, 3);
    grid_bg->Add(label_bgimage, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_bg->Add(choice_bgimage, 0, wxALL|wxEXPAND, 3);
    grid_bg->Add(label_rotatehue, 0, wxALIGN_CENTER_VERTICAL|wxALL, 3);
    grid_bg->Add(slider_rotatehue, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 3);
    grid_bg->AddGrowableCol(1);
    sizer_bg->Add(grid_bg, 1, wxALIGN_RIGHT, 0);
    sizer_previewpanel->Add(preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(sizer_previewpanel);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_bg->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_bg, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(static_line, 0, wxEXPAND, 0);
    sizer_bottom->Add(button_ok, 0, wxALL, 3);
    sizer_bottom->Add(button_cancel, 0, wxALL, 3);
    sizer_main->Add(sizer_bottom, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    Layout();
    // end wxGlade
}

