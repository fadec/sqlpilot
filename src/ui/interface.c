
#include <ui/interface.h>
#include <ui/callbacks.h>
#include <gtk/gtk.h>
#include <db/db.h>
#include <libintl.h>


static void create_window(MainView*);
static void create_menu(GtkWidget*);
static void create_flight_cell(GtkWidget*);

static void lookatme(GtkWidget *container);

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
	
	lookatme( vbox );
	//create_flight_cell( vbox );
	//create_menu( vbox );
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

void build_airport_manager(GtkWidget *container)
{
	GtkWidget *vbox;
}

GtkTreeStore *new_store_from_stmt(DBStatement *stmt)
{
	GType *column_types;
	GtkListStore *store;
	int ncolumns, n;
	
	ncolumns = db_column_count(stmt);
	
	column_types = malloc(sizeof(GType) * ncolumns);
	for (n=0; n < ncolumns; n++)
	{
		column_types[n] = G_TYPE_STRING;
	}

	store = gtk_list_store_newv(ncolumns, column_types);
	free(column_types);

	return GTK_TREE_MODEL(store);
}

void populate_store_from_stmt(GtkTreeStore *store, DBStatement *stmt)
{
	int result_code, row, i, ncolumns;
	GtkTreeIter iter;
	const unsigned char *text;

	ncolumns = db_column_count(stmt);

	while ((result_code = db_step(stmt)) == DB_ROW)
	{
		gtk_list_store_append(store, &iter);
		for(i = 0; i < ncolumns; i++)
		{
			text = db_column_text(stmt, i);
			gtk_list_store_set(store, &iter, i, text, -1);
		}
	}
}

GtkTreeStore *build_store_from_stmt(DBStatement *stmt)
{
	GtkListStore *store;

	store = new_store_from_stmt(stmt);
	populate_store_from_stmt(store, stmt);

	return GTK_TREE_MODEL(store);
}

GtkTreeStore *build_store_from_sql(DB *db, const char *sql)
{
	GtkListStore *store;
	DBStatement *stmt;
	int ncolumns;
	const char *unused_sql;

	db_prepare(db, sql, &stmt, &unused_sql);
	build_store_from_stmt(stmt);

	db_finalize(stmt);
	return GTK_TREE_MODEL(store);
}

GtkTreeStore *build_store_from_file_sql(const char *filename, const char *sql)
{
	DB *db;
	DBStatement *stmt;
	GtkListStore *store;
	const unsigned char *unused_sql;

	db = db_open(filename);
	db_prepare(db, sql, &stmt, &unused_sql);

	store = build_store_from_stmt(stmt);

	db_finalize(stmt);
	db_close(db);

	return GTK_TREE_MODEL(store);
}

static void add_columns_from_stmt(GtkTreeView *treeview, DBStatement *stmt)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	GtkTreeModel *model = gtk_tree_view_get_model (treeview);
	const char *header;
	int ncolumns, i;

	ncolumns = db_column_count(stmt);
	for (i = 0; i < ncolumns; i++)
	{
		renderer = gtk_cell_renderer_text_new();
		header = db_column_name(stmt, i);
		column = gtk_tree_view_column_new_with_attributes (header,
				renderer,
				"text",
				i,
				NULL);
		gtk_tree_view_column_set_sort_column_id (column, i);
		gtk_tree_view_append_column (treeview, column);
	}
}


GtkTreeView *build_view_from_stmt(stmt)
{
}

static void lookatme(GtkWidget *container)
{
	DB *db;
	DBStatement *stmt;
	const char *sql = "select subject as Subject, body as Body from memos;";
	GtkTreeStore *store;
	GtkTreeView *view;
	GtkWidget *sw;
	
	db = db_open("test.db");
	stmt = db_prep(db, sql);
	store = build_store_from_stmt(stmt);

	view = gtk_tree_view_new_with_model (store);
	gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (view), TRUE);
	gtk_tree_view_set_search_column (GTK_TREE_VIEW (view), 0);
	
	g_object_unref (store);
	sw = gtk_scrolled_window_new (NULL, NULL);
	gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
			GTK_SHADOW_ETCHED_IN);
	gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
			GTK_POLICY_NEVER,
			GTK_POLICY_AUTOMATIC);
	gtk_container_add (GTK_CONTAINER (sw), view);

	add_columns_from_stmt(GTK_TREE_VIEW (view), stmt);

	gtk_container_add(GTK_CONTAINER(container), sw);
	gtk_widget_show_all(sw);

	db_finalize(stmt);
	db_close(db);
}
