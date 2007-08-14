
#include <sqlite3.h>

typedef enum {
	DB_NO_ERROR = 0,
	DB_OPEN_FAIL
} DBError;

typedef sqlite3 DB;

DB* db_open(char*);
void db_close(DB*);