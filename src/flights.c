#include "flights.h"

int flights_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *dep=NULL,
    *arr=NULL,
    *date=NULL;
  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       FLIGHTS_COL_DATE, &date,
		       FLIGHTS_COL_DEP, &dep,
		       FLIGHTS_COL_ARR, &arr,
		       -1);
    snprintf(show, size, "%s %s-%s", date, dep, arr);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }
}

void flights_after_change(Sqlpilot *sqlpilot)
{
  sqlpilot->roles_stale = TRUE;
  sqlpilot->aircraft_stale = TRUE;
  sqlpilot->types_stale = TRUE;
  sqlpilot->airports_stale = TRUE;
}

int flights_can_delete(GtkTreeSelection *selection)
{
  /* Okay to delete any flight */
  return 1;
}

/* Writes stuff in entry portion of gui to db - if id is NULL it inserts, else updates */
DBint64 flights_write_entries(const gchar *id, Sqlpilot *sqlpilot)
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
  char				/* For other timezone strings, local if flights_utc and vice versa */
    _date[BUF_DATE],
    _aout[BUF_TIME],
    _ain[BUF_TIME],
    _sout[BUF_TIME],
    _sin[BUF_TIME],
    deptz[BUF_TZ],
    arrtz[BUF_TZ];
  char *deptz1, *deptz2, *arrtz1, *arrtz2;
  gint dland, nland;
  gboolean xc, hold;
  DBStatement *stmt;
  gboolean utc;

  utc = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_utc));

  aircraft = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_aircraft));
  role     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_role));
  dep      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_dep));
  arr      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_arr));
  dur      = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_dur));
  night    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_night));
  inst     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_inst));
  siminst  = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_siminst));
  aprch    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_aprch));
  fltno    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_fltno));
  sdur     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_sdur));
  trip     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_trip));
  dland    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sqlpilot->flights_dland));
  nland    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(sqlpilot->flights_nland));
  hold     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_hold));
  xc       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_xc));
  crew     = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->flights_crew));
  notes    = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->flights_notes));
  date = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_date));
  aout = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_aout));
  ain  = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_ain));
  sout = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_sout));
  sin  = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_sin));


  tz_of_airport_ident(sqlpilot->db, dep, deptz, sizeof(deptz));
  tz_of_airport_ident(sqlpilot->db, arr, arrtz, sizeof(arrtz));

  if (utc) {
    /* to Local */
    deptz1 = "UTC";
    arrtz1 = "UTC";
    deptz2 = deptz;
    arrtz2 = arrtz;

  } else {
    /* to UTC */
    deptz1 = deptz;
    arrtz1 = arrtz;
    deptz2 = "UTC";
    arrtz2 = "UTC";
  }

  move_time(arrtz1, arrtz2, date, ain, _date, _ain);
  move_time(arrtz1, arrtz2, date, sin, _date, _sin);
  /* Adjusting order of calls _date right, preferring aout over sout */
  /* Note that we depend on the behavior of move_time for _date if sout is funky */
  if (strlen(aout)) {
    move_time(deptz1, deptz2, date, sout, _date, _sout);
    move_time(deptz1, deptz2, date, aout, _date, _aout);
  } else {
    move_time(deptz1, deptz2, date, aout, _date, _aout);
    move_time(deptz1, deptz2, date, sout, _date, _sout);
  }
   
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
  db_bind_text(stmt, FLIGHTS_WRITE_SDUR, sdur);
  db_bind_text(stmt, FLIGHTS_WRITE_TRIP, trip);  

  db_bind_text(stmt, FLIGHTS_WRITE_DATE, utc ? _date : date);
  db_bind_text(stmt, FLIGHTS_WRITE_AOUT, utc ? _aout : aout);
  db_bind_text(stmt, FLIGHTS_WRITE_AIN,  utc ? _ain  : ain);
  db_bind_text(stmt, FLIGHTS_WRITE_SOUT, utc ? _sout : sout);
  db_bind_text(stmt, FLIGHTS_WRITE_SIN,  utc ? _sin  : sin);
  db_bind_text(stmt, FLIGHTS_WRITE_DATEUTC, utc ? date : _date);
  db_bind_text(stmt, FLIGHTS_WRITE_AOUTUTC, utc ? aout : _aout);
  db_bind_text(stmt, FLIGHTS_WRITE_AINUTC,  utc ? ain  : _ain);
  db_bind_text(stmt, FLIGHTS_WRITE_SOUTUTC, utc ? sout : _sout);
  db_bind_text(stmt, FLIGHTS_WRITE_SINUTC,  utc ? sin  : _sin);

  db_stp_res_clr(stmt);
  g_free(crew);
  g_free(notes);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(sqlpilot->db);
  }
}



