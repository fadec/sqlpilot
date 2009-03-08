include $(top_srcdir)/Makefile.decl

bin_PROGRAMS = sqlpilot-gtk

AM_CPPFLAGS = \
	-I$(top_srcdir) \
	$(GLIB_CFLAGS)  \
	$(GTK_CFLAGS)   \
	$(SQLITE_CFLAGS) \
	$(LIBGLADE_CFLAGS) \
	$(GEE_CFLAGS)

BUILT_SOURCES = vala.stamp

sqlpilot_gtk_VALASOURCES = \
	main.vala \
	util.vala \
	browser.vala \
	fieldset.vala \
	filters.vala \
	filtertree.vala \
	flightfields.vala \
	gui.vala \
	pane.vala \
	application.vala \
	about.vala \
	book.vala \
	airlineflightfields.vala \
	querylist.vala \
	component.vala \
	settreemodelfilter.vala

sqlpilot_gtk_SOURCES = \
	sqlpilot.vala.stamp \
	$(sqlpilot_gtk_VALASOURCES:.vala=.c) \
	$(sqlpilot_gtk_VALASOURCES:.vala=.h)

vala.stamp: $(sqlpilot_gtk_VALASOURCES)
	$(VALAC) $(VALAFLAGS) --basedir $(top_srcdir) \
	--vapidir $(top_srcdir)/libsqlpilot --pkg gmodule-2.0 --pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi \
	--pkg gtk+-2.0 \
	--pkg sqlite3 \
	--pkg libglade-2.0 \
	--pkg gee-1.0 \
	-C $^
	touch $@

sqlpilot_gtk_LDADD = \
	$(GLIB_LIBS) \
	$(GTK_LIBS) \
	$(SQLITE_LIBS) \
	$(LIBGLADE_LIBS) \
	$(GEE_LIBS) \
	../libsqlpilot/libsqlpilot.la

#vapidir = $(datadir)/vala/vapi

# dist_vapi_DATA = \
# 	gee-1.0.vapi \
# 	$(NULL)

# EXTRA_DIST += sqlpilot