
#ifndef _SQLPILOT_H_
#define _SQLPILOT_H_

#include <gtk/gtk.h>
#include <stdlib.h>
#include "db/db.h"
#include "logbook.h"
#include "store.h"
#include "ui/interface.h"

#define EXIT_SUCESS 0
#define EXIT_BARF  1

typedef enum {
    SQLPILOT_NO_ERROR = 0,
    SQLPILOT_ERROR_INVALID_URI,
    SQLPILOT_ERROR_SAVE_FAILED,
    SQLPILOT_ERROR_OPEN_FAILED
} SqlpilotError;

typedef struct Sqlpilot Sqlpilot;
struct Sqlpilot {
  Logbook *logbook;
  Interface *interface;
};

Sqlpilot *sqlpilot; /* Main app global var */
void sqlpilot_init(void);
void sqlpilot_finalize(void);

void barf(const char *message);

#endif
