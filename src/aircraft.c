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
#include "flights.h"
#include "logbook.h"

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

void aircraft_after_change(Logbook *logbook)
{
  logbook->flights_stale = TRUE;
  logbook->types_stale = TRUE;
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

DBint64 aircraft_write_entries(const gchar *id, Logbook *logbook)
{
  const gchar
    *ident,
    *type,
    *fleetno;
  gchar *notes;
  DBStatement *stmt;

  ident    = gtk_entry_get_text(GTK_ENTRY(logbook->aircraft_ident));
  type     = gtk_entry_get_text(GTK_ENTRY(logbook->aircraft_type));
  fleetno  = gtk_entry_get_text(GTK_ENTRY(logbook->aircraft_fleetno));
  notes    = text_view_get_text(GTK_TEXT_VIEW(logbook->aircraft_notes));
  
  /* Write entries to database */
  if (id) {
    stmt = logbook->aircraft_update;
    db_bind_text(stmt, AIRCRAFT_WRITE_ID, id);
  } else {
    stmt = logbook->aircraft_insert;
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
    return db_last_insert_rowid(logbook->db);
  }

}

void aircraft_load_selection(Logbook *logb)
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

void aircraft_refresh(Logbook *logbook)
{
  store_repopulate_from_stmt(GTK_LIST_STORE(logbook->aircraft_treemodel), logbook->aircraft_select_all);
  aircraft_load_selection(logbook);
  logbook->aircraft_stale = FALSE;
}

int aircraft_count_flights(Logbook *logbook, DBint64 id)
{
  DBStatement *stmt;
  int c;

  stmt = logbook->aircraft_count_flights;

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

int aircraft_ident_validate(Logbook *logbook)
{
  gchar *id=NULL;
  const gchar *ident;

  id = get_text_from_tree_selection(logbook->aircraft_selection, COL_ID);
  ident = gtk_entry_get_text(GTK_ENTRY(logbook->aircraft_ident));
  
  if (unique_but_for(logbook->db, "aircraft", "ident", ident, "id", EMPTY_IF_NULL(id))) {
    logbook->aircraft_ident_error = 0;
  } else {
    logbook->aircraft_ident_error = 1;
  }

  g_free(id);

  return logbook->aircraft_ident_error;
}

int aircraft_fleetno_validate(Logbook *logbook)
{
  gchar *id=NULL;
  const gchar *fleetno;

  id = get_text_from_tree_selection(logbook->aircraft_selection, COL_ID);
  fleetno = gtk_entry_get_text(GTK_ENTRY(logbook->aircraft_fleetno));

  if (unique_but_for(logbook->db, "aircraft", "fleetno", fleetno, "id", EMPTY_IF_NULL(id))) {
    logbook->aircraft_fleetno_error = 0;
  } else {
    logbook->aircraft_fleetno_error = 1;
  }

  g_free(id);

  return logbook->aircraft_fleetno_error;
}

int aircraft_error(Logbook *logbook)
{
  const gchar *ident, *fleetno;
  ident = gtk_entry_get_text(GTK_ENTRY(logbook->aircraft_ident));
  fleetno = gtk_entry_get_text(GTK_ENTRY(logbook->aircraft_fleetno));

  return ( !((ident && *ident) || (fleetno && *fleetno))
	   || logbook->aircraft_ident_error
	   || logbook->aircraft_fleetno_error);
}
