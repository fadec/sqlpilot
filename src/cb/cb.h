#ifndef CB_H
#define CB_H

#include "sqlpilot.h"

void on_flights_selection_changed(GtkTreeSelection *, Sqlpilot *);
void on_roles_selection_changed(GtkTreeSelection *, Sqlpilot *);
void on_aircraft_selection_changed(GtkTreeSelection *, Sqlpilot *);
void on_types_selection_changed(GtkTreeSelection *, Sqlpilot *);
void on_airports_selection_changed(GtkTreeSelection *, Sqlpilot *);

#endif
