#include <gtk/gtk.h>

#define ALLOWED_TEXT_ALPHA

gboolean str_bool(const char *);

gchar *text_view_get_text(GtkTextView *);

void text_view_set_text(GtkTextView *, const char *);

int is_ident_char(char);

void entry_clamp_text(GtkEntry *entry, int length, int setcase, int allowed(char));

