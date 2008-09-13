/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.                                       */
/*                                                                      */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* Sqlpilot is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#include "flights.h"
#include "aircraft.h"
#include <string.h>

StoreColumnKind flights_column_kinds[] = {
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_FLOAT,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_NUM,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR,
  STORE_COLUMN_KIND_STR_NUM
};

void flights_lookup_dep_tz(Logbook *logb, char *tz)
{
  const char *txt;
  DBStatement *stmt;
  txt = gtk_entry_get_text(GTK_ENTRY(logb->flights_depicao));
  stmt = logb->flights_tz_of_airport_icao;
  if (!strlen(txt)) {
    txt = gtk_entry_get_text(GTK_ENTRY(logb->flights_depiata));
    stmt = logb->flights_tz_of_airport_iata;
  }
  tz_of_airport_or_utc(stmt, txt, tz, BUF_TZ);
}

void flights_lookup_arr_tz(Logbook *logb, char *tz)
{
  const char *txt;
  DBStatement *stmt;
  txt = gtk_entry_get_text(GTK_ENTRY(logb->flights_arricao));
  stmt = logb->flights_tz_of_airport_icao;
  if (!strlen(txt)) {
    txt = gtk_entry_get_text(GTK_ENTRY(logb->flights_arriata));
    stmt = logb->flights_tz_of_airport_iata;
  }
  tz_of_airport_or_utc(stmt, txt, tz, BUF_TZ);
}

void flights_refresh_aircraft_utilized(Logbook *logbook)
{
/*   DBint64 id; */

/*   char txt[16]; */
/*   const char *aircraft; */

/*   aircraft = gtk_entry_get_text(GTK_ENTRY(logbook->flights_aircraft)); */
/*   if (!strlen(aircraft)) { */
/*     snprintf(txt, sizeof(txt), " ");  */
/*   } else if (find_row_id(logbook->db, "Aircraft", "Ident", aircraft, &id)) { */
/*     snprintf(txt, sizeof(txt), "(%d)", aircraft_count_flights(logbook, id)); */
/*   } else { */
/*     snprintf(txt, sizeof(txt), "(?)"); */
/*   } */

/*   gtk_label_set_text(GTK_LABEL(logbook->flights_aircraft_utilized), txt); */
}

void flights_refresh_role_utilized(Logbook *logbook)
{
  DBint64 id;

  char txt[16];
  const char *role;
  role = gtk_entry_get_text(GTK_ENTRY(logbook->flights_role));

  if (!strlen(role) || find_row_id(logbook->db, "Roles", "Ident", role, &id)) {
    snprintf(txt, sizeof(txt), " ");
  } else {
    snprintf(txt, sizeof(txt), "(?)");
  }
  
  gtk_label_set_text(GTK_LABEL(logbook->flights_role_utilized), txt);
}

void flights_refresh_dep_utilized(Logbook *logbook)
{
/*   DBint64 id; */

/*   char txt[16]; */
/*   const char *dep; */

/*   dep = gtk_entry_get_text(GTK_ENTRY(logbook->flights_dep)); */

/*   if (!strlen(dep) || find_row_id(logbook->db, "Airports", */
/* 				  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_icao_toggle)) ? "icao" : "iata", */
/* 				  dep, &id)) { */
/*     snprintf(txt, sizeof(txt), " "); */
/*   } else { */
/*     snprintf(txt, sizeof(txt), "(?)"); */
/*   } */

/*   gtk_label_set_text(GTK_LABEL(logbook->flights_dep_utilized), txt); */
}

void flights_refresh_arr_utilized(Logbook *logbook)
{
/*   DBint64 id; */

/*   char txt[16]; */
/*   const char *arr; */

/*   arr = gtk_entry_get_text(GTK_ENTRY(logbook->flights_arr)); */

/*   if (!strlen(arr) || find_row_id(logbook->db, "Airports", */
/* 				  gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_icao_toggle)) ? "icao" : "iata", */
/* 				  arr, &id)) { */
/*     snprintf(txt, sizeof(txt), " "); */
/*   } else { */
/*     snprintf(txt, sizeof(txt), "(?)"); */
/*   } */

/*   gtk_label_set_text(GTK_LABEL(logbook->flights_arr_utilized), txt); */
}

