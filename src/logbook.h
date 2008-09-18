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


#ifndef _LOGBOOK_H_
#define _LOGBOOK_H_

#include <db/db.h>
#include "sqlpilot.h"
#include "edctrl.h"
#include "scripter.h"

#ifdef USING_GTK_BUILDER
#define pull_widget(x) logbook->x = GTK_WIDGET(gtk_builder_get_object(builder, (#x)))
#define pull_action(x) logbook->x = GTK_ACTION(gtk_builder_get_object(builder, (#x)))
#else // fall back on libglade
#include <glade/glade-xml.h>
#define pull_widget(x) logbook->x = GTK_WIDGET(glade_xml_get_widget(gxml, (#x)));
#endif

#include <libgtkhtml/document/htmldocument.h>

#define REGISTRY_BUF_VALUE 4096


struct Logbook {
  DB *db;
  char *db_filename;
  GHashTable *column_prefs;
  GtkWidget *window;
  DBStatement *ta_begin;
  DBStatement *ta_commit;
  DBStatement *ta_rollback;

  int flights_stale;	   /* Treemodel contains stale data as a result of updates to db elsewhere ... the model for the view is stale; it has nothing to do with the db */
  DBStatement *flights_select_all; /* Select for tree model */
  DBStatement *flights_select_by_id; /* Select for update a single treemodel row */
  DBStatement *flights_insert;
  DBStatement *flights_update;
  DBStatement *flights_delete;
  DBStatement *flights_routing_insert;
  DBStatement *flights_routing_delete;
  DBStatement *flights_aircraft_fleetno_from_tail;
  DBStatement *flights_aircraft_tail_from_fleetno;
  DBStatement *flights_tz_of_airport_iata;
  DBStatement *flights_tz_of_airport_icao;
  DBStatement *flights_icao_from_iata;
  DBStatement *flights_iata_from_icao;
  GtkWidget *flights_airline_pane;
  GtkWidget *flights_ga_pane;
  GtkAction *flights_edit_ga;
  GtkAction *flights_edit_airline;
  GtkWidget *flights_where;
  GtkWidget *flights_refresh;
  GtkWidget *flights_query_progress;
  GtkWidget *flights_results_summary;
  GtkWidget *flights_sw;	/* Scrollable Window - contains the treeview */
  GtkTreeModel *flights_treemodel;
  GtkTreeSelection *flights_selection;
  GtkWidget *flights_treeview;
  GtkWidget *flights_utc;
  GtkWidget *flights_utc_lbl;
  GtkWidget *flights_icao_toggle;
  GtkWidget *flights_icao_toggle_lbl;
  GtkWidget *flights_fleetno_toggle;
  GtkWidget *flights_fleetno_toggle_lbl;
  GtkWidget *flights_tail;
  GtkWidget *flights_tail_ga;
  GtkWidget *flights_tail_airline;
  GtkWidget *flights_fleetno;
  GtkWidget *flights_fleetno_ga;
  GtkWidget *flights_fleetno_airline;
  GtkWidget *flights_aircraft_utilized;
  GtkWidget *flights_date;
  GtkWidget *flights_date_ga;
  GtkWidget *flights_date_airline;
  GtkWidget *flights_leg;
  GtkWidget *flights_leg_ga;
  GtkWidget *flights_leg_airline;
  GtkWidget *flights_role;
  GtkWidget *flights_role_ga;
  GtkWidget *flights_role_airline;
  GtkWidget *flights_role_utilized;
  GtkWidget *flights_depicao;
  GtkWidget *flights_depiata;
  GtkWidget *flights_dep_utilized;
  GtkWidget *flights_arricao;
  GtkWidget *flights_arriata;
  GtkWidget *flights_arr_utilized;
  GtkWidget *flights_route;
  GtkWidget *flights_aout;
  GtkWidget *flights_ain;
  GtkWidget *flights_dur;
  GtkWidget *flights_dur_ga;
  GtkWidget *flights_dur_airline;
  GtkWidget *flights_night;
  GtkWidget *flights_night_ga;
  GtkWidget *flights_night_airline;
  GtkWidget *flights_inst;
  GtkWidget *flights_inst_ga;
  GtkWidget *flights_inst_airline;
  GtkWidget *flights_siminst;
  GtkWidget *flights_siminst_ga;
  GtkWidget *flights_siminst_airline;
  GtkWidget *flights_hold;
  GtkWidget *flights_hold_ga;
  GtkWidget *flights_hold_airline;
  GtkWidget *flights_aprch;
  GtkWidget *flights_aprch_ga;
  GtkWidget *flights_aprch_airline;
  GtkWidget *flights_xc;
  GtkWidget *flights_xc_ga;
  GtkWidget *flights_xc_airline;
  GtkWidget *flights_dland;
  GtkWidget *flights_dland_ga;
  GtkWidget *flights_dland_airline;
  GtkWidget *flights_nland;
  GtkWidget *flights_nland_ga;
  GtkWidget *flights_nland_airline;
  GtkWidget *flights_crew;
  GtkWidget *flights_crew_ga;
  GtkWidget *flights_crew_airline;
  GtkWidget *flights_notes;
  GtkWidget *flights_notes_ga;
  GtkWidget *flights_notes_airline;
  GtkWidget *flights_fltno;
  GtkWidget *flights_sout;
  GtkWidget *flights_sin;
  GtkWidget *flights_sdur;
  GtkWidget *flights_trip;
  GtkWidget *flights_tripdate;

