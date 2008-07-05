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

#include "sqlpilot.h"
#include "cb/cb.h"
#include "logbook.h"

void on_window_destroy(GtkObject *object, Logbook *logbook)
{
  gtk_main_quit();
}

void on_notebook_switch_page(GtkNotebook *notebook, GtkNotebookPage *page, guint npage, Logbook *logbook)
{
  switch (npage) {
  case NOTEBOOK_PAGE_FLIGHTS:
    if (logbook->flights_stale) flights_refresh(logbook);
    break;
  case NOTEBOOK_PAGE_ROLES:
    if (logbook->roles_stale) roles_refresh(logbook);
    break;
  case NOTEBOOK_PAGE_AIRCRAFT:
    if (logbook->aircraft_stale) aircraft_refresh(logbook);
    break;
  case NOTEBOOK_PAGE_TYPES:
    if (logbook->types_stale) types_refresh(logbook);
    break;
  case NOTEBOOK_PAGE_AIRPORTS:
    if (logbook->airports_stale) airports_refresh(logbook);
    break;
  }
}
