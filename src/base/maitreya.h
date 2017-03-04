/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/base/maitreya.h
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
#ifndef _MAITREYA_H_
#define _MAITREYA_H_

#include <stdio.h>

#include <wx/colour.h>
#include <wx/dynarray.h>
#include <wx/gdicmn.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <set>
#include <vector>
#include <complex>

using namespace std;

typedef complex<double> MPoint;

wxPoint pointToWxPoint( const MPoint& );
MPoint findOrthogonalPoint( const MPoint&, const double length = 1.0 );

/*************************************************//**
*
*  
*
******************************************************/
class MRect
{
public:
	MRect() { reset(); }
	MRect( const double &x0, const double &y0, const double &w, const double &h ) { x = x0; y = y0; width = w; height = h; }
	MRect( const MPoint &p, const double &w, const double &h ) { x = p.real(); y = p.imag(); width = w; height = h; }
	MRect( const MPoint &p, const MPoint &q ) { x = p.real(); y = p.imag(); width = q.real(); height = q.imag(); }
	MRect( const wxRect &r ) { x = r.x; y = r.y; width = r.width; height = r.height; }

	wxRect toWxRect() { return wxRect( (int)x, (int)y, (int)width, (int)height ); }

	MPoint getCenter() { return MPoint( x + .5 * width, y + .5 * height ); }

	wxString toString();
	bool isNil() { return width <= 0 || height <= 0; }
	void reset() { x = y = width = height = 0; }

	double getRight() const { return x + width; }
	double getBottom() const { return y + height; }

	bool intersects( const MRect& ) const;
	MRect& intersect( const MRect& );
	MRect intersect( const MRect& ) const;

	MRect grow( const double& );

	double x, y, width, height;
};

bool pointInRect( const MPoint&, const MRect& );
bool pointInRect( const wxPoint&, const MRect& );

/*************************************************//**
*
* 
*
******************************************************/
class MBrush 
{
public:

	MBrush();
	MBrush( const MBrush& );

 	MBrush( const wxColour &color, const int style = wxSOLID );
	MBrush( const wxString stippleFilename, const int rotateHue = 0 );

	bool IsOk() const;
	wxString toString() const;

	wxColour color;
	wxString filename;
	int style, rotateHue;

};


#define WXPDFDOC_BUILTIN_VERSION wxT( "0.9.4" )
#define WXSQLITE3_BUILTIN_VERSION wxT( "3.0.3" )

const wxString getApplicationVersion();

/**************************************************************
***
**   character stuff
***
***************************************************************/

// Line feed
#ifdef __WXMSW__
#define Endl wxT( "\r\n" )
#else
#define Endl wxT( "\n" )
#endif

#ifndef uint
#define uint unsigned int
#endif

#define SYMBOL_CODE_ERROR 'Z'

#define CSV_DELIMITER ';'

#define SPACE wxT( " " )
#define COLON_SPACE wxT( ": " )
#define FILE_CONF_MODE wxConvUTF8

//#define str2char( s ) (const char*) wxConvCurrent->cWX2MB(s)
#define str2char( s ) (const char*) wxConvCurrent->cWX2MB(s.c_str())

// console output for wxString
#define Cout( s ) printf( "%s", str2char( s ));
#define PrintLn( s ) printf( "%s\n", str2char( s ));

#if ( wxMAJOR_VERSION == 2 && wxMINOR_VERSION <= 8 )
#define _WX_V2_
#endif

#ifdef __WXMAC__
#define MAC_PDF_START_COMMAND wxT( "open -a Preview" )
#define MAC_RESOURCE_DIRECTORY wxT( "maitreya8.app/Contents/Resources" )
#endif

#define AllTrim( s ) s.Trim( true ).Trim( false )
#define NONE wxT( "[None]" )

/**************************************************************
***
**  VARIOUS CONSTANTS
***
***************************************************************/

#define YEAR_LEN 365.25;
#define ICUSTOM_YEAR_LEN 365.25f;
#define WCUSTOM_YEAR_LEN 362.83f;

#define PI 3.14159265358979323846
#define SQRT2 1.414213562

// Constants for transformations deg <=> rad
#define DEG2RAD 0.0174532925199433
#define RAD2DEG 57.2957795130823

// point to mm
#define PT2MM .352777777778

#define DEFAULT_T0 2415020.0
#define DEFAULT_AYAN_T0 22.46047
#define DEFAULT_AYA_PERIOD 25771.4021

#define MIN_EPHEM_YEAR -3000
#define MAX_EPHEM_YEAR 3000
#define MIN_EPHEM_JD 0
#define MAX_EPHEM_JD 5000000
#define EPHEM_JD_2000 2451544.5
/*
#define MIN_EPHEM_JD 625674.5
#define MAX_EPHEM_JD 2817151.5
*/

// Min and Max macros
#ifndef Min
#define Min( a, b ) ( a < b ? a : b )
#endif
#ifndef Max
#define Max( a, b ) ( a > b ? a : b )
#endif

