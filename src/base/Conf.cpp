/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/Conf.cpp
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

#include "Conf.h"

#include "ColorConfig.h"
#include "DataSet.h"
#include "FileConfig.h"
#include "JSonTool.h"
#include "PrintoutConfigBase.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"

#include <wx/filename.h>
#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>
#include <wx/log.h>
#include <wx/wfstream.h>

//#define DEBUG_CONFIG_FILE_OPERATIONS

/*****************************************************
**
**   ConfigBase   ---   reportMissingKey_legacy
**
******************************************************/
void ConfigBase::reportMissingKey( wxString cname, wxString attname )
{
	wxString message;
	message << wxT( "config value for " ) << cname << wxT( "/" ) << attname << wxT( " not found" );
	wxLogWarning( message );
}

/*****************************************************
**
**   WriterConfig   ---   Constructor
**
******************************************************/
WriterConfig::WriterConfig()
{
  vedicSignNames = vedicPlanetNames = vedicPositions = signSymbols = planetSymbols = false;
	uranusSymbol = plutoSymbol = capricornSymbol = 0;
}

/*****************************************************
**
**   WesternConfig   ---   Constructor
**
******************************************************/
WesternConfig::WesternConfig()
{
	objects = OI_INNER | OI_OUTER | OI_DRAGONHEAD | OI_ASCENDANT | OI_MERIDIAN;
	columnStyle = TAB_CT_OBJECT_NAME | TAB_CT_LONGITUDE | TAB_CT_ELEMENT | TAB_CT_SIGN_QUALITY | TAB_CT_HOUSE_POSITION;
	planetOrder = 0;
}

/*****************************************************
**
**   UranianConfig   ---   Constructor
**
******************************************************/
UranianConfig::UranianConfig()
{
	sortOrder = AS_RPLANET;
	gradkreis = GK_360;

	eventsIncludeMidpoints = true;
	eventsIncludeSums = false;
	eventsIncludeDifferences = false;
	eventsIncludeReflectionPoints = false;
	eventsIncludeTriples = false;
	eventsIncludeAntiscia = false;

	orbisRadix = 1.5;
	orbisTransit = .5;
	orbisPartner = 1.5;
};

/*****************************************************
**
**   WesternChartDisplayConfig   ---   Constructor
**
******************************************************/
WesternChartDisplayConfig::WesternChartDisplayConfig()
{
	showHouses = true;
	showAspects = true;
	showRetro = true;
	showPlanetColors = true;
	showSignColors = true;
	showHouseColors = true;
	showAspectColors = true;
	showAspectSymbols = false;
	secondchartStyle = 0;
	chartOrientation = 0;
	houseNumberStyle = 2;
	graphicSkin = 0;
}

/*****************************************************
**
**   WesternCalculationConfig   ---   Constructor
**
******************************************************/
WesternCalculationConfig::WesternCalculationConfig()
{
	ayanamsa = AY_NONE;
	houseSystem = HS_PLACIDUS;
	lunarNodeMode = LUNAR_NODE_TRUE;
	yearLength = 0;
	customYearLength = WCUSTOM_YEAR_LEN;
}

/*****************************************************
**
**   VedicConfig   ---   Constructor
**
******************************************************/
VedicConfig::VedicConfig()
{
	objects = OI_INNER | OI_DRAGONTAIL | OI_DRAGONHEAD | OI_ASCENDANT;
	orderLagna = 0;
	columnStyle = TAB_CT_OBJECT_NAME | TAB_CT_LONGITUDE | TAB_CT_NAVAMSA | TAB_CT_NAKSHATRA;
}

/*****************************************************
**
**   KalaVelaConfig   ---   Constructor
**
******************************************************/
KalaVelaConfig::KalaVelaConfig()
{
	const static int c_kalavela_lord[6] = { KALAVELA_LORD1, KALAVELA_LORD3, KALAVELA_LORD4,
		KALAVELA_LORD5, KALAVELA_LORD7, KALAVELA_LORD_NONE };
	for( int i = 0; i < 6; i++ )
	{
		lord.push_back( c_kalavela_lord[i] );
		portion.push_back( KALAVELA_PORTION_START );
	}
}

