/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/dialogs/ColorPanel.cpp
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

#include "ColorPanel.h"

#include "Ashtakavarga.h"
#include "BarDiagram.h"
#include "BrushDialog.h"
#include "ChartProperties.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "Dasa.h"
#include "guibase.h"
#include "Horoscope.h"
#include "Lang.h"
#include "mathbase.h"
#include "mvalidator.h"
#include "Painter.h"
#include "PenDialog.h"
#include "PlanetList.h"
#include "SheetWidget.h"
#include "SymbolProvider.h"
#include "TextHelper.h"

#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/clrpicker.h>
#include <wx/dc.h>
#include <wx/dcclient.h>
#include <wx/notebook.h>
#include <wx/sizer.h>
#include <wx/spinctrl.h>
#include <wx/statbox.h>
#include <wx/statline.h>
#include <wx/stattext.h>

extern Config *config;

enum {
	// left
	CD_FGCOLOR = wxID_HIGHEST + 1,
	CD_BENEFIC_COLOR,
	CD_MALEFIC_COLOR,
	CD_TRANSIT_COLOR,
	CD_BGCOLOR,
	CD_MARKED_FIELDS_SBC_COLOR,
	CD_MARKED_FIELDS_COLOR,
	CD_ERROR_COLOR,

	// right
	CD_SIGNS,
	CD_HOUSES, CD_PLANETS, // choices
	CD_PLANET_FGCOLOR,
	CD_SIGN_FGCOLOR,
	CD_SIGN_BRUSH,
	CD_HOUSE_FGCOLOR,
	CD_HOUSE_BRUSH,

	// bar diagram panel
	CD_BAR_OBJECT, CD_BAR_PEN, CD_BAR_BRUSH,
	CD_CHECK_ZOOM, CD_SPIN_CORNER_RADIUS
};

class BasicView;

IMPLEMENT_CLASS( ColorPanel, ConfigPanel )

/*************************************************//**
*
* 
*
******************************************************/
class ObjectColorPreviewPanel : public wxPanel
{
public:
	wxColour fgcolor;
	MBrush brush;
	int item;

protected:
	ObjectColorPreviewPanel( wxWindow* parent )
		: wxPanel( parent, -1, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE )
		{
			item = 0;
			Connect( wxEVT_PAINT, wxPaintEventHandler( ObjectColorPreviewPanel::OnPaint ));
		}

	virtual void OnPaint( wxPaintEvent& )
	{
		wxCoord h, w;
		wxPaintDC context( this );
		PrepareDC( context );

		Painter *painter = new DcPainter( &context );
		painter->setBrush( brush );
		painter->setPen( brush.color );

		GetSize( &w, &h );
		MRect rect ( 0, 0, w, h );
		painter->drawRectangle( rect );

		painter->setTextColor( fgcolor );
		paintInternal( painter, rect );
		delete painter;
	}

	virtual void paintInternal( Painter *painter, const MRect &rect ) = 0;

	Lang lang;
};

