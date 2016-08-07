/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Conf.h
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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <wx/colour.h>
#include <wx/gdicmn.h>
#include <wx/string.h>
#include <list>
#include <map>

#include "AspectConfig.h"
#include "ConfigBase.h"
#include "maitreya.h"
#include "FontProvider.h"

using namespace std;

class wxJSONValue;

class ObjectColorConfig;
class ColorConfig;
class BarDiagramConfig;
class Location;
class PdfDocumentConfig;
class TableStyleConfig;
class VedicChartBehaviorConfig;
class WesternChartBehaviorConfig;

/*************************************************//**
*
* 
*
******************************************************/
class WriterConfig : public ConfigBase
{
public:
  WriterConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

  bool vedicSignNames;
  bool vedicPlanetNames;
  bool vedicPositions;
  bool signSymbols;
  bool planetSymbols;
  int uranusSymbol;
  int plutoSymbol;
  int capricornSymbol;
};

/*************************************************//**
*
* 
*
******************************************************/
class WesternCalculationConfig : public ConfigBase
{
public:
	WesternCalculationConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	AYANAMSA ayanamsa;
	HOUSE_SYSTEM houseSystem;
	int lunarNodeMode;
	int yearLength;
	double customYearLength;
};

/*************************************************//**
*
* 
*
******************************************************/
class WesternConfig : public ConfigBase
{
public:
	WesternConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	OBJECT_INCLUDES objects;
	TAB_COLUMN_TYPE columnStyle;
	ObjectArray arabicParts;
	int planetOrder;
};

/*************************************************//**
*
* 
*
******************************************************/
class UranianConfig : public ConfigBase
{
public:
	UranianConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	GRADKREIS gradkreis;
	ASPECT_SORTORDER sortOrder;
	ObjectFilter objectFilter;

	bool eventsIncludeMidpoints;
	bool eventsIncludeSums;
	bool eventsIncludeDifferences;
	bool eventsIncludeTriples;

	bool radixIncludeAntiscia;
	double orbisRadix;

	int transitIncludeType;
	ObjectArray transitIncludedObjects;
	bool transitIncludeAntiscia;
	bool transitIncludeRunningPairs;
	double orbisTransit;

	bool solararcIncludeAntiscia;
	bool solararcIncludeRunningPairs;
	double orbisSolararc;

	int directionIncludeType;
	ObjectArray directionIncludedObjects;
	bool directionIncludeAntiscia;
	bool directionIncludeRunningPairs;
	double orbisDirection;

	bool partnerIncludeAntiscia;
	double orbisPartner;
};

/*************************************************//**
*
* 
*
******************************************************/
class WesternChartDisplayConfig : public ConfigBase
{
public:
	WesternChartDisplayConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );
	
	bool showHouses;
	bool showAspects;
	bool showRetro;
	bool showPlanetColors;
	bool showSignColors;
	bool showHouseColors;
	bool showAspectColors;
	bool showAspectSymbols;
	int transitStyle;
	int chartOrientation;
	int houseNumberStyle;
	int graphicSkin;
};

/*************************************************//**
*
* 
*
******************************************************/
class VedicConfig : public ConfigBase
{
public:
	VedicConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	OBJECT_INCLUDES objects;
	int orderLagna;
	ObjectArray arabicParts;
	TAB_COLUMN_TYPE columnStyle;
};

/*************************************************//**
*
* 
*
******************************************************/
class KalaVelaConfig : public ConfigBase
{
public:
	KalaVelaConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	vector<int> lord;
	vector<int> portion;
};

/*************************************************//**
*
* 
*
******************************************************/
class VedicCalculationConfig : public ConfigBase
{
public:
	VedicCalculationConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	AYANAMSA ayanamsa;
	int lunarNodeMode;
	HOUSE_SYSTEM houseSystem;
	bool houseUseCusps;

	int ashtakavargaMode;
	int ashtakavargaSarva;
	int ashtakavargaNumberMode;
	int ashtakavargaSodhyaPindaMode;

	int moonBenefic;
	int mercuryBenefic;

	KalaVelaConfig kalavela;
	vector<int> kalavelaLord;
	vector<int> kalavelaPortion;

	int arudhaMode;
	int charaKarakaMode;

