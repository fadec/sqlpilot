
#include "logbook.h"
#include "import.h"

static void import_script_selector_init(Logbook *logbook)
{
  filename_combo_box_build_model(GTK_COMBO_BOX(logbook->import_script));
  filename_combo_box_merge_dir(GTK_COMBO_BOX(logbook->import_script), IMPORT_SCRIPT_DIR);
  gtk_combo_box_set_active(GTK_COMBO_BOX(logbook->import_script), 0);
}

void import_init(Logbook *logbook)
{
  import_script_selector_init(logbook);
}

void import_setup_script_options(Logbook *logbook)
{

}

void import_read_text(Logbook *logbook)
{

}

void import_read_file(Logbook *logbook)
{

}
