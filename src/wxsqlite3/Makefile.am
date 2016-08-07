# unused dlopen calls cause linker errors on several platforms
SQLITEFLAG = -D SQLITE_OMIT_LOAD_EXTENSION

AM_CPPFLAGS = $(SQLITEFLAG) $(WX_CPPFLAGS) -w -DSTRICT 

noinst_LIBRARIES = libmaitreya_wxsqlite3.a
libmaitreya_wxsqlite3_a_SOURCES = sqlite3.c sqlite3.h wxsqlite3.cpp wx/wxsqlite3def.h \
	wx/wxsqlite3dyn.h wx/wxsqlite3.h wx/wxsqlite3opt.h

EXTRA_DIST = Makefile.win 

clean:
	rm -f lib*.a *.o *~ *.bak
	rm -fr obj bin


