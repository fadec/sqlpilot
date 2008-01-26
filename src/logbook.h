
#ifndef _LOGBOOK_H_
#define _LOGBOOK_H_

#include <db/db.h>

typedef struct Logbook Logbook;
struct Logbook
{
	DB *db;
	const char *db_filename;
};

Logbook *logbook_open(const char *filename);
void logbook_close(Logbook *logbook);

#endif
