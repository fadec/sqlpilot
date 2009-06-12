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
	approach.vala \
	approachtable.vala \
	approachtype.vala \
	approachtypetable.vala \
	crew.vala \
	crewtable.vala \
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
	hold.vala \
	holdtable.vala \
	ijoinrecord.vala \
	ijointable.vala \
	landing.vala \
	landingtable.vala \
	launchtype.vala \
	launchtypetable.vala \
	logbook.vala \
	model.vala \
	modeltable.vala \
	modeltaggingtable.vala \
	modeltagtable.vala \
	ordinal.vala \
	person.vala \
	persontable.vala \
	record.vala \
	role.vala \
	roletable.vala \
	roletagtable.vala \
	roletaggingtable.vala \
	route.vala \
	routing.vala \
	routingtable.vala \
	surface.vala \
	surfacetable.vala \
	table.vala \
	tag.vala \
	tagging.vala \
	takeoff.vala \
	takeofftable.vala \
	timeofday.vala \
	timezone.vala \
	transaction.vala \
	util.vala

libsqlpilot_la_SOURCES = \
	vala.stamp \
	$(libsqlpilot_la_VALASOURCES:.vala=.c)

libsqlpilot.vapi vala.stamp: $(libsqlpilot_la_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) --library libsqlpilot \
	--vapidir $(top_srcdir)/vapi --pkg sqlite3 --pkg time \
	-C -H libsqlpilot.h $^
	touch $@

libsqlpilot_la_LIBADD = \
	$(GLIB_LIBS)    \
	$(SQLITE_LIBS)


EXTRA_DIST += $(libsqlpilot_la_VALASOURCES) libsqlpilot.vapi vala.stamp

MAINTAINERCLEANFILES = \
	libsqlpilot.vapi \
	libsqlpilot.h \
	$(libsqlpilot_la_VALASOURCES:.vala=.c)