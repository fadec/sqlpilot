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

#define UI_XML_FILE  "data/ui/interface.xml"

#include "config.h"

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "db/db.h"
#include "util.h"
#include "store.h"

/* Editing states for view */
typedef enum {
  EDSTATE_EMPTY,
  EDSTATE_SELECTED,
  EDSTATE_MODIFIED,
  EDSTATE_INVALID,
  EDSTATE_DELETEARMED
} Edstate;

typedef struct Sqlpilot Sqlpilot;

typedef struct Edctrl Edctrl;
struct Edctrl {
  Edstate edstate;
  GtkWidget *new_btn;
  GtkWidget *save_btn;
  GtkWidget *armdel_btn;
  GtkWidget *todel_lbl;
  GtkWidget *del_btn;
  GtkTreeSelection *selection;
  DBStatement *delete_stmt;
  DBStatement *select_by_id_stmt;
  void (*load_selection)(Sqlpilot *);
  void *load_selection_data;
  int (*selection_show)(GtkTreeSelection *, char *, size_t);
  int (*can_delete)(GtkTreeSelection *);
  void (*after_change)(Sqlpilot *);
  void *after_change_data;
  DBint64 (*save)(const char *id, Sqlpilot *);
  void *save_data;
};

#define BUF_ED_TODEL 32

void edctrl_set_empty(Edctrl *ec);
void edctrl_set_selected(Edctrl *ec);
void edctrl_set_modified(Edctrl *ec);
void edctrl_set_invalid(Edctrl *ec);
void edctrl_set_deletearmed(Edctrl *ec);

void edctrl_selection_changed(Edctrl *ec);
void edctrl_new_btn_clicked(Edctrl *ec);
void edctrl_save_btn_clicked(Edctrl *ec);
void edctrl_del_btn_clicked(Edctrl *ec);

void edctrl_register_load_selection(Edctrl *ec, void func(Sqlpilot *), Sqlpilot *);
void edctrl_register_save(Edctrl *ec, DBint64 func(const char *, Sqlpilot *), Sqlpilot *);
void edctrl_register_after_change(Edctrl *ec, void (Sqlpilot *), Sqlpilot *);

void edctrl_armdel_btn_toggled(Edctrl *ec);


//#include "edctrl.h"

#define EXIT_SUCESS 0
#define EXIT_BARF  1
#define SQLPILOT(x) ((Sqlpilot*)x)

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

struct Sqlpilot {
  DB *db;
  GtkWidget *window;

  int flights_stale;	   /* Treemodel contains stale data as a result of updates to db elsewhere ... the model for the view is stale; it has nothing to do with the db */
  DBStatement *flights_select_all; /* Select for tree model */
  DBStatement *flights_select_by_id; /* Select for update a single treemodel row */
  DBStatement *flights_insert;
  DBStatement *flights_update;
  DBStatement *flights_delete;
  GtkWidget *flights_sw;	/* Scrollable Window - contains the treeview */
  GtkTreeModel *flights_treemodel;
  GtkTreeSelection *flights_selection;
  GtkWidget *flights_treeview;
  GtkWidget *flights_utc;
  GtkWidget *flights_utc_lbl;
  GtkWidget *flights_aircraft;
  GtkWidget *flights_date;
  GtkWidget *flights_leg;
  GtkWidget *flights_role;
  GtkWidget *flights_dep;
  GtkWidget *flights_arr;
  GtkWidget *flights_aout;
  GtkWidget *flights_ain;
  GtkWidget *flights_dur;
  GtkWidget *flights_night;
  GtkWidget *flights_inst;
  GtkWidget *flights_siminst;
  GtkWidget *flights_hold;
  GtkWidget *flights_aprch;
  GtkWidget *flights_xc;
  GtkWidget *flights_dland;
  GtkWidget *flights_nland;
  GtkWidget *flights_crew;
  GtkWidget *flights_notes;
  GtkWidget *flights_fltno;
  GtkWidget *flights_sout;
  GtkWidget *flights_sin;
  GtkWidget *flights_sdur;
  GtkWidget *flights_trip;
  GtkWidget *flights_tripdate;
  GtkWidget *flights_new_btn;
  GtkWidget *flights_save_btn;
  GtkWidget *flights_armdel_btn;
  GtkWidget *flights_todel_lbl;
  GtkWidget *flights_del_btn;
  Edctrl _flights_edctrl;
  Edctrl *flights_edctrl;

