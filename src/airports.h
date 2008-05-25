/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.				        */
/* 								        */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.				        */
/* 								        */
/* Sqlpilot is distributed in the hope that it will be useful,	        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.			        */
/* 								        */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#ifndef AIRPORTS_H
#define AIRPORTS_H

#include <sqlpilot.h>

enum {
  AIRPORTS_COL_ID = COL_ID,
  AIRPORTS_COL_IDENT,
  AIRPORTS_COL_ICAO,
  AIRPORTS_COL_NAME,
  AIRPORTS_COL_TZONE,
  AIRPORTS_COL_LAT,
  AIRPORTS_COL_LON,
  AIRPORTS_COL_ELEV,
  AIRPORTS_COL_CITY,
  AIRPORTS_COL_PROVINCE,
  AIRPORTS_COL_COUNTRY,
  AIRPORTS_COL_NOTES,
  AIRPORTS_COL_DEP,
  AIRPORTS_COL_ARR
};

#define AIRPORTS_SELECT					\
  "select a.id as _id"					\
  ", a.ident as Ident"					\
  ", a.icao as ICAO"					\
  ", a.name as Name"					\
  ", a.tzone as TZone"					\
  ", a.lat as Lat"					\
  ", a.lon as Lon"					\
  ", a.elev as Elev"					\
  ", a.city as City"					\
  ", a.province as Province"				\
  ", a.country as Country"				\
  ", a.notes as _Notes"					\
  " from Airports a"					\

#define AIRPORTS_GROUP_BY \
  " group by a.id order by a.ident"

#define AIRPORTS_WHERE_ID \
  " where a.id = ?"

enum {
  AIRPORTS_WRITE_IDENT = 1,
  AIRPORTS_WRITE_ICAO,
  AIRPORTS_WRITE_NAME,
  AIRPORTS_WRITE_CITY,
  AIRPORTS_WRITE_PROVINCE,
  AIRPORTS_WRITE_COUNTRY,
  AIRPORTS_WRITE_LAT,
  AIRPORTS_WRITE_LON,
  AIRPORTS_WRITE_ELEV,
  AIRPORTS_WRITE_TZONE,
  AIRPORTS_WRITE_NOTES,
  AIRPORTS_WRITE_ID
};

#define AIRPORTS_INSERT				\
  "insert into airports "			\
  "( ident"					\
  ", icao"					\
  ", name"					\
  ", city"					\
  ", province"					\
  ", country"					\
  ", lat"					\
  ", lon"					\
  ", elev"					\
  ", tzone"					\
  ", notes"					\
  ") values (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);"

#define AIRPORTS_UPDATE				\
  "update airports set"				\
  "  ident = ?"					\
  ", icao = ?"					\
  ", name = ?"					\
  ", city = ?"					\
  ", province = ?"				\
  ", country = ?"				\
  ", lat = ?"					\
  ", lon = ?"					\
  ", elev = ?"					\
  ", tzone = ?"					\
  ", notes = ?"					\
  " where id = ?;"

#define AIRPORTS_DELETE \
  "delete from airports where id = ?;"

#include "airports.h"


void airports_after_change(Sqlpilot *sqlpilot);
DBint64 airports_write_entries(const gchar *id, Sqlpilot *sqlpilot);
void airports_load_selection(Sqlpilot *logb);
void airports_refresh(Sqlpilot *sqlpilot);
int airports_selection_show(GtkTreeSelection *selection, char *show, size_t size);
int airports_can_delete(GtkTreeSelection *selection);

#endif
