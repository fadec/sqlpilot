#include "sqlpilot.h"

#include <ui/interface.h>
#include "ui/flights.h"
#include "ui/callbacks.h"
#include <gtk/gtk.h>
#include "db/db.h"
#include "store.h"
#include <libintl.h>



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
  sqlpilot->pane_box = vbox;
}

