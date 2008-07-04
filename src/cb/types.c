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


#include "sqlpilot.h"

void on_types_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_types_ident(entry);
  if (types_ident_validate(sqlpilot)) {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->types_ident_valid_wart), IMAGE_INVAL ,GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->types_ident_valid_wart), IMAGE_KEY ,GTK_ICON_SIZE_BUTTON);
  }
  edctrl_set_modified(sqlpilot->types_edctrl);
}

void on_types_make_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->types_edctrl);
}

void on_types_model_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->types_edctrl);
}

void on_types_option_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->types_edctrl);
}

void on_types_selection_changed(GtkTreeSelection *selection, Sqlpilot *sqlpilot)
{
  edctrl_selection_changed(sqlpilot->types_edctrl);
  gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->types_ident_valid_wart), IMAGE_KEY ,GTK_ICON_SIZE_BUTTON);
}

void on_types_del_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_del_btn_clicked(sqlpilot->types_edctrl);
}

void on_types_save_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_save_btn_clicked(sqlpilot->types_edctrl);
}

void on_types_new_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_new_btn_clicked(sqlpilot->types_edctrl);
}

void on_types_armdel_btn_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_armdel_btn_toggled(sqlpilot->types_edctrl);
}