void flights_refresh_utilization(Logbook *logbook)
{
  flights_refresh_role_utilized(logbook);
  flights_refresh_aircraft_utilized(logbook);
  flights_refresh_dep_utilized(logbook);
  flights_refresh_arr_utilized(logbook);
}

int flights_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *treemod;
  gchar
    *dep=NULL,
    *arr=NULL,
    *date=NULL;
  if (gtk_tree_selection_get_selected (selection, &treemod, &iter)) {
    gtk_tree_model_get(treemod, &iter,
		       FLIGHTS_COL_DATE, &date,
		       FLIGHTS_COL_DEPIATA, &dep,
		       FLIGHTS_COL_ARRIATA, &arr,
		       -1);
    snprintf(show, size, "%s %s-%s", date, dep, arr);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }
}

void flights_after_change(Logbook *logbook)
{
  logbook->roles_stale = TRUE;
  logbook->aircraft_stale = TRUE;
  logbook->models_stale = TRUE;
  logbook->airports_stale = TRUE;

  flights_load_selection(logbook);
}

int flights_can_delete(GtkTreeSelection *selection)
{
  /* Okay to delete any flight */
  return 1;
}

/* Writes stuff in entry portion of gui to db - if id is NULL it inserts, else updates */
DBint64 flights_write_entries(const gchar *id, Logbook *logbook)
{
  const gchar
    *tail,
    *fleetno,
    *role,
    *depicao,
    *arricao,
    *depiata,
    *arriata,
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
    *tripdate;
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
  gint dland, nland, leg;
  gboolean xc, hold;
  DBStatement *stmt;
  gboolean utc;

  utc = any_toggle_button_get_active(logbook->flights_utc);

  tail     = gtk_entry_get_text(GTK_ENTRY(logbook->flights_tail));
  fleetno  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_fleetno));
  role     = gtk_entry_get_text(GTK_ENTRY(logbook->flights_role));
  depicao  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_depicao));
  arricao  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_arricao));
  depiata  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_depiata));
  arriata  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_arriata));
  dur      = gtk_entry_get_text(GTK_ENTRY(logbook->flights_dur));
  night    = gtk_entry_get_text(GTK_ENTRY(logbook->flights_night));
  inst     = gtk_entry_get_text(GTK_ENTRY(logbook->flights_inst));
  siminst  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_siminst));
  aprch    = gtk_entry_get_text(GTK_ENTRY(logbook->flights_aprch));
  fltno    = gtk_entry_get_text(GTK_ENTRY(logbook->flights_fltno));
  sdur     = gtk_entry_get_text(GTK_ENTRY(logbook->flights_sdur));
  trip     = gtk_entry_get_text(GTK_ENTRY(logbook->flights_trip));
  tripdate = gtk_entry_get_text(GTK_ENTRY(logbook->flights_tripdate));
  dland    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(logbook->flights_dland));
  nland    = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(logbook->flights_nland));
  hold     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_hold));
  xc       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_xc));
  crew     = text_view_get_text(GTK_TEXT_VIEW(logbook->flights_crew));
  notes    = text_view_get_text(GTK_TEXT_VIEW(logbook->flights_notes));
  date     = gtk_entry_get_text(GTK_ENTRY(logbook->flights_date));
  leg      = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(logbook->flights_leg)); 
  aout = gtk_entry_get_text(GTK_ENTRY(logbook->flights_aout));
  ain  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_ain));
  sout = gtk_entry_get_text(GTK_ENTRY(logbook->flights_sout));
  sin  = gtk_entry_get_text(GTK_ENTRY(logbook->flights_sin));

  flights_lookup_dep_tz(logbook, deptz);
  flights_lookup_arr_tz(logbook, arrtz);

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

  move_time(deptz1, deptz2, date, aout, _date, _aout);
  move_time(deptz1, deptz2, date, sout, _date, _sout);
  move_time(arrtz1, arrtz2, date, ain, _date, _ain);
  move_time(arrtz1, arrtz2, date, sin, _date, _sin);
   
  /* Write entries to database */
  if (id) {
    stmt = logbook->flights_update;
    db_bind_text(stmt, FLIGHTS_WRITE_ID, id);
  } else {
    stmt = logbook->flights_insert;
  }

  if (tail && strlen(tail)) {
    bind_id_of(stmt, FLIGHTS_WRITE_AIRCRAFT, "aircraft", "tail", tail);
  } else if (fleetno && strlen(fleetno)) {
    bind_id_of(stmt, FLIGHTS_WRITE_AIRCRAFT, "aircraft", "fleetno", fleetno);
  }
  if (depicao && strlen(depicao)) {
    bind_id_of(stmt, FLIGHTS_WRITE_DEP, "airports", "icao", depicao);
  } else {
    bind_id_of(stmt, FLIGHTS_WRITE_DEP, "airports", "iata", depiata);
  }
  if (arricao && strlen(arricao)) {
    bind_id_of(stmt, FLIGHTS_WRITE_ARR, "airports", "icao", arricao);
  } else {
    bind_id_of(stmt, FLIGHTS_WRITE_ARR, "airports", "iata", arriata);
  }
  bind_id_of(stmt, FLIGHTS_WRITE_ROLE, "roles", "ident", role);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_DUR, dur);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_NIGHT, night);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_INST, inst);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_SIMINST, siminst);
  db_bind_int(stmt, FLIGHTS_WRITE_HOLD, hold ? 1 : 0);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_APRCH, aprch);
  db_bind_int(stmt, FLIGHTS_WRITE_XC, xc ? 1 : 0);
  db_bind_int(stmt, FLIGHTS_WRITE_DLAND, dland);
  db_bind_int(stmt, FLIGHTS_WRITE_NLAND, nland);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_CREW, crew);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_NOTES, notes);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_FLTNO, fltno);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_SDUR, sdur);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_TRIP, trip);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_TRIPDATE, tripdate);

  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_DATE, date);
  db_bind_int(stmt, FLIGHTS_WRITE_LEG, leg);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_AOUT, utc ? _aout : aout);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_AIN,  utc ? _ain  : ain);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_SOUT, utc ? _sout : sout);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_SIN,  utc ? _sin  : sin);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_AOUTUTC, utc ? aout : _aout);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_AINUTC,  utc ? ain  : _ain);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_SOUTUTC, utc ? sout : _sout);
  db_bind_nonempty_text_else_null(stmt, FLIGHTS_WRITE_SINUTC,  utc ? sin  : _sin);

  db_stp_res_clr(stmt);
  g_free(crew);
  g_free(notes);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(logbook->db);
  }
}

