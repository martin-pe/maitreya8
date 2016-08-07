/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/SheetStylePanel.cpp
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

#include "SheetStylePanel.h"

#include "BrushDialog.h"
#include "ChartProperties.h"
#include "Conf.h"
#include "ColorConfig.h"
#include "SheetWidget.h"
#include "guibase.h"
#include "Horoscope.h"
#include "Lang.h"
#include "PenDialog.h"
#include "Painter.h"
#include "SheetConfig.h"
#include "StyleManagerDialog.h"
#include "TextHelper.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/log.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

extern Config *config;

enum {
	CD_SELECT_STYLE = wxID_HIGHEST + 1,
	CD_MANAGE_STYLES,

	CD_SHEET_TEXTCOLOR,
	CD_SHEET_BRUSH,
	CD_SELECTED_MODE,
	CD_SELECTED_BRUSH,

	CD_USE_HEADER_COLORS,
	CD_HEADER_TEXTCOLOR,
	CD_HEADER_BRUSH,

	CD_USE_CELL_COLORS,
	CD_CELL_TEXTCOLOR,
	CD_CHOICE_CELL_BGMODE,
	CD_ALL_CELL_BRUSH,
	CD_ODD_CELL_BRUSH,
	CD_EVEN_CELL_BRUSH,

	CD_TABLE_DRAW_BORDER, 
	CD_OUTER_BORDER,
	CD_OUTER_BORDER_PEN,
	CD_HEADER_BORDER,
	CD_HEADER_BORDER_PEN,
	CD_CELL_BORDER,
	CD_CELL_BORDER_PEN,