#define UINT_FOR_NOT_FOUND UINT_MAX - 1
#define UINT_FOR_OUTSIDE UINT_MAX - 2
#define UINT_FOR_NOT_SET UINT_MAX - 3

#define APP_NAME wxT( "maitreya8" )
#define CONFIG_DIR_NAME wxT( "maitreya-8.0" )
#define VENDOR_NAME wxT( "Saravali" )
#define GUI_APP_NAME wxT( "Maitreya" )
#define SYMBOL_FONT_NAME wxT( "Saravali" )

// standard path for swiss ephem data files on Debian systems
#define DEBIAN_SWE_STANDARD_DATA_PATH wxT( "/usr/share/libswe/ephe" )

// Various file and directory names
#define LOGO_FILE wxT( "logo.jpg" )
#define LICENSE_FILE_NAME wxT( "COPYING" )
#define WIN_LICENSE_FILE_NAME wxT( "Copying.txt" )
#define DASA_DIR_NAME wxT( "dasas" )
#define YOGA_DIR_NAME wxT( "yogas" )
#define PIC_DIR_NAME wxT( "pics" )
#define RESOURCE_DIR_NAME wxT( "resources" )
#define FONT_DIR_NAME wxT( "fonts" )
#define ATLAS_DIR_NAME wxT( "atlas" )
#define EPHEM_DIR_NAME wxT( "libswe/ephe" )

// file names for various config files
#define CFG_JSON_EXTENSION wxT( ".json" )
#define CONFIG_FILE_NAME wxT( "config.json" )
#define CFG_SHEET wxT( "sheet" )
#define CFG_WESTERNCHART wxT( "wchart" )
#define CFG_VEDICCHART wxT( "vchart" )
#define CFG_MULTIPLEVIEW wxT( "mview" )
#define CFG_PRINTOUT wxT( "print" )

// Atlas
#define ATLAS_MAX_GRID_ELEMENTS 50
#define ATLAS_DEFAULT_DBFILENAME  wxT( "geonames.db" )
#define ATLAS_DEFAULT_SQLFILENAME wxT( "geonames1000.sql" )

// AboutDialog
#define AUTHOR wxT( "Martin Pettau" )
#define SARAVALI_URL wxT( "http://www.saravali.de" )
#define SARAVALI_HELP_URL wxT( "http://www.saravali.de/documentation.html" )

enum ChartType { CT_RADIX, CT_TRANSIT, CT_PARTNER };
#define ASSERT_VALID_CHARTTYPE( t ) assert( t == CT_RADIX || t == CT_TRANSIT || t == CT_PARTNER );

enum ASPECT_TYPE { AT_NONE = -1,
	AT_CONJUNCTION = 0, AT_OPPOSITION, AT_TRINE, AT_SQUARE,
	AT_SEXTILE, AT_QUINCUNX, AT_SEMISQUARE, AT_SESQUISQUARE, AT_SEMISEXTILE,
	AT_QUINTILE, AT_BIQUINTILE, AT_PARALLELE, AT_CONTRAPARALLELE,
	AT_SEPTILE, AT_BISEPTILE, AT_TRISEPTILE, AT_NOVILE,
	AT_BINOVILE, AT_QUADNOVILE,
	AT_QUARTERSQUARE, AT_QUAVER, AT_SEMIQUAVER,
	AT_GRAHA_DRISHTI, AT_RASI_DRISHTI, AT_SBC_VEDHA
};

// only western aspects are counted
#define MAX_ASPECT_TYPES 22

ASPECT_TYPE operator++ ( ASPECT_TYPE& );
ASPECT_TYPE operator++ ( ASPECT_TYPE&, int );

#define ASSERT_VALID_ASPECT( p ) assert( p >= AT_CONJUNCTION && p <= AT_SEMIQUAVER );

// Sort orders for aspects
enum ASPECT_SORTORDER {
	AS_NONE = -1,
	AS_RPLANET = 0,     // planet on right side of equation p1 = p2
	AS_LPLANET,         // left side of p1 = p2
	AS_ORBIS,           // negativve values first i.e. matchings in the past for e.g. solar arc
	AS_ORBIS_ABSOLUTE,  // smallest value (i.e. best matching) first
	AS_ORBIS_REVERSE,   // see orbis
	AS_TYPE,            // conjunction, opposition etc. also used in uranian astrology (gradkreis)
	AS_TYPE_REVERSE,    // same as above, but reverse
	AS_DATE,            // matching date
	AS_JD,
	AS_VALUE,           // unused
	AS_SCORE            // unused
};

#define MAX_BARDIAGRAM_COLORS 22 // 12 + 10
#define ASSERT_VALID_DASA_COLOR( p ) assert( p >= 0 && p < MAX_DASA_COLORS );

/**************************************************************
***
**       PLANETARY OBJECTS
***
***************************************************************/
enum ObjectId
{
	OERROR = -2,
	ONONE = -1,

	OSUN = 0, OMOON, OMERCURY, OVENUS, OMARS, OJUPITER, OSATURN, // 0 .. 6
	OURANUS, ONEPTUNE, OPLUTO, // 7 ... 9

