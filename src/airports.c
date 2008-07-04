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

#include "airports.h"

int airports_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *treemodel;
  gchar
    *ident=NULL,
    *icao=NULL,
    *city=NULL;
  if (gtk_tree_selection_get_selected (selection, &treemodel, &iter)) {
    gtk_tree_model_get(treemodel, &iter,
		       AIRPORTS_COL_IDENT, &ident,
		       AIRPORTS_COL_ICAO, &icao,
		       AIRPORTS_COL_CITY, &city,
		       -1);
    snprintf(show, size, "%s/%s - %s", ident, icao, city);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }

  g_free(ident);
  g_free(icao);
  g_free(city);
}

int airports_can_delete(GtkTreeSelection *selection)
{
/*   GtkTreeIter iter; */
/*   GtkTreeModel *model; */
/*   gchar *aircraft=NULL; */
/*   long _aircraft=0; */

/*   if (gtk_tree_selection_get_selected(selection, &model, &iter)) { */
/*     gtk_tree_model_get(model, &iter, AIRPORTS_COL_OPS, &aircraft, -1); */
/*     sscanf(aircraft, "%ld", &_aircraft); */
/*   } */

/*   g_free(aircraft); */

  //  return !_aircraft;
  return FALSE;
}

void airports_after_change(Sqlpilot *sqlpilot)
{
  sqlpilot->flights_stale = TRUE;
}

DBint64 airports_write_entries(const gchar *id, Sqlpilot *sqlpilot)
{
  const gchar
    *ident,
    *icao,
    *lat,
    *lon,
    *elev,
    *city,
    *province,
    *country,
    *name,
    *tzone;
  gchar *notes;
  DBStatement *stmt;

  ident    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_ident));
  icao     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_icao));
  lat      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_lat));
  lon      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_lon));
  elev     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_elev));
  city     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_city));
  province = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_province));
  country  = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_country));
  name     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_name));
  tzone    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_tzone));
  notes    = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->airports_notes));

  
  /* Write entries to database */
  if (id) {
    stmt = sqlpilot->airports_update;
    db_bind_text(stmt, AIRPORTS_WRITE_ID, id);
  } else {
    stmt = sqlpilot->airports_insert;
  }
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_IDENT, ident);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_ICAO,  icao);
  db_bind_text(stmt, AIRPORTS_WRITE_LAT,   lat);
  db_bind_text(stmt, AIRPORTS_WRITE_LON,   lon);
  db_bind_text(stmt, AIRPORTS_WRITE_ELEV,  elev);
  db_bind_text(stmt, AIRPORTS_WRITE_CITY, city);
  db_bind_text(stmt, AIRPORTS_WRITE_PROVINCE, province);
  db_bind_text(stmt, AIRPORTS_WRITE_COUNTRY, country);
  db_bind_text(stmt, AIRPORTS_WRITE_NAME,  name);
  db_bind_text(stmt, AIRPORTS_WRITE_TZONE, tzone);
  db_bind_text(stmt, AIRPORTS_WRITE_NOTES, notes);
  
  db_stp_res_clr(stmt);

  g_free(notes);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(sqlpilot->db);
  }
}

void airports_load_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id=NULL,
    *ident=NULL,
    *icao=NULL,
    *lat=NULL,
    *lon=NULL,
    *elev=NULL,
    *name=NULL,
    *city=NULL,
    *province=NULL,
    *country=NULL,
    *tzone=NULL,
    *notes=NULL;

  if (gtk_tree_selection_get_selected (logb->airports_selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       AIRPORTS_COL_ID, &id,
		       AIRPORTS_COL_IDENT, &ident,
		       AIRPORTS_COL_ICAO, &icao,
		       AIRPORTS_COL_LAT, &lat,
		       AIRPORTS_COL_LON, &lon,
		       AIRPORTS_COL_ELEV, &elev,
		       AIRPORTS_COL_NAME, &name,
		       AIRPORTS_COL_CITY, &city,
		       AIRPORTS_COL_PROVINCE, &province,
		       AIRPORTS_COL_COUNTRY, &country,
		       AIRPORTS_COL_TZONE, &tzone,
		       AIRPORTS_COL_NOTES, &notes,
		       -1);
  }
  
  gtk_entry_set_text(GTK_ENTRY(logb->airports_ident), EMPTY_IF_NULL(ident));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_icao), EMPTY_IF_NULL(icao));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_lat), EMPTY_IF_NULL(lat));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_lon), EMPTY_IF_NULL(lon));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_elev), EMPTY_IF_NULL(elev));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_name), EMPTY_IF_NULL(name));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_city), EMPTY_IF_NULL(city));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_province), EMPTY_IF_NULL(province));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_country), EMPTY_IF_NULL(country));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_tzone), EMPTY_IF_NULL(tzone));
  text_view_set_text(GTK_TEXT_VIEW(logb->airports_notes), EMPTY_IF_NULL(notes));

  g_free(id);
  g_free(ident);
  g_free(icao);
  g_free(lat);
  g_free(lon);
  g_free(elev);
  g_free(name);
  g_free(city);
  g_free(province);
  g_free(country);
  g_free(tzone);
  g_free(notes);
}

void airports_refresh(Sqlpilot *sqlpilot)
{
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->airports_treemodel), sqlpilot->airports_select_all);
    airports_load_selection(sqlpilot);
    sqlpilot->airports_stale = FALSE;
}

int airports_ident_validate(Sqlpilot *sqlpilot)
{
  gchar *id=NULL;
  const gchar *ident;

  id = get_text_from_tree_selection(sqlpilot->airports_selection, COL_ID);
  ident = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_ident));
  
  if ((strlen(ident) == 3) && unique_but_for(sqlpilot->db, "airports", "ident", ident, "id", EMPTY_IF_NULL(id))) {
    sqlpilot->airports_ident_error = 0;
  } else {
    sqlpilot->airports_ident_error = 1;
  }

  g_free(id);

  return sqlpilot->airports_ident_error;
}

int airports_icao_validate(Sqlpilot *sqlpilot)
{
  gchar *id=NULL;
  const gchar *icao;

  id = get_text_from_tree_selection(sqlpilot->airports_selection, COL_ID);
  icao = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_icao));

  if ((strlen(icao) == 4) && unique_but_for(sqlpilot->db, "airports", "icao", icao, "id", EMPTY_IF_NULL(id))) {
    sqlpilot->airports_icao_error = 0;
  } else {
    sqlpilot->airports_icao_error = 1;
  }

  g_free(id);

  return sqlpilot->airports_icao_error;
}

int airports_error(Sqlpilot *sqlpilot)
{
  const gchar *ident, *icao;
  ident = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_ident));
  icao = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_icao));

  return ( !((ident && *ident) || (icao && *icao))
	   || sqlpilot->airports_ident_error
	   || sqlpilot->airports_icao_error);
}

