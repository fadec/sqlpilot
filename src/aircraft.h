#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "sqlpilot.h"

enum {
  AIRCRAFT_COL_ID = COL_ID,
  AIRCRAFT_COL_IDENT,
  AIRCRAFT_COL_TYPE,
  AIRCRAFT_COL_FLEETNO,
  AIRCRAFT_COL_NOTES,
  AIRCRAFT_COL_FLIGHTS,
  AIRCRAFT_COL_TIME
};

#define AIRCRAFT_SELECT						\
  "select aircraft.id as _id"					\
  ", aircraft.ident as Ident"					\
  ", types.ident as Type"					\
  ", aircraft.fleetno as FleetNo"				\
  ", aircraft.notes as _Notes"					\
  ", count(flights.id) as Flights"				\
  ", m_to_hhmm(sum(flights.dur)) as Time"			\
  " from aircraft"						\
  " left join types on aircraft.type_id = types.id"		\
  " left join flights on flights.aircraft_id = aircraft.id"	\

#define AIRCRAFT_GROUP_BY \
  " group by aircraft.id order by flights desc"

#define AIRCRAFT_WHERE_ID \
  " where aircraft.id = ?"

enum {
  AIRCRAFT_WRITE_IDENT = 1,
  AIRCRAFT_WRITE_TYPE,
  AIRCRAFT_WRITE_FLEETNO,
  AIRCRAFT_WRITE_NOTES,
  AIRCRAFT_WRITE_ID
};

#define AIRCRAFT_INSERT \
  "insert into aircraft (ident, type_id, fleetno, notes) values (?, ?, ?, ?);"

#define AIRCRAFT_UPDATE \
  "update aircraft set ident = ?, type_id = ?, fleetno = ?, notes = ? where id = ?;"

#define AIRCRAFT_DELETE	\
  "delete from aircraft where id = ?;"

int aircraft_selection_show(GtkTreeSelection *selection, char *show, size_t size);
void aircraft_after_change(Sqlpilot *sqlpilot);
int aircraft_can_delete(GtkTreeSelection *selection);
DBint64 aircraft_write_entries(const gchar *id, Sqlpilot *sqlpilot);
void aircraft_load_selection(Sqlpilot *logb);

#endif
