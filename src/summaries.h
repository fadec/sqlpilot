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

#ifndef SUMMARIES_H
#define SUMMARIES_H

#include <sqlpilot.h>
#include "scripter.h"

#define SUMMARIES_DIR "script/summary"

enum {
  SUMMARIES_COL_FILENAME=0,
  SUMMARIES_COL_TITLE,
  SUMMARIES_NUMCOL
};

void summaries_init(Sqlpilot *sqlpilot);
void summaries_refresh(Sqlpilot *sqlpilot);
void summaries_rebuild_parameters(Sqlpilot *sqlpilot);

#endif
