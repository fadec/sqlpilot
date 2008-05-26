/************************************************************************/
/* Copyright (C) 2008  Sam Danielson                                    */
/*                                                                      */
/* This file is part of Sqlpilot.				        */
/* 								        */
/* Sqlpilot is free software: you can redistribute it and/or modify     */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or    */
/* (at your option) any later version.				        */
/* 								        */
/* Sqlpilot is distributed in the hope that it will be useful,	        */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/* GNU General Public License for more details.			        */
/* 								        */
/* You should have received a copy of the GNU General Public License    */
/* along with Sqlpilot.  If not, see <http://www.gnu.org/licenses/>.    */
/************************************************************************/


#ifndef _LOGBOOK_H_
#define _LOGBOOK_H_

#include <db/db.h>

// Interface EditView
#define EditView(x) ((EditView *)(x))

struct SelectEdit {
  char **fields;
  GtkWidget *main_widget;
  GtkWidget *view_sw;
  GtkWidget *view_model;
};

/* struct Flights { */
/*   struct SelectEdit; */

typedef struct Logbook Logbook;
struct Logbook
{
  DB *db;
  const char *db_filename;
  GtkWidget *notebook;
  GtkWidget *page_flights;
  GtkWidget *page_roles;
  GtkWidget *page_aircraft;
  GtkWidget *page_types;
  GtkWidget *page_airports;
  /* flights */
  DBStatement *flights_select;
  DBStatement *flights_insert;
  DBStatement *flights_update;
  DBStatement *flights_delete;
  GtkTreeModel *flights_treemodel;
  GtkWidget  *flights_treeview;
  int flights_col_id;
  int flights_col_date;
  int flights_col_aircraft;
  int flights_col_role;
  int flights_col_dep;
  int flights_col_arr;
  int flights_col_aout;
  int flights_col_ain;
  int flights_col_dur;
  int flights_col_night;
  int flights_col_inst;
  int flights_col_siminst;
  int flights_col_hold;
  int flights_col_aprch;
  int flights_col_xc;
  int flights_col_dland;
  int flights_col_nland;
  int flights_col_crew;
  int flights_col_notes;
  int flights_col_fltno;
  int flights_col_sout;
  int flights_col_sin;
  int flights_col_sdur;
  int flights_col_trip;
  GtkTreeSelection *flights_selected_row;
  unsigned int flights_selected_id;
  GtkWidget *flights_id_label;
  GtkWidget *flights_date_entry;
  GtkWidget *flights_aircraft_entry;
  GtkWidget *flights_role_entry;
  GtkWidget *flights_dep_entry;
  GtkWidget *flights_arr_entry;
  GtkWidget *flights_aout_entry;
  GtkWidget *flights_ain_entry;
  GtkWidget *flights_dur_entry;
  GtkWidget *flights_night_entry;
  GtkWidget *flights_instrument_entry;
  GtkWidget *flights_siminst_entry;
  GtkWidget *flights_hold_toggle;
  GtkWidget *flights_aprch_entry;
  GtkWidget *flights_xc_toggle;
  GtkWidget *flights_dland_entry;
  GtkWidget *flights_nland_entry;
  GtkWidget *flights_crew_txtbuf;
  GtkWidget *flights_notes_txtbuf;
  GtkWidget *flights_fltno_entry;
  GtkWidget *flights_sout_entry;
  GtkWidget *flights_sin_entry;
  GtkWidget *flights_sdur_entry;
  GtkWidget *flights_trip_entry;
  /* Roles */
  DBStatement *roles_select;
};

#define LOGBOOK(x) ((Logbook *)(x))
Logbook *logbook_open(const char *filename);
void logbook_close(Logbook *logbook);
/* int editview_column_number(EditView *a */
#endif
