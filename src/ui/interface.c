
#include <ui/interface.h>
#include <ui/callbacks.h>
#include <gtk/gtk.h>
#include <libintl.h>

#include <hildon-widgets/hildon-file-chooser-dialog.h>
#include <hildon-widgets/hildon-note.h>
#include <hildon-widgets/hildon-font-selection-dialog.h>


/* Creates and initialises a main_view */
MainView* interface_main_view_new( AppData *data )
{
	/* Zero memory with g_new0 */
	MainView* result = g_new0( MainView, 1 );
	/* Store handle to app's data to view's data */
	result->data = data;
	/* Store handle to hildon_window to app's data */
	result->data->window = HILDON_WINDOW(hildon_window_new());

	/* Create main vbox add it to window */
	GtkWidget *main_vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(result->data->window), main_vbox);

	GtkWidget* button = gtk_button_new_with_label("Hello");
	gtk_container_add(GTK_CONTAINER(main_vbox), button);

	gtk_widget_show(GTK_WIDGET(button));
	gtk_widget_show(GTK_WIDGET(main_vbox));

	gtk_widget_show(GTK_WIDGET(result->data->window));

	/* Return with new mainview handle */
	return result;
}

/* clean up the allocated memory */
void interface_main_view_destroy( MainView *main )
{
	g_free( main );
}

