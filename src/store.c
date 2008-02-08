
#include "sqlpilot.h"
/*
static GtkListStore *store_new_from_stmt(DBStatement *stmt);
static void store_populate_from_stmt(GtkListStore *store, DBStatement *stmt);
static GtkListStore *store_from_stmt(DBStatement *stmt);
static GtkWidget *store_view_new_from_stmt(DBStatement *stmt);
static void store_add_columns_from_stmt(GtkTreeView *treeview, DBStatement *stmt);
*/


/*********/
/* Model */
/*********/
static GtkListStore *store_new_from_stmt(DBStatement *stmt)
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

static void store_populate_from_stmt(GtkListStore *store, DBStatement *stmt)
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

static GtkListStore *store_from_stmt(DBStatement *stmt)
{
	GtkListStore *store;

	store = store_new_from_stmt(stmt);
	store_populate_from_stmt(store, stmt);

	return store;
}

/********/
/* View */
/********/
static void store_add_columns_from_stmt(GtkTreeView *treeview, DBStatement *stmt)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  const char *header;
  int ncolumns, i;

  ncolumns = db_column_count(stmt);
  for (i = 0; i < ncolumns; i++)
    {
      header = db_column_name(stmt, i);
      if (strcmp(header, "rowid")) {
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes (header, renderer, "text", i, NULL);
	gtk_tree_view_column_set_sort_column_id (column, i);
	gtk_tree_view_append_column (treeview, column);
      }
    }
}

void store_build_query_stmt_widget(DBStatement *stmt, GtkWidget **ret_view, GtkTreeModel **ret_store)
{
  GtkListStore *store;
  GtkWidget *view;
	
  store = store_from_stmt(stmt);

  view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (view), TRUE);
  gtk_tree_view_set_search_column (GTK_TREE_VIEW (view), 0);
	
  g_object_unref (store);

  store_add_columns_from_stmt(GTK_TREE_VIEW (view), stmt);

  *ret_view = view;
  *ret_store = GTK_TREE_MODEL(store);
}
