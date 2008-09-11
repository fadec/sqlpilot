
#include "sqlpilot.h"
#include "logbook.h"
#include "import.h"

void on_import_read_btn_clicked(GtkButton *btn, Logbook *logbook)
{
  int nrow;
  char status[64];

  switch (gtk_notebook_get_current_page(GTK_NOTEBOOK(logbook->import_input_notebook))) {
  case IMPORT_INPUT_TEXT:
    nrow = import_read_text(logbook);
    break;
  case IMPORT_INPUT_FILE:
    nrow = import_read_file(logbook);
    break;
  default: assert(!"Good");
  }

  snprintf(status, sizeof(status), "%d flights ready to import", nrow);
  gtk_label_set_text(GTK_LABEL(logbook->import_interpreted_status), status);
  gtk_widget_set_sensitive(logbook->import_write_controls, nrow);
}

void on_import_write_btn_clicked(GtkButton *btn, Logbook *logbook)
{
  import_write(logbook);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->import_write_normal), TRUE);
}

void on_import_input_expander_activate(GtkExpander *exp, Logbook *logbook)
{

}
void on_import_interpreted_expander_activate(GtkExpander *exp, Logbook *logbook)
{

}
void on_import_response_expander_activate(GtkExpander *exp, Logbook *logbook)
{

}
