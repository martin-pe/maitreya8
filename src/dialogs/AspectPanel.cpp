/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/AspectPanel.cpp
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

#include "AspectPanel.h"

#include <wx/button.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/stattext.h>

#include "ColorConfig.h"
#include "Conf.h"
#include "Painter.h"
#include "PenDialog.h"
#include "SymbolProvider.h"

extern Config *config;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( COMMAND_SELECTED_ASPECT_CHANGED, wxID_HIGHEST + 9002 )
DECLARE_EVENT_TYPE( COMMAND_ASPECT_DOUBLECLICK, wxID_HIGHEST + 9003 )
END_DECLARE_EVENT_TYPES()

DEFINE_EVENT_TYPE( COMMAND_SELECTED_ASPECT_CHANGED )
DEFINE_EVENT_TYPE( COMMAND_ASPECT_DOUBLECLICK )

enum { CD_PREVIEW_PANEL = wxID_HIGHEST + 1,
	CD_STYLE_CHOICE, 
	CD_CHECK_ACTIVE,
	CD_BUTTON_PEN,
	CD_SPIN_ORBIS
};

IMPLEMENT_CLASS( AspectPanel, ConfigPanel )

/*************************************************//**
*
* \brief Panel for aspect preview
*
******************************************************/
class AspectPreviewPanel: public wxPanel
{
	DECLARE_CLASS( AspectPreviewPanel )

public:

	/*****************************************************
	**
	**   AspectPreviewPanel   ---   Constructor
	**
	******************************************************/
	AspectPreviewPanel( wxWindow* parent, wxWindowID id, vector<AspectConfig> &a )
			: wxPanel( parent, id, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_SUNKEN | wxFULL_REPAINT_ON_RESIZE ),
			configs( a )
	{
		showSymbols = true;
		selected = 0;

		Connect( wxEVT_PAINT, wxPaintEventHandler( AspectPreviewPanel::OnPaint ));
		Connect( wxEVT_LEFT_DOWN, wxMouseEventHandler( AspectPreviewPanel::OnMouseLeftDown ));
		Connect( wxEVT_LEFT_DCLICK, wxMouseEventHandler( AspectPreviewPanel::OnMouseLeftDoubleClick ));
	}

	void setSelected( const int &sel ) { selected = sel; }

private:

	/*****************************************************
	**
	**   AspectPreviewPanel   ---   drawSingleAspect
	**
	******************************************************/
	void drawSingleAspect( Painter *painter, const int &aspectindex, const int &xleft, const int &xright, const int &y )
	{
		wxChar c;
		const int xh = 15;
		const int w = xright - xleft;

		painter->setPen( configs[aspectindex].pen );
		painter->setTextColor( configs[aspectindex].pen.GetColour() );

		if ( showSymbols )
		{
			painter->drawLine( xleft, y, xleft + w/2 - xh, y );
			painter->drawLine( xleft + w/2 + xh, y, xright, y );
			c = SymbolProvider().getAspectCode( (ASPECT_TYPE)aspectindex );

			if ( ! c )
			{
				painter->setGraphicFont();
				painter->drawTextFormatted( MRect( xleft + w/2 - xh, y - xh, 2 * xh, 2 * xh ),
					AspectExpert::getAspectShortDescription( aspectindex ), Align::Center );
			}
			else
			{
				painter->setSymbolFont();
				painter->drawTextFormatted( MRect( xleft + w/2 - xh, y - xh, 2 * xh, 2 * xh ), c, Align::Center );
			}
		}
		else painter->drawLine( xleft, y, xright, y );
	}


	/*****************************************************
	**
	**   AspectPreviewPanel   ---   OnPaint
	**
	******************************************************/
	void OnPaint( wxPaintEvent& )
	{
		wxCoord w, h;
		const int xborder = 5;
		wxString s;
		const int selborder = 2;
		const int borderwidth = 1;

		wxPaintDC dc( this );
		PrepareDC( dc );
		DcPainter painter( &dc );

		GetSize( &w, &h  );

		painter.setDefaultBrush();
		painter.setPen( wxPen( config->colors->fgColor ));
		painter.drawRectangle( 0, 0, w, h );

		const int imed = ( MAX_ASPECT_PANEL_ITEMS + 1 ) / 2;
		const int steplen = h / imed;
		painter.drawLine( w / 2 , 0, w / 2, h );

		const int col2 = ( selected >= imed );
		const int sely = selected - col2 * imed;
		painter.setTransparentBrush();

		painter.setPen( wxPen( *wxBLUE, borderwidth, wxDOT ));

		painter.drawRectangle( selborder + col2 * w/2, selborder + sely * steplen, w/2 - 2 * selborder, steplen - 2 * selborder);

		for ( int i = 0; i < imed; i++ )
		{
			drawSingleAspect( &painter, i, xborder, w/2 - xborder, steplen / 2 + i * steplen );
		}
		for ( int i = imed; i < MAX_ASPECT_PANEL_ITEMS; i++ )
		{
			drawSingleAspect( &painter, i, w/2 + xborder, w - xborder, steplen / 2 + ( i - imed ) * steplen );
		}
	}