/*****************************************************
**
**   VedicCalculationConfig   ---   Constructor
**
******************************************************/
VedicCalculationConfig::VedicCalculationConfig()
{
	ayanamsa = AY_LAHIRI;
	lunarNodeMode = LUNAR_NODE_MEAN;
	houseSystem = HS_PLACIDUS;
	houseUseCusps = true;

	ashtakavargaMode = 0;
	ashtakavargaSarva = 0;
	ashtakavargaNumberMode = 0;

	// 0 = Parasara 1 = Mantreswar
	ashtakavargaSodhyaPindaMode = 0;

	moonBenefic = 1;
	mercuryBenefic = 1;

	arudhaMode = 0;
	charaKarakaMode = 0;

	const static int c_kalavela_lord[6] = { KALAVELA_LORD1, KALAVELA_LORD3, KALAVELA_LORD4,
		KALAVELA_LORD5, KALAVELA_LORD7, KALAVELA_LORD_NONE };
	for( int i = 0; i < 6; i++ )
	{
		kalavelaLord.push_back( c_kalavela_lord[i] );
		kalavelaPortion.push_back( KALAVELA_PORTION_START );
	}

	tempFriendMode = 0; // 0 = Rasi, 1 = Varga
	vargaHoraMode = 0;
	vargaDrekkanaMode = 0;
	vargaChaturtamsaMode = 0;

	nakshatraPortionMode = 0;
	kalachakraMode = 0;
	yoginiDasaLordDisplayType = 2;
	dasaShowDateMode = 0; // 0 = startdate 1 = end date

	yearLength = 0;
	customYearLength = ICUSTOM_YEAR_LEN;
}

/*****************************************************
**
**   EphemConfig   ---   Constructor
**
******************************************************/
EphemConfig::EphemConfig()
{
	useEphemerisTime = 0;
	ephemCoordinateSystem = 0;
	useTruePositions = 0;
	custom_t0 = DEFAULT_T0;
	custom_ayan_t0 = DEFAULT_AYAN_T0;
	custom_aya_period = DEFAULT_AYA_PERIOD;
	custom_aya_constant = false;

#if ! defined __WXMSW__ && ! defined __WXMAC__
	sePath = FileConfig::get()->getEphemDir();
#endif

	sunrise_refrac = false;
	sunrise_def = 0;
	showEphemWarning = true;
}

/*****************************************************
**
**   VedicChartDisplayConfig   ---   Constructor
**
******************************************************/
VedicChartDisplayConfig::VedicChartDisplayConfig()
{
	showArudhas = false;
	showAshtakavarga = false;
	showRetro = true;
	showPlanetColors = true;

	indianChartType = VCT_SOUTH;
	centerInfoType = VCC_NAME_SIGNIFICATION;
	northIndianSignDisplayType = VCN_NUMBER;
	southIndianAscendantMarkup = 0;

	showSbcSanskritSymbols = false;
	showSbcNakshatraQuality = true;
	graphicSkin = 0;
}

/*****************************************************
**
**   ViewPreferencesConfig   ---   Constructor
**
******************************************************/
ViewPreferencesConfig::ViewPreferencesConfig()
{
	pMasterWindow = wxSize( 50, 50 );
	sashMasterWindowLog = 150;

	sashPartnerDouble = 250;
	sashPartnerComposite = 250;
	sashSolar = 250;
	sashTransit = 250;

	doSplitDasaTree = true;
	dasaTreeTextViewWidth = 300;
	sashDasaTree = 400;

	// active notebook pages
	configDialogWChartPreviewTransits = 0;
	configDialogVChartPreviewTransits = 0;
	configDialogWChartPanelPage = 0;
	configDialogVChartPanelPage = 0;
	configDialogVCalculationPanelPage = 0;
	configDialogColorPanelPage = 0;

	activePagePartner= 0;
	activePageTransit = 0;
	activePageAshtakaVarga = 0;
	activePageVargaOverview = 0;
	activePageShadbala = 0;
	textWindowPage = 0;
	activePageUranian = 0;
	uranianWindowPage = 0;
	partnerStyle = 0;
	configDialogActivePage = 0;

	// show warning dialogs
	doNotShowAgainAfterLanguageChange = false;
	doNotShowAgainEphemConfigChange = false;

	// Main window options
	listStyle = 0;
	showMainToolbar = true;
	showStatusBar = true;

	// graphical dasa
	antarDasaLevelChoice = 1;
	graphicalDasaWidgetZoom = 100;
	const int defaultDasaList[3] = { 0, 1, 2 };
	for( uint i = 0; i < 3; i++ ) graphicalDasaList.push_back( defaultDasaList[i] );

	// Yoga view
	yogaSourceFilter = 0;
	yogaGroupFilter = 0;
	yogaCustomFilter = 0;

	// transit view
	transitmode = PcTransit;
	transitTimezone = 0;
	transitJD = 0;

	// Ephem view
	ephemTimezone = 0;
	ephemMode = 0;
	ephemCircleType = 0;
};

