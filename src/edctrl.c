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

#include "edctrl.h"
#include "sqlpilot.h"

static void edctrl_show_record(Edctrl *ec)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  char show[BUF_ED_TODEL - 2] = ""; /* -2 to save room for '* ' prefix */
  char labeltext[BUF_ED_TODEL] = "";
  

  if (gtk_tree_selection_get_selected (ec->selection, &model, &iter)) {
    ec->selection_show(ec->selection, show, sizeof(show));
    snprintf(labeltext, sizeof(labeltext), "%s%s", (ec->edstate == EDSTATE_MODIFIED) ? "* " : "", show);
    gtk_label_set_text(GTK_LABEL(ec->todel_lbl), labeltext);
  } else {
    gtk_label_set_text(GTK_LABEL(ec->todel_lbl), (ec->edstate == EDSTATE_MODIFIED) ? "* New Record" : "New Record" );
  }
}

void edctrl_set_empty(Edctrl *ec)
{
  if (ec->edstate == EDSTATE_EMPTY) return;
  ec->edstate = EDSTATE_EMPTY;
  gtk_widget_set_sensitive(ec->new_btn, 0);
  gtk_widget_set_sensitive(ec->save_btn, 0);
  gtk_widget_set_sensitive(ec->armdel_btn, 0);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ec->armdel_btn), 0);
  gtk_widget_set_sensitive(ec->del_btn, 0);
  edctrl_show_record(ec);
}  

void edctrl_set_selected(Edctrl *ec)
{
  if (ec->edstate == EDSTATE_SELECTED) return;
  ec->edstate = EDSTATE_SELECTED;
  gtk_widget_set_sensitive(ec->new_btn, 1);
  gtk_widget_set_sensitive(ec->save_btn, 0);
  gtk_widget_set_sensitive(ec->armdel_btn, ec->can_delete(ec->selection));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(ec->armdel_btn), 0);
  gtk_widget_set_sensitive(ec->del_btn, 0);
  edctrl_show_record(ec);
}

void edctrl_ignore_modifications(Edctrl *ec, int bool)
{
  ec->ignore_modifications = bool;
}

void edctrl_set_modified(Edctrl *ec)
{
  if (ec->ignore_modifications) return;
  if (ec->validator && ec->validator(ec->validator_data)) {
    gtk_widget_set_sensitive(GTK_WIDGET(ec->save_btn), 0);
  } else {
    gtk_widget_set_sensitive(GTK_WIDGET(ec->save_btn), 1);
  }
  if (ec->edstate == EDSTATE_MODIFIED) return;
  gtk_widget_set_sensitive(GTK_WIDGET(ec->armdel_btn), 0);
  gtk_widget_set_sensitive(GTK_WIDGET(ec->del_btn), 0);
  gtk_widget_set_sensitive(GTK_WIDGET(ec->new_btn), 0);
  edctrl_show_record(ec);
  ec->edstate = EDSTATE_MODIFIED;
}

void edctrl_set_deletearmed(Edctrl *ec)
{
  if (ec->edstate == EDSTATE_DELETEARMED) return;
  ec->edstate = EDSTATE_DELETEARMED;
  gtk_widget_set_sensitive(ec->del_btn, 1);
}

void edctrl_selection_changed(Edctrl *ec)
{
  GtkTreeIter iter;
  GtkTreeModel *model;

  ec->load_selection(ec->load_selection_data);

  if (gtk_tree_selection_get_selected (ec->selection, &model, &iter)) {
    edctrl_set_selected(ec);
  } else {
    edctrl_set_empty(ec);
  }
}

void edctrl_armdel_btn_toggled(Edctrl *ec)
{
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(ec->armdel_btn))) {
    edctrl_set_deletearmed(ec);
  } else {
    edctrl_set_selected(ec);
  }
}

void edctrl_register_load_selection(Edctrl *ec, void func(Sqlpilot *), Sqlpilot *data)
{
  ec->load_selection = func;
  ec->load_selection_data = data;
}

void edctrl_register_after_change(Edctrl *ec, void func(Sqlpilot *), Sqlpilot *data)
{
  ec->after_change = func;
  ec->after_change_data = data;
}

void edctrl_register_save(Edctrl *ec, DBint64 func(const char*, Sqlpilot *), Sqlpilot *data)
{
  ec->save = func;
  ec->save_data = data;
}

void edctrl_register_validator(Edctrl *ec, int func(Sqlpilot *), Sqlpilot *data)
{
  ec->validator = func;
  ec->validator_data = data;
}

void edctrl_del_btn_clicked(Edctrl *ec)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *id=NULL;
  
  if (gtk_tree_selection_get_selected(ec->selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, COL_ID, &id, -1);
    db_bind_text(ec->delete_stmt, 1, id);
    db_stp_res_clr(ec->delete_stmt);
    gtk_list_store_remove(GTK_LIST_STORE(model), &iter);
    g_free(id);
    ec->after_change(ec->after_change_data);
  }

  edctrl_set_empty(ec);
}
  
void edctrl_new_btn_clicked(Edctrl *ec)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  
  if (gtk_tree_selection_get_selected(ec->selection, &model, &iter)) {
    gtk_tree_selection_unselect_iter(ec->selection, &iter);
  }

  edctrl_set_empty(ec);
}

void edctrl_save_btn_clicked(Edctrl *ec)
{
  DBStatement *stmt;
  GtkTreeIter iter;
  GtkTreeModel *model;
  DBint64 inserted_id;
  gchar *id=NULL;

  stmt = ec->select_by_id_stmt;

  if (gtk_tree_selection_get_selected(ec->selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, COL_ID, &id, -1);

    ec->save(id, ec->save_data);
    db_bind_text(stmt, 1, id);
  } else {
    inserted_id = ec->save(NULL, ec->save_data);
    db_bind_int64(stmt, 1, inserted_id);

    gtk_list_store_insert(GTK_LIST_STORE(model), &iter, 0);
  }
  store_update_row(GTK_LIST_STORE(model), &iter, stmt);
  gtk_tree_selection_select_iter(ec->selection, &iter);

  db_reset(stmt);
  db_clear_bindings(stmt);

  g_free(id);

  ec->after_change(ec->after_change_data);

  edctrl_set_selected(ec);
}
