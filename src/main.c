#include "sqlpilot.h"

#include <gtk/gtk.h>

int
main (int argc, char *argv[])
{
  Sqlpilot *sqlpilot;
  
  gtk_init (&argc, &argv);

  if ((sqlpilot = sqlpilot_new()) == NULL) return 1;
  gtk_widget_show (sqlpilot->window);
  gtk_main ();
  sqlpilot_finalize (sqlpilot);

  return 0;
}
