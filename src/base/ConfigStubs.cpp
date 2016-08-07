// generated file
#include <wx/jsonval.h>
#include "AspectConfig.h"
#include "ColorConfig.h"
#include "Conf.h"
#include "DataSet.h"
#include "FontProvider.h"
#include "JSonTool.h"
#include "mathbase.h"
#include "PrintoutConfig.h"
#include "PrintoutConfigBase.h"
#include "SheetConfig.h"
#include "VedicChartConfig.h"
#include "WesternChartConfig.h"


void AspectConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "active" ) ].IsNull()) active = v[ wxT( "active" ) ].AsBool();
	else reportMissingKey( wxT( "AspectConfig" ), wxT( "active" ));

	if ( ! v[ wxT( "orbis" ) ].IsNull()) orbis = myatof( v[ wxT( "orbis" ) ].AsString());
	else reportMissingKey( wxT( "AspectConfig" ), wxT( "orbis" ));

	if ( ! v[ wxT( "pen" ) ].IsNull()) tool.readPen( v[ wxT( "pen" ) ], pen );
	else reportMissingKey( wxT( "AspectConfig" ), wxT( "pen" ));

}

void AspectConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "active" ) ] = active;
	v[ wxT( "orbis" ) ] = printfDouble( orbis );
	v[ wxT( "pen" ) ] = tool.writePen( pen );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void BarDiagramStyle::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "name" ) ].IsNull()) name = v[ wxT( "name" ) ].AsString();
	else reportMissingKey( wxT( "BarDiagramStyle" ), wxT( "name" ));

	if ( ! v[ wxT( "pen" ) ].IsNull()) tool.readPen( v[ wxT( "pen" ) ], pen );
	else reportMissingKey( wxT( "BarDiagramStyle" ), wxT( "pen" ));

	if ( ! v[ wxT( "brush" ) ].IsNull()) tool.readBrush( v[ wxT( "brush" ) ], brush );
	else reportMissingKey( wxT( "BarDiagramStyle" ), wxT( "brush" ));

}

void BarDiagramStyle::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "name" ) ] = name;
	v[ wxT( "pen" ) ] = tool.writePen( pen );
	v[ wxT( "brush" ) ] = tool.writeBrush( brush );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void BarDiagramConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "showToolbarMessages" ) ].IsNull()) showToolbarMessages = v[ wxT( "showToolbarMessages" ) ].AsBool();
	else reportMissingKey( wxT( "BarDiagramConfig" ), wxT( "showToolbarMessages" ));

	if ( ! v[ wxT( "itemZoom" ) ].IsNull()) itemZoom = v[ wxT( "itemZoom" ) ].AsInt();
	else reportMissingKey( wxT( "BarDiagramConfig" ), wxT( "itemZoom" ));

	if ( ! v[ wxT( "cornerRadius" ) ].IsNull()) cornerRadius = v[ wxT( "cornerRadius" ) ].AsInt();
	else reportMissingKey( wxT( "BarDiagramConfig" ), wxT( "cornerRadius" ));

	if ( ! v[ wxT( "doItemZoom" ) ].IsNull()) doItemZoom = v[ wxT( "doItemZoom" ) ].AsBool();
	else reportMissingKey( wxT( "BarDiagramConfig" ), wxT( "doItemZoom" ));

}

void BarDiagramConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "showToolbarMessages" ) ] = showToolbarMessages;
	v[ wxT( "itemZoom" ) ] = itemZoom;
	v[ wxT( "cornerRadius" ) ] = cornerRadius;
	v[ wxT( "doItemZoom" ) ] = doItemZoom;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void BgPalette::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
}

void BgPalette::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void ColorConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "sbcMarkedFieldsBgColor" ) ].IsNull()) sbcMarkedFieldsBgColor = wxColour( v[ wxT( "sbcMarkedFieldsBgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "sbcMarkedFieldsBgColor" ));

	if ( ! v[ wxT( "bgColor" ) ].IsNull()) bgColor = wxColour( v[ wxT( "bgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "bgColor" ));

	if ( ! v[ wxT( "transitFgColor" ) ].IsNull()) transitFgColor = wxColour( v[ wxT( "transitFgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "transitFgColor" ));

	if ( ! v[ wxT( "maleficFgColor" ) ].IsNull()) maleficFgColor = wxColour( v[ wxT( "maleficFgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "maleficFgColor" ));

	if ( ! v[ wxT( "chartMarkupBgColor" ) ].IsNull()) chartMarkupBgColor = wxColour( v[ wxT( "chartMarkupBgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "chartMarkupBgColor" ));

	if ( ! v[ wxT( "fgColor" ) ].IsNull()) fgColor = wxColour( v[ wxT( "fgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "fgColor" ));

	if ( ! v[ wxT( "errorBgColor" ) ].IsNull()) errorBgColor = wxColour( v[ wxT( "errorBgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "errorBgColor" ));

	if ( ! v[ wxT( "beneficFgColor" ) ].IsNull()) beneficFgColor = wxColour( v[ wxT( "beneficFgColor" ) ].AsString());
	else reportMissingKey( wxT( "ColorConfig" ), wxT( "beneficFgColor" ));

}

void ColorConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "sbcMarkedFieldsBgColor" ) ] = sbcMarkedFieldsBgColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "bgColor" ) ] = bgColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "transitFgColor" ) ] = transitFgColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "maleficFgColor" ) ] = maleficFgColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "chartMarkupBgColor" ) ] = chartMarkupBgColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "fgColor" ) ] = fgColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "errorBgColor" ) ] = errorBgColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "beneficFgColor" ) ] = beneficFgColor.GetAsString( wxC2S_HTML_SYNTAX );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void Location::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "locName" ) ].IsNull()) locName = v[ wxT( "locName" ) ].AsString();
	else reportMissingKey( wxT( "Location" ), wxT( "locName" ));

	if ( ! v[ wxT( "longitude" ) ].IsNull()) longitude = myatof( v[ wxT( "longitude" ) ].AsString());
	else reportMissingKey( wxT( "Location" ), wxT( "longitude" ));

	if ( ! v[ wxT( "latitude" ) ].IsNull()) latitude = myatof( v[ wxT( "latitude" ) ].AsString());
	else reportMissingKey( wxT( "Location" ), wxT( "latitude" ));

	if ( ! v[ wxT( "tz" ) ].IsNull()) tz = myatof( v[ wxT( "tz" ) ].AsString());
	else reportMissingKey( wxT( "Location" ), wxT( "tz" ));

	if ( ! v[ wxT( "dst" ) ].IsNull()) dst = myatof( v[ wxT( "dst" ) ].AsString());
	else reportMissingKey( wxT( "Location" ), wxT( "dst" ));

}

void Location::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "locName" ) ] = locName;
	v[ wxT( "longitude" ) ] = printfDouble( longitude );
	v[ wxT( "latitude" ) ] = printfDouble( latitude );
	v[ wxT( "tz" ) ] = printfDouble( tz );
	v[ wxT( "dst" ) ] = printfDouble( dst );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void FontConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "facename" ) ].IsNull()) facename = v[ wxT( "facename" ) ].AsString();
	else reportMissingKey( wxT( "FontConfig" ), wxT( "facename" ));

	if ( ! v[ wxT( "weight" ) ].IsNull()) weight = v[ wxT( "weight" ) ].AsInt();
	else reportMissingKey( wxT( "FontConfig" ), wxT( "weight" ));

	if ( ! v[ wxT( "underline" ) ].IsNull()) underline = v[ wxT( "underline" ) ].AsInt();
	else reportMissingKey( wxT( "FontConfig" ), wxT( "underline" ));

	if ( ! v[ wxT( "pointSize" ) ].IsNull()) pointSize = v[ wxT( "pointSize" ) ].AsInt();
	else reportMissingKey( wxT( "FontConfig" ), wxT( "pointSize" ));

	if ( ! v[ wxT( "family" ) ].IsNull()) family = v[ wxT( "family" ) ].AsInt();
	else reportMissingKey( wxT( "FontConfig" ), wxT( "family" ));

	if ( ! v[ wxT( "style" ) ].IsNull()) style = v[ wxT( "style" ) ].AsInt();
	else reportMissingKey( wxT( "FontConfig" ), wxT( "style" ));

	if ( ! v[ wxT( "ok" ) ].IsNull()) ok = v[ wxT( "ok" ) ].AsBool();
	else reportMissingKey( wxT( "FontConfig" ), wxT( "ok" ));

}

void FontConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "facename" ) ] = facename;
	v[ wxT( "weight" ) ] = weight;
	v[ wxT( "underline" ) ] = underline;
	v[ wxT( "pointSize" ) ] = pointSize;
	v[ wxT( "family" ) ] = family;
	v[ wxT( "style" ) ] = style;
	v[ wxT( "ok" ) ] = ok;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void PrintoutConfigItem::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "type" ) ].IsNull()) type = v[ wxT( "type" ) ].AsString();
	else reportMissingKey( wxT( "PrintoutConfigItem" ), wxT( "type" ));

	if ( ! v[ wxT( "subtype" ) ].IsNull()) subtype = v[ wxT( "subtype" ) ].AsInt();
	else reportMissingKey( wxT( "PrintoutConfigItem" ), wxT( "subtype" ));

	if ( ! v[ wxT( "vedic" ) ].IsNull()) vedic = v[ wxT( "vedic" ) ].AsBool();
	else reportMissingKey( wxT( "PrintoutConfigItem" ), wxT( "vedic" ));

	if ( ! v[ wxT( "dasaId" ) ].IsNull()) dasaId = v[ wxT( "dasaId" ) ].AsInt();
	else reportMissingKey( wxT( "PrintoutConfigItem" ), wxT( "dasaId" ));

	if ( ! v[ wxT( "vargaIds" ) ].IsNull()) tool.readIntVector( v[ wxT( "vargaIds" ) ], vargaIds );
}

void PrintoutConfigItem::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "type" ) ] = type;
	v[ wxT( "subtype" ) ] = subtype;
	v[ wxT( "vedic" ) ] = vedic;
	v[ wxT( "dasaId" ) ] = dasaId;
	v[ wxT( "vargaIds" ) ] = tool.writeIntVector( vargaIds );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void PrintoutConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "description" ) ].IsNull()) description = v[ wxT( "description" ) ].AsString();
	else reportMissingKey( wxT( "PrintoutConfig" ), wxT( "description" ));

	if ( ! v[ wxT( "name" ) ].IsNull()) name = v[ wxT( "name" ) ].AsString();
	else reportMissingKey( wxT( "PrintoutConfig" ), wxT( "name" ));

	if ( ! v[ wxT( "vedic" ) ].IsNull()) vedic = v[ wxT( "vedic" ) ].AsBool();
	else reportMissingKey( wxT( "PrintoutConfig" ), wxT( "vedic" ));

}

void PrintoutConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "description" ) ] = description;
	v[ wxT( "name" ) ] = name;
	v[ wxT( "vedic" ) ] = vedic;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void PdfCustomTextField::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "showSeparator" ) ].IsNull()) showSeparator = v[ wxT( "showSeparator" ) ].AsBool();
	else reportMissingKey( wxT( "PdfCustomTextField" ), wxT( "showSeparator" ));

	if ( ! v[ wxT( "text" ) ].IsNull()) text = v[ wxT( "text" ) ].AsString();
	else reportMissingKey( wxT( "PdfCustomTextField" ), wxT( "text" ));

	if ( ! v[ wxT( "showOnFirstPage" ) ].IsNull()) showOnFirstPage = v[ wxT( "showOnFirstPage" ) ].AsBool();
	else reportMissingKey( wxT( "PdfCustomTextField" ), wxT( "showOnFirstPage" ));

	if ( ! v[ wxT( "orientation" ) ].IsNull()) orientation = v[ wxT( "orientation" ) ].AsInt();
	else reportMissingKey( wxT( "PdfCustomTextField" ), wxT( "orientation" ));

	if ( ! v[ wxT( "enabled" ) ].IsNull()) enabled = v[ wxT( "enabled" ) ].AsBool();
	else reportMissingKey( wxT( "PdfCustomTextField" ), wxT( "enabled" ));

}

