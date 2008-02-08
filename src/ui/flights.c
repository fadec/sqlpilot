#include "sqlpilot.h"

static GtkWidget *build_log_view(Logbook *);
static GtkWidget *build_edit_view(Logbook *);

GtkWidget *logbook_page_flights_build(Logbook *logbook)
{
  DBStatement *stmt;
  GtkWidget *db_ui;
  stmt = db_prep(logbook->db, "select * from flights;");
  db_ui = build_db_ui(stmt);

  logbook->flights_select = stmt;
  logbook->page_flights = db_ui;
  return db_ui;
}

GtkWidget *logbook_flights_page_build(Logbook *logbook)
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

void flight_load_edit_fields(Flight *f, Logbook *logb)
{
  f->aircraft = gtk_entry_get_text(GTK_ENTRY(logb->flights_aircraft_entry));
  f->role = gtk_entry_get_text(GTK_ENTRY(logb->flights_role_entry));
  f->depart = gtk_entry_get_text(GTK_ENTRY(logb->flights_depart_entry));
  f->arrive =  gtk_entry_get_text(GTK_ENTRY(logb->flights_arrive_entry));
  f->scheduled_out_at = gtk_entry_get_text(GTK_ENTRY(logb->flights_scheduled_out_at_entry));
  f->scheduled_in_at = gtk_entry_get_text(GTK_ENTRY(logb->flights_scheduled_in_at_entry));
  f->scheduled_duration = gtk_entry_get_text(GTK_ENTRY(logb->flights_scheduled_duration_entry));
  f->out_at = gtk_entry_get_text(GTK_ENTRY(logb->flights_out_at_entry));
  f->in_at = gtk_entry_get_text(GTK_ENTRY(logb->flights_in_at_entry));
  f->duration = gtk_entry_get_text(GTK_ENTRY(logb->flights_duration_entry));
  f->night = gtk_entry_get_text(GTK_ENTRY(logb->flights_night_entry));
  f->instrument = gtk_entry_get_text(GTK_ENTRY(logb->flights_instrument_entry));

  return f;
}

void flight_set_list_store(Flight *f, Logbook *logb, GtkTreeIter *iter)
{
  gtk_list_store_set(logb->flights_log_treemodel, iter,
		     logb->flights_log_id_column, f->id,
		     logb->flights_log_aircraft_column, f->aircraft,
		     logb->flights_log_role_column, f->role,
		     logb->flights_log_depart_column, f->depart,
		     logb->flights_log_arrive_column, f->arrive,
		     logb->flights_log_out_at_column, f->out_at,
		     logb->flights_log_in_at_column, f->in_at,
		     logb->flights_log_duration_column, f->duration,
		     logb->flights_log_scheduled_out_at_column, f->scheduled_out_at,
		     logb->flights_log_scheduled_in_at_column, f->scheduled_in_at,
		     logb->flights_log_scheduled_duration_column, f->scheduled_duration,
		     logb->flights_log_night_column, f->night,
		     logb->flights_log_instrument_column, f->instrument,
		     -1);
}

static void update_selected_row(Logbook *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char sql[4096];
  Flight f = {0};
  
  flight_load_edit_fields(&f, logb);
  
  gtk_tree_selection_get_selected(logb->flights_log_selected_row, &model, &iter);

  gtk_tree_model_get(model, &iter, logb->flights_log_id_column, &f.id, -1);

  flight_set_list_store(&f, logb, &iter);

  snprintf(sql, 4096,
	   "update flights set aircraft = '%s', "
	   "role = '%s', "
	   "depart = '%s', "
	   "arrive = '%s', "
	   "scheduled_out_at = '%s', "
	   "scheduled_in_at = '%s', "
	   "scheduled_duration = '%s', "
	   "out_at = '%s', "
	   "in_at = '%s', "
	   "duration = '%s', "
	   "night = '%s', "
	   "instrument = '%s' "
	   "where id = '%s';",
	   f.aircraft, f.role, f.depart, f.arrive, f.scheduled_out_at, f.scheduled_in_at, f.scheduled_duration,
	   f.out_at, f.in_at, f.duration, f.night, f.instrument, f.id);
  
  db_exec_simple(logb->db, sql);
}