	CD_DRAW_SHADOW,
	CD_SHADOW_WIDTH,
	CD_SHADOW_BRUSH
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
    notebook_style = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    notebook_pane_border = new wxPanel(notebook_style, wxID_ANY);
    notebook_pane_table = new wxPanel(notebook_style, wxID_ANY);
    notebook_pane_sheet = new wxPanel(notebook_style, wxID_ANY);
    sizer_selected_staticbox = new wxStaticBox(notebook_pane_sheet, wxID_ANY, _("Selection on Mouse Over"));
    sizer_table_headers_staticbox = new wxStaticBox(notebook_pane_table, wxID_ANY, _("Table Header"));
    sizer_table_cells_staticbox = new wxStaticBox(notebook_pane_table, wxID_ANY, _("Table Cells"));
    sizer_table_border_staticbox = new wxStaticBox(notebook_pane_border, wxID_ANY, _("Borders"));
    sizer_shadows_staticbox = new wxStaticBox(notebook_pane_border, wxID_ANY, _("Table Shadows"));
    sizer_preview_staticbox = new wxStaticBox(this, wxID_ANY, _("Preview"));
    sizer_sheetstyle_staticbox = new wxStaticBox(notebook_pane_sheet, wxID_ANY, _("Sheet Style"));
    label_style = new wxStaticText(this, wxID_ANY, _("Style"));
    const wxString choice_styles_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_styles = new wxChoice(this, CD_SELECT_STYLE, wxDefaultPosition, wxDefaultSize, 4, choice_styles_choices, 0);
    button_manage_styles = new wxButton(this, CD_MANAGE_STYLES, _("Manage Styles ..."));
    static_line_1 = new wxStaticLine(notebook_pane_sheet, wxID_ANY);
    label_sheet_textcolor = new wxStaticText(notebook_pane_sheet, wxID_ANY, _("Text Color"));
    button_sheet_textcolor = new wxColourPickerCtrl(notebook_pane_sheet, CD_SHEET_TEXTCOLOR);
    label_sheet_brush = new wxStaticText(notebook_pane_sheet, wxID_ANY, _("Paper"));
    button_sheet_brush = new wxButton(notebook_pane_sheet, CD_SHEET_BRUSH, _("Brush ..."));
    label_selection_mode = new wxStaticText(notebook_pane_sheet, wxID_ANY, _("Highlight on Mouse Over"));
    const wxString choice_selection_mode_choices[] = {
        _("Never"),
        _("Cell"),
        _("Row"),
        _("Row/Column")
    };
    choice_selection_mode = new wxChoice(notebook_pane_sheet, CD_SELECTED_MODE, wxDefaultPosition, wxDefaultSize, 4, choice_selection_mode_choices, 0);
    label_selected_brush = new wxStaticText(notebook_pane_sheet, wxID_ANY, _("Background"));
    button_selected_brush = new wxButton(notebook_pane_sheet, CD_SELECTED_BRUSH, _("Brush ..."));
    static_line_2 = new wxStaticLine(notebook_pane_table, wxID_ANY);
    check_use_header_colors = new wxCheckBox(notebook_pane_table, CD_USE_HEADER_COLORS, _("Use Header Colors"));
    label_header_textcolor = new wxStaticText(notebook_pane_table, wxID_ANY, _("Text Color"));
    button_header_textcolor = new wxColourPickerCtrl(notebook_pane_table, CD_HEADER_TEXTCOLOR);
    label_header_brush = new wxStaticText(notebook_pane_table, wxID_ANY, _("Background"));
    button_header_brush = new wxButton(notebook_pane_table, CD_HEADER_BRUSH, _("Brush ..."));
    check_use_cell_colors = new wxCheckBox(notebook_pane_table, CD_USE_CELL_COLORS, _("Use Cell Colors"));
    label_cell_textcolor = new wxStaticText(notebook_pane_table, wxID_ANY, _("Text Color"));
    button_cell_textcolor = new wxColourPickerCtrl(notebook_pane_table, CD_CELL_TEXTCOLOR);
    label_cell_bgmode = new wxStaticText(notebook_pane_table, wxID_ANY, _("Cell Background Mode"));
    const wxString choice_cell_bgmode_choices[] = {
        _("Transparent"),
        _("Constant"),
        _("Alternating")
    };
    choice_cell_bgmode = new wxChoice(notebook_pane_table, CD_CHOICE_CELL_BGMODE, wxDefaultPosition, wxDefaultSize, 3, choice_cell_bgmode_choices, 0);
    label_all_row_bgcolor = new wxStaticText(notebook_pane_table, wxID_ANY, _("All Rows"));
    button_all_cell_brush = new wxButton(notebook_pane_table, CD_ALL_CELL_BRUSH, _("Brush ..."));
    label_even_row_bgcolor = new wxStaticText(notebook_pane_table, wxID_ANY, _("Even Rows"));
    button_even_cell_brush = new wxButton(notebook_pane_table, CD_EVEN_CELL_BRUSH, _("Brush ..."));
    label_odd_row_bgcolor = new wxStaticText(notebook_pane_table, wxID_ANY, _("Odd Rows"));
    button_odd_cell_brush = new wxButton(notebook_pane_table, CD_ODD_CELL_BRUSH, _("Brush ..."));
    static_line_3 = new wxStaticLine(notebook_pane_border, wxID_ANY);
    check_draw_border = new wxCheckBox(notebook_pane_border, CD_TABLE_DRAW_BORDER, _("Draw Border"));
    label_outer_border = new wxStaticText(notebook_pane_border, wxID_ANY, _("Outer Border"));
    const wxString choice_outer_border_choices[] = {
        _("None"),
        _("Left/Right"),
        _("Top/Bottom"),
        _("All")
    };
    choice_outer_border = new wxChoice(notebook_pane_border, CD_OUTER_BORDER, wxDefaultPosition, wxDefaultSize, 4, choice_outer_border_choices, 0);
    button_outer_border_pen = new wxButton(notebook_pane_border, CD_OUTER_BORDER_PEN, _("Pen ..."));
    label_header_border = new wxStaticText(notebook_pane_border, wxID_ANY, _("Header Border"));
    const wxString choice_header_border_choices[] = {
        _("None"),
        _("Left/Right"),
        _("Top/Bottom"),
        _("All")
    };
    choice_header_border = new wxChoice(notebook_pane_border, CD_HEADER_BORDER, wxDefaultPosition, wxDefaultSize, 4, choice_header_border_choices, 0);
    button_header_border_pen = new wxButton(notebook_pane_border, CD_HEADER_BORDER_PEN, _("Pen ..."));
    label_cell_border = new wxStaticText(notebook_pane_border, wxID_ANY, _("Cell Border"));
    const wxString choice_cell_border_choices[] = {
        _("None"),
        _("Left/Right"),
        _("Top/Bottom"),
        _("All")
    };
    choice_cell_border = new wxChoice(notebook_pane_border, CD_CELL_BORDER, wxDefaultPosition, wxDefaultSize, 4, choice_cell_border_choices, 0);
    button_cell_border_pen = new wxButton(notebook_pane_border, CD_CELL_BORDER_PEN, _("Pen ..."));
    check_draw_shadow = new wxCheckBox(notebook_pane_border, CD_DRAW_SHADOW, _("Draw Shadow"));
    label_cell_bgcolor_copy = new wxStaticText(notebook_pane_border, wxID_ANY, _("Shadow Width"));
    spin_shadow_width = new wxSpinCtrl(notebook_pane_border, CD_SHADOW_WIDTH, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, -100, 100);
    label_cell_bgcolor_copy_1 = new wxStaticText(notebook_pane_border, wxID_ANY, _("Background"));
    button_shadow_brush = new wxButton(notebook_pane_border, CD_SHADOW_BRUSH, _("Brush ..."));
    preview = new SheetWidget(panel_preview, props, sheetcfg);