	/**************************************************************
	***
	**   AspectPreviewPanel   ---   getSelectedItemByMousePos
	***
	***************************************************************/
	int getSelectedItemByMousePos( wxMouseEvent &event )
	{
		wxCoord w, h;
		int sel = 0;
		GetSize( &w, &h );
		bool rightside = ( event.m_x > w / 2 );

		const int imed = ( MAX_ASPECT_PANEL_ITEMS + 1 ) / 2;
		const int steplen = h / imed;

		sel = event.m_y / steplen;
		if ( rightside ) sel += imed;

		if ( sel >= MAX_ASPECT_PANEL_ITEMS ) sel = MAX_ASPECT_PANEL_ITEMS - 1;
		if ( sel < 0 ) sel = 0;
		return sel;
	}

	/**************************************************************
	***
	**   AspectPreviewPanel   ---   OnMouseLeftDown
	***
	***************************************************************/
	void OnMouseLeftDown( wxMouseEvent &event )
	{
		selected = getSelectedItemByMousePos( event );

		wxCommandEvent e( COMMAND_SELECTED_ASPECT_CHANGED, GetId() );
		e.SetInt( selected );
		wxPostEvent( GetParent(), e );

		Refresh();
	}

	/**************************************************************
	***
	**   AspectPreviewPanel   ---   OnMouseLeftDoubleClick
	***
	***************************************************************/
	void OnMouseLeftDoubleClick( wxMouseEvent &event )
	{
		selected = getSelectedItemByMousePos( event );

		wxCommandEvent e( COMMAND_ASPECT_DOUBLECLICK, GetId() );
		e.SetInt( selected );
		wxPostEvent( GetParent(), e );

		Refresh();
	}

	vector<AspectConfig> &configs;
	bool showSymbols;
	int selected;
};

IMPLEMENT_CLASS( AspectPreviewPanel, wxPanel )

/*****************************************************
**
**   AspectPanel   ---   Constructor
**
******************************************************/
AspectPanel::AspectPanel( wxWindow* parent ) :  ConfigPanel( parent )
{
	config2model();

    // begin wxGlade: AspectPanel::AspectPanel
    sizer_options_staticbox = new wxStaticBox(this, wxID_ANY, _("Options"));
    sizer_preview_staticbox = new wxStaticBox(this, wxID_ANY, _("Preview and Aspect Selection"));
    sizer_type_staticbox = new wxStaticBox(this, wxID_ANY, _("Aspect Type"));
    const wxString choice_aspect_type_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_aspect_type = new wxChoice(this, CD_STYLE_CHOICE, wxDefaultPosition, wxDefaultSize, 5, choice_aspect_type_choices, 0);
    check_active = new wxCheckBox(this, CD_CHECK_ACTIVE, _("Active"));
    button_pen = new wxButton(this, CD_BUTTON_PEN, _("Pen ..."));
    label_orbis = new wxStaticText(this, wxID_ANY, _("Orbis (deg)"));
    spin_orbis = new wxSpinCtrl(this, CD_SPIN_ORBIS, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 20);
    preview_panel = new AspectPreviewPanel(this, CD_PREVIEW_PANEL, aspects);

    set_properties();
    do_layout();
    // end wxGlade

	choice_aspect_type->Clear();
	for ( int i = 0; i < MAX_ASPECT_PANEL_ITEMS; i++ ) choice_aspect_type->Append( AspectExpert::getAspectName( i ));
	selconfig = 0;
	choice_aspect_type->SetSelection( selconfig );

	preview_panel->Refresh();

	Connect( CD_STYLE_CHOICE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( AspectPanel::OnAspectChoice ));
	Connect( CD_CHECK_ACTIVE, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AspectPanel::OnCheckActive ));
	Connect( CD_SPIN_ORBIS, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( AspectPanel::OnSpinOrbis ));
	Connect( CD_BUTTON_PEN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( AspectPanel::OnButtonPen ));
	Connect( CD_PREVIEW_PANEL, COMMAND_SELECTED_ASPECT_CHANGED, wxCommandEventHandler( AspectPanel::OnSelectedAspectChanged ));
	Connect( CD_PREVIEW_PANEL, COMMAND_ASPECT_DOUBLECLICK, wxCommandEventHandler( AspectPanel::OnAspectDoubleClick ));
}

/*****************************************************
**
**   AspectPanel   ---   Life cycle
**
******************************************************/
AspectPanel::~AspectPanel() {} 
void AspectPanel::config2model() { aspects = config->aspects; }
void AspectPanel::model2config() { config->aspects = aspects; }

/*****************************************************
**
**   AspectPanel   ---   OnCheckActive
**
******************************************************/
void AspectPanel::OnCheckActive( wxCommandEvent &event )
{
	aspects[selconfig].active = event.IsChecked();
	setDirty();
	updateUi();
}

