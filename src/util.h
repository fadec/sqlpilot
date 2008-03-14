#include <gtk/gtk.h>
#include "db/db.h"
#include "sqlpilot.h"

#define ALLOWED_TEXT_ALPHA

gboolean str_bool(const char *);

gchar *text_view_get_text(GtkTextView *);

void text_view_set_text(GtkTextView *, const char *);

int is_ident_char(char);
int is_time_char(char);

int daywrap_minutes(int);

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
int tz_of_airport_ident(DB *db, const char *ident, char *tz, int tz_bufsize);
int row_exists(DB *db, const char *table, const char *column, const char *value);
int bind_id_of(DBStatement *stmt, int i, const char *table, const char *column, const char *value);

int parseB60(const char *ts);
int parsetime(const char *ts, int b60numerals);
void format_time(const char *input, char *out, char separator);

