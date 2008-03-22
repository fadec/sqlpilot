#include "sqlpilot.h"

#include <gtk/gtk.h>

int
main (int argc, char *argv[])
{
  Sqlpilot *sqlpilot;
  char *dbfilename;
  int console=FALSE;
  
  if (argc < 2) {
    fprintf(stderr, "Usage: sqlpilot <filename>\n");
    exit(0);
  }

  if (*argv[1] == '-') {
    if (argc < 3) {
	fprintf(stderr, "duh\n");
	exit(1);
    }
    dbfilename = argv[2];
    console = TRUE;
  } else {
    dbfilename = argv[1];
  }

  if (console) {
    
  } else {
    gtk_init (&argc, &argv);
    if ((sqlpilot = sqlpilot_new(dbfilename)) == NULL) exit(1);
    gtk_widget_show (sqlpilot->window);
    gtk_main ();
    sqlpilot_finalize (sqlpilot);    
  }

  return 0;
}