/*************************************************//**
*
* 
*
******************************************************/
class PlanetColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	PlanetColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MRect &rect )
	{
		ASSERT_VALID_EPHEM_OBJECT( item );
		SymbolProvider sp;
		painter->setSymbolFont();
		painter->drawTextFormatted( rect, sp.getPlanetCode( (ObjectId)item ), Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class HouseFgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	HouseFgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MRect &rect )
	{
		wxString s;
		assert( item >= 0 && item < 4 );
		painter->setGraphicFont();
		//painter->drawTextFormatted( rect, wxString::Format( wxT( "%d %d %d" ), item + 1, item + 5, item + 9 ), Align::Center );

		const int houseNumberStyle = ChartProperties().getWesternChartDisplayConfig().houseNumberStyle == 2 ? 2 : 1;
		s << getHouseNumberFormatted( item + 1, houseNumberStyle )
			<< SPACE << getHouseNumberFormatted( item + 5, houseNumberStyle )
			<< SPACE << getHouseNumberFormatted( ( item + 9 ) % 12, houseNumberStyle );
		painter->drawTextFormatted( rect, s, Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class HouseBgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	HouseBgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MRect &rect )
	{
		wxString s;
		assert( item >= 0 && item < 4 );
		painter->setGraphicFont();
		//painter->drawTextFormatted( rect, wxString::Format( wxT( "%d %d %d" ), item + 1, item + 5, item + 9 ), Align::Center );
		const int houseNumberStyle = ChartProperties().getWesternChartDisplayConfig().houseNumberStyle == 2 ? 2 : 1;
		s << getHouseNumberFormatted( item + 1, houseNumberStyle )
			<< SPACE << getHouseNumberFormatted( item + 5, houseNumberStyle )
			<< SPACE << getHouseNumberFormatted( ( item + 9 ) % 12, houseNumberStyle );
		painter->drawTextFormatted( rect, s, Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class SignFgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	SignFgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MRect &rect )
	{
		assert( item >= 0 && item < 4 );

		SymbolProvider sp;
		painter->setSymbolFont();
		wxString s;
		s << sp.getSignCode( (Rasi)item ) << SPACE
			<< sp.getSignCode( (Rasi)( item + 4 )) << SPACE
			<< sp.getSignCode( (Rasi)( item + 8 ));
		painter->drawTextFormatted( rect, s, Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class SignBgColorPreviewPanel : public ObjectColorPreviewPanel
{
public:
	SignBgColorPreviewPanel( wxWindow* parent ) : ObjectColorPreviewPanel( parent ) {}

protected:
	virtual void paintInternal( Painter *painter, const MRect &rect )
	{
		assert( item >= 0 && item < 4 );

		SymbolProvider sp;
		painter->setSymbolFont();
		wxString s;
		s << sp.getSignCode( (Rasi)item ) << SPACE
			<< sp.getSignCode( (Rasi)( item + 4 )) << SPACE
			<< sp.getSignCode( (Rasi)( item + 8 ));
		painter->drawTextFormatted( rect, s, Align::Center );
	}
};

/*************************************************//**
*
* 
*
******************************************************/
class DasaBarPreviewItem : public SheetWidgetItem
{
public:

	DasaBarPreviewItem( ChartProperties *props, BarDiagramConfig *cfg )
	 : SheetWidgetItem( props ),
		barconfig( cfg ),
		ybar( 30 ),
		xborder( 30 )
	{
		//ratio = .1;
		yfixed = 70;
	}

	virtual SheetItem *cloneClean() { return new DasaBarPreviewItem( props, barconfig ); }

	void setObjectId( const int &id ) { objectId = id; }

	/*****************************************************
	**
	**   DasaBarPreviewItem   ---   doPaint
	**
	******************************************************/
	void doPaint( Painter *painter, const MRect& /*refreshRect*/ )
	{
		//printf( "DasaBarPreviewItem::doPaint objectId %d\n", objectId );
		FontProvider *fc = FontProvider::get();

		// background
		painter->setDefaultPen();
		painter->setDefaultBrush();
		painter->drawRectangle( rect, 10 );

		bool isGrahaLord = ( objectId >= 12 );

		DasaExpert *expert = DasaExpertFactory().getDasaExpert( isGrahaLord ? D_VIMSOTTARI : D_KALACHAKRA );
		int lordid = objectId;
		if ( isGrahaLord )
		{
			if ( objectId <= 18 ) lordid = objectId - 12;
			else if ( objectId == 19 ) lordid = OMEANNODE;
			else if ( objectId == 20 ) lordid = OMEANDESCNODE;
			else if ( objectId == 21 ) lordid = OASCENDANT;
			else assert( false );
		}

		assert( lordid >= 0 );

		const double y0 = rect.y + .5 * rect.height;
		wxRect r( rect.x + xborder, y0 - .5 * ybar, rect.width - 2 * xborder, ybar );
		if ( barconfig->doItemZoom && hasMarkup())
		{
			r = wxRect( r.x - barconfig->itemZoom, r.y - barconfig->itemZoom, r.width + 2 * barconfig->itemZoom, r.height + 2 * barconfig->itemZoom );
		}

		BarDiagramStyle &style = barconfig->style[objectId];

		painter->setFont( *fc->getFont( FONT_GRAPHIC_DEFAULT ));

		painter->setPen( style.pen );
		painter->setTextColor( style.pen.GetColour() );
		painter->setBrush( style.brush );
		//printf( "BRUSH %s\n", str2char( style.brush.toString()));

		painter->drawRectangle( r, barconfig->cornerRadius );
		MString lordname = expert->getDasaLordNameF( lordid, TF_LONG );
		painter->drawMString( r, lordname, Align::Center );

		delete expert;
	}

	/*****************************************************
	**
	**   DasaBarPreviewItem   ---   getItem4Point
	**
	******************************************************/
	uint getItem4Point( const wxPoint &p ) const
	{ 
		const double y0 = rect.y + .5 * rect.height;
		wxRect r( rect.x + xborder, y0 - .5 * ybar, rect.width - 2 * xborder, ybar );
		return pointInRect( p, r ) ? 0 : NO_ITEM;
	}   

private:
	BarDiagramConfig *barconfig;
	int objectId;

	const int ybar;	
	const int xborder;
};

/*****************************************************
**
**   ColorPanel   ---   Constructor
**
******************************************************/
ColorPanel::ColorPanel( wxWindow* parent  ) : ConfigPanel( parent )
{
	colorConfig = new ColorConfig;
	objectColors = new ObjectColorConfig;
	barconfig = new BarDiagramConfig;
	props = new ChartProperties;
	config2model();

    // begin wxGlade: ColorPanel::ColorPanel
    notebook_main = new wxNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0);
    notebook_pane_bar = new wxPanel(notebook_main, wxID_ANY);
    notebook_pane_charts = new wxPanel(notebook_main, wxID_ANY);
    notebook_pane_general = new wxPanel(notebook_main, wxID_ANY);
    sizer_bgcolors_staticbox = new wxStaticBox(notebook_pane_general, wxID_ANY, _("Background Colors"));
    sizer_planets_staticbox = new wxStaticBox(notebook_pane_charts, wxID_ANY, _("Planets"));
    sizer_signs_staticbox = new wxStaticBox(notebook_pane_charts, wxID_ANY, _("Signs"));
    sizer_houses_staticbox = new wxStaticBox(notebook_pane_charts, wxID_ANY, _("Houses"));
    sizer_object_staticbox = new wxStaticBox(notebook_pane_bar, wxID_ANY, _("Object"));
    sizer_mouseover_staticbox = new wxStaticBox(notebook_pane_bar, wxID_ANY, _("Mouse over Behaviour"));
    sizer_general_staticbox = new wxStaticBox(notebook_pane_bar, wxID_ANY, _("Graphical Dasa View Options"));
    sizer_preview_staticbox = new wxStaticBox(notebook_pane_bar, wxID_ANY, _("Preview"));
    sizer_fgcolors_staticbox = new wxStaticBox(notebook_pane_general, wxID_ANY, _("Foreground Colors"));
    label_fg = new wxStaticText(notebook_pane_general, wxID_ANY, _("Text Color"));
    button_color_fg = new wxColourPickerCtrl(notebook_pane_general, CD_FGCOLOR);
    label_benefic = new wxStaticText(notebook_pane_general, wxID_ANY, _("Benefics"));
    button_color_benefic = new wxColourPickerCtrl(notebook_pane_general, CD_BENEFIC_COLOR);
    label_malefic = new wxStaticText(notebook_pane_general, wxID_ANY, _("Malefics"));
    button_color_malefic = new wxColourPickerCtrl(notebook_pane_general, CD_MALEFIC_COLOR);
    label_transit = new wxStaticText(notebook_pane_general, wxID_ANY, _("Transit Planets"));
    button_color_transit = new wxColourPickerCtrl(notebook_pane_general, CD_TRANSIT_COLOR);
    label_bg = new wxStaticText(notebook_pane_general, wxID_ANY, _("Default Background"));
    button_color_bg = new wxColourPickerCtrl(notebook_pane_general, CD_BGCOLOR);
    label_marked = new wxStaticText(notebook_pane_general, wxID_ANY, _("Marked Fields in SBC"));
    button_color_marked = new wxColourPickerCtrl(notebook_pane_general, CD_MARKED_FIELDS_SBC_COLOR);
    label_chart_markup = new wxStaticText(notebook_pane_general, wxID_ANY, _("Marked Fields in Charts"));
    button_color_chart_markup = new wxColourPickerCtrl(notebook_pane_general, CD_MARKED_FIELDS_COLOR);
    label_error = new wxStaticText(notebook_pane_general, wxID_ANY, _("Errors"));
    button_color_error = new wxColourPickerCtrl(notebook_pane_general, CD_ERROR_COLOR);
    const wxString choice_planets_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_planets = new wxChoice(notebook_pane_charts, CD_PLANETS, wxDefaultPosition, wxDefaultSize, 5, choice_planets_choices, 0);
    label_fgplanets = new wxStaticText(notebook_pane_charts, wxID_ANY, _("Foreground"));
    color_fgplanets = new wxColourPickerCtrl(notebook_pane_charts, CD_PLANET_FGCOLOR);
    preview_planetcolor = new PlanetColorPreviewPanel(notebook_pane_charts);
    const wxString choice_signs_choices[] = {
        _("Fire"),
        _("Earth"),
        _("Air"),
        _("Water")
    };
    choice_signs = new wxChoice(notebook_pane_charts, CD_SIGNS, wxDefaultPosition, wxDefaultSize, 4, choice_signs_choices, 0);
    label_fg_signs = new wxStaticText(notebook_pane_charts, wxID_ANY, _("Foreground"));
    color_fgsigns = new wxColourPickerCtrl(notebook_pane_charts, CD_SIGN_FGCOLOR);
    preview_signfgcolor = new SignFgColorPreviewPanel(notebook_pane_charts);
    label_bg_signs = new wxStaticText(notebook_pane_charts, wxID_ANY, _("Background"));
    button_signbrush = new wxButton(notebook_pane_charts, CD_SIGN_BRUSH, _("Brush ..."));
    preview_signbgcolor = new SignBgColorPreviewPanel(notebook_pane_charts);
    const wxString choice_houses_choices[] = {
        _("Fire"),
        _("Earth"),
        _("Air"),
        _("Water")
    };
    choice_houses = new wxChoice(notebook_pane_charts, CD_HOUSES, wxDefaultPosition, wxDefaultSize, 4, choice_houses_choices, 0);
    label_fg_houses = new wxStaticText(notebook_pane_charts, wxID_ANY, _("Foreground"));
    color_fghouses = new wxColourPickerCtrl(notebook_pane_charts, CD_HOUSE_FGCOLOR);
    preview_housefgcolor = new HouseFgColorPreviewPanel(notebook_pane_charts);
    label_bg_houses = new wxStaticText(notebook_pane_charts, wxID_ANY, _("Background"));
    button_housebrush = new wxButton(notebook_pane_charts, CD_HOUSE_BRUSH, _("Brush ..."));
    preview_housebgcolor = new HouseBgColorPreviewPanel(notebook_pane_charts);
    label_2 = new wxStaticText(notebook_pane_bar, wxID_ANY, _("Select Object"));
    const wxString choice_object_choices[] = {
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy"),
        _("dummy")
    };
    choice_object = new wxChoice(notebook_pane_bar, CD_BAR_OBJECT, wxDefaultPosition, wxDefaultSize, 10, choice_object_choices, 0);
    button_pen = new wxButton(notebook_pane_bar, CD_BAR_PEN, _("Pen ..."));
    button_brush = new wxButton(notebook_pane_bar, CD_BAR_BRUSH, _("Brush ..."));
    check_zoom = new wxCheckBox(notebook_pane_bar, CD_CHECK_ZOOM, _("Zoom on Mouse Over"));
    spin_zoom = new wxSpinCtrl(notebook_pane_bar, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 1, 15);
    label_pixel = new wxStaticText(notebook_pane_bar, wxID_ANY, _("Pixel"));
    check_showtoolbarmsg = new wxCheckBox(notebook_pane_bar, wxID_ANY, _("Show Messages in Toolbar"));
    label_cornerradius = new wxStaticText(notebook_pane_bar, wxID_ANY, _("Dasa Bar Corner Radius"));
    spin_cornerradius = new wxSpinCtrl(notebook_pane_bar, CD_SPIN_CORNER_RADIUS, wxT(""), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 45);
    label_pixel2 = new wxStaticText(notebook_pane_bar, wxID_ANY, _("Pixel"));
    barwidget = new SheetWidget(notebook_pane_bar, props, NULL, NULL, colorConfig);

    set_properties();
    do_layout();
    // end wxGlade

	choice_object->Clear();
	for ( int i = 0; i < MAX_BARDIAGRAM_COLORS; i++ )
	{
		choice_object->Append( barconfig->style[i].name );
	}
	choice_object->SetSelection( 0 );

	horoscope = new Horoscope();
	horoscope->update();
	expert = new AshtakavargaExpert( horoscope, V_RASI );
	expert->update();

	notebook_main->SetSelection( config->viewprefs->configDialogColorPanelPage );

	button_color_fg->SetValidator( MColourPickerValidator( &colorConfig->fgColor ));
	button_color_bg->SetValidator( MColourPickerValidator( &colorConfig->bgColor ));
	button_color_marked->SetValidator( MColourPickerValidator( &colorConfig->sbcMarkedFieldsBgColor ));
	button_color_benefic->SetValidator( MColourPickerValidator( &colorConfig->beneficFgColor ));
	button_color_malefic->SetValidator( MColourPickerValidator( &colorConfig->maleficFgColor ));
	button_color_transit->SetValidator( MColourPickerValidator( &colorConfig->transitFgColor ));
	button_color_error->SetValidator( MColourPickerValidator( &colorConfig->errorBgColor ));
	button_color_chart_markup->SetValidator( MColourPickerValidator( &colorConfig->chartMarkupBgColor ));

	check_showtoolbarmsg->SetValidator( MCheckValidator( &barconfig->showToolbarMessages ));
	spin_cornerradius->SetValidator( MSpinValidator( &barconfig->cornerRadius ));
	spin_zoom->SetValidator( MSpinValidator( &barconfig->itemZoom ));
	check_zoom->SetValidator( MCheckValidator( &barconfig->doItemZoom ));

	// 1 st tab
	Connect( CD_FGCOLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));
	Connect( CD_BENEFIC_COLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));
	Connect( CD_MALEFIC_COLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));
	Connect( CD_TRANSIT_COLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));
	Connect( CD_BGCOLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));
	Connect( CD_MARKED_FIELDS_SBC_COLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));
	Connect( CD_MARKED_FIELDS_COLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));
	Connect( CD_ERROR_COLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnGlobalColorChanged ));

	// handlers for object, sign and house colors (2nd panel)
	Connect( CD_PLANETS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ColorPanel::OnObjectChoice ));
	Connect( CD_PLANET_FGCOLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnObjectColorChanged ));

	Connect( CD_SIGNS, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ColorPanel::OnObjectChoice ));
	Connect( CD_SIGN_FGCOLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnObjectColorChanged ));
	Connect( CD_SIGN_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ColorPanel::OnButtonSignBrush ));

	Connect( CD_HOUSES, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ColorPanel::OnObjectChoice ));
	Connect( CD_HOUSE_FGCOLOR, wxEVT_COMMAND_COLOURPICKER_CHANGED, wxColourPickerEventHandler( ColorPanel::OnObjectColorChanged ));
	Connect( CD_HOUSE_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ColorPanel::OnButtonHouseBrush ));

	// bar diagrams
	Connect( wxID_ANY, wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( ColorPanel::OnBarSpin ));
	Connect( wxID_ANY, wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( ColorPanel::OnBarCheckBox ));
	Connect( CD_BAR_OBJECT, wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( ColorPanel::OnBarObjectChoice ));

	Connect( CD_BAR_PEN, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ColorPanel::OnButtonBarPen ));
	Connect( CD_BAR_BRUSH, wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( ColorPanel::OnButtonBarBrush ));
}