void entry_format_time_of_day(GtkEntry *entry, const char *local_tz, const char *to_tz, const char *date)
{
  const char *txt;

  txt = gtk_entry_get_text(entry);
  if (txt && strlen(txt)) {
    char formed[BUF_TIME];
    char moved[BUF_TIME];
    char moveddate[BUF_DATE];
    format_time(txt, formed, ':');
    m_to_strtime(daywrap_minutes(strtime_to_m(formed)), formed, BUF_TIME, ':');
    if (strcasestr(txt, "l")) {
      move_time(local_tz, to_tz, date, formed, moveddate, moved);
    } else if (strcasestr(txt, "u") || strcasestr(txt, "z")) {
      move_time("UTC", to_tz, date, formed, moveddate, moved);
    } else {
      strncpy(moved, formed, BUF_TIME);
      moved[BUF_TIME-1] = '\0';
    }
    gtk_entry_set_text(entry, moved);
  }
}

void entry_format_time_on_focus_out(GtkEntry *entry, char separator)
{
  const char *txt;

  txt = gtk_entry_get_text(entry);
  if (txt && strlen(txt)) { 
    char result[BUF_TIME];
    format_time(txt, result, separator);
    if (separator == ':') {
      m_to_strtime(daywrap_minutes(strtime_to_m(result)), result, BUF_TIME, separator);
    }
    gtk_entry_set_text(entry, result);
  }
}

void entry_format_time_on_focus_in(GtkEntry *entry)
{
  const char *ptr;
  char result[BUF_TIME];
  int i = 0;

  ptr = gtk_entry_get_text(entry);

  while (*ptr != '\0' && i < BUF_TIME - 1) {
    if (*ptr >= '0' && *ptr <= '9') {
      result[i] = *ptr;
      i++;
    }
      ptr++;
  }  
  result[i] = '\0';

  gtk_entry_set_text(entry, result);
}

