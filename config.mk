# sqlpilot version
VERSION = 0.0

# Customize below to fit your system

# paths
PREFIX = /usr/local
CONFPREFIX = ${PREFIX}/etc
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -Isrc -I${PREFIX}/include -I/usr/include `pkg-config gtk+-2.0 --cflags`
LIBS = -L/usr/lib -lc -L${PREFIX}/lib -lm `pkg-config gtk+-2.0 --libs`

# flags
CFLAGS = -Os ${INCS} -DVERSION=\"${VERSION}\"
LDFLAGS = ${LIBS}
#CFLAGS = -g -Wall -O2 ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = -g ${LIBS}

# Solaris
#CFLAGS = -fast ${INCS} -DVERSION=\"${VERSION}\"
#LDFLAGS = ${LIBS}
#LDFLAGS += -lsocket -lnsl
#CFLAGS += -xtarget=ultra

# compiler and linker
CC = cc
LD = ${CC}
