include $(top_srcdir)/Makefile.decl

SUBDIRS = \
	libcsv \
	sqlite3ext \
	libsqlpilot \
	import \
	gtk \
	test

db-reset: sql/logbook.sql
	-rm -f logbook.db
	cat sql/logbook.sql | script/console logbook.db

db-reload: db-reset
	-awk -f db/airports/airports.awk db/airports/airports.csv | sqlite3 logbook.db

wc:
	find -name '*.vala' | xargs wc

gdb:
	gdb --args import/sqlpilot-import logbook.db ttt