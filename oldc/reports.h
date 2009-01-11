#include "sqlpilot.h"
#include "logbook.h"

enum {
  REPORTS_COL_TITLE,
  REPORTS_COL_SQL
};

enum {
  REPORTS_WRITE_TITLE = 1,
  REPORTS_WRITE_SQL
};

void reports_refresh(Logbook *);
void reports_save(Logbook *);
void reports_delete(Logbook *);
void reports_title_combo_init(Logbook *logbook);
