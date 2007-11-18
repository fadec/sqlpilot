
GtkTreeStore *new_store_from_stmt(DBStatement *stmt);

void populate_store_from_stmt(GtkTreeStore *store, DBStatement *stmt);

GtkTreeStore *build_store_from_stmt(DBStatement *stmt);
