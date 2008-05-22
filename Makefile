include config.mk

SRC = src/sqlpilot.c \
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

HEADERS = src/config.h \
	  src/sqlpilot.h \
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

ifeq ($(USING_HILDON),true)
  UI_GLADE=data/ui/hildon_nokia.glade
else
  UI_GLADE=data/ui/gtk_full.glade
endif

build: sqlpilot importcsv shell ui

src/config.h: config.mk
	@echo '/* config.h is autogenerated by Makefile */' > src/config.h
	@echo "#define VERSION_MAJOR $(VERSION_MAJOR)"      >> src/config.h
	@echo "#define VERSION_MINOR $(VERSION_MINOR)"      >> src/config.h
	@echo "#define VERSION_TINY  $(VERSION_TINY)"       >> src/config.h
	@echo "#define VERSION_STRING \"$(VERSION_MAJOR).$(VERSION_MINOR).$(VERSION_TINY)\"" >> src/config.h
	@if [ '$(USING_HILDON)' == 'true' ]; then echo "#define USING_HILDON" >> src/config.h; fi;
	@if [ '$(USING_GTK_BUILDER)' == 'true' ]; then echo "#define USING_GTK_BUILDER" >> src/config.h; fi;
	@echo Generated config.h

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

data/ui/sqlpilot.xml: $(UI_GLADE) config.mk
	sed 's/<property name="response_id">0<\/property>//g' $(UI_GLADE) > data/ui/sqlpilot.glade.tmp
	$(PYTHON) `which gtk-builder-convert` data/ui/sqlpilot.glade.tmp data/ui/sqlpilot.xml
	rm data/ui/sqlpilot.glade.tmp

etags:
	etags.emacs `find -name "*.[h|c]"`

run: sqlpilot
	./sqlpilot logbook.db

clean:
	-rm -f `find -name '*.o'`
	-rm -f core sqlpilot shell importcsv *_test test.db data/ui/sqlpilot.xml
	-rm src/config.h

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

