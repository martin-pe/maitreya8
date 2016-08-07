
bin_PROGRAMS = maitreya8.bin maitreya8

maitreya8_SOURCES = maitreya8.template

AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../base -I../jyotish

maitreya8_bin_LDADD =  ../views/libmaitreya_views.a ../jyotish/libjyotish.a ../dialogs/libmaitreya_dialogs.a \
	../base/libmaitreya_base.a  ../wxJSON/libmaitreya_wxJSON.a

if USE_SWE_BUILTIN
maitreya8_bin_LDADD += ../swe/libswe.a
# maitreya8_bin_LDADD += ../swe/libswe.la
else
maitreya8_bin_LDADD += $(LIBSWE_LIBS)
endif !USE_SWE_BUILTIN

if USE_WXSQLITE_BUILTIN
maitreya8_bin_LDADD += ../wxsqlite3/libmaitreya_wxsqlite3.a
else
maitreya8_bin_LDADD += $(WXSQLITE3_LIBS)
endif !USE_WXSQLITE_BUILTIN


if USE_WXPDFDOC_BUILTIN
AM_CPPFLAGS += -I../wxpdfdoc
maitreya8_bin_LDADD += ../wxpdfdoc/libmaitreya_wxpdfdoc.a $(WX_LIBS) $(FONTCONFIG_LIBS) 
# maitreya8_bin_LDADD += ../wxpdfdoc/libmaitreya_wxpdfdoc.a $(FONTCONFIG_LIBS) 
# maitreya8_bin_LDADD += $(FONTCONFIG_LIBS) 
else
AM_CPPFLAGS += $(WXPDFDOC_CFLAGS)
maitreya8_bin_LDADD += $(WXPDFDOC_LIBS) $(WX_LIBS)
endif !USE_WXPDFDOC_BUILTIN

AM_CPPFLAGS += -I../dialogs -DSTRICT
# maitreya8_bin_LDADD += $(WX_LIBS) $(FONTCONFIG_LIBS) 

maitreya8_bin_SOURCES = ApplicationWindow.cpp ApplicationWindow.h BarDiagram.cpp \
	BarDiagram.h BasicVedicChart.cpp BasicVedicChart.h BasicView.cpp BasicView.h \
	BasicWesternChart.cpp BasicWesternChart.h BasicWidget.cpp BasicWidget.h ChartField.cpp \
	ChartField.h ChildWindow.cpp ChildWindow.h ChildWindowListCtrl.cpp ChildWindowListCtrl.h \
	DasaTreeWidget.cpp DasaTreeWidget.h Document.cpp Document.h GraphicalChart.cpp \
	GraphicalChart.h GraphicGrid.cpp GraphicGrid.h guibase.cpp guibase.h MenuProvider.cpp \
	MenuProvider.h MultipleChildWindow.cpp MultipleChildWindow.h PdfBase.cpp PdfBase.h \
	PdfPainter.cpp PdfPainter.h PdfTools.cpp PdfTools.h PrintoutSheetCreator.cpp \
	PrintoutSheetCreator.h SarvatobhadraChart.cpp SarvatobhadraChart.h SheetWidget.cpp \
	SheetWidget.h SheetWriter.cpp SheetWriter.h SplitterWidget.cpp SplitterWidget.h \
	Statusbar.cpp Statusbar.h ToolPanel.cpp ToolPanel.h TreeWidget.cpp TreeWidget.h \
	VedicChart.cpp VedicChart.h VedicRasiChart.cpp VedicRasiChart.h WesternChart.cpp \
	WesternChart.h YogaEditor.cpp YogaEditor.h


all-am: maitreya8

maitreya8$(EXEEXT) : maitreya8.template Makefile
	sed "s|DATAROOTDIR|${datarootdir}|g; s|ABSTOPBUILDDIR|${abs_top_builddir}|g" maitreya8.template > maitreya8
	chmod 755 maitreya8

links: 
	for l in `cat ../../po/LINGUAS`; \
	do \
	echo Creating link for language $$l; \
	test -d $$l || mkdir $$l; \
	test -L $$l/maitreya8.mo || ln -fs ../../../po/$$l.gmo $$l/maitreya8.mo; \
	done

pkgdatadir = ${datarootdir}/maitreya8

EXTRA_DIST = Makefile.win maitreya8.rc maitreya8.template

clean:
	rm -f *.o *~ *.bak maitreya8 maitreya8.bin
	rm -fr obj bin .libs
	for l in `cat ../../po/LINGUAS`; \
	do \
	rm -fr $$l; \
	done


