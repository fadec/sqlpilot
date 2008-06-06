/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.                                       */
/*                                                                      */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* Sqlpilot is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

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