	// Nodes
	OMEANNODE, OTRUENODE, OMEANDESCNODE, OTRUEDESCNODE, // 10 ... 13

	// AS-MC-DC-IC
	OASCENDANT, OMERIDIAN, ODESCENDANT, OIMUMCOELI, // 14 ... 17

	// Uranian
	OCUPIDO, OHADES, OZEUS, OKRONOS, OAPOLLON, OADMETOS, OVULKANUS, OPOSEIDON, // 18 ... 25

	// Planetoids and Lilith
	OCHIRON, OPHOLUS, OCERES, OPALLAS, OJUNO, OVESTA, OLILITH,       // 26 ... 32

	OARIES,                                                         // 33
	ODHUMA, OVYATIPATA, OPARIVESHA, OCHAPA, OUPAKETU,               // 34 ... 38
	OKALA, OMRITYU, OARDHAPRAHARA, OYAMAGHANTAKA, OGULIKA, OMANDI,  // 39 ... 44
	OBHAVA_LAGNA, OHORA_LAGNA, OGHATIKA_LAGNA,                      // 45 ... 47
	OD9LAGNA,                                                       // 48

	OHOUSE1, OHOUSE2, OHOUSE3, OHOUSE4, OHOUSE5, OHOUSE6, OHOUSE7, OHOUSE8, OHOUSE9, OHOUSE10, OHOUSE11, OHOUSE12
};

ObjectId operator++ ( ObjectId& );
ObjectId operator++ ( ObjectId&, int );
ObjectId operator+ ( const ObjectId&, const ObjectId& );

#define ObjectArray vector<ObjectId>
#define ObjectFilter set<ObjectId>
wxArrayInt objectArray2wxArrayInt( const ObjectArray& );
ObjectArray wxArrayInt2objectArray( const wxArrayInt& );

#define FIRST_EPHEM_OBJECT OSUN   // 0
#define MAX_EPHEM_OBJECTS 33      
#define LAST_EPHEM_OBJECT (ObjectId)( FIRST_EPHEM_OBJECT + MAX_EPHEM_OBJECTS - 1 ) // 32

#define FIRST_EXTENDED_OBJECT OARIES  // 33
#define MAX_EXTENDED_OBJECTS 16
#define LAST_EXTENDED_OBJECT (ObjectId)( FIRST_EXTENDED_OBJECT + MAX_EXTENDED_OBJECTS - 1 ) // 48

#define OFORTUNE (ObjectId)( OHOUSE12 + 1 ) // 61
#define FIRST_ARABIC_OBJECT OFORTUNE
#define MAX_ARABIC_OBJECTS 177
#define LAST_ARABIC_OBJECT (ObjectId)(FIRST_ARABIC_OBJECT + MAX_ARABIC_OBJECTS - 1 )  // 237

#define IS_ANGLE_OBJECT( p ) ( p == OASCENDANT || p == ODESCENDANT || p == OMERIDIAN || p == OIMUMCOELI )

#define IS_ASC_NODE( p ) ( p == OMEANNODE || p == OTRUENODE )
#define IS_DESC_NODE( p ) ( p == OMEANDESCNODE || p == OTRUEDESCNODE )
#define IS_LUNAR_NODE( p ) ( IS_ASC_NODE( p ) || IS_DESC_NODE( p ))

#define IS_EPHEM_OBJECT( p ) ( p >= FIRST_EPHEM_OBJECT && p <= LAST_EPHEM_OBJECT )
#define IS_EXTENDED_OBJECT( p ) ( p >= FIRST_EXTENDED_OBJECT && p <= LAST_EXTENDED_OBJECT )
#define IS_HOUSE_OBJECT( p ) ( p >= OHOUSE1 && p <= OHOUSE12 )
#define IS_ARABIC_OBJECT( p ) ( p >= FIRST_ARABIC_OBJECT && p <= LAST_ARABIC_OBJECT );

#define ASSERT_VALID_EPHEM_OBJECT( p ) assert( p >= FIRST_EPHEM_OBJECT && p <= LAST_EPHEM_OBJECT );
#define ASSERT_VALID_EXTENDED_OBJECT( p ) assert( p >= FIRST_EXTENDED_OBJECT && p <= LAST_EXTENDED_OBJECT );
#define ASSERT_VALID_HOUSE( p ) assert( p >= HOUSE1 && p <= HOUSE12 );
#define ASSERT_VALID_ARABIC_OBJECT( p ) assert( p >= FIRST_ARABIC_OBJECT && p <= LAST_ARABIC_OBJECT );

#define ASSERT_VALID_OBJECT( p ) \
  assert(( p >= FIRST_EPHEM_OBJECT && p <= LAST_EPHEM_OBJECT ) \
	|| ( p >= FIRST_EXTENDED_OBJECT && p <= LAST_EXTENDED_OBJECT ) \
	|| ( p >= FIRST_ARABIC_OBJECT && p <= LAST_ARABIC_OBJECT ) \
	|| ( p >= OHOUSE1 && p <= OHOUSE12 ));

