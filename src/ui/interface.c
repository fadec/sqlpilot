
#include <ui/interface.h>
#include <ui/callbacks.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <libintl.h>


static void create_window(MainView*);
static void create_menu(GtkWidget*);
static void create_flight_cell(GtkWidget*);

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
	GtkWidget* vbox;
	
	main->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_signal_connect( GTK_OBJECT(main->window), "delete_event", gtk_main_quit, NULL );

	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add( GTK_CONTAINER(main->window), vbox);
	gtk_widget_show( vbox );

	create_flight_cell( vbox );
	create_menu( vbox );
}

static void create_menu(GtkWidget* container)
{
	GtkWidget* menu;
	menu = gtk_button_new_with_label("hello");
	gtk_container_add(GTK_CONTAINER(container), menu);
	gtk_widget_show(menu);
}

static void create_flight_cell(GtkWidget* container)
{
	GtkWidget* vbox;
	GtkWidget* row1;

	GtkWidget* fltno;
	GtkWidget* sdep;
	GtkWidget* sarr;

	vbox = gtk_vbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(container), vbox);
	gtk_widget_show(vbox);

	row1 = gtk_hbox_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(vbox), row1);
	gtk_widget_show(row1);

	fltno = gtk_entry_new_with_max_length(10);
	gtk_container_add(GTK_CONTAINER(row1), fltno);
	gtk_widget_show(fltno);

	sdep = gtk_entry_new_with_max_length(10);
	gtk_container_add(GTK_CONTAINER(row1), sdep);
	gtk_widget_show(sdep);

}

static void build_airport_manager(GtkWidget *container)
{
	GtkWidget *vbox;


};

GtkListStore *create_list_store_from_sql(const char *sql)
{
}

void create_list_store_columns(const char *sql, sqlite3_stmt stmt)
{
}

void populate_list_store_from_sql(GtkListStore *model, const char *sql)
{

}
