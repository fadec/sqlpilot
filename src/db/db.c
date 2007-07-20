#include <sqlite3.h>
#include <db/db.h>

#include <stdio.h>
#include <gtk/gtk.h>

DB* db_open(char* filename)
{
	DB* db;
	int rc;
	char *errmsg = 0;

	rc = sqlite3_open(filename, &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	return;
}

void db_close(DB* db)
{
	sqlite3_close( (sqlite3*) db );
}

enum
{
	COL_NAME = 0,
	COL_AGE,
	NUM_COLS
} ;

GtkTreeModel* list_from_sql(void) //DB* db, char* sql)
{
	GtkListStore* store;
	GtkTreeIter iter;

	//store = gtk_list_store_newv(2, ...);
	store = gtk_list_store_new (NUM_COLS, G_TYPE_STRING, G_TYPE_UINT);
	
        /* Append a row and fill in some data */
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter,
			COL_NAME, "Heinz El-Mann",
			COL_AGE, 51,
			-1);

	/* append another row and fill in some data */
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter,
			COL_NAME, "Jane Doe",
			COL_AGE, 23,
			-1);

	/* ... and a third row */
	gtk_list_store_append (store, &iter);
	gtk_list_store_set (store, &iter,
			COL_NAME, "Joe Bungop",
			COL_AGE, 91,
			-1);

	return GTK_TREE_MODEL (store);


}