/*****************************************************
**
**   ToolbarConfig   ---   Constructor
**
******************************************************/
ToolbarConfig::ToolbarConfig()
{
	// valid values are text - icon - text and icon
	toolbarStyle = 1; // text and Icons
}

/*****************************************************
**
**   ViewConfig   ---   Constructor
**
******************************************************/
ViewConfig::ViewConfig()
{
	showStatusInfo = true;
	maxRecentFiles = 12;
	childViewFrameWidth = 0;
	showTextViewHeaders = false;
	sheetStyle = 0;

	defOpenFiletype = 0;
	defGraphicExportType = 0;
	graphicExportSizeMode = 0;
	logMode = 0;
	logLevel = 1; // see IdConverter -> wxLOG_Warning;
	inputFieldValidation = IFV_NONE;

	exportAskOnOverwrite = true;
	backupFileMode = 1;
}

/*****************************************************
**
**   ViewSizeConfig   ---   Constructor
**
******************************************************/
ViewSizeConfig::ViewSizeConfig()
{
	const int xMediumWindow =  800;
	const int yMediumWindow =  600;
	const int yyMasterWindow =  300;
	const int xBigWindow =  1000;
	const int yBigWindow =  800;
	const int xSmallWindow =  400;
	const int ySmallWindow =  400;
	const int rVedicChart = 500;
	const int rWesternChart = 800;

	// Window Sizes
	sMasterWindow = wxSize( xMediumWindow, yyMasterWindow );

	sMultipleView = wxSize( xBigWindow, yBigWindow );
	sMainWindow = wxSize( xSmallWindow, ySmallWindow );
	sVedicGraphicWindow = wxSize( rVedicChart, rVedicChart );
	sWesternGraphicWindow = wxSize( rWesternChart, rWesternChart );
	sUranianChartWindow = wxSize( rWesternChart, rWesternChart );
	sTextWindow = wxSize( xBigWindow, yBigWindow );
	sDasaTreeWindow = wxSize( xSmallWindow, yMediumWindow );
	sDasaCompositeWindow = wxSize( xMediumWindow, yMediumWindow );
	sGraphicalDasaWindow = wxSize( xBigWindow, ySmallWindow );
	sSbcWindow = wxSize( xBigWindow, yBigWindow );
	sSolarWindow = wxSize( xMediumWindow, yMediumWindow );
	sUranianWindow = wxSize( xBigWindow, yBigWindow );
	sTransitWindow = wxSize( xBigWindow, yBigWindow );
	sYogaWindow = wxSize( xBigWindow, yBigWindow );
	sPrintPreviewWindow = wxSize( xBigWindow, yBigWindow );
	sVargaSummaryWindow = wxSize( xBigWindow, yBigWindow );
	sShadbalaWindow = wxSize( xBigWindow, yBigWindow );
	sAshtakaVargaWindow = wxSize( xBigWindow, yBigWindow );
	sEphemWindow = wxSize( xBigWindow, yBigWindow );
	sEclipseWindow = wxSize( xMediumWindow, yMediumWindow );
	sHoraWindow = wxSize( xMediumWindow, yMediumWindow );
	sPartnerWindow = wxSize( xBigWindow, yBigWindow );
	sAdditionalLogWindow = wxSize( xSmallWindow, ySmallWindow );
	
	sConfigDialog = wxSize( xBigWindow, yMediumWindow );
	sSimpleConfigDialog = wxSize( xMediumWindow, yMediumWindow );
	sYogaEditor = wxSize( xMediumWindow, yMediumWindow );
	sAtlasDialog = wxSize( xMediumWindow, yBigWindow );
	sObjectFilterDialog = wxSize( xSmallWindow, ySmallWindow );
	sPrintoutSelectionDialog = wxSize( xSmallWindow, ySmallWindow );
}

