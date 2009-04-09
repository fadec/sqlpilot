include $(top_srcdir)/Makefile.decl

noinst_LTLIBRARIES = libsqlpilot.la

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)

BUILT_SOURCES = vala.stamp

libsqlpilot_la_VALASOURCES = \
	aircraft.vala \
	aircrafttable.vala \
	airport.vala \
	airporttable.vala \
	database.vala \
	date.vala \
	datetime.vala \
	duration.vala \
	flight.vala \
	flighttable.vala \
	flighttagtable.vala \
	flighttaggingtable.vala \
	glide.vala \
	glidetable.vala \
	launchtype.vala \
	launchtypetable.vala \
	logbook.vala \
	model.vala \
	modeltable.vala \
	modeltaggingtable.vala \
	modeltagtable.vala \
	record.vala \
	role.vala \
	roletable.vala \
	roletagtable.vala \
	roletaggingtable.vala \
	route.vala \
	routing.vala \
	routingtable.vala \
	table.vala \
	tag.vala \
	tagging.vala \
	taggingtable.vala \
	tagtable.vala \
	timeofday.vala \
	timezone.vala \
	transaction.vala \
	util.vala

libsqlpilot_la_SOURCES = \
	$(libsqlpilot_la_VALASOURCES:.vala=.c) \
	$(libsqlpilot_la_VALASOURCES:.vala=.h)

# libsqlpilotincludedir = $(includedir)/libsqlpilot
# libsqlpilotinclude_HEADERS = \
# 	$(libsqlpilot_la_VALASOURCES:.vala=.h)

vala.stamp: $(libsqlpilot_la_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) --library libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 --pkg time \
	-C $^
	touch $@

libsqlpilot_la_LIBADD = \
	$(GLIB_LIBS)    \
	$(SQLITE_LIBS)


EXTRA_DIST += vala.stamp
