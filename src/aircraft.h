/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.                                       */
/*                                                                      */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* Sqlpilot is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include "sqlpilot.h"

enum {
  AIRCRAFT_COL_ID = COL_ID,
  AIRCRAFT_COL_TAIL,
  AIRCRAFT_COL_MODEL,
  AIRCRAFT_COL_FLEETNO,
  AIRCRAFT_COL_NOTES,
  AIRCRAFT_COL_FLIGHTS,
  AIRCRAFT_COL_TIME
};

#define AIRCRAFT_SELECT							\
  "select aircraft.id as id"						\
  ", aircraft.tail as Tail"						\
  ", models.ident as Model"						\
  ", aircraft.fleetno as FleetNo"					\
  ", aircraft.notes as Notes"						\
  ", count(flights.id) as Flights"					\
  ", m_to_hhmm(sum(flights.dur)) as Time"				\
  " from aircraft"							\
  " left join models on aircraft.model_id = models.id"			\
  " left join flights on flights.aircraft_id = aircraft.id"		\
  
#define AIRCRAFT_GROUP_BY \
  " group by aircraft.id order by count(flights.id) desc"

#define AIRCRAFT_WHERE_ID \
  " where aircraft.id = ?"

enum {
  AIRCRAFT_WRITE_TAIL = 1,
  AIRCRAFT_WRITE_MODEL,
  AIRCRAFT_WRITE_FLEETNO,
  AIRCRAFT_WRITE_NOTES,
  AIRCRAFT_WRITE_ID
};

#define AIRCRAFT_INSERT \
  "insert into aircraft (tail, model_id, fleetno, notes) values (?, ?, ?, ?);"

#define AIRCRAFT_UPDATE \
  "update aircraft set tail = ?, model_id = ?, fleetno = ?, notes = ? where id = ?;"

#define AIRCRAFT_DELETE	\
  "delete from aircraft where id = ?;"

#define AIRCRAFT_COUNT_FLIGHTS \
  "select count(id) from Flights where Flights.aircraft_id = ?;"

int aircraft_selection_show(GtkTreeSelection *selection, char *show, size_t size);
void aircraft_after_change(Logbook *logbook);
int aircraft_can_delete(GtkTreeSelection *selection);
DBint64 aircraft_write_entries(const gchar *id, Logbook *logbook);
void aircraft_load_selection(Logbook *logb);
int aircraft_count_flights(Logbook *, DBint64 id);
int aircraft_error(Logbook *logbook);
int aircraft_tail_validate(Logbook *logbook);
int aircraft_fleetno_validate(Logbook *logbook);

#endif