enum { CDHUMA = 0, CVYATIPATA, CPARIVESHA, CCHAPA, CUPAKETU, CKALA, CMRITYU, CARDHAPRAHARA, CYAMAGHANTAKA, CGULIKA, CMANDI };
#define NUM_UPA 11

enum { CBHAVA_LAGNA = 0, CHORA_LAGNA, CGHATIKA_LAGNA };
#define NUM_LAGNA 3

// Ephemeris
enum { EPHEM_COORDINATES_GEOCENTRIC = 0, EPHEM_COORDINATES_TOPOCENTRIC, EPHEM_COORDINATES_EQUATORIAL,
	EPHEM_COORDINATES_BARYCENTRIC, EPHEM_COORDINATES_HELIOCENTRIC
};

// Solar events
enum { SOLAR_EVENT_SUNRISE, SOLAR_EVENT_SUNSET, SOLAR_EVENT_MIDNIGHT, SOLAR_EVENT_NOON };

// convenience indices for houses, so we don't ge confused when counting them
enum { HOUSE1 = 0, HOUSE2, HOUSE3, HOUSE4, HOUSE5, HOUSE6, HOUSE7, HOUSE8, HOUSE9, HOUSE10, HOUSE11, HOUSE12 };

#define HOUSE_SYS_NUM 13
enum HOUSE_SYSTEM {
	HS_NONE = -1,
	HS_PLACIDUS = 0, HS_KOCH, HS_REGIOMONTANUS,
	HS_CAMPANUS, HS_PORPHYRY, HS_EQUAL, HS_VEHLOW, HS_AXIAL,
	HS_AZIMUTAL, HS_POLICH, HS_ALCABITUS, HS_MORINUS, HS_KRUSINSKI
};

enum MOVING_DIRECTION { MD_NONE = -1, MD_DIRECT = 0, MD_RETROGRADE, MD_STATIONARY };

enum AYANAMSA { AY_NONE = 0, AY_LAHIRI, AY_RAMAN, AY_KRISHNAMURTI, AY_CUSTOM };

#define ASSERT_VALID_DEGREE( l ) assert( l >= 0 && l < 360 );

#define ASSERT_VALID_SHADBALA_INDEX( p ) assert( p >= 0 && p <= OSATURN )

// planetary friendship
enum FRIENDSHIP { PF_NONE = -1, PF_BEST_FRIEND = 0, PF_FRIEND, PF_NEUTRAL, PF_ENEMY, PF_SWORN_ENEMY };
#define ASSERT_VALID_FRIENDSHIP( f ) assert( f >= PF_BEST_FRIEND && f <= PF_SWORN_ENEMY );


// Zodiacal signs,
enum Rasi
{
	R_ERROR = -2,
	R_NONE = -1,
	R_ARIES = 0, R_TAURUS, R_GEMINI, R_CANCER, R_LEO, R_VIRGO,
	R_LIBRA, R_SCORPIO, R_SAGITTARIUS, R_CAPRICORN, R_AQUARIUS, R_PISCES
};

#define ASSERT_VALID_RASI( r ) assert ( r >= R_ARIES && r <= R_PISCES );
#define ASSERT_VALID_EXTENDED_RASI( r ) assert ( r >= R_NONE && r <= R_PISCES );

Rasi operator++ ( Rasi& );
Rasi operator++ ( Rasi&, int );
Rasi operator+ ( const Rasi&, const Rasi& );

#define IS_AVPLANET( p ) ( ( p >= OSUN && p <= OSATURN ) || p == OASCENDANT )

enum PlanetContext {
	PcNone = -1,
	PcRadix = 0,
	PcRadixAntiscia,
	PcTransit,
	PcDirection,
	PcSolarArc,
	PcSolarArcReverse,
	PcShiftedGravitationPoint,
	PcShiftedMeridian,
	PcLunarArc,
	PcConstantArc,
	PcPeriod,
	PcPartner1,
	PcPartner2
};
#define ASSERT_VALID_PLANET_CONTEXT( m ) assert( m >= PcRadix && m <= PcPartner2 );
#define ASSERT_VALID_TRANSIT_CONTEXT( m ) assert( m >= PcTransit && m <= PcConstantArc );
#define MAX_PLANET_CONTEXT 13

/**************************************************************
***
**     Includes for lists of objects in various views
***
***************************************************************/
enum OBJECT_INCLUDES {
	OI_NONE          = 0x00000000,
	OI_INNER         = 0x00000001,
	OI_OUTER         = 0x00000002,
	OI_DRAGONTAIL    = 0x00000004,
	OI_DRAGONHEAD    = 0x00000008,
	OI_ASCENDANT     = 0x00000010,
	OI_MERIDIAN      = 0x00000020,
	OI_DESCENDANT    = 0x00000040,
	OI_IMUMCOELI     = 0x00000080,
	OI_URANIAN_INNER = 0x00000100,
	OI_URANIAN_OUTER = 0x00000200,
	OI_CHIRON        = 0x00000400,
	OI_PHOLUS        = 0x00000800,
	OI_PLANETOIDS    = 0x00001000,
	OI_LILITH        = 0x00002000,