void entry_format_date_on_focus_out(GtkEntry *entry)
{
  int yy=0, mm=1, dd=1;
  const char *text;
  char result[11]; 		/* strlen("yyyy-mm-dd\0") == 11 */
  struct tm tm = {0};

  text = gtk_entry_get_text(entry);

  if (text && strlen(text)) {
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

}

void entry_format_time_on_focus_out(GtkEntry *entry, char separator)
{
  const char *txt;

  txt = gtk_entry_get_text(entry);
  if (txt && strlen(txt)) { 
    char result[BUF_TIME];
    format_time(txt, result, separator);
    if (separator == ':') m_to_strtime(daywrap_minutes(strtime_to_m(result)), result, BUF_TIME, separator);
    gtk_entry_set_text(entry, result);
  }
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

long elapsed_seconds(const char *date, const char *t1, const char *tz1, const char *t2, const char *tz2)
{
  /* this will not correctly compute across a DST change */
  return daywrap(dtz_mktime(date, t2, tz2) - dtz_mktime(date, t1, tz1));
}

/* Changed is an alias to either start, end, or elapsed. */
void reconcile_time_entries(Sqlpilot *logb,
			    GtkEntry *changed,
			    GtkEntry *start,
			    GtkEntry *end,
			    GtkEntry *elapsed)
{
  const char *tstart, *tend, *telapsed, *tdate, *tdep, *tarr;
  GtkEntry *tochange = NULL;
  long m = 0, hh, mm;
  time_t t;
  struct tm tm;
  char deptz[BUF_TZ] = "UTC", arrtz[BUF_TZ] = "UTC";
  char result[30];

  tstart = gtk_entry_get_text(start);
  tend = gtk_entry_get_text(end);
  telapsed = gtk_entry_get_text(elapsed);

  tdate = gtk_entry_get_text(GTK_ENTRY(logb->flights_date));
  tdep = gtk_entry_get_text(GTK_ENTRY(logb->flights_dep));
  tarr = gtk_entry_get_text(GTK_ENTRY(logb->flights_arr));

  if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logb->flights_utc))) {
    tz_of_airport_ident(logb->db, tdep, deptz, BUF_TZ);
    tz_of_airport_ident(logb->db, tarr, arrtz, BUF_TZ);
  }

  if (changed == start && strlen(tstart)) {
    if (strlen(tend)) {
      m = elapsed_seconds(tdate, tstart, deptz, tend, arrtz) / 60;
      tochange = elapsed;
    } else if (strlen(telapsed)) {
      t = dtz_mktime(tdate, tstart, deptz) + strtime_to_m(telapsed) * 60;
      tochange = end;
    }
  } else if (changed == end && strlen(tend)) {
    if (strlen(tstart)) {
      m = elapsed_seconds(tdate, tstart, deptz, tend, arrtz) / 60;
      tochange = elapsed;
    } else if (strlen(telapsed)) {
      t = dtz_mktime(tdate, tend, arrtz) - strtime_to_m(telapsed) * 60;
      tochange = start;
    }
  } else if (changed == elapsed && strlen(telapsed)) {
    if (strlen(tstart)) {
      t = dtz_mktime(tdate, tstart, deptz) + strtime_to_m(telapsed) * 60;
      tochange = end;
    } else if (strlen(tend)) {
      t = dtz_mktime(tdate, tend, arrtz) - strtime_to_m(telapsed) * 60;
      tochange = start;
    }
  }

  if (tochange) {
    if (tochange == elapsed) {
      hh = m / 60;
      mm = m - (hh * 60);
      snprintf(result, sizeof(result), "%ld+%02ld", hh, mm);
    } else {
      localtime_tz(&t, ((tochange == start) ? deptz : arrtz), &tm);
      strftime(result, sizeof(result), "%H:%M", &tm);
    }
    gtk_entry_set_text(tochange, result);
  }

}