/*****************************************************
**
**   AspectPanel   ---   showPenDialog
**
******************************************************/
void AspectPanel::showPenDialog()
{
	PenDialog dialog( this, aspects[selconfig].pen );
	if ( dialog.ShowModal() == wxID_OK )
	{
		aspects[selconfig].pen  = *dialog.getPen();
		setDirty();
		preview_panel->Refresh();
	}
}

/*****************************************************
**
**   AspectPanel   ---   OnSpinOrbis
**
******************************************************/
void AspectPanel::OnSpinOrbis( wxSpinEvent& )
{
	aspects[selconfig].orbis  = spin_orbis->GetValue();
	setDirty();
}

/*****************************************************
**
**   AspectPanel   ---   OnAspectChoice
**
******************************************************/
void AspectPanel::OnAspectChoice( wxCommandEvent& )
{
	assert( selconfig < MAX_ASPECT_PANEL_ITEMS );
	assert( aspects.size() > selconfig );

	selconfig = choice_aspect_type->GetSelection();
	assert( selconfig < MAX_ASPECT_PANEL_ITEMS );
	assert( aspects.size() > selconfig );

	updateUi();
	preview_panel->setSelected( selconfig);
	preview_panel->Refresh();
}

/**************************************************************
***
**   AspectPanel   ---   OnSelectedAspectChanged
***
***************************************************************/
void AspectPanel::OnSelectedAspectChanged( wxCommandEvent &event )
{
	assert( selconfig < MAX_ASPECT_PANEL_ITEMS );

	selconfig = event.GetInt();
	ASSERT_VALID_ASPECT( (int)selconfig );

	choice_aspect_type->SetSelection( selconfig );
	updateUi();
}

/**************************************************************
***
**   AspectPanel   ---   OnAspectDoubleClick
***
***************************************************************/
void AspectPanel::OnAspectDoubleClick( wxCommandEvent &event )
{
	assert( selconfig < MAX_ASPECT_PANEL_ITEMS );

	selconfig = event.GetInt();
	ASSERT_VALID_ASPECT( (int)selconfig );

	showPenDialog();
	updateUi();
}

/*****************************************************
**
**   AspectPanel   ---   updateUi
**
******************************************************/
void AspectPanel::updateUi()
{
	ASSERT_VALID_ASPECT( (int)selconfig );

	const bool active = aspects[selconfig].active;

	check_active->SetValue( active );
	button_pen->Enable( active );
	label_orbis->Enable( active );
	spin_orbis->Enable( active );
	spin_orbis->SetValue( aspects[selconfig].orbis );
}

/*****************************************************
**
**   AspectPanel   ---   restoreDefaults
**
******************************************************/
void AspectPanel::restoreDefaults()
{
	aspects = AspectConfigLoader::get()->getDefaultAspectConfigList();
	preview_panel->Refresh();
}

/*****************************************************
**
**   AspectPanel   ---   set_properties
**
******************************************************/
void AspectPanel::set_properties()
{
    // begin wxGlade: AspectPanel::set_properties
    choice_aspect_type->SetSelection(0);
    preview_panel->SetMinSize(wxSize(150, 100));
    // end wxGlade
}

/*****************************************************
**
**   AspectPanel   ---   do_layout
**
******************************************************/
void AspectPanel::do_layout()
{
    // begin wxGlade: AspectPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 2, 0, 0);
    wxFlexGridSizer* sizer_styles_right = new wxFlexGridSizer(1, 1, 0, 0);
    sizer_preview_staticbox->Lower();
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxHORIZONTAL);
    wxFlexGridSizer* sizer_left = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_options_staticbox->Lower();
    wxStaticBoxSizer* sizer_options = new wxStaticBoxSizer(sizer_options_staticbox, wxVERTICAL);
    wxGridSizer* grid_options = new wxGridSizer(2, 2, 0, 0);
    sizer_type_staticbox->Lower();
    wxStaticBoxSizer* sizer_type = new wxStaticBoxSizer(sizer_type_staticbox, wxVERTICAL);
    sizer_type->Add(choice_aspect_type, 0, wxALL|wxEXPAND, 3);
    sizer_left->Add(sizer_type, 1, wxEXPAND, 0);
    grid_options->Add(check_active, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options->Add(button_pen, 0, wxALL, 3);
    grid_options->Add(label_orbis, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_options->Add(spin_orbis, 0, wxALL, 3);
    sizer_options->Add(grid_options, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_options, 1, wxEXPAND, 0);
    sizer_main->Add(sizer_left, 1, wxALL|wxEXPAND, 3);
    sizer_preview->Add(preview_panel, 1, wxALL|wxEXPAND, 3);
    sizer_styles_right->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_styles_right->AddGrowableRow(0);
    sizer_styles_right->AddGrowableCol(0);
    sizer_main->Add(sizer_styles_right, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(1);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createAspectPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createAspectPanel( wxWindow *parent )
{
	return new AspectPanel( parent );
}

