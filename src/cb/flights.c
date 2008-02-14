#include "sqlpilot.h"

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
}

static void flights_load_entries_from_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id,
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
    *trip,
    *crew,
    *notes,
    *dland,
    *nland,
    *xc,
    *hold;

  int _dland, _nland;

  if (gtk_tree_selection_get_selected (logb->flights_selection, &model, &iter))
    {
      gtk_tree_model_get(model, &iter,
			 FLIGHTS_COL_ID, &id,
			 FLIGHTS_COL_DATE, &date,
			 FLIGHTS_COL_AIRCRAFT, &aircraft,
			 FLIGHTS_COL_ROLE, &role,
			 FLIGHTS_COL_DEP, &dep,
			 FLIGHTS_COL_ARR, &arr,
			 FLIGHTS_COL_AOUT, &aout,
			 FLIGHTS_COL_AIN, &ain,
			 FLIGHTS_COL_DUR, &dur,
			 FLIGHTS_COL_NIGHT, &night,
			 FLIGHTS_COL_INST, &inst,
			 FLIGHTS_COL_SIMINST, &siminst,
			 FLIGHTS_COL_HOLD, &hold,
			 FLIGHTS_COL_APRCH, &aprch,
			 FLIGHTS_COL_XC, &xc,
			 FLIGHTS_COL_DLAND, &dland,
			 FLIGHTS_COL_NLAND, &nland,
			 FLIGHTS_COL_CREW, &crew,
			 FLIGHTS_COL_NOTES, &notes,
			 FLIGHTS_COL_FLTNO, &fltno,
			 FLIGHTS_COL_SOUT, &sout,
			 FLIGHTS_COL_SIN, &sin,
			 FLIGHTS_COL_SDUR, &sdur,
			 FLIGHTS_COL_TRIP, &trip,
			 -1);

      sscanf(EMPTY_IF_NULL(dland), "%d", &_dland);
      sscanf(EMPTY_IF_NULL(nland), "%d", &_nland);

      gtk_entry_set_text(GTK_ENTRY(logb->flights_date), EMPTY_IF_NULL(date));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_aircraft), EMPTY_IF_NULL(aircraft));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_role), EMPTY_IF_NULL(role));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_dep), EMPTY_IF_NULL(dep));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_arr), EMPTY_IF_NULL(arr));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_aout), EMPTY_IF_NULL(aout));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_ain), EMPTY_IF_NULL(ain));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_dur), EMPTY_IF_NULL(dur));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_night), EMPTY_IF_NULL(night));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_inst), EMPTY_IF_NULL(inst));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_siminst), EMPTY_IF_NULL(siminst));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->flights_hold), str_bool(hold));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_aprch), EMPTY_IF_NULL(aprch));
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->flights_xc), str_bool(xc));
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(logb->flights_dland), _dland);
      gtk_spin_button_set_value(GTK_SPIN_BUTTON(logb->flights_nland), _nland);

      text_view_set_text(GTK_TEXT_VIEW(logb->flights_crew), EMPTY_IF_NULL(crew));
      text_view_set_text(GTK_TEXT_VIEW(logb->flights_notes), EMPTY_IF_NULL(notes));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_fltno), EMPTY_IF_NULL(fltno));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_sout), EMPTY_IF_NULL(sout));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_sin), EMPTY_IF_NULL(sin));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_sdur), EMPTY_IF_NULL(sdur));
      gtk_entry_set_text(GTK_ENTRY(logb->flights_trip), EMPTY_IF_NULL(trip));
      
      g_free(id);
      g_free(date);
      g_free(aircraft);
      g_free(role);
      g_free(dep);
      g_free(arr);
      g_free(aout);
      g_free(ain);
      g_free(dur);
      g_free(night);
      g_free(inst);
      g_free(siminst);
      g_free(hold);
      g_free(aprch);
      g_free(xc);
      g_free(dland);
      g_free(nland);
      g_free(crew);
      g_free(notes);
      g_free(fltno);
      g_free(sout);
      g_free(sin);
      g_free(sdur);
      g_free(trip);
    }
}

/* Find or create a record the value of a column and return the id */
DBint64 id_of_with_insert(DB *db, const char *table, const char *column, const char *value, int *inserted)
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
    *inserted = TRUE;
  } else {
    id = db_column_int64(select, 0);
    *inserted = FALSE;
  }

  db_finalize(select);

  return id;
  #undef __sql_size
}

/* Binds id of a row in a table with column matching a value to the i'th variable in stmt */
/* Will insert into table if no row where column = value is found */
/* Binds null when value is empty */
/* Returns TRUE if row was inserted */
int bind_id_of(DBStatement *stmt, int i, const char *table, const char *column, const char *value)
{
  DB *db;
  int inserted;

  db = db_handle(stmt);
  if (value && strlen(value)) {
    db_bind_int64(stmt, i, id_of_with_insert(db, table, column, value, &inserted));
  } else {
    db_bind_null(stmt, i);
  }

  return inserted;
}

