dnl
dnl  Maitreya
dnl
dnl  File: configure.ac
dnl
dnl

AC_INIT(maitreya8, 8.0.alpha1e)
AM_INIT_AUTOMAKE()

dnl should be included according to documentation, but not supported by older versions of automake
AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB

AC_PROG_YACC
AM_PROG_LEX

AM_CONFIG_HEADER(config.h)
AC_HEADER_STDC

AM_GNU_GETTEXT([external])


dnl
dnl  Check for GUI toolkit wxWidgets
dnl
AM_OPTIONS_WXCONFIG

AM_PATH_WXCONFIG(2.8.0, wxWin=1)
if test "$wxWin" != 1; then
AC_MSG_ERROR([
wxWindows >= 2.8 must be installed on your system
but wx-config script could not be found.

Please check that wx-config is in path, the directory
where wxWindows libraries are installed (returned by
'wx-config --libs' command) is in LD_LIBRARY_PATH or
equivalent variable and wxWindows version is 2.8.0 or above.
])
fi

dnl
dnl Fontconfig is implicitly needed by wxpdfdoc for wxgtk
dnl
dnl so check if WXGTK is used and add fontconfig stuff then
dnl
WX_BASENAME=`$WX_CONFIG_WITH_ARGS --basename`
wxgtkset="`echo $WX_BASENAME | grep gtk`"

if test "x$wxgtkset" != x; then
	PKG_CHECK_MODULES([FONTCONFIG], [fontconfig >= 2.0])
fi

dnl
dnl Swiss Ephemeris
dnl
AC_ARG_WITH([swe], [  --with-swe=builtin|system: use
            1) builtin Swiss Ephemeris (default) or
            2) determined by pkg-config or variables LIBSWE_CFLAGS and LIBSWE_LIBS
], [sweval=${withval}], [sweval=builtin])

case ${sweval} in
	system)
		PKG_CHECK_MODULES([LIBSWE], [libswe >= 1.77])
		use_swe_builtin=0

		swe_ver=`$PKG_CONFIG --modversion --print-errors "libswe"`
		if test x$swe_ver = "x"; then
			swe_ver=unknown
		fi 
		USE_SWE="system/${swe_ver}"
	;;
	builtin)
		use_swe_builtin=1
		swe_ver=none
		USE_SWE=builtin
	;;
	*) AC_MSG_ERROR([bad value ${withval} for --with-swe]) ;;
esac

# export variables to config.h and Makefiles
AC_DEFINE_UNQUOTED(USE_SWE_BUILTIN, $use_swe_builtin, [Use builtin Swiss Ephemeris])
AM_CONDITIONAL( [USE_SWE_BUILTIN], [ test "${use_swe_builtin}" = "1" ])
AC_DEFINE_UNQUOTED(SWE_SYSTEM_VERSION, "$swe_ver", [Package version of system wide Swiss Ephemeris])
dnl AC_DEFINE_UNQUOTED(LIBSWE_CFLAGS, "$LIBSWE_CFLAGS", [CFLAGS for Swiss Ephemeris])
dnl AC_DEFINE_UNQUOTED(LIBSWE_LIBS, "$LIBSWE_LIBS", [LIBS for Swiss Ephemeris])


dnl
dnl wxsqlite
dnl
AC_ARG_WITH([wxsqlite3], [  --with-wxsqlite3=builtin|system: use
            1) builtin wxsqlite3 (default) or
            2) determined by pkg-config or variables WXSQLITE3_CFLAGS and WXSQLITE3_LIBS
], [wxsqliteval=${withval}], [wxsqliteval=builtin])

case ${wxsqliteval} in
	system)
		PKG_CHECK_MODULES([WXSQLITE3], [wxsqlite3 >= 2.8])
		use_wxsqlite_builtin=0

		wxsqlite_ver=`$PKG_CONFIG --modversion --print-errors "wxsqlite3"`
		if test x$wxsqlite_ver = "x"; then
			wxsqlite_ver=unknown
		fi
		use_wxsqlite_builtin=0
		USE_WXSQLITE="system/${wxsqlite_ver}"
	;;
	builtin)
		use_wxsqlite_builtin=1
		wxsqlite_ver=none
		USE_WXSQLITE=builtin
	;;
	*) AC_MSG_ERROR([bad value ${withval} for --with-wxsqlite3]) ;;
esac

