#include <sqlite3.h>
#include <db/db.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>


/* Takes a numeric column representing minutes and converts it to a hh+mm string. */
static void m_to_hhmm_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  char z[30];
  int m, hh, mm;
  //assert(argc == 1);
  switch (sqlite3_value_type(argv[0])) {
  case SQLITE_NULL:
  case SQLITE_TEXT:
  case SQLITE_BLOB: m = 0;
    break;
  case SQLITE_FLOAT:
  case SQLITE_INTEGER: m = sqlite3_value_int(argv[0]);
  }
  hh = m / 60;
  mm = m - (hh * 60);
  snprintf(z, 30, "%02d+%02d", hh, mm);
  sqlite3_result_text(context, z, -1, SQLITE_TRANSIENT);
}

/* Takes time string in hhhhhh+mm format and returns integer minutes */
/* Integers, floats, and string integers */
static void hhmm_to_m_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  int hh=0, mm=0;
  const char *str;
  switch (sqlite3_value_type(argv[0])) {
  case SQLITE_NULL:
  case SQLITE_BLOB:
    break;
  case SQLITE_FLOAT:
  case SQLITE_INTEGER:
    
    break;
  case SQLITE_TEXT:
    str = (char *)sqlite3_value_text(argv[0]);
    sscanf(str, "%d+%d", &hh, &mm);
    sqlite3_result_int(context, hh * 60 + mm);
    break;
  }
}

DB* db_open(const char* filename)
{
	DB* db;
	int rc;

	rc = sqlite3_open(filename, &db);
	if (rc) {
	  sqlite3_close(db);
	  fprintf(stderr, "Unable to open database\n");
	  exit(1);
	}

	/* register custom functions */
	sqlite3_create_function(db, "m_to_hhmm", 1, SQLITE_ANY, 0, m_to_hhmm_func, 0, 0);
	sqlite3_create_function(db, "hhmm_to_m", 1, SQLITE_ANY, 0, hhmm_to_m_func, 0, 0);
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

int db_prepare(DB *db, const char *sql, DBStatement **stmt, const char **sql_tail)
{
	return sqlite3_prepare_v2((sqlite3*)db, sql, strlen(sql), (sqlite3_stmt **)stmt, sql_tail);
}

DBStatement *db_prep(DB *db, const char *sql)
{
	const char *sql_tail;
	sqlite3_stmt *stmt;
	int err;
	if ((err = sqlite3_prepare_v2((sqlite3*)db, sql, strlen(sql), &stmt, &sql_tail)) != SQLITE_OK)
	{
		fprintf(stderr, "Error preparing statement for: %s\n", sql);
		sqlite3_close(db);
		exit(2);
	}
	return stmt;
}

int db_bind_text(DBStatement *stmt, int i, const char *text)
{
  return sqlite3_bind_text(stmt, i, text, strlen(text), SQLITE_TRANSIENT);
}

int db_bind_int(DBStatement *stmt, int i, int n)
{
  return sqlite3_bind_int(stmt, i, n);
}

int db_bind_int64(DBStatement *stmt, int i, int n)
{
  return sqlite3_bind_int64(stmt, i, n);
}

int db_clear_bindings(DBStatement *stmt)
{
  return sqlite3_clear_bindings(stmt);
}

int db_reset(DBStatement *stmt)
{
  return sqlite3_reset(stmt);
}

int db_column_count(DBStatement *stmt)
{
	return sqlite3_column_count((sqlite3_stmt*)stmt);
}

int db_finalize(DBStatement *stmt)
{
	return sqlite3_finalize(stmt);
}

int db_step(DBStatement *stmt)
{
	return sqlite3_step(stmt);
}

DBint64 db_last_insert_rowid(DB *db)
{
  return sqlite3_last_insert_rowid(db);
}

const unsigned char *db_column_text(DBStatement *stmt, int icolumn)
{
	return sqlite3_column_text((sqlite3_stmt *)stmt, icolumn);
}

int db_column_int(DBStatement *stmt, int icolumn)
{
  return sqlite3_column_int(stmt, icolumn);
}

DBint64 db_column_int64(DBStatement *stmt, int i)
{
  return (DBint64) sqlite3_column_int64(stmt, i);
}

int db_column_bytes(DBStatement *stmt, int icolumn)
{
	return sqlite3_column_bytes((sqlite3_stmt *)stmt, icolumn);
}

const char *db_column_name(DBStatement *stmt, int icolumn)
{
	return sqlite3_column_name((sqlite3_stmt *)stmt, icolumn);
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

int db_exec_simple(DB *db, const char *sql)
{
  char *errmsg;
  int rc;
  rc = sqlite3_exec((sqlite3*) db, sql, NULL, NULL, &errmsg);
  if (errmsg) {
    fprintf(stdout, "%s\n", errmsg);
    sqlite3_free(errmsg);
    //    exit(1);
  }
  return rc;
}
