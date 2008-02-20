#include <gtk/gtk.h>
#include "db/db.h"

#define ALLOWED_TEXT_ALPHA

gboolean str_bool(const char *);

gchar *text_view_get_text(GtkTextView *);

void text_view_set_text(GtkTextView *, const char *);

int is_ident_char(char);
int is_time_char(char);

void entry_clamp_text(GtkEntry *entry, int length, int setcase, int allowed(char));

void entry_clamp_roles_ident(GtkEntry *);
void entry_clamp_aircraft_ident(GtkEntry *);
void entry_clamp_types_ident(GtkEntry *);
void entry_clamp_airports_ident(GtkEntry *);

int hmstr_to_m(const char *str);

char *m_to_hmstr(int m);

int bind_id_of(DBStatement *stmt, int i, const char *table, const char *column, const char *value);
