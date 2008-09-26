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

#ifndef FLIGHTS_H
#define FLIGHTS_H

#include "sqlpilot.h"
#include "logbook.h"
/* Column numbers for treemodel and select statements */
enum {
  FLIGHTS_COL_ID = COL_ID,
  FLIGHTS_COL_AIRCRAFT_ID,
  FLIGHTS_COL_ROLE_ID,
  FLIGHTS_COL_DEP_ID,
  FLIGHTS_COL_ARR_ID,
  FLIGHTS_COL_DATE,
  FLIGHTS_COL_LEG,
  FLIGHTS_COL_TAIL,
  FLIGHTS_COL_FLEETNO,
  FLIGHTS_COL_MODEL,
  FLIGHTS_COL_ROLE,
  FLIGHTS_COL_DEPIATA,
  FLIGHTS_COL_DEPICAO,
  FLIGHTS_COL_ARRIATA,
  FLIGHTS_COL_ARRICAO,
  FLIGHTS_COL_RTIATA,
  FLIGHTS_COL_RTICAO,
  FLIGHTS_COL_STOPS,
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
  FLIGHTS_COL_OVER,
  FLIGHTS_NUMCOL
};

/* All columns must be selected for use during update queries */
#define FLIGHTS_SELECT							\
  "select * from (select flights.id as flight_id"			\
  ", a.id as 'aircraft_id'"						\
  ", r.id as 'role_id'"							\
  ", dep.id as 'dep_id'"						\
  ", arr.id as 'arr_id'"						\
  ", flights.Date as Date"						\
  ", flights.Leg as Leg"						\
  ", a.tail as Tail"							\
  ", a.fleetno as FleetNo"						\
  ", m.ident as Model"							\
  ", r.ident as Role"							\
  ", dep.iata as DepIATA"						\
  ", dep.icao as DepICAO"						\
  ", arr.iata as ArrIATA"						\
  ", arr.icao as ArrICAO"						\
  ", CASE WHEN dep.iata IS NOT NULL THEN dep.iata ELSE dep.icao END || ' ' || CASE WHEN count(rta.id) THEN group_concat(CASE WHEN rta.iata IS NOT NULL THEN rta.iata ELSE rta.icao END, ' ') || ' ' ELSE '' END || CASE WHEN arr.iata IS NOT NULL THEN arr.iata ELSE arr.icao END AS RtIATA" \
  ", CASE WHEN dep.icao IS NOT NULL THEN dep.icao ELSE dep.iata END || ' ' || CASE WHEN count(rta.id) THEN group_concat(CASE WHEN rta.icao IS NOT NULL THEN rta.icao ELSE rta.iata END, ' ') || ' ' ELSE '' END || CASE WHEN arr.icao IS NOT NULL THEN arr.icao ELSE arr.iata END AS RtICAO" \
  ", count(rta.id) AS Stops"						\
  ", flights.aout as AOut"						\
  ", flights.AOutUTC as AOutUTC"					\
  ", flights.ain as AIn"						\
  ", flights.AInUTC as AInUTC"						\
  ", m_to_hhmm(flights.dur) as Dur"					\
  ", m_to_hhmm(flights.night) as Night"					\
  ", m_to_hhmm(flights.inst) as Inst"					\
  ", m_to_hhmm(flights.siminst) as SimInst"				\
  ", bool(flights.hold) as Hold"					\
  ", flights.aprch as Aprch"						\
  ", linecount(flights.aprch) as nApr"					\
  ", bool(flights.xc) as XC"						\
  ", round(dist_nm(dep.lat, dep.lon, arr.lat, arr.lon)) as Dist"	\
  ", flights.dland as DLand"						\
  ", flights.nland as NLand"						\
  ", flights.crew as Crew"						\
  ", linecount(flights.crew) as Crw"					\
  ", flights.notes as Notes"						\
  ", linecount(flights.notes) as Nts"					\
  ", flights.fltno as FltNo"						\
  ", flights.sout as SOut"						\
  ", flights.SOutUTC as SOutUTC"					\
  ", flights.sin as SIn"						\
  ", flights.SInUTC as SInUTC"						\
  ", m_to_hhmm(flights.sdur) as SDur"					\
  ", flights.trip as Trip"						\
  ", flights.TripDate as TripDate"					\
  ", m_to_hhmm(flights.dur - flights.sdur) as Over"			\
  " from flights"							\
  " left join aircraft a on flights.aircraft_id = a.id"			\
  " left join models m on a.model_id = m.id"				\
  " left join roles r on flights.role_id = r.id"			\
  " left join airports dep on flights.dep_id = dep.id"			\
  " left join airports arr on flights.arr_id = arr.id"			\
  " left join routing rt on rt.flight_id = flights.id"			\
  " left join airports rta on rt.airport_id = rta.id"			\
  " group by flight_id)"						