void flights_load_selection(Sqlpilot *logb)
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
    *dateutc=NULL,
    *aout=NULL,
    *aoututc=NULL,
    *ain=NULL,
    *ainutc=NULL,
    *dur=NULL,
    *night=NULL,
    *inst=NULL,
    *siminst=NULL,
    *aprch=NULL,
    *fltno=NULL,
    *sout=NULL,
    *soututc=NULL,
    *sin=NULL,
    *sinutc=NULL,
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
		       FLIGHTS_COL_DATE, &dateutc,
		       FLIGHTS_COL_AIRCRAFT, &aircraft,
		       FLIGHTS_COL_ROLE, &role,
		       FLIGHTS_COL_DEP, &dep,
		       FLIGHTS_COL_ARR, &arr,
		       FLIGHTS_COL_AOUT, &aout,
		       FLIGHTS_COL_AOUTUTC, &aoututc,
		       FLIGHTS_COL_AIN, &ain,
		       FLIGHTS_COL_AINUTC, &ainutc,
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
		       FLIGHTS_COL_SOUTUTC, &soututc,
		       FLIGHTS_COL_SIN, &sin,
		       FLIGHTS_COL_SINUTC, &sinutc,
		       FLIGHTS_COL_SDUR, &sdur,
		       FLIGHTS_COL_TRIP, &trip,
		       -1);
    sscanf(EMPTY_IF_NULL(dland), "%d", &_dland);
    sscanf(EMPTY_IF_NULL(nland), "%d", &_nland);
  }

  gtk_entry_set_text(GTK_ENTRY(logb->flights_aircraft), EMPTY_IF_NULL(aircraft));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_role), EMPTY_IF_NULL(role));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_dep), EMPTY_IF_NULL(dep));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_arr), EMPTY_IF_NULL(arr));
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
  gtk_entry_set_text(GTK_ENTRY(logb->flights_sdur), EMPTY_IF_NULL(sdur));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_trip), EMPTY_IF_NULL(trip));

  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logb->flights_utc))) {
    gtk_entry_set_text(GTK_ENTRY(logb->flights_date), EMPTY_IF_NULL(dateutc));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_aout), EMPTY_IF_NULL(aoututc));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_ain), EMPTY_IF_NULL(ainutc));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sout), EMPTY_IF_NULL(soututc));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sin), EMPTY_IF_NULL(sinutc));
  } else {
    gtk_entry_set_text(GTK_ENTRY(logb->flights_date), EMPTY_IF_NULL(date));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_aout), EMPTY_IF_NULL(aout));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_ain), EMPTY_IF_NULL(ain));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sout), EMPTY_IF_NULL(sout));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sin), EMPTY_IF_NULL(sin));
  }
      
  g_free(id);
  g_free(date);
  g_free(dateutc);
  g_free(aircraft);
  g_free(role);
  g_free(dep);
  g_free(arr);
  g_free(aout);
  g_free(aoututc);
  g_free(ain);
  g_free(ainutc);
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
  g_free(soututc);
  g_free(sin);
  g_free(sinutc);
  g_free(sdur);
  g_free(trip);
}

void flights_refresh(Sqlpilot *sqlpilot)
{
  store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->flights_treemodel), sqlpilot->flights_select_all);
  flights_load_selection(sqlpilot);
  sqlpilot->flights_stale = FALSE;
}
