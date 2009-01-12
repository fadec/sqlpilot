include $(top_srcdir)/Makefile.decl

noinst_LTLIBRARIES = libsqlpilot.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

BUILT_SOURCES = vala.stamp

libsqlpilot_la_VALASOURCES = \
	aircraft.vala \
	airport.vala \
	date.vala \
	duration.vala \
	flight.vala \
	model.vala \
	nicedb.vala \
	record.vala \
	role.vala \
	routing.vala \
	timeofday.vala

libsqlpilot_la_SOURCES = \
	$(libsqlpilot_la_VALASOURCES:.vala=.c) \
	$(libsqlpilot_la_VALASOURCES:.vala=.h)

# sqlpilotinclude_HEADERS = \
# 	$(libgee_la_VALASOURCES:.vala=.h)

vala.stamp: $(libsqlpilot_la_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) --library libsqlpilot --vapidir $(top_srcdir)/vapi --pkg sqlite3 -C $^
	touch $@

libsqlpilot_la_LIBADD = \
	$(GLIB_LIBS)    \
	$(SQLITE_LIBS)


EXTRA_DIST += vala.stamp
