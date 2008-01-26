
#include <gtk/gtk.h>
#include "db/db.h"
#include "sqlpilot.h"

static GtkListStore *new_store_from_stmt(DBStatement *stmt);
static void populate_store_from_stmt(GtkListStore *store, DBStatement *stmt);
static void add_columns_from_stmt(GtkTreeView *treeview, DBStatement *stmt);

/*********/
/* Model */
/*********/
GtkListStore *new_store_from_stmt(DBStatement *stmt)
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

	return store;
}

void populate_store_from_stmt(GtkListStore *store, DBStatement *stmt)
{
	int result_code, i, ncolumns;
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

GtkListStore *build_store_from_stmt(DBStatement *stmt)
{
	GtkListStore *store;

	store = new_store_from_stmt(stmt);
	populate_store_from_stmt(store, stmt);

	return store;
}

/********/
/* View */
/********/
GtkWidget *build_view_from_stmt(DBStatement *stmt)
{
	GtkTreeModel *store;
	GtkWidget *view;
	
	store = GTK_TREE_MODEL(build_store_from_stmt(stmt));

	view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(store));
	gtk_tree_view_set_rules_hint(GTK_TREE_VIEW (view), TRUE);
	gtk_tree_view_set_search_column(GTK_TREE_VIEW (view), 0);
	
	g_object_unref(store);

	add_columns_from_stmt(GTK_TREE_VIEW(view), stmt);

	return view;
}

static void add_columns_from_stmt(GtkTreeView *treeview, DBStatement *stmt)
{
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
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

GtkWidget *build_query_stmt_widget(DBStatement *stmt, GtkWidget **ret_view, GtkTreeModel **ret_store)
{
  GtkListStore *store;
  GtkWidget *view;
  GtkWidget *sw;
	
  store = build_store_from_stmt(stmt);

  view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));
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

  gtk_widget_show_all(sw);

  *ret_view = view;
  *ret_store = GTK_TREE_MODEL(store);
  return sw;
}

/* lame
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
end of (useless) bad ideas */
