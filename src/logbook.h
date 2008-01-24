#include <db/db.h>

typedef struct Logbook Logbook;
struct Logbook
{
	DB *db;
	const char *db_filename;
};

int logbook_open(Logbook **logbook, const char *filename);
void logbook_close(Logbook *logbook);


