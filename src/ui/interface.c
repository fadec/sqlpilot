
#include <ui/interface.h>
#include <ui/callbacks.h>
#include <gtk/gtk.h>
#include <libintl.h>


static void create_window(MainView*);
static void create_window_menu(GtkWidget*);

void interface_main_view_destroy(MainView* main_view)
{
	g_free(main_view);
}

MainView* interface_main_view_create(void)
{
	MainView* result;
	result = g_new0(MainView, 1);
	
	create_window( result );
	
	return result;
}

static void create_window(MainView* main)
{
	main->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect( GTK_OBJECT(main->window), "delete_event", gtk_main_quit, NULL );
	
	create_window_menu( main->window );
}

static void create_window_menu(GtkWidget* window)
{
	GtkWidget* menu;
	menu = gtk_button_new_with_label("hello");
	gtk_container_add(GTK_CONTAINER(window), menu);
	gtk_widget_show(menu);
}
