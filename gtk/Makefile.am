include $(top_srcdir)/Makefile.decl

bin_PROGRAMS = sqlpilot-gtk

AM_CPPFLAGS = \
	-I$(top_srcdir)/libsqlpilot \
	$(GLIB_CFLAGS)  \
	$(GTK_CFLAGS)   \
	$(SQLITE_CFLAGS) \
	$(LIBGLADE_CFLAGS) \
	$(GEE_CFLAGS)

BUILT_SOURCES = vala.stamp

sqlpilot_gtk_VALASOURCES = \
	about.vala \
	aircrafteditor.vala \
	aircraftfields.vala \
	airporteditor.vala \
	airportfields.vala \
	application.vala \
	approachtypeeditor.vala \
	approacheditor.vala \
	approachview.vala \
	book.vala \
	browser.vala \
	component.vala \
	crewchooser.vala \
	fieldset.vala \
	filters.vala \
	filtertree.vala \
	flightbrowser.vala \
	flightfields.vala \
	glideeditor.vala \
	glideview.vala \
	gui.vala \
	holdeditor.vala \
	holdview.vala \
	jointableeditor.vala \
	landingeditor.vala \
	landingview.vala \
	launchtypeeditor.vala \
	main.vala \
	modeleditor.vala \
	modelfields.vala \
	pane.vala \
	parentchildeditor.vala \
	personeditor.vala \
	roleeditor.vala \
	rolefields.vala \
	settreemodelfilter.vala \
	surfaceeditor.vala \
	tableeditor.vala \
	tableeditorwithfields.vala \
	tableobserverstore.vala \
	tableview.vala \
	tagchooser.vala \
	tageditor.vala \
	takeoffeditor.vala \
	takeoffview.vala \
	util.vala

sqlpilot_gtk_SOURCES = \
	sqlpilot.vala.stamp \
	$(sqlpilot_gtk_VALASOURCES:.vala=.c)

vala.stamp: $(sqlpilot_gtk_VALASOURCES)
	$(VALAC) $(VALAFLAGS) \
	--vapidir $(top_srcdir)/libsqlpilot \
	--pkg libsqlpilot \
	--vapidir $(top_srcdir)/vapi \
	--pkg gmodule-2.0 \
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

MAINTAINERCLEANFILES = \
	$(sqlpilot_gtk_VALASOURCES:.vala=.c)