#include <test.h>
#include <stdio.h>
#include <db/db.h>
#include <ui/interface.h>

void test_create_list_store_from_sql()
{
	DB *db;
	GtkListStore *store;
	db = db_open("test.db");
	store = create_list_store_from_sql(db, "select * from memos");
}

int main(int argc, char **argv)
{
	test_init();
	test(test_create_list_store_from_sql);
	test_finalize();
	return 0;
}

