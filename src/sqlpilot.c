#include "sqlpilot.h"

void barf(const char *message)
{
  if (message) fprintf(stderr, "Error: %s", message);
  exit(EXIT_BARF);
}

Sqlpilot *sqlpilot_new(void)
{
  Sqlpilot        *sqlpilot;
  GtkBuilder      *builder;
  GError          *err;

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
  sqlpilot->flights_select = db_prep(sqlpilot->db, "select * from flights;");
  sqlpilot->flights_insert = db_prep(sqlpilot->db, FLIGHTS_INSERT);

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

  g_object_unref (G_OBJECT (builder));
  return sqlpilot;
}

void sqlpilot_finalize(Sqlpilot *sqlpilot)
{
  g_slice_free(Sqlpilot, sqlpilot);
}