/*****************************************************
**
**   AtlasConfig   ---   Constructor
**
******************************************************/
AtlasConfig::AtlasConfig()
{
	FileConfig *fc = FileConfig::get();
	wxString separator = wxFileName::GetPathSeparator();
	const static wxChar *c_countries[8] = {
		wxT( "IN" ), wxT( "US" ), wxT( "DE" ), wxT( "RU" ), wxT( "FR" ), wxT( "IT" ), wxT( "GB" ), wxT( "CA" )
	};
	for( int i = 0; i < 8; i++ ) favouriteCountries.push_back( c_countries[i] );

	filterMode = 0;
	filterCaseSensitive = false;
	cacheSize = 2000;

	sqlFile = fc->getAtlasSqlFile();	
	databaseFile = fc->getAtlasDbFile();
}

/*****************************************************
**
**   AnimationConfig   ---   Constructor
**
******************************************************/
AnimationConfig::AnimationConfig()
{
	interval = 5;
	mode = 0;
	stepLength = 5;
	stepDimension = 1;
}

/*****************************************************
**
**   MultipleViewConfiguration   ---   Constructor
**
******************************************************/
MultipleViewConfiguration::MultipleViewConfiguration()
{
	useMultipleViews = false;
	defaultView = 0;

	// Toolbook
	notebookStyle = 1;

	// Top
	notebookOrientation = 0;
}

/*****************************************************
**
**   Config   ---   Constructor
**
******************************************************/
Config::Config()
{
	writer = new WriterConfig;

	western = new WesternConfig;
	westernCalculation = new WesternCalculationConfig;
	westernChart = new WesternChartDisplayConfig;
	westernChartBehavior = new WesternChartBehaviorConfig;

	uranian = new UranianConfig;

	vedic = new VedicConfig;
	vedicCalculation = new VedicCalculationConfig;
	vedicChart = new VedicChartDisplayConfig;
	vedicChartBehavior = new VedicChartBehaviorConfig;

	ephem = new EphemConfig;

	viewprefs = new ViewPreferencesConfig;
	view = new ViewConfig;

	atlas = new AtlasConfig;
	animation = new AnimationConfig;
	bardiagram = new BarDiagramConfig;
	toolbar = new ToolbarConfig;

	defaultLocation = new Location;
	colors = new ColorConfig;
	chartColors = new ObjectColorConfig;

	print = new PdfDocumentConfig;
	multipleView = new MultipleViewConfiguration;

	preferVedic = true;
	openNewDocOnStart = true;
	askOnQuit = false;
}

/*****************************************************
**
**   Config   ---   Destructor
**
******************************************************/
Config::~Config()
{
	delete writer;
	delete western;
	delete westernCalculation;
	delete westernChart;
	delete westernChartBehavior;
	delete uranian;
	delete vedic;
	delete vedicChart;
	delete vedicChartBehavior;
	delete vedicCalculation;
	delete ephem;
	delete view;
	delete viewprefs;
	delete atlas;
	delete animation;
	delete bardiagram;
	delete toolbar;
	delete defaultLocation;
	delete colors;
	delete chartColors;
	delete print;
	delete multipleView;
}