/* Writes stuff in entry portion of gui to db - if id is "0" it inserts, else updates */
void flights_write_entries(Sqlpilot *sqlpilot, const gchar *id)
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
  
  /* Write entries to database */
  if (strcmp("0", id)) {
    stmt = sqlpilot->flights_update;
    db_bind_text(stmt, FLIGHTS_WRITE_ID, id);
  } else {
    stmt = sqlpilot->flights_insert;
  }
  bind_id_of(stmt, FLIGHTS_WRITE_AIRCRAFT, "aircraft", "ident", aircraft);
  bind_id_of(stmt, FLIGHTS_WRITE_ROLE, "roles", "ident", role);
  bind_id_of(stmt, FLIGHTS_WRITE_DEP , "airports", "ident", dep);
  bind_id_of(stmt, FLIGHTS_WRITE_ARR, "airports", "ident", arr);
  db_bind_text(stmt, FLIGHTS_WRITE_DATE, date);
  db_bind_text(stmt, FLIGHTS_WRITE_AOUT, aout);
  db_bind_text(stmt, FLIGHTS_WRITE_AIN, ain);
  db_bind_text(stmt, FLIGHTS_WRITE_DUR, dur);
  db_bind_text(stmt, FLIGHTS_WRITE_NIGHT, night);
  db_bind_text(stmt, FLIGHTS_WRITE_INST, inst);
  db_bind_text(stmt, FLIGHTS_WRITE_SIMINST, siminst);
  db_bind_int(stmt, FLIGHTS_WRITE_HOLD, hold ? 1 : 0);
  db_bind_text(stmt, FLIGHTS_WRITE_APRCH, aprch);
  db_bind_int(stmt, FLIGHTS_WRITE_XC, xc ? 1 : 0);
  db_bind_int(stmt, FLIGHTS_WRITE_DLAND, dland);
  db_bind_int(stmt, FLIGHTS_WRITE_NLAND, nland);
  db_bind_text(stmt, FLIGHTS_WRITE_CREW, crew);
  db_bind_text(stmt, FLIGHTS_WRITE_NOTES, notes);
  db_bind_text(stmt, FLIGHTS_WRITE_FLTNO, fltno);
  db_bind_text(stmt, FLIGHTS_WRITE_SOUT, sout);
  db_bind_text(stmt, FLIGHTS_WRITE_SIN, sin);
  db_bind_text(stmt, FLIGHTS_WRITE_SDUR, sdur);
  db_bind_text(stmt, FLIGHTS_WRITE_TRIP, trip);  

  db_step(stmt);
  db_reset(stmt);
  db_clear_bindings(stmt);

  g_free(crew);
  g_free(notes);
}

void on_flights_insert_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  DBStatement *stmt;
  GtkTreeIter iter;
  DBint64 inserted_id;

  flights_write_entries(sqlpilot, "0");
  inserted_id = db_last_insert_rowid(sqlpilot->db); 

  //treemodel_insert(sqlpilot->flights_treemodel, sqlpilot->flights_select_by_id, inserted_id);
  /* Read row into treemodel */
  stmt = sqlpilot->flights_select_by_id;
  db_bind_int64(stmt, 1, inserted_id);
  gtk_list_store_insert(GTK_LIST_STORE(sqlpilot->flights_treemodel), &iter, 0);
  store_update_row(GTK_LIST_STORE(sqlpilot->flights_treemodel), &iter, stmt);
  
  db_reset(stmt);
  db_clear_bindings(stmt);  
}

void on_flights_update_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  DBStatement *stmt;
  
  if (gtk_tree_selection_get_selected(sqlpilot->flights_selection, &sqlpilot->flights_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->flights_treemodel, &iter, FLIGHTS_COL_ID, &id, -1);

    flights_write_entries(sqlpilot, id);

    /* Read row into treemodel */
    stmt = sqlpilot->flights_select_by_id;
    db_bind_text(stmt, 1, id);
    store_update_row(GTK_LIST_STORE(sqlpilot->flights_treemodel), &iter, stmt);
  
    db_reset(stmt);
    db_clear_bindings(stmt);
    g_free(id);
  }
}

void on_flights_delete_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  
  if (gtk_tree_selection_get_selected(sqlpilot->flights_selection, &sqlpilot->flights_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->flights_treemodel, &iter, FLIGHTS_COL_ID, &id, -1);
    
    db_bind_text(sqlpilot->flights_delete, FLIGHTS_DELETE_ID, id);

    db_step(sqlpilot->flights_delete);
    db_reset(sqlpilot->flights_delete);
    db_clear_bindings(sqlpilot->flights_delete);
  
    gtk_list_store_remove(GTK_LIST_STORE(sqlpilot->flights_treemodel), &iter);

    g_free(id);
  }
}
void on_flights_cancel_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  flights_load_entries_from_selection(sqlpilot);
}  

void on_flights_trip_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
}

void on_flights_aircraft_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_text(entry, 6, 1, is_ident_char);
}
void on_flights_dep_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_text(entry, 4, 1, is_ident_char);
}
void on_flights_arr_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_text(entry, 4, 1, is_ident_char);
}

void on_flights_aout_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, ':');
}
void on_flights_ain_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_format_time_partial(entry, ':');
}
int on_flights_sout_focus_in_event(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  fprintf(stderr, "i");
  return FALSE;
}
int on_flights_sout_focus_out_event(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  fprintf(stderr, "o");
  return FALSE;
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
      flights_load_entries_from_selection(logb);      
    }
}
