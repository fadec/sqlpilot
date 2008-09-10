
#include <libgtkhtml/document/htmldocument.h>
#include <libgtkhtml/view/htmlview.h>


#include "sqlpilot.h"
#include "summaries.h"
#include "logbook.h"


void summaries_refresh(Logbook *logbook)
{
  gchar *sstdout, *sstderr;
  gchar *html_src;
  GError *error=NULL;
  char *argv[32];
  int exit_code;

  argv[0] = scripter_get_script_filename(logbook->summaries_scripter);
  argv[1] = logbook->db_filename;
  scripter_get_parameter_values(logbook->summaries_scripter, argv+2);

  if (spawn_script(NULL, argv, NULL, "", &sstdout, &sstderr, &exit_code, &error, NULL, NULL)) {
    html_src = sstdout;
  } else {
    html_src = error->message;
  }

  html_document_clear(logbook->summaries_html_document);
  if (html_document_open_stream (logbook->summaries_html_document, "text/html")) {
    html_document_write_stream (logbook->summaries_html_document, html_src, strlen(html_src)); 
  }

  html_document_close_stream(logbook->summaries_html_document);

  g_free(sstdout);
  g_free(sstderr);
}

static void summaries_html_init(Logbook *logbook)
{
  logbook->summaries_html_view = html_view_new();
  logbook->summaries_html_document = html_document_new();
  html_view_set_document(HTML_VIEW(logbook->summaries_html_view), logbook->summaries_html_document);
  gtk_container_add(GTK_CONTAINER(logbook->summaries_sw), logbook->summaries_html_view);
  gtk_widget_show(logbook->summaries_html_view);
}

void summaries_init(Logbook *logbook)
{
  summaries_html_init(logbook);
  scripter_init(logbook->summaries_scripter, GTK_COMBO_BOX(logbook->summaries_select_summary), GTK_BOX(logbook->summaries_parameters));
  scripter_merge_script_dir(logbook->summaries_scripter, SUMMARIES_DIR);
  gtk_combo_box_set_active(GTK_COMBO_BOX(logbook->summaries_select_summary), 0);
}
