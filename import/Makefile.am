include $(top_srcdir)/Makefile.decl

bin_PROGRAMS = sqlpilot-import

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	-I$(top_srcdir)/libsqlpilot \
	-I$(top_srcdir)/libcsv \
	$(GLIB_CFLAGS)

BUILT_SOURCES = vala.stamp

sqlpilot_import_VALASOURCES = importer.vala

sqlpilot_import_SOURCES = \
	$(sqlpilot_import_VALASOURCES:.vala=.c) \
	$(sqlpilot_import_VALASOURCES:.vala=.h)


# sqlpilotinclude_HEADERS = \
# 	$(libgee_la_VALASOURCES:.vala=.h)


vala.stamp: $(sqlpilot_import_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir)     \
	--vapidir $(top_srcdir)/vapi \
	--vapidir $(top_srcdir)/libcsv --pkg libcsv         \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--pkg sqlite3 -C $^
	touch $@

sqlpilot_import_LDADD = \
	$(GLIB_LIBS) \
	../libcsv/libcsv.la \
	../libsqlpilot/libsqlpilot.la

#vapidir = $(datadir)/vala/vapi

# dist_vapi_DATA = \
# 	gee-1.0.vapi


# EXTRA_DIST += sqlpilot
