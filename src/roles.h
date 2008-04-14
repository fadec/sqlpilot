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

#define ROLES_SELECT						\
  "select roles.id as _id"					\
  ", roles.ident as Ident"					\
  ", roles.name as Name"					\
  ", bool(roles.pic) as PIC"					\
  ", bool(roles.sic) as SIC"					\
  ", bool(roles.fe) as FE"					\
  ", bool(roles.solo) as Solo"					\
  ", bool(roles.dual) as Dual"					\
  ", bool(roles.instruct) as Instruct"				\
  ", bool(roles.total) as Total"				\
  ", count(flights.id) as Flights"				\
  ", m_to_hhmm(sum(flights.dur)) as Time"			\
  " from roles"							\
  " left join flights on flights.role_id = roles.id"		\

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


void roles_after_change(Sqlpilot *sqlpilot);

DBint64 roles_write_entries(const gchar *id, Sqlpilot *sqlpilot);

void roles_load_selection(Sqlpilot *sqlpilot);

int roles_selection_show(GtkTreeSelection *selection, char *show, size_t size);

int roles_selection_show(GtkTreeSelection *selection, char *show, size_t size);
int roles_can_delete(GtkTreeSelection *selection);

#endif