static void sync_with_selected_row(Logbook *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *id, *aircraft, *role, *depart, *arrive, *scheduled_out_at, *scheduled_in_at,
    *scheduled_duration, *out_at, *in_at, *duration, *night, *instrument;

  if (gtk_tree_selection_get_selected (logb->flights_log_selected_row, &model, &iter))
    {
      gtk_tree_model_get(model, &iter,
			 logb->flights_log_id_column, &id,
			 logb->flights_log_aircraft_column, &aircraft,
			 logb->flights_log_role_column, &role,
			 logb->flights_log_depart_column, &depart,
			 logb->flights_log_arrive_column, &arrive,
			 logb->flights_log_out_at_column, &out_at,
			 logb->flights_log_in_at_column, &in_at,
			 logb->flights_log_duration_column, &duration,
			 logb->flights_log_scheduled_out_at_column, &scheduled_out_at,
			 logb->flights_log_scheduled_in_at_column, &scheduled_in_at,
			 logb->flights_log_scheduled_duration_column, &scheduled_duration,
			 logb->flights_log_night_column, &night,
			 logb->flights_log_instrument_column, &instrument,
			 -1);

      gtk_entry_set_text(GTK_ENTRY(logb->flights_aircraft_entry), EMPTY_IF_NULL(aircraft));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_role_entry), EMPTY_IF_NULL(role));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_depart_entry), EMPTY_IF_NULL(depart));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_arrive_entry), EMPTY_IF_NULL(arrive));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_scheduled_out_at_entry), EMPTY_IF_NULL(scheduled_out_at));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_scheduled_in_at_entry), EMPTY_IF_NULL(scheduled_in_at));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_scheduled_duration_entry), EMPTY_IF_NULL(scheduled_duration));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_out_at_entry), EMPTY_IF_NULL(out_at));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_in_at_entry), EMPTY_IF_NULL(in_at));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_duration_entry), EMPTY_IF_NULL(duration));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_night_entry), EMPTY_IF_NULL(night));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_instrument_entry), EMPTY_IF_NULL(instrument));
      
      sscanf(id, "%u", &logb->flights_log_selected_id);

      g_free(id);
      g_free(aircraft);
      g_free(role);
      g_free(depart);
      g_free(arrive);
      g_free(scheduled_out_at);
      g_free(scheduled_in_at);
      g_free(scheduled_duration);
      g_free(out_at);
      g_free(in_at);
      g_free(duration);
      g_free(night);
      g_free(instrument);
    }
}


static void row_selection_changed(GtkTreeSelection *selection, gpointer logbook)
{
  Logbook *logb;
  GtkTreeIter iter;
  GtkTreeModel *model;

  logb = LOGBOOK(logbook);

  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      logb->flights_log_selected_row = selection;
      sync_with_selected_row(logb);
      
    }
}