/*****************************************************
**
**   Config   ---   readAll
**
******************************************************/
void Config::readAll()
{
	wxJSONReader reader;
	wxJSONValue root;
	uint i;
	JSonTool tool;

	wxString filename = FileConfig::get()->getConfigFilename();
	if ( ! wxFileName::FileExists( filename ))
	{
		//wxLogMessage( wxT( "Config file %s does not exist, using defaults" ), filename.c_str());
	}
	else if ( ! wxFileName::IsFileReadable( filename ))
	{
		wxLogError( wxT( "Config file %s is not readable, using defaults" ), filename.c_str());
	}
	else if ( wxFileName::GetSize( filename ) == 0 )
	{
		wxLogError( wxT( "Config file %s has NULL size, using defaults" ), filename.c_str());
	}
	else
	{
		wxFileInputStream stream( filename );
		const int numErrors = reader.Parse( stream, &root );

		if ( numErrors > 0 )
		{
			wxLogError( wxT( "Config file %s has errors, using defaults" ), filename.c_str());
			const wxArrayString& errors = reader.GetErrors();
			for ( i = 0; i < errors.GetCount(); i++ )
			{
				wxLogError( errors[i] );
			}
		}
		else
		{
			load( root, wxT( "config" ));
#ifdef DEBUG_CONFIG_FILE_OPERATIONS
			printf( "Config file %s successfully loaded\n", str2char( filename ));
#endif
		}
	}

	// aspects
	{
		wxJSONValue &vaspects = root[ wxT( "aspects" ) ];
		if ( ! vaspects.IsNull()) // may be null if config is empty
		{
			if ( vaspects.IsArray())
			{
				for ( int i = 0; i < vaspects.Size(); i++ )
				{
					AspectConfig cfg;
					cfg.load( vaspects[i] );
					aspects.push_back( cfg );
				}
			}
			else wxLogWarning( wxT( "No valid aspect array found" ));
		}
		AspectConfigLoader::get()->initAspects();
	}

	// Bar Diagram
	{
		wxJSONValue &vbar = root[ wxT( "bardiagram" ) ];
		if (  ! vbar.IsNull())
		{
			if ( vbar.IsArray())
			{
				if ( vbar.Size() != MAX_BARDIAGRAM_COLORS ) 
					wxLogError( wxT( "array for bar diagram config has wrong size %d instead of %d" ), vbar.Size(), MAX_BARDIAGRAM_COLORS );

				for ( i = 0; i < (uint)vbar.Size() && i < MAX_BARDIAGRAM_COLORS; i++ )
				{
					BarDiagramStyle cfg;
					cfg.load( vbar[i] );
					bardiagram->style[i] = cfg;
				}
			}
			else wxLogError( wxT( "No valid array for bar diagram config found" ));
		}
	}

	// chart colors
	wxJSONValue &vchartColors = root[ wxT( "chartColors" ) ];
	{
		wxJSONValue &vsignFgColors = vchartColors[ wxT( "signFgColors" ) ];
		if ( ! vsignFgColors.IsNull())
		{
			if ( vsignFgColors.IsArray())
			{
				if ( vsignFgColors.Size() != 4 ) 
					wxLogError( wxT( "array for sign foregound color config has wrong size %d instead of 4" ), vsignFgColors.Size() );

				for ( i = 0; i < (uint)vsignFgColors.Size() && i < 4; i++ )
				{
					chartColors->signFgColor[i] = wxColour( vsignFgColors[i].AsString() );
				}
			}
			else wxLogError( wxT( "No valid array for sign foreground color config found" ));
		}
	}

	{
		wxJSONValue &vsignPalette = vchartColors[ wxT( "signPalette" ) ];
		if ( ! vsignPalette.IsNull())
		{
			if ( vsignPalette.IsArray())
			{
				tool.readPalette( vsignPalette, chartColors->signPalette );
			}
			else wxLogError( wxT( "No valid array for sign background color config found" ));
		}
	}

	{
		wxJSONValue &vhouseFgColors = vchartColors[ wxT( "houseFgColors" ) ];
		if ( ! vhouseFgColors.IsNull())
		{
			if ( vhouseFgColors.IsArray())
			{
				if ( vhouseFgColors.Size() != 4 ) 
					wxLogError( wxT( "array for house foregound color config has wrong size %d instead of 4" ), vhouseFgColors.Size() );

				for ( i = 0; i < (uint)vhouseFgColors.Size() && i < 4; i++ )
				{
					chartColors->houseFgColor[i] = wxColour( vhouseFgColors[i].AsString() );
				}
			}
			else wxLogError( wxT( "No valid array for house foreground color config found" ));
		}
	}

	{
		wxJSONValue &vhousePalette = vchartColors[ wxT( "housePalette" ) ];
		if ( ! vhousePalette.IsNull())
		{
			if ( vhousePalette.IsArray())
			{
				tool.readPalette( vhousePalette, chartColors->housePalette );
			}
			else wxLogError( wxT( "No valid array for sign background color config found" ));
		}
	}

	// planet colors
	{
		wxJSONValue &vplanetFgColors = vchartColors[ wxT( "planetFgColors" ) ];
		if ( ! vplanetFgColors.IsNull())
		{
			if ( vplanetFgColors.IsArray())
			{
				if ( vplanetFgColors.Size() != MAX_EPHEM_OBJECTS ) 
					wxLogError( wxT( "array for house backgound color config has wrong size %d instead of %d" ), vplanetFgColors.Size(), MAX_EPHEM_OBJECTS );

				for ( i = 0; i < (uint)vplanetFgColors.Size() && i < MAX_EPHEM_OBJECTS; i++ )
				{
					chartColors->planetFgColor[i] = wxColour( vplanetFgColors[i].AsString() );
				}
			}
			else wxLogError( wxT( "No valid array for planet foreground color config found" ));
		}
	}

	// Font
	{
		wxJSONValue &vfont = root[ wxT( "font" ) ];
		if ( ! vfont.IsNull())
		{
			if ( vfont.IsArray())
			{
				if ( vfont.Size() != MAX_FONTS ) 
					wxLogError( wxT( "array for font config has wrong size %d instead of %d" ), vfont.Size(), MAX_FONTS );

				for ( i = 0; i < (uint)vfont.Size() && i < MAX_FONTS; i++ )
				{
					FontConfig cfg;
					cfg.load( vfont[i] );
					font[i] = cfg;
				}
			}
			else wxLogError( wxT( "No valid array for font config found" ));
		}
	}
}

