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
#include "summaries.h"
#include "logbook.h"

void on_summaries_refresh_clicked(GtkButton *button, Logbook *logbook)
{
  summaries_refresh(logbook);
  any_toggle_button_set_active(logbook->summaries_view_btn, TRUE);
}

void on_summaries_view_btn_toggled(GtkWidget *button, Logbook *logbook)
{
  gtk_widget_hide(logbook->summaries_export_pane);
  gtk_widget_show(logbook->summaries_view_pane);
}

void on_summaries_export_btn_toggled(GtkWidget *button, Logbook *logbook)
{
  gtk_widget_hide(logbook->summaries_view_pane);
  gtk_widget_show(logbook->summaries_export_pane);
}
