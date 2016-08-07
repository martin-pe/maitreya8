
AM_CPPFLAGS = $(WX_CPPFLAGS) -I. -I../gui -I../base -I../jyotish -I../dialogs

noinst_LIBRARIES = libmaitreya_views.a

libmaitreya_views_a_SOURCES = AshtakaVargaView.cpp DasaGraphicView.cpp \
	DasaGraphicView.h DasaView.cpp DasaView.h EclipseView.cpp EphemView.cpp \
	EphemView.h GraphicView.cpp HoraView.cpp PartnerView.cpp PrintView.cpp \
	SbcView.cpp SheetView.cpp SheetView.h SolarView.cpp TextView.cpp \
	TransitView.cpp VargaView.cpp YogaView.cpp



all-am: 

clean:
	rm -f lib*.a *.o core *~ *.bak
	rm -fr obj bin .libs

EXTRA_DIST = Makefile.win 

