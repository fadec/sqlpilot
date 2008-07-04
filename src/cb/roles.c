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

void on_roles_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_roles_ident(entry);
  if (roles_ident_validate(sqlpilot)) {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->roles_ident_valid_wart), IMAGE_INVAL, GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->roles_ident_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
  }
  edctrl_set_modified(sqlpilot->roles_edctrl);
}

void on_roles_del_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_del_btn_clicked(sqlpilot->roles_edctrl);
}

void on_roles_save_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_save_btn_clicked(sqlpilot->roles_edctrl);
}

void on_roles_new_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_new_btn_clicked(sqlpilot->roles_edctrl);
}

void on_roles_name_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_pic_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_sic_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_fe_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_solo_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_dual_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_instruct_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_total_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}

void on_roles_armdel_btn_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_armdel_btn_toggled(sqlpilot->roles_edctrl);
}

void on_roles_selection_changed(GtkTreeSelection *selection, Sqlpilot *sqlpilot)
{
  edctrl_selection_changed(sqlpilot->roles_edctrl);
  gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->roles_ident_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
}
