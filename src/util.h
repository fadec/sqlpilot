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

#ifndef UTIL_H
#define UTIL_H

#include <gtk/gtk.h>
#include "db/db.h"
#include "sqlpilot.h"

#define ALLOWED_TEXT_ALPHA

typedef enum {
  RECONCILE_TIME_BEGIN,
  RECONCILE_TIME_END,
  RECONCILE_TIME_ELAPSED
} ReconcileTime;

gboolean str_bool(const char *);

gchar *text_view_get_text(GtkTextView *);

void text_view_set_text(GtkTextView *, const char *);

int is_ident_char(char);
int is_time_char(char);
int is_num_char(char);

long daywrap(long);
long daywrap_minutes(long);

void entry_clamp_text(GtkEntry *entry, int length, int setcase, int allowed(char));

void entry_clamp_roles_ident(GtkEntry *);
void entry_clamp_aircraft_ident(GtkEntry *);
void entry_clamp_types_ident(GtkEntry *);
void entry_clamp_airports_ident(GtkEntry *);

int strtime_to_m(const char *str);
void m_to_strtime(int m, char *str, int nstr, char sep);
void tm_read_strdate(struct tm *tm, const char *str);
void tm_read_strtime(struct tm *tm, const char *str);
time_t tmtz_mktime(struct tm *tm, const char *tz);
time_t dtz_mktime(const char *date, const char *time, const char *tz);
int tz_of_airport_ident(DB *db, const char *ident, char *tz, int tz_bufsize);
struct tm *localtime_tz(const time_t *timep, const char *tz, struct tm *retrn);
void move_time(const char *fromtz, const char *totz, const char *strdate, const char *strtime, char *strdate_r, char *strtime_r);
int row_exists(DB *db, const char *table, const char *column, const char *value);
int bind_id_of(DBStatement *stmt, int i, const char *table, const char *column, const char *value);

int parseB60(const char *ts);
int parsetime(const char *ts, int b60numerals);
void format_time(const char *input, char *out, char separator);

#endif
