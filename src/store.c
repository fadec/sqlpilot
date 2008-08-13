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

static long store_populate_from_stmt(GtkListStore *store, DBStatement *stmt, GtkProgressBar *progress)
{
  int result_code, i, ncolumns;
  long nrows=0,maxrows=0;
  GtkTreeIter iter;
  const unsigned char *text;
  
  if (progress) {
    gtk_progress_bar_set_fraction(progress, 0.0);
    while ((result_code = db_step(stmt)) == DB_ROW) maxrows++;
  }
  db_reset(stmt);

  ncolumns = db_column_count(stmt);  

  while ((result_code = db_step(stmt)) == DB_ROW) {
    gtk_list_store_append(store, &iter);
    for(i = 0; i < ncolumns; i++) {
      text = db_column_text(stmt, i);
      gtk_list_store_set(store, &iter, i, text, -1);
    }
    if (!(nrows % 128)) {
      while (gtk_events_pending()) gtk_main_iteration();
    }
    gtk_main_iteration_do(FALSE);
    nrows++;
    if (progress) gtk_progress_bar_set_fraction(progress, nrows/(double)maxrows);
  }
  
  db_reset(stmt);
  return nrows;
}

long store_repopulate_from_stmt(GtkListStore *store, DBStatement *stmt)
{
  gtk_list_store_clear(store);
  return store_populate_from_stmt(store, stmt, NULL);
}

long store_repopulate_from_stmt_with_progress(GtkListStore *store, DBStatement *stmt, GtkProgressBar *progress)
{
  gtk_list_store_clear(store);
  return store_populate_from_stmt(store, stmt, progress);
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

/* Parse int compare that filters out non-[0-9] characters before parse */
static gint iter_compare_by_noisy_str_int_column(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer column)
{
  char *a_val, *b_val, *pread, *pwrite;
  gint ret;

  gtk_tree_model_get(model, a, (gint)column, &a_val, -1);
  gtk_tree_model_get(model, b, (gint)column, &b_val, -1);
  
  for (pread = pwrite = a_val; a_val && *pread; pread++) {
    if (isdigit(*pread)) {
      *pwrite = *pread;
      pwrite++;
    }
  }

  for (pread = pwrite = b_val; b_val && *pread; pread++) {
    if (isdigit(*pread)) {
      *pwrite = *pread;
      pwrite++;
    }
  }

  ret = atol(EMPTY_IF_NULL(a_val)) - atol(EMPTY_IF_NULL(b_val));

  g_free(a_val);
  g_free(b_val);
  return ret;
}

static gint iter_compare_by_str_float_column(GtkTreeModel *model, GtkTreeIter *a, GtkTreeIter *b, gpointer column)
{
  char *a_val, *b_val;
  gint ret;

  gtk_tree_model_get(model, a, (gint)column, &a_val, -1);
  gtk_tree_model_get(model, b, (gint)column, &b_val, -1);
  
  ret = atof(EMPTY_IF_NULL(a_val)) - atof(EMPTY_IF_NULL(b_val));

  g_free(a_val);
  g_free(b_val);
  return ret;
}

static void store_add_columns_from_stmt(GtkTreeView *treeview, DBStatement *stmt)
{
  GtkCellRenderer *renderer;
  GtkTreeViewColumn *column;
  const char *column_name;
  char meta[32], header[256];
  int ncolumns, i;
  /* Special colums */
  /* _ makes hidden column */
  /* n noisy string numeric sort */
  /* f parse string as float sort */

  ncolumns = db_column_count(stmt);
  for (i = 0; i < ncolumns; i++) {
    column_name = db_column_name(stmt, i);
    meta[0] = header[0] = '\0';
    if (strchr(column_name, '\\')) {
      sscanf(column_name, "%31[^\\]\\%255s", meta, header);
    } else {
      sscanf(column_name, "%255s", header);
    }
    if (!strchr(meta, '_')) {
      renderer = gtk_cell_renderer_text_new();
      column = gtk_tree_view_column_new_with_attributes(header, renderer, "text", i, NULL);
      gtk_tree_view_column_set_sort_column_id(column, i);
      gtk_tree_view_column_set_reorderable(column, TRUE);
      gtk_tree_view_column_set_resizable(column, TRUE);
      gtk_tree_view_append_column(treeview, column);
      if (strchr(meta, 'n')) {
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(gtk_tree_view_get_model(treeview)), i, iter_compare_by_noisy_str_int_column, (gpointer)i, NULL);
      } else if (strchr(meta, 'f')) {
	gtk_tree_sortable_set_sort_func(GTK_TREE_SORTABLE(gtk_tree_view_get_model(treeview)), i, iter_compare_by_str_float_column, (gpointer)i, NULL);
      }
    }
  }
}

GtkTreeViewColumn *store_view_find_column_by_title(GtkTreeView *view, const char *title)
{
  int i;
  GtkTreeViewColumn *col;
  for (i = 0; (col = gtk_tree_view_get_column(view, i)); i++) {
    if (!strcmp(gtk_tree_view_column_get_title(col), title)) return col;
  }
  return NULL;
}

void store_view_set_column_visible_by_title(GtkTreeView *view, const char *title, gboolean visible)
{
  gtk_tree_view_column_set_visible(store_view_find_column_by_title(view, title), visible);
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

/* int store_build_from_csv_fread(FILE *in, GtkWidget **ret_view, GtkTreeModel **ret_store) */
/* { */
  