/*****************************************************
**
**   ColorPanel   ---   Destructor
**
******************************************************/
ColorPanel::~ColorPanel()
{
	config->viewprefs->configDialogColorPanelPage = notebook_main->GetSelection();

	delete expert;
	delete horoscope;

	delete colorConfig;
	delete objectColors;
	delete barconfig;
	delete props;
}

/*****************************************************
**
**   ColorPanel   ---   config2model
**
******************************************************/
void ColorPanel::config2model()
{
	*colorConfig = *config->colors;
	*objectColors = *config->chartColors;
	*barconfig = *config->bardiagram;
}

/*****************************************************
**
**   ColorPanel   ---   model2config
**
******************************************************/
void ColorPanel::model2config()
{
	wxString color = colorConfig->fgColor.GetAsString(  wxC2S_CSS_SYNTAX );
	*config->colors = *colorConfig;
	*config->chartColors = *objectColors;
	*config->bardiagram = *barconfig;
}

/*****************************************************
**
**   ColorPanel   ---   updateUi
**
******************************************************/
void ColorPanel::updateUi()
{
	updateUiObjectColors();
	updateUiBarDiagram();
}

/*****************************************************
**
**   ColorPanel   ---   OnGlobalColorChanged
**
******************************************************/
void ColorPanel::OnGlobalColorChanged( wxColourPickerEvent &event )
{
	//printf( "ColorPanel::OnGlobalColorChanged\n" );
	TransferDataFromWindow();
	updateUi();
	setDirty();
	event.Skip();
}

