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
#include "models.h"
#include "logbook.h"

void on_models_ident_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_models_ident(entry);
  if (models_ident_validate(logbook)) {
    gtk_image_set_from_stock(GTK_IMAGE(logbook->models_ident_valid_wart), IMAGE_INVAL ,GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_stock(GTK_IMAGE(logbook->models_ident_valid_wart), IMAGE_KEY ,GTK_ICON_SIZE_BUTTON);
  }
  edctrl_set_modified(logbook->models_edctrl);
}

void on_models_make_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->models_edctrl);
}

void on_models_type_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->models_edctrl);
}

void on_models_option_toggled(GtkButton *button, Logbook *logbook)
{
  edctrl_set_modified(logbook->models_edctrl);
}

void on_models_selection_changed(GtkTreeSelection *selection, Logbook *logbook)
{
  edctrl_selection_changed(logbook->models_edctrl);
  gtk_image_set_from_stock(GTK_IMAGE(logbook->models_ident_valid_wart), IMAGE_KEY ,GTK_ICON_SIZE_BUTTON);
}

void on_models_del_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_del_btn_clicked(logbook->models_edctrl);
}

void on_models_save_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_save_btn_clicked(logbook->models_edctrl);
}

void on_models_new_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_new_btn_clicked(logbook->models_edctrl);
}

void on_models_armdel_btn_toggled(GtkButton *button, Logbook *logbook)
{
  edctrl_armdel_btn_toggled(logbook->models_edctrl);
}
