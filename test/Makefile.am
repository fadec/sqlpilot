include $(top_srcdir)/Makefile.decl

TESTS = test-csv

check_PROGRAMS = test-csv

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

BUILT_SOURCES = vala.stamp

test_csv_VALASOURCES = testcsv.vala

test_csv_SOURCES = \
	$(test_csv_VALASOURCES:.vala=.c) \
	$(test_csv_VALASOURCES:.vala=.h)


vala.stamp: $(test_csv_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) \
	--vapidir $(top_srcdir)/libcsv --pkg libcsv -C $^
	touch $@

test_csv_LDADD = \
	$(GLIB_LIBS) \
	$(top_srcdir)/libcsv/libcsv.la


