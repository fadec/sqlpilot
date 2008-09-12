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
#include "models.h"
#include "logbook.h"

int models_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *treemodel;
  gchar
    *ident=NULL,
    *make=NULL,
    *type=NULL;
  if (gtk_tree_selection_get_selected (selection, &treemodel, &iter)) {
    gtk_tree_model_get(treemodel, &iter,
		       MODELS_COL_IDENT, &ident,
		       MODELS_COL_MAKE, &make,
		       MODELS_COL_TYPE, &type,
		       -1);
    snprintf(show, size, "%s %s - %s", make, ident, type);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }
}

int models_can_delete(GtkTreeSelection *selection)
{
  GtkTreeIter iter;
  GtkTreeModel *treemod;
  gchar *aircraft=NULL;
  long _aircraft=0;

  if (gtk_tree_selection_get_selected(selection, &treemod, &iter)) {
    gtk_tree_model_get(treemod, &iter, MODELS_COL_AIRCRAFT, &aircraft, -1);
    sscanf(aircraft, "%ld", &_aircraft);
  }

  g_free(aircraft);

  return !_aircraft;
}

void models_after_change(Logbook *logbook)
{
  logbook->flights_stale = TRUE;
  logbook->aircraft_stale = TRUE;
}

DBint64 models_write_entries(const gchar *id, Logbook *logbook)
{
  const gchar
    *ident,
    *make,
    *type;
  gboolean
    airplane,
    rotorcraft,
    glider,
    lta,
    poweredlift,
    ppc,
    weightshift,
    heli,
    gyro,
    airship,
    balloon,
    single,
    multi,
    land,
    sea,
    turbine,
    jet,
    highperf,
    retract,
    complex,
    pressurized,
    large,
    sport,
    ultralight,
    footlaunch,
    sim,
    ftd,
    total;
  DBStatement *stmt;

  ident       = gtk_entry_get_text(GTK_ENTRY(logbook->models_ident));
  make        = gtk_entry_get_text(GTK_ENTRY(logbook->models_make));
  type        = gtk_entry_get_text(GTK_ENTRY(logbook->models_type));
  airplane    = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_airplane));
  rotorcraft  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_rotorcraft));
  glider      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_glider));
  lta         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_lta));
  poweredlift = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_poweredlift));
  ppc         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_ppc));
  weightshift = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_weightshift));
  heli        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_heli));
  gyro        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_gyro));
  airship     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_airship));
  balloon     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_balloon));
  single      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_single));
  multi       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_multi));
  land        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_land));
  sea         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_sea));
  turbine     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_turbine));
  jet         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_jet));
  highperf    = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_highperf));
  retract     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_retract));
  complex     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_complex));
  pressurized = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_pressurized));
  large       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_large));
  sport       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_sport));
  ultralight  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_ultralight));
  footlaunch  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_footlaunch));
  sim         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_sim));
  ftd         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_ftd));
  total       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->models_total));
  
  /* Write entries to database */
  if (id) {
    stmt = logbook->models_update;
    db_bind_text(stmt, MODELS_WRITE_ID, id); 
  } else {
    stmt = logbook->models_insert;
  }
  db_bind_nonempty_text_else_null(stmt, MODELS_WRITE_IDENT, ident);
  db_bind_nonempty_text_else_null(stmt, MODELS_WRITE_MAKE, make);
  db_bind_nonempty_text_else_null(stmt, MODELS_WRITE_TYPE, type);
  db_bind_int(stmt, MODELS_WRITE_AIRPLANE, airplane ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_ROTORCRAFT, rotorcraft ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_GLIDER, glider ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_LTA, lta ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_POWEREDLIFT, poweredlift ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_PPC, ppc ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_WEIGHTSHIFT, weightshift ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_HELI, heli ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_GYRO, gyro ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_AIRSHIP, airship ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_BALLOON, balloon ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_SINGLE, single ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_MULTI, multi ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_LAND, land ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_SEA, sea ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_TURBINE, turbine ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_JET, jet ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_HIGHPERF, highperf ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_RETRACT, retract ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_COMPLEX, complex ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_PRESSURIZED, pressurized ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_LARGE, large ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_SPORT, sport ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_ULTRULIGHT, ultralight ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_FOOTLAUNCH, footlaunch ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_SIM, sim ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_FTD, ftd ? 1 : 0);
  db_bind_int(stmt, MODELS_WRITE_TOTAL, total ? 1 : 0);

  db_stp_res_clr(stmt);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(logbook->db);
  }
}

