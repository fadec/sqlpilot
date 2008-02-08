#ifndef _STORE_H_
#define _STORE_H_

#include <gtk/gtk.h>
#include "db/db.h"

void store_build_query_stmt_widget(DBStatement *stmt, GtkWidget **ret_view, GtkTreeModel **ret_store);

#endif
