#include "sqlpilot.h"


static void flights_load_entries_from_selection(Sqlpilot *logb)
{
/*   GtkTreeIter iter; */
/*   GtkTreeModel *model; */
/*   gchar *id, *aircraft, *role, *depart, *arrive, *scheduled_out_at, *scheduled_in_at, */
/*     *scheduled_duration, *out_at, *in_at, *duration, *night, *instrument; */

/*   if (gtk_tree_selection_get_selected (logb->flights_log_selected_row, &model, &iter)) */
/*     { */
/*       gtk_tree_model_get(model, &iter, */
/* 			 logb->flights_log_id_column, &id, */
/* 			 logb->flights_log_aircraft_column, &aircraft, */
/* 			 logb->flights_log_role_column, &role, */
/* 			 logb->flights_log_depart_column, &depart, */
/* 			 logb->flights_log_arrive_column, &arrive, */
/* 			 logb->flights_log_out_at_column, &out_at, */
/* 			 logb->flights_log_in_at_column, &in_at, */
/* 			 logb->flights_log_duration_column, &duration, */
/* 			 logb->flights_log_scheduled_out_at_column, &scheduled_out_at, */
/* 			 logb->flights_log_scheduled_in_at_column, &scheduled_in_at, */
/* 			 logb->flights_log_scheduled_duration_column, &scheduled_duration, */
/* 			 logb->flights_log_night_column, &night, */
/* 			 logb->flights_log_instrument_column, &instrument, */
/* 			 -1); */

/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_aircraft_entry), EMPTY_IF_NULL(aircraft)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_role_entry), EMPTY_IF_NULL(role)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_depart_entry), EMPTY_IF_NULL(depart)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_arrive_entry), EMPTY_IF_NULL(arrive)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_scheduled_out_at_entry), EMPTY_IF_NULL(scheduled_out_at)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_scheduled_in_at_entry), EMPTY_IF_NULL(scheduled_in_at)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_scheduled_duration_entry), EMPTY_IF_NULL(scheduled_duration)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_out_at_entry), EMPTY_IF_NULL(out_at)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_in_at_entry), EMPTY_IF_NULL(in_at)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_duration_entry), EMPTY_IF_NULL(duration)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_night_entry), EMPTY_IF_NULL(night)); */
/*       gtk_entry_set_text(GTK_ENTRY(logb->flights_instrument_entry), EMPTY_IF_NULL(instrument)); */
      
/*       sscanf(id, "%u", &logb->flights_log_selected_id); */

/*       g_free(id); */
/*       g_free(aircraft); */
/*       g_free(role); */
/*       g_free(depart); */
/*       g_free(arrive); */
/*       g_free(scheduled_out_at); */
/*       g_free(scheduled_in_at); */
/*       g_free(scheduled_duration); */
/*       g_free(out_at); */
/*       g_free(in_at); */
/*       g_free(duration); */
/*       g_free(night); */
/*       g_free(instrument); */
/*     } */
}

void entry_clamp_text(GtkEntry *entry, int length, int setcase)
{
  const gchar *text;
  gchar *clamped;
  
  text = gtk_entry_get_text(entry);

  switch (setcase) {
  case 1: clamped = g_ascii_strup(text, length);
    break;
  case -1: clamped = g_ascii_strdown(text, length);
    break;
  default: clamped = g_strndup(text, length);
  }

  gtk_entry_set_text(entry, clamped);
  g_free(clamped);
  /* *text need not be freed per gtk_entry_get_text docs */
}

void entry_format_time_partial(GtkEntry *entry, char separator)
{
  const char *ptr = NULL;
  char result[6]; 		/* 00:00 + null = 6 chars  */
  int i = 0;

  ptr = gtk_entry_get_text(entry);

  while (*ptr != '\0' && i < 5) {
    if (i == 2) {
      result[i] = separator;
      i++;
    }
    if (*ptr >= '0' && *ptr <= '9') {
      result[i] = *ptr;
      i++;
    }
    ptr++;
  }
  result[i] = '\0';

/*   if (gtk_editable_get_position(GTK_EDITABLE(entry)) == 3) */
/*     gtk_editable_set_position(GTK_EDITABLE(entry), 4); */

  gtk_entry_set_text(entry, result);


  fprintf(stderr, ".");
}

/* Returns an allocated string */
gchar *text_view_get_text(GtkTextView *tv)
{
  GtkTextIter start, end;
  GtkTextBuffer *buffer;
  
  buffer = gtk_text_view_get_buffer(tv);
  gtk_text_buffer_get_start_iter(buffer, &start);
  gtk_text_buffer_get_end_iter(buffer, &end);
  return gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
}

