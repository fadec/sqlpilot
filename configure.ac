AC_INIT(Makefile.am)
AM_INIT_AUTOMAKE(sqlpilot, `date +%Y%m%d`)
AM_CONFIG_HEADER(config.h)

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_RANLIB
AC_PROG_LIBTOOL
AC_PROG_INTLTOOL
AC_HEADER_STDC

# Option to enable debugging
AC_ARG_ENABLE(debug, 
	      [AC_HELP_STRING([  --enable-debug],[Debugging (default=no)])], 
	      [with_debug=yes], [with_debug=no])

if test "x$with_debug" == "xyes" ; then
	CFLAGS="$CFLAGS -ggdb -O0 -DDEBUG -Wall -ansi -pedantic"
else
	CFLAGS="$CFLAGS -O2 -Wall -ansi -pedantic"
fi

# Hildon library dependsncies
#PKG_CHECK_MODULES(HILDON, hildon-libs >= 0.9.50 hildon-fm libossohelp)
#AC_SUBST(HILDON_LIBS)
#AC_SUBST(HILDON_CFLAGS)

# Only used for the .service file path
#PKG_CHECK_MODULES(DBUS, dbus-glib-1 >= 0.60)

# OSSO application framework dependencies
#PKG_CHECK_MODULES(OSSO, osso-af-settings >= 0.8.5 libosso >= 0.9.17 gnome-vfs-2.0 >= 2.8.4.11 gnome-vfs-module-2.0 >= 2.8.4.11)
#AC_SUBST(OSSO_LIBS)
#AC_SUBST(OSSO_CFLAGS)

# Localisation
GETTEXT_PACKAGE=$PACKAGE
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], "${GETTEXT_PACKAGE}", [Name of gettext package])
ALL_LINGUAS="en_US"
AM_GLIB_GNU_GETTEXT

# To make application visible in sqlpilot Task Navigator it needs a Desktop
# file for the application.
# D-BUS service file is needed to be able to launch the sqlpilot application
# and connect it to D-BUS services.
# The following line defines install directories for these files.
desktopentrydir=`$PKG_CONFIG osso-af-settings --variable=desktopentrydir`
serviceentrydir=`$PKG_CONFIG osso-af-settings --variable=dbusservicedir`

# Application locale install directory
localedir=`$PKG_CONFIG osso-af-settings --variable=localedir`

# Application pixmaps install directory
pixmapdir=`$PKG_CONFIG osso-af-settings --variable=hildonpixmapdir`

# Application icon install directories
icon_26x26dir=$datadir/icons/hicolor/26x26/hildon
icon_34x34dir=$datadir/icons/hicolor/34x34/hildon
icon_40x40dir=$datadir/icons/hicolor/40x40/hildon
icon_50x50dir=$datadir/icons/hicolor/50x50/hildon
icon_scalabledir=$datadir/icons/hicolor/scalable/hildon

# Help file install directory
helpfile_engbdir=$datadir/osso-help/en_US

# Hildon control panel plugin install directories
pluginlibdir=`$PKG_CONFIG hildon-control-panel --variable=plugindir`
plugindesktopentrydir=`$PKG_CONFIG hildon-control-panel --variable=plugindesktopentrydir`

# Define as variables in Makefiles
AC_SUBST(desktopentrydir)
AC_SUBST(serviceentrydir)
AC_SUBST(localedir)
AC_SUBST(pixmapdir)
AC_SUBST(icon_26x26dir)
AC_SUBST(icon_34x34dir)
AC_SUBST(icon_40x40dir)
AC_SUBST(icon_50x50dir)
AC_SUBST(icon_scalabledir)
AC_SUBST(pluginlibdir)
AC_SUBST(plugindesktopentrydir)
AC_SUBST(helpfile_engbdir)

AC_DEFINE_UNQUOTED([LOCALEDIR], "${localedir}", [Runtime locale catalog files path])
AC_DEFINE_UNQUOTED([PIXMAPDIR], "${pixmapdir}", [Runtime pixmap files path])

# Produce output files
AC_OUTPUT(
	  Makefile \
	  src/Makefile \
	  data/Makefile \
	  po/Makefile.in \
	  po/Makefile 
)
