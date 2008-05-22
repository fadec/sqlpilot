# sqlpilot version
VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_TINY  = 0

HILDON=true

# for gtk-builder-convert
PYTHON=python2.5

# paths
PREFIX = /usr/local
CONFPREFIX = ${PREFIX}/etc
MANPREFIX = ${PREFIX}/share/man

ifeq ($(HILDON),true)
  INCS = -Itest -Isrc -I${PREFIX}/include -I/usr/include `pkg-config hildon-1 --cflags`
  LIBS = -L/usr/lib -lc -L${PREFIX}/lib -lm -lsqlite3 -lreadline `pkg-config hildon-1 --libs`
else
  INCS = -Itest -Isrc -I${PREFIX}/include -I/usr/include `pkg-config gtk+-2.0 --cflags`
  LIBS = -L/usr/lib -lc -L${PREFIX}/lib -lm -lsqlite3 -lreadline `pkg-config gtk+-2.0 --libs`
endif

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