/* find or create a record the value of a column and return the id */
DBint64 id_of_with_insert(DB *db, const char *table, const char *column, const char *value)
{
  #define __sql_size 500
  DBStatement *select, *insert;
  DBint64 id;
  char sql[__sql_size];

  snprintf(sql, __sql_size, "select id from %s where %s = ?;", table, column);
  select = db_prep(db, sql);
  db_bind_text(select, 1, value);

  if (db_step(select) == DB_DONE) {
    snprintf(sql, __sql_size, "insert into %s (%s) values (?);", table, column);
    insert = db_prep(db, sql);
    db_bind_text(insert, 1, value);
    db_step(insert);
    id = db_last_insert_rowid(db);
    db_finalize(insert);
  } else {
    id = db_column_int64(select, 0);
  }

  db_finalize(select);

  return id;
  #undef __sql_size
}

DBint64 aircraft_id_of_ident_with_insert(DB *db, const char *ident)
{
  return id_of_with_insert(db, "aircraft", "ident", ident);
}
DBint64 roles_id_of_ident_with_insert(DB *db, const char *ident)
{
  return id_of_with_insert(db, "roles", "ident", ident);
}
DBint64 airports_id_of_ident_with_insert(DB *db, const char *ident)
{
  return id_of_with_insert(db, "airports", "ident", ident);
}

void on_window_destroy(GtkObject *object, Sqlpilot *sqlpilot)
{
  gtk_main_quit();
}

void on_flights_update_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  fprintf(stderr, "update_clicked");
}

void on_flights_insert_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  const gchar
    *aircraft,
    *role,
    *dep,
    *arr,
    *date,
    *aout,
    *ain,
    *dur,
    *night,
    *inst,
    *siminst,
    *aprch,
    *fltno,
    *sout,
    *sin,
    *sdur,
    *trip;
  gchar *crew, *notes;
  gint dland, nland;
  gboolean xc, hold;
  DBStatement *stmt;
  int i;

  aircraft = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_aircraft));
  role     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_role));
  dep      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_dep));
  arr      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_arr));
  date     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_date));
  aout     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_aout));
  ain      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_ain));
  dur      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_dur));
  night    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_night));
  inst     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_inst));
  siminst  = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_siminst));
  aprch    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_aprch));
  fltno    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_fltno));
  sout     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_sout));
  sin      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_sin));
  sdur     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_sdur));
  trip     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_trip));
  dland    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sqlpilot->flights_dland));
  nland    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sqlpilot->flights_nland));
  hold     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_hold));
  xc       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_xc));
  crew     = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->flights_crew));
  notes    = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->flights_notes));
  
  i = 1;
  stmt = sqlpilot->flights_insert;
  db_bind_int64(stmt, i++, aircraft_id_of_ident_with_insert(sqlpilot->db, aircraft));
  db_bind_int64(stmt, i++, roles_id_of_ident_with_insert(sqlpilot->db, role));
  db_bind_int64(stmt, i++, airports_id_of_ident_with_insert(sqlpilot->db, dep));
  db_bind_int64(stmt, i++, airports_id_of_ident_with_insert(sqlpilot->db, arr));
  db_bind_text(stmt, i++, date);
  db_bind_text(stmt, i++, aout);
  db_bind_text(stmt, i++, ain);
  db_bind_text(stmt, i++, dur);
  db_bind_text(stmt, i++, night);
  db_bind_text(stmt, i++, inst);
  db_bind_text(stmt, i++, siminst);
  db_bind_int(stmt, i++, hold);
  db_bind_text(stmt, i++, aprch);
  db_bind_int(stmt, i++, xc);
  db_bind_int(stmt, i++, dland);
  db_bind_int(stmt, i++, nland);
  db_bind_text(stmt, i++, crew);
  db_bind_text(stmt, i++, notes);
  db_bind_text(stmt, i++, fltno);
  db_bind_text(stmt, i++, sout);
  db_bind_text(stmt, i++, sin);
  db_bind_text(stmt, i++, sdur);
  db_bind_text(stmt, i++, trip);
  db_step(stmt);
  db_reset(stmt);

  g_free(crew);
  g_free(notes);

  fprintf(stderr, "insert_clicked\n");
}

void on_flights_trip_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  fprintf(stderr, "%s\n", text);
}

void on_flights_aircraft_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_text(entry, 6, 1);
  //  fprintf(stderr, "%s\n", text);
}
void on_flights_dep_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_text(entry, 4, 1);
}
void on_flights_arr_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_text(entry, 4, 1);
}

void on_flights_aout_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, ':');
}
void on_flights_ain_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, ':');
}
void on_flights_sout_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, ':');
}
void on_flights_sin_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, ':');
}
void on_flights_dur_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, '+');
}
void on_flights_sdur_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, '+');
}

void on_flights_selection_changed(GtkTreeSelection *selection, Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;

  if (gtk_tree_selection_get_selected (selection, &model, &iter))
    {
      logb->flights_selection = selection;
      flights_load_entries_from_selection(logb);      
    }
}
