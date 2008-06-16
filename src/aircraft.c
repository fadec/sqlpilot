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

#include "aircraft.h"

int aircraft_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *ident=NULL;
  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       FLIGHTS_COL_DATE, &ident,
		       -1);
    snprintf(show, size, "%s", ident);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }
}

void aircraft_after_change(Sqlpilot *sqlpilot)
{
  sqlpilot->flights_stale = TRUE;
  sqlpilot->types_stale = TRUE;
}

int aircraft_can_delete(GtkTreeSelection *selection)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *flights=NULL;
  long _flights=0;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, AIRCRAFT_COL_FLIGHTS, &flights, -1);
    sscanf(flights, "%ld", &_flights);
  }

  g_free(flights);

  return !_flights;
}

DBint64 aircraft_write_entries(const gchar *id, Sqlpilot *sqlpilot)
{
  const gchar
    *ident,
    *type,
    *fleetno;
  gchar *notes;
  DBStatement *stmt;

  ident    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_ident));
  type     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_type));
  fleetno  = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_fleetno));
  notes    = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->aircraft_notes));
  
  /* Write entries to database */
  if (id) {
    stmt = sqlpilot->aircraft_update;
    db_bind_text(stmt, AIRCRAFT_WRITE_ID, id);
  } else {
    stmt = sqlpilot->aircraft_insert;
  }
  db_bind_nonempty_text_else_null(stmt, AIRCRAFT_WRITE_IDENT, ident);
  db_bind_nonempty_text_else_null(stmt, AIRCRAFT_WRITE_FLEETNO, fleetno);
  bind_id_of(stmt, AIRCRAFT_WRITE_TYPE, "types", "ident", type);
  db_bind_text(stmt, AIRCRAFT_WRITE_NOTES, notes);

  db_step(stmt);
  db_reset(stmt);
  db_clear_bindings(stmt);

  g_free(notes);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(sqlpilot->db);
  }

}

void aircraft_load_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id=NULL,
    *ident=NULL,
    *type=NULL,
    *fleetno=NULL,
    *notes=NULL;

  if (gtk_tree_selection_get_selected (logb->aircraft_selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       AIRCRAFT_COL_ID, &id,
		       AIRCRAFT_COL_IDENT, &ident,
		       AIRCRAFT_COL_TYPE, &type,
		       AIRCRAFT_COL_FLEETNO, &fleetno,
		       AIRCRAFT_COL_NOTES, &notes,
		       -1);
  }

  gtk_entry_set_text(GTK_ENTRY(logb->aircraft_ident), EMPTY_IF_NULL(ident));
  gtk_entry_set_text(GTK_ENTRY(logb->aircraft_type), EMPTY_IF_NULL(type));
  gtk_entry_set_text(GTK_ENTRY(logb->aircraft_fleetno), EMPTY_IF_NULL(fleetno));
  text_view_set_text(GTK_TEXT_VIEW(logb->aircraft_notes), EMPTY_IF_NULL(notes));

  g_free(id);
  g_free(ident);
  g_free(type);
  g_free(fleetno);
  g_free(notes);
}

void aircraft_refresh(Sqlpilot *sqlpilot)
{
  store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->aircraft_treemodel), sqlpilot->aircraft_select_all);
  aircraft_load_selection(sqlpilot);
  sqlpilot->aircraft_stale = FALSE;
}

int aircraft_count_flights(Sqlpilot *sqlpilot, DBint64 id)
{
  DBStatement *stmt;
  int c;

  stmt = sqlpilot->aircraft_count_flights;

  db_bind_int64(stmt, 1, id);
  if (db_step(stmt) == DB_ROW) {
    c = db_column_int(stmt,0);
  } else {
    c = 0;
  }

  db_reset(stmt);
  db_clear_bindings(stmt);
  
  return c;
}

int aircraft_ident_validate(Sqlpilot *sqlpilot)
{
  gchar *id=NULL;
  const gchar *ident;

  id = get_text_from_tree_selection(sqlpilot->aircraft_selection, COL_ID);
  ident = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_ident));
  
  if (unique_but_for(sqlpilot->db, "aircraft", "ident", ident, "id", EMPTY_IF_NULL(id))) {
    sqlpilot->aircraft_ident_error = 0;
  } else {
    sqlpilot->aircraft_ident_error = 1;
  }

  g_free(id);

  return sqlpilot->aircraft_ident_error;
}

int aircraft_fleetno_validate(Sqlpilot *sqlpilot)
{
  gchar *id=NULL;
  const gchar *fleetno;

  id = get_text_from_tree_selection(sqlpilot->aircraft_selection, COL_ID);
  fleetno = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_fleetno));

  if (unique_but_for(sqlpilot->db, "aircraft", "fleetno", fleetno, "id", EMPTY_IF_NULL(id))) {
    sqlpilot->aircraft_fleetno_error = 0;
  } else {
    sqlpilot->aircraft_fleetno_error = 1;
  }

  g_free(id);

  return sqlpilot->aircraft_fleetno_error;
}

int aircraft_error(Sqlpilot *sqlpilot)
{
  const gchar *ident, *fleetno;
  ident = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_ident));
  fleetno = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_fleetno));

  return ( !((ident && *ident) || (fleetno && *fleetno))
	   || sqlpilot->aircraft_ident_error
	   || sqlpilot->aircraft_fleetno_error);
}
