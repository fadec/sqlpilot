#include <test.h>
#include <stdio.h>
#include <db/db.h>

void test_db()
{
	DB *mydb;
	DBResults *results;
	GSList *row;
	int code;

	mydb = db_open("test.db");
	results = db_get_many(mydb, "select * from memos;");

	row = results->list;
	assert_equal_str(((unsigned char **)(row->data))[0], "Hello");

	char **table;
	int nrow;
	int ncolumn;
	char *errormsg;
	
	if ((code = sqlite3_get_table(mydb, "select * from memos", &table, &nrow, &ncolumn, &errormsg)) == SQLITE_OK)
	{
		printf("%s\n", table[0]);
	}
	assert_equal_
	
	db_close(mydb);
	return 0;
}

int main(int argc, char **argv)
{
	test_init();
	test(&test_db);
	return 0;
}