long elapsed_seconds(const char *date, const char *t1, const char *tz1, const char *t2, const char *tz2)
{
  /* this will not correctly compute across a DST change */
  return daywrap(dtz_mktime(date, t2, tz2) - dtz_mktime(date, t1, tz1));
}

/* Changed is an alias to either start, end, or elapsed. */
void reconcile_time_entries(Logbook *logb,
			    GtkEntry *changed,
			    GtkEntry *start,
			    GtkEntry *end,
			    GtkEntry *elapsed)
{
  const char *tstart, *tend, *telapsed, *tdate;
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
  if (!strlen(tdate)) {
    /* Times act screwy if date is blank */
    tdate = "1979-05-25";
  }

  flights_lookup_dep_tz(logb, deptz);
  flights_lookup_arr_tz(logb, arrtz);

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

/* Load the selected line into fields */
void flights_load_selection(Logbook *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *treemod;
  gchar
    *id=NULL,
    *tail=NULL,
    *fleetno=NULL,
    *role=NULL,
    *depicao=NULL,
    *arricao=NULL,
    *depiata=NULL,
    *arriata=NULL,
    *date=NULL,
    *leg=NULL,
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
    *tripdate=NULL,
    *crew=NULL,
    *notes=NULL,
    *dland=NULL,
    *nland=NULL,
    *xc=NULL,
    *hold=NULL;

  int _dland=0, _nland=0, _leg=0;

  if (gtk_tree_selection_get_selected (logb->flights_selection, &treemod, &iter)) {
    gtk_tree_model_get(treemod, &iter,
		       FLIGHTS_COL_ID, &id,
		       FLIGHTS_COL_DATE, &date,
		       FLIGHTS_COL_LEG, &leg,
		       FLIGHTS_COL_TAIL, &tail,
		       FLIGHTS_COL_FLEETNO, &fleetno,
		       FLIGHTS_COL_ROLE, &role,
		       FLIGHTS_COL_DEPICAO, &depicao,
		       FLIGHTS_COL_DEPIATA, &depiata,
		       FLIGHTS_COL_ARRICAO, &arricao,
		       FLIGHTS_COL_ARRIATA, &arriata,
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
		       FLIGHTS_COL_TRIPDATE, &tripdate,
		       -1);
    sscanf(EMPTY_IF_NULL(dland), "%d", &_dland);
    sscanf(EMPTY_IF_NULL(nland), "%d", &_nland);
    sscanf(EMPTY_IF_NULL(leg),   "%d", &_leg);
  }

  gtk_entry_set_text(GTK_ENTRY(logb->flights_tail), EMPTY_IF_NULL(tail));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_fleetno), EMPTY_IF_NULL(fleetno));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_role), EMPTY_IF_NULL(role));

  gtk_entry_set_text(GTK_ENTRY(logb->flights_depicao), EMPTY_IF_NULL(depicao));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_arricao), EMPTY_IF_NULL(arricao));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_depiata), EMPTY_IF_NULL(depiata));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_arriata), EMPTY_IF_NULL(arriata));

  gtk_entry_set_text(GTK_ENTRY(logb->flights_dur), EMPTY_IF_NULL(dur));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_night), EMPTY_IF_NULL(night));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_inst), EMPTY_IF_NULL(inst));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_siminst), EMPTY_IF_NULL(siminst));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->flights_hold), str_bool(hold));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_aprch), EMPTY_IF_NULL(aprch));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->flights_xc), str_bool(xc));
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(logb->flights_dland), _dland);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(logb->flights_nland), _nland);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(logb->flights_leg), _leg);

  text_view_set_text(GTK_TEXT_VIEW(logb->flights_crew), EMPTY_IF_NULL(crew));
  text_view_set_text(GTK_TEXT_VIEW(logb->flights_notes), EMPTY_IF_NULL(notes));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_fltno), EMPTY_IF_NULL(fltno));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_sdur), EMPTY_IF_NULL(sdur));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_trip), EMPTY_IF_NULL(trip));
  gtk_entry_set_text(GTK_ENTRY(logb->flights_tripdate), EMPTY_IF_NULL(tripdate));

  gtk_entry_set_text(GTK_ENTRY(logb->flights_date), EMPTY_IF_NULL(date));
  if (any_toggle_button_get_active(logb->flights_utc)) {
    gtk_entry_set_text(GTK_ENTRY(logb->flights_aout), EMPTY_IF_NULL(aoututc));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_ain), EMPTY_IF_NULL(ainutc));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sout), EMPTY_IF_NULL(soututc));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sin), EMPTY_IF_NULL(sinutc));
  } else {
    gtk_entry_set_text(GTK_ENTRY(logb->flights_aout), EMPTY_IF_NULL(aout));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_ain), EMPTY_IF_NULL(ain));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sout), EMPTY_IF_NULL(sout));
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sin), EMPTY_IF_NULL(sin));
  }
  flights_refresh_utilization(logb);

  g_free(id);
  g_free(date);
  g_free(leg);
  g_free(tail);
  g_free(fleetno);
  g_free(role);
  g_free(depicao);
  g_free(arricao);
  g_free(depiata);
  g_free(arriata);
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
  g_free(tripdate);
}

