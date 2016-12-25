/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/SheetStylePanel.cpp
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

#include "SheetStylePanel.h"

#include "BrushDialog.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "ColorConfig.h"
#include "guibase.h"
#include "Horoscope.h"
#include "Lang.h"
#include "PenDialog.h"
#include "Painter.h"
#include "Sheet.h"
#include "SheetConfig.h"
#include "StyleManagerDialog.h"
#include "TextHelper.h"
#include "TextWidget.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

extern Config *config;

enum {
	CD_SELECT_STYLE = wxID_HIGHEST + 1,
	CD_MANAGE_STYLES,

	CD_USE_HEADER_COLORS,
	CD_HEADER_TEXTCOLOR,
	CD_HEADER_BGCOLOR,

	CD_USE_CELL_COLORS,
	CD_CELL_TEXTCOLOR,
	CD_CHOICE_CELL_BGMODE,
	CD_ALL_CELL_BGCOLOR,
	CD_ODD_CELL_BGCOLOR,
	CD_EVEN_CELL_BGCOLOR,

	CD_TABLE_DRAW_BORDER
};

IMPLEMENT_CLASS( SheetStylePanel, ConfigPanel )

/*****************************************************
**
**   SheetStylePanel   ---   Constructor
**
******************************************************/
SheetStylePanel::SheetStylePanel( wxWindow* parent  ) : ConfigPanel( parent )
{
	props = new ChartProperties;
	horoscope = new Horoscope;

	SheetConfigLoader *loader = SheetConfigLoader::get();
	v = loader->getConfigsDeep();
	selected = config->view->sheetStyle;
	SheetConfig *sheetcfg = getSheetConfig( selected );

    // begin wxGlade: SheetStylePanel::SheetStylePanel
    panel_preview = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxTAB_TRAVERSAL);
    sizer_table_cells_staticbox = new wxStaticBox(this, wxID_ANY, _("Table Cells"));
    sizer_table_border_staticbox = new wxStaticBox(this, wxID_ANY, _("Borders"));
    sizer_preview_staticbox = new wxStaticBox(this, wxID_ANY, _("Preview"));
    sizer_table_headers_staticbox = new wxStaticBox(this, wxID_ANY, _("Table Header"));
    label_style = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_styles_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_styles = new wxChoice(this, CD_SELECT_STYLE, wxDefaultPosition, wxDefaultSize, 4, choice_styles_choices, 0);
    button_manage_styles = new wxButton(this, CD_MANAGE_STYLES, _("Manage Styles ..."));
    check_use_header_colors = new wxCheckBox(this, CD_USE_HEADER_COLORS, _("Use Header Colors"));
    label_header_textcolor = new wxStaticText(this, wxID_ANY, _("Text Color"));
    button_header_textcolor = new wxColourPickerCtrl(this, CD_HEADER_TEXTCOLOR);
    label_header_bgcolor = new wxStaticText(this, wxID_ANY, _("Background"));
    button_header_bgcolor = new wxColourPickerCtrl(this, CD_HEADER_BGCOLOR);
    check_use_cell_colors = new wxCheckBox(this, CD_USE_CELL_COLORS, _("Use Cell Colors"));
    label_cell_textcolor = new wxStaticText(this, wxID_ANY, _("Text Color"));
    button_cell_textcolor = new wxColourPickerCtrl(this, CD_CELL_TEXTCOLOR);
    label_cell_bgmode = new wxStaticText(this, wxID_ANY, _("Cell Background Mode"));
    const wxString choice_cell_bgmode_choices[] = {
        _("Transparent"),
        _("Constant"),
        _("Alternating")
    };
    choice_cell_bgmode = new wxChoice(this, CD_CHOICE_CELL_BGMODE, wxDefaultPosition, wxDefaultSize, 3, choice_cell_bgmode_choices, 0);
    label_all_row_bgcolor = new wxStaticText(this, wxID_ANY, _("All Rows"));
    button_all_cell_bgcolor = new wxColourPickerCtrl(this, CD_ALL_CELL_BGCOLOR);
    label_even_row_bgcolor = new wxStaticText(this, wxID_ANY, _("Even Rows"));
    button_even_cell_bgcolor = new wxColourPickerCtrl(this, CD_EVEN_CELL_BGCOLOR);
    label_odd_row_bgcolor = new wxStaticText(this, wxID_ANY, _("Odd Rows"));
    button_odd_cell_bgcolor = new wxColourPickerCtrl(this, CD_ODD_CELL_BGCOLOR);
    check_draw_border = new wxCheckBox(this, CD_TABLE_DRAW_BORDER, _("Draw Border"));
    preview = new TextWidget(panel_preview, props, sheetcfg);

    set_properties();
    do_layout();
    // end wxGlade

	Connect( CD_SELECT_STYLE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnSelectStyle ));
	Connect( CD_MANAGE_STYLES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnManageStyles ));

	Connect( wxID_ANY, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( SheetStylePanel::OnTableColorPickerEvent ));
	Connect( CD_CHOICE_CELL_BGMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SheetStylePanel::OnCommand ));

	populateStyleChoice();
	model2gui( selected );
}

