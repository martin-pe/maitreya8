
AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../pics -I../wxJSON

if USE_SWE_BUILTIN
AM_CPPFLAGS += -I../swe
else
AM_CPPFLAGS += $(LIBSWE_CFLAGS)
endif !USE_SWE_BUILTIN

if USE_WXSQLITE_BUILTIN
AM_CPPFLAGS += -I../wxsqlite3
else
AM_CPPFLAGS += $(WXSQLITE3_CFLAGS)
endif !USE_WXSQLITE_BUILTIN

# Remove this when gimp writes xpm like const *char
AM_CPPFLAGS += -Wno-write-strings

noinst_LIBRARIES = libmaitreya_base.a

libmaitreya_base_a_SOURCES = AspectConfig.cpp AspectConfig.h AtlasDao.cpp AtlasDao.h \
	AtlasImporter.cpp AtlasImporter.h AtlasLogic.cpp AtlasLogic.h BaseDao.cpp BaseDao.h \
	Calculator.cpp Calculator.h ChartConfig.cpp ChartConfig.h chart_images.h ChartProperties.cpp \
	ChartProperties.h ColorConfig.cpp ColorConfig.h Commands.cpp Commands.h Conf.cpp Conf.h \
	ConfigBase.h ConfigListLoader.cpp ConfigListLoader.h ConfigLoader.h ConfigStubs.cpp \
	DataSet.cpp DataSet.h FileConfig.cpp FileConfig.h FontProvider.cpp FontProvider.h \
	GraphConfig.cpp GraphConfig.h IdConverter.cpp IdConverter.h ImageProvider.cpp ImageProvider.h \
	JSonTool.cpp JSonTool.h Lang.cpp Lang.h LanguageConfig.cpp LanguageConfig.h maitreya.cpp \
	maitreya.h mathbase.cpp mathbase.h MultipleViewConfig.cpp MultipleViewConfig.h PlanetList.cpp \
	PlanetList.h PrintoutConfigBase.h PrintoutConfig.cpp PrintoutConfig.h SheetConfig.cpp \
	SheetConfig.h SymbolProvider.cpp SymbolProvider.h VargaBase.cpp VargaBase.h \
	VedicChartConfig.cpp VedicChartConfig.h ViewNode.cpp ViewNode.h WesternChartConfig.cpp \
	WesternChartConfig.h xpm_images.h

all-am: 

clean:
	rm -f lib*.a *.o core *~ *.bak
	rm -fr obj bin .libs

EXTRA_DIST = Makefile.win 

