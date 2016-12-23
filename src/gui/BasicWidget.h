/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/gui/BasicWidget.h
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


#ifndef _BASICWIDGET_H_
#define _BASICWIDGET_H_

#include <wx/scrolwin.h>
#include "Commands.h"
#include "maitreya.h"

class ChartProperties;
class Painter;
class wxWindow;

BEGIN_DECLARE_EVENT_TYPES()
DECLARE_EVENT_TYPE( COMMAND_CHART_SKIN_CHANGED, wxID_HIGHEST + 6001 )
END_DECLARE_EVENT_TYPES()

// widget options
#define WO_NONE                          0x00000000
#define WO_SUPPORTS_EW_TOGGLE            0x00000001
#define WO_IS_MAIN_VIEW                  0x00000002
#define WO_MENU_FULL_GRAPHIC_STYLE       0x00000004
#define WO_MENU_RESTRICTED_GRAPHIC_STYLE 0x00000008
#define WO_MENU_FULL_OBJECT              0x00000010
#define WO_MENU_RESTRICTED_OBJECT        0x00000020
#define WO_MENU_SBC_STYLE                0x00000040
#define WO_MENU_SKIN                     0x00000080
#define WO_MENU_TRANSIT                  0x00000100
#define WO_MENU_EDIT_CHARTDATA           0x00000200

#define WO_EXPORT_GRAPHIC                0x00001000
#define WO_EXPORT_PDF                    0x00002000
#define WO_EXPORT_PLAINTEXT              0x00004000
#define WO_EXPORT_CSVTEXT                0x00008000
#define WO_EXPORT_HTMLTEXT               0x00010000
#define WO_EXPORT_ALL WO_EXPORT_GRAPHIC | WO_EXPORT_PDF | WO_EXPORT_PLAINTEXT | WO_EXPORT_HTMLTEXT | WO_EXPORT_CSVTEXT


/*************************************************//**
*
* \brief Basic abstract class for graphical display based upon wxScrolledWindow
*
******************************************************/
class BasicWidget :  public wxScrolledWindow, public KeyEventObserver
{
	DECLARE_CLASS( BasicWidget )

public:
	BasicWidget( wxWindow *parent, ChartProperties*, wxWindowID id = -1,
		const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );
	~BasicWidget();

	virtual void OnDataChanged();

	virtual int getWidgetOptions() { return widgetOptions; }
	void setWidgetOptions( const int &a ) { widgetOptions = a; }
	void addWidgetOption( const int &a ) { widgetOptions |= a; }

	virtual void exportAs( const WidgetExportType& );
	void doImageExport();

	virtual bool isVedic() const;
	void setFixedVedic( const bool = true );
	void setFixedWestern( const bool = true );

	ChartProperties *getChartProperties() const { return chartprops; }

protected:

	ChartProperties *chartprops;
	Painter *painter;
	int hborder, vborder;

	virtual void doPaint( const wxRect&, const bool eraseBackground = true ) = 0;

	// Mouse
	void OnMouseWheelEvent( wxMouseEvent& );
	void OnMouseLeave( wxMouseEvent& );
	void OnMouseEnter( wxMouseEvent& );
	void OnMouseLeftDown( wxMouseEvent& );
	void OnMouseLeftUp( wxMouseEvent& );
	virtual void HandleMouseWheelEvent( wxMouseEvent& );
	virtual void onNavigationKeyCommand( wxKeyEvent& );

	void OnKeyDown( wxKeyEvent& );
	void OnChar( wxKeyEvent &event );
	void OnSize( wxSizeEvent& );
	void OnMenuCommand( wxCommandEvent& );
	void OnIdle( wxIdleEvent& );

	virtual void mouseHasMoved( const bool &outside );

	bool dragMode, exportMode;

	wxPoint mousePosition;
	wxPoint unscrolledMousePosition;
	bool mouseInside;
	int keyMod;

	void echo( wxString );

private:

	bool fixedVedic;
	bool fixedWestern;
	int widgetOptions;

	void OnPaint( wxPaintEvent& );
	void OnCommand( wxCommandEvent& );
	void OnContextMenuEvent( wxMouseEvent& );
};

/*************************************************//**
*
* \brief Empty widget (used on error)
*
******************************************************/
class EmptyWidget : public BasicWidget
{
public:
	EmptyWidget( wxWindow *parent, ChartProperties* );

protected:

	virtual void doPaint( const wxRect&, const bool eraseBackground = true );
};

#endif

