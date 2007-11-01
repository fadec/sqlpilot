#include <sqlite3.h>
#include <db/db.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

DB* db_open(char* filename)
{
	DB* db;
	int rc;

	rc = sqlite3_open(filename, &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		exit(1);
	}
	return db;
}

void db_close(DB* db)
{
	sqlite3_close( (sqlite3*) db );
}

DBResults *db_get_table(DB *db, const char *sql, char **errormsg)
{
	char **data;
	int nrow;
	int ncolumn;
	DBResults *results;
	int code;
	
	if ((code = sqlite3_get_table(db, sql, &data, &nrow, &ncolumn, errormsg)) != SQLITE_OK)
	{
		return NULL;
	}

	results = malloc(sizeof(DBResults));
	results->column_names = data;
	results->table = data + ncolumn;
	results->column_count = ncolumn;
	results->row_count = ncolumn;
	return results;
}

void db_results_free(DBResults *results)
{
	sqlite3_free_table(results->column_names); // column_names are actually the beginning of the sqlite3 allocated structure
	results->table = NULL;
	free(results);
	results = NULL;
}

char *db_results_table_lookup(DBResults *results, int row, int column)
{
	return results->table[row * results->column_count + column];
}

char *db_results_column_name(DBResults *results, int column)
{
	return results->column_names[column];
}


// incomplete
DBResults *db_get_list(DB *db, const char *sql, char **errormsg)
{
	sqlite3_stmt *stmt;
	const char *unused_sql;
	int result_code;
	int column_count;
	int i;
	const unsigned char *text;
	unsigned char **row;
	GSList *list = NULL;
	DBResults *results;

	sqlite3_prepare_v2((sqlite3*)db, sql, strlen(sql), &stmt, &unused_sql);
	column_count = sqlite3_column_count(stmt);

	while ((result_code = sqlite3_step(stmt)) == SQLITE_ROW)
	{
		row = malloc(sizeof(unsigned char*) * column_count);
		//printf("%d\n", row);
		for(i = 0; i < column_count; i++)
		{
			text = sqlite3_column_text(stmt, i);
			row[i] = malloc(sqlite3_column_bytes(stmt, i) + 1);
			strcpy( ((char **)row)[i], (char *)text);
		}
		list = g_slist_prepend(list, row);
		//printf("%s\n", ((unsigned char **)list->data)[0]);
	}

	sqlite3_finalize(stmt);
	
	results = malloc(sizeof(DBResults));
	results->column_count = column_count;
	results->row_count = g_slist_length(list);
	// I think reverse is in place - no new list is allocated??
	list = g_slist_reverse(list);
	results->list = list;
	
	return results;
}

const char **db_get_one(DB *db, const char *sql);


