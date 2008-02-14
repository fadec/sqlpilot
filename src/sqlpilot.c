#include "sqlpilot.h"
#include "cb/window.h"

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

  sqlpilot->roles_select_all     = db_prep(sqlpilot->db, ROLES_SELECT ";");
  sqlpilot->roles_select_by_id   = db_prep(sqlpilot->db, ROLES_SELECT ROLES_WHERE_ID);
  sqlpilot->roles_insert         = db_prep(sqlpilot->db, ROLES_INSERT);
  sqlpilot->roles_update         = db_prep(sqlpilot->db, ROLES_UPDATE);
  sqlpilot->roles_delete         = db_prep(sqlpilot->db, ROLES_DELETE);

  sqlpilot->aircraft_select_all     = db_prep(sqlpilot->db, AIRCRAFT_SELECT ";");
  sqlpilot->aircraft_select_by_id   = db_prep(sqlpilot->db, AIRCRAFT_SELECT AIRCRAFT_WHERE_ID);
  sqlpilot->aircraft_insert         = db_prep(sqlpilot->db, AIRCRAFT_INSERT);
  sqlpilot->aircraft_update         = db_prep(sqlpilot->db, AIRCRAFT_UPDATE);
  sqlpilot->aircraft_delete         = db_prep(sqlpilot->db, AIRCRAFT_DELETE);

  sqlpilot->types_select_all     = db_prep(sqlpilot->db, TYPES_SELECT ";");
  sqlpilot->types_select_by_id   = db_prep(sqlpilot->db, TYPES_SELECT TYPES_WHERE_ID);
  sqlpilot->types_insert         = db_prep(sqlpilot->db, TYPES_INSERT);
  sqlpilot->types_update         = db_prep(sqlpilot->db, TYPES_UPDATE);
  sqlpilot->types_delete         = db_prep(sqlpilot->db, TYPES_DELETE);

  sqlpilot->airports_select_all     = db_prep(sqlpilot->db, AIRPORTS_SELECT ";");
  sqlpilot->airports_select_by_id   = db_prep(sqlpilot->db, AIRPORTS_SELECT AIRPORTS_WHERE_ID);
  sqlpilot->airports_insert         = db_prep(sqlpilot->db, AIRPORTS_INSERT);
  sqlpilot->airports_update         = db_prep(sqlpilot->db, AIRPORTS_UPDATE);
  sqlpilot->airports_delete         = db_prep(sqlpilot->db, AIRPORTS_DELETE);
  
  /* Set UI components in Sqlpilot struct */
  #define __get_widget(x) GTK_WIDGET(gtk_builder_get_object(builder, (x)));
  sqlpilot->window           = __get_widget("window");
  sqlpilot->flights_sw       = __get_widget("flights_sw");
  sqlpilot->flights_aircraft = __get_widget("flights_aircraft");
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
  sqlpilot->roles_solo       = __get_widget("roles_solo");
  sqlpilot->roles_dual       = __get_widget("dual");
  sqlpilot->roles_instruct   = __get_widget("instruct");
  sqlpilot->aircraft_sw      = __get_widget("aircraft_sw");
  sqlpilot->aircraft_ident   = __get_widget("aircraft_ident");
  sqlpilot->types_sw         = __get_widget("types_sw");
  sqlpilot->types_ident      = __get_widget("types_ident");
  sqlpilot->airports_sw      = __get_widget("airports_sw");
  sqlpilot->airports_ident   = __get_widget("airports_ident");

  #undef __get_widget

  /* Types for model */
  sqlpilot->flights_gtypes[FLIGHTS_COL_ID]       = G_TYPE_INT;
  sqlpilot->flights_gtypes[FLIGHTS_COL_DATE]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_AIRCRAFT] = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_ROLE]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_DEP]      = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_ARR]      = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_AOUT]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_AIN]      = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_DUR]      = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_NIGHT]    = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_INST]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_SIMINST]  = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_HOLD]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_APRCH]    = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_XC]       = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_DLAND]    = G_TYPE_INT;
  sqlpilot->flights_gtypes[FLIGHTS_COL_NLAND]    = G_TYPE_INT;
  sqlpilot->flights_gtypes[FLIGHTS_COL_CREW]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_NOTES]    = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_FLTNO]    = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_SOUT]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_SIN]      = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_SDUR]     = G_TYPE_STRING;
  sqlpilot->flights_gtypes[FLIGHTS_COL_TRIP]     = G_TYPE_STRING;

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

  g_object_unref (G_OBJECT (builder));
  return sqlpilot;
}

void sqlpilot_finalize(Sqlpilot *sqlpilot)
{
  g_slice_free(Sqlpilot, sqlpilot);
}

