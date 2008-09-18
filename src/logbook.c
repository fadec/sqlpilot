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
#include "store.h"
#include "flights.h"
#include "roles.h"
#include "aircraft.h"
#include "models.h"
#include "airports.h"
#include "reports.h"
#include "summaries.h"
#include "import.h"
#include "cb/cb.h"

ColumnPref columns[] = {
  { "AIn"		, COLUMN_SORT_NUM	 , TRUE },
  { "AOut"		, COLUMN_SORT_NUM	 , TRUE },
  { "Aircraft"          , COLUMN_SORT_NUM        , TRUE },
  { "Airplane"		, COLUMN_SORT_NUM	 , TRUE },
  { "Airship"		, COLUMN_SORT_NUM	 , TRUE },
  { "Aprch"		, COLUMN_SORT_STR	 , TRUE },
  { "AprchN"		, COLUMN_SORT_NUM	 , TRUE },
  { "Arr"		, COLUMN_SORT_STR	 , TRUE },
  { "ArrIATA"		, COLUMN_SORT_STR	 , TRUE },
  { "ArrICAO"		, COLUMN_SORT_STR	 , TRUE },
  { "Balloon"		, COLUMN_SORT_NUM	 , TRUE },
  { "City"		, COLUMN_SORT_STR	 , TRUE },
  { "Complex"		, COLUMN_SORT_NUM	 , TRUE },
  { "Country"		, COLUMN_SORT_STR	 , TRUE },
  { "Crew"		, COLUMN_SORT_STR	 , TRUE },
  { "CrewN"		, COLUMN_SORT_NUM	 , TRUE },
  { "Crw"		, COLUMN_SORT_NUM	 , TRUE },
  { "DLand"		, COLUMN_SORT_NUM	 , TRUE },
  { "Date"		, COLUMN_SORT_STR	 , TRUE },
  { "Dep"		, COLUMN_SORT_STR	 , TRUE },
  { "DepIATA"		, COLUMN_SORT_STR	 , TRUE },
  { "DepICAO"		, COLUMN_SORT_STR	 , TRUE },
  { "Dist"		, COLUMN_SORT_FLOAT	 , TRUE },
  { "Dual"		, COLUMN_SORT_NUM	 , TRUE },
  { "Dur"		, COLUMN_SORT_NUM	 , TRUE },
  { "Elev"		, COLUMN_SORT_FLOAT	 , TRUE },
  { "FE"		, COLUMN_SORT_NUM	 , TRUE },
  { "FTD"		, COLUMN_SORT_NUM	 , TRUE },
  { "FleetNo"		, COLUMN_SORT_STR	 , TRUE },
  { "Flights"		, COLUMN_SORT_NUM	 , TRUE },
  { "FltNo"		, COLUMN_SORT_NUM	 , TRUE },
  { "FootLaunch"	, COLUMN_SORT_NUM	 , TRUE },
  { "Glider"		, COLUMN_SORT_NUM	 , TRUE },
  { "Gyro"		, COLUMN_SORT_NUM	 , TRUE },
  { "Heli"		, COLUMN_SORT_NUM	 , TRUE },
  { "HighPerf"		, COLUMN_SORT_NUM	 , TRUE },
  { "Hold"		, COLUMN_SORT_NUM	 , TRUE },
  { "IATA"		, COLUMN_SORT_STR	 , TRUE },
  { "ICAO"		, COLUMN_SORT_STR	 , TRUE },
  { "Ident"		, COLUMN_SORT_STR	 , TRUE },
  { "Inst"		, COLUMN_SORT_NUM	 , TRUE },
  { "Instruct"		, COLUMN_SORT_NUM	 , TRUE },
  { "Jet"		, COLUMN_SORT_NUM	 , TRUE },
  { "LTA"		, COLUMN_SORT_NUM	 , TRUE },
  { "Land"		, COLUMN_SORT_NUM	 , TRUE },
  { "Large"		, COLUMN_SORT_NUM	 , TRUE },
  { "Lat"		, COLUMN_SORT_FLOAT	 , TRUE },
  { "Leg"		, COLUMN_SORT_NUM	 , TRUE },
  { "Lon"		, COLUMN_SORT_FLOAT	 , TRUE },
  { "Make"		, COLUMN_SORT_STR	 , TRUE },
  { "Model"		, COLUMN_SORT_STR	 , TRUE },
  { "Multi"		, COLUMN_SORT_NUM	 , TRUE },
  { "NLand"		, COLUMN_SORT_NUM	 , TRUE },
  { "Name"		, COLUMN_SORT_STR	 , TRUE },
  { "Night"		, COLUMN_SORT_NUM	 , TRUE },
  { "Notes"		, COLUMN_SORT_STR	 , TRUE },
  { "NotesN"		, COLUMN_SORT_NUM	 , TRUE },
  { "Nts"		, COLUMN_SORT_NUM	 , TRUE },
  { "Over"		, COLUMN_SORT_NUM	 , TRUE },
  { "Overs"		, COLUMN_SORT_NUM	 , TRUE },
  { "PIC"		, COLUMN_SORT_NUM	 , TRUE },
  { "PPC"		, COLUMN_SORT_NUM	 , TRUE },
  { "PoweredLift"	, COLUMN_SORT_NUM	 , TRUE },
  { "Pressurized"	, COLUMN_SORT_NUM	 , TRUE },
  { "Province"		, COLUMN_SORT_STR	 , TRUE },
  { "Retract"		, COLUMN_SORT_NUM	 , TRUE },
  { "Route"             , COLUMN_SORT_STR        , TRUE },
  { "Role"		, COLUMN_SORT_STR	 , TRUE },
  { "Rotorcraft"	, COLUMN_SORT_NUM	 , TRUE },
  { "RtIATA"		, COLUMN_SORT_STR	 , TRUE },
  { "RtICAO"		, COLUMN_SORT_STR	 , TRUE },
  { "SDur"		, COLUMN_SORT_NUM	 , TRUE },
  { "SIC"		, COLUMN_SORT_NUM	 , TRUE },
  { "SIn"		, COLUMN_SORT_NUM	 , TRUE },
  { "SOut"		, COLUMN_SORT_NUM	 , TRUE },
  { "Sea"		, COLUMN_SORT_NUM	 , TRUE },
  { "Sim"		, COLUMN_SORT_NUM	 , TRUE },
  { "SimInst"		, COLUMN_SORT_NUM	 , TRUE },
  { "Single"		, COLUMN_SORT_NUM	 , TRUE },
  { "Solo"		, COLUMN_SORT_NUM	 , TRUE },
  { "Sport"		, COLUMN_SORT_NUM	 , TRUE },
  { "TZone"		, COLUMN_SORT_STR	 , TRUE },
  { "Tail"		, COLUMN_SORT_STR	 , TRUE },
  { "Time"		, COLUMN_SORT_NUM	 , TRUE },
  { "Total"		, COLUMN_SORT_NUM	 , TRUE },
  { "Trip"		, COLUMN_SORT_STR	 , TRUE },
  { "TripDate"		, COLUMN_SORT_STR	 , TRUE },
  { "Turbine"		, COLUMN_SORT_NUM	 , TRUE },
  { "Type"		, COLUMN_SORT_STR	 , TRUE },
  { "Ultralight"	, COLUMN_SORT_NUM	 , TRUE },
  { "WeightShift"	, COLUMN_SORT_NUM	 , TRUE },
  { "XC"		, COLUMN_SORT_NUM	 , TRUE },
  { "id"                , COLUMN_SORT_NUM        , FALSE },
  { "aircraft_id"	, COLUMN_SORT_NUM	 , FALSE },
  { "arr_id"		, COLUMN_SORT_NUM	 , FALSE },
  { "dep_id"		, COLUMN_SORT_NUM	 , FALSE },
  { "flight_id"		, COLUMN_SORT_NUM	 , FALSE },
  { "nAprch"		, COLUMN_SORT_NUM	 , FALSE },
  { "role_id"		, COLUMN_SORT_NUM	 , FALSE },
  {0}
};