/*****************************************************
**
**   ColorPanel   ---   updateUiObjectColors
***
******************************************************/
void ColorPanel::updateUiObjectColors()
{
	// planets
	const int selplanet = planetChoiceToIndex( choice_planets->GetSelection());
	assert( selplanet >= 0 && selplanet < MAX_EPHEM_OBJECTS - 2 );
	color_fgplanets->SetColour( objectColors->planetFgColor[selplanet] );
	preview_planetcolor->item = selplanet;
	preview_planetcolor->fgcolor = objectColors->planetFgColor[selplanet];
	preview_planetcolor->brush = MBrush( colorConfig->bgColor );
	preview_planetcolor->Refresh();

	// signs
	const int selsign = choice_signs->GetSelection();
	assert( selsign >= 0 && selsign < 4 );
	color_fgsigns->SetColour( objectColors->signFgColor[selsign] );

	preview_signfgcolor->item = selsign;
	preview_signfgcolor->fgcolor = objectColors->signFgColor[selsign];
	preview_signfgcolor->brush = MBrush( colorConfig->bgColor );
	preview_signfgcolor->Refresh();

	preview_signbgcolor->item = selsign;
	preview_signbgcolor->fgcolor = colorConfig->fgColor;
	preview_signbgcolor->brush = objectColors->signPalette.brushes[selsign];
	preview_signbgcolor->Refresh();

	// houses
	const int selhouse = choice_houses->GetSelection();
	assert( selhouse >= 0 && selhouse < 4 );
	color_fghouses->SetColour( objectColors->houseFgColor[selhouse] );

	preview_housefgcolor->item = selhouse;
	preview_housefgcolor->brush = MBrush( colorConfig->bgColor );
	preview_housefgcolor->fgcolor = objectColors->houseFgColor[selhouse];
	preview_housefgcolor->Refresh();

	preview_housebgcolor->item = selhouse;
	preview_housebgcolor->fgcolor = colorConfig->fgColor;
	preview_housebgcolor->brush = objectColors->housePalette.brushes[selhouse];
	preview_housebgcolor->Refresh();
}

