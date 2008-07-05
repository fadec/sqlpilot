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

#ifndef EDCTRL_H
#define EDCTRL_H

#include <gtk/gtk.h>
#include "sqlpilot.h"
#include "db/db.h"

#define BUF_ED_TODEL 32

/* Editing states for view */
typedef enum {
  EDSTATE_EMPTY,
  EDSTATE_SELECTED,
  EDSTATE_MODIFIED,
  EDSTATE_INVALID,
  EDSTATE_DELETEARMED
} Edstate;

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
  void (*load_selection)(Logbook *);
  void *load_selection_data;
  int (*selection_show)(GtkTreeSelection *, char *, size_t);
  int (*can_delete)(GtkTreeSelection *);
  void (*after_change)(Logbook *);
  void *after_change_data;
  int (*validator)(Logbook *);
  void *validator_data;
  DBint64 (*save)(const char *id, Logbook *);
  void *save_data;
  int ignore_modifications;
};

void edctrl_set_empty(Edctrl *ec);
void edctrl_set_selected(Edctrl *ec);
void edctrl_set_modified(Edctrl *ec);
void edctrl_set_deletearmed(Edctrl *ec);

void edctrl_ignore_modifications(Edctrl *ec, int bool);
void edctrl_selection_changed(Edctrl *ec);
void edctrl_new_btn_clicked(Edctrl *ec);
void edctrl_save_btn_clicked(Edctrl *ec);
void edctrl_del_btn_clicked(Edctrl *ec);

void edctrl_register_load_selection(Edctrl *ec, void func(Logbook *), Logbook *);
void edctrl_register_save(Edctrl *ec, DBint64 func(const char *, Logbook *), Logbook *);
void edctrl_register_after_change(Edctrl *ec, void (Logbook *), Logbook *);
void edctrl_register_validator(Edctrl *ec, int func(Logbook *), Logbook *data);

void edctrl_armdel_btn_toggled(Edctrl *ec);

#endif