Logbook *logbook_new(const char *filename)
{
  Logbook         *logbook = g_slice_new0(Logbook);

  logbook->column_prefs = g_hash_table_new(g_str_hash, g_str_equal);
  int i;
  for (i=0; columns[i].name; i++) {
    g_hash_table_insert(logbook->column_prefs, columns[i].name, &columns[i]);
  }

  /* Open Database */
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
  logbook->ta_begin = db_prep(logbook->db, "BEGIN TRANSACTION;");
  logbook->ta_commit = db_prep(logbook->db, "COMMIT;");
  logbook->ta_rollback = db_prep(logbook->db, "ROLLBACK;");

  logbook->flights_select_all   = db_prep(logbook->db, FLIGHTS_SELECT FLIGHTS_ORDER ";");
  logbook->flights_select_by_id = db_prep(logbook->db, FLIGHTS_SELECT FLIGHTS_WHERE_ID);
  logbook->flights_insert       = db_prep(logbook->db, FLIGHTS_INSERT);
  logbook->flights_update       = db_prep(logbook->db, FLIGHTS_UPDATE);
  logbook->flights_delete       = db_prep(logbook->db, FLIGHTS_DELETE);
  logbook->flights_routing_insert = db_prep(logbook->db, FLIGHTS_ROUTING_INSERT);
  logbook->flights_routing_delete = db_prep(logbook->db, FLIGHTS_ROUTING_DELETE);
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

  logbook->models_select_all     = db_prep(logbook->db, MODELS_SELECT MODELS_GROUP_BY ";");
  logbook->models_select_by_id   = db_prep(logbook->db, MODELS_SELECT MODELS_WHERE_ID MODELS_GROUP_BY ";");
  logbook->models_insert         = db_prep(logbook->db, MODELS_INSERT);
  logbook->models_update         = db_prep(logbook->db, MODELS_UPDATE);
  logbook->models_delete         = db_prep(logbook->db, MODELS_DELETE);

  logbook->airports_select_all     = db_prep(logbook->db, AIRPORTS_SELECT AIRPORTS_GROUP_BY ";");
  logbook->airports_select_by_id   = db_prep(logbook->db, AIRPORTS_SELECT AIRPORTS_WHERE_ID ";");
  logbook->airports_insert         = db_prep(logbook->db, AIRPORTS_INSERT);
  logbook->airports_update         = db_prep(logbook->db, AIRPORTS_UPDATE);
  logbook->airports_delete         = db_prep(logbook->db, AIRPORTS_DELETE);

  logbook->registry_select_by_path_key = db_prep(logbook->db, "SELECT value FROM Registry WHERE path = ? AND key = ?;");
  logbook->registry_insert             = db_prep(logbook->db, "INSERT INTO Registry (value, path, key) VALUES (?, ?, ?);");
  logbook->registry_update             = db_prep(logbook->db, "UPDATE Registry SET value = ? WHERE path = ? AND key = ?;");
  logbook->registry_delete             = db_prep(logbook->db, "DELETE FROM Registry WHERE path = ? AND key = ?;");

  logbook->reports_sql_by_title    = db_prep(logbook->db, "SELECT SQL FROM Reports WHERE Title = ?;");
  logbook->reports_delete_by_title = db_prep(logbook->db, "DELETE FROM Reports WHERE Title = ?;");
  logbook->reports_insert          = db_prep(logbook->db, "INSERT INTO Reports (Title, SQL) VALUES (?, ?);");
  
  pull_widget(window);
  pull_widget(flights_airline_pane);
  pull_widget(flights_ga_pane);
  pull_action(flights_edit_ga);
  pull_action(flights_edit_airline);
  pull_widget(flights_where);
  pull_widget(flights_refresh);
  pull_widget(flights_query_progress);
  pull_widget(flights_results_summary);
  pull_widget(flights_sw);
  pull_widget(flights_tail_ga);
  pull_widget(flights_tail_airline);
  pull_widget(flights_fleetno_ga);
  pull_widget(flights_fleetno_airline);
  pull_widget(flights_aircraft_utilized);
  pull_widget(flights_utc);
  pull_widget(flights_utc_lbl);
  pull_widget(flights_icao_toggle);
  pull_widget(flights_icao_toggle_lbl);
  pull_widget(flights_fleetno_toggle);
  pull_widget(flights_fleetno_toggle_lbl);
  pull_widget(flights_date_airline);
  pull_widget(flights_date_ga);
  pull_widget(flights_leg_ga);
  pull_widget(flights_leg_airline);
  pull_widget(flights_role_ga);
  pull_widget(flights_role_airline);
  pull_widget(flights_role_utilized);
  pull_widget(flights_depiata);
  pull_widget(flights_depicao);
  pull_widget(flights_dep_utilized);
  pull_widget(flights_arricao);
  pull_widget(flights_arriata);
  pull_widget(flights_arr_utilized);
  pull_widget(flights_route);
  pull_widget(flights_aout);
  pull_widget(flights_ain);
  pull_widget(flights_dur_ga);
  pull_widget(flights_dur_airline);
  pull_widget(flights_night_ga);
  pull_widget(flights_night_airline);
  pull_widget(flights_inst_ga);
  pull_widget(flights_inst_airline);
  pull_widget(flights_siminst_ga);
  pull_widget(flights_siminst_airline);
  pull_widget(flights_hold_ga);
  pull_widget(flights_hold_airline);
  pull_widget(flights_aprch_ga);
  pull_widget(flights_aprch_airline);
  pull_widget(flights_xc_ga);
  pull_widget(flights_xc_airline);
  pull_widget(flights_dland_ga);
  pull_widget(flights_dland_airline);
  pull_widget(flights_nland_ga);
  pull_widget(flights_nland_airline);
  pull_widget(flights_crew);
  pull_widget(flights_crew_ga);
  pull_widget(flights_crew_airline);
  pull_widget(flights_notes);
  pull_widget(flights_notes_ga);
  pull_widget(flights_notes_airline);
  pull_widget(flights_fltno);
  pull_widget(flights_sout);
  pull_widget(flights_sin);
  pull_widget(flights_sdur);
  pull_widget(flights_trip);
  pull_widget(flights_tripdate);
  pull_widget(flights_view_tail);
  pull_widget(flights_view_fleetno);
  pull_widget(flights_view_model);
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
  pull_widget(aircraft_model);
  pull_widget(aircraft_notes);
  pull_widget(aircraft_new_btn);
  pull_widget(aircraft_save_btn);
  pull_widget(aircraft_armdel_btn);
  pull_widget(aircraft_del_btn);
  pull_widget(aircraft_todel_lbl);
  pull_widget(models_sw);
  pull_widget(models_ident);
  pull_widget(models_ident_valid_wart);
  pull_widget(models_make);
  pull_widget(models_type);
  pull_widget(models_airplane);
  pull_widget(models_rotorcraft);
  pull_widget(models_glider);
  pull_widget(models_lta);
  pull_widget(models_poweredlift);
  pull_widget(models_ppc);
  pull_widget(models_weightshift);
  pull_widget(models_heli);
  pull_widget(models_gyro);
  pull_widget(models_airship);
  pull_widget(models_balloon);
  pull_widget(models_single);
  pull_widget(models_multi);
  pull_widget(models_land);
  pull_widget(models_sea);
  pull_widget(models_turbine);
  pull_widget(models_jet);
  pull_widget(models_highperf);
  pull_widget(models_retract);
  pull_widget(models_complex);
  pull_widget(models_pressurized);
  pull_widget(models_large);
  pull_widget(models_sport);
  pull_widget(models_ultralight);
  pull_widget(models_footlaunch);
  pull_widget(models_sim);
  pull_widget(models_ftd);
  pull_widget(models_total);
  pull_widget(models_new_btn);
  pull_widget(models_save_btn);
  pull_widget(models_armdel_btn);
  pull_widget(models_del_btn);
  pull_widget(models_todel_lbl);
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
  pull_widget(airports_tzone_combo);
  pull_widget(airports_notes);
  pull_widget(airports_new_btn);
  pull_widget(airports_save_btn);
  pull_widget(airports_armdel_btn);
  pull_widget(airports_del_btn);
  pull_widget(airports_todel_lbl);
  pull_widget(reports_view_btn);
  pull_widget(reports_export_btn);
  pull_widget(reports_view_pane);
  pull_widget(reports_export_pane);
  pull_widget(reports_sw);
  pull_widget(reports_err_msg);
  pull_widget(reports_title);
  pull_widget(reports_title_combo);
  pull_widget(reports_refresh);
  pull_widget(reports_query_progress);
  pull_widget(reports_results_summary);
  pull_widget(reports_sql_expander);
  pull_widget(reports_sql_text);
  pull_widget(reports_save_btn);
  pull_widget(reports_armdel_btn);
  pull_widget(reports_del_btn);
  pull_widget(summaries_view_btn);
  pull_widget(summaries_export_btn);
  pull_widget(summaries_view_pane);
  pull_widget(summaries_export_pane);
  pull_widget(summaries_select_summary);
  pull_widget(summaries_sw);
  pull_widget(summaries_parameters);
  pull_widget(import_script);
  pull_widget(import_script_options);
  pull_widget(import_read_btn);
  pull_widget(import_write_controls);
  pull_widget(import_write_btn);
  pull_widget(import_input_expander);
  pull_widget(import_input_text);
  pull_widget(import_input_file);
  pull_widget(import_interpreted_expander);
  pull_widget(import_interpreted_sw);
  pull_widget(import_interpreted_status);
  pull_widget(import_response_expander);
  pull_widget(import_response_text);
  pull_widget(import_write_normal);
  pull_widget(import_write_dryrun);
  pull_widget(import_write_force);
  pull_widget(import_input_notebook);
  
  /* Add treeview */
  flights_build_store_view(logbook);

  store_build_query_stmt_widget(logbook->roles_select_all, NULL, &logbook->roles_treeview, &logbook->roles_treemodel);
  gtk_widget_show_all(logbook->roles_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->roles_sw), logbook->roles_treeview);

  store_build_query_stmt_widget(logbook->aircraft_select_all, NULL, &logbook->aircraft_treeview, &logbook->aircraft_treemodel);
  gtk_widget_show_all(logbook->aircraft_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->aircraft_sw), logbook->aircraft_treeview);

  store_build_query_stmt_widget(logbook->models_select_all, NULL, &logbook->models_treeview, &logbook->models_treemodel);
  gtk_widget_show_all(logbook->models_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->models_sw), logbook->models_treeview);

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
  logbook->models_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (logbook->models_treeview));
  gtk_tree_selection_set_mode (logbook->models_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (logbook->models_selection), "changed",
		    G_CALLBACK (on_models_selection_changed),
		    logbook);
  logbook->airports_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (logbook->airports_treeview));
  gtk_tree_selection_set_mode (logbook->airports_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (logbook->airports_selection), "changed",
		    G_CALLBACK (on_airports_selection_changed),
		    logbook);

  /* Setup text view buffer callbacks */
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->flights_crew_ga))), "changed",
		   G_CALLBACK(on_flights_crew_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->flights_notes_ga))), "changed",
		   G_CALLBACK(on_flights_notes_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->flights_crew_airline))), "changed",
		   G_CALLBACK(on_flights_crew_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->flights_notes_airline))), "changed",
		   G_CALLBACK(on_flights_notes_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->aircraft_notes))), "changed",
		   G_CALLBACK(on_aircraft_notes_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->airports_notes))), "changed",
		   G_CALLBACK(on_airports_notes_changed), logbook);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(logbook->reports_sql_text))), "changed",
		   G_CALLBACK(on_reports_sql_text_changed), logbook);

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

  logbook->models_edctrl                 = &logbook->_models_edctrl;
  logbook->models_edctrl->edstate        = EDSTATE_EMPTY;
  logbook->models_edctrl->new_btn        = logbook->models_new_btn;
  logbook->models_edctrl->save_btn       = logbook->models_save_btn;
  logbook->models_edctrl->armdel_btn     = logbook->models_armdel_btn;
  logbook->models_edctrl->todel_lbl      = logbook->models_todel_lbl;
  logbook->models_edctrl->del_btn        = logbook->models_del_btn;
  logbook->models_edctrl->selection      = logbook->models_selection;
  logbook->models_edctrl->selection_show = models_selection_show;
  logbook->models_edctrl->can_delete     = models_can_delete;
  logbook->models_edctrl->delete_stmt    = logbook->models_delete;
  logbook->models_edctrl->select_by_id_stmt    = logbook->models_select_by_id;
  edctrl_register_save(logbook->models_edctrl, models_write_entries, logbook);
  edctrl_register_after_change(logbook->models_edctrl, models_after_change, logbook);
  edctrl_register_load_selection(logbook->models_edctrl, models_load_selection, logbook);
  edctrl_register_validator(logbook->models_edctrl, models_error, logbook);

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
  
  logbook->summaries_scripter = &logbook->_summaries_scripter;
  logbook->import_scripter = &logbook->_import_scripter;

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
  logbook->models_stale = 1;
  logbook->airports_stale = 1;
  reports_title_combo_init(logbook);
  summaries_init(logbook);
  import_init(logbook);

  flights_load_selection(logbook);
  roles_load_selection(logbook);
  aircraft_load_selection(logbook);
  models_load_selection(logbook);
  airports_load_selection(logbook);
  airports_tzone_combo_box_init(logbook);

  return logbook;
}

void logbook_save_options(Logbook *logbook)
{
  flights_save_options(logbook);
}

void logbook_finalize(Logbook *logbook)
{
  g_free(logbook->db_filename);
  g_hash_table_destroy(logbook->column_prefs);
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

void registry_set_text(Logbook *logbook, const char *path, const char *key, const char *text)
{
  DBStatement *stmt;
  if (registry_key_exists(logbook, path, key)) {
    stmt = logbook->registry_update;
  } else {
    stmt = logbook->registry_insert;
  }
  db_bind_text(stmt, 1, text);
  db_bind_text(stmt, 2, path);
  db_bind_text(stmt, 3, key);
  db_stp_res_clr(stmt);
}