# export variables to config.h and Makefiles
AC_DEFINE_UNQUOTED(USE_WXSQLITE_BUILTIN, $use_wxsqlite_builtin, [Use builtin wxsqlite3])
AM_CONDITIONAL( [USE_WXSQLITE_BUILTIN], [test "${use_wxsqlite_builtin}" == "1" ])
AC_DEFINE_UNQUOTED(WXSQLITE_SYSTEM_VERSION, "$wxsqlite_ver", [Package version of system wide wxsqlite3])
AC_DEFINE_UNQUOTED(WXSQLITE3_CFLAGS, "$WXSQLITE3_CFLAGS", [CFLAGS for wxsqlite3])
AC_DEFINE_UNQUOTED(WXSQLITE3_LIBS, "$WXSQLITE3_LIBS", [LIBS for wxsqlite3])


dnl
dnl wxpdfdoc
dnl
AC_ARG_WITH([wxpdfdoc], [  --with-wxpdfdoc=builtin|system: use
            1) builtin wxpdfdoc (default) or
            2) determined by pkg-config or variables WXPDFDOC_CFLAGS and WXPDFDOC_LIBS
], [wxpdfdocval=${withval}], [wxpdfdocval=builtin])

case ${wxpdfdocval} in
	system)
		PKG_CHECK_MODULES([WXPDFDOC], [wxpdfdoc >= 0.9.3])
		use_wxpdfdoc_builtin=0

		wxpdfdoc_ver=`$PKG_CONFIG --modversion --print-errors "wxpdfdoc"`
		if test x$wxpdfdoc_ver = "x"; then
			wxpdfdoc_ver=unknown
		fi
		USE_WXPDFDOC="system/${wxpdfdoc_ver}"
	;;
	builtin)
		use_wxpdfdoc_builtin=1
		wxpdfdoc_ver=none
		USE_WXPDFDOC=builtin
	;;
	*) AC_MSG_ERROR([bad value ${withval} for --with-wxPDFDOC]) ;;
esac

AC_DEFINE_UNQUOTED(USE_WXPDFDOC_BUILTIN, $use_wxpdfdoc_builtin, [Use builtin wxpdfdoc])
AM_CONDITIONAL( [USE_WXPDFDOC_BUILTIN], [test "${use_wxpdfdoc_builtin}" = "1" ])
AC_DEFINE_UNQUOTED(WXPDFDOC_SYSTEM_VERSION, "$wxpdfdoc_ver", [Package version of system wide wxpdfdoc])
AC_DEFINE_UNQUOTED(WXPDFDOC_CFLAGS, "$WXPDFDOC_CFLAGS", [CFLAGS for wxpdfdoc])
AC_DEFINE_UNQUOTED(WXPDFDOC_LIBS, "$WXPDFDOC_LIBS", [LIBS for wxpdfdoc])

AC_OUTPUT( Makefile po/Makefile.in
  debian/Makefile
  src/Makefile
  src/wxJSON/Makefile
  src/base/Makefile
  src/wxsqlite3/Makefile
  src/swe/Makefile
  src/jyotish/Makefile
  src/views/Makefile
  src/gui/Makefile
  src/pics/Makefile
  src/wxpdfdoc/Makefile
  src/atlas/Makefile
  src/fonts/Makefile
  src/resources/Makefile
  src/resources/yogas/Makefile
  src/resources/dasas/Makefile
  src/resources/wchart/Makefile
  src/resources/vchart/Makefile
  src/resources/mview/Makefile
  src/resources/print/Makefile
  src/dialogs/Makefile
)

echo
echo "------------------------------------------------------"
echo
echo "  Configuration successful!"
echo
echo "General options"
echo "   Software version :   $PACKAGE_VERSION"
echo "   Prefix           :   $prefix"
echo "   C Compiler       :   $CC"
echo "   C++ Compiler     :   $CXX"
echo
echo "wxWidgets options"
echo "   wx version       :   $WX_VERSION"
echo "   wx-config        :   $WX_CONFIG_WITH_ARGS"
echo
echo "fontconfig options"
echo "   cflags : ${FONTCONFIG_CFLAGS}"
echo "   libs   : ${FONTCONFIG_LIBS}"
echo
echo "Additional library options"
echo "   Swiss Ephemeris   : ${USE_SWE}"
echo "   wxpdfdoc          : ${USE_WXPDFDOC}"
echo "   wxsqlite3         : ${USE_WXSQLITE}"
echo
echo "You can type 'make' now"
echo
echo "------------------------------------------------------"
echo

