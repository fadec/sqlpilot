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

#ifndef _STORE_H_
#define _STORE_H_

#include <gtk/gtk.h>
#include "db/db.h"

typedef enum {
  COLUMN_SORT_STR,
  COLUMN_SORT_NUM,
  COLUMN_SORT_FLOAT
} ColumnSort;

typedef struct {
  char *name;
  ColumnSort sort;
  gboolean visible;
} ColumnPref;


void store_build_query_stmt_widget(DBStatement *stmt, GHashTable *column_prefs, GtkWidget **ret_view, GtkTreeModel **ret_store);
int store_update_row(GtkListStore *store, GtkTreeIter *iter, DBStatement *stmt);
long store_repopulate_from_stmt(GtkListStore *store, DBStatement *stmt);
long store_repopulate_from_stmt_with_progress(GtkListStore *store, DBStatement *stmt, GtkProgressBar *progress);
void store_view_arrange_columns(GtkTreeView *view, int ntitles, const char **titles);
GtkTreeViewColumn *store_view_find_column_by_title(GtkTreeView *view, const char *title);
void store_view_set_column_visible_by_title(GtkTreeView *view, const char *title, gboolean visible);
void store_fwrite_csv(GtkTreeView *, FILE *);
#endif