/*****************************************************
**
**   ColorPanel   ---   OnObjectColorChanged
***
******************************************************/
void ColorPanel::OnObjectColorChanged( wxColourPickerEvent& )
{
	const int selplanet = choice_planets->GetSelection();
	assert( selplanet >= 0 && selplanet < MAX_EPHEM_OBJECTS - 2 );
	const int pindex = planetChoiceToIndex( selplanet );
	objectColors->planetFgColor[pindex] = color_fgplanets->GetColour();

	// nodes are pair each
	if ( pindex == OTRUENODE )
	{
		objectColors->planetFgColor[OMEANNODE] = color_fgplanets->GetColour();
	}
	else if ( pindex == OTRUEDESCNODE )
	{
		objectColors->planetFgColor[OMEANDESCNODE] = color_fgplanets->GetColour();
	}

	const int selsign = choice_signs->GetSelection();
	assert( selsign >= 0 && selsign < 4 );
	objectColors->signFgColor[selsign] = color_fgsigns->GetColour();

	const int selhouse = choice_houses->GetSelection();
	assert( selhouse >= 0 && selhouse < 4 );
	objectColors->houseFgColor[selhouse] = color_fghouses->GetColour();

	updateUiObjectColors();
	setDirty();
}

/*****************************************************
**
**   ColorPanel   ---   OnObjectChoice
***
******************************************************/
void ColorPanel::OnObjectChoice( wxCommandEvent& )
{
	updateUiObjectColors();
}

