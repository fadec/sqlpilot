
#ifndef _DB_H_
#define _DB_H_

#include <glib.h>
#include "db/sqlite3.h"

/* Ripped shamefully from sqlite3 docs and s/SQLITE/DB/g, */
#define DB_OK           0   /* Successful result */
#define DB_ERROR        1   /* SQL error or missing database */
#define DB_INTERNAL     2   /* NOT USED. Internal logic error in SQLite */
#define DB_PERM         3   /* Access permission denied */
#define DB_ABORT        4   /* Callback routine requested an abort */
#define DB_BUSY         5   /* The database file is locked */
#define DB_LOCKED       6   /* A table in the database is locked */
#define DB_NOMEM        7   /* A malloc() failed */
#define DB_READONLY     8   /* Attempt to write a readonly database */
#define DB_INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
#define DB_IOERR       10   /* Some kind of disk I/O error occurred */
#define DB_CORRUPT     11   /* The database disk image is malformed */
#define DB_NOTFOUND    12   /* NOT USED. Table or record not found */
#define DB_FULL        13   /* Insertion failed because database is full */
#define DB_CANTOPEN    14   /* Unable to open the database file */
#define DB_PROTOCOL    15   /* NOT USED. Database lock protocol error */
#define DB_EMPTY       16   /* Database is empty */
#define DB_SCHEMA      17   /* The database schema changed */
#define DB_TOOBIG      18   /* String or BLOB exceeds size limit */
#define DB_CONSTRAINT  19   /* Abort due to contraint violation */
#define DB_MISMATCH    20   /* Data type mismatch */
#define DB_MISUSE      21   /* Library used incorrectly */
#define DB_NOLFS       22   /* Uses OS features not supported on host */
#define DB_AUTH        23   /* Authorization denied */
#define DB_FORMAT      24   /* Auxiliary database format error */
#define DB_RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
#define DB_NOTADB      26   /* File opened that is not a database file */
#define DB_ROW         100  /* sqlite3_step() has another row ready */
#define DB_DONE        101  /* sqlite3_step() has finished executing */


#define DB_RESULTS_TYPE_TABLE 0
#define DB_RESULTS_TYPE_LIST 1

typedef sqlite3 DB;
typedef sqlite3_stmt DBStatement;
/* Had trouble with sqlite3_int64 on Ubuntu ??? */
//typedef sqlite3_int64 DBint64;
typedef long long int DBint64;

typedef struct DBResults DBResults;
struct DBResults {
	int type;
	int row_count;
	int column_count;
	char **column_names;
	union
	{
		GSList *list;
		char **table;
	};
};

DB* db_open(const char *filename);
void db_close(DB*);
void db_register_sqlpilot_functions(DB *db);

void db_results_free(DBResults *);
DBResults *db_get_table(DB *db, const char *sql, char **errormsg);
char *db_results_column_name(DBResults *results, int column);
char *db_results_table_lookup(DBResults*, int row, int column);
int db_prepare(DB *db, const char *sql, DBStatement **stmt, const char **sql_tail);
DBStatement *db_prep(DB *db, const char *sql);
int db_clear_bindings(DBStatement *);
int db_reset(DBStatement *stmt);
int db_stp_res_clr(DBStatement *stmt);
int db_bind_text(DBStatement *stmt, int i, const char *text);
int db_bind_int(DBStatement *stmt, int i, int n);
int db_bind_int64(DBStatement *, int, int);
int db_bind_double(DBStatement *, int, double);
int db_bind_null(DBStatement *, int);
int db_column_count(DBStatement *stmt);
int db_finalize(DBStatement *stmt);
int db_step(DBStatement *stmt);
DB *db_handle(DBStatement *);
DBint64 db_last_insert_rowid(DB *);
const unsigned char *db_column_text(DBStatement *stmt, int icolumn);
int db_column_bytes(DBStatement *stmt, int icolumn);
int db_column_int(DBStatement *stmt, int icolumn);
DBint64 db_column_int64(DBStatement *stmt, int i);
const char *db_column_name(DBStatement *stmt, int icolumn);
const char **db_get_one(DB *db, const char *sql);
int db_exec_simple(DB *db, const char *sql);

#endif
