AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../base -I../jyotish -I../gui 

noinst_LIBRARIES = libmaitreya_dialogs.a

libmaitreya_dialogs_a_SOURCES = AboutDialog.cpp AboutDialog.h AnimationPanel.cpp AnimationPanel.h \
	AspectPanel.cpp AspectPanel.h AtlasAliasDialog.cpp AtlasAliasDialog.h AtlasDialog.cpp \
	AtlasDialog.h AtlasEditDialog.cpp AtlasEditDialog.h AtlasImportDialog.cpp AtlasImportDialog.h \
	AtlasPanel.cpp AtlasPanel.h BrushDialog.cpp BrushDialog.h ColorPanel.cpp ColorPanel.h \
	ConfigDialog.cpp ConfigDialog.h ConfigPanel.cpp ConfigPanel.h DasaPanel.cpp DasaPanel.h \
	DataDialog.cpp DataDialog.h DialogElements.cpp DialogElements.h DragChart.cpp DragChart.h \
	EphemerisPanel.cpp EphemerisPanel.h ExportDialog.cpp ExportDialog.h FontPanel.cpp FontPanel.h \
	GeneralPanel.cpp GeneralPanel.h MiscellaneousPanel.cpp MiscellaneousPanel.h mspin.cpp mspin.h \
	MultipleViewPanel.cpp MultipleViewPanel.h mvalidator.cpp mvalidator.h ObjectFilterDialog.cpp \
	ObjectFilterDialog.h PenDialog.cpp PenDialog.h PlanetSignDisplayPanel.cpp \
	PlanetSignDisplayPanel.h PrintoutPanel.cpp PrintoutPanel.h SheetStylePanel.cpp \
	SheetStylePanel.h StyleManagerDialog.cpp StyleManagerDialog.h ToolbarPanel.cpp ToolbarPanel.h \
	VedicCalculationPanel.cpp VedicCalculationPanel.h VedicChartPanel.cpp VedicChartPanel.h VedicPanel.cpp \
	VedicPanel.h WesternCalculationPanel.cpp WesternCalculationPanel.h WesternChartPanel.cpp \
	WesternChartPanel.h WesternPanel.cpp WesternPanel.h YogaEditorPanel.cpp YogaEditorPanel.h



EXTRA_DIST = Makefile.win

clean:
	rm -f lib*.a *.o *~ *.bak
	rm -fr obj bin .libs 

