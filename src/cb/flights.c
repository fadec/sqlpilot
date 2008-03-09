#include "sqlpilot.h"

/* Tabs whos view depends on flights because of joins need to be refreshed if the flights table changes */
static void set_dependents_stale(Sqlpilot *sqlpilot)
{
  sqlpilot->roles_stale = TRUE;
  sqlpilot->aircraft_stale = TRUE;
  sqlpilot->types_stale = TRUE;
  sqlpilot->airports_stale = TRUE;
}

void entry_format_date_on_focus_out(GtkEntry *entry)
{
  int yy=0, mm=0, dd=0;
  const char *text;
  char result[11]; 		/* strlen("yyyy-mm-dd\0") == 11 */
  struct tm tm = {0};

  text = gtk_entry_get_text(entry);
  sscanf(text, "%d-%d-%d", &yy, &mm, &dd);

  tm.tm_year = yy - 1900;
  tm.tm_mon = mm - 1;
  tm.tm_mday = dd;

  if (mktime(&tm) == -1) {
    result[0] = '\0';
  } else {
    strftime(result, 11, "%Y-%m-%d", &tm);
  }

  gtk_entry_set_text(entry, result);
}

void entry_format_time_on_focus_out(GtkEntry *entry, char separator)
{
  char result[BUF_TIME];
  format_time(gtk_entry_get_text(entry), result, separator);
  m_to_strtime(daywrap_minutes(strtime_to_m(result)), result, BUF_TIME, separator);
  gtk_entry_set_text(entry, result);
}

void entry_format_time_on_focus_in(GtkEntry *entry)
{
  #define __strmax 10

  const char *ptr;
  char result[__strmax+1];
  int i = 0;

  ptr = gtk_entry_get_text(entry);

  while (*ptr != '\0' && i < __strmax) {
    if (*ptr >= '0' && *ptr <= '9') {
      result[i] = *ptr;
      i++;
    }
      ptr++;
  }  
  result[i] = '\0';

  gtk_entry_set_text(entry, result);
  #undef __strmax
}

void reconcile_time_entries(GtkEntry *changed, GtkEntry *start, GtkEntry *end, GtkEntry *elapsed)
{
  const char *tstart, *tend, *telapsed;
  GtkEntry *tochange = NULL;
  int m = 0, hh, mm;
  char result[30];

  tstart = gtk_entry_get_text(start);
  tend = gtk_entry_get_text(end);
  telapsed = gtk_entry_get_text(elapsed);

  if (changed == start && strlen(tstart)) {
    if (strlen(tend)) {
      m = strtime_to_m(tend) - strtime_to_m(tstart);
      tochange = elapsed;
    } else if (strlen(telapsed)) {
      m = strtime_to_m(tstart) + strtime_to_m(telapsed);
      tochange = end;
    }
  } else if (changed == end && strlen(tend)) {
    if (strlen(tstart)) {
      m = strtime_to_m(tend) - strtime_to_m(tstart);
      tochange = elapsed;
    } else if (strlen(telapsed)) {
      m = strtime_to_m(tend) - strtime_to_m(telapsed);
      tochange = start;
    }
  } else if (changed == elapsed && strlen(telapsed)) {
    if (strlen(tstart)) {
      m = strtime_to_m(tstart) + strtime_to_m(telapsed);
      tochange = end;
    } else if (strlen(tend)) {
      m = strtime_to_m(tend) - strtime_to_m(telapsed);
      tochange = start;
    }
  }

  if (tochange) {
    m = daywrap_minutes(m);

    hh = m / 60;
    mm = m - (hh * 60);
    
    snprintf(result, 30, (tochange == elapsed) ? "%d+%02d" : "%02d:%02d", hh, mm);
    gtk_entry_set_text(tochange, result);
  }

}

