#include "sqlpilot.h"

static GtkWidget *build_log_view(Interface *);
static GtkWidget *build_edit_view(Interface *);

GtkWidget *build_flights_pane(Interface *iface)
{
  GtkWidget *b_outer;
  GtkWidget *edit_pane;
  GtkWidget *log_pane;

  b_outer = gtk_hbox_new(FALSE, 0);
  log_pane = build_log_view(iface);
  gtk_box_pack_end(GTK_BOX(b_outer), log_pane, TRUE, TRUE, 0);

  edit_pane = build_edit_view(iface);
  gtk_box_pack_start(GTK_BOX(b_outer), edit_pane, FALSE, TRUE, 0);

  gtk_widget_show(b_outer);

  return b_outer;
}

static GtkWidget *build_log_view(Interface *iface)
{
  DBStatement *stmt;
  GtkWidget *view;
  const char *sql = "select aircraft, depart, arrive, out_at, in_at, duration from flights order by out_at";

  stmt = db_prep(sqlpilot->logbook->db, sql);
  view = build_query_stmt_widget(stmt);
  db_finalize(stmt);
  
  iface->flights_log_sw = view;

  return view;
}

static GtkWidget *update_clicked (GtkButton* button, gpointer *iface)
{
  printf("%s\n", gtk_entry_get_text(sqlpilot->interface->flights_aircraft_entry));
  printf("%s\n", gtk_entry_get_text(sqlpilot->interface->flights_role_entry));
}

static GtkWidget *add_text_field(GtkWidget *table, int row, const char *label_text)
{
  GtkWidget *entry;
  GtkWidget *label;
  GtkWidget *label_hbox;
  entry = gtk_entry_new();
  label = gtk_label_new(label_text);

  label_hbox = gtk_hbox_new(FALSE, 0);
  gtk_box_pack_end(GTK_BOX(label_hbox), label, FALSE, TRUE, 0);
  gtk_table_attach_defaults(GTK_TABLE(table), label_hbox, 0, 1, row, row + 1);
  gtk_table_attach_defaults(GTK_TABLE(table), entry, 1, 2, row, row + 1);

  return entry;
}

static GtkWidget *build_edit_view(Interface *iface)
{
  GtkWidget *vbox;
  GtkWidget *table;
  GtkWidget *buttons, *update, *cancel, *new, *add;
  int row = 0;
  
  vbox = gtk_vbox_new(FALSE, 0);

  /* Text Fields */
  table = gtk_table_new(8, 2, FALSE); // first arg = number of text fields!
  gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, TRUE, 0);

  iface->flights_aircraft_entry = add_text_field(table, row++, "Aircraft");
  iface->flights_role_entry = add_text_field(table, row++, "Role");
  add_text_field(table, row++, "From");
  add_text_field(table, row++, "To");
  add_text_field(table, row++, "Out");
  add_text_field(table, row++, "In");
  add_text_field(table, row++, "Night");
  add_text_field(table, row++, "Instrument");

  /* Buttons */
  buttons = gtk_hbox_new(TRUE, 0);
  update = gtk_button_new_with_label("Update");
  cancel = gtk_button_new_with_label("Revert");

  gtk_signal_connect(update, "clicked", update_clicked, sqlpilot->interface);

  gtk_box_pack_start(GTK_BOX(buttons), update, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), cancel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), buttons, FALSE, FALSE, 0);

  gtk_widget_show_all(vbox);
  
  return vbox;
}