	OI_ARIES         = 0x00004000,
	OI_KALAVELAS     = 0x00008000,
	OI_UPAGRAHAS     = 0x00010000,
	OI_SPECIALLAGNAS = 0x00020000,
	OI_D9_LAGNA      = 0x00040000,
	OI_ARABICPARTS   = 0x00080000,
	OI_ALL_HOUSES    = 0x00100000,
	OI_4_HOUSES      = 0x00200000
};

OBJECT_INCLUDES operator~ ( const OBJECT_INCLUDES& a );
OBJECT_INCLUDES operator& ( const OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b );
OBJECT_INCLUDES operator| ( const OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b );
void operator&= ( OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b );
void operator|= ( OBJECT_INCLUDES& a, const OBJECT_INCLUDES& b );

// Graphical chart frame types
enum CHART_FRAME { CF_NONE = -1, CF_INHERITED = 0, CF_CIRCLE, CF_ELLIPSE, CF_SQUARE, CF_RECTANGLE, CF_TRIANGLE };

enum FIELD_PART { FP_ALL = 0, FP_INNER, FP_OUTER };

// Vedic chart type
enum { VCT_SOUTH = 0, VCT_NORTH, VCT_EAST };

// Vedic chart north indian options
enum { VCN_ASC = 0, VCN_NUMBER, VCN_SHORT, VCN_SYMBOL };

// Vedic chart center
enum { VCC_NOTHING = 0, VCC_NAME, VCC_NAME_SIGNIFICATION, VCC_DIVISION, VCC_DIVISION_SIGNIFICATION };

// Text styles for planets, signs etc
enum TEXT_FORMAT { TF_SHORT = 0, TF_MEDIUM, TF_LONG };

// Text Window
enum TEXT_MODE {
	TM_BASE = 0,
	TM_ARABICPARTS,
	TM_ASHTAKAVARGA,
	TM_ASTRONOMICAL,
	TM_BHAVA,
	TM_DASA,
	TM_DASACOMPACT,
	TM_DASALONG,
	TM_JAIMINI,
	TM_KP,
	TM_NAKSHATRA,
	TM_VARGA,
	TM_VEDIC_ASPECTARIUM,
	TM_VEDIC_PLANETS,
	TM_WESTERN_ASPECTARIUM,
	TM_WESTERN_PLANETS,
	TM_WESTERN_ASPECTARIUM_PLAIN,
	TM_MAIN_VIEW
};

// Notebook styles
enum { NB_STYLE_NOTEBOOK = 0, NB_STYLE_LISTBOOK, NB_STYLE_CHOICEBOOK, NB_STYLE_TOOLBOOK, NB_STYLE_TREEBOOK };

// text formatting constants
#define FORMAT_DEGREE         1
#define FORMAT_HOUR           2
#define FORMAT_SHOW_SECONDS  16
#define FORMAT_RED_Rasi      32
#define FORMAT_SHOW_DIR      64

// input field validation
enum  { IFV_NONE = 0, IFV_RESTORE, IFV_ADJUST };

enum { DEG_PRECISION_MINUTE, DEG_PRECISION_SECOND, DEG_PRECISION_MORE, DEG_PRECISION_FLEXIBLE };
//enum { DIR_NONE = -1, DIR_DIRECT, DIR_RETROGRADE, DIR_STATIONARY, DIR_DIRECT_SHOW };

/**************************************************************
***
**  VEDIC ASTROLOGY
***
***************************************************************/
// Vargas
#define NB_VARGAS 21

enum Varga
{
	V_NONE = -1,
	V_RASI = 0, V_NAVAMSA, V_HORA, V_DREKKANA, V_CHATURTAMSA,
	V_SASTAMSA,	V_SAPTAMAMSA, V_ASHTAMSA, V_DASAMSA, V_DVADASAMSA,
	V_SHODASAMSA, V_VIMSAMSA,	V_SIDDHAMSA, V_BHAMSA, V_TRIMSAMSA,
	V_CHATVARIMSAMSA, V_AKSHAVEDAMSA,	V_SHASTIAMSA,
	V_ASHTOTTARAMSA, V_DVADASDVADASAMSA, V_BHAVA
};

#define V_FIRST V_RASI
#define V_LAST V_BHAVA

//Varga operator++ ( const Varga&, const int = 1 );
Varga operator++ ( Varga& );
Varga operator++ ( Varga&, int );
#define IS_VALID_VARGA( v ) ( v >= V_FIRST && v <= V_LAST )
#define ASSERT_VALID_VARGA( v ) assert ( IS_VALID_VARGA( v ));

// Karakas of Jaimini Astrology
enum { ATMA_KARAKA, AMATYA_KARAKA, BHRATRU_KARAKA, MATRU_KARAKA, PITRU_KARAKA, PUTRA_KARAKA, GNATI_KARAKA, DARA_KARAKA };
#define NO_KARAKA -1