/*****************************************************
**
**   Config   ---   writeAll
**
******************************************************/
void Config::writeAll()
{
	wxJSONValue root;
	uint i;
	JSonTool tool;
	FileConfig *fc = FileConfig::get();

	wxString filename = fc->getConfigFilename();

#ifdef DEBUG_CONFIG_FILE_OPERATIONS
	printf( "SAVE %s\n", str2char( filename ));
#endif

	fc->backupFile( filename );

	save( root, wxT( "config" ));

	// Aspects
	wxJSONValue &vaspects = root[ wxT( "aspects" ) ];
	for( i = 0; i < MAX_ASPECT_TYPES && i < aspects.size(); i++ )
	{
		aspects[i].save( vaspects );
	}

	// Graphical Dasa
	wxJSONValue &vbar = root[ wxT( "bardiagram" ) ];
	for ( i = 0; i < MAX_BARDIAGRAM_COLORS; i++ )
	{
		bardiagram->style[i].save( vbar );
	}
	
	// chart colors
	wxJSONValue &vchartColors = root[ wxT( "chartColors" ) ];
	wxJSONValue vsignFgColors;
	wxJSONValue houseFgColors;
	for ( i = 0; i < 4; i++ )
	{
		vsignFgColors.Append( wxJSONValue( chartColors->signFgColor[i].GetAsString( wxC2S_HTML_SYNTAX )));
		houseFgColors.Append( wxJSONValue( chartColors->houseFgColor[i].GetAsString( wxC2S_HTML_SYNTAX )));
	}
	vchartColors[ wxT( "signFgColors" ) ] = vsignFgColors;
	vchartColors[ wxT( "houseFgColors" ) ] = houseFgColors;

	vchartColors[ wxT( "signPalette" ) ] = tool.writePalette( chartColors->signPalette );
	vchartColors[ wxT( "housePalette" ) ] = tool.writePalette( chartColors->housePalette );

	wxJSONValue vplanetFgColors;
	for ( i = 0; i < MAX_EPHEM_OBJECTS; i++ )
	{
		vplanetFgColors.Append( wxJSONValue( chartColors->planetFgColor[i].GetAsString( wxC2S_HTML_SYNTAX )));
	}
	vchartColors[ wxT( "planetFgColors" ) ] = vplanetFgColors;

	// Font
	wxJSONValue &vfont = root[ wxT( "font" ) ];
	for ( i = 0; i < MAX_FONTS; i++ )
	{
		font[i].save( vfont );
	}

	// debug output to console
	wxJSONWriter w;
	/*
	wxString s;
	w.Write( root, s );
	PrintLn( s );
	*/

	wxFileOutputStream stream( filename );
	w.Write( root, stream );
}


