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
#include "logbook.h"
#include "flights.h"
#include "roles.h"
#include "aircraft.h"
#include "types.h"
#include "airports.h"
#include "reports.h"
#include "summaries.h"
#include "cb/cb.h"

Logbook *logbook_new(const char *filename)
{
  Logbook         *logbook;

  /* Open Database */
  logbook = g_slice_new0(Logbook);
  if (!(logbook->db = db_open(filename))) {
    barf ("Couldn't open database.");
    return NULL;
  }
  logbook->db_filename = g_strdup(filename);

  /* Build UI from XML */
#ifdef USING_GTK_BUILDER
  GtkBuilder       *builder;
  GError           *err;

  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, UI_XML_FILE, &err) == 0) {
    barf (err->message);
    g_error_free (err);
    return NULL;
  }

  /* Connect Signals */
  gtk_builder_connect_signals(builder, logbook);

#else
  GladeXML *gxml;
  gxml = glade_xml_new(UI_XML_FILE, NULL, NULL);
  if (!gxml) {
    barf ("Can't load glade xml file\n");
    return NULL;
  }
#endif

  /* Set DB statements */
  logbook->flights_select_all   = db_prep(logbook->db, FLIGHTS_SELECT FLIGHTS_ORDER ";");
  logbook->flights_select_by_id = db_prep(logbook->db, FLIGHTS_SELECT FLIGHTS_WHERE_ID);
  logbook->flights_insert       = db_prep(logbook->db, FLIGHTS_INSERT);
  logbook->flights_update       = db_prep(logbook->db, FLIGHTS_UPDATE);
  logbook->flights_delete       = db_prep(logbook->db, FLIGHTS_DELETE);
  logbook->flights_aircraft_fleetno_from_tail = db_prep(logbook->db, "SELECT fleetno FROM aircraft WHERE tail = ? LIMIT 1;");
  logbook->flights_aircraft_tail_from_fleetno = db_prep(logbook->db, "SELECT tail FROM aircraft WHERE fleetno = ? LIMIT 1;");
  logbook->flights_tz_of_airport_iata  = db_prep(logbook->db, AIRPORTS_SELECT_TZONE_BY_IATA);
  logbook->flights_tz_of_airport_icao  = db_prep(logbook->db, AIRPORTS_SELECT_TZONE_BY_ICAO);
  logbook->flights_iata_from_icao = db_prep(logbook->db, AIRPORTS_SELECT_IATA_BY_ICAO);
  logbook->flights_icao_from_iata = db_prep(logbook->db, AIRPORTS_SELECT_ICAO_BY_IATA);

  logbook->roles_select_all     = db_prep(logbook->db, ROLES_SELECT ROLES_GROUP_BY ";");
  logbook->roles_select_by_id   = db_prep(logbook->db, ROLES_SELECT ROLES_WHERE_ID ROLES_GROUP_BY ";");
  logbook->roles_insert         = db_prep(logbook->db, ROLES_INSERT);
  logbook->roles_update         = db_prep(logbook->db, ROLES_UPDATE);
  logbook->roles_delete         = db_prep(logbook->db, ROLES_DELETE);

  logbook->aircraft_select_all     = db_prep(logbook->db, AIRCRAFT_SELECT AIRCRAFT_GROUP_BY ";");
  logbook->aircraft_select_by_id   = db_prep(logbook->db, AIRCRAFT_SELECT AIRCRAFT_WHERE_ID AIRCRAFT_GROUP_BY ";");
  logbook->aircraft_insert         = db_prep(logbook->db, AIRCRAFT_INSERT);
  logbook->aircraft_update         = db_prep(logbook->db, AIRCRAFT_UPDATE);
  logbook->aircraft_delete         = db_prep(logbook->db, AIRCRAFT_DELETE);
  logbook->aircraft_count_flights  = db_prep(logbook->db, AIRCRAFT_COUNT_FLIGHTS);

  logbook->types_select_all     = db_prep(logbook->db, TYPES_SELECT TYPES_GROUP_BY ";");
  logbook->types_select_by_id   = db_prep(logbook->db, TYPES_SELECT TYPES_WHERE_ID TYPES_GROUP_BY ";");
  logbook->types_insert         = db_prep(logbook->db, TYPES_INSERT);
  logbook->types_update         = db_prep(logbook->db, TYPES_UPDATE);
  logbook->types_delete         = db_prep(logbook->db, TYPES_DELETE);

  logbook->airports_select_all     = db_prep(logbook->db, AIRPORTS_SELECT AIRPORTS_GROUP_BY ";");
  logbook->airports_select_by_id   = db_prep(logbook->db, AIRPORTS_SELECT AIRPORTS_WHERE_ID ";");
  logbook->airports_insert         = db_prep(logbook->db, AIRPORTS_INSERT);
  logbook->airports_update         = db_prep(logbook->db, AIRPORTS_UPDATE);
  logbook->airports_delete         = db_prep(logbook->db, AIRPORTS_DELETE);

  logbook->registry_select_by_path_key = db_prep(logbook->db, "SELECT value FROM Registry WHERE path = ? AND key = ?;");
  logbook->registry_insert             = db_prep(logbook->db, "INSERT INTO Registry (value, path, key) VALUES (?, ?, ?);");
  logbook->registry_update             = db_prep(logbook->db, "UPDATE Registry SET value = ? WHERE path = ? AND key = ?;");
  logbook->registry_delete             = db_prep(logbook->db, "DELETE FROM Registry WHERE path = ? AND key = ?;");

  
  pull_widget(window);
  pull_widget(flights_where);
  pull_widget(flights_refresh);
  pull_widget(flights_query_progress);
  pull_widget(flights_results_summary);
  pull_widget(flights_sw);
  pull_widget(flights_aircraft); /* Remove this */
  pull_widget(flights_tail);
  pull_widget(flights_fleetno);
  pull_widget(flights_aircraft_utilized);
  pull_widget(flights_utc);
  pull_widget(flights_utc_lbl);
  pull_widget(flights_icao_toggle);
  pull_widget(flights_icao_toggle_lbl);
  pull_widget(flights_fleetno_toggle);
  pull_widget(flights_fleetno_toggle_lbl);
  pull_widget(flights_date);
  pull_widget(flights_leg);
  pull_widget(flights_role);
  pull_widget(flights_role_utilized);
  pull_widget(flights_dep);
  pull_widget(flights_dep_utilized);
  pull_widget(flights_arr);
  pull_widget(flights_arr_utilized);
  pull_widget(flights_aout);
  pull_widget(flights_ain);
  pull_widget(flights_dur);
  pull_widget(flights_night);
  pull_widget(flights_inst);
  pull_widget(flights_siminst);
  pull_widget(flights_hold);
  pull_widget(flights_aprch);
  pull_widget(flights_xc);
  pull_widget(flights_dland);
  pull_widget(flights_nland);
  pull_widget(flights_crew);
  pull_widget(flights_notes);
  pull_widget(flights_fltno);
  pull_widget(flights_sout);
  pull_widget(flights_sin);
  pull_widget(flights_sdur);
  pull_widget(flights_trip);
  pull_widget(flights_tripdate);
  pull_widget(flights_view_tail);
  pull_widget(flights_view_fleetno);
  pull_widget(flights_view_type);
  pull_widget(flights_view_date);
  pull_widget(flights_view_leg);
  pull_widget(flights_view_dist);
  pull_widget(flights_view_role);
  pull_widget(flights_view_dep);
  pull_widget(flights_view_arr);
  pull_widget(flights_view_aout);
  pull_widget(flights_view_ain);
  pull_widget(flights_view_dur);
  pull_widget(flights_view_night);
  pull_widget(flights_view_inst);
  pull_widget(flights_view_siminst);
  pull_widget(flights_view_hold);
  pull_widget(flights_view_aprch);
  pull_widget(flights_view_aprchn);
  pull_widget(flights_view_xc);
  pull_widget(flights_view_dland);
  pull_widget(flights_view_nland);
  pull_widget(flights_view_crew);
  pull_widget(flights_view_crewn);
  pull_widget(flights_view_notes);
  pull_widget(flights_view_notesn);
  pull_widget(flights_view_fltno);
  pull_widget(flights_view_sout);
  pull_widget(flights_view_sin);
  pull_widget(flights_view_sdur);
  pull_widget(flights_view_trip);
  pull_widget(flights_view_tripdate);
  pull_widget(flights_view_over);
  pull_widget(flights_new_btn);
  pull_widget(flights_save_btn);
  pull_widget(flights_armdel_btn);
  pull_widget(flights_del_btn);
  pull_widget(flights_todel_lbl);
  pull_widget(roles_sw);
  pull_widget(roles_ident);
  pull_widget(roles_ident_valid_wart);
  pull_widget(roles_name);
  pull_widget(roles_pic);
  pull_widget(roles_sic);
  pull_widget(roles_fe);
  pull_widget(roles_solo);
  pull_widget(roles_dual);
  pull_widget(roles_instruct);
  pull_widget(roles_total);
  pull_widget(roles_new_btn);
  pull_widget(roles_save_btn);
  pull_widget(roles_armdel_btn);
  pull_widget(roles_del_btn);
  pull_widget(roles_todel_lbl);
  pull_widget(aircraft_sw);
  pull_widget(aircraft_tail);
  pull_widget(aircraft_tail_valid_wart);
  pull_widget(aircraft_fleetno);
  pull_widget(aircraft_fleetno_valid_wart);
  pull_widget(aircraft_type);
  pull_widget(aircraft_notes);
  pull_widget(aircraft_new_btn);
  pull_widget(aircraft_save_btn);
  pull_widget(aircraft_armdel_btn);
  pull_widget(aircraft_del_btn);
  pull_widget(aircraft_todel_lbl);
  pull_widget(types_sw);
  pull_widget(types_ident);
  pull_widget(types_ident_valid_wart);
  pull_widget(types_make);
  pull_widget(types_model);
  pull_widget(types_airplane);
  pull_widget(types_rotorcraft);
  pull_widget(types_glider);
  pull_widget(types_lta);
  pull_widget(types_poweredlift);
  pull_widget(types_ppc);
  pull_widget(types_weightshift);
  pull_widget(types_heli);
  pull_widget(types_gyro);
  pull_widget(types_airship);
  pull_widget(types_balloon);
  pull_widget(types_single);
  pull_widget(types_multi);
  pull_widget(types_land);
  pull_widget(types_sea);
  pull_widget(types_turbine);
  pull_widget(types_jet);
  pull_widget(types_highperf);
  pull_widget(types_retract);
  pull_widget(types_complex);
  pull_widget(types_pressurized);
  pull_widget(types_large);
  pull_widget(types_sport);
  pull_widget(types_ultralight);
  pull_widget(types_footlaunch);
  pull_widget(types_sim);
  pull_widget(types_ftd);
  pull_widget(types_total);
  pull_widget(types_new_btn);
  pull_widget(types_save_btn);
  pull_widget(types_armdel_btn);
  pull_widget(types_del_btn);
  pull_widget(types_todel_lbl);
  pull_widget(airports_sw);
  pull_widget(airports_iata);
  pull_widget(airports_iata_valid_wart);
  pull_widget(airports_icao);
  pull_widget(airports_icao_valid_wart);
  pull_widget(airports_name);
  pull_widget(airports_lat);
  pull_widget(airports_lon);
  pull_widget(airports_elev);
  pull_widget(airports_city);
  pull_widget(airports_province);
  pull_widget(airports_country);
  pull_widget(airports_tzone);
  pull_widget(airports_notes);
  pull_widget(airports_new_btn);
  pull_widget(airports_save_btn);
  pull_widget(airports_armdel_btn);
  pull_widget(airports_del_btn);
  pull_widget(airports_todel_lbl);
  pull_widget(reports_title);
  pull_widget(reports_refresh);
  pull_widget(reports_query_progress);
  pull_widget(reports_results_summary);
  pull_widget(reports_sql_text);
  pull_widget(reports_save_btn);
  pull_widget(reports_armdel_btn);
  pull_widget(reports_del_btn);
  pull_widget(summaries_select_summary);
  pull_widget(summaries_sw);
  pull_widget(summaries_parameters);
  
  /* Add treeview */
  flights_build_store_view(logbook);

  store_build_query_stmt_widget(logbook->roles_select_all, NULL, &logbook->roles_treeview, &logbook->roles_treemodel);
  gtk_widget_show_all(logbook->roles_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->roles_sw), logbook->roles_treeview);

  store_build_query_stmt_widget(logbook->aircraft_select_all, NULL, &logbook->aircraft_treeview, &logbook->aircraft_treemodel);
  gtk_widget_show_all(logbook->aircraft_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->aircraft_sw), logbook->aircraft_treeview);

  store_build_query_stmt_widget(logbook->types_select_all, NULL, &logbook->types_treeview, &logbook->types_treemodel);
  gtk_widget_show_all(logbook->types_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->types_sw), logbook->types_treeview);

  store_build_query_stmt_widget(logbook->airports_select_all, NULL, &logbook->airports_treeview, &logbook->airports_treemodel);
  gtk_widget_show_all(logbook->airports_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->airports_sw), logbook->airports_treeview);

  /* Setup treeview callbacks */
  logbook->flights_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (logbook->flights_treeview));
  gtk_tree_selection_set_mode (logbook->flights_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (logbook->flights_selection), "changed",
		    G_CALLBACK (on_flights_selection_changed),
		    logbook);
  logbook->roles_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (logbook->roles_treeview));
  gtk_tree_selection_set_mode (logbook->roles_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (logbook->roles_selection), "changed",
		    G_CALLBACK (on_roles_selection_changed),
		    logbook);
  logbook->aircraft_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (logbook->aircraft_treeview));
  gtk_tree_selection_set_mode (logbook->aircraft_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (logbook->aircraft_selection), "changed",
		    G_CALLBACK (on_aircraft_selection_changed),
		    logbook);
  logbook->types_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (logbook->types_treeview));
  gtk_tree_selection_set_mode (logbook->types_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (logbook->types_selection), "changed",
		    G_CALLBACK (on_types_selection_changed),
		    logbook);
  logbook->airports_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (logbook->airports_treeview));
  gtk_tree_selection_set_mode (logbook->airports_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (logbook->airports_selection), "changed",
		    G_CALLBACK (on_airports_selection_changed),
		    logbook);

  /* Setup text view buffer callbacks */
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->flights_crew))), "changed",
		   G_CALLBACK(on_flights_crew_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->flights_notes))), "changed",
		   G_CALLBACK(on_flights_notes_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->aircraft_notes))), "changed",
		   G_CALLBACK(on_aircraft_notes_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->airports_notes))), "changed",
		   G_CALLBACK(on_airports_notes_changed), logbook);

  /* Edit Controls */
  logbook->flights_edctrl                 = &logbook->_flights_edctrl;
  logbook->flights_edctrl->edstate        = EDSTATE_EMPTY;
  logbook->flights_edctrl->new_btn        = logbook->flights_new_btn;
  logbook->flights_edctrl->save_btn       = logbook->flights_save_btn;
  logbook->flights_edctrl->armdel_btn     = logbook->flights_armdel_btn;
  logbook->flights_edctrl->todel_lbl      = logbook->flights_todel_lbl;
  logbook->flights_edctrl->del_btn        = logbook->flights_del_btn;
  logbook->flights_edctrl->selection      = logbook->flights_selection;
  logbook->flights_edctrl->selection_show = flights_selection_show;
  logbook->flights_edctrl->can_delete     = flights_can_delete;
  logbook->flights_edctrl->delete_stmt    = logbook->flights_delete;
  logbook->flights_edctrl->select_by_id_stmt    = logbook->flights_select_by_id;
  edctrl_register_save(logbook->flights_edctrl, flights_write_entries, logbook);
  edctrl_register_after_change(logbook->flights_edctrl, flights_after_change, logbook);
  edctrl_register_load_selection(logbook->flights_edctrl, flights_load_selection, logbook);
  edctrl_register_validator(logbook->flights_edctrl, flights_error, logbook);

  logbook->roles_edctrl                 = &logbook->_roles_edctrl;
  logbook->roles_edctrl->edstate        = EDSTATE_EMPTY;
  logbook->roles_edctrl->new_btn        = logbook->roles_new_btn;
  logbook->roles_edctrl->save_btn       = logbook->roles_save_btn;
  logbook->roles_edctrl->armdel_btn     = logbook->roles_armdel_btn;
  logbook->roles_edctrl->todel_lbl      = logbook->roles_todel_lbl;
  logbook->roles_edctrl->del_btn        = logbook->roles_del_btn;
  logbook->roles_edctrl->selection      = logbook->roles_selection;
  logbook->roles_edctrl->selection_show = roles_selection_show;
  logbook->roles_edctrl->can_delete     = roles_can_delete;
  logbook->roles_edctrl->delete_stmt    = logbook->roles_delete;
  logbook->roles_edctrl->select_by_id_stmt    = logbook->roles_select_by_id;
  edctrl_register_save(logbook->roles_edctrl, roles_write_entries, logbook);
  edctrl_register_after_change(logbook->roles_edctrl, roles_after_change, logbook);
  edctrl_register_load_selection(logbook->roles_edctrl, roles_load_selection, logbook);
  edctrl_register_validator(logbook->roles_edctrl, roles_error, logbook);

  logbook->aircraft_edctrl                        = &logbook->_aircraft_edctrl;
  logbook->aircraft_edctrl->edstate               = EDSTATE_EMPTY;
  logbook->aircraft_edctrl->new_btn               = logbook->aircraft_new_btn;
  logbook->aircraft_edctrl->save_btn              = logbook->aircraft_save_btn;
  logbook->aircraft_edctrl->armdel_btn            = logbook->aircraft_armdel_btn;
  logbook->aircraft_edctrl->todel_lbl             = logbook->aircraft_todel_lbl;
  logbook->aircraft_edctrl->del_btn               = logbook->aircraft_del_btn;
  logbook->aircraft_edctrl->selection             = logbook->aircraft_selection;
  logbook->aircraft_edctrl->selection_show        = aircraft_selection_show;
  logbook->aircraft_edctrl->can_delete            = aircraft_can_delete;
  logbook->aircraft_edctrl->delete_stmt           = logbook->aircraft_delete;
  logbook->aircraft_edctrl->select_by_id_stmt     = logbook->aircraft_select_by_id;
  edctrl_register_save(logbook->aircraft_edctrl, aircraft_write_entries, logbook);
  edctrl_register_after_change(logbook->aircraft_edctrl, aircraft_after_change, logbook);
  edctrl_register_load_selection(logbook->aircraft_edctrl, aircraft_load_selection, logbook);
  edctrl_register_validator(logbook->aircraft_edctrl, aircraft_error, logbook);

  logbook->types_edctrl                 = &logbook->_types_edctrl;
  logbook->types_edctrl->edstate        = EDSTATE_EMPTY;
  logbook->types_edctrl->new_btn        = logbook->types_new_btn;
  logbook->types_edctrl->save_btn       = logbook->types_save_btn;
  logbook->types_edctrl->armdel_btn     = logbook->types_armdel_btn;
  logbook->types_edctrl->todel_lbl      = logbook->types_todel_lbl;
  logbook->types_edctrl->del_btn        = logbook->types_del_btn;
  logbook->types_edctrl->selection      = logbook->types_selection;
  logbook->types_edctrl->selection_show = types_selection_show;
  logbook->types_edctrl->can_delete     = types_can_delete;
  logbook->types_edctrl->delete_stmt    = logbook->types_delete;
  logbook->types_edctrl->select_by_id_stmt    = logbook->types_select_by_id;
  edctrl_register_save(logbook->types_edctrl, types_write_entries, logbook);
  edctrl_register_after_change(logbook->types_edctrl, types_after_change, logbook);
  edctrl_register_load_selection(logbook->types_edctrl, types_load_selection, logbook);
  edctrl_register_validator(logbook->types_edctrl, types_error, logbook);

  logbook->airports_edctrl                 = &logbook->_airports_edctrl;
  logbook->airports_edctrl->edstate        = EDSTATE_EMPTY;
  logbook->airports_edctrl->new_btn        = logbook->airports_new_btn;
  logbook->airports_edctrl->save_btn       = logbook->airports_save_btn;
  logbook->airports_edctrl->armdel_btn     = logbook->airports_armdel_btn;
  logbook->airports_edctrl->todel_lbl      = logbook->airports_todel_lbl;
  logbook->airports_edctrl->del_btn        = logbook->airports_del_btn;
  logbook->airports_edctrl->selection      = logbook->airports_selection;
  logbook->airports_edctrl->selection_show = airports_selection_show;
  logbook->airports_edctrl->can_delete     = airports_can_delete;
  logbook->airports_edctrl->delete_stmt    = logbook->airports_delete;
  logbook->airports_edctrl->select_by_id_stmt    = logbook->airports_select_by_id;
  edctrl_register_save(logbook->airports_edctrl, airports_write_entries, logbook);
  edctrl_register_after_change(logbook->airports_edctrl, airports_after_change, logbook);
  edctrl_register_load_selection(logbook->airports_edctrl, airports_load_selection, logbook);
  edctrl_register_validator(logbook->airports_edctrl, airports_error, logbook);

  
  logbook->summaries_parameter_pane = &logbook->_summaries_parameter_pane;
  logbook->summaries_parameter_pane->container = logbook->summaries_parameters;
  summaries_init(logbook);

