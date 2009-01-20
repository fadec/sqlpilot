include $(top_srcdir)/Makefile.decl

noinst_LTLIBRARIES = libsqlpilot.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

BUILT_SOURCES = vala.stamp

libsqlpilot_la_VALASOURCES = \
	aircraft.vala \
	aircraftcrud.vala \
	airport.vala \
	airportcrud.vala \
	crud.vala \
	duration.vala \
	flight.vala \
	flightcrud.vala \
	logbook.vala \
	model.vala \
	modelcrud.vala \
	record.vala \
	role.vala \
	rolecrud.vala \
	route.vala \
	routing.vala \
	routingcrud.vala \
	timeofday.vala \
	util.vala

libsqlpilot_la_SOURCES = \
	$(libsqlpilot_la_VALASOURCES:.vala=.c) \
	$(libsqlpilot_la_VALASOURCES:.vala=.h)

# libsqlpilotincludedir = $(includedir)/libsqlpilot
# libsqlpilotinclude_HEADERS = \
# 	$(libsqlpilot_la_VALASOURCES:.vala=.h)

vala.stamp: $(libsqlpilot_la_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) --library libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 --pkg stardate \
	--pkg stardate \
	-C $^
	touch $@

libsqlpilot_la_LIBADD = \
	$(GLIB_LIBS)    \
	$(SQLITE_LIBS)


EXTRA_DIST += vala.stamp