/*****************************************************
**
**   ColorPanel   ---   OnButtonSignBrush
**
******************************************************/
void ColorPanel::OnButtonSignBrush( wxCommandEvent& )
{
	//printf( "ColorPanel::OnButtonSignBrush\n" );
	const int selsign = choice_signs->GetSelection();
	assert( selsign >= 0 && selsign < 4 );
	BrushDialog dialog( this, objectColors->signPalette.brushes[selsign] );
	if ( dialog.ShowModal() == wxID_OK )
	{
		objectColors->signPalette.brushes[selsign] = dialog.getBrush();
		setDirty();
		updateUiObjectColors();
	}
}

/*****************************************************
**
**   ColorPanel   ---   OnButtonHouseBrush
**
******************************************************/
void ColorPanel::OnButtonHouseBrush( wxCommandEvent& )
{
	//printf( "ColorPanel::OnButtonHouseBrush\n" );
	const int selhouse = choice_houses->GetSelection();
	assert( selhouse >= 0 && selhouse < 4 );
	BrushDialog dialog( this, objectColors->housePalette.brushes[selhouse] );
	if ( dialog.ShowModal() == wxID_OK )
	{
		objectColors->housePalette.brushes[selhouse] = dialog.getBrush();
		setDirty();
		updateUiObjectColors();
	}
}

/*****************************************************
**
**   ColorPanel   ---   planetChoiceToIndex
***
******************************************************/
int ColorPanel::planetChoiceToIndex( const int &selection )
{
	 int index = selection;

	if ( selection == OTRUENODE ) index++;
	else if ( selection >= OMEANDESCNODE ) index += 2;

	return index;
}

#define RASIDIAGRAM_XMIN 600
#define RASIDIAGRAM_XMAX 600
#define RASIDIAGRAM_RATIO .6

/*****************************************************
**
**   ColorPanel   ---   updateUiBarDiagram
**
******************************************************/
void ColorPanel::updateUiBarDiagram()
{
	spin_zoom->Enable( barconfig->doItemZoom  );
	label_pixel->Enable( barconfig->doItemZoom  );

	const int sel = choice_object->GetSelection();
	if ( sel < 0 ) return;
	assert( sel < MAX_BARDIAGRAM_COLORS );
	const bool isGraha = ( sel >= 12 );
	//printf( "ColorPanel::updateBarSheet sel %d graha %d cornerRadius %d\n", sel, isGraha, barconfig->cornerRadius );

	barwidget->clearSheet();
	barwidget->setMinXRight( 300 );
	Sheet *sheet = barwidget->getSheet();

	DasaBarPreviewItem *baritem = new DasaBarPreviewItem( props, barconfig );
	baritem->setObjectId( sel );
	sheet->addItem( baritem );
	if ( isGraha )
	{
		ObjectArray obs = PlanetList().getVedicPlanetOnlyList();
		obs.push_back( OASCENDANT );

		BarDiagramWidget *diagram = new BarDiagramWidget( props, _( "Graha Pinda" ), true, BARDIAGRAM_XMIN, BARDIAGRAM_XMAX, BARDIAGRAM_RATIO, barconfig );
		for ( unsigned i = 0; i < obs.size(); i++ )
		{
			diagram->addItem( new BarDiagramItem( obs[i], expert->getPinda( GRAHAPINDA, i )));
		}
		sheet->addItem( diagram );
	}
	else
	{
		BarDiagramWidget *diagram = new BarDiagramWidget( props, _( "Rekha" ), false, RASIDIAGRAM_XMIN, RASIDIAGRAM_XMAX, RASIDIAGRAM_RATIO, barconfig );
		for ( Rasi rasi = R_ARIES; rasi <= R_PISCES; rasi++ )
		{
			diagram->addItem( new BarDiagramItem( (int)rasi, expert->getSarva( REKHA, rasi )));
		}
		sheet->addItem( diagram );
	}
	barwidget->OnDataChanged();
}

/*****************************************************
**
**   ColorPanel   ---   OnButtonBarPen
**
******************************************************/
void ColorPanel::OnButtonBarPen( wxCommandEvent& )
{
	//printf( "PEN\n" );
	const int sel = choice_object->GetSelection();
	if ( sel < 0 ) return;
	assert( sel < MAX_BARDIAGRAM_COLORS );

	PenDialog dialog( this, barconfig->style[sel].pen, &barconfig->style[sel].brush );
	if ( dialog.ShowModal() == wxID_OK )
	{
		barconfig->style[sel].pen = *dialog.getPen();
		updateUiBarDiagram();
		setDirty();
	}
}

