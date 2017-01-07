AM_CPPFLAGS = $(WX_CPPFLAGS) -w -DSTRICT 

noinst_LIBRARIES = libmaitreya_wxJSON.a
libmaitreya_wxJSON_a_SOURCES = jsonreader.cpp jsonval.cpp jsonwriter.cpp \
	wx/json_defs.h wx/jsonreader.h wx/jsonval.h wx/jsonwriter.h

EXTRA_DIST = Makefile.win 

clean:
	rm -f lib*.a *.o *~ *.bak
	rm -fr obj bin


