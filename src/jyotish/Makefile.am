
AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../base -I../wxJSON

bin_PROGRAMS = maitreya8t
maitreya8t_SOURCES = textclient.cpp

maitreya8t_LDADD = libjyotish.a ../base/libmaitreya_base.a ../wxJSON/libmaitreya_wxJSON.a

if USE_SWE_BUILTIN
maitreya8t_LDADD += ../swe/libswe.a
# maitreya8t_LDADD += ../swe/libswe.la
else
maitreya8t_LDADD += $(LIBSWE_LIBS)
endif !USE_SWE_BUILTIN

if USE_WXSQLITE_BUILTIN
maitreya8t_LDADD += ../wxsqlite3/libmaitreya_wxsqlite3.a
else
maitreya8t_LDADD += $(WXSQLITE3_LIBS)
endif !USE_WXSQLITE_BUILTIN

maitreya8t_LDADD += $(WX_LIBS)

BUILT_SOURCES = MdlParser.hh 

MdlLexer.ll : MdlParser.yy 
MdlParser.hh : MdlParser.yy

MdlLexer.cc : MdlLexer.ll
	$(LEX) -t MdlLexer.ll > MdlLexer.cc

MdlParser.cc : MdlParser.yy
	$(YACC) -d MdlParser.yy
	mv y.tab.c MdlParser.cc
	mv y.tab.h MdlParser.hh

noinst_LIBRARIES = libjyotish.a

libjyotish_a_SOURCES = ArabicParts.cpp ArabicParts.h Ashtakavarga.cpp Ashtakavarga.h \
	AshtottariDasa.cpp AshtottariDasa.h Aspect.cpp Aspect.h astrobase.cpp astrobase.h \
	BasicHoroscope.cpp BasicHoroscope.h DasaConfig.cpp DasaConfig.h Dasa.cpp Dasa.h \
	DasaTool.cpp DasaTool.h Eclipse.cpp Eclipse.h Ephemeris.cpp Ephemeris.h Expert.cpp \
	Expert.h Exporter.cpp Exporter.h Expression.cpp Expression.h GenericDasa.cpp GenericDasa.h \
	GenericTableWriter.cpp GenericTableWriter.h Hora.cpp Hora.h Horoscope.cpp Horoscope.h \
	HtmlExporter.cpp Jaimini.cpp Jaimini.h KalachakraDasa.cpp KalachakraDasa.h \
	MdlInterpreter.cpp MdlInterpreter.h MdlLexer.cc MdlLexer.ll MdlParser.cc MdlParser.yy \
	Nakshatra.cpp Nakshatra.h Painter.cpp Painter.h Partner.cpp Partner.h PrintoutTextHelper.cpp \
	PrintoutTextHelper.h ShadBala.cpp ShadBala.h Sheet.cpp Sheet.h SingletonLifeline.cpp \
	SingletonLifeline.h SolarChart.cpp SolarChart.h Table.cpp Table.h \
	TextHelper.cpp TextHelper.h Transit.cpp Transit.h UranianBase.cpp UranianBase.h \
	Uranian.cpp Uranian.h UranianHelper.cpp UranianHelper.h Varga.cpp Varga.h \
	VargaHoroscope.cpp VargaHoroscope.h VedicPlanet.cpp VedicPlanet.h VimsottariDasa.cpp \
	VimsottariDasa.h YogaConfig.cpp YogaConfig.h Yoga.cpp Yoga.h

clean:
	rm -f lib*.a *.o y.tab.* core *~ *.bak
	rm -fr obj bin .libs
	rm -f $(bin_PROGRAMS)
	rm -f locations.dat
	
EXTRA_DIST = Makefile.win MdlParser.h*


