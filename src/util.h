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
void entry_clamp_aircraft_fleetno(GtkEntry *);
void entry_clamp_types_ident(GtkEntry *);
void entry_clamp_airports_iata(GtkEntry *);
void entry_clamp_airports_icao(GtkEntry *);
void entry_format_date_on_focus_out(GtkEntry *);

int strtime_to_m(const char *str);
void m_to_strtime(int m, char *str, int nstr, char sep);
void tm_read_strdate(struct tm *tm, const char *str);
void tm_read_strtime(struct tm *tm, const char *str);
time_t tmtz_mktime(struct tm *tm, const char *tz);
time_t dtz_mktime(const char *date, const char *time, const char *tz);
int tz_of_airport(DBStatement *select, const char *key, char *tz, int tz_bufsize);
void tz_of_airport_or_utc(DBStatement *select, const char *key, char *tz, int tz_bufsize);
struct tm *localtime_tz(const time_t *timep, const char *tz, struct tm *retrn);
void move_time(const char *fromtz, const char *totz, const char *strdate, const char *strtime, char *strdate_r, char *strtime_r);
int row_exists(DB *db, const char *table, const char *column, const char *value);
int find_row_id(DB *db, const char *table, const char *column, const char *value, DBint64 *r_id);
int bind_id_of(DBStatement *stmt, int i, const char *table, const char *column, const char *value);
int unique_but_for(DB *db, const char *table, const char *col, const char *value, const char *butcol, const char *butvalue);
char *get_text_from_tree_selection(GtkTreeSelection *tsel, int column);

int parseB60(const char *ts);
int parsetime(const char *ts, int b60numerals);
void format_time(const char *input, char *out, char separator);

gboolean spawn_script(const gchar *wdir,
		      gchar **argv,
		      gchar **envp,
		      gchar *standard_input,
		      gchar **standard_output,
		      gchar **standard_error,
		      gint *exit_code,
		      GError **error,
		      int busy_func(void*),
		      void *busy_data);

void filename_combo_box_build_model(GtkComboBox *cbox);
void filename_combo_box_merge_dir(GtkComboBox *cbox, const char *dir);

#endif
