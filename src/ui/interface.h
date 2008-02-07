#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>

#define _(String) String

/* confirmation responses */
#define CONFRESP_YES 1
#define CONFRESP_NO 2
#define CONFRESP_OK 3
#define CONFRESP_CANCEL 4
#define CONFRESP_ERROR 5

GtkWidget *logbook_notebook_build(Logbook *);
void build_main_window(void);


#endif
