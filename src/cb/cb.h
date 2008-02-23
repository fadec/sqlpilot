#ifndef CB_H
#define CB_H

#include "sqlpilot.h"

void on_flights_selection_changed(GtkTreeSelection *, Sqlpilot *);
void flights_refresh(Sqlpilot *);

void on_roles_selection_changed(GtkTreeSelection *, Sqlpilot *);
void roles_refresh(Sqlpilot *);

void on_aircraft_selection_changed(GtkTreeSelection *, Sqlpilot *);
void aircraft_refresh(Sqlpilot *);

void on_types_selection_changed(GtkTreeSelection *, Sqlpilot *);
void types_refresh(Sqlpilot *);

void on_airports_selection_changed(GtkTreeSelection *, Sqlpilot *);
void airports_refresh(Sqlpilot *);

#endif
