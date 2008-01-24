
#include "db/db.h"
#include "logbook.h"

int logbook_open(Logbook **logbook, const char *filename)
{
  Logbook *logb;
  int err;

  logb = malloc(sizeof(Logbook));
  if (err = db_open(&logb->db, filename)) return err;

  *logbook = logb;
  return 0;
}

void logbook_close(Logbook *logbook)
{
  db_close(logbook->db);
  free(logbook);
}
