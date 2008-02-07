#ifndef _STORE_H_
#define _STORE_H_

#include <gtk/gtk.h>
#include "db/db.h"

#define EDITOR_TEXT
#define EDITOR_MTEXT
#define EDITOR_BOOL

struct ColumnSettings {
  char name[32];
  int editor;
};

GtkListStore *build_store_from_stmt(DBStatement *stmt);

GtkWidget *build_view_from_stmt(DBStatement *stmt);

GtkWidget *build_query_stmt_widget(DBStatement *stmt, GtkWidget **ret_view, GtkTreeModel **ret_store);

GtkWidget *build_edit_widget(DBStatement *stmt);
GtkWidget *build_db_ui(DBStatement *stmt);

#endif
