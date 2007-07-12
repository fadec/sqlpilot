
#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>

#define _(String) gettext(String)

/* confirmation responses */
#define CONFRESP_YES 1
#define CONFRESP_NO 2
#define CONFRESP_OK 3
#define CONFRESP_CANCEL 4
#define CONFRESP_ERROR 5

typedef enum {
    SQLPILOT_NO_ERROR = 0,
    SQLPILOT_ERROR_INVALID_URI,
    SQLPILOT_ERROR_SAVE_FAILED,
    SQLPILOT_ERROR_OPEN_FAILED
} SqlpilotError;

#endif