void flights_set_view_column_visibility(Logbook *logbook)
{
  char *headers[] = {
    "Date",
    "Leg",
    "Tail",
    "FleetNo",
    "Model",
    "Role",
    "Dur",
    "Night",
    "Inst",
    "SimInst",
    "Hold",
    "Aprch",
    "nApr",
    "XC",
    "Dist",
    "DLand",
    "NLand",
    "Crew",
    "Crw",
    "Notes",
    "Nts",
    "FltNo",
    "SDur",
    "Trip",
    "TripDate",
    "Over",
    NULL
  };
  GtkWidget *toggles[] = {
    logbook->flights_view_date,
    logbook->flights_view_leg,
    logbook->flights_view_tail,
    logbook->flights_view_fleetno,
    logbook->flights_view_model,
    logbook->flights_view_role,
    logbook->flights_view_dur,
    logbook->flights_view_night,
    logbook->flights_view_inst,
    logbook->flights_view_siminst,
    logbook->flights_view_hold,
    logbook->flights_view_aprch,
    logbook->flights_view_aprchn,
    logbook->flights_view_xc,
    logbook->flights_view_dist,
    logbook->flights_view_dland,
    logbook->flights_view_nland,
    logbook->flights_view_crew,
    logbook->flights_view_crewn,
    logbook->flights_view_notes,
    logbook->flights_view_notesn,
    logbook->flights_view_fltno,
    logbook->flights_view_sdur,
    logbook->flights_view_trip,
    logbook->flights_view_tripdate,
    logbook->flights_view_over,
    NULL
  };
  int i;

  for (i=0; headers[i]; i++) {
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   headers[i],
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(toggles[i])));
  }

  if (any_toggle_button_get_active(logbook->flights_icao_toggle)) {
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "DepICAO",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dep)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "ArrICAO",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_arr)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "DepIATA", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "ArrIATA", FALSE);
  } else {
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "DepIATA",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dep)));    
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "ArrIATA",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_arr)));    
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "DepICAO", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "ArrICAO", FALSE);
  }

  if (any_toggle_button_get_active(logbook->flights_utc)) {
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "SOutUTC",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sout)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "SInUTC",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sin)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "AOutUTC",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aout)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "AInUTC",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_ain)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "SOut", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "SIn", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "AOut", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "AIn", FALSE);
  } else {
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "SOut",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sout)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "SIn",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sin)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "AOut",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aout)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview),
					   "AIn",
					   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_ain)));
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "SOutUTC", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "SInUTC", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "AOutUTC", FALSE);
    store_view_set_column_visible_by_title(GTK_TREE_VIEW(logbook->flights_treeview), "AInUTC", FALSE);
  }
}