void PdfCustomTextField::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "showSeparator" ) ] = showSeparator;
	v[ wxT( "text" ) ] = text;
	v[ wxT( "showOnFirstPage" ) ] = showOnFirstPage;
	v[ wxT( "orientation" ) ] = orientation;
	v[ wxT( "enabled" ) ] = enabled;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void PdfDocumentConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "pdfViewerCommand" ) ].IsNull()) pdfViewerCommand = v[ wxT( "pdfViewerCommand" ) ].AsString();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "pdfViewerCommand" ));

	if ( ! v[ wxT( "customHeader" ) ].IsNull()) customHeader.load( v, wxT( "customHeader" ) );
	if ( ! v[ wxT( "compressPdf" ) ].IsNull()) compressPdf = v[ wxT( "compressPdf" ) ].AsBool();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "compressPdf" ));

	if ( ! v[ wxT( "vGraphicSkin" ) ].IsNull()) vGraphicSkin = v[ wxT( "vGraphicSkin" ) ].AsInt();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "vGraphicSkin" ));

	if ( ! v[ wxT( "defaultPrintout" ) ].IsNull()) defaultPrintout = v[ wxT( "defaultPrintout" ) ].AsInt();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "defaultPrintout" ));

	if ( ! v[ wxT( "customFooter" ) ].IsNull()) customFooter.load( v, wxT( "customFooter" ) );
	if ( ! v[ wxT( "paperFormat" ) ].IsNull()) paperFormat = v[ wxT( "paperFormat" ) ].AsInt();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "paperFormat" ));

	if ( ! v[ wxT( "launchPdfViewer" ) ].IsNull()) launchPdfViewer = v[ wxT( "launchPdfViewer" ) ].AsBool();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "launchPdfViewer" ));

	if ( ! v[ wxT( "saveFileOption" ) ].IsNull()) saveFileOption = v[ wxT( "saveFileOption" ) ].AsBool();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "saveFileOption" ));

	if ( ! v[ wxT( "showSelectDialog" ) ].IsNull()) showSelectDialog = v[ wxT( "showSelectDialog" ) ].AsBool();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "showSelectDialog" ));

	if ( ! v[ wxT( "sheetStyle" ) ].IsNull()) sheetStyle = v[ wxT( "sheetStyle" ) ].AsInt();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "sheetStyle" ));

	if ( ! v[ wxT( "askOverwrite" ) ].IsNull()) askOverwrite = v[ wxT( "askOverwrite" ) ].AsBool();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "askOverwrite" ));

	if ( ! v[ wxT( "wGraphicSkin" ) ].IsNull()) wGraphicSkin = v[ wxT( "wGraphicSkin" ) ].AsInt();
	else reportMissingKey( wxT( "PdfDocumentConfig" ), wxT( "wGraphicSkin" ));

}

void PdfDocumentConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "pdfViewerCommand" ) ] = pdfViewerCommand;
	customHeader.save( v, wxT( "customHeader" ) );
	v[ wxT( "compressPdf" ) ] = compressPdf;
	v[ wxT( "vGraphicSkin" ) ] = vGraphicSkin;
	v[ wxT( "defaultPrintout" ) ] = defaultPrintout;
	customFooter.save( v, wxT( "customFooter" ) );
	v[ wxT( "paperFormat" ) ] = paperFormat;
	v[ wxT( "launchPdfViewer" ) ] = launchPdfViewer;
	v[ wxT( "saveFileOption" ) ] = saveFileOption;
	v[ wxT( "showSelectDialog" ) ] = showSelectDialog;
	v[ wxT( "sheetStyle" ) ] = sheetStyle;
	v[ wxT( "askOverwrite" ) ] = askOverwrite;
	v[ wxT( "wGraphicSkin" ) ] = wGraphicSkin;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void TableStyle::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "cellBgMode" ) ].IsNull()) cellBgMode = v[ wxT( "cellBgMode" ) ].AsInt();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "cellBgMode" ));

	if ( ! v[ wxT( "oddCellBrush" ) ].IsNull()) tool.readBrush( v[ wxT( "oddCellBrush" ) ], oddCellBrush );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "oddCellBrush" ));

	if ( ! v[ wxT( "gridStyle" ) ].IsNull()) gridStyle = v[ wxT( "gridStyle" ) ].AsInt();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "gridStyle" ));

	if ( ! v[ wxT( "shadowWidth" ) ].IsNull()) shadowWidth = v[ wxT( "shadowWidth" ) ].AsInt();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "shadowWidth" ));

	if ( ! v[ wxT( "outerBorderPen" ) ].IsNull()) tool.readPen( v[ wxT( "outerBorderPen" ) ], outerBorderPen );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "outerBorderPen" ));

	if ( ! v[ wxT( "cellBorderStyle" ) ].IsNull()) cellBorderStyle = v[ wxT( "cellBorderStyle" ) ].AsInt();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "cellBorderStyle" ));

	if ( ! v[ wxT( "shadowStyle" ) ].IsNull()) shadowStyle = v[ wxT( "shadowStyle" ) ].AsInt();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "shadowStyle" ));

	if ( ! v[ wxT( "headerBorderPen" ) ].IsNull()) tool.readPen( v[ wxT( "headerBorderPen" ) ], headerBorderPen );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "headerBorderPen" ));

	if ( ! v[ wxT( "cellBorderPen" ) ].IsNull()) tool.readPen( v[ wxT( "cellBorderPen" ) ], cellBorderPen );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "cellBorderPen" ));

	if ( ! v[ wxT( "useHeaderColors" ) ].IsNull()) useHeaderColors = v[ wxT( "useHeaderColors" ) ].AsBool();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "useHeaderColors" ));

	if ( ! v[ wxT( "outerBorderStyle" ) ].IsNull()) outerBorderStyle = v[ wxT( "outerBorderStyle" ) ].AsInt();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "outerBorderStyle" ));

	if ( ! v[ wxT( "headerTextColor" ) ].IsNull()) headerTextColor = wxColour( v[ wxT( "headerTextColor" ) ].AsString());
	else reportMissingKey( wxT( "TableStyle" ), wxT( "headerTextColor" ));

	if ( ! v[ wxT( "evenCellBrush" ) ].IsNull()) tool.readBrush( v[ wxT( "evenCellBrush" ) ], evenCellBrush );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "evenCellBrush" ));

	if ( ! v[ wxT( "cellBrush" ) ].IsNull()) tool.readBrush( v[ wxT( "cellBrush" ) ], cellBrush );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "cellBrush" ));

	if ( ! v[ wxT( "headerBrush" ) ].IsNull()) tool.readBrush( v[ wxT( "headerBrush" ) ], headerBrush );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "headerBrush" ));

	if ( ! v[ wxT( "useCellColors" ) ].IsNull()) useCellColors = v[ wxT( "useCellColors" ) ].AsBool();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "useCellColors" ));

	if ( ! v[ wxT( "headerBorderStyle" ) ].IsNull()) headerBorderStyle = v[ wxT( "headerBorderStyle" ) ].AsInt();
	else reportMissingKey( wxT( "TableStyle" ), wxT( "headerBorderStyle" ));

	if ( ! v[ wxT( "shadowBrush" ) ].IsNull()) tool.readBrush( v[ wxT( "shadowBrush" ) ], shadowBrush );
	else reportMissingKey( wxT( "TableStyle" ), wxT( "shadowBrush" ));

	if ( ! v[ wxT( "cellTextColor" ) ].IsNull()) cellTextColor = wxColour( v[ wxT( "cellTextColor" ) ].AsString());
	else reportMissingKey( wxT( "TableStyle" ), wxT( "cellTextColor" ));

}

void TableStyle::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "cellBgMode" ) ] = cellBgMode;
	v[ wxT( "oddCellBrush" ) ] = tool.writeBrush( oddCellBrush );
	v[ wxT( "gridStyle" ) ] = gridStyle;
	v[ wxT( "shadowWidth" ) ] = shadowWidth;
	v[ wxT( "outerBorderPen" ) ] = tool.writePen( outerBorderPen );
	v[ wxT( "cellBorderStyle" ) ] = cellBorderStyle;
	v[ wxT( "shadowStyle" ) ] = shadowStyle;
	v[ wxT( "headerBorderPen" ) ] = tool.writePen( headerBorderPen );
	v[ wxT( "cellBorderPen" ) ] = tool.writePen( cellBorderPen );
	v[ wxT( "useHeaderColors" ) ] = useHeaderColors;
	v[ wxT( "outerBorderStyle" ) ] = outerBorderStyle;
	v[ wxT( "headerTextColor" ) ] = headerTextColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "evenCellBrush" ) ] = tool.writeBrush( evenCellBrush );
	v[ wxT( "cellBrush" ) ] = tool.writeBrush( cellBrush );
	v[ wxT( "headerBrush" ) ] = tool.writeBrush( headerBrush );
	v[ wxT( "useCellColors" ) ] = useCellColors;
	v[ wxT( "headerBorderStyle" ) ] = headerBorderStyle;
	v[ wxT( "shadowBrush" ) ] = tool.writeBrush( shadowBrush );
	v[ wxT( "cellTextColor" ) ] = cellTextColor.GetAsString( wxC2S_HTML_SYNTAX );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void SheetConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "name" ) ].IsNull()) name = v[ wxT( "name" ) ].AsString();
	else reportMissingKey( wxT( "SheetConfig" ), wxT( "name" ));

	if ( ! v[ wxT( "selectionMode" ) ].IsNull()) selectionMode = v[ wxT( "selectionMode" ) ].AsInt();
	else reportMissingKey( wxT( "SheetConfig" ), wxT( "selectionMode" ));

	if ( ! v[ wxT( "tablestyle" ) ].IsNull()) tablestyle.load( v, wxT( "tablestyle" ) );
	if ( ! v[ wxT( "textColor" ) ].IsNull()) textColor = wxColour( v[ wxT( "textColor" ) ].AsString());
	else reportMissingKey( wxT( "SheetConfig" ), wxT( "textColor" ));

	if ( ! v[ wxT( "brush" ) ].IsNull()) tool.readBrush( v[ wxT( "brush" ) ], brush );
	else reportMissingKey( wxT( "SheetConfig" ), wxT( "brush" ));

	if ( ! v[ wxT( "selectedItemBrush" ) ].IsNull()) tool.readBrush( v[ wxT( "selectedItemBrush" ) ], selectedItemBrush );
	else reportMissingKey( wxT( "SheetConfig" ), wxT( "selectedItemBrush" ));

}

void SheetConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "name" ) ] = name;
	v[ wxT( "selectionMode" ) ] = selectionMode;
	tablestyle.save( v, wxT( "tablestyle" ) );
	v[ wxT( "textColor" ) ] = textColor.GetAsString( wxC2S_HTML_SYNTAX );
	v[ wxT( "brush" ) ] = tool.writeBrush( brush );
	v[ wxT( "selectedItemBrush" ) ] = tool.writeBrush( selectedItemBrush );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void WesternChartBehaviorConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "magnifyAspects" ) ].IsNull()) magnifyAspects = v[ wxT( "magnifyAspects" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartBehaviorConfig" ), wxT( "magnifyAspects" ));

	if ( ! v[ wxT( "zoomOnMouseOver" ) ].IsNull()) zoomOnMouseOver = v[ wxT( "zoomOnMouseOver" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartBehaviorConfig" ), wxT( "zoomOnMouseOver" ));

	if ( ! v[ wxT( "zoomFactor" ) ].IsNull()) zoomFactor = v[ wxT( "zoomFactor" ) ].AsInt();
	else reportMissingKey( wxT( "WesternChartBehaviorConfig" ), wxT( "zoomFactor" ));

}

void WesternChartBehaviorConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "magnifyAspects" ) ] = magnifyAspects;
	v[ wxT( "zoomOnMouseOver" ) ] = zoomOnMouseOver;
	v[ wxT( "zoomFactor" ) ] = zoomFactor;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void VedicChartBehaviorConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "highlightOnMouseOver" ) ].IsNull()) highlightOnMouseOver = v[ wxT( "highlightOnMouseOver" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "highlightOnMouseOver" ));

	if ( ! v[ wxT( "aspectedObjectsMode" ) ].IsNull()) aspectedObjectsMode = v[ wxT( "aspectedObjectsMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "aspectedObjectsMode" ));

	if ( ! v[ wxT( "zoomFactor" ) ].IsNull()) zoomFactor = v[ wxT( "zoomFactor" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "zoomFactor" ));

	if ( ! v[ wxT( "grahaDrishtiMinimumStrength" ) ].IsNull()) grahaDrishtiMinimumStrength = (VEDICASPECT_STRENGTH)v[ wxT( "grahaDrishtiMinimumStrength" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "grahaDrishtiMinimumStrength" ));

	if ( ! v[ wxT( "aspectingObjectsMode" ) ].IsNull()) aspectingObjectsMode = v[ wxT( "aspectingObjectsMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "aspectingObjectsMode" ));

	if ( ! v[ wxT( "beneficMaleficDisplay" ) ].IsNull()) beneficMaleficDisplay = v[ wxT( "beneficMaleficDisplay" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "beneficMaleficDisplay" ));

	if ( ! v[ wxT( "zoomOnMouseOver" ) ].IsNull()) zoomOnMouseOver = v[ wxT( "zoomOnMouseOver" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "zoomOnMouseOver" ));

	if ( ! v[ wxT( "aspectType" ) ].IsNull()) aspectType = v[ wxT( "aspectType" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "aspectType" ));

	if ( ! v[ wxT( "allAspectsMode" ) ].IsNull()) allAspectsMode = v[ wxT( "allAspectsMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "allAspectsMode" ));

	if ( ! v[ wxT( "includeAsMc" ) ].IsNull()) includeAsMc = v[ wxT( "includeAsMc" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartBehaviorConfig" ), wxT( "includeAsMc" ));

}

void VedicChartBehaviorConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "highlightOnMouseOver" ) ] = highlightOnMouseOver;
	v[ wxT( "aspectedObjectsMode" ) ] = aspectedObjectsMode;
	v[ wxT( "zoomFactor" ) ] = zoomFactor;
	v[ wxT( "grahaDrishtiMinimumStrength" ) ] = grahaDrishtiMinimumStrength;
	v[ wxT( "aspectingObjectsMode" ) ] = aspectingObjectsMode;
	v[ wxT( "beneficMaleficDisplay" ) ] = beneficMaleficDisplay;
	v[ wxT( "zoomOnMouseOver" ) ] = zoomOnMouseOver;
	v[ wxT( "aspectType" ) ] = aspectType;
	v[ wxT( "allAspectsMode" ) ] = allAspectsMode;
	v[ wxT( "includeAsMc" ) ] = includeAsMc;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void WriterConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "vedicSignNames" ) ].IsNull()) vedicSignNames = v[ wxT( "vedicSignNames" ) ].AsBool();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "vedicSignNames" ));

	if ( ! v[ wxT( "vedicPlanetNames" ) ].IsNull()) vedicPlanetNames = v[ wxT( "vedicPlanetNames" ) ].AsBool();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "vedicPlanetNames" ));

	if ( ! v[ wxT( "signSymbols" ) ].IsNull()) signSymbols = v[ wxT( "signSymbols" ) ].AsBool();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "signSymbols" ));

	if ( ! v[ wxT( "plutoSymbol" ) ].IsNull()) plutoSymbol = v[ wxT( "plutoSymbol" ) ].AsInt();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "plutoSymbol" ));

	if ( ! v[ wxT( "vedicPositions" ) ].IsNull()) vedicPositions = v[ wxT( "vedicPositions" ) ].AsBool();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "vedicPositions" ));

	if ( ! v[ wxT( "planetSymbols" ) ].IsNull()) planetSymbols = v[ wxT( "planetSymbols" ) ].AsBool();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "planetSymbols" ));

	if ( ! v[ wxT( "capricornSymbol" ) ].IsNull()) capricornSymbol = v[ wxT( "capricornSymbol" ) ].AsInt();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "capricornSymbol" ));

	if ( ! v[ wxT( "uranusSymbol" ) ].IsNull()) uranusSymbol = v[ wxT( "uranusSymbol" ) ].AsInt();
	else reportMissingKey( wxT( "WriterConfig" ), wxT( "uranusSymbol" ));

}

void WriterConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "vedicSignNames" ) ] = vedicSignNames;
	v[ wxT( "vedicPlanetNames" ) ] = vedicPlanetNames;
	v[ wxT( "signSymbols" ) ] = signSymbols;
	v[ wxT( "plutoSymbol" ) ] = plutoSymbol;
	v[ wxT( "vedicPositions" ) ] = vedicPositions;
	v[ wxT( "planetSymbols" ) ] = planetSymbols;
	v[ wxT( "capricornSymbol" ) ] = capricornSymbol;
	v[ wxT( "uranusSymbol" ) ] = uranusSymbol;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void WesternCalculationConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "yearLength" ) ].IsNull()) yearLength = v[ wxT( "yearLength" ) ].AsInt();
	else reportMissingKey( wxT( "WesternCalculationConfig" ), wxT( "yearLength" ));

	if ( ! v[ wxT( "houseSystem" ) ].IsNull()) houseSystem = (HOUSE_SYSTEM)v[ wxT( "houseSystem" ) ].AsInt();
	else reportMissingKey( wxT( "WesternCalculationConfig" ), wxT( "houseSystem" ));

	if ( ! v[ wxT( "ayanamsa" ) ].IsNull()) ayanamsa = (AYANAMSA)v[ wxT( "ayanamsa" ) ].AsInt();
	else reportMissingKey( wxT( "WesternCalculationConfig" ), wxT( "ayanamsa" ));

	if ( ! v[ wxT( "customYearLength" ) ].IsNull()) customYearLength = myatof( v[ wxT( "customYearLength" ) ].AsString());
	else reportMissingKey( wxT( "WesternCalculationConfig" ), wxT( "customYearLength" ));

	if ( ! v[ wxT( "lunarNodeMode" ) ].IsNull()) lunarNodeMode = v[ wxT( "lunarNodeMode" ) ].AsInt();
	else reportMissingKey( wxT( "WesternCalculationConfig" ), wxT( "lunarNodeMode" ));

}

void WesternCalculationConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "yearLength" ) ] = yearLength;
	v[ wxT( "houseSystem" ) ] = houseSystem;
	v[ wxT( "ayanamsa" ) ] = ayanamsa;
	v[ wxT( "customYearLength" ) ] = printfDouble( customYearLength );
	v[ wxT( "lunarNodeMode" ) ] = lunarNodeMode;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void WesternConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "arabicParts" ) ].IsNull()) tool.readObjectArray( v[ wxT( "arabicParts" ) ], arabicParts );
	if ( ! v[ wxT( "columnStyle" ) ].IsNull()) columnStyle = (TAB_COLUMN_TYPE)v[ wxT( "columnStyle" ) ].AsInt();
	else reportMissingKey( wxT( "WesternConfig" ), wxT( "columnStyle" ));

	if ( ! v[ wxT( "planetOrder" ) ].IsNull()) planetOrder = v[ wxT( "planetOrder" ) ].AsInt();
	else reportMissingKey( wxT( "WesternConfig" ), wxT( "planetOrder" ));

	if ( ! v[ wxT( "objects" ) ].IsNull()) objects = (OBJECT_INCLUDES)v[ wxT( "objects" ) ].AsInt();
	else reportMissingKey( wxT( "WesternConfig" ), wxT( "objects" ));

}

void WesternConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "arabicParts" ) ] = tool.writeObjectArray( arabicParts );
	v[ wxT( "columnStyle" ) ] = columnStyle;
	v[ wxT( "planetOrder" ) ] = planetOrder;
	v[ wxT( "objects" ) ] = objects;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void UranianConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "eventsIncludeSums" ) ].IsNull()) eventsIncludeSums = v[ wxT( "eventsIncludeSums" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "eventsIncludeSums" ));

	if ( ! v[ wxT( "transitIncludeRunningPairs" ) ].IsNull()) transitIncludeRunningPairs = v[ wxT( "transitIncludeRunningPairs" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "transitIncludeRunningPairs" ));

	if ( ! v[ wxT( "gradkreis" ) ].IsNull()) gradkreis = (GRADKREIS)v[ wxT( "gradkreis" ) ].AsInt();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "gradkreis" ));

	if ( ! v[ wxT( "eventsIncludeMidpoints" ) ].IsNull()) eventsIncludeMidpoints = v[ wxT( "eventsIncludeMidpoints" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "eventsIncludeMidpoints" ));

	if ( ! v[ wxT( "orbisRadix" ) ].IsNull()) orbisRadix = myatof( v[ wxT( "orbisRadix" ) ].AsString());
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "orbisRadix" ));

	if ( ! v[ wxT( "eventsIncludeDifferences" ) ].IsNull()) eventsIncludeDifferences = v[ wxT( "eventsIncludeDifferences" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "eventsIncludeDifferences" ));

	if ( ! v[ wxT( "sortOrder" ) ].IsNull()) sortOrder = (ASPECT_SORTORDER)v[ wxT( "sortOrder" ) ].AsInt();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "sortOrder" ));

	if ( ! v[ wxT( "orbisPartner" ) ].IsNull()) orbisPartner = myatof( v[ wxT( "orbisPartner" ) ].AsString());
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "orbisPartner" ));

	if ( ! v[ wxT( "orbisTransit" ) ].IsNull()) orbisTransit = myatof( v[ wxT( "orbisTransit" ) ].AsString());
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "orbisTransit" ));

	if ( ! v[ wxT( "transitIncludedObjects" ) ].IsNull()) tool.readObjectArray( v[ wxT( "transitIncludedObjects" ) ], transitIncludedObjects );
	if ( ! v[ wxT( "directionIncludeType" ) ].IsNull()) directionIncludeType = v[ wxT( "directionIncludeType" ) ].AsInt();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "directionIncludeType" ));

	if ( ! v[ wxT( "directionIncludedObjects" ) ].IsNull()) tool.readObjectArray( v[ wxT( "directionIncludedObjects" ) ], directionIncludedObjects );
	if ( ! v[ wxT( "directionIncludeRunningPairs" ) ].IsNull()) directionIncludeRunningPairs = v[ wxT( "directionIncludeRunningPairs" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "directionIncludeRunningPairs" ));

	if ( ! v[ wxT( "radixIncludeAntiscia" ) ].IsNull()) radixIncludeAntiscia = v[ wxT( "radixIncludeAntiscia" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "radixIncludeAntiscia" ));

	if ( ! v[ wxT( "orbisSolararc" ) ].IsNull()) orbisSolararc = myatof( v[ wxT( "orbisSolararc" ) ].AsString());
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "orbisSolararc" ));

	if ( ! v[ wxT( "directionIncludeAntiscia" ) ].IsNull()) directionIncludeAntiscia = v[ wxT( "directionIncludeAntiscia" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "directionIncludeAntiscia" ));

	if ( ! v[ wxT( "transitIncludeAntiscia" ) ].IsNull()) transitIncludeAntiscia = v[ wxT( "transitIncludeAntiscia" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "transitIncludeAntiscia" ));

	if ( ! v[ wxT( "partnerIncludeAntiscia" ) ].IsNull()) partnerIncludeAntiscia = v[ wxT( "partnerIncludeAntiscia" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "partnerIncludeAntiscia" ));

	if ( ! v[ wxT( "solararcIncludeRunningPairs" ) ].IsNull()) solararcIncludeRunningPairs = v[ wxT( "solararcIncludeRunningPairs" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "solararcIncludeRunningPairs" ));

	if ( ! v[ wxT( "eventsIncludeTriples" ) ].IsNull()) eventsIncludeTriples = v[ wxT( "eventsIncludeTriples" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "eventsIncludeTriples" ));

	if ( ! v[ wxT( "objectFilter" ) ].IsNull()) tool.readObjectFilter( v[ wxT( "objectFilter" ) ], objectFilter );
	if ( ! v[ wxT( "transitIncludeType" ) ].IsNull()) transitIncludeType = v[ wxT( "transitIncludeType" ) ].AsInt();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "transitIncludeType" ));

	if ( ! v[ wxT( "orbisDirection" ) ].IsNull()) orbisDirection = myatof( v[ wxT( "orbisDirection" ) ].AsString());
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "orbisDirection" ));

	if ( ! v[ wxT( "solararcIncludeAntiscia" ) ].IsNull()) solararcIncludeAntiscia = v[ wxT( "solararcIncludeAntiscia" ) ].AsBool();
	else reportMissingKey( wxT( "UranianConfig" ), wxT( "solararcIncludeAntiscia" ));

}

void UranianConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "eventsIncludeSums" ) ] = eventsIncludeSums;
	v[ wxT( "transitIncludeRunningPairs" ) ] = transitIncludeRunningPairs;
	v[ wxT( "gradkreis" ) ] = gradkreis;
	v[ wxT( "eventsIncludeMidpoints" ) ] = eventsIncludeMidpoints;
	v[ wxT( "orbisRadix" ) ] = printfDouble( orbisRadix );
	v[ wxT( "eventsIncludeDifferences" ) ] = eventsIncludeDifferences;
	v[ wxT( "sortOrder" ) ] = sortOrder;
	v[ wxT( "orbisPartner" ) ] = printfDouble( orbisPartner );
	v[ wxT( "orbisTransit" ) ] = printfDouble( orbisTransit );
	v[ wxT( "transitIncludedObjects" ) ] = tool.writeObjectArray( transitIncludedObjects );
	v[ wxT( "directionIncludeType" ) ] = directionIncludeType;
	v[ wxT( "directionIncludedObjects" ) ] = tool.writeObjectArray( directionIncludedObjects );
	v[ wxT( "directionIncludeRunningPairs" ) ] = directionIncludeRunningPairs;
	v[ wxT( "radixIncludeAntiscia" ) ] = radixIncludeAntiscia;
	v[ wxT( "orbisSolararc" ) ] = printfDouble( orbisSolararc );
	v[ wxT( "directionIncludeAntiscia" ) ] = directionIncludeAntiscia;
	v[ wxT( "transitIncludeAntiscia" ) ] = transitIncludeAntiscia;
	v[ wxT( "partnerIncludeAntiscia" ) ] = partnerIncludeAntiscia;
	v[ wxT( "solararcIncludeRunningPairs" ) ] = solararcIncludeRunningPairs;
	v[ wxT( "eventsIncludeTriples" ) ] = eventsIncludeTriples;
	v[ wxT( "objectFilter" ) ] = tool.writeObjectFilter( objectFilter );
	v[ wxT( "transitIncludeType" ) ] = transitIncludeType;
	v[ wxT( "orbisDirection" ) ] = printfDouble( orbisDirection );
	v[ wxT( "solararcIncludeAntiscia" ) ] = solararcIncludeAntiscia;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void WesternChartDisplayConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "showAspectSymbols" ) ].IsNull()) showAspectSymbols = v[ wxT( "showAspectSymbols" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showAspectSymbols" ));

	if ( ! v[ wxT( "showHouseColors" ) ].IsNull()) showHouseColors = v[ wxT( "showHouseColors" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showHouseColors" ));

	if ( ! v[ wxT( "transitStyle" ) ].IsNull()) transitStyle = v[ wxT( "transitStyle" ) ].AsInt();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "transitStyle" ));

	if ( ! v[ wxT( "houseNumberStyle" ) ].IsNull()) houseNumberStyle = v[ wxT( "houseNumberStyle" ) ].AsInt();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "houseNumberStyle" ));

	if ( ! v[ wxT( "showRetro" ) ].IsNull()) showRetro = v[ wxT( "showRetro" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showRetro" ));

	if ( ! v[ wxT( "graphicSkin" ) ].IsNull()) graphicSkin = v[ wxT( "graphicSkin" ) ].AsInt();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "graphicSkin" ));

	if ( ! v[ wxT( "showPlanetColors" ) ].IsNull()) showPlanetColors = v[ wxT( "showPlanetColors" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showPlanetColors" ));

	if ( ! v[ wxT( "showSignColors" ) ].IsNull()) showSignColors = v[ wxT( "showSignColors" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showSignColors" ));

	if ( ! v[ wxT( "showHouses" ) ].IsNull()) showHouses = v[ wxT( "showHouses" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showHouses" ));

	if ( ! v[ wxT( "chartOrientation" ) ].IsNull()) chartOrientation = v[ wxT( "chartOrientation" ) ].AsInt();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "chartOrientation" ));

	if ( ! v[ wxT( "showAspects" ) ].IsNull()) showAspects = v[ wxT( "showAspects" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showAspects" ));

	if ( ! v[ wxT( "showAspectColors" ) ].IsNull()) showAspectColors = v[ wxT( "showAspectColors" ) ].AsBool();
	else reportMissingKey( wxT( "WesternChartDisplayConfig" ), wxT( "showAspectColors" ));

}

void WesternChartDisplayConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "showAspectSymbols" ) ] = showAspectSymbols;
	v[ wxT( "showHouseColors" ) ] = showHouseColors;
	v[ wxT( "transitStyle" ) ] = transitStyle;
	v[ wxT( "houseNumberStyle" ) ] = houseNumberStyle;
	v[ wxT( "showRetro" ) ] = showRetro;
	v[ wxT( "graphicSkin" ) ] = graphicSkin;
	v[ wxT( "showPlanetColors" ) ] = showPlanetColors;
	v[ wxT( "showSignColors" ) ] = showSignColors;
	v[ wxT( "showHouses" ) ] = showHouses;
	v[ wxT( "chartOrientation" ) ] = chartOrientation;
	v[ wxT( "showAspects" ) ] = showAspects;
	v[ wxT( "showAspectColors" ) ] = showAspectColors;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void VedicConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "orderLagna" ) ].IsNull()) orderLagna = v[ wxT( "orderLagna" ) ].AsInt();
	else reportMissingKey( wxT( "VedicConfig" ), wxT( "orderLagna" ));

	if ( ! v[ wxT( "objects" ) ].IsNull()) objects = (OBJECT_INCLUDES)v[ wxT( "objects" ) ].AsInt();
	else reportMissingKey( wxT( "VedicConfig" ), wxT( "objects" ));

	if ( ! v[ wxT( "arabicParts" ) ].IsNull()) tool.readObjectArray( v[ wxT( "arabicParts" ) ], arabicParts );
	if ( ! v[ wxT( "columnStyle" ) ].IsNull()) columnStyle = (TAB_COLUMN_TYPE)v[ wxT( "columnStyle" ) ].AsInt();
	else reportMissingKey( wxT( "VedicConfig" ), wxT( "columnStyle" ));

}

void VedicConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "orderLagna" ) ] = orderLagna;
	v[ wxT( "objects" ) ] = objects;
	v[ wxT( "arabicParts" ) ] = tool.writeObjectArray( arabicParts );
	v[ wxT( "columnStyle" ) ] = columnStyle;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void KalaVelaConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "lord" ) ].IsNull()) tool.readIntVector( v[ wxT( "lord" ) ], lord );
	if ( ! v[ wxT( "portion" ) ].IsNull()) tool.readIntVector( v[ wxT( "portion" ) ], portion );
}

void KalaVelaConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "lord" ) ] = tool.writeIntVector( lord );
	v[ wxT( "portion" ) ] = tool.writeIntVector( portion );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void VedicCalculationConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "lunarNodeMode" ) ].IsNull()) lunarNodeMode = v[ wxT( "lunarNodeMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "lunarNodeMode" ));

	if ( ! v[ wxT( "customYearLength" ) ].IsNull()) customYearLength = myatof( v[ wxT( "customYearLength" ) ].AsString());
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "customYearLength" ));

	if ( ! v[ wxT( "nakshatraPortionMode" ) ].IsNull()) nakshatraPortionMode = v[ wxT( "nakshatraPortionMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "nakshatraPortionMode" ));

	if ( ! v[ wxT( "ashtakavargaNumberMode" ) ].IsNull()) ashtakavargaNumberMode = v[ wxT( "ashtakavargaNumberMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "ashtakavargaNumberMode" ));

	if ( ! v[ wxT( "ayanamsa" ) ].IsNull()) ayanamsa = (AYANAMSA)v[ wxT( "ayanamsa" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "ayanamsa" ));

	if ( ! v[ wxT( "mercuryBenefic" ) ].IsNull()) mercuryBenefic = v[ wxT( "mercuryBenefic" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "mercuryBenefic" ));

	if ( ! v[ wxT( "vargaDrekkanaMode" ) ].IsNull()) vargaDrekkanaMode = v[ wxT( "vargaDrekkanaMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "vargaDrekkanaMode" ));

	if ( ! v[ wxT( "arudhaMode" ) ].IsNull()) arudhaMode = v[ wxT( "arudhaMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "arudhaMode" ));

	if ( ! v[ wxT( "charaKarakaMode" ) ].IsNull()) charaKarakaMode = v[ wxT( "charaKarakaMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "charaKarakaMode" ));

	if ( ! v[ wxT( "kalavelaLord" ) ].IsNull()) tool.readIntVector( v[ wxT( "kalavelaLord" ) ], kalavelaLord );
	if ( ! v[ wxT( "moonBenefic" ) ].IsNull()) moonBenefic = v[ wxT( "moonBenefic" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "moonBenefic" ));

	if ( ! v[ wxT( "kalavela" ) ].IsNull()) kalavela.load( v, wxT( "kalavela" ) );
	if ( ! v[ wxT( "houseUseCusps" ) ].IsNull()) houseUseCusps = v[ wxT( "houseUseCusps" ) ].AsBool();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "houseUseCusps" ));

	if ( ! v[ wxT( "houseSystem" ) ].IsNull()) houseSystem = (HOUSE_SYSTEM)v[ wxT( "houseSystem" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "houseSystem" ));

	if ( ! v[ wxT( "ashtakavargaSarva" ) ].IsNull()) ashtakavargaSarva = v[ wxT( "ashtakavargaSarva" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "ashtakavargaSarva" ));

	if ( ! v[ wxT( "vargaSignifications" ) ].IsNull()) tool.readStringVector( v[ wxT( "vargaSignifications" ) ], vargaSignifications );
	if ( ! v[ wxT( "vargaChaturtamsaMode" ) ].IsNull()) vargaChaturtamsaMode = v[ wxT( "vargaChaturtamsaMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "vargaChaturtamsaMode" ));

	if ( ! v[ wxT( "kalachakraMode" ) ].IsNull()) kalachakraMode = v[ wxT( "kalachakraMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "kalachakraMode" ));

	if ( ! v[ wxT( "yoginiDasaLordDisplayType" ) ].IsNull()) yoginiDasaLordDisplayType = v[ wxT( "yoginiDasaLordDisplayType" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "yoginiDasaLordDisplayType" ));

	if ( ! v[ wxT( "kalavelaPortion" ) ].IsNull()) tool.readIntVector( v[ wxT( "kalavelaPortion" ) ], kalavelaPortion );
	if ( ! v[ wxT( "ashtakavargaMode" ) ].IsNull()) ashtakavargaMode = v[ wxT( "ashtakavargaMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "ashtakavargaMode" ));

	if ( ! v[ wxT( "yearLength" ) ].IsNull()) yearLength = v[ wxT( "yearLength" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "yearLength" ));

	if ( ! v[ wxT( "ashtakavargaSodhyaPindaMode" ) ].IsNull()) ashtakavargaSodhyaPindaMode = v[ wxT( "ashtakavargaSodhyaPindaMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "ashtakavargaSodhyaPindaMode" ));

	if ( ! v[ wxT( "vargaHoraMode" ) ].IsNull()) vargaHoraMode = v[ wxT( "vargaHoraMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "vargaHoraMode" ));

	if ( ! v[ wxT( "tempFriendMode" ) ].IsNull()) tempFriendMode = v[ wxT( "tempFriendMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "tempFriendMode" ));

	if ( ! v[ wxT( "dasaShowDateMode" ) ].IsNull()) dasaShowDateMode = v[ wxT( "dasaShowDateMode" ) ].AsInt();
	else reportMissingKey( wxT( "VedicCalculationConfig" ), wxT( "dasaShowDateMode" ));

}

void VedicCalculationConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "lunarNodeMode" ) ] = lunarNodeMode;
	v[ wxT( "customYearLength" ) ] = printfDouble( customYearLength );
	v[ wxT( "nakshatraPortionMode" ) ] = nakshatraPortionMode;
	v[ wxT( "ashtakavargaNumberMode" ) ] = ashtakavargaNumberMode;
	v[ wxT( "ayanamsa" ) ] = ayanamsa;
	v[ wxT( "mercuryBenefic" ) ] = mercuryBenefic;
	v[ wxT( "vargaDrekkanaMode" ) ] = vargaDrekkanaMode;
	v[ wxT( "arudhaMode" ) ] = arudhaMode;
	v[ wxT( "charaKarakaMode" ) ] = charaKarakaMode;
	v[ wxT( "kalavelaLord" ) ] = tool.writeIntVector( kalavelaLord );
	v[ wxT( "moonBenefic" ) ] = moonBenefic;
	kalavela.save( v, wxT( "kalavela" ) );
	v[ wxT( "houseUseCusps" ) ] = houseUseCusps;
	v[ wxT( "houseSystem" ) ] = houseSystem;
	v[ wxT( "ashtakavargaSarva" ) ] = ashtakavargaSarva;
	v[ wxT( "vargaSignifications" ) ] = tool.writeStringVector( vargaSignifications );
	v[ wxT( "vargaChaturtamsaMode" ) ] = vargaChaturtamsaMode;
	v[ wxT( "kalachakraMode" ) ] = kalachakraMode;
	v[ wxT( "yoginiDasaLordDisplayType" ) ] = yoginiDasaLordDisplayType;
	v[ wxT( "kalavelaPortion" ) ] = tool.writeIntVector( kalavelaPortion );
	v[ wxT( "ashtakavargaMode" ) ] = ashtakavargaMode;
	v[ wxT( "yearLength" ) ] = yearLength;
	v[ wxT( "ashtakavargaSodhyaPindaMode" ) ] = ashtakavargaSodhyaPindaMode;
	v[ wxT( "vargaHoraMode" ) ] = vargaHoraMode;
	v[ wxT( "tempFriendMode" ) ] = tempFriendMode;
	v[ wxT( "dasaShowDateMode" ) ] = dasaShowDateMode;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void EphemConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "sePath" ) ].IsNull()) sePath = v[ wxT( "sePath" ) ].AsString();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "sePath" ));

	if ( ! v[ wxT( "custom_t0" ) ].IsNull()) custom_t0 = myatof( v[ wxT( "custom_t0" ) ].AsString());
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "custom_t0" ));

	if ( ! v[ wxT( "custom_ayan_t0" ) ].IsNull()) custom_ayan_t0 = myatof( v[ wxT( "custom_ayan_t0" ) ].AsString());
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "custom_ayan_t0" ));

	if ( ! v[ wxT( "custom_aya_period" ) ].IsNull()) custom_aya_period = myatof( v[ wxT( "custom_aya_period" ) ].AsString());
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "custom_aya_period" ));

	if ( ! v[ wxT( "sunrise_def" ) ].IsNull()) sunrise_def = v[ wxT( "sunrise_def" ) ].AsInt();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "sunrise_def" ));

	if ( ! v[ wxT( "useEphemerisTime" ) ].IsNull()) useEphemerisTime = v[ wxT( "useEphemerisTime" ) ].AsInt();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "useEphemerisTime" ));

	if ( ! v[ wxT( "custom_aya_constant" ) ].IsNull()) custom_aya_constant = v[ wxT( "custom_aya_constant" ) ].AsBool();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "custom_aya_constant" ));

	if ( ! v[ wxT( "useTruePositions" ) ].IsNull()) useTruePositions = v[ wxT( "useTruePositions" ) ].AsInt();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "useTruePositions" ));

	if ( ! v[ wxT( "showEphemWarning" ) ].IsNull()) showEphemWarning = v[ wxT( "showEphemWarning" ) ].AsBool();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "showEphemWarning" ));

	if ( ! v[ wxT( "sunrise_refrac" ) ].IsNull()) sunrise_refrac = v[ wxT( "sunrise_refrac" ) ].AsBool();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "sunrise_refrac" ));

	if ( ! v[ wxT( "ephemCoordinateSystem" ) ].IsNull()) ephemCoordinateSystem = v[ wxT( "ephemCoordinateSystem" ) ].AsInt();
	else reportMissingKey( wxT( "EphemConfig" ), wxT( "ephemCoordinateSystem" ));

}

void EphemConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "sePath" ) ] = sePath;
	v[ wxT( "custom_t0" ) ] = printfDouble( custom_t0 );
	v[ wxT( "custom_ayan_t0" ) ] = printfDouble( custom_ayan_t0 );
	v[ wxT( "custom_aya_period" ) ] = printfDouble( custom_aya_period );
	v[ wxT( "sunrise_def" ) ] = sunrise_def;
	v[ wxT( "useEphemerisTime" ) ] = useEphemerisTime;
	v[ wxT( "custom_aya_constant" ) ] = custom_aya_constant;
	v[ wxT( "useTruePositions" ) ] = useTruePositions;
	v[ wxT( "showEphemWarning" ) ] = showEphemWarning;
	v[ wxT( "sunrise_refrac" ) ] = sunrise_refrac;
	v[ wxT( "ephemCoordinateSystem" ) ] = ephemCoordinateSystem;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void VedicChartDisplayConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "graphicSkin" ) ].IsNull()) graphicSkin = v[ wxT( "graphicSkin" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "graphicSkin" ));

	if ( ! v[ wxT( "showArudhas" ) ].IsNull()) showArudhas = v[ wxT( "showArudhas" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "showArudhas" ));

	if ( ! v[ wxT( "showPlanetColors" ) ].IsNull()) showPlanetColors = v[ wxT( "showPlanetColors" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "showPlanetColors" ));

	if ( ! v[ wxT( "showSbcNakshatraQuality" ) ].IsNull()) showSbcNakshatraQuality = v[ wxT( "showSbcNakshatraQuality" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "showSbcNakshatraQuality" ));

	if ( ! v[ wxT( "southIndianAscendantMarkup" ) ].IsNull()) southIndianAscendantMarkup = v[ wxT( "southIndianAscendantMarkup" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "southIndianAscendantMarkup" ));

	if ( ! v[ wxT( "showRetro" ) ].IsNull()) showRetro = v[ wxT( "showRetro" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "showRetro" ));

	if ( ! v[ wxT( "northIndianSignDisplayType" ) ].IsNull()) northIndianSignDisplayType = v[ wxT( "northIndianSignDisplayType" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "northIndianSignDisplayType" ));

	if ( ! v[ wxT( "showSbcSanskritSymbols" ) ].IsNull()) showSbcSanskritSymbols = v[ wxT( "showSbcSanskritSymbols" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "showSbcSanskritSymbols" ));

	if ( ! v[ wxT( "indianChartType" ) ].IsNull()) indianChartType = v[ wxT( "indianChartType" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "indianChartType" ));

	if ( ! v[ wxT( "showAshtakavarga" ) ].IsNull()) showAshtakavarga = v[ wxT( "showAshtakavarga" ) ].AsBool();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "showAshtakavarga" ));

	if ( ! v[ wxT( "centerInfoType" ) ].IsNull()) centerInfoType = v[ wxT( "centerInfoType" ) ].AsInt();
	else reportMissingKey( wxT( "VedicChartDisplayConfig" ), wxT( "centerInfoType" ));

}

void VedicChartDisplayConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "graphicSkin" ) ] = graphicSkin;
	v[ wxT( "showArudhas" ) ] = showArudhas;
	v[ wxT( "showPlanetColors" ) ] = showPlanetColors;
	v[ wxT( "showSbcNakshatraQuality" ) ] = showSbcNakshatraQuality;
	v[ wxT( "southIndianAscendantMarkup" ) ] = southIndianAscendantMarkup;
	v[ wxT( "showRetro" ) ] = showRetro;
	v[ wxT( "northIndianSignDisplayType" ) ] = northIndianSignDisplayType;
	v[ wxT( "showSbcSanskritSymbols" ) ] = showSbcSanskritSymbols;
	v[ wxT( "indianChartType" ) ] = indianChartType;
	v[ wxT( "showAshtakavarga" ) ] = showAshtakavarga;
	v[ wxT( "centerInfoType" ) ] = centerInfoType;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void ViewSizeConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "sAshtakaVargaWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sAshtakaVargaWindow" ) ], sAshtakaVargaWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sAshtakaVargaWindow" ));

	if ( ! v[ wxT( "sUranianWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sUranianWindow" ) ], sUranianWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sUranianWindow" ));

	if ( ! v[ wxT( "sObjectFilterDialog" ) ].IsNull()) tool.readSize( v[ wxT( "sObjectFilterDialog" ) ], sObjectFilterDialog );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sObjectFilterDialog" ));

	if ( ! v[ wxT( "sWesternGraphicWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sWesternGraphicWindow" ) ], sWesternGraphicWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sWesternGraphicWindow" ));

	if ( ! v[ wxT( "sYogaWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sYogaWindow" ) ], sYogaWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sYogaWindow" ));

	if ( ! v[ wxT( "sShadbalaWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sShadbalaWindow" ) ], sShadbalaWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sShadbalaWindow" ));

	if ( ! v[ wxT( "sTransitWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sTransitWindow" ) ], sTransitWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sTransitWindow" ));

	if ( ! v[ wxT( "sSolarWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sSolarWindow" ) ], sSolarWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sSolarWindow" ));

	if ( ! v[ wxT( "sSimpleConfigDialog" ) ].IsNull()) tool.readSize( v[ wxT( "sSimpleConfigDialog" ) ], sSimpleConfigDialog );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sSimpleConfigDialog" ));

	if ( ! v[ wxT( "sVargaSummaryWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sVargaSummaryWindow" ) ], sVargaSummaryWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sVargaSummaryWindow" ));

	if ( ! v[ wxT( "sDasaTreeWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sDasaTreeWindow" ) ], sDasaTreeWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sDasaTreeWindow" ));

	if ( ! v[ wxT( "sMainWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sMainWindow" ) ], sMainWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sMainWindow" ));

	if ( ! v[ wxT( "sPartnerWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sPartnerWindow" ) ], sPartnerWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sPartnerWindow" ));

	if ( ! v[ wxT( "sUranianGraphicWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sUranianGraphicWindow" ) ], sUranianGraphicWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sUranianGraphicWindow" ));

	if ( ! v[ wxT( "sAdditionalLogWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sAdditionalLogWindow" ) ], sAdditionalLogWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sAdditionalLogWindow" ));

	if ( ! v[ wxT( "sConfigDialog" ) ].IsNull()) tool.readSize( v[ wxT( "sConfigDialog" ) ], sConfigDialog );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sConfigDialog" ));

	if ( ! v[ wxT( "sEclipseWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sEclipseWindow" ) ], sEclipseWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sEclipseWindow" ));

	if ( ! v[ wxT( "sAtlasDialog" ) ].IsNull()) tool.readSize( v[ wxT( "sAtlasDialog" ) ], sAtlasDialog );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sAtlasDialog" ));

	if ( ! v[ wxT( "sSbcWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sSbcWindow" ) ], sSbcWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sSbcWindow" ));

	if ( ! v[ wxT( "sPrintPreviewWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sPrintPreviewWindow" ) ], sPrintPreviewWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sPrintPreviewWindow" ));

	if ( ! v[ wxT( "sGraphicalDasaWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sGraphicalDasaWindow" ) ], sGraphicalDasaWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sGraphicalDasaWindow" ));

	if ( ! v[ wxT( "sDataDialog" ) ].IsNull()) tool.readSize( v[ wxT( "sDataDialog" ) ], sDataDialog );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sDataDialog" ));

	if ( ! v[ wxT( "sHoraWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sHoraWindow" ) ], sHoraWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sHoraWindow" ));

	if ( ! v[ wxT( "sPrintoutSelectionDialog" ) ].IsNull()) tool.readSize( v[ wxT( "sPrintoutSelectionDialog" ) ], sPrintoutSelectionDialog );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sPrintoutSelectionDialog" ));

	if ( ! v[ wxT( "sEphemWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sEphemWindow" ) ], sEphemWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sEphemWindow" ));

	if ( ! v[ wxT( "sYogaEditor" ) ].IsNull()) tool.readSize( v[ wxT( "sYogaEditor" ) ], sYogaEditor );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sYogaEditor" ));

	if ( ! v[ wxT( "sDasaCompositeWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sDasaCompositeWindow" ) ], sDasaCompositeWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sDasaCompositeWindow" ));

	if ( ! v[ wxT( "sMultipleView" ) ].IsNull()) tool.readSize( v[ wxT( "sMultipleView" ) ], sMultipleView );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sMultipleView" ));

	if ( ! v[ wxT( "sTextWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sTextWindow" ) ], sTextWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sTextWindow" ));

	if ( ! v[ wxT( "sMasterWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sMasterWindow" ) ], sMasterWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sMasterWindow" ));

	if ( ! v[ wxT( "sVedicGraphicWindow" ) ].IsNull()) tool.readSize( v[ wxT( "sVedicGraphicWindow" ) ], sVedicGraphicWindow );
	else reportMissingKey( wxT( "ViewSizeConfig" ), wxT( "sVedicGraphicWindow" ));

}

void ViewSizeConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "sAshtakaVargaWindow" ) ] = tool.writeSize( sAshtakaVargaWindow );
	v[ wxT( "sUranianWindow" ) ] = tool.writeSize( sUranianWindow );
	v[ wxT( "sObjectFilterDialog" ) ] = tool.writeSize( sObjectFilterDialog );
	v[ wxT( "sWesternGraphicWindow" ) ] = tool.writeSize( sWesternGraphicWindow );
	v[ wxT( "sYogaWindow" ) ] = tool.writeSize( sYogaWindow );
	v[ wxT( "sShadbalaWindow" ) ] = tool.writeSize( sShadbalaWindow );
	v[ wxT( "sTransitWindow" ) ] = tool.writeSize( sTransitWindow );
	v[ wxT( "sSolarWindow" ) ] = tool.writeSize( sSolarWindow );
	v[ wxT( "sSimpleConfigDialog" ) ] = tool.writeSize( sSimpleConfigDialog );
	v[ wxT( "sVargaSummaryWindow" ) ] = tool.writeSize( sVargaSummaryWindow );
	v[ wxT( "sDasaTreeWindow" ) ] = tool.writeSize( sDasaTreeWindow );
	v[ wxT( "sMainWindow" ) ] = tool.writeSize( sMainWindow );
	v[ wxT( "sPartnerWindow" ) ] = tool.writeSize( sPartnerWindow );
	v[ wxT( "sUranianGraphicWindow" ) ] = tool.writeSize( sUranianGraphicWindow );
	v[ wxT( "sAdditionalLogWindow" ) ] = tool.writeSize( sAdditionalLogWindow );
	v[ wxT( "sConfigDialog" ) ] = tool.writeSize( sConfigDialog );
	v[ wxT( "sEclipseWindow" ) ] = tool.writeSize( sEclipseWindow );
	v[ wxT( "sAtlasDialog" ) ] = tool.writeSize( sAtlasDialog );
	v[ wxT( "sSbcWindow" ) ] = tool.writeSize( sSbcWindow );
	v[ wxT( "sPrintPreviewWindow" ) ] = tool.writeSize( sPrintPreviewWindow );
	v[ wxT( "sGraphicalDasaWindow" ) ] = tool.writeSize( sGraphicalDasaWindow );
	v[ wxT( "sDataDialog" ) ] = tool.writeSize( sDataDialog );
	v[ wxT( "sHoraWindow" ) ] = tool.writeSize( sHoraWindow );
	v[ wxT( "sPrintoutSelectionDialog" ) ] = tool.writeSize( sPrintoutSelectionDialog );
	v[ wxT( "sEphemWindow" ) ] = tool.writeSize( sEphemWindow );
	v[ wxT( "sYogaEditor" ) ] = tool.writeSize( sYogaEditor );
	v[ wxT( "sDasaCompositeWindow" ) ] = tool.writeSize( sDasaCompositeWindow );
	v[ wxT( "sMultipleView" ) ] = tool.writeSize( sMultipleView );
	v[ wxT( "sTextWindow" ) ] = tool.writeSize( sTextWindow );
	v[ wxT( "sMasterWindow" ) ] = tool.writeSize( sMasterWindow );
	v[ wxT( "sVedicGraphicWindow" ) ] = tool.writeSize( sVedicGraphicWindow );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void ViewPreferencesConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "antarDasaLevelChoice" ) ].IsNull()) antarDasaLevelChoice = v[ wxT( "antarDasaLevelChoice" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "antarDasaLevelChoice" ));

	if ( ! v[ wxT( "partnerStyle" ) ].IsNull()) partnerStyle = v[ wxT( "partnerStyle" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "partnerStyle" ));

	if ( ! v[ wxT( "yogaGroupFilter" ) ].IsNull()) yogaGroupFilter = v[ wxT( "yogaGroupFilter" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "yogaGroupFilter" ));

	if ( ! v[ wxT( "transitmode" ) ].IsNull()) transitmode = (TRANSIT_MODE)v[ wxT( "transitmode" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "transitmode" ));

	if ( ! v[ wxT( "showStatusBar" ) ].IsNull()) showStatusBar = v[ wxT( "showStatusBar" ) ].AsBool();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "showStatusBar" ));

	if ( ! v[ wxT( "yogaCustomFilter" ) ].IsNull()) yogaCustomFilter = v[ wxT( "yogaCustomFilter" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "yogaCustomFilter" ));

	if ( ! v[ wxT( "lastYogaConfigFile" ) ].IsNull()) lastYogaConfigFile = v[ wxT( "lastYogaConfigFile" ) ].AsString();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "lastYogaConfigFile" ));

	if ( ! v[ wxT( "uranianWindowPage" ) ].IsNull()) uranianWindowPage = v[ wxT( "uranianWindowPage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "uranianWindowPage" ));

	if ( ! v[ wxT( "activePageUranian" ) ].IsNull()) activePageUranian = v[ wxT( "activePageUranian" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "activePageUranian" ));

	if ( ! v[ wxT( "dasaTreeTextViewWidth" ) ].IsNull()) dasaTreeTextViewWidth = v[ wxT( "dasaTreeTextViewWidth" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "dasaTreeTextViewWidth" ));

	if ( ! v[ wxT( "configDialogActivePage" ) ].IsNull()) configDialogActivePage = v[ wxT( "configDialogActivePage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogActivePage" ));

	if ( ! v[ wxT( "configDialogVChartPreviewTransits" ) ].IsNull()) configDialogVChartPreviewTransits = v[ wxT( "configDialogVChartPreviewTransits" ) ].AsBool();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogVChartPreviewTransits" ));

	if ( ! v[ wxT( "gradkreis" ) ].IsNull()) gradkreis = v[ wxT( "gradkreis" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "gradkreis" ));

	if ( ! v[ wxT( "configDialogWChartPreviewTransits" ) ].IsNull()) configDialogWChartPreviewTransits = v[ wxT( "configDialogWChartPreviewTransits" ) ].AsBool();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogWChartPreviewTransits" ));

	if ( ! v[ wxT( "activePagePartner" ) ].IsNull()) activePagePartner = v[ wxT( "activePagePartner" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "activePagePartner" ));

	if ( ! v[ wxT( "doNotShowAgainAfterLanguageChange" ) ].IsNull()) doNotShowAgainAfterLanguageChange = v[ wxT( "doNotShowAgainAfterLanguageChange" ) ].AsBool();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "doNotShowAgainAfterLanguageChange" ));

	if ( ! v[ wxT( "activePageVargaOverview" ) ].IsNull()) activePageVargaOverview = v[ wxT( "activePageVargaOverview" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "activePageVargaOverview" ));

	if ( ! v[ wxT( "configDialogWChartPanelPage" ) ].IsNull()) configDialogWChartPanelPage = v[ wxT( "configDialogWChartPanelPage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogWChartPanelPage" ));

	if ( ! v[ wxT( "sashPartnerComposite" ) ].IsNull()) sashPartnerComposite = v[ wxT( "sashPartnerComposite" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "sashPartnerComposite" ));

	if ( ! v[ wxT( "activePageShadbala" ) ].IsNull()) activePageShadbala = v[ wxT( "activePageShadbala" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "activePageShadbala" ));

	if ( ! v[ wxT( "graphicalDasaList" ) ].IsNull()) tool.readIntVector( v[ wxT( "graphicalDasaList" ) ], graphicalDasaList );
	if ( ! v[ wxT( "ephemMode" ) ].IsNull()) ephemMode = v[ wxT( "ephemMode" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "ephemMode" ));

	if ( ! v[ wxT( "orbisPartner" ) ].IsNull()) orbisPartner = v[ wxT( "orbisPartner" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "orbisPartner" ));

	if ( ! v[ wxT( "orbisTransit" ) ].IsNull()) orbisTransit = v[ wxT( "orbisTransit" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "orbisTransit" ));

	if ( ! v[ wxT( "sashSolar" ) ].IsNull()) sashSolar = v[ wxT( "sashSolar" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "sashSolar" ));

	if ( ! v[ wxT( "sashTransit" ) ].IsNull()) sashTransit = v[ wxT( "sashTransit" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "sashTransit" ));

	if ( ! v[ wxT( "sashPartnerDouble" ) ].IsNull()) sashPartnerDouble = v[ wxT( "sashPartnerDouble" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "sashPartnerDouble" ));

	if ( ! v[ wxT( "defOpenPath" ) ].IsNull()) defOpenPath = v[ wxT( "defOpenPath" ) ].AsString();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "defOpenPath" ));

	if ( ! v[ wxT( "activePageTransit" ) ].IsNull()) activePageTransit = v[ wxT( "activePageTransit" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "activePageTransit" ));

	if ( ! v[ wxT( "doNotShowAgainEphemConfigChange" ) ].IsNull()) doNotShowAgainEphemConfigChange = v[ wxT( "doNotShowAgainEphemConfigChange" ) ].AsBool();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "doNotShowAgainEphemConfigChange" ));

	if ( ! v[ wxT( "listStyle" ) ].IsNull()) listStyle = v[ wxT( "listStyle" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "listStyle" ));

	if ( ! v[ wxT( "configDialogVCalculationPanelPage" ) ].IsNull()) configDialogVCalculationPanelPage = v[ wxT( "configDialogVCalculationPanelPage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogVCalculationPanelPage" ));

	if ( ! v[ wxT( "defPdfSavePath" ) ].IsNull()) defPdfSavePath = v[ wxT( "defPdfSavePath" ) ].AsString();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "defPdfSavePath" ));

	if ( ! v[ wxT( "pAdditionalLogWindow" ) ].IsNull()) tool.readSize( v[ wxT( "pAdditionalLogWindow" ) ], pAdditionalLogWindow );
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "pAdditionalLogWindow" ));

	if ( ! v[ wxT( "orbisRadix" ) ].IsNull()) orbisRadix = v[ wxT( "orbisRadix" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "orbisRadix" ));

	if ( ! v[ wxT( "doSplitDasaTree" ) ].IsNull()) doSplitDasaTree = v[ wxT( "doSplitDasaTree" ) ].AsBool();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "doSplitDasaTree" ));

	if ( ! v[ wxT( "ephemTimezone" ) ].IsNull()) ephemTimezone = v[ wxT( "ephemTimezone" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "ephemTimezone" ));

	if ( ! v[ wxT( "yogaSourceFilter" ) ].IsNull()) yogaSourceFilter = v[ wxT( "yogaSourceFilter" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "yogaSourceFilter" ));

	if ( ! v[ wxT( "textWindowPage" ) ].IsNull()) textWindowPage = v[ wxT( "textWindowPage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "textWindowPage" ));

	if ( ! v[ wxT( "pMasterWindow" ) ].IsNull()) tool.readSize( v[ wxT( "pMasterWindow" ) ], pMasterWindow );
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "pMasterWindow" ));

	if ( ! v[ wxT( "sizes" ) ].IsNull()) sizes.load( v, wxT( "sizes" ) );
	if ( ! v[ wxT( "graphicalDasaWidgetZoom" ) ].IsNull()) graphicalDasaWidgetZoom = v[ wxT( "graphicalDasaWidgetZoom" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "graphicalDasaWidgetZoom" ));

	if ( ! v[ wxT( "transitTimezone" ) ].IsNull()) transitTimezone = v[ wxT( "transitTimezone" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "transitTimezone" ));

	if ( ! v[ wxT( "configDialogColorPanelPage" ) ].IsNull()) configDialogColorPanelPage = v[ wxT( "configDialogColorPanelPage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogColorPanelPage" ));

	if ( ! v[ wxT( "showMainToolbar" ) ].IsNull()) showMainToolbar = v[ wxT( "showMainToolbar" ) ].AsBool();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "showMainToolbar" ));

	if ( ! v[ wxT( "activePageAshtakaVarga" ) ].IsNull()) activePageAshtakaVarga = v[ wxT( "activePageAshtakaVarga" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "activePageAshtakaVarga" ));

	if ( ! v[ wxT( "defExportPath" ) ].IsNull()) defExportPath = v[ wxT( "defExportPath" ) ].AsString();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "defExportPath" ));

	if ( ! v[ wxT( "sashDasaTree" ) ].IsNull()) sashDasaTree = v[ wxT( "sashDasaTree" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "sashDasaTree" ));

	if ( ! v[ wxT( "defSavePath" ) ].IsNull()) defSavePath = v[ wxT( "defSavePath" ) ].AsString();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "defSavePath" ));

	if ( ! v[ wxT( "configDialogVChartPanelPage" ) ].IsNull()) configDialogVChartPanelPage = v[ wxT( "configDialogVChartPanelPage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogVChartPanelPage" ));

	if ( ! v[ wxT( "sashMasterWindowLog" ) ].IsNull()) sashMasterWindowLog = v[ wxT( "sashMasterWindowLog" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "sashMasterWindowLog" ));

	if ( ! v[ wxT( "ephemCircleType" ) ].IsNull()) ephemCircleType = v[ wxT( "ephemCircleType" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "ephemCircleType" ));

	if ( ! v[ wxT( "configDialogSheetStylePanelPage" ) ].IsNull()) configDialogSheetStylePanelPage = v[ wxT( "configDialogSheetStylePanelPage" ) ].AsInt();
	else reportMissingKey( wxT( "ViewPreferencesConfig" ), wxT( "configDialogSheetStylePanelPage" ));

	if ( ! v[ wxT( "recentFiles" ) ].IsNull()) tool.readStringVector( v[ wxT( "recentFiles" ) ], recentFiles );
}

void ViewPreferencesConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "antarDasaLevelChoice" ) ] = antarDasaLevelChoice;
	v[ wxT( "partnerStyle" ) ] = partnerStyle;
	v[ wxT( "yogaGroupFilter" ) ] = yogaGroupFilter;
	v[ wxT( "transitmode" ) ] = transitmode;
	v[ wxT( "showStatusBar" ) ] = showStatusBar;
	v[ wxT( "yogaCustomFilter" ) ] = yogaCustomFilter;
	v[ wxT( "lastYogaConfigFile" ) ] = lastYogaConfigFile;
	v[ wxT( "uranianWindowPage" ) ] = uranianWindowPage;
	v[ wxT( "activePageUranian" ) ] = activePageUranian;
	v[ wxT( "dasaTreeTextViewWidth" ) ] = dasaTreeTextViewWidth;
	v[ wxT( "configDialogActivePage" ) ] = configDialogActivePage;
	v[ wxT( "configDialogVChartPreviewTransits" ) ] = configDialogVChartPreviewTransits;
	v[ wxT( "gradkreis" ) ] = gradkreis;
	v[ wxT( "configDialogWChartPreviewTransits" ) ] = configDialogWChartPreviewTransits;
	v[ wxT( "activePagePartner" ) ] = activePagePartner;
	v[ wxT( "doNotShowAgainAfterLanguageChange" ) ] = doNotShowAgainAfterLanguageChange;
	v[ wxT( "activePageVargaOverview" ) ] = activePageVargaOverview;
	v[ wxT( "configDialogWChartPanelPage" ) ] = configDialogWChartPanelPage;
	v[ wxT( "sashPartnerComposite" ) ] = sashPartnerComposite;
	v[ wxT( "activePageShadbala" ) ] = activePageShadbala;
	v[ wxT( "graphicalDasaList" ) ] = tool.writeIntVector( graphicalDasaList );
	v[ wxT( "ephemMode" ) ] = ephemMode;
	v[ wxT( "orbisPartner" ) ] = orbisPartner;
	v[ wxT( "orbisTransit" ) ] = orbisTransit;
	v[ wxT( "sashSolar" ) ] = sashSolar;
	v[ wxT( "sashTransit" ) ] = sashTransit;
	v[ wxT( "sashPartnerDouble" ) ] = sashPartnerDouble;
	v[ wxT( "defOpenPath" ) ] = defOpenPath;
	v[ wxT( "activePageTransit" ) ] = activePageTransit;
	v[ wxT( "doNotShowAgainEphemConfigChange" ) ] = doNotShowAgainEphemConfigChange;
	v[ wxT( "listStyle" ) ] = listStyle;
	v[ wxT( "configDialogVCalculationPanelPage" ) ] = configDialogVCalculationPanelPage;
	v[ wxT( "defPdfSavePath" ) ] = defPdfSavePath;
	v[ wxT( "pAdditionalLogWindow" ) ] = tool.writeSize( pAdditionalLogWindow );
	v[ wxT( "orbisRadix" ) ] = orbisRadix;
	v[ wxT( "doSplitDasaTree" ) ] = doSplitDasaTree;
	v[ wxT( "ephemTimezone" ) ] = ephemTimezone;
	v[ wxT( "yogaSourceFilter" ) ] = yogaSourceFilter;
	v[ wxT( "textWindowPage" ) ] = textWindowPage;
	v[ wxT( "pMasterWindow" ) ] = tool.writeSize( pMasterWindow );
	sizes.save( v, wxT( "sizes" ) );
	v[ wxT( "graphicalDasaWidgetZoom" ) ] = graphicalDasaWidgetZoom;
	v[ wxT( "transitTimezone" ) ] = transitTimezone;
	v[ wxT( "configDialogColorPanelPage" ) ] = configDialogColorPanelPage;
	v[ wxT( "showMainToolbar" ) ] = showMainToolbar;
	v[ wxT( "activePageAshtakaVarga" ) ] = activePageAshtakaVarga;
	v[ wxT( "defExportPath" ) ] = defExportPath;
	v[ wxT( "sashDasaTree" ) ] = sashDasaTree;
	v[ wxT( "defSavePath" ) ] = defSavePath;
	v[ wxT( "configDialogVChartPanelPage" ) ] = configDialogVChartPanelPage;
	v[ wxT( "sashMasterWindowLog" ) ] = sashMasterWindowLog;
	v[ wxT( "ephemCircleType" ) ] = ephemCircleType;
	v[ wxT( "configDialogSheetStylePanelPage" ) ] = configDialogSheetStylePanelPage;
	v[ wxT( "recentFiles" ) ] = tool.writeStringVector( recentFiles );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void ToolbarConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "toolbarStyle" ) ].IsNull()) toolbarStyle = v[ wxT( "toolbarStyle" ) ].AsInt();
	else reportMissingKey( wxT( "ToolbarConfig" ), wxT( "toolbarStyle" ));

	if ( ! v[ wxT( "toolbarIconSize" ) ].IsNull()) toolbarIconSize = v[ wxT( "toolbarIconSize" ) ].AsInt();
	else reportMissingKey( wxT( "ToolbarConfig" ), wxT( "toolbarIconSize" ));

	if ( ! v[ wxT( "mainToolbarItems" ) ].IsNull()) tool.readIntVector( v[ wxT( "mainToolbarItems" ) ], mainToolbarItems );
}

void ToolbarConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "toolbarStyle" ) ] = toolbarStyle;
	v[ wxT( "toolbarIconSize" ) ] = toolbarIconSize;
	v[ wxT( "mainToolbarItems" ) ] = tool.writeIntVector( mainToolbarItems );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void ViewConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "defGraphicExportType" ) ].IsNull()) defGraphicExportType = v[ wxT( "defGraphicExportType" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "defGraphicExportType" ));

	if ( ! v[ wxT( "childViewFrameWidth" ) ].IsNull()) childViewFrameWidth = v[ wxT( "childViewFrameWidth" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "childViewFrameWidth" ));

	if ( ! v[ wxT( "graphicExportSizeMode" ) ].IsNull()) graphicExportSizeMode = v[ wxT( "graphicExportSizeMode" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "graphicExportSizeMode" ));

	if ( ! v[ wxT( "backupFileMode" ) ].IsNull()) backupFileMode = v[ wxT( "backupFileMode" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "backupFileMode" ));

	if ( ! v[ wxT( "logMode" ) ].IsNull()) logMode = v[ wxT( "logMode" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "logMode" ));

	if ( ! v[ wxT( "exportAskOnOverwrite" ) ].IsNull()) exportAskOnOverwrite = v[ wxT( "exportAskOnOverwrite" ) ].AsBool();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "exportAskOnOverwrite" ));

	if ( ! v[ wxT( "defOpenFiletype" ) ].IsNull()) defOpenFiletype = v[ wxT( "defOpenFiletype" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "defOpenFiletype" ));

	if ( ! v[ wxT( "sheetStyle" ) ].IsNull()) sheetStyle = v[ wxT( "sheetStyle" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "sheetStyle" ));

	if ( ! v[ wxT( "logLevel" ) ].IsNull()) logLevel = v[ wxT( "logLevel" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "logLevel" ));

	if ( ! v[ wxT( "showStatusInfo" ) ].IsNull()) showStatusInfo = v[ wxT( "showStatusInfo" ) ].AsBool();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "showStatusInfo" ));

	if ( ! v[ wxT( "maxRecentFiles" ) ].IsNull()) maxRecentFiles = v[ wxT( "maxRecentFiles" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "maxRecentFiles" ));

	if ( ! v[ wxT( "inputFieldValidation" ) ].IsNull()) inputFieldValidation = v[ wxT( "inputFieldValidation" ) ].AsInt();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "inputFieldValidation" ));

	if ( ! v[ wxT( "showTextViewHeaders" ) ].IsNull()) showTextViewHeaders = v[ wxT( "showTextViewHeaders" ) ].AsBool();
	else reportMissingKey( wxT( "ViewConfig" ), wxT( "showTextViewHeaders" ));

}

void ViewConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "defGraphicExportType" ) ] = defGraphicExportType;
	v[ wxT( "childViewFrameWidth" ) ] = childViewFrameWidth;
	v[ wxT( "graphicExportSizeMode" ) ] = graphicExportSizeMode;
	v[ wxT( "backupFileMode" ) ] = backupFileMode;
	v[ wxT( "logMode" ) ] = logMode;
	v[ wxT( "exportAskOnOverwrite" ) ] = exportAskOnOverwrite;
	v[ wxT( "defOpenFiletype" ) ] = defOpenFiletype;
	v[ wxT( "sheetStyle" ) ] = sheetStyle;
	v[ wxT( "logLevel" ) ] = logLevel;
	v[ wxT( "showStatusInfo" ) ] = showStatusInfo;
	v[ wxT( "maxRecentFiles" ) ] = maxRecentFiles;
	v[ wxT( "inputFieldValidation" ) ] = inputFieldValidation;
	v[ wxT( "showTextViewHeaders" ) ] = showTextViewHeaders;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void FontConfigEntries::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "pdfTableHeader" ) ].IsNull()) pdfTableHeader = v[ wxT( "pdfTableHeader" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "pdfTableHeader" ));

	if ( ! v[ wxT( "htmlDefault" ) ].IsNull()) htmlDefault = v[ wxT( "htmlDefault" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "htmlDefault" ));

	if ( ! v[ wxT( "textHeader" ) ].IsNull()) textHeader = v[ wxT( "textHeader" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "textHeader" ));

	if ( ! v[ wxT( "pdfTableContents" ) ].IsNull()) pdfTableContents = v[ wxT( "pdfTableContents" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "pdfTableContents" ));

	if ( ! v[ wxT( "textTableHeader" ) ].IsNull()) textTableHeader = v[ wxT( "textTableHeader" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "textTableHeader" ));

	if ( ! v[ wxT( "plainDefault" ) ].IsNull()) plainDefault = v[ wxT( "plainDefault" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "plainDefault" ));

	if ( ! v[ wxT( "pdfTiny" ) ].IsNull()) pdfTiny = v[ wxT( "pdfTiny" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "pdfTiny" ));

	if ( ! v[ wxT( "textTableContents" ) ].IsNull()) textTableContents = v[ wxT( "textTableContents" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "textTableContents" ));

	if ( ! v[ wxT( "pdfHeader" ) ].IsNull()) pdfHeader = v[ wxT( "pdfHeader" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "pdfHeader" ));

	if ( ! v[ wxT( "graphicDefault" ) ].IsNull()) graphicDefault = v[ wxT( "graphicDefault" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "graphicDefault" ));

	if ( ! v[ wxT( "textDefault" ) ].IsNull()) textDefault = v[ wxT( "textDefault" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "textDefault" ));

	if ( ! v[ wxT( "pdfDefault" ) ].IsNull()) pdfDefault = v[ wxT( "pdfDefault" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "pdfDefault" ));

	if ( ! v[ wxT( "graphicSymbols" ) ].IsNull()) graphicSymbols = v[ wxT( "graphicSymbols" ) ].AsString();
	else reportMissingKey( wxT( "FontConfigEntries" ), wxT( "graphicSymbols" ));

}