  GtkWidget *flights_view_tail;
  GtkWidget *flights_view_fleetno;
  GtkWidget *flights_view_model;
  GtkWidget *flights_view_date;
  GtkWidget *flights_view_leg;
  GtkWidget *flights_view_dist;
  GtkWidget *flights_view_role;
  GtkWidget *flights_view_dep;
  GtkWidget *flights_view_arr;
  GtkWidget *flights_view_aout;
  GtkWidget *flights_view_ain;
  GtkWidget *flights_view_dur;
  GtkWidget *flights_view_night;
  GtkWidget *flights_view_inst;
  GtkWidget *flights_view_siminst;
  GtkWidget *flights_view_hold;
  GtkWidget *flights_view_aprch;
  GtkWidget *flights_view_aprchn;
  GtkWidget *flights_view_xc;
  GtkWidget *flights_view_dland;
  GtkWidget *flights_view_nland;
  GtkWidget *flights_view_crew;
  GtkWidget *flights_view_crewn;
  GtkWidget *flights_view_notes;
  GtkWidget *flights_view_notesn;
  GtkWidget *flights_view_fltno;
  GtkWidget *flights_view_sout;
  GtkWidget *flights_view_sin;
  GtkWidget *flights_view_sdur;
  GtkWidget *flights_view_trip;
  GtkWidget *flights_view_tripdate;
  GtkWidget *flights_view_over;

  GtkWidget *flights_new_btn;
  GtkWidget *flights_save_btn;
  GtkWidget *flights_armdel_btn;
  GtkWidget *flights_todel_lbl;
  GtkWidget *flights_del_btn;
  Edctrl _flights_edctrl;
  Edctrl *flights_edctrl;

  int roles_stale;
  int roles_ident_error;
  DBStatement *roles_select_all;
  DBStatement *roles_select_by_id;
  DBStatement *roles_insert;
  DBStatement *roles_update;
  DBStatement *roles_delete;
  GtkWidget *roles_sw;
  GtkTreeModel *roles_treemodel;
  GtkTreeSelection *roles_selection;
  GtkWidget *roles_treeview;
  GtkWidget *roles_ident;
  GtkWidget *roles_ident_valid_wart;
  GtkWidget *roles_name;
  GtkWidget *roles_pic;
  GtkWidget *roles_sic;
  GtkWidget *roles_fe;
  GtkWidget *roles_solo;
  GtkWidget *roles_dual;
  GtkWidget *roles_instruct;
  GtkWidget *roles_total;
  GtkWidget *roles_new_btn;
  GtkWidget *roles_save_btn;
  GtkWidget *roles_armdel_btn;
  GtkWidget *roles_todel_lbl;
  GtkWidget *roles_del_btn;
  Edctrl _roles_edctrl;
  Edctrl *roles_edctrl;

