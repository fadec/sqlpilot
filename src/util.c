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

