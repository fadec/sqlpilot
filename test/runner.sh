#!/bin/sh


if [ $1 = "./test-logbook" ]; then
    rm test-logbook.db
    echo 'SELECT load_extension("../sqlite3ext/.libs/libsqlpilot-sqlite3ext.so");' > test-logbook.sql
    cat test-logbook.sql ../sql/logbook.sql | sqlite3 test-logbook.db
fi;

$1