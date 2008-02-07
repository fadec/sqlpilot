
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


// new

static GtkWidget *add_text_field(GtkContainer *container, const char *label_text)
{
  GtkWidget *entry;
  GtkWidget *label;
  GtkWidget *hbox;
  entry = gtk_entry_new();
  label = gtk_label_new(label_text);

  hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_end(GTK_BOX(hbox), entry, FALSE, TRUE, 0);
  gtk_box_pack_end(GTK_BOX(hbox), label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(container), hbox, FALSE, FALSE, 0);
  return entry;
}

static GtkWidget *add_mtext_field(GtkContainer *container, const char *label_text)
{
  GtkWidget *vbox, *label;
  GtkWidget *view;
  GtkTextBuffer *buffer;

  label = gtk_label_new(label_text);
  view = gtk_text_view_new ();
  buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));

  vbox = gtk_vbox_new(FALSE, 0);
  gtk_box_pack_end(GTK_BOX(vbox), view, FALSE, TRUE, 0);
  gtk_box_pack_end(GTK_BOX(vbox), label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(container), vbox, FALSE, FALSE, 0);


  //gtk_text_buffer_set_text (buffer, "Hello, this is some text", -1);
  return view;
}

GtkWidget *build_edit_widget(DBStatement *stmt)
{
  GtkWidget *vbox;
  GtkWidget *buttons, *update, *cancel, *new, *insert;
  GtkWidget *entry;
  int ncols, i;

  ncols = db_column_count(stmt);
  vbox = gtk_vbox_new(FALSE, 0);

  for(i=0; i<ncols; i++) {
    entry = add_text_field(GTK_CONTAINER(vbox), db_column_name(stmt, i));
  }

  /* Buttons */
  buttons = gtk_hbox_new(FALSE, 0);
  update = gtk_button_new_with_label("Update");
  cancel = gtk_button_new_with_label("Cancel");
  insert = gtk_button_new_with_label("Add");
  new = gtk_button_new_with_label("New");

  gtk_box_pack_start(GTK_BOX(buttons), update, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), cancel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), new, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), insert, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), buttons, FALSE, FALSE, 0);


  return vbox;
}

GtkWidget *build_db_ui(DBStatement *stmt)
{
  GtkWidget *outer, *model, *treeview, *sw, *editor;

  outer = gtk_hbox_new(FALSE, 0);
  sw = build_query_stmt_widget(stmt, &treeview, &model);

  gtk_box_pack_end(GTK_BOX(outer), sw, TRUE, TRUE, 0);

  editor = build_edit_widget(stmt);
  gtk_box_pack_start(GTK_BOX(outer), editor, FALSE, TRUE, 0);

  gtk_widget_show_all(outer);

  return outer;

}
