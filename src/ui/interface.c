#include "sqlpilot.h"

#include <ui/interface.h>
#include "ui/flights.h"
#include "ui/callbacks.h"
#include <gtk/gtk.h>
#include "db/db.h"
#include "store.h"
#include <libintl.h>

extern Sqlpilot *sqlpilot;

void build_main_window(void)
{
  GtkWidget *window;
  GtkWidget *vbox;
	
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_signal_connect( GTK_OBJECT(window), "delete_event", gtk_main_quit, NULL );

  vbox = gtk_vbox_new(TRUE, 0);
  gtk_container_add( GTK_CONTAINER(window), vbox);
  gtk_widget_show( vbox );
	
  sqlpilot->window = window;
  sqlpilot->window_pane = vbox;
}

GtkWidget *logbook_notebook_build(Logbook *logbook)
{
  GtkWidget
    *notebook,
    *page_flights,
    *label_flights,
    *page_roles,
    *label_roles,
    *page_aircraft,
    *label_aircraft,
    *page_types,
    *label_types,
    *page_airports,
    *label_airports;

  notebook = gtk_notebook_new();

  label_flights = gtk_label_new("Flights");
  label_roles = gtk_label_new("Roles");
  label_aircraft = gtk_label_new("Aircraft");
  label_types = gtk_label_new("Types");
  label_airports = gtk_label_new("Airports");

  page_flights = logbook_page_flights_build(logbook);
  page_roles = logbook_page_roles_build(logbook);

  page_aircraft = gtk_vbox_new(FALSE, 0);
  page_types = gtk_vbox_new(FALSE, 0);
  page_airports = gtk_vbox_new(FALSE, 0);

  gtk_notebook_append_page(notebook, page_flights, label_flights);
  gtk_notebook_append_page(notebook, page_roles, label_roles);
  gtk_notebook_append_page(notebook, page_aircraft, label_aircraft);
  gtk_notebook_append_page(notebook, page_types, label_types);
  gtk_notebook_append_page(notebook, page_airports, label_airports);
 
  logbook->notebook = notebook;

  return notebook;
}

