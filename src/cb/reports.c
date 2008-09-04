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
#include "reports.h"

void on_reports_refresh_clicked(GtkButton *button, Logbook *logbook)
{
  reports_refresh(logbook);
}

void on_reports_sql_changed(GtkTextBuffer *tb, Logbook *logbook)
{

}

void on_reports_title_changed(GtkWidget *entry, Logbook *logbook)
{

}

void on_reports_armdel_btn_toggled(GtkToggleButton *button, Logbook *logbook)
{
  gtk_widget_set_sensitive(logbook->reports_del_btn, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->reports_armdel_btn)));
}

void on_reports_del_btn_clicked(GtkButton *button, Logbook *logbook)
{

}

void on_reports_save_btn_clicked(GtkButton *button, Logbook *logbook)
{

}