    set_properties();
    do_layout();
    // end wxGlade

	notebook_style->SetSelection( config->viewprefs->configDialogSheetStylePanelPage );

	Connect( CD_SELECT_STYLE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnSelectStyle ));
	Connect( CD_MANAGE_STYLES, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnManageStyles ));

	Connect( CD_SHEET_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonBrush ));
	Connect( CD_HEADER_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonBrush ));
	Connect( CD_ALL_CELL_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonBrush ));
	Connect( CD_EVEN_CELL_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonBrush ));
	Connect( CD_ODD_CELL_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonBrush ));
	Connect( CD_SHADOW_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonBrush ));
	Connect( CD_SELECTED_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonBrush ));

	Connect( CD_OUTER_BORDER_PEN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonPen ));
	Connect( CD_HEADER_BORDER_PEN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonPen ));
	Connect( CD_CELL_BORDER_PEN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( SheetStylePanel::OnButtonPen ));

	Connect( wxID_ANY, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( SheetStylePanel::OnTableColorPickerEvent ));

	Connect( CD_CHOICE_CELL_BGMODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SheetStylePanel::OnCommand ));
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxCommandEventHandler( SheetStylePanel::OnCommand ));
	Connect( CD_OUTER_BORDER, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnCommand ));
	Connect( CD_HEADER_BORDER, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnCommand ));
	Connect( CD_CELL_BORDER, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnCommand ));
	Connect( CD_SELECTED_MODE, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( SheetStylePanel::OnCommand ));

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
	config->viewprefs->configDialogSheetStylePanelPage = notebook_style->GetSelection();
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
	
	button_sheet_textcolor->SetColour( sheetcfg->textColor );
	printf( "COLOR %s\n", str2char( sheetcfg->textColor.GetAsString( wxC2S_CSS_SYNTAX ) ));
	choice_selection_mode->SetSelection( sheetcfg->selectionMode );

	check_use_header_colors->SetValue( sheetcfg->tablestyle.useHeaderColors );
	button_header_textcolor->SetColour( sheetcfg->tablestyle.headerTextColor );

	check_use_cell_colors->SetValue( sheetcfg->tablestyle.useCellColors );
	choice_cell_bgmode->SetSelection( sheetcfg->tablestyle.cellBgMode );
	button_cell_textcolor->SetColour( sheetcfg->tablestyle.cellTextColor );

	check_draw_border->SetValue( sheetcfg->tablestyle.gridStyle );
	choice_outer_border->SetSelection( sheetcfg->tablestyle.outerBorderStyle );
	choice_header_border->SetSelection( sheetcfg->tablestyle.headerBorderStyle );
	choice_cell_border->SetSelection( sheetcfg->tablestyle.cellBorderStyle );

	// shadow
	check_draw_shadow->SetValue( sheetcfg->tablestyle.shadowStyle );
	spin_shadow_width->SetValue( sheetcfg->tablestyle.shadowWidth );

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
	printf( "SheetStylePanel::getSheetConfig sel %d size %d\n", sel, (int)v.size());
	//assert( sel >= 0 && sel < (int)v.size());
	if( sel < 0 || sel >= (int)v.size())
	{
		wxLogError( wxT( "invalid sheet selection %d, using default" ), sel );
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

	// 1st tab: sheet
	sheetcfg->textColor = button_sheet_textcolor->GetColour();
	sheetcfg->selectionMode = choice_selection_mode->GetSelection();
	button_selected_brush->Enable( sheetcfg->selectionMode != 0 );
	label_selected_brush->Enable( sheetcfg->selectionMode != 0 );

	// 2nd tab: table
	const bool activateHeaderItems = check_use_header_colors->GetValue();
	sheetcfg->tablestyle.useHeaderColors = activateHeaderItems;
	sheetcfg->tablestyle.headerTextColor = button_header_textcolor->GetColour();

	label_header_textcolor->Enable( activateHeaderItems );
	button_header_textcolor->Enable( activateHeaderItems );
	label_header_brush->Enable( activateHeaderItems );
	button_header_brush->Enable( activateHeaderItems );

	// 2a: table cell items
	const bool activateCellItems = check_use_cell_colors->GetValue();
	sheetcfg->tablestyle.useCellColors = activateCellItems;
	sheetcfg->tablestyle.cellTextColor = button_cell_textcolor->GetColour();
	sheetcfg->tablestyle.cellBgMode = choice_cell_bgmode->GetSelection();

	label_cell_textcolor->Enable( activateCellItems );
	button_cell_textcolor->Enable( activateCellItems );
	label_cell_bgmode->Enable( activateCellItems );
	choice_cell_bgmode->Enable( activateCellItems );

	const bool enableAllCellBg = ( activateCellItems && choice_cell_bgmode->GetSelection() == 1 );
	label_all_row_bgcolor->Enable( enableAllCellBg );
	button_all_cell_brush->Enable( enableAllCellBg );

	const bool enableOddEvenBg = ( activateCellItems && choice_cell_bgmode->GetSelection() == 2 );
	label_odd_row_bgcolor->Enable( enableOddEvenBg );
	button_odd_cell_brush->Enable( enableOddEvenBg );
	label_even_row_bgcolor->Enable( enableOddEvenBg );
	button_even_cell_brush->Enable( enableOddEvenBg );

	// 3rd tab: table borders
	const bool dogrid = check_draw_border->GetValue();
	sheetcfg->tablestyle.gridStyle = dogrid;
	sheetcfg->tablestyle.outerBorderStyle = choice_outer_border->GetSelection();
	sheetcfg->tablestyle.cellBorderStyle = choice_cell_border->GetSelection();
	sheetcfg->tablestyle.headerBorderStyle = choice_header_border->GetSelection();

	choice_outer_border->Enable( dogrid );
	choice_cell_border->Enable( dogrid );
	choice_header_border->Enable( dogrid );

	button_outer_border_pen->Enable( choice_outer_border->GetSelection() != 0 && dogrid );
	button_header_border_pen->Enable( choice_header_border->GetSelection() != 0 && dogrid );
	button_cell_border_pen->Enable( choice_cell_border->GetSelection() != 0 && dogrid );

	// shadow
	const bool useShadows = check_draw_shadow->IsChecked();
	sheetcfg->tablestyle.shadowStyle = useShadows;
	sheetcfg->tablestyle.shadowWidth = spin_shadow_width->GetValue();

	button_shadow_brush->Enable( useShadows );
	spin_shadow_width->Enable( useShadows );

	// update preview 
	preview->setSheetConfig( sheetcfg );
	preview->clearSheet();
	Sheet *sheet = preview->getSheet();
	horoscope->setCurrentDate();
	horoscope->update();
	TextHelper( horoscope, props, sheet ).writePlanets();
	sheet->addHeader( wxT( "Brihat Parasara Hora Shastra" ));
	sheet->addParagraph(
		wxT( "The unborn Lord has many incarnations. He has incarnated as the 9 Grahas to bestow on the living beings the results due to their Karmas." ));
	preview->OnDataChanged();	
}


