
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

/* Column numbers for treemodel and select statements */
enum {
  FLIGHTS_COL_ID = 0,
  FLIGHTS_COL_DATE,
  FLIGHTS_COL_AIRCRAFT,
  FLIGHTS_COL_ROLE,
  FLIGHTS_COL_DEP,
  FLIGHTS_COL_ARR,
  FLIGHTS_COL_AOUT,
  FLIGHTS_COL_AIN,
  FLIGHTS_COL_DUR,
  FLIGHTS_COL_NIGHT,
  FLIGHTS_COL_INST,
  FLIGHTS_COL_SIMINST,
  FLIGHTS_COL_HOLD,
  FLIGHTS_COL_APRCH,
  FLIGHTS_COL_XC,
  FLIGHTS_COL_DLAND,
  FLIGHTS_COL_NLAND,
  FLIGHTS_COL_CREW,
  FLIGHTS_COL_NOTES,
  FLIGHTS_COL_FLTNO,
  FLIGHTS_COL_SOUT,
  FLIGHTS_COL_SIN,
  FLIGHTS_COL_SDUR,
  FLIGHTS_COL_TRIP,
  FLIGHTS_NUMCOL
};

#define FLIGHTS_SELECT					\
  "select f.id as rowid"				\
  ", f.date as Date"					\
  ", a.ident as Aircraft"				\
  ", r.ident as Role"					\
  ", dep.ident as Dep"					\
  ", arr.ident as Arr"					\
  ", f.aout as AOut"					\
  ", f.ain as AIn"					\
  ", f.dur as Dur"					\
  ", f.night as Night"					\
  ", f.inst as Inst"					\
  ", f.siminst as SimInst"				\
  ", f.hold as Hold"					\
  ", f.aprch as Aprch"					\
  ", f.xc as XC"					\
  ", f.dland as DLand"					\
  ", f.nland as NLand"					\
  ", f.crew as Crew"					\
  ", f.notes as Notes"					\
  ", f.fltno as FltNo"					\
  ", f.sout as SOut"					\
  ", f.sin as SIn"					\
  ", f.sdur as SDur"					\
  ", f.trip as Trip"					\
  " from flights f"					\
  " left join aircraft a on f.aircraft_id = a.id"	\
  " left join roles r on f.role_id = r.id"		\
  " left join airports dep on f.dep_id = dep.id"	\
  " left join airports arr on f.arr_id = arr.id;"

/* Insert and update bindings */
enum {
  FLIGHTS_WRITE_AIRCRAFT = 1,
  FLIGHTS_WRITE_ROLE,
  FLIGHTS_WRITE_DEP,
  FLIGHTS_WRITE_ARR,
  FLIGHTS_WRITE_DATE,
  FLIGHTS_WRITE_AOUT,
  FLIGHTS_WRITE_AIN,
  FLIGHTS_WRITE_DUR,
  FLIGHTS_WRITE_NIGHT,
  FLIGHTS_WRITE_INST,
  FLIGHTS_WRITE_SIMINST,
  FLIGHTS_WRITE_HOLD,
  FLIGHTS_WRITE_APRCH,
  FLIGHTS_WRITE_XC,
  FLIGHTS_WRITE_DLAND,
  FLIGHTS_WRITE_NLAND,
  FLIGHTS_WRITE_CREW,
  FLIGHTS_WRITE_NOTES,
  FLIGHTS_WRITE_FLTNO,
  FLIGHTS_WRITE_SOUT,
  FLIGHTS_WRITE_SIN,
  FLIGHTS_WRITE_SDUR,
  FLIGHTS_WRITE_TRIP,
  FLIGHTS_WRITE_ID
};


#define FLIGHTS_INSERT							\
  "insert into flights (aircraft_id, role_id, dep_id, arr_id, date, aout, ain, dur, night, " \
  "inst, siminst, hold, aprch, xc, dland, nland, crew, notes, fltno, sout, sin, sdur, trip) " \
  "values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"

#define FLIGHTS_UPDATE \
  "update flights set aircraft_id = ?, role_id = ?, dep_id = ?"	  \
  ", arr_id = ?, date = ?, aout = ?, ain = ?, dur = ?, night = ?" \
  ", inst = ?, siminst = ?, hold = ?, aprch = ?, xc = ?"	  \
  ", dland = ?, nland = ?, crew = ?, notes = ?, fltno = ?"	  \
  ", sout = ?, sin = ?, sdur = ?, trip = ?"			  \
  " where id = ?;"

enum {
  FLIGHTS_DELETE_ID = 1
};
#define FLIGHTS_DELETE				\
  "delete from flights where id = ?;"


typedef struct Sqlpilot Sqlpilot;
struct Sqlpilot {
  DB *db;
  GtkWidget *window;
  DBStatement *flights_select;
  DBStatement *flights_insert;
  DBStatement *flights_update;
  DBStatement *flights_delete;
  GType flights_gtypes[FLIGHTS_NUMCOL];
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