/*****************************************************
**
**   ColorPanel   ---   OnButtonBarBrush
**
******************************************************/
void ColorPanel::OnButtonBarBrush( wxCommandEvent& )
{
	//printf( "BRUSH\n" );
	const int sel = choice_object->GetSelection();
	if ( sel < 0 ) return;
	assert( sel < MAX_BARDIAGRAM_COLORS );

	BrushDialog dialog( this, barconfig->style[sel].brush );
	if ( dialog.ShowModal() == wxID_OK )
	{
		barconfig->style[sel].brush = dialog.getBrush();
		updateUiBarDiagram();
		setDirty();
	}
}

/*****************************************************
**
**   ColorPanel   ---   restoreDefaults
**
******************************************************/
void ColorPanel::restoreDefaults()
{
	*colorConfig = ColorConfig();
	*objectColors = ObjectColorConfig();
	*barconfig = BarDiagramConfig();
}

/*****************************************************
**
**   ColorPanel   ---   set_properties
**
******************************************************/
void ColorPanel::set_properties()
{
	Lang lang;
	choice_planets->Clear();

	for ( ObjectId i = OSUN; i < MAX_EPHEM_OBJECTS; i++ )
	{
		if ( i == OMEANNODE || i == OMEANDESCNODE ) continue;
		choice_planets->Append( lang.getObjectName( i, TF_LONG ));
	}

    // begin wxGlade: ColorPanel::set_properties
    choice_planets->SetSelection(0);
    preview_planetcolor->SetMinSize(wxSize(100, -1));
    choice_signs->SetSelection(0);
    choice_houses->SetSelection(0);
    choice_object->SetToolTip(_("Sign or Lord of the Dasa"));
    choice_object->SetSelection(0);
    barwidget->SetMinSize(wxSize(300, 300));
    // end wxGlade
}

