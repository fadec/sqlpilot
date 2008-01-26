#include "sqlpilot.h"

extern Sqlpilot *sqlpilot;

#include <ui/interface.h>
#include "ui/flights.h"
#include "ui/callbacks.h"
#include <gtk/gtk.h>
#include "db/db.h"
#include "store.h"
#include <libintl.h>

static void build_window(Interface*);

void interface_destroy(Interface *interface)
{
	g_free(interface);
}

Interface* interface_create(void)
{
	Interface* result;
	result = g_new0(Interface, 1);
	
	build_window( result );
	
	return result;
}

static void build_window(Interface *iface)
{
	GtkWidget *vbox;
	GtkWidget *flights_pane;
	
	iface->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect( GTK_OBJECT(iface->window), "delete_event", gtk_main_quit, NULL );

	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add( GTK_CONTAINER(iface->window), vbox);
	gtk_widget_show( vbox );
	
	flights_pane = build_flights_pane(iface);
	gtk_container_add (GTK_CONTAINER(vbox), flights_pane);
	//create_menu( vbox );
}

