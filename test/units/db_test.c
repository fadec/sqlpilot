/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.				        */
/* 								        */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.				        */
/* 								        */
/* Sqlpilot is distributed in the hope that it will be useful,	        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.			        */
/* 								        */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#include <test.h>
#include <stdio.h>
#include <db/db.h>

void test_db(void)
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
}

void test_db_prepare(void)
{
	DB *db;
	DBStatement *stmt;
	const char *sql_tail;
	db = db_open("test.db");
	db_prepare(db, "select * from memos;", &stmt, &sql_tail);
	assert_equal_str(sql_tail, "");
	db_finalize(stmt);
	db_close(db);
}

int main(int argc, char **argv)
{
	test_init();
	test(test_db);
	test(test_db_prepare);
	test_finalize();
	return 0;
}