/*****************************************************
**
**   SheetStylePanel   ---   Destructor
**
******************************************************/
SheetStylePanel::~SheetStylePanel()
{
	delete props;
	delete horoscope;

	for( vector<SheetConfig*>::iterator iter = v.begin(); iter != v.end(); iter++ )
	{
		delete *iter;
	}
	v.clear();
}

/*****************************************************
**
**   SheetStylePanel   ---   config2model
**
******************************************************/
void SheetStylePanel::config2model()
{
}

/*****************************************************
**
**   SheetStylePanel   ---   model2config
**
******************************************************/
void SheetStylePanel::model2config()
{
	SheetConfigLoader *loader = SheetConfigLoader::get();

	// int selected is required for reentrance on apply()
	selected = choice_styles->GetSelection();

	loader->setConfigs( v );
	loader->saveConfigs();
	config->view->sheetStyle = selected;

	// pointers are now owned by loader -> clear and copy deep again
	v.clear();
	v = loader->getConfigsDeep();
}

/*****************************************************
**
**   SheetStylePanel   ---   model2gui
**
******************************************************/
void SheetStylePanel::model2gui( const uint &sel )
{
	SheetConfig *sheetcfg = getSheetConfig( sel );
	assert( sheetcfg );
	
	check_use_header_colors->SetValue( sheetcfg->tablestyle.useHeaderColors );
	button_header_textcolor->SetColour( sheetcfg->tablestyle.headerTextColor );
	button_header_bgcolor->SetColour( sheetcfg->tablestyle.headerBgColor );

	check_use_cell_colors->SetValue( sheetcfg->tablestyle.useCellColors );
	choice_cell_bgmode->SetSelection( sheetcfg->tablestyle.cellBgMode );
	button_cell_textcolor->SetColour( sheetcfg->tablestyle.cellTextColor );
	button_all_cell_bgcolor->SetColour( sheetcfg->tablestyle.allCellBgColor );
	button_odd_cell_bgcolor->SetColour( sheetcfg->tablestyle.oddCellBgColor );
	button_even_cell_bgcolor->SetColour( sheetcfg->tablestyle.evenCellBgColor );

	check_draw_border->SetValue( sheetcfg->tablestyle.useGrid );

	updateUi();
}

/*****************************************************
**
**   SheetStylePanel   ---   OnSelectStyle
**
******************************************************/
void SheetStylePanel::OnSelectStyle( wxCommandEvent& )
{
	selected = choice_styles->GetSelection();
	model2gui( selected );
	setDirty();
}

/*****************************************************
**
**   SheetStylePanel   ---   populateStyleChoice
**
******************************************************/
void SheetStylePanel::populateStyleChoice()
{
	choice_styles->Clear();
	for( vector<SheetConfig*>::iterator iter = v.begin(); iter != v.end(); iter++ )
	{
		choice_styles->Append( (*iter)->name );
	}
	if ( selected >= 0 && choice_styles->GetCount() > (uint)selected ) choice_styles->SetSelection( selected );
	else
	{
		wxLogError( wxT( "style choice contains %d items but selected item index is %d" ), (int)choice_styles->GetCount(), selected );
		choice_styles->SetSelection( 0 );
		selected = 0;
	}
}

/*****************************************************
**
**   SheetStylePanel   ---   getSheetConfig
**
******************************************************/
SheetConfig *SheetStylePanel::getSheetConfig( const int &sel )
{
	//printf( "SheetStylePanel::getSheetConfig sel %d size %d\n", sel, (int)v.size());
	//assert( sel >= 0 && sel < (int)v.size());
	if( sel < 0 || sel >= (int)v.size())
	{
		wxLogError( wxT( "invalid sheet selection %d, using default" ), sel );
		assert( v.size() > 0 );
		return v[0];
	}
	return v[sel];
}

