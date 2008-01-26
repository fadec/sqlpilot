
#include "sqlpilot.h"

Logbook *logbook_open(const char *filename)
{
  Logbook *logb;

  logb = malloc(sizeof(Logbook));
  if (!(logb->db = db_open(filename))) {
    barf("Couldn't open database.");
  }
  logb->db_filename = filename;
  build_flights_pane(logb);

  return logb;
}

void logbook_close(Logbook *logbook)
{
  //g_free(logbook->flights_pane);  // Must free memory for gtk stuff
  db_close(logbook->db);
  free(logbook);
}