/*****************************************************
**
**   SheetStylePanel   ---   OnManageStyles
**
******************************************************/
void SheetStylePanel::OnManageStyles( wxCommandEvent& )
{
	printf( "SheetStylePanel::OnManageStyles size %d\n", (int)v.size() );

	StyleManagerDialog dialog( this, v, selected );
	dialog.ShowModal();
	selected = dialog.getSelection();

	//setData();
	populateStyleChoice();
	model2gui( selected );

	setDirty();
	printf( "END SheetStylePanel::OnManageStyles size %d\n", (int)v.size() );
}

/*****************************************************
**
**   SheetStylePanel   ---   OnButtonBrush
**
******************************************************/
void SheetStylePanel::OnButtonBrush( wxCommandEvent &event )
{
	const int id = event.GetId();
	printf( "SheetStylePanel::OnButtonBrush id %d\n", id );
	SheetConfig *sheetcfg = getSheetConfig( selected );

	MBrush *b;
	switch( event.GetId() )
	{
		case CD_SHEET_BRUSH:
			b = &sheetcfg->brush;
		break;
		case CD_HEADER_BRUSH:
			b = &sheetcfg->tablestyle.headerBrush;
		break;
		case CD_ALL_CELL_BRUSH:
			b = &sheetcfg->tablestyle.cellBrush;
		break;
		case CD_EVEN_CELL_BRUSH:
			b = &sheetcfg->tablestyle.evenCellBrush;
		break;
		case CD_ODD_CELL_BRUSH:
			b = &sheetcfg->tablestyle.oddCellBrush;
		break;
		case CD_SHADOW_BRUSH:
			b = &sheetcfg->tablestyle.shadowBrush;
		break;
		case CD_SELECTED_BRUSH:
			b = &sheetcfg->selectedItemBrush;
		break;
		default:
			assert( false );
		break;
	}

	printf( "Before style %d\n", b->style );
	BrushDialog dialog( this, *b );
	if ( dialog.ShowModal() == wxID_OK )
	{
		*b = dialog.getBrush();
		printf( "Afte style %d\n", b->style );
		setDirty();
		preview->OnDataChanged();	
	}
}

