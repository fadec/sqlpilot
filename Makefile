include config.mk

SRC = src/sqlpilot.c \
      src/tmz.c \
      src/util.c \
      src/lib/csv.c \
      src/db/db.c \
      src/store.c \
      src/cb/window.c \
      src/cb/flights.c \
      src/cb/roles.c \
      src/cb/aircraft.c \
      src/cb/types.c \
      src/cb/airports.c \
      src/edctrl.c \
      src/flights.c \
      src/roles.c \
      src/aircraft.c \
      src/types.c \
      src/airports.c

APP_SRC = src/main.c ${SRC}

IMPORTCSV_SRC = src/importcsv.c ${SRC}

SHELL_SRC = src/shell.c  src/db/db.c

# Example: make test db, make test-run db
TEST_SRC = test/units/$(unit)_test.c test/test.c ${SRC}


HEADERS = src/sqlpilot.h \
	  src/tmz.h \
	  src/util.h \
	  src/lib/csv.h \
	  src/db/db.h \
          src/store.h \
	  src/cb/cb.h \
	  src/edctrl.h \
	  src/flights.h \
	  src/roles.h \
	  src/aircraft.h \
	  src/types.h \
	  src/airports.h

APP_HEADERS = src/sqlpilot.h ${HEADERS}

IMPORTCSV_HEADERS = ${HEADERS}

SHELL_HEADERS = src/db/db.h

TEST_HEADERS = test/test.h ${HEADERS}



APP_OBJ = ${APP_SRC:.c=.o}

IMPORTCSV_OBJ = ${IMPORTCSV_SRC:.c=.o}

SHELL_OBJ = ${SHELL_SRC:.c=.o}

TEST_OBJ = ${TEST_SRC:.c=.o}



app: sqlpilot importcsv shell ui

${APP_OBJ}: ${APP_HEADERS} config.mk

${IMPORTCSV_OBJ}: ${IMPORTCSV_HEADERS} config.mk

${SHELL_OBJ}: ${SHELL_HEADERS} config.mk

${TEST_OBJ}: ${TEST_HEADERS} config.mk



sqlpilot: ${APP_OBJ}
	${LD} -o $@ ${APP_OBJ} ${LDFLAGS}
#	@strip $@

importcsv: ${IMPORTCSV_OBJ}
	${LD} -o $@ ${IMPORTCSV_OBJ} ${LDFLAGS}
#	@strip $@

shell: ${SHELL_OBJ}
	${LD} -o $@ ${SHELL_OBJ} ${LDFLAGS}
#	@strip $@

ui: data/ui/sqlpilot.xml

data/ui/sqlpilot.xml: data/ui/sqlpilot.glade script/gtk-builder-convert
	sed -i 's/<property name="response_id">0<\/property>//g' data/ui/sqlpilot.glade
	script/gtk-builder-convert data/ui/sqlpilot.glade data/ui/sqlpilot.xml

etags:
	etags.emacs `find -name "*.[h|c]"`

run: sqlpilot
	./sqlpilot logbook.db

clean:
	-rm -f `find -name '*.o'`
	-rm -f core sqlpilot shell importcsv *_test test.db data/ui/sqlpilot.xml

# Remove units and directories that are not under source control - use with care
scm-clean:
	-rm -rf `svn stat | grep ^? | sed s/.......//`

wc:
	@wc `find -name '*.c'` `find -name '*.h'`

db-reset: db/logbook.sql
	-rm -f logbook.db
	cat db/logbook.sql | ./shell logbook.db

db-reload: db-reset
	-awk -f db/airports/airports.awk db/airports/airports.csv | sqlite3 logbook.db
#	cat ~/logbook/save.sql | sqlite3 logbook.db
#	./importcsv ~/logbook/_finished.csv logbook.db


test: $(unit)_test

test-run: test
	@printf "### Running $(unit)_test ###\n"
	@rm -f test.db
	@if [ -f test/fixtures/$(unit).sql ]; then sqlite3 test.db "`cat test/fixtures/$(unit).sql`"; fi;
	@./$(unit)_test
	@printf "\n"

$(unit)_test: ${TEST_OBJ}
	${LD} -o $@ ${TEST_OBJ} ${LDFLAGS}

