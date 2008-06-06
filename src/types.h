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

#ifndef TYPES_H
#define TYPES_H

#include "sqlpilot.h"

enum {
  TYPES_COL_ID = COL_ID,
  TYPES_COL_IDENT,
  TYPES_COL_MAKE,
  TYPES_COL_MODEL,
  TYPES_COL_AIRPLANE,
  TYPES_COL_ROTORCRAFT,
  TYPES_COL_GLIDER,
  TYPES_COL_LTA,
  TYPES_COL_POWEREDLIFT,
  TYPES_COL_PPC,
  TYPES_COL_WEIGHTSHIFT,
  TYPES_COL_HELI,
  TYPES_COL_GYRO,
  TYPES_COL_AIRSHIP,
  TYPES_COL_BALLOON,
  TYPES_COL_SINGLE,
  TYPES_COL_MULTI,
  TYPES_COL_LAND,
  TYPES_COL_SEA,
  TYPES_COL_TURBINE,
  TYPES_COL_JET,
  TYPES_COL_HIGHPERF,
  TYPES_COL_RETRACT,
  TYPES_COL_COMPLEX,
  TYPES_COL_PRESSURIZED,
  TYPES_COL_LARGE,
  TYPES_COL_SPORT,
  TYPES_COL_ULTRULIGHT,
  TYPES_COL_FOOTLAUNCH,
  TYPES_COL_SIM,
  TYPES_COL_FTD,
  TYPES_COL_TOTAL,
  TYPES_COL_AIRCRAFT,
  TYPES_COL_FLIGHTS,
  TYPES_COL_TIME
};

#define TYPES_SELECT						\
  "select types.id as _id"					\
  ", types.ident as Ident"					\
  ", types.make as Make"					\
  ", types.model as Model"					\
  ", types.airplane as _Airplane"				\
  ", types.rotorcraft as _Rotorcraft"				\
  ", types.glider as _Glider"					\
  ", types.lta as _LTA"						\
  ", types.poweredlift as _Poweredlift"				\
  ", types.ppc as _PPC"						\
  ", types.weightshift as _Weightshift"				\
  ", types.heli as _Heli"					\
  ", types.gyro as _Gyro"					\
  ", types.airship as _Airship"					\
  ", types.balloon as _Balloon"					\
  ", types.single as _Single"					\
  ", types.multi as _Multi"					\
  ", types.land as _Land"					\
  ", types.sea as _Sea"						\
  ", types.turbine as _Turbine"					\
  ", types.jet as _Jet"						\
  ", types.highperf as _HighPerf"				\
  ", types.retract as _Retract"					\
  ", types.complex as _Complex"					\
  ", types.pressurized as _Pressurized"				\
  ", types.large as _Large"					\
  ", types.sport as _Sport"					\
  ", types.ultralight as _Ultralight"				\
  ", types.footlaunch as _Footlaunch"				\
  ", types.sim as _Sim"						\
  ", types.ftd as _FTD"						\
  ", types.total as _Total"					\
  ", count(distinct(aircraft.id)) as Aircraft"			\
  ", count(flights.id) as Flights"				\
  ", m_to_hhmm(sum(flights.dur)) as Time"			\
  " from types"							\
  " left join aircraft on aircraft.type_id = types.id"		\
  " left join flights on flights.aircraft_id = aircraft.id"

#define TYPES_GROUP_BY \
  " group by types.id order by types.ident"

#define TYPES_WHERE_ID \
  " where types.id = ?"

enum {
  TYPES_WRITE_IDENT = 1,
  TYPES_WRITE_MAKE,
  TYPES_WRITE_MODEL,
  TYPES_WRITE_AIRPLANE,
  TYPES_WRITE_ROTORCRAFT,
  TYPES_WRITE_GLIDER,
  TYPES_WRITE_LTA,
  TYPES_WRITE_POWEREDLIFT,
  TYPES_WRITE_PPC,
  TYPES_WRITE_WEIGHTSHIFT,
  TYPES_WRITE_HELI,
  TYPES_WRITE_GYRO,
  TYPES_WRITE_AIRSHIP,
  TYPES_WRITE_BALLOON,
  TYPES_WRITE_SINGLE,
  TYPES_WRITE_MULTI,
  TYPES_WRITE_LAND,
  TYPES_WRITE_SEA,
  TYPES_WRITE_TURBINE,
  TYPES_WRITE_JET,
  TYPES_WRITE_HIGHPERF,
  TYPES_WRITE_RETRACT,
  TYPES_WRITE_COMPLEX,
  TYPES_WRITE_PRESSURIZED,
  TYPES_WRITE_LARGE,
  TYPES_WRITE_SPORT,
  TYPES_WRITE_ULTRULIGHT,
  TYPES_WRITE_FOOTLAUNCH,
  TYPES_WRITE_SIM,
  TYPES_WRITE_FTD,
  TYPES_WRITE_TOTAL,
  TYPES_WRITE_ID
};

#define TYPES_INSERT				\
  "insert into types "				\
  "( ident"					\
  ", make"					\
  ", model"					\
  ", airplane"					\
  ", rotorcraft"				\
  ", glider"					\
  ", lta"					\
  ", poweredlift"				\
  ", ppc"					\
  ", weightshift"				\
  ", heli"					\
  ", gyro"					\
  ", airship"					\
  ", balloon"					\
  ", single"					\
  ", multi"					\
  ", land"					\
  ", sea"					\
  ", turbine"					\
  ", jet"					\
  ", highperf"					\
  ", retract"					\
  ", complex"					\
  ", pressurized"				\
  ", large"					\
  ", sport"					\
  ", ultralight"				\
  ", footlaunch"				\
  ", sim"					\
  ", ftd"					\
  ", total)"					\
  " values ("					\
  "  ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"		\
  ", ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"		\
  ", ?, ?, ?, ?, ?, ?, ?, ?, ?, ?"		\
  ", ?"					\
  " );"

#define TYPES_UPDATE	  \
  "update types set"	  \
  " ident = ?"		  \
  ", make = ?"		  \
  ", model = ?"		  \
  ", airplane = ?"	  \
  ", rotorcraft = ?"	  \
  ", glider = ?"	  \
  ", lta = ?"		  \
  ", poweredlift = ?"	  \
  ", ppc = ?"		  \
  ", weightshift = ?"	  \
  ", heli = ?"		  \
  ", gyro = ?"		  \
  ", airship = ?"	  \
  ", balloon = ?"	  \
  ", single = ?"	  \
  ", multi = ?"		  \
  ", land = ?"		  \
  ", sea = ?"		  \
  ", turbine = ?"	  \
  ", jet = ?"		  \
  ", highperf = ?"	  \
  ", retract = ?"	  \
  ", complex = ?"	  \
  ", pressurized = ?"	  \
  ", large = ?"		  \
  ", sport = ?"		  \
  ", ultralight = ?"	  \
  ", footlaunch = ?"	  \
  ", sim = ?"		  \
  ", ftd = ?"		  \
  ", total = ?"		  \
  "where id = ?;"


#define TYPES_DELETE \
  "delete from types where id = ?;"

int types_selection_show(GtkTreeSelection *selection, char *show, size_t size);
int types_can_delete(GtkTreeSelection *selection);
void types_after_change(Sqlpilot *sqlpilot);
DBint64 types_write_entries(const gchar *id, Sqlpilot *sqlpilot);
void types_load_selection(Sqlpilot *logb);
void types_refresh(Sqlpilot *sqlpilot);

#endif
