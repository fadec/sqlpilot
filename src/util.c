#include "util.h"
#include <string.h>

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
  return (c >= '0' && c <= '9') || c == '.' || c == ':' || c == '+';
}

int is_ident_char(char c)
{
  return (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '-');
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
  if (setenv("TZ", tzsave, 1)) {
    fprintf(stderr, "boom\n");
  }

  return t;
}

/* daywrap_minutes(25 * 60) = 1 * 60, daywrap_minutes(-1 * 60) = 23 * 60 */
int daywrap_minutes(int m)
{
  // Is this the only way to do an always positive machine independent modulus?
  if (m < 0 || m >= 24 * 60) m %= (24 * 60);
  if (m < 0) m += (24 * 60);
  return m;
}

/* Returns 1 if ident found, otherwise 0 - places  */
int tz_of_airport_ident(DB *db, const char *ident, char *tz, int tz_bufsize)
{
  DBStatement *select;
  char sql[] = "select tzone from airports where ident = ?;";
  int found;
  char *tztext;

  select = db_prep(db, sql);

  db_bind_text(select, 1, ident);

  found = (db_step(select) != DB_DONE);

  if (found) {
    tz[tz_bufsize - 1] = 0;
    tztext = (char *)db_column_text(select, 0);
    if (tztext) strncpy(tz, tztext, tz_bufsize);
    if (tz[tz_bufsize - 1] != 0) {
      fprintf(stderr, "Timezone buffer passed to tz_of_airport_ident() too small\n");
      exit(1);
    }
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

  printf("%s\n", s);

  return parseB60(s);
}

/* formats strings like 234 to 02:34 */
/* out must be a buffer of at least length BUF_TIME */
void format_time(const char *input, char *out, char separator)
{
  char str[BUF_TIME], *ptr;
  int i = 0;

  strncpy(str, input, BUF_TIME);
  str[BUF_TIME] = '\0';
  ptr = g_strreverse(str);

  if (strlen(str)) {
    while (i < BUF_TIME - 1) {
	if (i == 2) {		/* BUF_TIME must be >= 4 */
	  out[i] = separator;
	  i++;
	}
	if (*ptr >= '0' && *ptr <= '9') {
	  out[i] = *ptr;
	  i++;
	}
	if (*ptr == '\0') {
	  /* format like (separator == ':') ? "%02d:%02d" : "%d:%02d */
	  out[i] = (i < 4 || separator == ':') ? '0' : '\0';
	  i++;
	} else {
	  ptr++;
	}
      }
    out[i] = '\0';
    g_strreverse(out);
  } else {
    out[0] = '\0';
  }
}
