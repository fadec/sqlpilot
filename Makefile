include config.mk

SRC = src/db/db.c \
      src/ui/interface.c \
      src/ui/callbacks.c

PROG_SRC = src/main.c ${SRC}

# Example: make test db, make test-run db
TEST_SRC = test/units/$(unit).c test/test.c ${SRC}

HEADERS = src/db/db.h \
	  src/ui/interface.h \
	  src/ui/callbacks.h

PROG_HEADERS = src/sqlpilot.h ${HEADERS}

TEST_HEADERS = test/test.h ${HEADERS}

PROG_OBJ = ${PROG_SRC:.c=.o}

TEST_OBJ = ${TEST_SRC:.c=.o}

build: sqlpilot

${PROG_OBJ}: ${PROG_HEADERS} config.mk

${TEST_OBJ}: ${TEST_HEADERS} config.mk

sqlpilot: ${PROG_OBJ}
	${LD} -o $@ ${PROG_OBJ} ${LDFLAGS}
#	@strip $@

clean:
	-rm -f `find -name '*.o'`
	-rm -f core sqlpilot *_test test.db

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

