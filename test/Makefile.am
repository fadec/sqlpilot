include $(top_srcdir)/Makefile.decl

check_PROGRAMS = \
	test-csv \
	test-date \
	test-timeofday \
	test-datetime \
	test-duration \
	test-logbook


TESTS = $(check_PROGRAMS)

TESTS_ENVIRONMENT = $(srcdir)/runner.sh

AM_CPPFLAGS = \
	-I$(top_srcdir)/libsqlpilot \
	$(GLIB_CFLAGS)

BUILT_SOURCES = \
	test_csv.vala.stamp \
	test_date.vala.stamp \
	test_timeofday.vala.stamp \
	test_datetime.vala.stamp \
	test_duration.vala.stamp \
	test_logbook.vala.stamp


test_csv_VALASOURCES = testcsv.vala

test_csv_SOURCES = \
	$(test_csv_VALASOURCES:.vala=.c) \
	$(test_csv_VALASOURCES:.vala=.h)


test_csv.vala.stamp: $(test_csv_VALASOURCES)
	$(VALAC) \
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
	$(VALAC) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 -C $^
	touch $@

test_logbook_LDADD = \
	$(GLIB_LIBS) \
	$(SQLITE_LIBS) \
	$(top_srcdir)/libsqlpilot/libsqlpilot.la



test_date_VALASOURCES = testdate.vala

test_date_SOURCES = \
	$(test_date_VALASOURCES:.vala=.c) \
	$(test_date_VALASOURCES:.vala=.h)


test_date.vala.stamp: $(test_date_VALASOURCES)
	$(VALAC) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 -C $^
	touch $@

test_date_LDADD = \
	$(GLIB_LIBS) \
	$(SQLITE_LIBS) \
	$(top_srcdir)/libsqlpilot/libsqlpilot.la


test_timeofday_VALASOURCES = testtimeofday.vala

test_timeofday_SOURCES = \
	$(test_timeofday_VALASOURCES:.vala=.c) \
	$(test_timeofday_VALASOURCES:.vala=.h)


test_timeofday.vala.stamp: $(test_timeofday_VALASOURCES)
	$(VALAC) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 -C $^
	touch $@

test_timeofday_LDADD = \
	$(GLIB_LIBS) \
	$(SQLITE_LIBS) \
	$(top_srcdir)/libsqlpilot/libsqlpilot.la



test_datetime_VALASOURCES = testdatetime.vala

test_datetime_SOURCES = \
	$(test_datetime_VALASOURCES:.vala=.c) \
	$(test_datetime_VALASOURCES:.vala=.h)


test_datetime.vala.stamp: $(test_datetime_VALASOURCES)
	$(VALAC) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 -C $^
	touch $@

test_datetime_LDADD = \
	$(GLIB_LIBS) \
	$(SQLITE_LIBS) \
	$(top_srcdir)/libsqlpilot/libsqlpilot.la



test_duration_VALASOURCES = testduration.vala

test_duration_SOURCES = \
	$(test_duration_VALASOURCES:.vala=.c) \
	$(test_duration_VALASOURCES:.vala=.h)


test_duration.vala.stamp: $(test_duration_VALASOURCES)
	$(VALAC) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 -C $^
	touch $@

test_duration_LDADD = \
	$(GLIB_LIBS) \
	$(SQLITE_LIBS) \
	$(top_srcdir)/libsqlpilot/libsqlpilot.la