void FontConfigEntries::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "pdfTableHeader" ) ] = pdfTableHeader;
	v[ wxT( "htmlDefault" ) ] = htmlDefault;
	v[ wxT( "textHeader" ) ] = textHeader;
	v[ wxT( "pdfTableContents" ) ] = pdfTableContents;
	v[ wxT( "textTableHeader" ) ] = textTableHeader;
	v[ wxT( "plainDefault" ) ] = plainDefault;
	v[ wxT( "pdfTiny" ) ] = pdfTiny;
	v[ wxT( "textTableContents" ) ] = textTableContents;
	v[ wxT( "pdfHeader" ) ] = pdfHeader;
	v[ wxT( "graphicDefault" ) ] = graphicDefault;
	v[ wxT( "textDefault" ) ] = textDefault;
	v[ wxT( "pdfDefault" ) ] = pdfDefault;
	v[ wxT( "graphicSymbols" ) ] = graphicSymbols;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void AtlasConfig::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "lastSelectedCountry" ) ].IsNull()) lastSelectedCountry = v[ wxT( "lastSelectedCountry" ) ].AsString();
	else reportMissingKey( wxT( "AtlasConfig" ), wxT( "lastSelectedCountry" ));

	if ( ! v[ wxT( "filterCaseSensitive" ) ].IsNull()) filterCaseSensitive = v[ wxT( "filterCaseSensitive" ) ].AsBool();
	else reportMissingKey( wxT( "AtlasConfig" ), wxT( "filterCaseSensitive" ));

	if ( ! v[ wxT( "filterMode" ) ].IsNull()) filterMode = v[ wxT( "filterMode" ) ].AsInt();
	else reportMissingKey( wxT( "AtlasConfig" ), wxT( "filterMode" ));

	if ( ! v[ wxT( "cacheSize" ) ].IsNull()) cacheSize = v[ wxT( "cacheSize" ) ].AsInt();
	else reportMissingKey( wxT( "AtlasConfig" ), wxT( "cacheSize" ));

	if ( ! v[ wxT( "favouriteCountries" ) ].IsNull()) tool.readStringVector( v[ wxT( "favouriteCountries" ) ], favouriteCountries );
	if ( ! v[ wxT( "sqlFile" ) ].IsNull()) sqlFile = v[ wxT( "sqlFile" ) ].AsString();
	else reportMissingKey( wxT( "AtlasConfig" ), wxT( "sqlFile" ));

	if ( ! v[ wxT( "gridColumnSizes" ) ].IsNull()) tool.readIntVector( v[ wxT( "gridColumnSizes" ) ], gridColumnSizes );
	if ( ! v[ wxT( "databaseFile" ) ].IsNull()) databaseFile = v[ wxT( "databaseFile" ) ].AsString();
	else reportMissingKey( wxT( "AtlasConfig" ), wxT( "databaseFile" ));

}

void AtlasConfig::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	v[ wxT( "lastSelectedCountry" ) ] = lastSelectedCountry;
	v[ wxT( "filterCaseSensitive" ) ] = filterCaseSensitive;
	v[ wxT( "filterMode" ) ] = filterMode;
	v[ wxT( "cacheSize" ) ] = cacheSize;
	v[ wxT( "favouriteCountries" ) ] = tool.writeStringVector( favouriteCountries );
	v[ wxT( "sqlFile" ) ] = sqlFile;
	v[ wxT( "gridColumnSizes" ) ] = tool.writeIntVector( gridColumnSizes );
	v[ wxT( "databaseFile" ) ] = databaseFile;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void AnimationConfig::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "interval" ) ].IsNull()) interval = v[ wxT( "interval" ) ].AsInt();
	else reportMissingKey( wxT( "AnimationConfig" ), wxT( "interval" ));

	if ( ! v[ wxT( "stepDimension" ) ].IsNull()) stepDimension = v[ wxT( "stepDimension" ) ].AsInt();
	else reportMissingKey( wxT( "AnimationConfig" ), wxT( "stepDimension" ));

	if ( ! v[ wxT( "stepLength" ) ].IsNull()) stepLength = v[ wxT( "stepLength" ) ].AsInt();
	else reportMissingKey( wxT( "AnimationConfig" ), wxT( "stepLength" ));

	if ( ! v[ wxT( "mode" ) ].IsNull()) mode = v[ wxT( "mode" ) ].AsInt();
	else reportMissingKey( wxT( "AnimationConfig" ), wxT( "mode" ));

}

void AnimationConfig::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "interval" ) ] = interval;
	v[ wxT( "stepDimension" ) ] = stepDimension;
	v[ wxT( "stepLength" ) ] = stepLength;
	v[ wxT( "mode" ) ] = mode;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void MultipleViewConfiguration::load( wxJSONValue &root, wxString attname )
{
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "notebookStyle" ) ].IsNull()) notebookStyle = v[ wxT( "notebookStyle" ) ].AsInt();
	else reportMissingKey( wxT( "MultipleViewConfiguration" ), wxT( "notebookStyle" ));

	if ( ! v[ wxT( "useMultipleViews" ) ].IsNull()) useMultipleViews = v[ wxT( "useMultipleViews" ) ].AsBool();
	else reportMissingKey( wxT( "MultipleViewConfiguration" ), wxT( "useMultipleViews" ));

	if ( ! v[ wxT( "defaultView" ) ].IsNull()) defaultView = v[ wxT( "defaultView" ) ].AsInt();
	else reportMissingKey( wxT( "MultipleViewConfiguration" ), wxT( "defaultView" ));

	if ( ! v[ wxT( "notebookOrientation" ) ].IsNull()) notebookOrientation = v[ wxT( "notebookOrientation" ) ].AsInt();
	else reportMissingKey( wxT( "MultipleViewConfiguration" ), wxT( "notebookOrientation" ));

}

void MultipleViewConfiguration::save( wxJSONValue &root, wxString attname )
{
	wxJSONValue v;
	v[ wxT( "notebookStyle" ) ] = notebookStyle;
	v[ wxT( "useMultipleViews" ) ] = useMultipleViews;
	v[ wxT( "defaultView" ) ] = defaultView;
	v[ wxT( "notebookOrientation" ) ] = notebookOrientation;
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

void Config::load( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue &v = attname.IsEmpty() ? root : root[ attname ];
	if ( ! v[ wxT( "viewprefs" ) ].IsNull()) viewprefs->load( v, wxT( "viewprefs" ) );
	if ( ! v[ wxT( "preferVedic" ) ].IsNull()) preferVedic = v[ wxT( "preferVedic" ) ].AsBool();
	else reportMissingKey( wxT( "Config" ), wxT( "preferVedic" ));

	if ( ! v[ wxT( "westernChart" ) ].IsNull()) westernChart->load( v, wxT( "westernChart" ) );
	if ( ! v[ wxT( "vedicChartBehavior" ) ].IsNull()) vedicChartBehavior->load( v, wxT( "vedicChartBehavior" ) );
	if ( ! v[ wxT( "vedicCalculation" ) ].IsNull()) vedicCalculation->load( v, wxT( "vedicCalculation" ) );
	if ( ! v[ wxT( "multipleView" ) ].IsNull()) multipleView->load( v, wxT( "multipleView" ) );
	if ( ! v[ wxT( "print" ) ].IsNull()) print->load( v, wxT( "print" ) );
	if ( ! v[ wxT( "defaultLocation" ) ].IsNull()) defaultLocation->load( v, wxT( "defaultLocation" ) );
	if ( ! v[ wxT( "animation" ) ].IsNull()) animation->load( v, wxT( "animation" ) );
	if ( ! v[ wxT( "toolbar" ) ].IsNull()) toolbar->load( v, wxT( "toolbar" ) );
	if ( ! v[ wxT( "atlas" ) ].IsNull()) atlas->load( v, wxT( "atlas" ) );
	if ( ! v[ wxT( "vedicChart" ) ].IsNull()) vedicChart->load( v, wxT( "vedicChart" ) );
	if ( ! v[ wxT( "lang" ) ].IsNull()) lang = v[ wxT( "lang" ) ].AsString();
	else reportMissingKey( wxT( "Config" ), wxT( "lang" ));

	if ( ! v[ wxT( "openNewDocOnStart" ) ].IsNull()) openNewDocOnStart = v[ wxT( "openNewDocOnStart" ) ].AsBool();
	else reportMissingKey( wxT( "Config" ), wxT( "openNewDocOnStart" ));

	if ( ! v[ wxT( "bardiagram" ) ].IsNull()) bardiagram->load( v, wxT( "bardiagram" ) );
	if ( ! v[ wxT( "langList" ) ].IsNull()) langList = v[ wxT( "langList" ) ].AsString();
	else reportMissingKey( wxT( "Config" ), wxT( "langList" ));

	if ( ! v[ wxT( "writer" ) ].IsNull()) writer->load( v, wxT( "writer" ) );
	if ( ! v[ wxT( "westernChartBehavior" ) ].IsNull()) westernChartBehavior->load( v, wxT( "westernChartBehavior" ) );
	if ( ! v[ wxT( "askOnQuit" ) ].IsNull()) askOnQuit = v[ wxT( "askOnQuit" ) ].AsBool();
	else reportMissingKey( wxT( "Config" ), wxT( "askOnQuit" ));

	if ( ! v[ wxT( "ephem" ) ].IsNull()) ephem->load( v, wxT( "ephem" ) );
	if ( ! v[ wxT( "colors" ) ].IsNull()) colors->load( v, wxT( "colors" ) );
	if ( ! v[ wxT( "uranian" ) ].IsNull()) uranian->load( v, wxT( "uranian" ) );
	if ( ! v[ wxT( "western" ) ].IsNull()) western->load( v, wxT( "western" ) );
	if ( ! v[ wxT( "westernCalculation" ) ].IsNull()) westernCalculation->load( v, wxT( "westernCalculation" ) );
	if ( ! v[ wxT( "view" ) ].IsNull()) view->load( v, wxT( "view" ) );
	if ( ! v[ wxT( "vedic" ) ].IsNull()) vedic->load( v, wxT( "vedic" ) );
}

void Config::save( wxJSONValue &root, wxString attname )
{
	JSonTool tool;
	wxJSONValue v;
	viewprefs->save( v, wxT( "viewprefs" ) );
	v[ wxT( "preferVedic" ) ] = preferVedic;
	westernChart->save( v, wxT( "westernChart" ) );
	vedicChartBehavior->save( v, wxT( "vedicChartBehavior" ) );
	vedicCalculation->save( v, wxT( "vedicCalculation" ) );
	multipleView->save( v, wxT( "multipleView" ) );
	print->save( v, wxT( "print" ) );
	defaultLocation->save( v, wxT( "defaultLocation" ) );
	animation->save( v, wxT( "animation" ) );
	toolbar->save( v, wxT( "toolbar" ) );
	atlas->save( v, wxT( "atlas" ) );
	vedicChart->save( v, wxT( "vedicChart" ) );
	v[ wxT( "lang" ) ] = lang;
	v[ wxT( "openNewDocOnStart" ) ] = openNewDocOnStart;
	bardiagram->save( v, wxT( "bardiagram" ) );
	v[ wxT( "langList" ) ] = langList;
	writer->save( v, wxT( "writer" ) );
	westernChartBehavior->save( v, wxT( "westernChartBehavior" ) );
	v[ wxT( "askOnQuit" ) ] = askOnQuit;
	ephem->save( v, wxT( "ephem" ) );
	colors->save( v, wxT( "colors" ) );
	uranian->save( v, wxT( "uranian" ) );
	western->save( v, wxT( "western" ) );
	westernCalculation->save( v, wxT( "westernCalculation" ) );
	view->save( v, wxT( "view" ) );
	vedic->save( v, wxT( "vedic" ) );
	if ( ! attname.IsEmpty()) root[ attname ] = v;
	else root.Append( v );
}