//enum { DASA_TYPE_NONE = -1, DASA_TYPE_NAKSHATRA, DASA_TYPE_PLANET, DASA_TYPE_SIGN };
enum { LUNAR_NODE_TRUE = 0, LUNAR_NODE_MEAN };

enum Varna { VARNA_NONE = -1, VARNA_BRAHMIN, VARNA_KSHATRIYA, VARNA_VAISHYA, VARNA_SHUDRA };
enum Gana { GANA_NONE = -1, GANA_DEVA, GANA_MANUJ, GANA_RAKSHAS };
enum Nadi { NADI_NONE = -1, NADI_AADI, NADI_MADHYA, NADI_ANTYA };
enum Vashya{ VASHYA_NONE = -1, VASHYA_QUADRUPED, VASHYA_HUMAN, VASHYA_JALACHARA, VASHYA_LEO, VASHYA_SCORPIO };

enum VEDICASPECT_STRENGTH { VA_FULL = 0, VA_75, VA_50, VA_25 };

// 28 Nakshatras (plus Abhijit)
enum NakshatraId_28
{
	N28_NONE = -1,
	N28_ASWINI = 0, N28_BHARANI, N28_KRITTIKA, N28_ROHINI, N28_MRIGASIRA, N28_ARUDRA, N28_PUNARVASU, N28_PUSHYAMI, N28_ASLESHA,
	N28_MAKHA, N28_PPHALGUNI, N28_UPHALGUNI, N28_HASTHA, N28_CHITTA, N28_SWATI, N28_VISAKHA, N28_ANURADHA, N28_JYESHTA,
	N28_MULA, N28_PSHADHA, N28_USHADHA, N28_ABHIJIT, N28_SRAVANA, N28_DHANISHTA, N28_SATHABISHAK, N28_PBHADRA, N28_UBHADRA, N28_REVATI
};
#define ASSERT_VALID_NAKSHATRA_28( r ) assert ( r >= N28_ASWINI && r <= N28_REVATI );
NakshatraId_28 operator++ ( NakshatraId_28& );
NakshatraId_28 operator++ ( NakshatraId_28&, int );

// 27 Nakshatras (without Abhijit)
enum NakshatraId_27
{
	N27_NONE = -1,
	N27_ASWINI = 0, N27_BHARANI, N27_KRITTIKA, N27_ROHINI, N27_MRIGASIRA, N27_ARUDRA, N27_PUNARVASU, N27_PUSHYAMI, N27_ASLESHA,
	N27_MAKHA, N27_PPHALGUNI, N27_UPHALGUNI, N27_HASTHA, N27_CHITTA, N27_SWATI, N27_VISAKHA, N27_ANURADHA, N27_JYESHTA,
	N27_MULA, N27_PSHADHA, N27_USHADHA, N27_SRAVANA, N27_DHANISHTA, N27_SATHABISHAK, N27_PBHADRA, N27_UBHADRA, N27_REVATI
};
#define Nakshatra NakshatraId_27
#define ASSERT_VALID_NAKSHATRA( r ) assert ( r >= N27_ASWINI && r <= N27_REVATI );
NakshatraId_27 operator++ ( NakshatraId_27& );
NakshatraId_27 operator++ ( NakshatraId_27&, int );

#define ABHIJIT_INDEX 21

#define NAKSHATRA_LEN 13.33333333333
#define NAKSHATRA_LEN_R 13.33333333333 // 360 / 27
#define NAKSHATRA_LEN_N 12.85714286    // 360/28
#define N27_N28_RATIO .964285714       // 27/28
#define N28_N27_RATIO 1.037037037      // 28/27
#define PADA_LEN 3.333333333333

#define USHADHA_BEGIN_R 266.6666666667
#define ABHIJIT_BEGIN_R 276.6666666667
#define SRAVANA_BEGIN_R 280.8888888889
#define DHANISHTA_BEGIN_R 293.6666666667

#define USHADHA_LEN_R 10
#define ABHIJIT_LEN_R 4.2222222222 // = 4:13:20;
#define SRAVANA_LEN_R 12.444444444 // = 12:26:40

// Dasa IDs
enum DasaId
{
	D_NONE = -1,
	D_VIMSOTTARI = 0, D_YOGINI, D_KALACHAKRA, D_ASHTOTTARI, D_SHODSHOTTARI,
	D_DVADASHOTTARI, D_PANCHOTTARI, D_SHATABDIKA, D_CHATURASHITISAMA,
	D_DVISAPTATISAMA, D_SHATTRIMSATSAMA, D_LAGNAVIMSOTTARI
};
#define MAX_DASAEXPERTS 12
#define D_FIRST D_VIMSOTTARI
#define D_LAST D_LAGNAVIMSOTTARI
#define ASSERT_VALID_DASA_ID( d ) assert( d >= D_FIRST && d <= D_LAST );

enum DIGNITY
{ DG_NONE = -1, DG_EXALTED, DG_DEBILATED, DG_MOOLATRIKONA, DG_SVAKSHETRA, DG_ADHIMITRA, DG_MITRA,
	DG_SAMA, DG_SATRU, DG_ADHISATRU
};

