/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.                                       */
/*                                                                      */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* Sqlpilot is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/


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

static long store_populate_from_stmt(GtkListStore *store, DBStatement *stmt)
{
  int result_code, i, ncolumns;
  long nrows=0;
  GtkTreeIter iter;
  const unsigned char *text;
  
  ncolumns = db_column_count(stmt);
  
  db_reset(stmt);
  
  while ((result_code = db_step(stmt)) == DB_ROW) {
    gtk_list_store_append(store, &iter);
    for(i = 0; i < ncolumns; i++) {
      text = db_column_text(stmt, i);
      gtk_list_store_set(store, &iter, i, text, -1);
    }
    nrows++;
    while (!(nrows % 256) && gtk_events_pending()) {
      gtk_main_iteration();
    }
    gtk_main_iteration_do(FALSE);
  }
  
  db_reset(stmt);
  return nrows;
}

long store_repopulate_from_stmt(GtkListStore *store, DBStatement *stmt)
{
  gtk_list_store_clear(store);
  return store_populate_from_stmt(store, stmt);
}

int store_update_row(GtkListStore *store, GtkTreeIter *iter, DBStatement *stmt)
{
  int result_code, i, ncolumns;
  const unsigned char *text;

  ncolumns = db_column_count(stmt); // should also check list_store column width when not lazy

  if ((result_code = db_step(stmt)) == DB_ROW) {
    for(i = 0; i < ncolumns; i++) {
      text = db_column_text(stmt, i);
      gtk_list_store_set(store, iter, i, text, -1);
    }
  }

  return result_code; // or whatever
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
      if (header[0] != '_') {
	renderer = gtk_cell_renderer_text_new();
	column = gtk_tree_view_column_new_with_attributes (header, renderer, "text", i, NULL);
	gtk_tree_view_column_set_sort_column_id (column, i);
	gtk_tree_view_column_set_reorderable(column, TRUE);
	gtk_tree_view_column_set_resizable(column, TRUE);
	gtk_tree_view_append_column (treeview, column);
      }
    }
}

void store_build_query_stmt_widget(DBStatement *stmt, GtkWidget **ret_view, GtkTreeModel **ret_store)
{
  GtkListStore *store;
  GtkWidget *view;
	
  store = store_new_from_stmt(stmt);

  view = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));
  gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (view), TRUE);
  gtk_tree_view_set_search_column (GTK_TREE_VIEW (view), 0);
	
  g_object_unref (store);

  store_add_columns_from_stmt(GTK_TREE_VIEW (view), stmt);

  *ret_view = view;
  *ret_store = GTK_TREE_MODEL(store);
}
