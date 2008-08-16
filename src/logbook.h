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
#define pull_widget(x) logbook->x = GTK_WIDGET(gtk_builder_get_object(builder, (#x)));
#else // fall back on libglade
#include <glade/glade-xml.h>
#define pull_widget(x) logbook->x = GTK_WIDGET(glade_xml_get_widget(gxml, (#x)));
#endif

struct Logbook {
  DB *db;
  char *db_filename;
  GtkWidget *window;

  int flights_stale;	   /* Treemodel contains stale data as a result of updates to db elsewhere ... the model for the view is stale; it has nothing to do with the db */
  DBStatement *flights_select_all; /* Select for tree model */
  DBStatement *flights_select_by_id; /* Select for update a single treemodel row */
  DBStatement *flights_insert;
  DBStatement *flights_update;
  DBStatement *flights_delete;
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
  GtkWidget *flights_aircraft;
  GtkWidget *flights_aircraft_utilized;
  GtkWidget *flights_date;
  GtkWidget *flights_leg;
  GtkWidget *flights_role;
  GtkWidget *flights_role_utilized;
  GtkWidget *flights_dep;
  GtkWidget *flights_dep_utilized;
  GtkWidget *flights_arr;
  GtkWidget *flights_arr_utilized;
  GtkWidget *flights_aout;
  GtkWidget *flights_ain;
  GtkWidget *flights_dur;
  GtkWidget *flights_night;
  GtkWidget *flights_inst;
  GtkWidget *flights_siminst;
  GtkWidget *flights_hold;
  GtkWidget *flights_aprch;
  GtkWidget *flights_xc;
  GtkWidget *flights_dland;
  GtkWidget *flights_nland;
  GtkWidget *flights_crew;
  GtkWidget *flights_notes;
  GtkWidget *flights_fltno;
  GtkWidget *flights_sout;
  GtkWidget *flights_sin;
  GtkWidget *flights_sdur;
  GtkWidget *flights_trip;
  GtkWidget *flights_tripdate;

  GtkWidget *flights_view_aircraft;
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
  int aircraft_ident_error;
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
  GtkWidget *aircraft_ident;
  GtkWidget *aircraft_ident_valid_wart;
  GtkWidget *aircraft_fleetno;
  GtkWidget *aircraft_fleetno_valid_wart;
  GtkWidget *aircraft_type;
  GtkWidget *aircraft_notes;
  GtkWidget *aircraft_new_btn;
  GtkWidget *aircraft_save_btn;
  GtkWidget *aircraft_armdel_btn;
  GtkWidget *aircraft_todel_lbl;
  GtkWidget *aircraft_del_btn;
  Edctrl _aircraft_edctrl;
  Edctrl *aircraft_edctrl;

  int types_stale;
  int types_ident_error;
  DBStatement *types_select_all;
  DBStatement *types_select_by_id;
  DBStatement *types_insert;
  DBStatement *types_update;
  DBStatement *types_delete;
  GtkWidget *types_sw;
  GtkTreeModel *types_treemodel;
  GtkTreeSelection *types_selection;
  GtkWidget *types_treeview;
  GtkWidget *types_ident;
  GtkWidget *types_ident_valid_wart;
  GtkWidget *types_make;
  GtkWidget *types_model;
  GtkWidget *types_airplane;
  GtkWidget *types_rotorcraft;
  GtkWidget *types_glider;
  GtkWidget *types_lta;
  GtkWidget *types_poweredlift;
  GtkWidget *types_ppc;
  GtkWidget *types_weightshift;
  GtkWidget *types_heli;
  GtkWidget *types_gyro;
  GtkWidget *types_airship;
  GtkWidget *types_balloon;
  GtkWidget *types_single;
  GtkWidget *types_multi;
  GtkWidget *types_land;
  GtkWidget *types_sea;
  GtkWidget *types_turbine;
  GtkWidget *types_jet;
  GtkWidget *types_highperf;
  GtkWidget *types_retract;
  GtkWidget *types_complex;
  GtkWidget *types_pressurized;
  GtkWidget *types_large;
  GtkWidget *types_sport;
  GtkWidget *types_ultralight;
  GtkWidget *types_footlaunch;
  GtkWidget *types_sim;
  GtkWidget *types_ftd;
  GtkWidget *types_total;
  GtkWidget *types_new_btn;
  GtkWidget *types_save_btn;
  GtkWidget *types_armdel_btn;
  GtkWidget *types_todel_lbl;
  GtkWidget *types_del_btn;
  Edctrl _types_edctrl;
  Edctrl *types_edctrl;

  int airports_stale;
  int airports_ident_error;
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
  GtkWidget *airports_ident;
  GtkWidget *airports_ident_valid_wart;
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
  GtkWidget *airports_notes;
  GtkWidget *airports_new_btn;
  GtkWidget *airports_save_btn;
  GtkWidget *airports_armdel_btn;
  GtkWidget *airports_todel_lbl;
  GtkWidget *airports_del_btn;
  Edctrl _airports_edctrl;
  Edctrl *airports_edctrl;

  GtkWidget *reports_title;
  GtkWidget *reports_refresh;
  GtkWidget *reports_query_progress;
  GtkWidget *reports_results_summary;
  GtkWidget *reports_sql_text;
  GtkWidget *reports_save_btn;
  GtkWidget *reports_armdel_btn;
  GtkWidget *reports_del_btn;

  GtkWidget *summaries_select_summary;
  GtkWidget *summaries_sw;
  GtkWidget *summaries_html;
  GtkWidget *summaries_parameters;
  Scripter _summaries_parameter_pane;
  Scripter *summaries_parameter_pane;
};

Logbook *logbook_new(const char *filename);
void logbook_finalize(Logbook *);
void logbook_save_options(Logbook *logbook);
#endif
