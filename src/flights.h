#ifndef FLIGHTS_H
#define FLIGHTS_H

#include "sqlpilot.h"
/* Column numbers for treemodel and select statements */
enum {
  FLIGHTS_COL_ID = COL_ID,
  FLIGHTS_COL_DATE,
  FLIGHTS_COL_LEG,
  FLIGHTS_COL_AIRCRAFT,
  FLIGHTS_COL_ROLE,
  FLIGHTS_COL_DEP,
  FLIGHTS_COL_ARR,
  FLIGHTS_COL_AOUT,
  FLIGHTS_COL_AOUTUTC,
  FLIGHTS_COL_AIN,
  FLIGHTS_COL_AINUTC,
  FLIGHTS_COL_DUR,
  FLIGHTS_COL_NIGHT,
  FLIGHTS_COL_INST,
  FLIGHTS_COL_SIMINST,
  FLIGHTS_COL_HOLD,
  FLIGHTS_COL_APRCH,
  FLIGHTS_COL_APRCHN,
  FLIGHTS_COL_XC,
  FLIGHTS_COL_DIST,
  FLIGHTS_COL_DLAND,
  FLIGHTS_COL_NLAND,
  FLIGHTS_COL_CREW,
  FLIGHTS_COL_CREWN,
  FLIGHTS_COL_NOTES,
  FLIGHTS_COL_NOTESN,
  FLIGHTS_COL_FLTNO,
  FLIGHTS_COL_SOUT,
  FLIGHTS_COL_SOUTUTC,
  FLIGHTS_COL_SIN,
  FLIGHTS_COL_SINUTC,
  FLIGHTS_COL_SDUR,
  FLIGHTS_COL_TRIP,
  FLIGHTS_COL_TRIPDATE,
  FLIGHTS_NUMCOL
};

/* All columns must be selected for use during update queries */
/* Column names preceded with a _ are get hidden in the treeview */
#define FLIGHTS_SELECT							\
  "select flights.id as _id"						\
  ", flights.Date as Date"						\
  ", flights.Leg as Leg"						\
  ", a.ident as Aircraft"						\
  ", r.ident as Role"							\
  ", dep.ident as Dep"							\
  ", arr.ident as Arr"							\
  ", flights.aout as AOut"						\
  ", flights.AOutUTC as AOutUTC"					\
  ", flights.ain as AIn"						\
  ", flights.AInUTC as AInUTC"						\
  ", m_to_hhmm(flights.dur) as Dur"					\
  ", m_to_hhmm(flights.night) as Night"					\
  ", m_to_hhmm(flights.inst) as Inst"					\
  ", m_to_hhmm(flights.siminst) as SimInst"				\
  ", bool(flights.hold) as Hold"					\
  ", flights.aprch as _Aprch"						\
  ", linecount(flights.aprch) as Aprch"					\
  ", bool(flights.xc) as XC"						\
  ", round(dist_nm(dep.lat, dep.lon, arr.lat, arr.lon)) as Dist"	\
  ", flights.dland as DLand"						\
  ", flights.nland as NLand"						\
  ", flights.crew as _Crew"						\
  ", linecount(flights.crew) as Crw"					\
  ", flights.notes as _Notes"						\
  ", linecount(flights.notes) as Nts"					\
  ", flights.fltno as FltNo"						\
  ", flights.sout as SOut"						\
  ", flights.SOutUTC as SOutUTC"					\
  ", flights.sin as SIn"						\
  ", flights.SInUTC as SInUTC"						\
  ", m_to_hhmm(flights.sdur) as SDur"					\
  ", flights.trip as Trip"						\
  ", flights.TripDate as TripDate"					\
  " from flights"							\
  " left join aircraft a on flights.aircraft_id = a.id"			\
  " left join roles r on flights.role_id = r.id"			\
  " left join airports dep on flights.dep_id = dep.id"			\
  " left join airports arr on flights.arr_id = arr.id"

#define FLIGHTS_ORDER \
  " order by Date, Leg ASC"

#define FLIGHTS_WHERE_ID \
  " where flights.id = ?;"

