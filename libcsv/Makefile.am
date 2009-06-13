include $(top_srcdir)/Makefile.decl

noinst_LTLIBRARIES = libcsv.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

BUILT_SOURCES = vala.stamp

libcsv_la_VALASOURCES = \
	csv.vala \
	ccsv.vapi

libcsv_la_SOURCES = \
	ccsv.c \
	ccsv.h \
	$(libcsv_la_VALASOURCES:.vala=.c) \
	$(libcsv_la_VALASOURCES:.vala=.h)

vala.stamp: $(libcsv_la_VALASOURCES)
	$(VALAC) --library libcsv --basedir $(top_srcdir) -C -H libcsv.h $^
	touch $@

libcsv_la_LIBADD = \
	$(GLIB_LIBS)

EXTRA_DIST += vala.stamp