static void flights_load_entries_from_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id=NULL,
    *aircraft=NULL,
    *role=NULL,
    *dep=NULL,
    *arr=NULL,
    *date=NULL,
    *aout=NULL,
    *ain=NULL,
    *dur=NULL,
    *night=NULL,
    *inst=NULL,
    *siminst=NULL,
    *aprch=NULL,
    *fltno=NULL,
    *sout=NULL,
    *sin=NULL,
    *sdur=NULL,
    *trip=NULL,
    *crew=NULL,
    *notes=NULL,
    *dland=NULL,
    *nland=NULL,
    *xc=NULL,
    *hold=NULL;

  int _dland=0, _nland=0;

  if (gtk_tree_selection_get_selected (logb->flights_selection, &model, &iter)) {
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
  }

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

/* Writes stuff in entry portion of gui to db - if id is "0" it inserts, else updates */
static void flights_write_entries(Sqlpilot *sqlpilot, const gchar *id)
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
  if (id) {
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

void flights_refresh(Sqlpilot *sqlpilot)
{
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->flights_treemodel), sqlpilot->flights_select_all);
    flights_load_entries_from_selection(sqlpilot);
    sqlpilot->flights_stale = FALSE;
}


void on_flights_insert_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  DBStatement *stmt;
  GtkTreeIter iter;
  DBint64 inserted_id;

  flights_write_entries(sqlpilot, NULL);
  inserted_id = db_last_insert_rowid(sqlpilot->db); 

  /* Read row into treemodel */
  stmt = sqlpilot->flights_select_by_id;
  db_bind_int64(stmt, 1, inserted_id);
  gtk_list_store_insert(GTK_LIST_STORE(sqlpilot->flights_treemodel), &iter, 0);
  store_update_row(GTK_LIST_STORE(sqlpilot->flights_treemodel), &iter, stmt);
  
  db_reset(stmt);
  db_clear_bindings(stmt);

  set_dependents_stale(sqlpilot);
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

    set_dependents_stale(sqlpilot);
  }
}

void on_flights_delete_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  
  if (gtk_tree_selection_get_selected(sqlpilot->flights_selection, &sqlpilot->flights_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->flights_treemodel, &iter, FLIGHTS_COL_ID, &id, -1);
    
    db_bind_text(sqlpilot->flights_delete, 1, id);

    db_step(sqlpilot->flights_delete);
    db_reset(sqlpilot->flights_delete);
    db_clear_bindings(sqlpilot->flights_delete);
  
    gtk_list_store_remove(GTK_LIST_STORE(sqlpilot->flights_treemodel), &iter);

    g_free(id);

    set_dependents_stale(sqlpilot);
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

int is_numeric_date_char(char c)
{
  return (c >= '0' && c <= '9') || c == '-';
}
int on_flights_date_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  return FALSE;
}
int on_flights_date_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_date_on_focus_out(entry);
  return FALSE;
}
void on_flights_date_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 10, 0, is_numeric_date_char);
} 

void on_flights_role_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_roles_ident(entry);
}

void on_flights_aircraft_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_aircraft_ident(entry);
}
void on_flights_dep_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_airports_ident(entry);
}
void on_flights_arr_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  const gchar *text;
  text = gtk_entry_get_text(entry);
  entry_clamp_airports_ident(entry);
}

int on_flights_sout_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_sout_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sdur));
  return FALSE;
}
void on_flights_sout_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}

int on_flights_sin_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_sin_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sdur));
  return FALSE;
}
void on_flights_sin_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}

int on_flights_sdur_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_sdur_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  reconcile_time_entries(GTK_ENTRY(sqlpilot->flights_sdur),
			 GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sdur));
  return FALSE;
}

void on_flights_sdur_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}



int on_flights_aout_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_aout_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_dur));
  return FALSE;
}
void on_flights_aout_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}


int on_flights_ain_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_ain_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_dur));
  return FALSE;
}
void on_flights_ain_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}

int on_flights_dur_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_dur_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  reconcile_time_entries(GTK_ENTRY(sqlpilot->flights_dur),
			 GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_dur));
  return FALSE;
}
void on_flights_dur_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}

int on_flights_inst_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_inst_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_inst_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}

int on_flights_siminst_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_siminst_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_siminst_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}

int on_flights_night_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_night_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_night_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
}

void on_flights_selection_changed(GtkTreeSelection *selection, Sqlpilot *logb)
{
  flights_load_entries_from_selection(logb);
}
