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
	union
	{
		GSList *list;
		char **table;
	}
};

DB* db_open(char*);
void db_close(DB*);

DBResults *db_get_many(DB *db, const char *sql);
void db_free_results(DBResults *);

const char **db_get_one(DB *db, const char *sql);