void flights_save_options(Logbook *logbook)
{
  int i=0;
  GtkTreeViewColumn *col;
  /* Column order */
  while ((col = gtk_tree_view_get_column(GTK_TREE_VIEW(logbook->flights_treeview), i))) {
    registry_set_int(logbook, "flights/columns/order", gtk_tree_view_column_get_title(col), i);
    i++;
  }

  /* View visibility */
  registry_set_int(logbook, "flights/view", "Date", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_date)));
  registry_set_int(logbook, "flights/view", "Leg",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_leg)));
  registry_set_int(logbook, "flights/view", "Tail", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_tail)));
  registry_set_int(logbook, "flights/view", "FleetNo", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_fleetno)));
  registry_set_int(logbook, "flights/view", "Model", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_model)));
  registry_set_int(logbook, "flights/view", "Role", gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_role)));
  registry_set_int(logbook, "flights/view", "Dep",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dep)));
  registry_set_int(logbook, "flights/view", "Arr",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_arr)));
  registry_set_int(logbook, "flights/view", "Dur",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dur)));
  registry_set_int(logbook, "flights/view", "Night",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_night)));
  registry_set_int(logbook, "flights/view", "Inst",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_inst)));
  registry_set_int(logbook, "flights/view", "SimInst",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_siminst)));
  registry_set_int(logbook, "flights/view", "Hold",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_hold)));
  registry_set_int(logbook, "flights/view", "Aprch",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aprch)));
  registry_set_int(logbook, "flights/view", "nApr",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aprchn)));
  registry_set_int(logbook, "flights/view", "XC",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_xc)));
  registry_set_int(logbook, "flights/view", "Dist",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dist)));
  registry_set_int(logbook, "flights/view", "DLand",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dland)));
  registry_set_int(logbook, "flights/view", "NLand",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_nland)));
  registry_set_int(logbook, "flights/view", "Crew",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_crew)));
  registry_set_int(logbook, "flights/view", "Crw",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_crewn)));
  registry_set_int(logbook, "flights/view", "Notes",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_notes)));
  registry_set_int(logbook, "flights/view", "Nts",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_notesn)));
  registry_set_int(logbook, "flights/view", "FltNo",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_fltno)));
  registry_set_int(logbook, "flights/view", "SDur",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sdur)));
  registry_set_int(logbook, "flights/view", "Trip",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_trip)));
  registry_set_int(logbook, "flights/view", "TripDate",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_tripdate)));
  registry_set_int(logbook, "flights/view", "Over",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_over)));
  registry_set_int(logbook, "flights/view", "SOut",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sout)));
  registry_set_int(logbook, "flights/view", "SIn",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sin)));
  registry_set_int(logbook, "flights/view", "AOut",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aout)));
  registry_set_int(logbook, "flights/view", "AIn",   gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_ain)));

  registry_set_int(logbook, "flights", "UTC", any_toggle_button_get_active(logbook->flights_utc));
  registry_set_int(logbook, "flights", "ICAO", any_toggle_button_get_active(logbook->flights_icao_toggle));
  registry_set_int(logbook, "flights", "FleetNo", any_toggle_button_get_active(logbook->flights_fleetno_toggle));

}

void flights_refresh(Logbook *logbook)
{
  static int lock=0;
  //  DBStatement *select;
  char sql[4096];
  const char *where_clause;
  long nrows;
  int err;
  char results_summary[128];

  /* refreshing while already refreshing is bad */
  if (lock) return;
  lock = 1;
  logbook->flights_stale = FALSE;

  flights_set_view_column_visibility(logbook);

  gtk_widget_hide(logbook->flights_results_summary);
  gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(logbook->flights_query_progress), 0.0);
  gtk_widget_show(logbook->flights_query_progress);

  where_clause = gtk_entry_get_text(GTK_ENTRY(logbook->flights_where));

  if (strlen(where_clause)) {
    snprintf(sql, sizeof(sql), "%s where %s ;", FLIGHTS_SELECT, where_clause);
    //    snprintf(sql_nrows, sizeof(sql_nrows), "select count(*) from flights where %s ;");
  } else {
    snprintf(sql, sizeof(sql), "%s ;", FLIGHTS_SELECT);
  }
  db_finalize(logbook->flights_select_all);
  if ((err = db_prepare(logbook->db, sql, &logbook->flights_select_all)) != DB_OK) {
    if (logbook->flights_select_all) {
      db_finalize(logbook->flights_select_all);
    }
    logbook->flights_select_all = db_prep(logbook->db, "SELECT 42 WHERE 0 = 1;");
  }

  gtk_label_set_text(GTK_LABEL(logbook->flights_results_summary), "");

  while (gtk_events_pending()) gtk_main_iteration();
  nrows = store_repopulate_from_stmt_with_progress(GTK_LIST_STORE(logbook->flights_treemodel),
						   logbook->flights_select_all,
						   GTK_PROGRESS_BAR(logbook->flights_query_progress));

  if (err == DB_OK) {
    snprintf(results_summary, sizeof(results_summary), "%ld Flights", nrows);
  } else {
    snprintf(results_summary, sizeof(results_summary), "Error");
  }
  gtk_label_set_text(GTK_LABEL(logbook->flights_results_summary), results_summary);

  flights_load_selection(logbook);

  flights_refresh_utilization(logbook);

  gtk_widget_hide(logbook->flights_query_progress);
  gtk_widget_show(logbook->flights_results_summary);

  lock = 0;
}