// Kala Velas
enum { KALAVELA_LORD_NONE = -1, KALAVELA_LORD1, KALAVELA_LORD2, KALAVELA_LORD3, KALAVELA_LORD4,
	KALAVELA_LORD5, KALAVELA_LORD6, KALAVELA_LORD7, KALAVELA_LORD8
};

enum { KALAVELA_PORTION_START = 0, KALAVELA_PORTION_MIDDLE, KALAVELA_PORTION_END };

enum GRADKREIS { GK_NONE = -1, GK_360 = 0, GK_180, GK_90, GK_45, GK_225, GK_1125, GK_5625  };
#define ASSERT_VALID_GRADKREIS(g) assert( g >= GK_360 && g <= GK_5625 );

GRADKREIS operator++ ( GRADKREIS& );
GRADKREIS operator++ ( GRADKREIS&, int );

/**************************************************************
***
**  View IDs
***
***************************************************************/
enum VIEW_ID { VIEW_TEXT = 0, VIEW_TRANSIT, VIEW_SBC, VIEW_SOLAR, VIEW_YOGA, VIEW_GRAPHICALDASA,
	VIEW_DASA_TREE, VIEW_DASA_COMPOSITE, VIEW_URANIAN, VIEW_URANIAN_CHART, VIEW_VARGA, VIEW_ASHTAKAVARGA,
	VIEW_VIMSOPAKABALA, VIEW_GRAPHIC, VIEW_GRAPHICGRID, VIEW_PRINT, VIEW_PLANETLIST, VIEW_HORA, VIEW_ECLIPSE,
	VIEW_PARTNER, VIEW_EPHEM, VIEW_MULTIPLE, VIEW_DIAGRAM,
	VIEW_SHADBALA, VIEW_YOGA_EDITOR, VIEW_EMPTY
};

#define LAST_VIEW_ID VIEW_EMPTY
#define IS_VALID_SIZE( s ) ( s.x > 0 && s.y > 0 )

// Font
//#define MAX_FONTS 13
#define MAX_FONTS 12
enum FONT_ID {  //FONT_HTML_DEFAULT = 0, 
	FONT_GRAPHIC_DEFAULT = 0, FONT_GRAPHIC_SYMBOLS,
	FONT_TEXT_DEFAULT, FONT_TEXT_HEADER, FONT_TEXT_TABLE_CONTENTS, FONT_TEXT_TABLE_HEADER, 
	FONT_PDF_DEFAULT, FONT_PDF_HEADER, FONT_PDF_TABLE_CONTENTS, FONT_PDF_TABLE_HEADER, FONT_PDF_TINY,
	FONT_PLAIN_DEFAULT };

#define ASSERT_VALID_FONT_ID( x ) assert( x >= 0 && x < MAX_FONTS );

/*************************************************//**
*
* \brief constants for text alignment
*
******************************************************/
struct Align
{
	static int Center, VCenter, Bottom, Top, HCenter, Left, Right;
	wxString toString( const int &a );
};

// writer export options
enum WidgetExportType { WeNone = 0, WeText, WeCsv, WeHtml, WePdf, WeImage, WePlainHtml };

/*************************************************//**
*
* \brief direction to the center of the chart resp. page
*
******************************************************/
enum GRAVITATION
{
	GRAVITATION_ERROR = -1,
	GRAVITATION_S = 0,
	GRAVITATION_SW,
	GRAVITATION_W,
	GRAVITATION_NW,
	GRAVITATION_N,
	GRAVITATION_NE,
	GRAVITATION_E,
	GRAVITATION_SE,
	GRAVITATION_CENTER
};

/*****************************************************
*
*
*
******************************************************/
#define DECLARE_SINGLETON_WITH_EXPLICITE_CONSTRUCTOR( c ) \
public: \
	static c *get() \
	{ \
		if ( ego == 0 ) ego = new c; \
		return ego; \
	}\
protected: \
	static c *ego;

#define DECLARE_SINGLETON( c ) \
public: \
	static c *get() \
	{ \
		if ( ego == 0 ) ego = new c; \
		return ego; \
	}\
protected: \
	static c *ego; \
	c() {} \
	c( const c& ) {}

#define IMPLEMENT_SINGLETON( c ) c *c::ego = 0;

/*************************************************//**
*
*    TABLE STUFF 
*
******************************************************/
#define NO_ROW (uint)-1
#define HEADER_ROW (uint)-2
#define NO_COL NO_ROW 
#define NO_ITEM NO_ROW 

enum TAB_LIST_CONTEXT { TAB_LC_NONE = -1, TAB_LC_EMPTY, TAB_LC_PLANETS, TAB_LC_HOUSE_CUSPS, TAB_LC_URANIAN, TAB_LC_ERROR };

