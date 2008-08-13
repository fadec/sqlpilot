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

#ifndef ROLES_H
#define ROLES_H

#include "sqlpilot.h"

enum {
  ROLES_COL_ID = 0,
  ROLES_COL_IDENT,
  ROLES_COL_NAME,
  ROLES_COL_PIC,
  ROLES_COL_SIC,
  ROLES_COL_FE,
  ROLES_COL_SOLO,
  ROLES_COL_DUAL,
  ROLES_COL_INSTRUCT,
  ROLES_COL_TOTAL,
  ROLES_COL_FLIGHTS,
  ROLES_COL_TIME
};

#define ROLES_SELECT							\
  "select roles.id as '_\\id'"						\
  ", roles.ident as Ident"						\
  ", roles.name as Name"						\
  ", bool(roles.pic) as '_\\PIC'"					\
  ", bool(roles.sic) as '_\\SIC'"					\
  ", bool(roles.fe) as '_\\FE'"						\
  ", bool(roles.solo) as '_\\Solo'"					\
  ", bool(roles.dual) as '_\\Dual'"					\
  ", bool(roles.instruct) as '_\\Instruct'"				\
  ", bool(roles.total) as '_\\Total'"					\
  ", count(flights.id) as 'n\\Flights'"					\
  ", m_to_hhmm(sum(flights.dur)) as 'n\\Time'"				\
  " from roles"								\
  " left join flights on flights.role_id = roles.id and flights.dur > 0"

#define ROLES_GROUP_BY \
  " group by roles.id order by roles.ident"

#define ROLES_WHERE_ID \
  " where roles.id = ?"

enum {
  ROLES_WRITE_IDENT = 1,
  ROLES_WRITE_NAME,
  ROLES_WRITE_PIC,
  ROLES_WRITE_SIC,
  ROLES_WRITE_FE,
  ROLES_WRITE_SOLO,
  ROLES_WRITE_DUAL,
  ROLES_WRITE_INSTRUCT,
  ROLES_WRITE_TOTAL,
  ROLES_WRITE_ID
};

#define ROLES_INSERT \
  "insert into roles (ident, name, pic, sic, fe, solo, dual, instruct, total) values (?, ?, ?, ?, ?, ?, ?, ?, ?);"

#define ROLES_UPDATE \
  "update roles set ident = ?, name = ?, pic = ?, sic = ?, fe = ?, solo = ?, dual = ?, instruct = ?, total = ?" \
  " where id = ?;"

#define ROLES_DELETE \
  "delete from roles where id = ?;"


void roles_after_change(Logbook *logbook);

DBint64 roles_write_entries(const gchar *id, Logbook *logbook);

void roles_load_selection(Logbook *logbook);

int roles_selection_show(GtkTreeSelection *selection, char *show, size_t size);

int roles_selection_show(GtkTreeSelection *selection, char *show, size_t size);
int roles_can_delete(GtkTreeSelection *selection);

int roles_error(Logbook *logbook);
int roles_ident_validate(Logbook *logbook);


#endif
