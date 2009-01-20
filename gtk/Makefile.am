include $(top_srcdir)/Makefile.decl

bin_PROGRAMS = sqlpilot-gtk

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)  \
	$(GTK_CFLAGS)   \
	$(SQLITE_CFLAGS)

BUILT_SOURCES = vala.stamp

sqlpilot_gtk_VALASOURCES = \
	main.vala \
	editor.vala \
	fieldset.vala \
	flighteditor.vala \
	gui.vala \
	logbook.vala \
	mainwindow.vala \
	pane.vala

sqlpilot_gtk_SOURCES = \
	sqlpilot.vala.stamp \
	$(sqlpilot_gtk_VALASOURCES:.vala=.c) \
	$(sqlpilot_gtk_VALASOURCES:.vala=.h)

vala.stamp: $(sqlpilot_gtk_VALASOURCES)
	$(VALAC) --basedir $(top_srcdir) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi \
	--pkg gtk+-2.0 \
	--pkg sqlite3 \
	--pkg stardate \
	-C $^
	touch $@

sqlpilot_gtk_LDADD = \
	$(GLIB_LIBS) \
	$(GTK_LIBS) \
	$(SQLITE_LIBS) \
	../libsqlpilot/libsqlpilot.la

#vapidir = $(datadir)/vala/vapi

# dist_vapi_DATA = \
# 	gee-1.0.vapi \
# 	$(NULL)

# EXTRA_DIST += sqlpilot