void flights_build_store_view(Logbook *logbook)
{
  store_build_query_stmt_widget(logbook->flights_select_all, flights_column_kinds, &logbook->flights_treeview, &logbook->flights_treemodel);
  gtk_widget_show_all(logbook->flights_treeview);
  gtk_container_add(GTK_CONTAINER(logbook->flights_sw), logbook->flights_treeview);
}

void flights_restore_options(Logbook *logbook)
{ 
  /* Set column order from registry */
  char *errmsg;
  DBResults *results;
  if ((results = db_get_table(logbook->db, "SELECT key FROM Registry WHERE path = 'flights/columns/order' ORDER BY value;", &errmsg))) {
    store_view_arrange_columns(GTK_TREE_VIEW(logbook->flights_treeview), results->row_count, results->table);
    db_results_free(results);
  } else {
    fprintf(stderr, "flights_build_store_view(): %s\n", errmsg);
  }

  /* Set view check boxes */
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_date),     str_bool(registry_get_text(logbook, "flights/view", "Date")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_leg),      str_bool(registry_get_text(logbook, "flights/view", "Leg")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_tail),     str_bool(registry_get_text(logbook, "flights/view", "Tail")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_fleetno),  str_bool(registry_get_text(logbook, "flights/view", "FleetNo")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_model),     str_bool(registry_get_text(logbook, "flights/view", "Model")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_role),     str_bool(registry_get_text(logbook, "flights/view", "Role")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dep),      str_bool(registry_get_text(logbook, "flights/view", "Dep")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_arr),      str_bool(registry_get_text(logbook, "flights/view", "Arr")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dur),      str_bool(registry_get_text(logbook, "flights/view", "Dur")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_night),    str_bool(registry_get_text(logbook, "flights/view", "Night")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_inst),     str_bool(registry_get_text(logbook, "flights/view", "Inst")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_siminst),  str_bool(registry_get_text(logbook, "flights/view", "SimInst")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_hold),     str_bool(registry_get_text(logbook, "flights/view", "Hold")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aprch),    str_bool(registry_get_text(logbook, "flights/view", "Aprch")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aprchn),   str_bool(registry_get_text(logbook, "flights/view", "nApr")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_xc),       str_bool(registry_get_text(logbook, "flights/view", "XC")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dist),     str_bool(registry_get_text(logbook, "flights/view", "Dist")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_dland),    str_bool(registry_get_text(logbook, "flights/view", "DLand")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_nland),    str_bool(registry_get_text(logbook, "flights/view", "NLand")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_crew),     str_bool(registry_get_text(logbook, "flights/view", "Crew")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_crewn),    str_bool(registry_get_text(logbook, "flights/view", "Crw")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_notes),    str_bool(registry_get_text(logbook, "flights/view", "Notes")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_notesn),   str_bool(registry_get_text(logbook, "flights/view", "Notes")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_fltno),    str_bool(registry_get_text(logbook, "flights/view", "FltNo")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sdur),     str_bool(registry_get_text(logbook, "flights/view", "SDur")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_trip),     str_bool(registry_get_text(logbook, "flights/view", "Trip")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_tripdate), str_bool(registry_get_text(logbook, "flights/view", "TripDate")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_over),     str_bool(registry_get_text(logbook, "flights/view", "Over")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sout),     str_bool(registry_get_text(logbook, "flights/view", "SOut")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_sin),      str_bool(registry_get_text(logbook, "flights/view", "SIn")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_aout),     str_bool(registry_get_text(logbook, "flights/view", "AOut")));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->flights_view_ain),      str_bool(registry_get_text(logbook, "flights/view", "AIn")));

  /* Set Toggles - enters both states to ensure toggled signal emission */
  if (registry_get_int(logbook, "flights", "UTC")) {
    any_toggle_button_set_active(logbook->flights_utc, FALSE);
    any_toggle_button_set_active(logbook->flights_utc, TRUE);
  } else {
    any_toggle_button_set_active(logbook->flights_utc, TRUE);
    any_toggle_button_set_active(logbook->flights_utc, FALSE);
  }
  if (registry_get_int(logbook, "flights", "ICAO")) {
    any_toggle_button_set_active(logbook->flights_icao_toggle, FALSE);
    any_toggle_button_set_active(logbook->flights_icao_toggle, TRUE);
  } else {
    any_toggle_button_set_active(logbook->flights_icao_toggle, TRUE);
    any_toggle_button_set_active(logbook->flights_icao_toggle, FALSE);
  }
  if (registry_get_int(logbook, "flights", "FleetNo")) {
    any_toggle_button_set_active(logbook->flights_fleetno_toggle, FALSE);
    any_toggle_button_set_active(logbook->flights_fleetno_toggle, TRUE);
  } else {
    any_toggle_button_set_active(logbook->flights_fleetno_toggle, TRUE);
    any_toggle_button_set_active(logbook->flights_fleetno_toggle, FALSE);
  }
}

