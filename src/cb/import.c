
#include "sqlpilot.h"
#include "logbook.h"
#include "import.h"

void on_import_read_btn_clicked(GtkButton *btn, Logbook *logbook)
{
  switch (gtk_notebook_get_current_page(GTK_NOTEBOOK(logbook->import_input_notebook))) {
  case IMPORT_INPUT_TEXT:
    import_read_text(logbook);
    break;
  case IMPORT_INPUT_FILE:
    import_read_file(logbook);
    break;
  default: assert(!"Good");
  }
}

void on_import_write_btn_clicked(GtkButton *btn, Logbook *logbook)
{

}

void on_import_script_changed(GtkComboBox *cbox, Logbook *logbook)
{
  import_setup_script_options(logbook);
}
