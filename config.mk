## Makefile configuration

# sqlpilot version
VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_TINY  = 0

# Customize below to fit your system

UI_GLADE=data/ui/nokia.glade
PYTHON=python2.5

# paths
PREFIX = /usr/local
CONFPREFIX = ${PREFIX}/etc
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -Itest -Isrc -I${PREFIX}/include -I/usr/include `pkg-config gtk+-2.0 --cflags`
LIBS = -L/usr/lib -lc -L${PREFIX}/lib -lm -lsqlite3 -lreadline `pkg-config gtk+-2.0 --libs`

# flags
#CFLAGS = -Os ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}
CFLAGS = -g3 -Wall -O0 ${INCS} -DVERSION=\"${VERSION}\"
LDFLAGS = -g3 -export-dynamic ${LIBS}

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}
#LDFLAGS += -lsocket -lnsl
#CFLAGS += -xtarget=ultra

# compiler and linker
CC = cc
LD = ${CC}