	int tempFriendMode;
	int vargaHoraMode;
	int vargaDrekkanaMode;
	int vargaChaturtamsaMode;
	vector<wxString> vargaSignifications;

	int nakshatraPortionMode;
	int kalachakraMode;
	int yoginiDasaLordDisplayType;
	int dasaShowDateMode;

	int yearLength;
	double customYearLength;
};

/*************************************************//**
*
* 
*
******************************************************/
class EphemConfig : public ConfigBase
{
public:
	EphemConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	int useEphemerisTime;
	int ephemCoordinateSystem;
	int useTruePositions;
	double custom_t0;
	double custom_ayan_t0;
	double custom_aya_period;
	bool custom_aya_constant;
	bool sunrise_refrac;
	int sunrise_def;
	bool showEphemWarning;

	wxString sePath;
};

/*************************************************//**
*
* 
*
******************************************************/
class VedicChartDisplayConfig : public ConfigBase
{
public:
	VedicChartDisplayConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	bool showArudhas;
	bool showAshtakavarga;
	bool showRetro;
	bool showPlanetColors;

	int indianChartType;
	int northIndianSignDisplayType;
	int southIndianAscendantMarkup;
	int centerInfoType;

	bool showSbcSanskritSymbols;
	bool showSbcNakshatraQuality;

	int graphicSkin;
};

/*************************************************//**
*
* 
*
******************************************************/
class ViewSizeConfig : public ConfigBase
{
public:
	ViewSizeConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxSize sMasterWindow;
	wxSize sMultipleView;
	wxSize sMainWindow;
	wxSize sVedicGraphicWindow;
	wxSize sWesternGraphicWindow;

	// TODO currently not supported because view id is the same as graphic view
	wxSize sUranianGraphicWindow;

	wxSize sTextWindow;
	wxSize sDasaTreeWindow;
	wxSize sDasaCompositeWindow;
	wxSize sGraphicalDasaWindow;
	wxSize sSbcWindow;
	wxSize sSolarWindow;
	wxSize sUranianWindow;
	wxSize sTransitWindow;
	wxSize sPrintPreviewWindow;
	wxSize sYogaWindow;
	wxSize sVargaSummaryWindow;
	wxSize sShadbalaWindow;
	wxSize sAshtakaVargaWindow;
	wxSize sEphemWindow;
	wxSize sEclipseWindow;
	wxSize sHoraWindow;
	wxSize sPartnerWindow;
	wxSize sAdditionalLogWindow;

	wxSize sConfigDialog;
	wxSize sSimpleConfigDialog;
	wxSize sYogaEditor;
	wxSize sDataDialog;
	wxSize sAtlasDialog;
	wxSize sObjectFilterDialog;
	wxSize sPrintoutSelectionDialog;
};

/*************************************************//**
*
* 
*
******************************************************/
class ViewPreferencesConfig : public ConfigBase
{
public:
	ViewPreferencesConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	ViewSizeConfig sizes;

	wxSize pMasterWindow;
	wxSize pAdditionalLogWindow;
	int sashMasterWindowLog;

	int sashPartnerDouble;
	int sashPartnerComposite;
	int sashSolar;
	int sashTransit;
	
	bool doSplitDasaTree;
	int dasaTreeTextViewWidth;
	int sashDasaTree;

	// active notebook pages
	bool configDialogWChartPreviewTransits;
	bool configDialogVChartPreviewTransits;

	int configDialogWChartPanelPage;
	int configDialogVChartPanelPage;
	int configDialogVCalculationPanelPage;
	int configDialogSheetStylePanelPage;
	int configDialogColorPanelPage;

	int activePagePartner;
	int activePageTransit;
	int activePageAshtakaVarga;
	int activePageVargaOverview;
	int activePageShadbala;
	int textWindowPage;
	int activePageUranian;
	int uranianWindowPage;
	int partnerStyle;
	int configDialogActivePage;

	// show warning dialogs
	bool doNotShowAgainAfterLanguageChange;
	bool doNotShowAgainEphemConfigChange;

	// Main window options
	bool showStatusBar;
	int listStyle;
	bool showMainToolbar;

	// graphical dasa
	int antarDasaLevelChoice;
	int graphicalDasaWidgetZoom;
	vector<int> graphicalDasaList;

