include config.mk

SRC = src/main.c \
      src/db/db.c \
      src/ui/interface.c \
      src/ui/callbacks.c

HEADERS = src/sqlpilot.h \
	  src/db/db.h \
	  src/ui/interface.h \
	  src/ui/callbacks.h

OBJ = ${SRC:.c=.o}

all: sqlpilot

${OBJ}: ${HEADERS} config.mk

sqlpilot: ${OBJ}
	@${LD} -o $@ ${OBJ} ${LDFLAGS}
#	@strip $@

clean:
	-rm -f `find -name *.o`
	-rm -f core sqlpilot

# Remove files and directories that are not under source control
scm-clean:
	-rm -rf `svn stat | grep ^? | sed s/.......//`

wc:
	@wc `find -name *.c` `find -name *.h`
