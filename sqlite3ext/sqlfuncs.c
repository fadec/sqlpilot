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

/* Build like this:
 * gcc -c -fPIC db.c
 * gcc -shared -Wl,-soname,libdb.so -o libdb.so  db.o -lm -lsqlite3
 * and load with sql:
 * select load_extension("./libdb.so");
 */

#include <sqlite3ext.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

SQLITE_EXTENSION_INIT1

/* Takes a numeric column representing minutes and converts it to a hh+mm string. */
/* Returns null if result would be 00+00 */
static void m_to_hhmm_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  char z[30];
  int m=0, am, hh, mm;
  //assert(argc == 1);
  switch (sqlite3_value_type(argv[0])) {
  case SQLITE_NULL:
    return;
  case SQLITE_TEXT:
  case SQLITE_BLOB:
    break;
  case SQLITE_FLOAT:
  case SQLITE_INTEGER: m = sqlite3_value_int(argv[0]);
  }
  am = abs(m);
  hh = am / 60;
  mm = am - (hh * 60);
  snprintf(z, 30, "%s%d+%02d", m < 0 ? "-" : "", hh, mm);
  sqlite3_result_text(context, z, -1, SQLITE_TRANSIENT);
}

/* Takes time string in hhhhhh+mm format and returns integer minutes */
/* Integers, floats, and string integers */
static void hhmm_to_m_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  int hh=0, mm=0;
  const char *str;
  switch (sqlite3_value_type(argv[0])) {
  case SQLITE_NULL:
    return;
  case SQLITE_BLOB:
    break;
  case SQLITE_FLOAT:
  case SQLITE_INTEGER:
    
    break;
  case SQLITE_TEXT:
    str = (char *)sqlite3_value_text(argv[0]);
    sscanf(str, "%d+%d", &hh, &mm);
    sqlite3_result_int(context, (abs(hh) * 60 + mm) * (str[0] == '-' ? -1 : 1));
    break;
  }
}

/* formats a column as a text bool "T" "F" - NULL cast to F*/
/* text [ "" | "0" | "F" | "f" ] = false, else true */
static void bool_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  int b=0;
  const char *str;

  switch (sqlite3_value_type(argv[0])) {
  case SQLITE_NULL:
    return;
    break;
  case SQLITE_FLOAT: b = (sqlite3_value_double(argv[0]) != 0.0);
    break;
  case SQLITE_INTEGER: b = sqlite3_value_int(argv[0]);
    break;
  case SQLITE_BLOB:
  case SQLITE_TEXT:
    str = (char *)sqlite3_value_text(argv[0]);
    b = strcmp("", str) && strcmp("0", str) && strcmp("F", str) && strcmp("f", str);
    break;
  }
  sqlite3_result_text(context, b ? "X" : "", -1, SQLITE_STATIC);
}

/* Compute according to Haversine Formula
 * (from R.W. Sinnott, "Virtues of the Haversine", Sky and Telescope, vol. 68, no. 2, 1984, p. 159)
 * dlon = lon2 - lon1
 * dlat = lat2 - lat1
 * a = sin^2(dlat/2) + cos(lat1) * cos(lat2) * sin^2(dlon/2)
 * c = 2 * arcsin(min(1,sqrt(a)))
 * d = R * c
 */

/* Parameters are lat and lon in radians */
/* Returns great circle radians */
static double great_circle(double lat1, double lon1, double lat2, double lon2)
{
  double sin_dlon_ov2, sin_dlat_ov2, sqrt_a;
  sin_dlat_ov2 = sin(lat2 - lat1) / 2;
  sin_dlon_ov2 = sin(lon2 - lon1) / 2;
  sqrt_a = sqrt(sin_dlat_ov2 * sin_dlat_ov2 + cos(lat1) * cos(lat2) * sin_dlon_ov2 * sin_dlon_ov2);
  return 2 * asin(sqrt_a > 1.0 ? 1.0 : sqrt_a);
}

static void dist_nm(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  double lat1, lat2, lon1, lon2, d, conv;
  int i;

  conv = 3.14159265 / 180;

  for (i=0;i<4;i++) {
    if (sqlite3_value_type(argv[i]) == SQLITE_NULL) return;
  }

  lat1 = sqlite3_value_double(argv[0]) * conv;
  lon1 = sqlite3_value_double(argv[1]) * conv;
  lat2 = sqlite3_value_double(argv[2]) * conv;
  lon2 = sqlite3_value_double(argv[3]) * conv;

  d = 3437.74677143 * great_circle(lat1, lon1, lat2, lon2);

  sqlite3_result_double(context, d);
}

/* Counts number of non-empty lines */
static void linecount_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  int n, isempty;
  char *str;

  switch (sqlite3_value_type(argv[0])) {
  case SQLITE_NULL:
  case SQLITE_FLOAT:
  case SQLITE_INTEGER:
    break;
  case SQLITE_BLOB:
  case SQLITE_TEXT:
    for (str = (char *)sqlite3_value_text(argv[0]), n = 0, isempty = 1; *str; str++) {
      if (isempty && !isspace(*str)) {
	n++;
	isempty = 0;
      } 
      if (*str == '\n') {
	isempty = 1;
      }
    }
    sqlite3_result_int(context, n);
    break;
  }
}

int sqlite3_extension_init(
			   sqlite3 *db,          /* The database connection */
			   char **pzErrMsg,      /* Write error messages here */
			   const sqlite3_api_routines *pApi  /* API methods */
			   ) {
  SQLITE_EXTENSION_INIT2(pApi);

  sqlite3_create_function(db, "m_to_hhmm", 1, SQLITE_ANY, 0, m_to_hhmm_func, 0, 0);
  sqlite3_create_function(db, "hm", 1, SQLITE_ANY, 0, m_to_hhmm_func, 0, 0);
  sqlite3_create_function(db, "hhmm_to_m", 1, SQLITE_ANY, 0, hhmm_to_m_func, 0, 0);
  sqlite3_create_function(db, "bool", 1, SQLITE_ANY, 0, bool_func, 0, 0);
  sqlite3_create_function(db, "linecount", 1, SQLITE_ANY, 0, linecount_func, 0, 0);
  sqlite3_create_function(db, "dist_nm", 4, SQLITE_ANY, 0, dist_nm, 0, 0);

  return 0;
}
