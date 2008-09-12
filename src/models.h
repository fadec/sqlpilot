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

#ifndef MODELS_H
#define MODELS_H

#include "sqlpilot.h"

enum {
  MODELS_COL_ID = COL_ID,
  MODELS_COL_IDENT,
  MODELS_COL_MAKE,
  MODELS_COL_TYPE,
  MODELS_COL_AIRPLANE,
  MODELS_COL_ROTORCRAFT,
  MODELS_COL_GLIDER,
  MODELS_COL_LTA,
  MODELS_COL_POWEREDLIFT,
  MODELS_COL_PPC,
  MODELS_COL_WEIGHTSHIFT,
  MODELS_COL_HELI,
  MODELS_COL_GYRO,
  MODELS_COL_AIRSHIP,
  MODELS_COL_BALLOON,
  MODELS_COL_SINGLE,
  MODELS_COL_MULTI,
  MODELS_COL_LAND,
  MODELS_COL_SEA,
  MODELS_COL_TURBINE,
  MODELS_COL_JET,
  MODELS_COL_HIGHPERF,
  MODELS_COL_RETRACT,
  MODELS_COL_COMPLEX,
  MODELS_COL_PRESSURIZED,
  MODELS_COL_LARGE,
  MODELS_COL_SPORT,
  MODELS_COL_ULTRULIGHT,
  MODELS_COL_FOOTLAUNCH,
  MODELS_COL_SIM,
  MODELS_COL_FTD,
  MODELS_COL_TOTAL,
  MODELS_COL_AIRCRAFT,
  MODELS_COL_FLIGHTS,
  MODELS_COL_TIME
};

#define MODELS_SELECT						\
  "select models.id as '_\\id'"					\
  ", models.ident as Ident"					\
  ", models.make as Make"					\
  ", models.type as Type"					\
  ", models.airplane as '_\\Airplane'"				\
  ", models.rotorcraft as '_\\Rotorcraft'"			\
  ", models.glider as '_\\Glider'"				\
  ", models.lta as '_\\LTA'"					\
  ", models.poweredlift as '_\\Poweredlift'"			\
  ", models.ppc as '_\\PPC'"					\
  ", models.weightshift as '_\\Weightshift'"			\
  ", models.heli as '_\\Heli'"					\
  ", models.gyro as '_\\Gyro'"					\
  ", models.airship as '_\\Airship'"				\
  ", models.balloon as '_\\Balloon'"				\
  ", models.single as '_\\Single'"				\
  ", models.multi as '_\\Multi'"					\
  ", models.land as '_\\Land'"					\
  ", models.sea as '_\\Sea'"					\
  ", models.turbine as '_\\Turbine'"				\
  ", models.jet as '_\\Jet'"					\
  ", models.highperf as '_\\HighPerf'"				\
  ", models.retract as '_\\Retract'"				\
  ", models.complex as '_\\Complex'"				\
  ", models.pressurized as '_\\Pressurized'"			\
  ", models.large as '_\\Large'"					\
  ", models.sport as '_\\Sport'"					\
  ", models.ultralight as '_\\Ultralight'"			\
  ", models.footlaunch as '_\\Footlaunch'"			\
  ", models.sim as '_\\Sim'"					\
  ", models.ftd as '_\\FTD'"					\
  ", models.total as '_\\Total'"					\
  ", count(distinct(aircraft.id)) as 'n\\Aircraft'"		\
  ", count(flights.id) as 'n\\Flights'"				\
  ", m_to_hhmm(sum(flights.dur)) as 'n\\Time'"			\
  " from models"							\
  " left join aircraft on aircraft.model_id = models.id"		\
  " left join flights on flights.aircraft_id = aircraft.id"	\
  "   and flights.dur > 0"

#define MODELS_GROUP_BY \
  " group by models.id order by models.ident"

#define MODELS_WHERE_ID \
  " where models.id = ?"

enum {
  MODELS_WRITE_IDENT = 1,
  MODELS_WRITE_MAKE,
  MODELS_WRITE_TYPE,
  MODELS_WRITE_AIRPLANE,
  MODELS_WRITE_ROTORCRAFT,
  MODELS_WRITE_GLIDER,
  MODELS_WRITE_LTA,
  MODELS_WRITE_POWEREDLIFT,
  MODELS_WRITE_PPC,
  MODELS_WRITE_WEIGHTSHIFT,
  MODELS_WRITE_HELI,
  MODELS_WRITE_GYRO,
  MODELS_WRITE_AIRSHIP,
  MODELS_WRITE_BALLOON,
  MODELS_WRITE_SINGLE,
  MODELS_WRITE_MULTI,
  MODELS_WRITE_LAND,
  MODELS_WRITE_SEA,
  MODELS_WRITE_TURBINE,
  MODELS_WRITE_JET,
  MODELS_WRITE_HIGHPERF,
  MODELS_WRITE_RETRACT,
  MODELS_WRITE_COMPLEX,
  MODELS_WRITE_PRESSURIZED,
  MODELS_WRITE_LARGE,
  MODELS_WRITE_SPORT,
  MODELS_WRITE_ULTRULIGHT,
  MODELS_WRITE_FOOTLAUNCH,
  MODELS_WRITE_SIM,
  MODELS_WRITE_FTD,
  MODELS_WRITE_TOTAL,
  MODELS_WRITE_ID
};

#define MODELS_INSERT				\
  "insert into models "				\
  "( ident"					\
  ", make"					\
  ", type"					\
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

#define MODELS_UPDATE	  \
  "update models set"	  \
  " ident = ?"		  \
  ", make = ?"		  \
  ", type = ?"		  \
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


#define MODELS_DELETE \
  "delete from models where id = ?;"

int models_selection_show(GtkTreeSelection *selection, char *show, size_t size);
int models_can_delete(GtkTreeSelection *selection);
void models_after_change(Logbook *logbook);
DBint64 models_write_entries(const gchar *id, Logbook *logbook);
void models_load_selection(Logbook *logb);
void models_refresh(Logbook *logbook);
int models_ident_validate(Logbook *logbook);
int models_error(Logbook *logbook);


#endif
