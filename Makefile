include config.mk

SRC = src/sqlpilot.c \
      src/util.c \
      src/db/db.c \
      src/store.c \
      src/cb/window.c \
      src/cb/flights.c \
      src/cb/roles.c \
      src/cb/aircraft.c \
      src/cb/types.c \
      src/cb/airports.c \

PROG_SRC = src/main.c ${SRC}

# Example: make test db, make test-run db
TEST_SRC = test/units/$(unit).c test/test.c ${SRC}

HEADERS = src/sqlpilot.h \
	  src/util.h \
	  src/db/db.h \
          src/store.h \
	  src/cb/cb.h \

APP_HEADERS = src/sqlpilot.h ${HEADERS}

TEST_HEADERS = test/test.h ${HEADERS}

APP_OBJ = ${PROG_SRC:.c=.o}

TEST_OBJ = ${TEST_SRC:.c=.o}

app: sqlpilot ui

${APP_OBJ}: ${APP_HEADERS} config.mk

${TEST_OBJ}: ${TEST_HEADERS} config.mk

sqlpilot: ${APP_OBJ}
	${LD} -o $@ ${APP_OBJ} ${LDFLAGS}
#	@strip $@

ui: data/ui/sqlpilot.xml

data/ui/sqlpilot.xml: data/ui/sqlpilot.glade
	gtk-builder-convert data/ui/sqlpilot.glade data/ui/sqlpilot.xml

etags:
	etags.emacs `find -name "*.[h|c]"`

run: sqlpilot
	./sqlpilot

clean:
	-rm -f `find -name '*.o'`
	-rm -f core sqlpilot *_test test.db data/ui/sqlpilot.xml

# Remove units and directories that are not under source control - use with care
scm-clean:
	-rm -rf `svn stat | grep ^? | sed s/.......//`

wc:
	@wc `find -name '*.c'` `find -name '*.h'`

db-reset: db/logbook.sql
	-rm -f db/logbook.db
	sqlite3 -init db/logbook.sql db/logbook.db

test: $(unit)_test

test-run: test
	@printf "### Running $(unit)_test ###\n"
	@rm -f test.db
	@sqlite3 test.db "`cat test/fixtures/$(unit).sql`"
	@./$(unit)_test
	@printf "\n"

$(unit)_test: ${TEST_OBJ}
	${LD} -o $@ ${TEST_OBJ} ${LDFLAGS}

