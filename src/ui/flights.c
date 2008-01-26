#include "sqlpilot.h"

static GtkWidget *build_log_view(Logbook *);
static GtkWidget *build_edit_view(Logbook *);

GtkWidget *build_flights_pane(Logbook *logbook)
{
  GtkWidget *b_outer;
  GtkWidget *edit_pane;
  GtkWidget *log_pane;

  b_outer = gtk_hbox_new(FALSE, 0);
  log_pane = build_log_view(logbook);
  gtk_box_pack_end(GTK_BOX(b_outer), log_pane, TRUE, TRUE, 0);

  edit_pane = build_edit_view(logbook);
  gtk_box_pack_start(GTK_BOX(b_outer), edit_pane, FALSE, TRUE, 0);

  gtk_widget_show(b_outer);

  logbook->flights_pane = b_outer;

  return b_outer;
}

static void row_selection_changed(GtkTreeSelection *selection, gpointer logbook)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *author;

  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      gtk_tree_model_get (model, &iter, 0, &author, -1);
      g_print ("You selected a book by %s\n", author);
      g_free (author);
    }
}

static GtkWidget *build_log_view(Logbook *logbook)
{
  DBStatement *stmt;
  GtkWidget *sw, *treeview;
  GtkTreeModel *treemodel;
  GtkTreeSelection *select;
  const char *sql = "select aircraft, depart, arrive, out_at, in_at, duration from flights order by out_at";

  stmt = db_prep(logbook->db, sql);
  sw = build_query_stmt_widget(stmt, &treeview, &treemodel);
  db_finalize(stmt);

  // setup selection callback
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed",
                  G_CALLBACK (row_selection_changed),
                  NULL);


  // side effects and return
  logbook->flights_log_sw = sw;
  logbook->flights_log_treemodel = treemodel;
  logbook->flights_log_treeview = treeview;

  return sw;
}

static void update_clicked (GtkButton* button, gpointer *logbook)
{
  printf("%s\n", gtk_entry_get_text(LOGBOOK(logbook)->flights_aircraft_entry));
  printf("%s\n", gtk_entry_get_text(LOGBOOK(logbook)->flights_role_entry));
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

static GtkWidget *build_edit_view(Logbook *logbook)
{
  GtkWidget *vbox;
  GtkWidget *table;
  GtkWidget *buttons, *update, *cancel, *new, *add;
  int row = 0;
  
  vbox = gtk_vbox_new(FALSE, 0);

  /* Text Fields */
  table = gtk_table_new(8, 2, FALSE); // first arg = number of text fields!
  gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, TRUE, 0);

  logbook->flights_aircraft_entry = add_text_field(table, row++, "Aircraft");
  logbook->flights_role_entry = add_text_field(table, row++, "Role");
  add_text_field(table, row++, "From");
  add_text_field(table, row++, "To");
  add_text_field(table, row++, "Out");
  add_text_field(table, row++, "In");
  add_text_field(table, row++, "Night");
  add_text_field(table, row++, "Instrument");

  /* Buttons */
  buttons = gtk_hbox_new(FALSE, 0);
  update = gtk_button_new_with_label("Update");
  cancel = gtk_button_new_with_label("Cancel");
  add = gtk_button_new_with_label("Add");
  new = gtk_button_new_with_label("New");


  gtk_signal_connect(GTK_OBJECT(update), "clicked", GTK_SIGNAL_FUNC(update_clicked), logbook);

  gtk_box_pack_start(GTK_BOX(buttons), update, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), cancel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), new, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), add, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), buttons, FALSE, FALSE, 0);

  gtk_widget_show_all(vbox);
  
  return vbox;
}

