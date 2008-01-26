
#include "sqlpilot.h"

Logbook *logbook_open(const char *filename)
{
  Logbook *logb;

  logb = malloc(sizeof(Logbook));
  logb->db = db_open(filename);
  return logb;
}

void logbook_close(Logbook *logbook)
{
  db_close(logbook->db);
  free(logbook);
}

