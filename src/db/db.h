#include <glib.h>
#include <sqlite3.h>

#define DB_RESULTS_TYPE_TABLE 0
#define DB_RESULTS_TYPE_LIST 1

#define db_results_row(data) ((unsigned char **)(data));

typedef sqlite3 DB;


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

DB* db_open(char*);
void db_close(DB*);

void db_results_free(DBResults *);
DBResults *db_get_table(DB *db, const char *sql, char **errormsg);
char *db_results_column_name(DBResults *results, int column);
char *db_results_table_lookup(DBResults*, int row, int column);



const char **db_get_one(DB *db, const char *sql);