#ifdef USING_GTK_BUILDER
  g_object_unref (G_OBJECT (builder));
#else
  g_object_unref (G_OBJECT(gxml));
#endif

  flights_restore_options(logbook);

  logbook->flights_stale = 0;
  flights_refresh(logbook);
  logbook->roles_stale = 1;
  logbook->aircraft_stale = 1;
  logbook->types_stale = 1;
  logbook->airports_stale = 1;
  
  flights_load_selection(logbook);
  roles_load_selection(logbook);
  aircraft_load_selection(logbook);
  types_load_selection(logbook);
  aircraft_load_selection(logbook);

  return logbook;
}

void logbook_save_options(Logbook *logbook)
{
  flights_save_options(logbook);
}

void logbook_finalize(Logbook *logbook)
{
  g_free(logbook->db_filename);
  g_slice_free(Logbook, logbook);
}

char *registry_get_text(Logbook *logbook, const char *path, const char *key)
{
  char *value = NULL;
  DBStatement *stmt = logbook->registry_select_by_path_key;
  db_bind_text(stmt, 1, path);
  db_bind_text(stmt, 2, key);
  if (db_step(stmt) == DB_ROW) {
    strncpy(logbook->registry_value, (char*)db_column_text(stmt, 0), REGISTRY_BUF_VALUE);
    logbook->registry_value[REGISTRY_BUF_VALUE-1] = '\0';
    value = logbook->registry_value;
  }
  db_reset(stmt);
  db_clear_bindings(stmt);

  return value;
}

