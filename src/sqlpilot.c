#include "sqlpilot.h"
#include "cb/cb.h"

void barf(const char *message)
{
  if (message) fprintf(stderr, "Error: %s", message);
  exit(EXIT_BARF);
}

Sqlpilot *sqlpilot_new(void)
{
  Sqlpilot         *sqlpilot;
  GtkBuilder       *builder;
  GError           *err;

  /* Open Database */
  sqlpilot = g_slice_new0(Sqlpilot);
  if (!(sqlpilot->db = db_open(DB_FILE))) {
    barf ("Couldn't open database.");
    return NULL;
  }

  /* Build UI from XML */
  builder = gtk_builder_new ();
  if (gtk_builder_add_from_file (builder, UI_XML_FILE, &err) == 0) {
    barf (err->message);
    g_error_free (err);
    return NULL;
  }

  /* Connect Signals */
  gtk_builder_connect_signals(builder, sqlpilot);

  /* Set DB statements */
  sqlpilot->flights_select_all   = db_prep(sqlpilot->db, FLIGHTS_SELECT ";");
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

  sqlpilot->types_select_all     = db_prep(sqlpilot->db, TYPES_SELECT TYPES_GROUP_BY ";");
  sqlpilot->types_select_by_id   = db_prep(sqlpilot->db, TYPES_SELECT TYPES_WHERE_ID TYPES_GROUP_BY ";");
  sqlpilot->types_insert         = db_prep(sqlpilot->db, TYPES_INSERT);
  sqlpilot->types_update         = db_prep(sqlpilot->db, TYPES_UPDATE);
  sqlpilot->types_delete         = db_prep(sqlpilot->db, TYPES_DELETE);

  sqlpilot->airports_select_all     = db_prep(sqlpilot->db, AIRPORTS_SELECT AIRPORTS_GROUP_BY ";");
  sqlpilot->airports_select_by_id   = db_prep(sqlpilot->db, AIRPORTS_SELECT AIRPORTS_WHERE_ID AIRPORTS_GROUP_BY ";");
  sqlpilot->airports_insert         = db_prep(sqlpilot->db, AIRPORTS_INSERT);
  sqlpilot->airports_update         = db_prep(sqlpilot->db, AIRPORTS_UPDATE);
  sqlpilot->airports_delete         = db_prep(sqlpilot->db, AIRPORTS_DELETE);
  
  /* Set UI components in Sqlpilot struct */
  #define __get_widget(x) GTK_WIDGET(gtk_builder_get_object(builder, (x)));
  sqlpilot->window           = __get_widget("window");
  sqlpilot->flights_sw       = __get_widget("flights_sw");
  sqlpilot->flights_aircraft = __get_widget("flights_aircraft");
  sqlpilot->flights_utc      = __get_widget("flights_utc");
  sqlpilot->flights_utc_lbl  = __get_widget("flights_utc_lbl");
  sqlpilot->flights_date     = __get_widget("flights_date");
  sqlpilot->flights_role     = __get_widget("flights_role");
  sqlpilot->flights_dep      = __get_widget("flights_dep");
  sqlpilot->flights_arr      = __get_widget("flights_arr");
  sqlpilot->flights_aout     = __get_widget("flights_aout");
  sqlpilot->flights_ain      = __get_widget("flights_ain");
  sqlpilot->flights_dur      = __get_widget("flights_dur");
  sqlpilot->flights_night    = __get_widget("flights_night");
  sqlpilot->flights_inst     = __get_widget("flights_inst");
  sqlpilot->flights_siminst  = __get_widget("flights_siminst");
  sqlpilot->flights_hold     = __get_widget("flights_hold");
  sqlpilot->flights_aprch    = __get_widget("flights_aprch");
  sqlpilot->flights_xc       = __get_widget("flights_xc");
  sqlpilot->flights_dland    = __get_widget("flights_dland");
  sqlpilot->flights_nland    = __get_widget("flights_nland");
  sqlpilot->flights_crew     = __get_widget("flights_crew");
  sqlpilot->flights_notes    = __get_widget("flights_notes");
  sqlpilot->flights_fltno    = __get_widget("flights_fltno");
  sqlpilot->flights_sout     = __get_widget("flights_sout");
  sqlpilot->flights_sin      = __get_widget("flights_sin");
  sqlpilot->flights_sdur     = __get_widget("flights_sdur");
  sqlpilot->flights_trip     = __get_widget("flights_trip");
  sqlpilot->roles_sw         = __get_widget("roles_sw");
  sqlpilot->roles_ident      = __get_widget("roles_ident");
  sqlpilot->roles_name       = __get_widget("roles_name");
  sqlpilot->roles_pic        = __get_widget("roles_pic");
  sqlpilot->roles_sic        = __get_widget("roles_sic");
  sqlpilot->roles_fe         = __get_widget("roles_fe");
  sqlpilot->roles_solo       = __get_widget("roles_solo");
  sqlpilot->roles_dual       = __get_widget("roles_dual");
  sqlpilot->roles_instruct   = __get_widget("roles_instruct");
  sqlpilot->aircraft_sw      = __get_widget("aircraft_sw");
  sqlpilot->aircraft_ident   = __get_widget("aircraft_ident");
  sqlpilot->aircraft_type    = __get_widget("aircraft_type");
  sqlpilot->aircraft_fleetno = __get_widget("aircraft_fleetno");
  sqlpilot->aircraft_notes   = __get_widget("aircraft_notes");
  sqlpilot->types_sw         = __get_widget("types_sw");
  sqlpilot->types_ident      = __get_widget("types_ident");
  sqlpilot->types_make       = __get_widget("types_make");
  sqlpilot->types_model       = __get_widget("types_model");
  sqlpilot->types_airplane    = __get_widget("types_airplane");
  sqlpilot->types_rotorcraft  = __get_widget("types_rotorcraft");
  sqlpilot->types_glider      = __get_widget("types_glider");
  sqlpilot->types_lta         = __get_widget("types_lta");
  sqlpilot->types_poweredlift = __get_widget("types_poweredlift");
  sqlpilot->types_ppc         = __get_widget("types_ppc");
  sqlpilot->types_weightshift = __get_widget("types_weightshift");
  sqlpilot->types_heli        = __get_widget("types_heli");
  sqlpilot->types_gyro        = __get_widget("types_gyro");
  sqlpilot->types_airship     = __get_widget("types_airship");
  sqlpilot->types_balloon     = __get_widget("types_balloon");
  sqlpilot->types_single      = __get_widget("types_single");
  sqlpilot->types_multi       = __get_widget("types_multi");
  sqlpilot->types_land        = __get_widget("types_land");
  sqlpilot->types_sea         = __get_widget("types_sea");
  sqlpilot->types_turbine     = __get_widget("types_turbine");
  sqlpilot->types_jet         = __get_widget("types_jet");
  sqlpilot->types_highperf    = __get_widget("types_highperf");
  sqlpilot->types_retract     = __get_widget("types_retract");
  sqlpilot->types_complex     = __get_widget("types_complex");
  sqlpilot->types_pressurized = __get_widget("types_pressurized");
  sqlpilot->types_large       = __get_widget("types_large");
  sqlpilot->types_sport       = __get_widget("types_sport");
  sqlpilot->types_ultralight  = __get_widget("types_ultralight");
  sqlpilot->types_footlaunch  = __get_widget("types_footlaunch");
  sqlpilot->types_sim         = __get_widget("types_sim");
  sqlpilot->types_ftd         = __get_widget("types_ftd");
  sqlpilot->types_total       = __get_widget("types_total");
  sqlpilot->airports_sw       = __get_widget("airports_sw");
  sqlpilot->airports_ident    = __get_widget("airports_ident");
  sqlpilot->airports_icao     = __get_widget("airports_icao");
  sqlpilot->airports_name     = __get_widget("airports_name");
  sqlpilot->airports_tzone    = __get_widget("airports_tzone");
  sqlpilot->airports_notes    = __get_widget("airports_notes");
  #undef __get_widget

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

  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_utc), FALSE);
  gtk_label_set_text(GTK_LABEL(sqlpilot->flights_utc_lbl), "Local");

  g_object_unref (G_OBJECT (builder));
  return sqlpilot;
}

void sqlpilot_finalize(Sqlpilot *sqlpilot)
{
  g_slice_free(Sqlpilot, sqlpilot);
}

