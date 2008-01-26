#include "sqlpilot.h"

void barf(const char *message)
{
  if (message) fprintf(stderr, "Error: %s", message);
  exit(EXIT_BARF);
}

void sqlpilot_init(void)
{
  sqlpilot = malloc(sizeof(Sqlpilot));
  if (!(sqlpilot->logbook = logbook_open("db/logbook.db"))) {
    barf("Can't open logbook");
  }
}

void sqlpilot_finalize(void)
{
  logbook_close(sqlpilot->logbook);
  free(sqlpilot);
}
