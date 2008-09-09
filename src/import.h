
#include "sqlpilot.h"

#define IMPORT_SCRIPT_DIR "script/import"

enum {
  IMPORT_INPUT_TEXT=0,
  IMPORT_INPUT_FILE
};

void import_init(Logbook *logbook);

void import_setup_script_options(Logbook *logbook);

int import_read_file(Logbook *logbook);
int import_read_text(Logbook *logbook);

void import_write(Logbook *logbook);
