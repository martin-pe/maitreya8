
SUBDIRS = po src debian 

# m4 not automatically included on OS X make dist
EXTRA_DIST = config.h autogen.sh Makefile.win maitreya8.desktop \
	Doxyfile maitreya8.spec create_mac_release.sh MacOS/* m4/*

pkgdatadir = ${datarootdir}/maitreya8

dist_pkgdata_DATA = COPYING debian/changelog


all-am: echo-font-message

echo-font-message:
	@echo ""
	@echo ""
	@echo "HOW TO START"
	@echo "------------"
	@echo "You can start the program with the following commands"
	@echo "1. local installation: ./src/gui/maitreya8"
	@echo "2. system wide installation: type 'sudo make install' followed by 'maitreya8'"
	@echo ""
	@echo ""
	@echo "FONT INFO"
	@echo "---------"
	@echo "The symbol font will be installed on most systems by 'make install'."
	@echo "You can also manually install it."
	@echo "It is located at src/fonts/Saravali.ttf"
	@echo ""

AUTOMAKE_OPTIONS = dist-bzip2 dist-zip

ACLOCAL_AMFLAGS = -I m4

desktopdir = ${datarootdir}/applications
desktop_DATA = maitreya8.desktop

clean: clean-recursive
	rm -f *~ *.bak fonts
	rm -fr autom4te.cache bin obj doc/* Maitreya*.app

