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
#include "aircraft.h"
#include "logbook.h"

void on_aircraft_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_aircraft_ident(entry);

  if (aircraft_ident_validate(sqlpilot)) {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->aircraft_ident_valid_wart), IMAGE_INVAL, GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->aircraft_ident_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
  }
  edctrl_set_modified(sqlpilot->aircraft_edctrl);
}

void on_aircraft_fleetno_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_aircraft_fleetno(entry);
  if (aircraft_fleetno_validate(sqlpilot)) {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->aircraft_fleetno_valid_wart), IMAGE_INVAL, GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->aircraft_fleetno_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
  }
  edctrl_set_modified(sqlpilot->aircraft_edctrl);
}

void on_aircraft_type_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_types_ident(entry);
  edctrl_set_modified(sqlpilot->aircraft_edctrl);
}

void on_aircraft_selection_changed(GtkTreeSelection *selection, Sqlpilot *sqlpilot)
{
  edctrl_selection_changed(sqlpilot->aircraft_edctrl);
  gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->aircraft_ident_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
  gtk_image_set_from_stock(GTK_IMAGE(sqlpilot->aircraft_fleetno_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
}

void on_aircraft_notes_changed(GtkTextBuffer *tb, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->aircraft_edctrl);
}

void on_aircraft_del_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_del_btn_clicked(sqlpilot->aircraft_edctrl);
}

void on_aircraft_save_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_save_btn_clicked(sqlpilot->aircraft_edctrl);
}

void on_aircraft_new_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_new_btn_clicked(sqlpilot->aircraft_edctrl);
}

void on_aircraft_armdel_btn_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_armdel_btn_toggled(sqlpilot->aircraft_edctrl);
}