  int roles_stale;
  DBStatement *roles_select_all;
  DBStatement *roles_select_by_id;
  DBStatement *roles_insert;
  DBStatement *roles_update;
  DBStatement *roles_delete;
  GtkWidget *roles_sw;
  GtkTreeModel *roles_treemodel;
  GtkTreeSelection *roles_selection;
  GtkWidget *roles_treeview;
  GtkWidget *roles_ident;
  GtkWidget *roles_name;
  GtkWidget *roles_pic;
  GtkWidget *roles_sic;
  GtkWidget *roles_fe;
  GtkWidget *roles_solo;
  GtkWidget *roles_dual;
  GtkWidget *roles_instruct;
  GtkWidget *roles_total;
  GtkWidget *roles_new_btn;
  GtkWidget *roles_save_btn;
  GtkWidget *roles_armdel_btn;
  GtkWidget *roles_todel_lbl;
  GtkWidget *roles_del_btn;
  Edctrl _roles_edctrl;
  Edctrl *roles_edctrl;

  int aircraft_stale;
  DBStatement *aircraft_select_all;
  DBStatement *aircraft_select_by_id;
  DBStatement *aircraft_insert;
  DBStatement *aircraft_update;
  DBStatement *aircraft_delete;
  GtkWidget *aircraft_sw;
  GtkTreeModel *aircraft_treemodel;
  GtkTreeSelection *aircraft_selection;
  GtkWidget *aircraft_treeview;
  GtkWidget *aircraft_ident;
  GtkWidget *aircraft_type;
  GtkWidget *aircraft_fleetno;
  GtkWidget *aircraft_notes;
  GtkWidget *aircraft_new_btn;
  GtkWidget *aircraft_save_btn;
  GtkWidget *aircraft_armdel_btn;
  GtkWidget *aircraft_todel_lbl;
  GtkWidget *aircraft_del_btn;
  Edctrl _aircraft_edctrl;
  Edctrl *aircraft_edctrl;

  int types_stale;
  DBStatement *types_select_all;
  DBStatement *types_select_by_id;
  DBStatement *types_insert;
  DBStatement *types_update;
  DBStatement *types_delete;
  GtkWidget *types_sw;
  GtkTreeModel *types_treemodel;
  GtkTreeSelection *types_selection;
  GtkWidget *types_treeview;
  GtkWidget *types_ident;
  GtkWidget *types_make;
  GtkWidget *types_model;
  GtkWidget *types_airplane;
  GtkWidget *types_rotorcraft;
  GtkWidget *types_glider;
  GtkWidget *types_lta;
  GtkWidget *types_poweredlift;
  GtkWidget *types_ppc;
  GtkWidget *types_weightshift;
  GtkWidget *types_heli;
  GtkWidget *types_gyro;
  GtkWidget *types_airship;
  GtkWidget *types_balloon;
  GtkWidget *types_single;
  GtkWidget *types_multi;
  GtkWidget *types_land;
  GtkWidget *types_sea;
  GtkWidget *types_turbine;
  GtkWidget *types_jet;
  GtkWidget *types_highperf;
  GtkWidget *types_retract;
  GtkWidget *types_complex;
  GtkWidget *types_pressurized;
  GtkWidget *types_large;
  GtkWidget *types_sport;
  GtkWidget *types_ultralight;
  GtkWidget *types_footlaunch;
  GtkWidget *types_sim;
  GtkWidget *types_ftd;
  GtkWidget *types_total;
  GtkWidget *types_new_btn;
  GtkWidget *types_save_btn;
  GtkWidget *types_armdel_btn;
  GtkWidget *types_todel_lbl;
  GtkWidget *types_del_btn;
  Edctrl _types_edctrl;
  Edctrl *types_edctrl;

  int airports_stale;
  DBStatement *airports_select_all;
  DBStatement *airports_select_by_id;
  DBStatement *airports_insert;
  DBStatement *airports_update;
  DBStatement *airports_delete;
  GtkWidget *airports_sw;
  GtkTreeModel *airports_treemodel;
  GtkTreeSelection *airports_selection;
  GtkWidget *airports_treeview;
  GtkWidget *airports_ident;
  GtkWidget *airports_icao;
  GtkWidget *airports_name;
  GtkWidget *airports_city;
  GtkWidget *airports_province;
  GtkWidget *airports_country;
  GtkWidget *airports_elev;
  GtkWidget *airports_lat;
  GtkWidget *airports_lon;
  GtkWidget *airports_tzone;
  GtkWidget *airports_notes;
  GtkWidget *airports_new_btn;
  GtkWidget *airports_save_btn;
  GtkWidget *airports_armdel_btn;
  GtkWidget *airports_todel_lbl;
  GtkWidget *airports_del_btn;
  Edctrl _airports_edctrl;
  Edctrl *airports_edctrl;
};

#include "flights.h"
#include "roles.h"
#include "aircraft.h"
#include "types.h"
#include "airports.h"

Sqlpilot *sqlpilot_new(const char *filename);
void sqlpilot_finalize(Sqlpilot *);

void barf(const char *message);

#endif
