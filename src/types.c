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

#include "types.h"

int types_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *treemodel;
  gchar
    *ident=NULL,
    *make=NULL,
    *model=NULL;
  if (gtk_tree_selection_get_selected (selection, &treemodel, &iter)) {
    gtk_tree_model_get(treemodel, &iter,
		       TYPES_COL_IDENT, &ident,
		       TYPES_COL_MAKE, &make,
		       TYPES_COL_MODEL, &model,
		       -1);
    snprintf(show, size, "%s - %s %s", ident, make, model);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }
}

int types_can_delete(GtkTreeSelection *selection)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *aircraft=NULL;
  long _aircraft=0;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, TYPES_COL_AIRCRAFT, &aircraft, -1);
    sscanf(aircraft, "%ld", &_aircraft);
  }

  g_free(aircraft);

  return !_aircraft;
}

void types_after_change(Sqlpilot *sqlpilot)
{
  sqlpilot->flights_stale = TRUE;
  sqlpilot->aircraft_stale = TRUE;
}

DBint64 types_write_entries(const gchar *id, Sqlpilot *sqlpilot)
{
  const gchar
    *ident,
    *make,
    *model;
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

  ident       = gtk_entry_get_text(GTK_ENTRY(sqlpilot->types_ident));
  make        = gtk_entry_get_text(GTK_ENTRY(sqlpilot->types_make));
  model       = gtk_entry_get_text(GTK_ENTRY(sqlpilot->types_model));
  airplane    = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_airplane));
  rotorcraft  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_rotorcraft));
  glider      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_glider));
  lta         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_lta));
  poweredlift = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_poweredlift));
  ppc         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_ppc));
  weightshift = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_weightshift));
  heli        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_heli));
  gyro        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_gyro));
  airship     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_airship));
  balloon     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_balloon));
  single      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_single));
  multi       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_multi));
  land        = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_land));
  sea         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_sea));
  turbine     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_turbine));
  jet         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_jet));
  highperf    = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_highperf));
  retract     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_retract));
  complex     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_complex));
  pressurized = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_pressurized));
  large       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_large));
  sport       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_sport));
  ultralight  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_ultralight));
  footlaunch  = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_footlaunch));
  sim         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_sim));
  ftd         = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_ftd));
  total       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->types_total));
  
  /* Write entries to database */
  if (id) {
    stmt = sqlpilot->types_update;
    db_bind_text(stmt, TYPES_WRITE_ID, id); 
  } else {
    stmt = sqlpilot->types_insert;
  }
  db_bind_text(stmt, TYPES_WRITE_IDENT, ident);
  db_bind_text(stmt, TYPES_WRITE_MAKE, make);
  db_bind_text(stmt, TYPES_WRITE_MODEL, model);
  db_bind_int(stmt, TYPES_WRITE_AIRPLANE, airplane ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_ROTORCRAFT, rotorcraft ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_GLIDER, glider ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_LTA, lta ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_POWEREDLIFT, poweredlift ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_PPC, ppc ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_WEIGHTSHIFT, weightshift ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_HELI, heli ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_GYRO, gyro ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_AIRSHIP, airship ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_BALLOON, balloon ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_SINGLE, single ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_MULTI, multi ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_LAND, land ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_SEA, sea ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_TURBINE, turbine ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_JET, jet ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_HIGHPERF, highperf ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_RETRACT, retract ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_COMPLEX, complex ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_PRESSURIZED, pressurized ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_LARGE, large ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_SPORT, sport ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_ULTRULIGHT, ultralight ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_FOOTLAUNCH, footlaunch ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_SIM, sim ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_FTD, ftd ? 1 : 0);
  db_bind_int(stmt, TYPES_WRITE_TOTAL, total ? 1 : 0);

  db_stp_res_clr(stmt);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(sqlpilot->db);
  }
}

void types_load_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *tmodel;

  gchar
    *id=NULL,
    *ident=NULL,
    *make=NULL,
    *model=NULL,
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

  if (gtk_tree_selection_get_selected (logb->types_selection, &tmodel, &iter)) {
    gtk_tree_model_get(tmodel, &iter,
		       TYPES_COL_ID, &id,
		       TYPES_COL_IDENT, &ident,
		       TYPES_COL_MAKE, &make,
		       TYPES_COL_MODEL, &model,
		       TYPES_COL_AIRPLANE, &airplane,
		       TYPES_COL_ROTORCRAFT, &rotorcraft,
		       TYPES_COL_GLIDER, &glider,
		       TYPES_COL_LTA, &lta,
		       TYPES_COL_POWEREDLIFT, &poweredlift,
		       TYPES_COL_PPC, &ppc,
		       TYPES_COL_WEIGHTSHIFT, &weightshift,
		       TYPES_COL_HELI, &heli,
		       TYPES_COL_GYRO, &gyro,
		       TYPES_COL_AIRSHIP, &airship,
		       TYPES_COL_BALLOON, &balloon,
		       TYPES_COL_SINGLE, &single,
		       TYPES_COL_MULTI, &multi,
		       TYPES_COL_LAND, &land,
		       TYPES_COL_SEA, &sea,
		       TYPES_COL_TURBINE, &turbine,
		       TYPES_COL_JET, &jet,
		       TYPES_COL_HIGHPERF, &highperf,
		       TYPES_COL_RETRACT, &retract,
		       TYPES_COL_COMPLEX, &complex,
		       TYPES_COL_PRESSURIZED, &pressurized,
		       TYPES_COL_LARGE, &large,
		       TYPES_COL_SPORT, &sport,
		       TYPES_COL_ULTRULIGHT, &ultralight,
		       TYPES_COL_FOOTLAUNCH, &footlaunch,
		       TYPES_COL_SIM, &sim,
		       TYPES_COL_FTD, &ftd, 
		       TYPES_COL_TOTAL, &total,
		       -1);
  }

  gtk_entry_set_text(GTK_ENTRY(logb->types_ident), EMPTY_IF_NULL(ident));
  gtk_entry_set_text(GTK_ENTRY(logb->types_make), EMPTY_IF_NULL(make));
  gtk_entry_set_text(GTK_ENTRY(logb->types_model), EMPTY_IF_NULL(model));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_airplane), str_bool(airplane));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_rotorcraft), str_bool(rotorcraft));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_glider), str_bool(glider));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_lta), str_bool(lta));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_poweredlift), str_bool(poweredlift));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_ppc), str_bool(ppc));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_weightshift), str_bool(weightshift));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_heli), str_bool(heli));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_gyro), str_bool(gyro));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_airship), str_bool(airship));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_balloon), str_bool(balloon));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_single), str_bool(single));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_multi), str_bool(multi));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_land), str_bool(land));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_sea), str_bool(sea));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_turbine), str_bool(turbine));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_jet), str_bool(jet));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_highperf), str_bool(highperf));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_retract), str_bool(retract));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_complex), str_bool(complex));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_pressurized), str_bool(pressurized));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_large), str_bool(large));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_sport), str_bool(sport));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_ultralight), str_bool(ultralight));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_footlaunch), str_bool(footlaunch));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_sim), str_bool(sim));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_ftd), str_bool(ftd));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->types_total), str_bool(total));

  g_free(id);
  g_free(ident);
  g_free(make);
  g_free(model);
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

void types_refresh(Sqlpilot *sqlpilot)
{
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->types_treemodel), sqlpilot->types_select_all);
    types_load_selection(sqlpilot);
    sqlpilot->types_stale = FALSE;
}
