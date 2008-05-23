# sqlpilot version
VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_TINY  = 0

USING_HILDON=true
USING_GTK_BUILDER=false

PACKAGES = gtk+-2.0 libglade-2.0 hildon-libs

# for gtk-builder-convert
PYTHON=python2.5

# paths
PREFIX = /usr/local
CONFPREFIX = ${PREFIX}/etc
MANPREFIX = ${PREFIX}/share/man

INCS = -Itest -Isrc -I${PREFIX}/include -I/usr/include `pkg-config $(PACKAGES) --cflags`
LIBS = -L/usr/lib -lc -L${PREFIX}/lib -lm -lreadline `pkg-config $(PACKAGES) --libs`

# includes and libs


# flags
CFLAGS = -g3 -Wall -O0 ${INCS}
LDFLAGS = -g3 -export-dynamic ${LIBS}

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}
#LDFLAGS += -lsocket -lnsl
#CFLAGS += -xtarget=ultra

# compiler and linker
CC = cc
LD = ${CC}
