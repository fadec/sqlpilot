include $(top_srcdir)/Makefile.decl

check_PROGRAMS = \
	test-csv \
	test-logbook

TESTS = $(check_PROGRAMS)

TESTS_ENVIRONMENT = $(srcdir)/runner.sh

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

BUILT_SOURCES = test_csv.vala.stamp test_logbook.vala.stamp


test_csv_VALASOURCES = testcsv.vala

test_csv_SOURCES = \
	$(test_csv_VALASOURCES:.vala=.c) \
	$(test_csv_VALASOURCES:.vala=.h)


test_csv.vala.stamp: $(test_csv_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) \
	--vapidir $(top_srcdir)/libcsv --pkg libcsv -C $^
	touch $@

test_csv_LDADD = \
	$(GLIB_LIBS) \
	$(top_srcdir)/libcsv/libcsv.la


test_logbook_VALASOURCES = testlogbook.vala

test_logbook_SOURCES = \
	$(test_logbook_VALASOURCES:.vala=.c) \
	$(test_logbook_VALASOURCES:.vala=.h)


test_logbook.vala.stamp: $(test_logbook_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 --pkg stardate -C $^
	touch $@

test_logbook_LDADD = \
	$(GLIB_LIBS) \
	$(SQLITE_LIBS) \
	$(top_srcdir)/libsqlpilot/libsqlpilot.la