#define MAX_TC_COLUMN_TYPE 24
enum TAB_COLUMN_TYPE
{
	TAB_CT_NONE                = 0x00000000,
	TAB_CT_OBJECT_NAME         = 0x00000001,
	TAB_CT_LONGITUDE           = 0x00000002,
	TAB_CT_LATITUDE            = 0x00000004,
	TAB_CT_NAME_LONGITUDE      = 0x00000008,
 	TAB_CT_SIGN_LORD           = 0x00000010,
	TAB_CT_DIGNITY             = 0x00000020,
	TAB_CT_NAVAMSA             = 0x00000040, 
 	TAB_CT_NAKSHATRA           = 0x00000080,
	TAB_CT_NAKSHATRA_PADA      = 0x00000100,
	TAB_CT_KP_LORDS            = 0x00000200,
	TAB_CT_SHASTIAMSA_LORD     = 0x00000400,
	TAB_CT_DASA_VARGA          = 0x00000800,
	TAB_CT_HOUSE_POSITION      = 0x00001000,
	TAB_CT_CHARA_KARAKA        = 0x00002000,
	TAB_CT_AV_REKHAPOINTS      = 0x00004000,
	TAB_CT_ELEMENT             = 0x00008000,
	TAB_CT_SIGN_QUALITY        = 0x00010000,
	TAB_CT_90_DEGREE_LONGITUDE = 0x00020000,
	TAB_CT_45_DEGREE_LONGITUDE = 0x00040000,
	TAB_CT_ANTISCIA            = 0x00080000,
	TAB_CT_SIGNIFICATION       = 0x00100000,
	TAB_CT_WESTERN_ASPECTARIUM = 0x00200000,
	TAB_CT_CUSTOM_KEY_VALUE    = 0x00400000,
	TAB_CT_EMPTY               = 0x00800000,
	TAB_CT_ERROR               = 0x01000000
};

TAB_COLUMN_TYPE operator& ( const TAB_COLUMN_TYPE&, const TAB_COLUMN_TYPE& );
TAB_COLUMN_TYPE operator| ( const TAB_COLUMN_TYPE&, const TAB_COLUMN_TYPE& );
TAB_COLUMN_TYPE operator~ ( const TAB_COLUMN_TYPE& );
void operator&= ( TAB_COLUMN_TYPE&, const TAB_COLUMN_TYPE& );
void operator|= ( TAB_COLUMN_TYPE&, const TAB_COLUMN_TYPE& );

enum TAB_CELL_TYPE { TAB_CELL_NONE = 0, TAB_CELL_NAME, TAB_CELL_FULL_DATE,
	TAB_CELL_LOCAL_TIME, TAB_CELL_UNIVERSAL_TIME,
	TAB_CELL_TZ_NAME, TAB_CELL_DST, 
	TAB_CELL_LOCATION_NAME, TAB_CELL_LOCATION_LONGITUDE, TAB_CELL_LOCATION_LATITUDE,

	TAB_CELL_WEEKDAY, TAB_CELL_JULIAN_DATE, TAB_CELL_SIDEREAL_TIME,
	TAB_CELL_VEDIC_AYANAMSA, TAB_CELL_WESTERN_AYANAMSA,
	TAB_CELL_HORA_LORD, TAB_CELL_DINA_LORD, TAB_CELL_MASA_LORD, TAB_CELL_VARSHA_LORD,
	TAB_CELL_LUNAR_TITHI, TAB_CELL_SUNRISE, TAB_CELL_SUNSET,
	TAB_CELL_VARNA, TAB_CELL_GANA, TAB_CELL_YONI, TAB_CELL_NADI, TAB_CELL_RAJJU,
	TAB_CELL_INCFLUENCE_TIME, TAB_CELL_ERROR
};

/*************************************************//**
*
* 
*
******************************************************/
class TcCell
{
public:
	TcCell( const TAB_CELL_TYPE &t ) : type( t )
	{
		ok = ( type == TAB_CELL_ERROR ? false : true );
	}
	
	TAB_CELL_TYPE type;
	wxString errorMsg;
	bool ok;
};

/*************************************************//**
*
* 
*
******************************************************/
class TcColumn 
{
public:
	TcColumn( const TAB_COLUMN_TYPE t = TAB_CT_NONE, const bool v = true )
	 : type( t ),
		 vedic( v )
	{
		ok = ( type == TAB_CT_ERROR ? false : true );
	}

	TAB_COLUMN_TYPE type;
	bool vedic;

	wxString title;
	vector<TcCell> cells;

	wxString errorMsg;
	bool ok;
};

/*************************************************//**
*
* 
*
******************************************************/
class TcColumnSet 
{
public:
	TcColumnSet( const TAB_LIST_CONTEXT l = TAB_LC_NONE, const bool v = true, wxString header = wxEmptyString )
	  : listcontext( l ),
	   vedic( v ),
		 header( header )
	{
		ok = true;
	}

	TAB_LIST_CONTEXT listcontext;
	bool vedic;

	wxString header;
	wxString errorMsg;
	bool ok;

	vector<TcColumn> cols;
};

/*************************************************//**
*
* 
*
******************************************************/
class Tc 
{
public:
	Tc() { ok = true; }

	vector<TcColumnSet> colsets;

	wxString errorMsg;
	bool ok;
};

#endif


