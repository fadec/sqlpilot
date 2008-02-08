
#ifndef _SQLPILOT_H_
#define _SQLPILOT_H_

#define UI_XML_FILE  "data/ui/sqlpilot.xml"
#define DB_FILE      "db/logbook.db"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include "db/db.h"
#include "store.h"

#define EXIT_SUCESS 0
#define EXIT_BARF  1

#define EMPTY_IF_NULL(x) (x ? x : "")

typedef enum {
    SQLPILOT_NO_ERROR = 0,
    SQLPILOT_ERROR_INVALID_URI,
    SQLPILOT_ERROR_SAVE_FAILED,
    SQLPILOT_ERROR_OPEN_FAILED
} SqlpilotError;

#define FLIGHTS_SELECT \
  "select flights.id as rowid, aircraft.ident as aircraft from flights " \
  "left join aircraft on flights.aircraft_id = aircraft.id " \
  "left join airports da on flights.dep_id = da.id " \
  "left join airports aa on flights.arr_id = aa.id;"

#define FLIGHTS_INSERT \
  "insert into flights (aircraft_id, role_id, dep_id, arr_id, date, aout, ain, dur, night, " \
  "inst, siminst, hold, aprch, xc, dland, nland, crew, notes, fltno, sout, sin, sdur, trip) " \
  "values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"


typedef struct Sqlpilot Sqlpilot;
struct Sqlpilot {
  DB *db;
  GtkWidget *window;
  DBStatement *flights_select;
  DBStatement *flights_insert;
  GtkWidget *flights_sw;
  GtkTreeModel *flights_treemodel;
  GtkTreeSelection *flights_selection;
  GtkWidget *flights_treeview;
  GtkWidget *flights_aircraft;
  GtkWidget *flights_date;
  GtkWidget *flights_role;
  GtkWidget *flights_dep;
  GtkWidget *flights_arr;
  GtkWidget *flights_aout;
  GtkWidget *flights_ain;
  GtkWidget *flights_dur;
  GtkWidget *flights_night;
  GtkWidget *flights_inst;
  GtkWidget *flights_siminst;
  GtkWidget *flights_hold;
  GtkWidget *flights_aprch;
  GtkWidget *flights_xc;
  GtkWidget *flights_dland;
  GtkWidget *flights_nland;
  GtkWidget *flights_crew;
  GtkWidget *flights_notes;
  GtkWidget *flights_fltno;
  GtkWidget *flights_sout;
  GtkWidget *flights_sin;
  GtkWidget *flights_sdur;
  GtkWidget *flights_trip;

};

Sqlpilot *sqlpilot_new(void);
void sqlpilot_finalize(Sqlpilot *);

void barf(const char *message);

#endif
