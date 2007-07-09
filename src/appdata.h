
#ifndef MAEMOPAD_APPDATA_H
#define MAEMOPAD_APPDATA_H

#include <libosso.h>

typedef struct _AppData AppData;

struct _AppData
{
    HildonProgram *program; /* handle to application */
    HildonWindow *window; /* handle to app's window */
    osso_context_t *osso; /* handle to osso */
};

#endif