/*****************************************************
**
**   SheetStylePanel   ---   OnTableColorPickerEvent
**
******************************************************/
void SheetStylePanel::OnTableColorPickerEvent( wxColourPickerEvent& )
{
	updateUi();
	setDirty();
}

/*****************************************************
**
**   SheetStylePanel   ---   updateUi
**
******************************************************/
void SheetStylePanel::updateUi()
{
	SheetConfig *sheetcfg = getSheetConfig( selected );
	assert( sheetcfg );

	// 2nd tab: table
	const bool activateHeaderItems = check_use_header_colors->GetValue();
	sheetcfg->tablestyle.useHeaderColors = activateHeaderItems;
	sheetcfg->tablestyle.headerTextColor = button_header_textcolor->GetColour();
	sheetcfg->tablestyle.headerBgColor = button_header_bgcolor->GetColour();

	label_header_textcolor->Enable( activateHeaderItems );
	button_header_textcolor->Enable( activateHeaderItems );
	label_header_bgcolor->Enable( activateHeaderItems );
	button_header_bgcolor->Enable( activateHeaderItems );

	// 2a: table cell items
	const bool activateCellItems = check_use_cell_colors->GetValue();
	sheetcfg->tablestyle.useCellColors = activateCellItems;
	sheetcfg->tablestyle.cellTextColor = button_cell_textcolor->GetColour();
	sheetcfg->tablestyle.allCellBgColor = button_all_cell_bgcolor->GetColour();
	sheetcfg->tablestyle.oddCellBgColor = button_odd_cell_bgcolor->GetColour();
	sheetcfg->tablestyle.evenCellBgColor = button_even_cell_bgcolor->GetColour();
	sheetcfg->tablestyle.cellBgMode = choice_cell_bgmode->GetSelection();

	label_cell_textcolor->Enable( activateCellItems );
	button_cell_textcolor->Enable( activateCellItems );
	label_cell_bgmode->Enable( activateCellItems );
	choice_cell_bgmode->Enable( activateCellItems );

	const bool enableAllCellBg = ( activateCellItems && choice_cell_bgmode->GetSelection() == 1 );
	label_all_row_bgcolor->Enable( enableAllCellBg );
	button_all_cell_bgcolor->Enable( enableAllCellBg );

	const bool enableOddEvenBg = ( activateCellItems && choice_cell_bgmode->GetSelection() == 2 );
	label_odd_row_bgcolor->Enable( enableOddEvenBg );
	button_odd_cell_bgcolor->Enable( enableOddEvenBg );
	label_even_row_bgcolor->Enable( enableOddEvenBg );
	button_even_cell_bgcolor->Enable( enableOddEvenBg );

	// 3rd tab: table borders
	const bool dogrid = check_draw_border->GetValue();
	sheetcfg->tablestyle.useGrid = dogrid;

	// update preview 
	preview->setSheetConfig( sheetcfg );
	preview->clearSheet();
	Sheet *sheet = preview->getSheet();
	horoscope->setCurrentDate();
	horoscope->update();
	TextHelper( horoscope, props, sheet ).writePlanets();
	preview->OnDataChanged();	
}


/*****************************************************
**
**   SheetStylePanel   ---   OnManageStyles
**
******************************************************/
void SheetStylePanel::OnManageStyles( wxCommandEvent& )
{
	//printf( "SheetStylePanel::OnManageStyles size %d\n", (int)v.size() );

	StyleManagerDialog dialog( this, v, selected );
	dialog.ShowModal();
	selected = dialog.getSelection();

	//setData();
	populateStyleChoice();
	model2gui( selected );

	setDirty();
	//printf( "END SheetStylePanel::OnManageStyles size %d\n", (int)v.size() );
}

/*****************************************************
**
**   SheetStylePanel   ---   OnCommand
**
******************************************************/
void SheetStylePanel::OnCommand( wxCommandEvent& )
{
	updateUi();
	setDirty();
}