  int aircraft_stale;
  int aircraft_tail_error;
  int aircraft_fleetno_error;
  DBStatement *aircraft_select_all;
  DBStatement *aircraft_select_by_id;
  DBStatement *aircraft_insert;
  DBStatement *aircraft_update;
  DBStatement *aircraft_delete;
  DBStatement *aircraft_count_flights;
  GtkWidget *aircraft_sw;
  GtkTreeModel *aircraft_treemodel;
  GtkTreeSelection *aircraft_selection;
  GtkWidget *aircraft_treeview;
  GtkWidget *aircraft_tail;
  GtkWidget *aircraft_tail_valid_wart;
  GtkWidget *aircraft_fleetno;
  GtkWidget *aircraft_fleetno_valid_wart;
  GtkWidget *aircraft_model;
  GtkWidget *aircraft_notes;
  GtkWidget *aircraft_new_btn;
  GtkWidget *aircraft_save_btn;
  GtkWidget *aircraft_armdel_btn;
  GtkWidget *aircraft_todel_lbl;
  GtkWidget *aircraft_del_btn;
  Edctrl _aircraft_edctrl;
  Edctrl *aircraft_edctrl;

  int models_stale;
  int models_ident_error;
  DBStatement *models_select_all;
  DBStatement *models_select_by_id;
  DBStatement *models_insert;
  DBStatement *models_update;
  DBStatement *models_delete;
  GtkWidget *models_sw;
  GtkTreeModel *models_treemodel;
  GtkTreeSelection *models_selection;
  GtkWidget *models_treeview;
  GtkWidget *models_ident;
  GtkWidget *models_ident_valid_wart;
  GtkWidget *models_make;
  GtkWidget *models_type;
  GtkWidget *models_airplane;
  GtkWidget *models_rotorcraft;
  GtkWidget *models_glider;
  GtkWidget *models_lta;
  GtkWidget *models_poweredlift;
  GtkWidget *models_ppc;
  GtkWidget *models_weightshift;
  GtkWidget *models_heli;
  GtkWidget *models_gyro;
  GtkWidget *models_airship;
  GtkWidget *models_balloon;
  GtkWidget *models_single;
  GtkWidget *models_multi;
  GtkWidget *models_land;
  GtkWidget *models_sea;
  GtkWidget *models_turbine;
  GtkWidget *models_jet;
  GtkWidget *models_highperf;
  GtkWidget *models_retract;
  GtkWidget *models_complex;
  GtkWidget *models_pressurized;
  GtkWidget *models_large;
  GtkWidget *models_sport;
  GtkWidget *models_ultralight;
  GtkWidget *models_footlaunch;
  GtkWidget *models_sim;
  GtkWidget *models_ftd;
  GtkWidget *models_total;
  GtkWidget *models_new_btn;
  GtkWidget *models_save_btn;
  GtkWidget *models_armdel_btn;
  GtkWidget *models_todel_lbl;
  GtkWidget *models_del_btn;
  Edctrl _models_edctrl;
  Edctrl *models_edctrl;