void models_load_selection(Logbook *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *treemod;

  gchar
    *id=NULL,
    *ident=NULL,
    *make=NULL,
    *type=NULL,
    *airplane=NULL,
    *rotorcraft=NULL,
    *glider=NULL,
    *lta=NULL,
    *poweredlift=NULL,
    *ppc=NULL,
    *weightshift=NULL,
    *heli=NULL,
    *gyro=NULL,
    *airship=NULL,
    *balloon=NULL,
    *single=NULL,
    *multi=NULL,
    *land=NULL,
    *sea=NULL,
    *turbine=NULL,
    *jet=NULL,
    *highperf=NULL,
    *retract=NULL,
    *complex=NULL,
    *pressurized=NULL,
    *large=NULL,
    *sport=NULL,
    *ultralight=NULL,
    *footlaunch=NULL,
    *sim=NULL,
    *ftd=NULL,
    *total=NULL;

  if (gtk_tree_selection_get_selected (logb->models_selection, &treemod, &iter)) {
    gtk_tree_model_get(treemod, &iter,
		       MODELS_COL_ID, &id,
		       MODELS_COL_IDENT, &ident,
		       MODELS_COL_MAKE, &make,
		       MODELS_COL_TYPE, &type,
		       MODELS_COL_AIRPLANE, &airplane,
		       MODELS_COL_ROTORCRAFT, &rotorcraft,
		       MODELS_COL_GLIDER, &glider,
		       MODELS_COL_LTA, &lta,
		       MODELS_COL_POWEREDLIFT, &poweredlift,
		       MODELS_COL_PPC, &ppc,
		       MODELS_COL_WEIGHTSHIFT, &weightshift,
		       MODELS_COL_HELI, &heli,
		       MODELS_COL_GYRO, &gyro,
		       MODELS_COL_AIRSHIP, &airship,
		       MODELS_COL_BALLOON, &balloon,
		       MODELS_COL_SINGLE, &single,
		       MODELS_COL_MULTI, &multi,
		       MODELS_COL_LAND, &land,
		       MODELS_COL_SEA, &sea,
		       MODELS_COL_TURBINE, &turbine,
		       MODELS_COL_JET, &jet,
		       MODELS_COL_HIGHPERF, &highperf,
		       MODELS_COL_RETRACT, &retract,
		       MODELS_COL_COMPLEX, &complex,
		       MODELS_COL_PRESSURIZED, &pressurized,
		       MODELS_COL_LARGE, &large,
		       MODELS_COL_SPORT, &sport,
		       MODELS_COL_ULTRULIGHT, &ultralight,
		       MODELS_COL_FOOTLAUNCH, &footlaunch,
		       MODELS_COL_SIM, &sim,
		       MODELS_COL_FTD, &ftd, 
		       MODELS_COL_TOTAL, &total,
		       -1);
  }

  gtk_entry_set_text(GTK_ENTRY(logb->models_ident), EMPTY_IF_NULL(ident));
  gtk_entry_set_text(GTK_ENTRY(logb->models_make), EMPTY_IF_NULL(make));
  gtk_entry_set_text(GTK_ENTRY(logb->models_type), EMPTY_IF_NULL(type));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_airplane), str_bool(airplane));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_rotorcraft), str_bool(rotorcraft));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_glider), str_bool(glider));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_lta), str_bool(lta));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_poweredlift), str_bool(poweredlift));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_ppc), str_bool(ppc));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_weightshift), str_bool(weightshift));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_heli), str_bool(heli));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_gyro), str_bool(gyro));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_airship), str_bool(airship));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_balloon), str_bool(balloon));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_single), str_bool(single));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_multi), str_bool(multi));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_land), str_bool(land));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_sea), str_bool(sea));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_turbine), str_bool(turbine));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_jet), str_bool(jet));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_highperf), str_bool(highperf));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_retract), str_bool(retract));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_complex), str_bool(complex));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_pressurized), str_bool(pressurized));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_large), str_bool(large));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_sport), str_bool(sport));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_ultralight), str_bool(ultralight));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_footlaunch), str_bool(footlaunch));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_sim), str_bool(sim));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_ftd), str_bool(ftd));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->models_total), str_bool(total));

  g_free(id);
  g_free(ident);
  g_free(make);
  g_free(type);
  g_free(airplane);
  g_free(rotorcraft);
  g_free(glider);
  g_free(lta);
  g_free(poweredlift);
  g_free(ppc);
  g_free(weightshift);
  g_free(heli);
  g_free(gyro);
  g_free(airship);
  g_free(balloon);
  g_free(single);
  g_free(multi);
  g_free(land);
  g_free(sea);
  g_free(turbine);
  g_free(jet);
  g_free(highperf);
  g_free(retract);
  g_free(complex);
  g_free(pressurized);
  g_free(large);
  g_free(sport);
  g_free(ultralight);
  g_free(footlaunch);
  g_free(sim);
  g_free(ftd);
  g_free(total);  
}

void models_refresh(Logbook *logbook)
{
    logbook->models_stale = FALSE;
    store_repopulate_from_stmt(GTK_LIST_STORE(logbook->models_treemodel), logbook->models_select_all);
    models_load_selection(logbook);
}

int models_ident_validate(Logbook *logbook)
{
  gchar *id=NULL;
  const gchar *ident;

  id = get_text_from_tree_selection(logbook->models_selection, COL_ID);
  ident = gtk_entry_get_text(GTK_ENTRY(logbook->models_ident));

  if (unique_but_for(logbook->db, "models", "ident", ident, "id", EMPTY_IF_NULL(id))) {
    logbook->models_ident_error = 0;
  } else {
    logbook->models_ident_error = 1;
  }

  g_free(id);

  return logbook->models_ident_error;
}

int models_error(Logbook *logbook)
{
  const gchar *ident;
  ident = gtk_entry_get_text(GTK_ENTRY(logbook->models_ident));
  return (!*ident || logbook->models_ident_error);
}
