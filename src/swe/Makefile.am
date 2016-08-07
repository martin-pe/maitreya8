AM_CFLAGS = -w

noinst_LIBRARIES = libswe.a
libswe_a_SOURCES = swecl.c swedate.c swehouse.c swejpl.c swemini.c swemmoon.c \
	swemplan.c swepcalc.c swepdate.c sweph.c swephlib.c

EXTRA_PROGRAMS = swetest
swetest_SOURCES = swetest.c swehel.c
swetest_LDADD = libswe.a -lm

EXTRA_DIST = *.h swemptab.c Makefile.win 

clean:
	rm -f lib*.a *.o *~ *.bak 
	rm -fr obj bin .libs