/*****************************************************
**
**   SheetStylePanel   ---   restoreDefaults
**
******************************************************/
void SheetStylePanel::restoreDefaults()
{
	const int answer = doMessageBox( this,
		_( "Do you want to replace all entries by the built-in default values?" ),
		wxYES_NO + wxNO_DEFAULT + wxICON_QUESTION );

	if ( answer == wxID_YES )
	{
		for( vector<SheetConfig*>::iterator iter = v.begin(); iter != v.end(); iter++ )
		{
			delete *iter;
		}
		v.clear();

		SheetConfigLoader *loader = SheetConfigLoader::get();
		v = loader->getDefaultConfigsDeep();
		selected = 0;

		//setData();
		populateStyleChoice();
		model2gui( selected );
	}
}

/*****************************************************
**
**   SheetStylePanel   ---   set_properties
**
******************************************************/
void SheetStylePanel::set_properties()
{
    // begin wxGlade: SheetStylePanel::set_properties
    choice_styles->SetSelection(0);
    choice_cell_bgmode->SetSelection(0);
    preview->SetMinSize(wxSize(200, 220));
    // end wxGlade
}

/*****************************************************
**
**   SheetStylePanel   ---   do_layout
**
******************************************************/
void SheetStylePanel::do_layout()
{
    // begin wxGlade: SheetStylePanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
    wxBoxSizer* sizer_tablestyles = new wxBoxSizer(wxHORIZONTAL);
    sizer_preview_staticbox->Lower();
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* grid_tablestyles = new wxFlexGridSizer(2, 1, 0, 0);
    wxFlexGridSizer* grid_pane_table = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_table_border_staticbox->Lower();
    wxStaticBoxSizer* sizer_table_border = new wxStaticBoxSizer(sizer_table_border_staticbox, wxVERTICAL);
    sizer_table_cells_staticbox->Lower();
    wxStaticBoxSizer* sizer_table_cells = new wxStaticBoxSizer(sizer_table_cells_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_table_cells = new wxFlexGridSizer(5, 2, 0, 0);
    sizer_table_headers_staticbox->Lower();
    wxStaticBoxSizer* sizer_table_headers = new wxStaticBoxSizer(sizer_table_headers_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_properties = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* sizer_select_style = new wxFlexGridSizer(1, 3, 0, 0);
    sizer_select_style->Add(label_style, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_select_style->Add(choice_styles, 0, wxALL, 3);
    sizer_select_style->Add(button_manage_styles, 0, wxALL|wxALIGN_RIGHT, 3);
    sizer_select_style->AddGrowableCol(2);
    grid_tablestyles->Add(sizer_select_style, 1, wxALL|wxEXPAND, 3);
    sizer_table_headers->Add(check_use_header_colors, 0, wxALL, 3);
    grid_properties->Add(label_header_textcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_properties->Add(button_header_textcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_properties->Add(label_header_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_properties->Add(button_header_bgcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_properties->AddGrowableCol(1);
    sizer_table_headers->Add(grid_properties, 1, wxEXPAND, 0);
    grid_pane_table->Add(sizer_table_headers, 1, wxEXPAND, 0);
    sizer_table_cells->Add(check_use_cell_colors, 0, wxALL, 3);
    grid_table_cells->Add(label_cell_textcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_cell_textcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_cell_bgmode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(choice_cell_bgmode, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_all_row_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_all_cell_bgcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_even_row_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_even_cell_bgcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_odd_row_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_odd_cell_bgcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->AddGrowableCol(1);
    sizer_table_cells->Add(grid_table_cells, 1, wxALL|wxEXPAND, 3);
    grid_pane_table->Add(sizer_table_cells, 1, wxEXPAND, 0);
    sizer_table_border->Add(check_draw_border, 0, wxALL, 3);
    grid_pane_table->Add(sizer_table_border, 0, wxEXPAND, 0);
    grid_pane_table->AddGrowableCol(0);
    grid_tablestyles->Add(grid_pane_table, 1, wxEXPAND, 0);
    grid_tablestyles->AddGrowableCol(0);
    sizer_tablestyles->Add(grid_tablestyles, 0, wxALL|wxEXPAND, 3);
    grid_previewpanel->Add(preview, 1, wxALL|wxEXPAND, 3);
    panel_preview->SetSizer(grid_previewpanel);
    grid_previewpanel->AddGrowableRow(0);
    grid_previewpanel->AddGrowableCol(0);
    sizer_preview->Add(panel_preview, 1, wxALL|wxEXPAND, 3);
    sizer_tablestyles->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    sizer_main->Add(sizer_tablestyles, 1, wxEXPAND, 0);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(0);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createSheetStylePanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createSheetStylePanel( wxWindow *parent )
{
	return new SheetStylePanel( parent );
}

