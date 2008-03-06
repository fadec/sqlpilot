#include <test.h>
#include <stdio.h>
#include <db/db.h>
#include <ui/interface.h>

void test_build_store_from_sql()
{
	DB *db;
	GtkTreeStore *store;
	db = db_open("test.db");
	store = build_store_from_sql(db, "select * from memos");
	db_close(db);
}

void test_new_store_from_stmt()
{
	DB *db;
	DBStatement *stmt;
	GtkTreeStore *store;
	const char *unused_sql;

	db = db_open("test.db");
	db_prepare(db, "select * from memos", &stmt, &unused_sql);
	
	store = new_store_from_stmt(stmt);
	db_close(db);
}

void test_populate_store_from_stmt()
{
	DB *db;
	DBStatement *stmt;
	GtkTreeStore *store;
	const char *unused_sql;

	db = db_open("test.db");
	db_prepare(db, "select * from memos", &stmt, &unused_sql);
	
	store = new_store_from_stmt(stmt);
	populate_store_from_stmt(store, stmt);
	db_close(db);
}

int main(int argc, char **argv)
{
	test_init();
	gtk_init( &argc, &argv );
	//test(test_build_store_from_sql);
	//test(test_new_store_from_stmt);
	test(test_populate_store_from_stmt);
	test_finalize();
	return 0;
}

