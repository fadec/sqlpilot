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

void on_window_destroy(GtkObject *object, Sqlpilot *sqlpilot)
{
  gtk_main_quit();
}

void on_notebook_switch_page(GtkNotebook *notebook, GtkNotebookPage *page, guint npage, Sqlpilot *sqlpilot)
{
  switch (npage) {
  case NOTEBOOK_PAGE_FLIGHTS:
    if (sqlpilot->flights_stale) flights_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_ROLES:
    if (sqlpilot->roles_stale) roles_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_AIRCRAFT:
    if (sqlpilot->aircraft_stale) aircraft_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_TYPES:
    if (sqlpilot->types_stale) types_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_AIRPORTS:
    if (sqlpilot->airports_stale) airports_refresh(sqlpilot);
    break;
  }
}