#define FLIGHTS_ORDER \
  " order by Date, Leg ASC"

#define FLIGHTS_WHERE_ID \
  " where flight_id = ? LIMIT 1;"

#define FLIGHTS_WHERE_DATE_FLTNO_DEPIATA \
  " WHERE Date == ? AND FltNo == ? AND DepIATA == ? LIMIT 1;"

#define FLIGHTS_WHERE_DATE_FLTNO_DEPICAO \
  " WHERE Date == ? AND FltNo == ? AND DepICAO == ? LIMIT 1;"

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

enum {
  FLIGHTS_ROUTING_WRITE_FLIGHT_ID = 1,
  FLIGHTS_ROUTING_WRITE_AIRPORT_ID,
  FLIGHTS_ROUTING_WRITE_SEQ
};

#define FLIGHTS_ROUTING_INSERT \
  "insert into routing (flight_id, airport_id, seq) values (?, ?, ?);"

#define FLIGHTS_ROUTING_DELETE \
  "delete from routing where flight_id = ?;"


void flights_lookup_dep_tz(Logbook *, char *);
void flights_lookup_arr_tz(Logbook *, char *);
void flights_handle_iata_update(Logbook *, GtkEntry *iata, GtkEntry *icao);
void flights_handle_icao_update(Logbook *, GtkEntry *icao, GtkEntry *iata);
int flights_selection_show(GtkTreeSelection *selection, char *show, size_t size);
int flights_can_delete(GtkTreeSelection *selection);
void flights_after_change(Logbook *data);
DBint64 flights_write_entries(const gchar *id, Logbook *);

void entry_format_date_on_focus_out(GtkEntry *entry);
void entry_format_time_of_day(GtkEntry *entry, const char *local_tz, const char *to_tz, const char *date);
void entry_format_time_on_focus_out(GtkEntry *entry, char separator);
void entry_format_time_on_focus_in(GtkEntry *entry);
long elapsed_seconds(const char *date, const char *t1, const char *tz1, const char *t2, const char *tz2);
void reconcile_time_entries(Logbook *logb,
			    GtkEntry *changed,
			    GtkEntry *start,
			    GtkEntry *end,
			    GtkEntry *elapsed);
void flights_load_selection(Logbook *);
void flights_refresh(Logbook *logbook);
void flights_refresh_utilization(Logbook *logbook);
void flights_refresh_aircraft_utilized(Logbook *logbook);
void flights_refresh_role_utilized(Logbook *logbook);
void flights_refresh_dep_utilized(Logbook *logbook);
void flights_refresh_arr_utilized(Logbook *logbook);
void flights_build_store_view(Logbook *logbook);
void flights_save_options(Logbook *logbook);
void flights_restore_options(Logbook *logbook);
void flights_fleetno_toggle_set_sensitivity(Logbook *logbook);
int flights_error(Logbook *logbook);
int flights_swap_airport_key(Logbook *logbook, GtkEntry *entry);
void flights_fill_route(Logbook *logbook, int keypref, GtkEntry *from, GtkEntry *to);
void flights_route_fills_deparr(Logbook *logbook);
void flights_deparr_fills_route(Logbook *logbook);
#endif
