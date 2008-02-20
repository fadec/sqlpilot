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

int hmstr_to_m(const char *str)
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

char *m_to_hmstr(int m)
{
  int hh, mm;
  char *str;

  hh = m / 60;
  mm = m - (hh * 60);

  str = g_strnfill(10, '\0');

  snprintf(str, 10, "%d:%02d", hh, mm);

  return str;
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