	// Yoga view
	int yogaSourceFilter;
	int yogaGroupFilter;
	int yogaCustomFilter;

	TRANSIT_MODE transitmode;
	int orbisRadix;
	int orbisTransit;
	int orbisPartner;
	int gradkreis;
	int transitTimezone;

	// Ephem view
	int ephemTimezone;
	int ephemMode;
	int ephemCircleType;


	// remember paths
	vector<wxString> recentFiles;
	wxString defOpenPath;
	wxString defSavePath;
	wxString defExportPath;
	wxString defPdfSavePath;
	wxString lastYogaConfigFile;
};

/*************************************************//**
*
* 
*
******************************************************/
class ToolbarConfig : public ConfigBase
{
public:
	ToolbarConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	vector<int> mainToolbarItems;
	int toolbarStyle;
	int toolbarIconSize;
};

/*************************************************//**
*
* 
*
******************************************************/
class ViewConfig : public ConfigBase
{
public:
	ViewConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	bool showStatusInfo;
	int maxRecentFiles;
	int childViewFrameWidth;
	bool showTextViewHeaders;

	int sheetStyle;

	int defOpenFiletype;
	int defGraphicExportType;
	bool exportAskOnOverwrite;
	int graphicExportSizeMode;
	int logMode;
	int logLevel;
	int inputFieldValidation;

	int backupFileMode;
};

/*************************************************//**
*
* 
*
******************************************************/
class FontConfigEntries : public ConfigBase
{
public:

	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxString htmlDefault;
	wxString graphicDefault;
	wxString graphicSymbols;
	wxString textDefault;
	wxString textHeader;
	wxString textTableContents;
	wxString textTableHeader;
	wxString pdfDefault;
	wxString pdfHeader;
	wxString pdfTableContents;
	wxString pdfTableHeader;
	wxString pdfTiny;
	wxString plainDefault;
};

/*************************************************//**
*
* 
*
******************************************************/
class AtlasConfig : public ConfigBase
{
public:
	AtlasConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	wxString databaseFile;
	wxString sqlFile;
	vector<wxString> favouriteCountries;
	int filterMode;
	wxString lastSelectedCountry;
	vector<int> gridColumnSizes;
	bool filterCaseSensitive;
	int cacheSize;
};

/*************************************************//**
*
* 
*
******************************************************/
class AnimationConfig : public ConfigBase
{
public:
	AnimationConfig();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	int interval;
	int mode;
	int stepLength;
	int stepDimension;
};

/*************************************************//**
*
* 
*
******************************************************/
class MultipleViewConfiguration : public ConfigBase
{
public:
	MultipleViewConfiguration();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	bool useMultipleViews;
	int defaultView;
	int notebookStyle;
	int notebookOrientation;
};

/*************************************************//**
*
* \brief contains all configurations of the application
*
******************************************************/
class Config : public ConfigBase
{
public:
	Config();
	~Config();
	virtual void load( wxJSONValue&, wxString = wxEmptyString );
	virtual void save( wxJSONValue&, wxString = wxEmptyString );

	void readAll();
	void writeAll();

	WriterConfig *writer;

	WesternConfig *western;
	WesternCalculationConfig *westernCalculation;
	WesternChartDisplayConfig *westernChart;

	UranianConfig *uranian;

	vector<AspectConfig> aspects;

	VedicConfig *vedic;
	VedicCalculationConfig *vedicCalculation;

	EphemConfig *ephem;

	ViewPreferencesConfig *viewprefs;
	ViewConfig *view;

	FontConfig font[MAX_FONTS];
	AtlasConfig *atlas;
	AnimationConfig *animation;
	BarDiagramConfig *bardiagram;
	ToolbarConfig *toolbar;

	// Vedic Chart
	VedicChartDisplayConfig *vedicChart;
	VedicChartBehaviorConfig *vedicChartBehavior;
	WesternChartBehaviorConfig *westernChartBehavior;

	Location *defaultLocation;

	ColorConfig *colors;
	ObjectColorConfig *chartColors;

	PdfDocumentConfig *print;
	MultipleViewConfiguration *multipleView;

	bool preferVedic;
	bool openNewDocOnStart;
	bool askOnQuit;
	wxString lang;
	wxString langList;

private:


};

#endif

