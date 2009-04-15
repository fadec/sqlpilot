include $(top_srcdir)/Makefile.decl

SUBDIRS = \
	libcsv \
	sqlite3ext \
	libsqlpilot \
	import \
	gtk \
	test

clean-stamps:
	find -name '*.stamp' | xargs rm

db-reset: sql/logbook.sql
	-rm -f logbook.db
	cat sql/logbook.sql | script/console logbook.db

db-reload: db-reset
	awk -f sql/airports/airports.awk sql/airports/airports.csv | script/console logbook.db

wc:
	find -name '*.vala' | xargs wc

gdb:
	gdb --args import/sqlpilot-import logbook.db ttt