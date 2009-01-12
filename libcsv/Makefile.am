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

# sqlpilotinclude_HEADERS = \
# 	$(libgee_la_VALASOURCES:.vala=.h) \
# 	$(NULL)

vala.stamp: $(libcsv_la_VALASOURCES)
	$(VALAC) --library libcsv --basedir $(top_srcdir) -C $^
	touch $@

libcsv_la_LIBADD = \
	$(GLIB_LIBS)

#vapidir = $(datadir)/vala/vapi

# dist_vapi_DATA = \
# 	gee-1.0.vapi

EXTRA_DIST += vala.stamp

