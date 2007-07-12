include config.mk

SRC = src/main.c \
      src/ui/interface.c \
      src/ui/callbacks.c

OBJ = ${SRC:.c=.o}

all: sqlpilot

${OBJ}: src/ui/interface.h src/ui/callbacks.h config.mk

sqlpilot: ${OBJ}
	@${LD} -o $@ ${OBJ} ${LDFLAGS}
#	@strip $@

clean:
	-rm -f `find -name *.o`
	-rm -f core sqlpilot

# Remove files and directories that are not under source control
scm-clean:
	-rm -rf `svn stat | grep ^? | sed s/.......//`