/*****************************************************
**
**   ColorPanel   ---   do_layout
**
******************************************************/
void ColorPanel::do_layout()
{
    // begin wxGlade: ColorPanel::do_layout
    wxFlexGridSizer* sizer_main = new wxFlexGridSizer(1, 3, 0, 0);
    wxFlexGridSizer* grid_bar = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_preview_staticbox->Lower();
    wxStaticBoxSizer* sizer_preview = new wxStaticBoxSizer(sizer_preview_staticbox, wxHORIZONTAL);
    wxBoxSizer* sizer_left = new wxBoxSizer(wxVERTICAL);
    sizer_general_staticbox->Lower();
    wxStaticBoxSizer* sizer_general = new wxStaticBoxSizer(sizer_general_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_bar_general = new wxFlexGridSizer(3, 3, 0, 0);
    sizer_mouseover_staticbox->Lower();
    wxStaticBoxSizer* sizer_mouseover = new wxStaticBoxSizer(sizer_mouseover_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_mouseover = new wxFlexGridSizer(1, 3, 0, 0);
    sizer_object_staticbox->Lower();
    wxStaticBoxSizer* sizer_object = new wxStaticBoxSizer(sizer_object_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_object = new wxFlexGridSizer(2, 2, 0, 0);
    wxFlexGridSizer* grid_charts = new wxFlexGridSizer(3, 1, 0, 0);
    sizer_houses_staticbox->Lower();
    wxStaticBoxSizer* sizer_houses = new wxStaticBoxSizer(sizer_houses_staticbox, wxVERTICAL);
    wxGridSizer* grid_houses = new wxGridSizer(2, 3, 0, 0);
    sizer_signs_staticbox->Lower();
    wxStaticBoxSizer* sizer_signs = new wxStaticBoxSizer(sizer_signs_staticbox, wxVERTICAL);
    wxGridSizer* grid_signs = new wxGridSizer(2, 3, 0, 0);
    sizer_planets_staticbox->Lower();
    wxStaticBoxSizer* sizer_planets = new wxStaticBoxSizer(sizer_planets_staticbox, wxVERTICAL);
    wxBoxSizer* sizer_planet_fg = new wxBoxSizer(wxHORIZONTAL);
    wxFlexGridSizer* grid_general = new wxFlexGridSizer(1, 2, 0, 0);
    sizer_bgcolors_staticbox->Lower();
    wxStaticBoxSizer* sizer_bgcolors = new wxStaticBoxSizer(sizer_bgcolors_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_bgcolors = new wxFlexGridSizer(4, 2, 3, 3);
    wxBoxSizer* sizer_colors = new wxBoxSizer(wxVERTICAL);
    sizer_fgcolors_staticbox->Lower();
    wxStaticBoxSizer* sizer_fgcolors = new wxStaticBoxSizer(sizer_fgcolors_staticbox, wxVERTICAL);
    wxFlexGridSizer* grid_fgcolors = new wxFlexGridSizer(4, 2, 3, 3);
    grid_fgcolors->Add(label_fg, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_fg, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->Add(label_benefic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_benefic, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->Add(label_malefic, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_malefic, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->Add(label_transit, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_fgcolors->Add(button_color_transit, 1, wxALL|wxEXPAND, 3);
    grid_fgcolors->AddGrowableCol(1);
    sizer_fgcolors->Add(grid_fgcolors, 1, wxALL|wxALIGN_RIGHT, 3);
    sizer_colors->Add(sizer_fgcolors, 0, wxALL|wxEXPAND, 3);
    grid_general->Add(sizer_colors, 1, wxALL, 3);
    grid_bgcolors->Add(label_bg, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_bg, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->Add(label_marked, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_marked, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->Add(label_chart_markup, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_chart_markup, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->Add(label_error, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_bgcolors->Add(button_color_error, 1, wxALL|wxEXPAND, 3);
    grid_bgcolors->AddGrowableCol(1);
    sizer_bgcolors->Add(grid_bgcolors, 1, wxALL, 3);
    grid_general->Add(sizer_bgcolors, 0, wxALL|wxEXPAND, 3);
    notebook_pane_general->SetSizer(grid_general);
    sizer_planets->Add(choice_planets, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_planet_fg->Add(label_fgplanets, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_planet_fg->Add(color_fgplanets, 1, wxALL|wxEXPAND, 3);
    sizer_planet_fg->Add(preview_planetcolor, 1, wxALL|wxEXPAND, 3);
    sizer_planets->Add(sizer_planet_fg, 1, wxEXPAND, 0);
    grid_charts->Add(sizer_planets, 0, wxEXPAND, 0);
    sizer_signs->Add(choice_signs, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_signs->Add(label_fg_signs, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_signs->Add(color_fgsigns, 1, wxALL|wxEXPAND, 3);
    grid_signs->Add(preview_signfgcolor, 1, wxALL|wxEXPAND, 3);
    grid_signs->Add(label_bg_signs, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_signs->Add(button_signbrush, 0, wxALL, 3);
    grid_signs->Add(preview_signbgcolor, 1, wxALL|wxEXPAND, 3);
    sizer_signs->Add(grid_signs, 1, wxEXPAND, 0);
    grid_charts->Add(sizer_signs, 1, wxEXPAND, 0);
    sizer_houses->Add(choice_houses, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_houses->Add(label_fg_houses, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_houses->Add(color_fghouses, 1, wxALL|wxEXPAND, 3);
    grid_houses->Add(preview_housefgcolor, 1, wxALL|wxEXPAND, 3);
    grid_houses->Add(label_bg_houses, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_houses->Add(button_housebrush, 0, wxALL, 3);
    grid_houses->Add(preview_housebgcolor, 1, wxALL|wxEXPAND, 3);
    sizer_houses->Add(grid_houses, 1, wxEXPAND, 0);
    grid_charts->Add(sizer_houses, 1, wxEXPAND, 0);
    notebook_pane_charts->SetSizer(grid_charts);
    grid_object->Add(label_2, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_object->Add(choice_object, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 3);
    grid_object->Add(button_pen, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    grid_object->Add(button_brush, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 3);
    sizer_object->Add(grid_object, 1, wxEXPAND, 0);
    sizer_left->Add(sizer_object, 1, wxALL, 3);
    grid_mouseover->Add(check_zoom, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    grid_mouseover->Add(spin_zoom, 0, wxALL, 3);
    grid_mouseover->Add(label_pixel, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_mouseover->Add(grid_mouseover, 1, 0, 0);
    sizer_left->Add(sizer_mouseover, 1, 0, 0);
    sizer_general->Add(check_showtoolbarmsg, 0, wxALL, 3);
    grid_bar_general->Add(label_cornerradius, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 3);
    grid_bar_general->Add(spin_cornerradius, 0, wxALL, 3);
    grid_bar_general->Add(label_pixel2, 0, wxALL|wxALIGN_CENTER_VERTICAL, 3);
    sizer_general->Add(grid_bar_general, 1, 0, 0);
    sizer_left->Add(sizer_general, 1, 0, 0);
    grid_bar->Add(sizer_left, 1, wxALL, 3);
    sizer_preview->Add(barwidget, 1, wxALL|wxEXPAND, 3);
    grid_bar->Add(sizer_preview, 1, wxALL|wxEXPAND, 3);
    notebook_pane_bar->SetSizer(grid_bar);
    grid_bar->AddGrowableRow(0);
    grid_bar->AddGrowableCol(1);
    notebook_main->AddPage(notebook_pane_general, _("General"));
    notebook_main->AddPage(notebook_pane_charts, _("Charts"));
    notebook_main->AddPage(notebook_pane_bar, _("Bar Diagrams"));
    sizer_main->Add(notebook_main, 1, wxALL|wxEXPAND, 3);
    SetSizer(sizer_main);
    sizer_main->Fit(this);
    sizer_main->AddGrowableRow(0);
    sizer_main->AddGrowableCol(0);
    // end wxGlade
}

/*****************************************************
**
**   ConfigPanelFactory   ---   createColorPanel
**
******************************************************/
ConfigPanel *ConfigPanelFactory::createColorPanel( wxWindow *parent )
{
	return new ColorPanel( parent );
}