  int airports_stale;
  int airports_iata_error;
  int airports_icao_error;
  DBStatement *airports_select_all;
  DBStatement *airports_select_by_id;
  DBStatement *airports_insert;
  DBStatement *airports_update;
  DBStatement *airports_delete;
  GtkWidget *airports_sw;
  GtkTreeModel *airports_treemodel;
  GtkTreeSelection *airports_selection;
  GtkWidget *airports_treeview;
  GtkWidget *airports_iata;
  GtkWidget *airports_iata_valid_wart;
  GtkWidget *airports_icao;
  GtkWidget *airports_icao_valid_wart;
  GtkWidget *airports_name;
  GtkWidget *airports_city;
  GtkWidget *airports_province;
  GtkWidget *airports_country;
  GtkWidget *airports_elev;
  GtkWidget *airports_lat;
  GtkWidget *airports_lon;
  GtkWidget *airports_tzone;
  GtkWidget *airports_tzone_combo;
  GtkWidget *airports_notes;
  GtkWidget *airports_new_btn;
  GtkWidget *airports_save_btn;
  GtkWidget *airports_armdel_btn;
  GtkWidget *airports_todel_lbl;
  GtkWidget *airports_del_btn;
  Edctrl _airports_edctrl;
  Edctrl *airports_edctrl;

  GtkWidget *reports_view_btn;
  GtkWidget *reports_export_btn;
  GtkWidget *reports_view_pane;
  GtkWidget *reports_export_pane;
  DBStatement *reports_delete_by_title;
  DBStatement *reports_sql_by_title;
  DBStatement *reports_insert;
  GtkWidget *reports_sw;
  GtkTreeModel *reports_treemodel;
  GtkWidget *reports_err_msg;
  GtkWidget *reports_treeview;
  GtkWidget *reports_title;
  GtkWidget *reports_title_combo;
  GtkWidget *reports_refresh;
  GtkWidget *reports_query_progress;
  GtkWidget *reports_results_summary;
  GtkWidget *reports_sql_expander;
  GtkWidget *reports_sql_text;
  GtkWidget *reports_save_btn;
  GtkWidget *reports_armdel_btn;
  GtkWidget *reports_del_btn;

  GtkWidget *summaries_view_btn;
  GtkWidget *summaries_export_btn;
  GtkWidget *summaries_view_pane;
  GtkWidget *summaries_export_pane;
  GtkWidget *summaries_select_summary;
  GtkWidget *summaries_sw;
  HtmlDocument *summaries_html_document;
  GtkWidget *summaries_html_view;
  GtkWidget *summaries_parameters;
  Scripter _summaries_scripter;
  Scripter *summaries_scripter;

  GtkWidget *import_script;
  GtkWidget *import_script_options;
  GtkWidget *import_read_btn;
  GtkWidget *import_write_controls;
  GtkWidget *import_write_btn;
  GtkWidget *import_input_expander;
  GtkWidget *import_input_text;
  GtkWidget *import_input_file;
  GtkWidget *import_interpreted_expander;
  GtkWidget *import_interpreted_sw;
  GtkWidget *import_interpreted_status;
  GtkTreeModel *import_interpreted_treemodel;
  GtkWidget *import_interpreted_treeview;
  GtkWidget *import_response_expander;
  GtkWidget *import_response_text;
  GtkWidget *import_write_normal;
  GtkWidget *import_write_dryrun;
  GtkWidget *import_write_force;
  GtkWidget *import_input_notebook;
  Scripter _import_scripter;
  Scripter *import_scripter;

  char registry_value[REGISTRY_BUF_VALUE];
  DBStatement *registry_select_by_path_key;
  DBStatement *registry_insert;
  DBStatement *registry_update;
  DBStatement *registry_delete;
};

Logbook *logbook_new(const char *filename);
void logbook_finalize(Logbook *);
void logbook_save_options(Logbook *logbook);
char *registry_get_text(Logbook *logbook, const char *path, const char *key);
int registry_get_int(Logbook *logbook, const char *path, const char *key);
int registry_key_exists(Logbook *logbook, const char *path, const char *key);
void registry_set_int(Logbook *logbook, const char *path, const char *key, int val);
void registry_set_text(Logbook *logbook, const char *path, const char *key, const char *text);
#endif
