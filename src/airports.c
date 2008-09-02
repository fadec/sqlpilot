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

#include "sqlpilot.h"
#include "airports.h"
#include "logbook.h"
#include "summaries.h"
#include "assert.h"

int airports_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *treemodel;
  gchar
    *iata=NULL,
    *icao=NULL,
    *city=NULL;
  if (gtk_tree_selection_get_selected (selection, &treemodel, &iter)) {
    gtk_tree_model_get(treemodel, &iter,
		       AIRPORTS_COL_IATA, &iata,
		       AIRPORTS_COL_ICAO, &icao,
		       AIRPORTS_COL_CITY, &city,
		       -1);
    snprintf(show, size, "%s/%s - %s", iata, icao, city);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }

  g_free(iata);
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

void airports_after_change(Logbook *logbook)
{
  logbook->flights_stale = TRUE;
}

DBint64 airports_write_entries(const gchar *id, Logbook *logbook)
{
  const gchar
    *iata,
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

  iata    = gtk_entry_get_text(GTK_ENTRY(logbook->airports_iata));
  icao     = gtk_entry_get_text(GTK_ENTRY(logbook->airports_icao));
  lat      = gtk_entry_get_text(GTK_ENTRY(logbook->airports_lat));
  lon      = gtk_entry_get_text(GTK_ENTRY(logbook->airports_lon));
  elev     = gtk_entry_get_text(GTK_ENTRY(logbook->airports_elev));
  city     = gtk_entry_get_text(GTK_ENTRY(logbook->airports_city));
  province = gtk_entry_get_text(GTK_ENTRY(logbook->airports_province));
  country  = gtk_entry_get_text(GTK_ENTRY(logbook->airports_country));
  name     = gtk_entry_get_text(GTK_ENTRY(logbook->airports_name));
  tzone    = gtk_entry_get_text(GTK_ENTRY(logbook->airports_tzone));
  notes    = text_view_get_text(GTK_TEXT_VIEW(logbook->airports_notes));

  
  /* Write entries to database */
  if (id) {
    stmt = logbook->airports_update;
    db_bind_text(stmt, AIRPORTS_WRITE_ID, id);
  } else {
    stmt = logbook->airports_insert;
  }
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_IATA, iata);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_ICAO,  icao);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_LAT,   lat);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_LON,   lon);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_ELEV,  elev);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_CITY, city);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_PROVINCE, province);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_COUNTRY, country);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_NAME,  name);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_TZONE, tzone);
  db_bind_nonempty_text_else_null(stmt, AIRPORTS_WRITE_NOTES, notes);
  
  db_stp_res_clr(stmt);

  g_free(notes);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(logbook->db);
  }
}

void airports_load_selection(Logbook *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id=NULL,
    *iata=NULL,
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
		       AIRPORTS_COL_IATA, &iata,
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
  
  gtk_entry_set_text(GTK_ENTRY(logb->airports_iata), EMPTY_IF_NULL(iata));
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
  g_free(iata);
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

void airports_refresh(Logbook *logbook)
{
    logbook->airports_stale = FALSE;
    store_repopulate_from_stmt(GTK_LIST_STORE(logbook->airports_treemodel), logbook->airports_select_all);
    airports_load_selection(logbook);
}

int airports_iata_validate(Logbook *logbook)
{
  gchar *id=NULL;
  const gchar *iata;

  id = get_text_from_tree_selection(logbook->airports_selection, COL_ID);
  iata = gtk_entry_get_text(GTK_ENTRY(logbook->airports_iata));
  
  if ((strlen(iata) == 3) && unique_but_for(logbook->db, "airports", "iata", iata, "id", EMPTY_IF_NULL(id))) {
    logbook->airports_iata_error = 0;
  } else {
    logbook->airports_iata_error = 1;
  }

  g_free(id);

  return logbook->airports_iata_error;
}

int airports_icao_validate(Logbook *logbook)
{
  gchar *id=NULL;
  const gchar *icao;

  id = get_text_from_tree_selection(logbook->airports_selection, COL_ID);
  icao = gtk_entry_get_text(GTK_ENTRY(logbook->airports_icao));

  if ((strlen(icao) == 4) && unique_but_for(logbook->db, "airports", "icao", icao, "id", EMPTY_IF_NULL(id))) {
    logbook->airports_icao_error = 0;
  } else {
    logbook->airports_icao_error = 1;
  }

  g_free(id);

  return logbook->airports_icao_error;
}

int airports_error(Logbook *logbook)
{
  const gchar *iata, *icao;
  iata = gtk_entry_get_text(GTK_ENTRY(logbook->airports_iata));
  icao = gtk_entry_get_text(GTK_ENTRY(logbook->airports_icao));

  return ( !((iata && *iata) || (icao && *icao))
	   || logbook->airports_iata_error
	   || logbook->airports_icao_error);
}

void airports_tzone_combo_box_init(Logbook *logbook)
{
  gchar *argv[] = {"/usr/bin/find"};
  gchar tzone[BUF_TZ];
  int nstdin, nstdout, nstderr;
  FILE *fstdout=NULL, *fstderr=NULL;
  GPid pid;
  gboolean ret;
  int c;
  GSpawnFlags sf;
  GError **error = NULL;
  sf = 0;
  assert(ret = g_spawn_async_with_pipes(TZ_DIR,
					argv,
					NULL,
					sf, NULL, NULL,
					&pid,
					&nstdin,
					&nstdout,
					&nstderr,
					error));

  assert(((fstdout = fdopen(nstdout, "rb")) && (fstderr = fdopen(nstderr, "rb"))));

  while ((c = fgetc(fstderr)) != EOF) {
    putc(c, stderr);
  }


  GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
  GtkTreeIter iter;
  GtkCellRenderer *renderer;

  while (fgets(tzone, BUF_TZ, fstdout)) {
    gtk_list_store_append(store, &iter);
    gtk_list_store_set (store, &iter,
			0, tzone,
			-1);
  }

  fclose(fstdout);
  fclose(fstderr);

  g_spawn_close_pid(pid);

  gtk_combo_box_set_model(GTK_COMBO_BOX(logbook->airports_tzone_combo), GTK_TREE_MODEL(store));
  gtk_cell_layout_clear(GTK_CELL_LAYOUT(logbook->airports_tzone_combo));
  renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(logbook->airports_tzone_combo), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(logbook->airports_tzone_combo), renderer,
				 "text", 0,
				 NULL);

  gtk_combo_box_set_active(GTK_COMBO_BOX(logbook->airports_tzone_combo), 0);
}
