AM_CPPFLAGS = $(WX_CPPFLAGS) -w $(FONTCONFIG_CFLAGS) -DSTRICT 

noinst_LIBRARIES = libmaitreya_wxpdfdoc.a
libmaitreya_wxpdfdoc_a_SOURCES = pdfannotation.cpp pdfbarcode.cpp pdfcffdecoder.cpp \
	pdfcffindex.cpp pdfcolour.cpp pdfdc.cpp pdfdecode.cpp pdfdocument.cpp pdfencoding.cpp \
	pdfencrypt.cpp pdffont.cpp pdffontdata.cpp pdffontdatacore.cpp pdffontdataopentype.cpp \
	pdffontdatatruetype.cpp pdffontdatatype0.cpp pdffontdatatype1.cpp pdffontdescription.cpp \
	pdffontdetails.cpp pdffontextended.cpp pdffontmanager.cpp pdffontparser.cpp \
	pdffontparsertruetype.cpp pdffontparsertype1.cpp pdffontsubsetcff.cpp \
	pdffontsubsettruetype.cpp pdffontvolt.cpp pdfform.cpp pdfgradient.cpp pdfgraphics.cpp \
	pdfimage.cpp pdfkernel.cpp pdflayer.cpp pdfobjects.cpp pdfocg.cpp pdfparser.cpp \
	pdfpattern.cpp pdfrijndael.cpp pdftemplate.cpp pdfutility.cpp pdfxml.cpp \
	wx/pdfannotation.h wx/pdfarraydouble.h wx/pdfarraytypes.h wx/pdfbarcode.h wx/pdfbookmark.h \
	wx/pdfcffdecoder.h wx/pdfcffindex.h wx/pdfcjkfontdata.h wx/pdfcolour.h wx/pdfcoonspatchmesh.h \
	wx/pdfcorefontdata.h wx/pdfdc.h wx/pdfdc28.h wx/pdfdc29.h wx/pdfdoc.h wx/pdfdocdef.h \
	wx/pdfdocument.h wx/pdfencoding.h wx/pdfencrypt.h wx/pdffont.h wx/pdffontdata.h \
	wx/pdffontdatacore.h wx/pdffontdataopentype.h wx/pdffontdatatruetype.h wx/pdffontdatatype0.h \
	wx/pdffontdatatype1.h wx/pdffontdescription.h wx/pdffontdetails.h wx/pdffontextended.h \
	wx/pdffontmacosx.h wx/pdffontmanager.h wx/pdffontparser.h wx/pdffontparsertruetype.h \
	wx/pdffontparsertype1.h wx/pdffontsubsetcff.h wx/pdffontsubsettruetype.h wx/pdffontvolt.h \
	wx/pdfform.h wx/pdfgradient.h wx/pdfgraphics.h wx/pdfimage.h wx/pdfinfo.h wx/pdflayer.h \
	wx/pdflinestyle.h wx/pdflinks.h wx/pdfobjects.h wx/pdfparser.h wx/pdfpattern.h \
	wx/pdfproperties.h wx/pdfrijndael.h wx/pdfshape.h wx/pdfspotcolour.h wx/pdftemplate.h \
	wx/pdfutility.h wx/pdfxml.h wxmemdbg.h 

EXTRA_DIST = Makefile.win pdfcjkfontdata.inc pdfcmapdata.inc pdfcodepagedata.inc \
	pdfcolourdata.inc pdfcorefontdata.inc pdfcorefontkerning.inc pdfdc28.inc pdfdc29.inc pdfglyphnames.inc

clean:
	rm -f lib*.a *.o *~ *.bak
	rm -fr obj bin



