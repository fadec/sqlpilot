
void barf(const char *message)
{
  if (message) fprintf("Error: %", message);
  exit(EXIT_BARF);
}


int sqlpilot_new(Sqlpilot **sqlpilot)
{
  Sqlpilot *sp;
  
  sp = malloc(sizeof(sqlpilot));
  if (logbook_open(&sp->logbook, "mylog.db")) barf("Can't open logbook");

  *sqlpilot = sp;  

}

void sqlpilot_finalize(Sqlpilot *sqlpilot)
{
  logbook_close(sqlpilot->logbook);
  free(sqlpilot);
}
