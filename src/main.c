#include "sqlpilot.h"

#include <gtk/gtk.h>

int
main (int argc, char *argv[])
{
  Sqlpilot *sqlpilot;
  
  if (argc < 2) {
    fprintf(stderr, "Usage: sqlpilot <filename>\n");
    exit(0);
  }

  gtk_init (&argc, &argv);

  if ((sqlpilot = sqlpilot_new(argv[1])) == NULL) return 1;
  gtk_widget_show (sqlpilot->window);
  gtk_main ();
  sqlpilot_finalize (sqlpilot);

  return 0;
}