/*****************************************************
**
**   SheetStylePanel   ---   OnButtonPen
**
******************************************************/
void SheetStylePanel::OnButtonPen( wxCommandEvent &event )
{
	const int id = event.GetId();
	printf( "SheetStylePanel::OnButtonPen id %d\n", id );
	SheetConfig *sheetcfg = getSheetConfig( selected );

	wxPen *pen;
	switch( event.GetId() )
	{
		case CD_OUTER_BORDER_PEN:
			pen = &sheetcfg->tablestyle.outerBorderPen;
		break;
		case CD_HEADER_BORDER_PEN:
			pen = &sheetcfg->tablestyle.headerBorderPen;
		break;
		case CD_CELL_BORDER_PEN:
			pen = &sheetcfg->tablestyle.cellBorderPen;
		break;
		default:
			assert( false );
		break;
	}
	PenDialog dialog( this, *pen );
	if ( dialog.ShowModal() == wxID_OK )
	{
		*pen = *dialog.getPen();
		setDirty();
		preview->OnDataChanged();	
	}
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
		printf( "YES\n" );
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
    button_sheet_textcolor->SetMinSize(wxSize(60, 24));
    choice_selection_mode->SetSelection(0);
    button_header_textcolor->SetMinSize(wxSize(60, 24));
    button_cell_textcolor->SetMinSize(wxSize(60, 24));
    choice_cell_bgmode->SetSelection(0);
    choice_outer_border->SetSelection(0);
    choice_header_border->SetSelection(0);
    choice_cell_border->SetSelection(0);
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
	preview->setMinXRight( 400 );

    // begin wxGlade: SheetStylePanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 1, 0, 0);
    wxBoxSizer* sizer_tablestyles = new wxBoxSizer(wxHORIZONTAL);
    sizer_preview_staticbox->Lower();
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_previewpanel = new wxFlexGridSizer(1, 1, 3, 3);
    wxFlexGridSizer* grid_tablestyles = new wxFlexGridSizer(2, 1, 0, 0);
    wxFlexGridSizer* grid_pane_border = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_shadows_staticbox->Lower();
    wxStaticBoxSizer* sizer_shadows = new wxStaticBoxSizer(sizer_shadows_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_shadows = new wxFlexGridSizer(2, 2, 0, 0);
    sizer_table_border_staticbox->Lower();
    wxStaticBoxSizer* sizer_table_border = new wxStaticBoxSizer(sizer_table_border_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sizer_2 = new wxFlexGridSizer(3, 3, 0, 0);
    wxFlexGridSizer* grid_pane_table = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_table_cells_staticbox->Lower();
    wxStaticBoxSizer* sizer_table_cells = new wxStaticBoxSizer(sizer_table_cells_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_table_cells = new wxFlexGridSizer(5, 2, 0, 0);
    sizer_table_headers_staticbox->Lower();
    wxStaticBoxSizer* sizer_table_headers = new wxStaticBoxSizer(sizer_table_headers_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_properties = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* grid_pane_sheet = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_selected_staticbox->Lower();
    wxStaticBoxSizer* sizer_selected = new wxStaticBoxSizer(sizer_selected_staticbox, wxVERTICAL);
    wxFlexGridSizer* sizer_bgbrush = new wxFlexGridSizer(2, 2, 0, 0);
    sizer_sheetstyle_staticbox->Lower();
    wxStaticBoxSizer* sizer_sheetstyle = new wxStaticBoxSizer(sizer_sheetstyle_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_sheetstyle = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* sizer_select_style = new wxFlexGridSizer(1, 3, 0, 0);
    sizer_select_style->Add(label_style, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_select_style->Add(choice_styles, 0, wxALL, 3);
    sizer_select_style->Add(button_manage_styles, 0, wxALL|wxALIGN_RIGHT, 3);
    sizer_select_style->AddGrowableCol(2);
    grid_tablestyles->Add(sizer_select_style, 1, wxALL|wxEXPAND, 3);
    grid_pane_sheet->Add(static_line_1, 0, wxALL|wxEXPAND, 3);
    grid_sheetstyle->Add(label_sheet_textcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sheetstyle->Add(button_sheet_textcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_sheetstyle->Add(label_sheet_brush, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sheetstyle->Add(button_sheet_brush, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_sheetstyle->AddGrowableCol(1);
    sizer_sheetstyle->Add(grid_sheetstyle, 1, wxEXPAND, 0);
    grid_pane_sheet->Add(sizer_sheetstyle, 1, wxEXPAND, 3);
    sizer_bgbrush->Add(label_selection_mode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_bgbrush->Add(choice_selection_mode, 0, wxALL|wxALIGN_RIGHT, 3);
    sizer_bgbrush->Add(label_selected_brush, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_bgbrush->Add(button_selected_brush, 0, wxALL|wxALIGN_RIGHT, 3);
    sizer_bgbrush->AddGrowableCol(1);
    sizer_selected->Add(sizer_bgbrush, 1, wxEXPAND, 0);
    grid_pane_sheet->Add(sizer_selected, 1, wxEXPAND, 0);
    notebook_pane_sheet->SetSizer(grid_pane_sheet);
    grid_pane_sheet->AddGrowableCol(0);
    grid_pane_table->Add(static_line_2, 0, wxALL|wxEXPAND, 3);
    sizer_table_headers->Add(check_use_header_colors, 0, wxALL, 3);
    grid_properties->Add(label_header_textcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_properties->Add(button_header_textcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_properties->Add(label_header_brush, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_properties->Add(button_header_brush, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_properties->AddGrowableCol(1);
    sizer_table_headers->Add(grid_properties, 1, wxEXPAND, 0);
    grid_pane_table->Add(sizer_table_headers, 1, wxEXPAND, 0);
    sizer_table_cells->Add(check_use_cell_colors, 0, wxALL, 3);
    grid_table_cells->Add(label_cell_textcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_cell_textcolor, 1, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_cell_bgmode, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(choice_cell_bgmode, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_all_row_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_all_cell_brush, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_even_row_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_even_cell_brush, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->Add(label_odd_row_bgcolor, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_table_cells->Add(button_odd_cell_brush, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_table_cells->AddGrowableCol(1);
    sizer_table_cells->Add(grid_table_cells, 1, wxALL|wxEXPAND, 3);
    grid_pane_table->Add(sizer_table_cells, 1, wxEXPAND, 0);
    notebook_pane_table->SetSizer(grid_pane_table);
    grid_pane_table->AddGrowableCol(0);
    grid_pane_border->Add(static_line_3, 0, wxALL|wxEXPAND, 3);
    sizer_table_border->Add(check_draw_border, 0, wxALL, 3);
    grid_sizer_2->Add(label_outer_border, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(choice_outer_border, 0, wxALL, 3);
    grid_sizer_2->Add(button_outer_border_pen, 0, wxALL, 3);
    grid_sizer_2->Add(label_header_border, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(choice_header_border, 0, wxALL, 3);
    grid_sizer_2->Add(button_header_border_pen, 0, wxALL, 3);
    grid_sizer_2->Add(label_cell_border, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_sizer_2->Add(choice_cell_border, 0, wxALL, 3);
    grid_sizer_2->Add(button_cell_border_pen, 0, wxALL, 3);
    sizer_table_border->Add(grid_sizer_2, 1, wxEXPAND, 0);
    grid_pane_border->Add(sizer_table_border, 0, wxEXPAND, 0);
    sizer_shadows->Add(check_draw_shadow, 0, wxALL, 3);
    grid_shadows->Add(label_cell_bgcolor_copy, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_shadows->Add(spin_shadow_width, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_shadows->Add(label_cell_bgcolor_copy_1, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_shadows->Add(button_shadow_brush, 0, wxALL|wxALIGN_RIGHT, 3);
    grid_shadows->AddGrowableCol(1);
    sizer_shadows->Add(grid_shadows, 1, wxEXPAND, 0);
    grid_pane_border->Add(sizer_shadows, 0, wxEXPAND, 0);
    notebook_pane_border->SetSizer(grid_pane_border);
    notebook_style->AddPage(notebook_pane_sheet, _("Sheet"));
    notebook_style->AddPage(notebook_pane_table, _("Table"));
    notebook_style->AddPage(notebook_pane_border, _("Table Borders"));
    grid_tablestyles->Add(notebook_style, 1, wxALL|wxEXPAND, 4);
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