static GtkWidget *build_log_view(Logbook *logbook)
{
  DBStatement *stmt;
  GtkWidget *sw, *treeview;
  GtkTreeModel *treemodel;
  GtkTreeSelection *select;
  const char *sql =
    "select id, aircraft, role, depart, arrive, scheduled_out_at, scheduled_in_at, scheduled_duration, "
    "out_at, in_at, duration, night, instrument from flights order by scheduled_out_at";
  int col;

  stmt = db_prep(logbook->db, sql);
  sw = build_query_stmt_widget(stmt, &treeview, &treemodel);
  db_finalize(stmt);

  // setup selection callback
  select = gtk_tree_view_get_selection (GTK_TREE_VIEW (treeview));
  gtk_tree_selection_set_mode (select, GTK_SELECTION_SINGLE);
  g_signal_connect (G_OBJECT (select), "changed",
                  G_CALLBACK (row_selection_changed),
                  logbook);


  // side effects and return
  //logbook->flights_log_sql = sql; it's on the stack duh
  col = 0;
  logbook->flights_log_id_column = col++;
  logbook->flights_log_aircraft_column = col++;
  logbook->flights_log_role_column = col++;
  logbook->flights_log_depart_column = col++;
  logbook->flights_log_arrive_column = col++;
  logbook->flights_log_scheduled_out_at_column = col++;
  logbook->flights_log_scheduled_in_at_column = col++;
  logbook->flights_log_scheduled_duration_column = col++;
  logbook->flights_log_out_at_column = col++;
  logbook->flights_log_in_at_column = col++;
  logbook->flights_log_duration_column = col++;
  logbook->flights_log_night_column = col++;
  logbook->flights_log_instrument_column = col++;
  logbook->flights_log_selected_row = NULL;
  logbook->flights_log_selected_id = 0; // nothing value
  logbook->flights_log_sw = sw;
  logbook->flights_log_treemodel = treemodel;
  logbook->flights_log_treeview = treeview;

  return sw;
}

static void update_clicked (GtkButton* button, gpointer logbook)
{
  update_selected_row(LOGBOOK(logbook));
}

static void cancel_clicked(GtkButton *button, Logbook *logbook)
{
  sync_with_selected_row(logbook);
}

static void add_clicked(GtkButton *button, Logbook *logbook)
{
  Flight f;
  GtkTreeIter iter;
  flight_load_edit_fields(&f, logbook);
  f.id = "100";

  gtk_list_store_prepend(logbook->flights_log_treemodel, &iter);

  flight_set_list_store(&f, logbook, &iter);
  
  
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
  table = gtk_table_new(12, 2, FALSE); // first arg = number of text fields!
  gtk_box_pack_start(GTK_BOX(vbox), table, FALSE, TRUE, 0);

  logbook->flights_aircraft_entry = add_text_field(table, row++, "Aircraft");
  logbook->flights_role_entry     = add_text_field(table, row++, "Role");
  logbook->flights_depart_entry   = add_text_field(table, row++, "From");
  logbook->flights_arrive_entry   = add_text_field(table, row++, "To");
  logbook->flights_scheduled_out_at_entry = add_text_field(table, row++, "Sched Out");
  logbook->flights_scheduled_in_at_entry = add_text_field(table, row++, "Sched In");
  logbook->flights_scheduled_duration_entry = add_text_field(table, row++, "Sched Dur");
  logbook->flights_out_at_entry = add_text_field(table, row++, "Out");
  logbook->flights_in_at_entry = add_text_field(table, row++, "In");
  logbook->flights_duration_entry = add_text_field(table, row++, "Duration");
  logbook->flights_night_entry = add_text_field(table, row++, "Night");
  logbook->flights_instrument_entry = add_text_field(table, row++, "Instrument");


  /* Buttons */
  buttons = gtk_hbox_new(FALSE, 0);
  update = gtk_button_new_with_label("Update");
  cancel = gtk_button_new_with_label("Cancel");
  add = gtk_button_new_with_label("Add");
  new = gtk_button_new_with_label("New");

  gtk_signal_connect(GTK_OBJECT(update), "clicked", GTK_SIGNAL_FUNC(update_clicked), logbook);
  gtk_signal_connect(GTK_OBJECT(cancel), "clicked", GTK_SIGNAL_FUNC(cancel_clicked), logbook);
  gtk_signal_connect(GTK_OBJECT(add), "clicked", GTK_SIGNAL_FUNC(add_clicked), logbook);


  gtk_box_pack_start(GTK_BOX(buttons), update, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), cancel, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), new, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(buttons), add, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), buttons, FALSE, FALSE, 0);

  gtk_widget_show_all(vbox);
  
  return vbox;
}