/* Insert and update bindings */
enum {
  FLIGHTS_WRITE_AIRCRAFT = 1,
  FLIGHTS_WRITE_ROLE,
  FLIGHTS_WRITE_DEP,
  FLIGHTS_WRITE_ARR,
  FLIGHTS_WRITE_DATE,
  FLIGHTS_WRITE_LEG,
  FLIGHTS_WRITE_AOUT,
  FLIGHTS_WRITE_AOUTUTC,
  FLIGHTS_WRITE_AIN,
  FLIGHTS_WRITE_AINUTC,
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
  FLIGHTS_WRITE_SOUTUTC,
  FLIGHTS_WRITE_SIN,
  FLIGHTS_WRITE_SINUTC,
  FLIGHTS_WRITE_SDUR,
  FLIGHTS_WRITE_TRIP,
  FLIGHTS_WRITE_TRIPDATE,
  FLIGHTS_WRITE_ID
};


#define FLIGHTS_INSERT							\
  "insert into flights ("						\
  "aircraft_id"								\
  ",role_id"								\
  ", dep_id"								\
  ", arr_id"								\
  ", Date"								\
  ", Leg"								\
  ", aout"								\
  ", AOutUTC"								\
  ", ain"								\
  ", AInUTC"								\
  ", dur"								\
  ", night"								\
  ", inst"								\
  ", siminst"								\
  ", hold"								\
  ", aprch"								\
  ", xc"								\
  ", dland"								\
  ", nland"								\
  ", crew"								\
  ", notes"								\
  ", fltno"								\
  ", sout"								\
  ", SOutUTC"								\
  ", sin"								\
  ", SInUTC"								\
  ", sdur"								\
  ", Trip"								\
  ", TripDate)"								\
  " values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, hhmm_to_m(?), hhmm_to_m(?), hhmm_to_m(?), hhmm_to_m(?), ?, ?, ?, " \
  "?, ?, ?, ?, ?, ?, ?, ?, ?, hhmm_to_m(?), ?, ?);"

#define FLIGHTS_UPDATE				\
  "update flights set"				\
  " aircraft_id = ?"				\
  ", role_id = ?"				\
  ", dep_id = ?"				\
  ", arr_id = ?"				\
  ", Date = ?"					\
  ", Leg = ?"					\
  ", aout = ?"					\
  ", AOutUTC = ?"				\
  ", ain = ?"					\
  ", AInUTC = ?"				\
  ", dur = hhmm_to_m(?)"			\
  ", night = hhmm_to_m(?)"			\
  ", inst = hhmm_to_m(?)"			\
  ", siminst = hhmm_to_m(?)"			\
  ", hold = ?"					\
  ", aprch = ?"					\
  ", xc = ?"					\
  ", dland = ?"					\
  ", nland = ?"					\
  ", crew = ?"					\
  ", notes = ?"					\
  ", fltno = ?"					\
  ", sout = ?"					\
  ", SOutUTC = ?"				\
  ", sin = ?"					\
  ", SInUTC = ?"				\
  ", sdur = hhmm_to_m(?)"			\
  ", Trip = ?"					\
  ", TripDate = ?"				\
  " where id = ?;"

#define FLIGHTS_DELETE				\
  "delete from flights where id = ?;"

int flights_selection_show(GtkTreeSelection *selection, char *show, size_t size);
int flights_can_delete(GtkTreeSelection *selection);
void flights_after_change(Sqlpilot *data);
DBint64 flights_write_entries(const gchar *id, Sqlpilot *);

void entry_format_date_on_focus_out(GtkEntry *entry);
void entry_format_time_of_day(GtkEntry *entry, const char *local_tz, const char *to_tz, const char *date);
void entry_format_time_on_focus_out(GtkEntry *entry, char separator);
void entry_format_time_on_focus_in(GtkEntry *entry);
long elapsed_seconds(const char *date, const char *t1, const char *tz1, const char *t2, const char *tz2);
void reconcile_time_entries(Sqlpilot *logb,
			    GtkEntry *changed,
			    GtkEntry *start,
			    GtkEntry *end,
			    GtkEntry *elapsed);
void flights_load_selection(Sqlpilot *);
void flights_refresh(Sqlpilot *sqlpilot);

#endif
