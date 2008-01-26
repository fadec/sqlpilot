
#ifndef _LOGBOOK_H_
#define _LOGBOOK_H_

#include <db/db.h>

typedef struct Logbook Logbook;
struct Logbook
{
  DB *db;
  const char *db_filename;
  GtkWidget *flights_pane;
  GtkWidget *flights_log_sw;
  GtkTreeModel *flights_log_treemodel;
  GtkWidget  *flights_log_treeview;
  GtkWidget *flights_aircraft_entry;
  GtkWidget *flights_role_entry;

};

#define LOGBOOK(x) ((Logbook *)(x))

Logbook *logbook_open(const char *filename);
void logbook_close(Logbook *logbook);

#endif