int flights_error(Logbook *logbook)
{

  return 0;
}

static void iata_fills_icao(Logbook *logbook, GtkEntry *iata, GtkEntry *icao)
{
  DBStatement *stmt = logbook->flights_icao_from_iata;
  edctrl_ignore_modifications(logbook->flights_edctrl, TRUE);
  db_bind_text(stmt, 1, gtk_entry_get_text(iata));
  if (db_step(stmt) == DB_ROW) {
    gtk_entry_set_text(icao, EMPTY_IF_NULL((char*)db_column_text(stmt, 0)) );
  } else {
    gtk_entry_set_text(icao, "");
  }
  db_reset(stmt);
  db_clear_bindings(stmt);
  edctrl_ignore_modifications(logbook->flights_edctrl, FALSE);
}

static void icao_fills_iata(Logbook *logbook, GtkEntry *icao, GtkEntry *iata)
{
  DBStatement *stmt = logbook->flights_iata_from_icao;
  edctrl_ignore_modifications(logbook->flights_edctrl, TRUE);
  db_bind_text(stmt, 1, gtk_entry_get_text(icao));
  if (db_step(stmt) == DB_ROW) {
    gtk_entry_set_text(iata, EMPTY_IF_NULL((char*)db_column_text(stmt, 0)) );
  } else {
    gtk_entry_set_text(iata, "");
  }
  db_reset(stmt);
  db_clear_bindings(stmt);
  edctrl_ignore_modifications(logbook->flights_edctrl, FALSE);
}

void flights_handle_iata_update(Logbook *logbook, GtkEntry *iata, GtkEntry *icao)
{
  const char *text;
  int len;
  text = EMPTY_IF_NULL(gtk_entry_get_text(iata));
  len = strlen(text);
  if (len == 3) {
    iata_fills_icao(logbook, iata, icao);
  } else if (len == 4) {
    gtk_entry_set_text(icao, text);
    icao_fills_iata(logbook, icao, iata);
  } else {
    gtk_entry_set_text(iata, "");
  }
}

void flights_handle_icao_update(Logbook *logbook, GtkEntry *icao, GtkEntry *iata)
{
  const char *text;
  int len;
  text = EMPTY_IF_NULL(gtk_entry_get_text(icao));
  len = strlen(text);
  if (len == 4) {
    icao_fills_iata(logbook, icao, iata);
  } else if (len == 3) {
    gtk_entry_set_text(iata, text);
    iata_fills_icao(logbook, iata, icao);
  } else {
    gtk_entry_set_text(icao, "");
  }
}

