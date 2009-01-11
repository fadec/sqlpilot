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
  ANY_TOGGLE_SET_ACTIVE(logbook->reports_view_btn, TRUE);
}

void on_reports_view_btn_toggled(GtkWidget *button, Logbook *logbook)
{
  gtk_widget_hide(logbook->reports_export_pane);
  gtk_widget_show(logbook->reports_view_pane);
}

void on_reports_export_btn_toggled(GtkWidget *button, Logbook *logbook)
{
  gtk_widget_hide(logbook->reports_view_pane);
  gtk_widget_show(logbook->reports_export_pane);
}


static void reports_set_modified(Logbook *logbook)
{
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->reports_armdel_btn), FALSE);
    gtk_widget_set_sensitive(logbook->reports_armdel_btn, FALSE);
    gtk_widget_set_sensitive(logbook->reports_del_btn, FALSE);
    gtk_widget_set_sensitive(logbook->reports_save_btn, strlen(gtk_entry_get_text(GTK_ENTRY(logbook->reports_title))));
}

void on_reports_title_changed(GtkWidget *entry, Logbook *logbook)
{
  if (GTK_WIDGET_HAS_FOCUS(entry)) {
    reports_set_modified(logbook);
    gtk_expander_set_expanded(GTK_EXPANDER(logbook->reports_sql_expander), TRUE);
  } else {
    db_bind_text(logbook->reports_sql_by_title, 1, gtk_entry_get_text(GTK_ENTRY(logbook->reports_title)));
    db_step(logbook->reports_sql_by_title);
    fprintf(stderr, "Error's from here ...\n");
    text_view_set_text(GTK_TEXT_VIEW(logbook->reports_sql_text), (char*)db_column_text(logbook->reports_sql_by_title, 0));
    fprintf(stderr, "... to here during delete.\n");
    db_reset(logbook->reports_sql_by_title);
    db_clear_bindings(logbook->reports_sql_by_title);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->reports_armdel_btn), FALSE);
    gtk_widget_set_sensitive(logbook->reports_armdel_btn, TRUE);
    gtk_widget_set_sensitive(logbook->reports_del_btn, FALSE);
    gtk_widget_set_sensitive(logbook->reports_save_btn, FALSE);
  }
}

void on_reports_armdel_btn_toggled(GtkToggleButton *button, Logbook *logbook)
{
  gtk_widget_set_sensitive(logbook->reports_del_btn, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->reports_armdel_btn)));
}

void on_reports_del_btn_clicked(GtkButton *button, Logbook *logbook)
{
  reports_delete(logbook);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->reports_armdel_btn), FALSE);
  gtk_widget_set_sensitive(logbook->reports_armdel_btn, FALSE);
  gtk_widget_set_sensitive(logbook->reports_del_btn, FALSE);
  gtk_widget_set_sensitive(logbook->reports_save_btn, FALSE);
}

void on_reports_save_btn_clicked(GtkButton *button, Logbook *logbook)
{
  reports_save(logbook);
}

void on_reports_sql_text_changed(GtkTextBuffer *tb, Logbook *logbook)
{
  reports_set_modified(logbook);
}
