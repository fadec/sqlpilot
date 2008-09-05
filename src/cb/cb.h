/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.                                       */
/*                                                                      */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.                                  */
/*                                                                      */
/* Sqlpilot is distributed in the hope that it will be useful,          */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.                         */
/*                                                                      */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/

#ifndef CB_H
#define CB_H

#include "sqlpilot.h"

void on_flights_selection_changed(GtkTreeSelection *, Logbook *);
void on_flights_crew_changed(GtkTextBuffer *, Logbook);
void on_flights_notes_changed(GtkTextBuffer *, Logbook);
void flights_refresh(Logbook *);

void on_roles_selection_changed(GtkTreeSelection *, Logbook *);
void roles_refresh(Logbook *);

void on_aircraft_selection_changed(GtkTreeSelection *, Logbook *);
void on_aircraft_notes_changed(GtkTextBuffer *, Logbook);
void aircraft_refresh(Logbook *);

void on_types_selection_changed(GtkTreeSelection *, Logbook *);
void types_refresh(Logbook *);

void on_airports_selection_changed(GtkTreeSelection *, Logbook *);
void on_airports_notes_changed(GtkTextBuffer *, Logbook);
void airports_refresh(Logbook *);

void on_reports_sql_text_changed(GtkTextBuffer *, Logbook *);

#endif
