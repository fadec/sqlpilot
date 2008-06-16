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
#include "cb/cb.h"

#ifdef USING_GTK_BUILDER
#define pull_widget(x) sqlpilot->x = GTK_WIDGET(gtk_builder_get_object(builder, (#x)));
#else // fall back on libglade
#include <glade/glade-xml.h>
#define pull_widget(x) sqlpilot->x = GTK_WIDGET(glade_xml_get_widget(gxml, (#x)));
#endif


void barf(const char *message)
{
  if (message) fprintf(stderr, "Error: %s", message);
  exit(EXIT_BARF);
}

Sqlpilot *sqlpilot_new(const char *filename)
{
  Sqlpilot         *sqlpilot;

  /* Open Database */
  sqlpilot = g_slice_new0(Sqlpilot);
  if (!(sqlpilot->db = db_open(filename))) {
    barf ("Couldn't open database.");
    return NULL;
  }

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
  gtk_builder_connect_signals(builder, sqlpilot);

#else
  GladeXML *gxml;
  gxml = glade_xml_new(UI_XML_FILE, NULL, NULL);
  if (!gxml) {
    barf ("Can't load glade xml file\n");
    return NULL;
  }
#endif

  /* Set DB statements */
  sqlpilot->flights_select_all   = db_prep(sqlpilot->db, FLIGHTS_SELECT FLIGHTS_ORDER ";");
  sqlpilot->flights_select_by_id = db_prep(sqlpilot->db, FLIGHTS_SELECT FLIGHTS_WHERE_ID);
  sqlpilot->flights_insert       = db_prep(sqlpilot->db, FLIGHTS_INSERT);
  sqlpilot->flights_update       = db_prep(sqlpilot->db, FLIGHTS_UPDATE);
  sqlpilot->flights_delete       = db_prep(sqlpilot->db, FLIGHTS_DELETE);

  sqlpilot->roles_select_all     = db_prep(sqlpilot->db, ROLES_SELECT ROLES_GROUP_BY ";");
  sqlpilot->roles_select_by_id   = db_prep(sqlpilot->db, ROLES_SELECT ROLES_WHERE_ID ROLES_GROUP_BY ";");
  sqlpilot->roles_insert         = db_prep(sqlpilot->db, ROLES_INSERT);
  sqlpilot->roles_update         = db_prep(sqlpilot->db, ROLES_UPDATE);
  sqlpilot->roles_delete         = db_prep(sqlpilot->db, ROLES_DELETE);

  sqlpilot->aircraft_select_all     = db_prep(sqlpilot->db, AIRCRAFT_SELECT AIRCRAFT_GROUP_BY ";");
  sqlpilot->aircraft_select_by_id   = db_prep(sqlpilot->db, AIRCRAFT_SELECT AIRCRAFT_WHERE_ID AIRCRAFT_GROUP_BY ";");
  sqlpilot->aircraft_insert         = db_prep(sqlpilot->db, AIRCRAFT_INSERT);
  sqlpilot->aircraft_update         = db_prep(sqlpilot->db, AIRCRAFT_UPDATE);
  sqlpilot->aircraft_delete         = db_prep(sqlpilot->db, AIRCRAFT_DELETE);
  sqlpilot->aircraft_count_flights  = db_prep(sqlpilot->db, AIRCRAFT_COUNT_FLIGHTS);

  sqlpilot->types_select_all     = db_prep(sqlpilot->db, TYPES_SELECT TYPES_GROUP_BY ";");
  sqlpilot->types_select_by_id   = db_prep(sqlpilot->db, TYPES_SELECT TYPES_WHERE_ID TYPES_GROUP_BY ";");
  sqlpilot->types_insert         = db_prep(sqlpilot->db, TYPES_INSERT);
  sqlpilot->types_update         = db_prep(sqlpilot->db, TYPES_UPDATE);
  sqlpilot->types_delete         = db_prep(sqlpilot->db, TYPES_DELETE);

  sqlpilot->airports_select_all     = db_prep(sqlpilot->db, AIRPORTS_SELECT AIRPORTS_GROUP_BY ";");
  sqlpilot->airports_select_by_id   = db_prep(sqlpilot->db, AIRPORTS_SELECT AIRPORTS_WHERE_ID ";");
  sqlpilot->airports_insert         = db_prep(sqlpilot->db, AIRPORTS_INSERT);
  sqlpilot->airports_update         = db_prep(sqlpilot->db, AIRPORTS_UPDATE);
  sqlpilot->airports_delete         = db_prep(sqlpilot->db, AIRPORTS_DELETE);
  
  pull_widget(window);
  pull_widget(flights_where);
  pull_widget(flights_refresh);
  pull_widget(flights_query_progress);
  pull_widget(flights_results_summary);
  pull_widget(flights_sw);
  pull_widget(flights_view_date);
  pull_widget(flights_aircraft);
  pull_widget(flights_aircraft_utilized);
  pull_widget(flights_utc);
  pull_widget(flights_utc_lbl);
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
  pull_widget(aircraft_ident);
  pull_widget(aircraft_ident_valid_wart);
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
  pull_widget(airports_ident);
  pull_widget(airports_ident_valid_wart);
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
  
  /* Add treeview */
  store_build_query_stmt_widget(sqlpilot->flights_select_all, &sqlpilot->flights_treeview, &sqlpilot->flights_treemodel);
  gtk_widget_show_all(sqlpilot->flights_treeview);
  gtk_container_add(GTK_CONTAINER(sqlpilot->flights_sw), sqlpilot->flights_treeview);

  store_build_query_stmt_widget(sqlpilot->roles_select_all, &sqlpilot->roles_treeview, &sqlpilot->roles_treemodel);
  gtk_widget_show_all(sqlpilot->roles_treeview);
  gtk_container_add(GTK_CONTAINER(sqlpilot->roles_sw), sqlpilot->roles_treeview);

  store_build_query_stmt_widget(sqlpilot->aircraft_select_all, &sqlpilot->aircraft_treeview, &sqlpilot->aircraft_treemodel);
  gtk_widget_show_all(sqlpilot->aircraft_treeview);
  gtk_container_add(GTK_CONTAINER(sqlpilot->aircraft_sw), sqlpilot->aircraft_treeview);

  store_build_query_stmt_widget(sqlpilot->types_select_all, &sqlpilot->types_treeview, &sqlpilot->types_treemodel);
  gtk_widget_show_all(sqlpilot->types_treeview);
  gtk_container_add(GTK_CONTAINER(sqlpilot->types_sw), sqlpilot->types_treeview);

  store_build_query_stmt_widget(sqlpilot->airports_select_all, &sqlpilot->airports_treeview, &sqlpilot->airports_treemodel);
  gtk_widget_show_all(sqlpilot->airports_treeview);
  gtk_container_add(GTK_CONTAINER(sqlpilot->airports_sw), sqlpilot->airports_treeview);

  /* Setup treeview callbacks */
  sqlpilot->flights_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (sqlpilot->flights_treeview));
  gtk_tree_selection_set_mode (sqlpilot->flights_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (sqlpilot->flights_selection), "changed",
		    G_CALLBACK (on_flights_selection_changed),
		    sqlpilot);
  sqlpilot->roles_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (sqlpilot->roles_treeview));
  gtk_tree_selection_set_mode (sqlpilot->roles_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (sqlpilot->roles_selection), "changed",
		    G_CALLBACK (on_roles_selection_changed),
		    sqlpilot);
  sqlpilot->aircraft_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (sqlpilot->aircraft_treeview));
  gtk_tree_selection_set_mode (sqlpilot->aircraft_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (sqlpilot->aircraft_selection), "changed",
		    G_CALLBACK (on_aircraft_selection_changed),
		    sqlpilot);
  sqlpilot->types_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (sqlpilot->types_treeview));
  gtk_tree_selection_set_mode (sqlpilot->types_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (sqlpilot->types_selection), "changed",
		    G_CALLBACK (on_types_selection_changed),
		    sqlpilot);
  sqlpilot->airports_selection = gtk_tree_view_get_selection (GTK_TREE_VIEW (sqlpilot->airports_treeview));
  gtk_tree_selection_set_mode (sqlpilot->airports_selection, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (sqlpilot->airports_selection), "changed",
		    G_CALLBACK (on_airports_selection_changed),
		    sqlpilot);

  /* Setup text view buffer callbacks */
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(sqlpilot->flights_crew))), "changed",
		   G_CALLBACK(on_flights_crew_changed), sqlpilot);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(sqlpilot->flights_notes))), "changed",
		   G_CALLBACK(on_flights_notes_changed), sqlpilot);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(sqlpilot->aircraft_notes))), "changed",
		   G_CALLBACK(on_aircraft_notes_changed), sqlpilot);
  g_signal_connect(G_OBJECT(gtk_text_view_get_buffer(GTK_TEXT_VIEW(sqlpilot->airports_notes))), "changed",
		   G_CALLBACK(on_airports_notes_changed), sqlpilot);

  /* Out of place code to init flights utc button */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_utc), FALSE);
  gtk_label_set_text(GTK_LABEL(sqlpilot->flights_utc_lbl), "Local");

  /* Edit Controls */
  sqlpilot->flights_edctrl                 = &sqlpilot->_flights_edctrl;
  sqlpilot->flights_edctrl->edstate        = EDSTATE_EMPTY;
  sqlpilot->flights_edctrl->new_btn        = sqlpilot->flights_new_btn;
  sqlpilot->flights_edctrl->save_btn       = sqlpilot->flights_save_btn;
  sqlpilot->flights_edctrl->armdel_btn     = sqlpilot->flights_armdel_btn;
  sqlpilot->flights_edctrl->todel_lbl      = sqlpilot->flights_todel_lbl;
  sqlpilot->flights_edctrl->del_btn        = sqlpilot->flights_del_btn;
  sqlpilot->flights_edctrl->selection      = sqlpilot->flights_selection;
  sqlpilot->flights_edctrl->selection_show = flights_selection_show;
  sqlpilot->flights_edctrl->can_delete     = flights_can_delete;
  sqlpilot->flights_edctrl->delete_stmt    = sqlpilot->flights_delete;
  sqlpilot->flights_edctrl->select_by_id_stmt    = sqlpilot->flights_select_by_id;
  edctrl_register_save(sqlpilot->flights_edctrl, flights_write_entries, sqlpilot);
  edctrl_register_after_change(sqlpilot->flights_edctrl, flights_after_change, sqlpilot);
  edctrl_register_load_selection(sqlpilot->flights_edctrl, flights_load_selection, sqlpilot);

  sqlpilot->roles_edctrl                 = &sqlpilot->_roles_edctrl;
  sqlpilot->roles_edctrl->edstate        = EDSTATE_EMPTY;
  sqlpilot->roles_edctrl->new_btn        = sqlpilot->roles_new_btn;
  sqlpilot->roles_edctrl->save_btn       = sqlpilot->roles_save_btn;
  sqlpilot->roles_edctrl->armdel_btn     = sqlpilot->roles_armdel_btn;
  sqlpilot->roles_edctrl->todel_lbl      = sqlpilot->roles_todel_lbl;
  sqlpilot->roles_edctrl->del_btn        = sqlpilot->roles_del_btn;
  sqlpilot->roles_edctrl->selection      = sqlpilot->roles_selection;
  sqlpilot->roles_edctrl->selection_show = roles_selection_show;
  sqlpilot->roles_edctrl->can_delete     = roles_can_delete;
  sqlpilot->roles_edctrl->delete_stmt    = sqlpilot->roles_delete;
  sqlpilot->roles_edctrl->select_by_id_stmt    = sqlpilot->roles_select_by_id;
  edctrl_register_save(sqlpilot->roles_edctrl, roles_write_entries, sqlpilot);
  edctrl_register_after_change(sqlpilot->roles_edctrl, roles_after_change, sqlpilot);
  edctrl_register_load_selection(sqlpilot->roles_edctrl, roles_load_selection, sqlpilot);

  sqlpilot->aircraft_edctrl                        = &sqlpilot->_aircraft_edctrl;
  sqlpilot->aircraft_edctrl->edstate               = EDSTATE_EMPTY;
  sqlpilot->aircraft_edctrl->new_btn               = sqlpilot->aircraft_new_btn;
  sqlpilot->aircraft_edctrl->save_btn              = sqlpilot->aircraft_save_btn;
  sqlpilot->aircraft_edctrl->armdel_btn            = sqlpilot->aircraft_armdel_btn;
  sqlpilot->aircraft_edctrl->todel_lbl             = sqlpilot->aircraft_todel_lbl;
  sqlpilot->aircraft_edctrl->del_btn               = sqlpilot->aircraft_del_btn;
  sqlpilot->aircraft_edctrl->selection             = sqlpilot->aircraft_selection;
  sqlpilot->aircraft_edctrl->selection_show        = aircraft_selection_show;
  sqlpilot->aircraft_edctrl->can_delete            = aircraft_can_delete;
  sqlpilot->aircraft_edctrl->delete_stmt           = sqlpilot->aircraft_delete;
  sqlpilot->aircraft_edctrl->select_by_id_stmt     = sqlpilot->aircraft_select_by_id;
  edctrl_register_save(sqlpilot->aircraft_edctrl, aircraft_write_entries, sqlpilot);
  edctrl_register_after_change(sqlpilot->aircraft_edctrl, aircraft_after_change, sqlpilot);
  edctrl_register_load_selection(sqlpilot->aircraft_edctrl, aircraft_load_selection, sqlpilot);

  sqlpilot->types_edctrl                 = &sqlpilot->_types_edctrl;
  sqlpilot->types_edctrl->edstate        = EDSTATE_EMPTY;
  sqlpilot->types_edctrl->new_btn        = sqlpilot->types_new_btn;
  sqlpilot->types_edctrl->save_btn       = sqlpilot->types_save_btn;
  sqlpilot->types_edctrl->armdel_btn     = sqlpilot->types_armdel_btn;
  sqlpilot->types_edctrl->todel_lbl      = sqlpilot->types_todel_lbl;
  sqlpilot->types_edctrl->del_btn        = sqlpilot->types_del_btn;
  sqlpilot->types_edctrl->selection      = sqlpilot->types_selection;
  sqlpilot->types_edctrl->selection_show = types_selection_show;
  sqlpilot->types_edctrl->can_delete     = types_can_delete;
  sqlpilot->types_edctrl->delete_stmt    = sqlpilot->types_delete;
  sqlpilot->types_edctrl->select_by_id_stmt    = sqlpilot->types_select_by_id;
  edctrl_register_save(sqlpilot->types_edctrl, types_write_entries, sqlpilot);
  edctrl_register_after_change(sqlpilot->types_edctrl, types_after_change, sqlpilot);
  edctrl_register_load_selection(sqlpilot->types_edctrl, types_load_selection, sqlpilot);

  sqlpilot->airports_edctrl                 = &sqlpilot->_airports_edctrl;
  sqlpilot->airports_edctrl->edstate        = EDSTATE_EMPTY;
  sqlpilot->airports_edctrl->new_btn        = sqlpilot->airports_new_btn;
  sqlpilot->airports_edctrl->save_btn       = sqlpilot->airports_save_btn;
  sqlpilot->airports_edctrl->armdel_btn     = sqlpilot->airports_armdel_btn;
  sqlpilot->airports_edctrl->todel_lbl      = sqlpilot->airports_todel_lbl;
  sqlpilot->airports_edctrl->del_btn        = sqlpilot->airports_del_btn;
  sqlpilot->airports_edctrl->selection      = sqlpilot->airports_selection;
  sqlpilot->airports_edctrl->selection_show = airports_selection_show;
  sqlpilot->airports_edctrl->can_delete     = airports_can_delete;
  sqlpilot->airports_edctrl->delete_stmt    = sqlpilot->airports_delete;
  sqlpilot->airports_edctrl->select_by_id_stmt    = sqlpilot->airports_select_by_id;
  edctrl_register_save(sqlpilot->airports_edctrl, airports_write_entries, sqlpilot);
  edctrl_register_after_change(sqlpilot->airports_edctrl, airports_after_change, sqlpilot);
  edctrl_register_load_selection(sqlpilot->airports_edctrl, airports_load_selection, sqlpilot);

#ifdef USING_GTK_BUILDER
  g_object_unref (G_OBJECT (builder));
#else
  g_object_unref (G_OBJECT(gxml));
#endif

  sqlpilot->flights_stale = 0;
  flights_refresh(sqlpilot);
  sqlpilot->roles_stale = 1;
  sqlpilot->aircraft_stale = 1;
  sqlpilot->types_stale = 1;
  sqlpilot->airports_stale = 1;
  
  flights_load_selection(sqlpilot);
  roles_load_selection(sqlpilot);
  aircraft_load_selection(sqlpilot);
  types_load_selection(sqlpilot);
  aircraft_load_selection(sqlpilot);

  return sqlpilot;
}

void sqlpilot_finalize(Sqlpilot *sqlpilot)
{
  g_slice_free(Sqlpilot, sqlpilot);
}

