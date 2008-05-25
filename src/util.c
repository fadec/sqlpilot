/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.				        */
/* 								        */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.				        */
/* 								        */
/* Sqlpilot is distributed in the hope that it will be useful,	        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.			        */
/* 								        */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#include "util.h"
#include <string.h>
#include <time.h>

gboolean str_bool(const char *str)
{
  return (str != NULL) && strcmp("", str) && strcmp("0", str) && strcmp("F", str) && strcmp("f", str) ? 1 : 0;
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

void text_view_set_text(GtkTextView *tv, const char *text)
{
  GtkTextBuffer *buffer;
  
  buffer = gtk_text_view_get_buffer(tv);
  gtk_text_buffer_set_text(buffer, text, -1);
}

int is_time_char(char c)
{
  return isdigit(c) || c == '.' || c == ':' || c == '+' || c == 'Z' || c == 'L' || c == 'U';
}

int is_ident_char(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-');
}

int is_num_char(char c)
{
  return (isdigit(c) || (c == '.') || (c == '-'));
}

void entry_clamp_text(GtkEntry *entry, int length, int setcase, int allowed(char))
{
  const gchar *text;
  gchar *clamped, *ptr_put, *ptr_get;
  
  text = gtk_entry_get_text(entry);

  switch (setcase) {
  case 1: clamped = g_ascii_strup(text, length);
    break;
  case -1: clamped = g_ascii_strdown(text, length);
    break;
  default: clamped = g_strndup(text, length);
  }

  ptr_put = ptr_get = clamped;
  while (*ptr_get) {
    if (allowed == NULL || allowed(*ptr_get)) {
      *ptr_put = *ptr_get;
      ptr_put++;
    }
    ptr_get++;
  }
  *ptr_put = '\0';

  gtk_entry_set_text(entry, clamped);
  g_free(clamped);
  /* *text need not be freed per gtk_entry_get_text docs */
}

void entry_clamp_roles_ident(GtkEntry *entry)
{
  entry_clamp_text(entry, 16, 1, is_ident_char);
}

void entry_clamp_aircraft_ident(GtkEntry *entry)
{
  entry_clamp_text(entry, 6, 1, is_ident_char);
}

void entry_clamp_types_ident(GtkEntry *entry)
{
  entry_clamp_text(entry, 16, 1, is_ident_char);
}

void entry_clamp_airports_ident(GtkEntry *entry)
{
  entry_clamp_text(entry, 4, 1, is_ident_char);
}

int strtime_to_m(const char *str)
{
  int h = 0, m = 0;
  if (strchr(str, ':')) {
    sscanf(str, "%d:%d", &h, &m);
  } else if (strchr(str, '+')) {
    sscanf(str, "%d+%d", &h, &m);
  } else {
    scanf(str, "%d", &m);
  }
  return h * 60 + m;
}

/* Format depends on sep */
void m_to_strtime(int m, char *str, int nstr, char sep)
{
  int hh, mm;
  char *format;

  hh = m / 60;
  mm = m - (hh * 60);

  format = (sep == ':') ? "%02d:%02d" : "%d+%02d";
  snprintf(str, nstr, format, hh, mm);
}

void tm_read_strdate(struct tm *tm, const char *str)
{
  int yy, mm, dd;
  yy = mm = dd = 0;

  sscanf(str, "%d-%d-%d", &yy, &mm, &dd);

  tm->tm_year = yy - 1900;
  tm->tm_mon = mm - 1;
  tm->tm_mday = dd;
}

void tm_read_strtime(struct tm *tm, const char *str)
{
  int hh, mm, ss;
  hh = mm = ss = 0;

  sscanf(str, "%d:%d", &hh, &mm);

  tm->tm_hour = hh;
  tm->tm_min = mm;
  tm->tm_sec = ss;
  tm->tm_isdst = -1;		/* -1 to mean info not available (implementaions hopefully figure it out from other info) */
}

/* Not thread safe - sets time zone */
time_t tmtz_mktime(struct tm *tm, const char *tz)
{
  char tzsave[256] = {0};
  time_t t;

  if (getenv("TZ")) {
    strncpy(tzsave, getenv("TZ"), sizeof(tzsave));
  }

  if (tzsave[sizeof(tzsave)-1]) {
    fprintf(stderr, "static char tzsave[] not big enough\n");
    exit(1);
  }

  if (setenv("TZ", tz, 1)) {
    fprintf(stderr, "boom\n");
    exit(1);
  }
  tzset();

  t = mktime(tm);

  if (strlen(tzsave)) {
    if (setenv("TZ", tzsave, 1)) {
      fprintf(stderr, "boom\n");
    }
  } else {
    if (unsetenv("TZ") == -1) {
      fprintf(stderr, "unsetenv failed\n");
    }
  }
  tzset();

  return t;
}

time_t dtz_mktime(const char *date, const char *time, const char *tz)
{
  struct tm tm;

  tm_read_strdate(&tm, date);
  tm_read_strtime(&tm, time);

  return tmtz_mktime(&tm, tz);
}

struct tm *localtime_tz(const time_t *timep, const char *tz, struct tm *retrn)
{
  char tzsave[256] = {0};
  struct tm *rt;

  if (getenv("TZ")) {
    strncpy(tzsave, getenv("TZ"), sizeof(tzsave));
  }
  if (tzsave[sizeof(tzsave)-1]) {
    fprintf(stderr, "static char tzsave[] not big enough\n");
    exit(1);
  }
  if (setenv("TZ", tz, 1)) {
    fprintf(stderr, "boom\n");
    exit(1);
  }
  tzset();

  rt = localtime_r(timep, retrn);

  if (strlen(tzsave)) {
    if (setenv("TZ", tzsave, 1)) {
      fprintf(stderr, "boom\n");
    }
  } else {
    if (unsetenv("TZ") == -1) {
      fprintf(stderr, "unsetenv failed\n");
    }
  }
  tzset();

  return rt;
}

void reconcile_time(ReconcileTime rc, struct tm *t1, const char *tz1, struct tm *t2, const char *tz2, long *elapsed)
{
  time_t t1_t, t2_t;

  switch (rc) {
  case RECONCILE_TIME_BEGIN:
    t2_t = tmtz_mktime(t2, tz2);
    t1_t = t2_t - *elapsed;
    localtime_tz(&t1_t, tz1, t1);
    break;
  case RECONCILE_TIME_END:
    t1_t = tmtz_mktime(t1, tz1);
    t2_t = t1_t + *elapsed;
    localtime_tz(&t2_t, tz2, t2);
    break;
  case RECONCILE_TIME_ELAPSED:
    t1_t = tmtz_mktime(t1, tz1);
    t2_t = tmtz_mktime(t2, tz2);
    *elapsed = t2_t - t1_t;
    break;
  }
}

long daywrap(long m)
{
  if (m < 0 || m >= 24 * 3600) m %= (24 * 3600);
  if (m < 0) m += (24 * 3600);
  return m;
}
/* daywrap_minutes(25 * 60) = 1 * 60, daywrap_minutes(-1 * 60) = 23 * 60 */
long daywrap_minutes(long m)
{
  // Is this the only way to do an always positive machine independent modulus?
  if (m < 0 || m >= 24 * 60) m %= (24 * 60);
  if (m < 0) m += (24 * 60);
  return m;
}

/* strdate_r and strtime_r must be at least BUF_DATE AND BUF_TIME in size */
void move_time(const char *fromtz, const char *totz, const char *strdate, const char *strtime, char *strdate_r, char *strtime_r)
{
  if (strtime && strlen(strtime)) {
    time_t t;
    struct tm tm;

    if (strdate && strlen(strdate)) {
      tm_read_strdate(&tm, strdate);
    } else {
      time(&t);
      localtime_r(&t, &tm);
    } 
    tm_read_strtime(&tm, strtime);
    
    if ((t = tmtz_mktime(&tm, fromtz)) == -1) {
      fprintf(stderr, "tmtz_mktime failed in move_time\n");
      return;
    }
    localtime_tz(&t, totz, &tm);
    strftime(strtime_r, BUF_TIME, "%H:%M", &tm);
    strftime(strdate_r, BUF_DATE, "%Y-%m-%d", &tm);
  } else {
    *strtime_r = '\0';
    strncpy(strdate_r, strdate, BUF_DATE);
    strdate_r[BUF_DATE - 1] = '\0';
  }
  //fprintf(stderr, "move_time -- fromtz: %s, totz: %s\n", fromtz, totz);
}

/* Returns 1 if ident found, otherwise 0 - places  */
int tz_of_airport_ident(DB *db, const char *ident, char *tz, int tz_bufsize)
{
  DBStatement *select;
  char sql[] = "select tzone from airports where ident = ?;";
  int found;
  char *tztext;

  tz[tz_bufsize - 1] = 0;
  select = db_prep(db, sql);
  db_bind_text(select, 1, ident);
  found = (db_step(select) != DB_DONE);

  if (found) {
    tztext = (char *)db_column_text(select, 0);
  }
  if (!tztext || !strlen(tztext)) {
    tztext = "UTC";
  }

  strncpy(tz, tztext, tz_bufsize);

  if (tz[tz_bufsize - 1] != 0) {
    fprintf(stderr, "Timezone buffer passed to tz_of_airport_ident() too small\n");
    exit(1);
  }

  db_stp_res_clr(select);

  return found;
}

int row_exists(DB *db, const char *table, const char *column, const char *value)
{
  DBStatement *select;
  char sql[256];
  int ret;

  snprintf(sql, sizeof(sql), "select %s from %s where %s = ?;", column, table, column);
  select = db_prep(db, sql);
  db_bind_text(select, 1, value);

  ret = (db_step(select) != DB_DONE);

  db_finalize(select);
  return ret;
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


/* Parses base sixty of base ten segments (00:00.000:00:0) and returns value */
/* Base10 fractionals are truncated to the nearest Base60 integer e.g. */
/* parseB60("1.05:") ==  63, and parseB60("1.05") == 1 */
int parseB60(const char *ts)
{
  long
    dec=0,			/* decimal */
    frac=0,			/* fractional part */
    fracp=0,			/* 1/power of the frac part */
    val=0;			/* value */
  char c;
  while ((c = *ts++)) {
    if (c >= '0' && c <= '9') {
      if (fracp) {
	frac *= 10;
	frac += c - '0';
	fracp *= 10;
      } else {
	dec *= 10;
	dec += c - '0';
      }
    }
    else if (c == ':' || c == '+') {
      val += dec;
      val *= 60;
      if (fracp) val += frac * 60 / fracp;
      dec = frac = fracp = 0;
    }
    else if (c == '.') {
      frac = 0;
      fracp = 1;
    }
  }
  val += dec;
  if (fracp) val += frac / fracp;
  return val;
}

int parsetime(const char *ts, int b60numerals)
{
  char s[32], *sp=s;
  
  strncpy(s, ts, sizeof(s));
  s[sizeof(s) - 1] = '\0';

  while (sp++) {
    if (*sp == ':' || *sp == '+') --b60numerals;
    if (!b60numerals) {
      *sp = '\0';
      break;
    }
  }

  //printf("%s\n", s);

  return parseB60(s);
}

/* formats strings like 234 to 02:34 */
/* out must be a buffer of at least length BUF_TIME */
void format_time(const char *input, char *out, char separator)
{
  if (strlen(input)) {
    if (strstr(input, ".")) {
      float hours;
      sscanf(input, "%f", &hours);
      m_to_strtime(hours * 60, out, BUF_TIME, separator);
    } else {
      char str[BUF_TIME], *ptr;
      char str2[BUF_TIME];
      int i = 0;
      strncpy(str, input, BUF_TIME);
      str[BUF_TIME-1] = '\0';
      ptr = g_strreverse(str);
      while (i < BUF_TIME - 1) {
	if (i == 2) {		/* BUF_TIME must be >= 4 */
	  str2[i] = separator;
	  i++;
	}
	if (*ptr >= '0' && *ptr <= '9') {
	  str2[i] = *ptr;
	  i++;
	}
	if (*ptr == '\0') {
	  /* format like (separator == ':') ? "%02d:%02d" : "%d:%02d */
	  str2[i] = (i < 4 || separator == ':') ? '0' : '\0';
	  i++;
	} else {
	  ptr++;
	}
      }
      str2[i] = '\0';
      g_strreverse(str2);
      m_to_strtime(strtime_to_m(str2), out, BUF_TIME, separator);
    } 
  }
  else {
    out[0] = '\0';
  }
}
