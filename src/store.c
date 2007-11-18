
#include <gtk/gtk.h>
#include <db/db.h>
#include <store.h>

GtkTreeStore *new_store_from_stmt(DBStatement *stmt)
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

	return GTK_TREE_MODEL(store);
}

void populate_store_from_stmt(GtkTreeStore *store, DBStatement *stmt)
{
	int result_code, row, i, ncolumns;
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

GtkTreeStore *build_store_from_stmt(DBStatement *stmt)
{
	GtkListStore *store;

	store = new_store_from_stmt(stmt);
	populate_store_from_stmt(store, stmt);

	return GTK_TREE_MODEL(store);
}

/* Bad ideas
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
