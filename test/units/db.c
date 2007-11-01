#include <test.h>
#include <stdio.h>
#include <db/db.h>

void test_db()
{
	DB *mydb;
	DBResults *results;
	char *errormsg;

	mydb = db_open("test.db");
	results = db_get_table(mydb, "select * from memos;", &errormsg);

	assert_equal_str(db_results_table_lookup(results, 0, 0), "Hello");
	assert_equal_str(db_results_table_lookup(results, 0, 1), "World");
	assert_equal_str(db_results_table_lookup(results, 1, 0), "Yes");
	assert_equal_str(db_results_table_lookup(results, 1, 1), "No");

	assert_equal_str(db_results_column_name(results, 0), "subject");
	assert_equal_str(db_results_column_name(results, 1), "body");

	db_results_free(results);
	db_close(mydb);
	return 0;
}

int main(int argc, char **argv)
{
	test_init();
	test(&test_db);
	test_finalize();
	return 0;
}

