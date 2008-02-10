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
  sqlpilot->flights_select = db_prep(sqlpilot->db, FLIGHTS_SELECT ";");
  sqlpilot->flights_select_by_id = db_prep(sqlpilot->db, FLIGHTS_SELECT FLIGHTS_WHERE_ID);
  sqlpilot->flights_insert = db_prep(sqlpilot->db, FLIGHTS_INSERT);
  sqlpilot->flights_update = db_prep(sqlpilot->db, FLIGHTS_UPDATE);
  sqlpilot->flights_delete = db_prep(sqlpilot->db, FLIGHTS_DELETE);
  
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
  store_build_query_stmt_widget(sqlpilot->flights_select, &sqlpilot->flights_treeview, &sqlpilot->flights_treemodel);
  gtk_widget_show_all(sqlpilot->flights_treeview);
  gtk_container_add(GTK_CONTAINER(sqlpilot->flights_sw), sqlpilot->flights_treeview);

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

