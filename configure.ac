AC_INIT([sqlpilot], [0.0.1], [samdanielson@gmail.com], [sqlpilot])

AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS(config.h)
AM_INIT_AUTOMAKE([dist-bzip2])

# Checks for programs.
AC_PROG_CC
AM_PROG_CC_C_O
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AC_PATH_PROG(VALAC, valac, valac)
AC_SUBST(VALAC)

AC_SUBST(CFLAGS)
AC_SUBST(CPPFLAGS)
AC_SUBST(LDFLAGS)

GLIB_REQUIRED=2.10.0
PKG_CHECK_MODULES(GLIB, [
	glib-2.0 >= $GLIB_REQUIRED
	gobject-2.0 >= $GLIB_REQUIRED
])

PKG_CHECK_MODULES(GTK, [
	gtk+-2.0 >= 2.10.0
])

PKG_CHECK_MODULES(SQLITE, [
	sqlite3 >= 3.6.7
])


AC_SUBST(GLIB_CFLAGS)
AC_SUBST(GLIB_LIBS)

AC_CONFIG_FILES([
	Makefile
	libcsv/Makefile
	sqlite3ext/Makefile
	libsqlpilot/Makefile
	gtk/Makefile
	import/Makefile
	test/Makefile
	])

AC_OUTPUT
