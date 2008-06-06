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

extern Sqlpilot *sqlpilot;

Logbook *logbook_open(const char *filename)
{
  Logbook *logb;

  logb = malloc(sizeof(Logbook));
  if (!(logb->db = db_open(filename))) {
    barf("Couldn't open database.");
  }
  logb->db_filename = filename;
  logbook_notebook_build(logb);
  return logb;
}

void logbook_close(Logbook *logbook)
{
  //g_free(logbook->flights_pane);  // Must free memory for gtk stuff
  db_close(logbook->db);
  free(logbook);
}

