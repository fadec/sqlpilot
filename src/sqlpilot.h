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


#ifndef SQLPILOT_H
#define SQLPILOT_H

#define _GNU_SOURCE //for strcasestr

#define UI_XML_FILE  "data/ui/interface.xml"

typedef struct Logbook Sqlpilot;
typedef struct Logbook Logbook;
typedef struct Edctrl Edctrl;
typedef struct ParameterPane ParameterPane;
typedef struct ParameterPane Scripter;


#include "config.h"
#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "db/db.h"
#include "util.h"
#include "store.h"
  
#define IMAGE_KEY   GTK_STOCK_INDEX
#define IMAGE_FKEY  GTK_STOCK_JUMP_TO
#define IMAGE_INVAL GTK_STOCK_STOP

/* id column for select satements is always 0 */
#define COL_ID 0

#define EMPTY_IF_NULL(x) (x ? x : "")

#define BUF_DATE 11		/* "YYYY-MM-DD\0" */
#define BUF_TIME 6		/* "HH:MM\0" expecting no flight longer than 99 hours */
#define BUF_DATETIME 17		/* "YYYY-MM-DD HH:MM\0" */
#define BUF_TZ 256		/*  How ever big timezone names get??? */

enum {
  NOTEBOOK_PAGE_FLIGHTS = 0,
  NOTEBOOK_PAGE_ROLES,
  NOTEBOOK_PAGE_AIRCRAFT,
  NOTEBOOK_PAGE_TYPES,
  NOTEBOOK_PAGE_AIRPORTS
};

void barf(const char *message);

#endif
