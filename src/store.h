#include <gtk/gtk.h>
#include "db/db.h"

GtkListStore *build_store_from_stmt(DBStatement *stmt);

GtkWidget *build_view_from_stmt(DBStatement *stmt);

GtkWidget *build_query_stmt_widget(DBStatement *stmt, GtkWidget **ret_view, GtkTreeModel **ret_store);