int registry_get_int(Logbook *logbook, const char *path, const char *key)
{
  int value = 0;
  DBStatement *stmt = logbook->registry_select_by_path_key;
  db_bind_text(stmt, 1, path);
  db_bind_text(stmt, 2, key);
  if (db_step(stmt) == DB_ROW) {
    value = db_column_int(stmt, 0);
  }
  db_reset(stmt);
  db_clear_bindings(stmt);

  return value;
}

int registry_key_exists(Logbook *logbook, const char *path, const char *key)
{
  int value = 1;
  DBStatement *stmt = logbook->registry_select_by_path_key;
  db_bind_text(stmt, 1, path);
  db_bind_text(stmt, 2, key);
  if (db_step(stmt) == DB_DONE) {
    value = 0;
  }
  db_reset(stmt);
  db_clear_bindings(stmt);

  return value;
}

void registry_set_int(Logbook *logbook, const char *path, const char *key, int value)
{
  DBStatement *stmt;

  if (registry_key_exists(logbook, path, key)) {
    stmt = logbook->registry_update;
  } else {
    stmt = logbook->registry_insert;
  }
  db_bind_int(stmt, 1, value);
  db_bind_text(stmt, 2, path);
  db_bind_text(stmt, 3, key);
  db_stp_res_clr(stmt);
}
