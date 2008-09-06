########################################################################
# Copyright (C) 2008  Sam Danielson                                    #
#                                                                      #
# This file is part of Sqlpilot.                                       #
#                                                                      #
# Sqlpilot is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU General Public License as published by #
# the Free Software Foundation, either version 3 of the License, or    #
# (at your option) any later version.                                  #
#                                                                      #
# Sqlpilot is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of       #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        #
# GNU General Public License for more details.                         #
#                                                                      #
# You should have received a copy of the GNU General Public License    #
# along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    #
########################################################################

# sqlpilot version
VERSION_MAJOR = 0
VERSION_MINOR = 0
VERSION_TINY  = 0

USING_HILDON=false
USING_GTK_BUILDER=true
USING_READLINE=true

# for gtk-builder-convert
PYTHON=python2.5

PACKAGES = gtk+-2.0 libgtkhtml-3.14 # libglade-2.0 hildon-libs

# paths
PREFIX = /usr/local
CONFPREFIX = ${PREFIX}/etc
MANPREFIX = ${PREFIX}/share/man

# includes and libs
INCS = -Itest -Isrc -I${PREFIX}/include -I/usr/include `pkg-config $(PACKAGES) --cflags`
LIBS = -L/usr/lib -lc -L${PREFIX}/lib -lm -lpthread -lreadline `pkg-config $(PACKAGES) --libs` -lsqlite3

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
