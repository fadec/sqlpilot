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
#include "flights.h"

void on_flights_refresh_clicked(GtkButton *button, Logbook *logbook)
{
  flights_refresh(logbook);
}

void on_flights_view_date_toggled(GtkToggleButton *button, Logbook *logbook)
{
  GtkTreeViewColumn *col;

  col = gtk_tree_view_get_column(GTK_TREE_VIEW(logbook->flights_treeview), 1);
  gtk_tree_view_column_set_visible(col, gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(logbook->flights_view_date)));
}

void on_flights_trip_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

int is_numeric_date_char(char c)
{
  return (c >= '0' && c <= '9') || c == '-';
}
int on_flights_date_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  return FALSE;
}
int on_flights_date_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_date_on_focus_out(entry);
  return FALSE;
}
void on_flights_date_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 10, 0, is_numeric_date_char);
  edctrl_set_modified(logbook->flights_edctrl);
} 

void on_flights_leg_value_changed(GtkWidget *button, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_tripdate_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  return FALSE;
}
int on_flights_tripdate_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_date_on_focus_out(entry);
  return FALSE;
}
void on_flights_tripdate_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 10, 0, is_numeric_date_char);
  edctrl_set_modified(logbook->flights_edctrl);
} 

void on_flights_role_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_roles_ident(entry);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_role_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  flights_refresh_role_utilized(logbook);
  return FALSE;
}

void on_flights_fltno_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_tail_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_aircraft_ident(entry);
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_fleetno_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_aircraft_ident(entry);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_tail_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  DBStatement *stmt;
  edctrl_ignore_modifications(logbook->flights_edctrl, TRUE);
  stmt = logbook->flights_aircraft_fleetno_from_tail;
  db_bind_text(stmt, 1, gtk_entry_get_text(GTK_ENTRY(logbook->flights_tail)));
  if (db_step(stmt) == DB_ROW) {
    gtk_entry_set_text(GTK_ENTRY(logbook->flights_fleetno), (char*)db_column_text(stmt, 0) );
  } else {
    gtk_entry_set_text(GTK_ENTRY(logbook->flights_fleetno), "");
  }
  db_reset(stmt);
  db_clear_bindings(stmt);
  edctrl_ignore_modifications(logbook->flights_edctrl, FALSE);

  flights_refresh_aircraft_utilized(logbook);
  return FALSE;
}

int on_flights_fleetno_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  DBStatement *stmt;
  edctrl_ignore_modifications(logbook->flights_edctrl, TRUE);
  stmt = logbook->flights_aircraft_tail_from_fleetno;
  db_bind_text(stmt, 1, gtk_entry_get_text(GTK_ENTRY(logbook->flights_fleetno)));
  if (db_step(stmt) == DB_ROW) {
    gtk_entry_set_text(GTK_ENTRY(logbook->flights_tail), (char*)db_column_text(stmt, 0) );
  } else {
    gtk_entry_set_text(GTK_ENTRY(logbook->flights_tail), "");
  }

  db_reset(stmt);
  db_clear_bindings(stmt);
  edctrl_ignore_modifications(logbook->flights_edctrl, FALSE);

  flights_refresh_aircraft_utilized(logbook);
  return FALSE;
}

void on_flights_depicao_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_airports_icao(entry);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_depicao_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  flights_handle_icao_update(logbook, entry, GTK_ENTRY(logbook->flights_depiata));
  
  if (!any_toggle_button_get_active(logbook->flights_utc)) {
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_sout),
			   GTK_ENTRY(logbook->flights_sout),
			   GTK_ENTRY(logbook->flights_sin),
			   GTK_ENTRY(logbook->flights_sdur));
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_aout),
			   GTK_ENTRY(logbook->flights_aout),
			   GTK_ENTRY(logbook->flights_ain),
			   GTK_ENTRY(logbook->flights_dur));
  }
  flights_refresh_dep_utilized(logbook);
  return FALSE;
}

void on_flights_depiata_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_airports_iata(entry);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_depiata_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  flights_handle_iata_update(logbook, entry, GTK_ENTRY(logbook->flights_depicao));

  if (!any_toggle_button_get_active(logbook->flights_utc)) {
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_sout),
			   GTK_ENTRY(logbook->flights_sout),
			   GTK_ENTRY(logbook->flights_sin),
			   GTK_ENTRY(logbook->flights_sdur));
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_aout),
			   GTK_ENTRY(logbook->flights_aout),
			   GTK_ENTRY(logbook->flights_ain),
			   GTK_ENTRY(logbook->flights_dur));
  }
  flights_refresh_dep_utilized(logbook);
  return FALSE;
}

void on_flights_arricao_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_airports_icao(entry);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_arricao_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{

  flights_handle_icao_update(logbook, entry, GTK_ENTRY(logbook->flights_arriata));

  if (!any_toggle_button_get_active(logbook->flights_utc)) {
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_sin),
			   GTK_ENTRY(logbook->flights_sout),
			   GTK_ENTRY(logbook->flights_sin),
			   GTK_ENTRY(logbook->flights_sdur));
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_ain),
			   GTK_ENTRY(logbook->flights_aout),
			   GTK_ENTRY(logbook->flights_ain),
			   GTK_ENTRY(logbook->flights_dur));
  }
  flights_refresh_arr_utilized(logbook);
  return FALSE;
}

void on_flights_arriata_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_airports_icao(entry);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_arriata_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{

  flights_handle_iata_update(logbook, entry, GTK_ENTRY(logbook->flights_arricao));
  
  if (!any_toggle_button_get_active(logbook->flights_utc)) {
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_sin),
			   GTK_ENTRY(logbook->flights_sout),
			   GTK_ENTRY(logbook->flights_sin),
			   GTK_ENTRY(logbook->flights_sdur));
    reconcile_time_entries(logbook,
			   GTK_ENTRY(logbook->flights_ain),
			   GTK_ENTRY(logbook->flights_aout),
			   GTK_ENTRY(logbook->flights_ain),
			   GTK_ENTRY(logbook->flights_dur));
  }
  flights_refresh_arr_utilized(logbook);
  return FALSE;
}

int on_flights_sout_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  entry_format_time_on_focus_in(entry);
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
int on_flights_sout_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  const gchar *date;
  char local_tz[BUF_TZ], *to_tz;

  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  date = gtk_entry_get_text(GTK_ENTRY(logbook->flights_date));

  flights_lookup_dep_tz(logbook, local_tz);

  if (any_toggle_button_get_active(logbook->flights_utc)) {
    to_tz = "UTC";
  } else {
    to_tz = local_tz;
  }

  entry_format_time_of_day(entry, local_tz, to_tz, date);
  reconcile_time_entries(logbook,
			 GTK_ENTRY(logbook->flights_sout),
			 GTK_ENTRY(logbook->flights_sout),
			 GTK_ENTRY(logbook->flights_sin),
			 GTK_ENTRY(logbook->flights_sdur));
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
void on_flights_sout_changed(GtkEntry *entry, Logbook *logbook)
{
  /* Clamp to six chars, eg 12:34z */
  entry_clamp_text(entry, 6, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_sin_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  entry_format_time_on_focus_in(entry);
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
int on_flights_sin_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  const gchar *date;
  char local_tz[BUF_TZ], *to_tz;

  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  date = gtk_entry_get_text(GTK_ENTRY(logbook->flights_date));

  flights_lookup_arr_tz(logbook, local_tz);

  if (any_toggle_button_get_active(logbook->flights_utc)) {
    to_tz = "UTC";
  } else {
    to_tz = local_tz;
  }

  entry_format_time_of_day(entry, local_tz, to_tz, date);
  reconcile_time_entries(logbook,
			 GTK_ENTRY(logbook->flights_sin),
			 GTK_ENTRY(logbook->flights_sout),
			 GTK_ENTRY(logbook->flights_sin),
			 GTK_ENTRY(logbook->flights_sdur));
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
void on_flights_sin_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 6, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_sdur_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1); 
  entry_format_time_on_focus_in(entry);
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
int on_flights_sdur_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  entry_format_time_on_focus_out(entry, '+');
  reconcile_time_entries(logbook,
			 GTK_ENTRY(logbook->flights_sdur),
			 GTK_ENTRY(logbook->flights_sout),
			 GTK_ENTRY(logbook->flights_sin),
			 GTK_ENTRY(logbook->flights_sdur));
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}

void on_flights_sdur_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_aout_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  entry_format_time_on_focus_in(entry);
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
int on_flights_aout_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  const gchar *date;
  char local_tz[BUF_TZ], *to_tz;

  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  date = gtk_entry_get_text(GTK_ENTRY(logbook->flights_date));

  flights_lookup_dep_tz(logbook, local_tz);

  if (any_toggle_button_get_active(logbook->flights_utc)) {
    to_tz = "UTC";
  } else {
    to_tz = local_tz;
  }

  entry_format_time_of_day(entry, local_tz, to_tz, date);

  reconcile_time_entries(logbook,
			 GTK_ENTRY(logbook->flights_aout),
			 GTK_ENTRY(logbook->flights_aout),
			 GTK_ENTRY(logbook->flights_ain),
			 GTK_ENTRY(logbook->flights_dur));
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
void on_flights_aout_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 6, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_ain_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  entry_format_time_on_focus_in(entry);
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
int on_flights_ain_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  const gchar *date;
  char local_tz[BUF_TZ], *to_tz;

  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  date = gtk_entry_get_text(GTK_ENTRY(logbook->flights_date));

  flights_lookup_arr_tz(logbook, local_tz);

  if (any_toggle_button_get_active(logbook->flights_utc)) {
    to_tz = "UTC";
  } else {
    to_tz = local_tz;
  }

  entry_format_time_of_day(entry, local_tz, to_tz, date);

  reconcile_time_entries(logbook,
			 GTK_ENTRY(logbook->flights_ain),
			 GTK_ENTRY(logbook->flights_aout),
			 GTK_ENTRY(logbook->flights_ain),
			 GTK_ENTRY(logbook->flights_dur));
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
void on_flights_ain_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_dur_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  entry_format_time_on_focus_in(entry);
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
int on_flights_dur_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  edctrl_ignore_modifications(logbook->flights_edctrl, 1);
  entry_format_time_on_focus_out(entry, '+');
  reconcile_time_entries(logbook,
			 GTK_ENTRY(logbook->flights_dur),
			 GTK_ENTRY(logbook->flights_aout),
			 GTK_ENTRY(logbook->flights_ain),
			 GTK_ENTRY(logbook->flights_dur));
  edctrl_ignore_modifications(logbook->flights_edctrl, 0);
  return FALSE;
}
void on_flights_dur_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 6, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_inst_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_inst_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_inst_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_siminst_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_siminst_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_siminst_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

int on_flights_night_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_night_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_night_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_selection_changed(GtkTreeSelection *selection, Logbook *logbook)
{
  edctrl_selection_changed(logbook->flights_edctrl);
}

void on_flights_utc_toggled(GtkWidget *button, Logbook *logb)
{
  char deptz[BUF_TZ], arrtz[BUF_TZ];
  char *deptz1, *deptz2, *arrtz1, *arrtz2;
  const char *date, *aout, *ain, *sout, *sin;
  char strdate[BUF_DATE], strtime[BUF_TIME];

  date = gtk_entry_get_text(GTK_ENTRY(logb->flights_date));
  aout = gtk_entry_get_text(GTK_ENTRY(logb->flights_aout));
  ain  = gtk_entry_get_text(GTK_ENTRY(logb->flights_ain));
  sout = gtk_entry_get_text(GTK_ENTRY(logb->flights_sout));
  sin  = gtk_entry_get_text(GTK_ENTRY(logb->flights_sin));

  flights_lookup_dep_tz(logb, deptz);
  flights_lookup_arr_tz(logb, arrtz);

  if (any_toggle_button_get_active(button)) {
    /* to UTC */
    deptz1 = deptz;
    arrtz1 = arrtz;
    deptz2 = "UTC";
    arrtz2 = "UTC";
  } else {
    /* to Local */
    deptz1 = "UTC";
    arrtz1 = "UTC";
    deptz2 = deptz;
    arrtz2 = arrtz;
  }

  /* Do the switch */
  edctrl_ignore_modifications(logb->flights_edctrl, TRUE);
  if (strlen(aout)) {
    move_time(deptz1, deptz2, date, aout, strdate, strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_aout), strtime);
  } 
  if (strlen(sout)) {
    move_time(deptz1, deptz2, date, sout, strdate, strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sout), strtime);
  }
  if (strlen(ain)) {
    move_time(arrtz1, arrtz2, date, ain, strdate, strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_ain), strtime);
  }
  if (strlen(sin)) {
    move_time(arrtz1, arrtz2, date, sin, strdate, strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sin), strtime);
  }
/*   gtk_label_set_text(GTK_LABEL(logb->flights_utc_lbl), */
  /* 		     any_toggle_button_get_active(button) ? "UTC" : "Local"); */
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(logb->flights_utc),
			    any_toggle_button_get_active(button) ? "UTC" : "Local");
  edctrl_ignore_modifications(logb->flights_edctrl, FALSE);
}

void on_flights_icao_toggle_toggled(GtkWidget *button, Logbook *logbook)
{
  if (any_toggle_button_get_active(logbook->flights_icao_toggle)) {
    gtk_widget_hide(logbook->flights_depiata);
    gtk_widget_hide(logbook->flights_arriata);
    gtk_widget_show(logbook->flights_depicao);
    gtk_widget_show(logbook->flights_arricao);
  } else {
    gtk_widget_hide(logbook->flights_depicao);
    gtk_widget_hide(logbook->flights_arricao);
    gtk_widget_show(logbook->flights_depiata);
    gtk_widget_show(logbook->flights_arriata);
  }
/*   gtk_label_set_text(GTK_LABEL(logbook->flights_icao_toggle_lbl), */
/* 		     any_toggle_button_get_active(GTK_WIDET(button)) ? "ICAO" : "IATA"); */
  gtk_tool_button_set_label(GTK_TOOL_BUTTON(logbook->flights_icao_toggle),
			    any_toggle_button_get_active(button) ? "ICAO" : "IATA");

}

void on_flights_fleetno_toggle_toggled(GtkToggleButton *button, Logbook *logbook)
{
  if (any_toggle_button_get_active(logbook->flights_fleetno_toggle)) {
    gtk_widget_hide(logbook->flights_tail);
    gtk_widget_show(logbook->flights_fleetno);
/*     gtk_label_set_text(GTK_LABEL(logbook->flights_fleetno_toggle_lbl), "FleetNo"); */
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(logbook->flights_fleetno_toggle), "Fleet");
  } else {
    gtk_widget_hide(logbook->flights_fleetno);
    gtk_widget_show(logbook->flights_tail);
/*     gtk_label_set_text(GTK_LABEL(logbook->flights_fleetno_toggle_lbl), "Tail"); */
    gtk_tool_button_set_label(GTK_TOOL_BUTTON(logbook->flights_fleetno_toggle), "Tail");

  }
}

void on_flights_crew_changed(GtkTextBuffer *tb, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_notes_changed(GtkTextBuffer *tb, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_aprch_changed(GtkWidget *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_xc_toggled(GtkWidget *button, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_hold_toggled(GtkWidget *button, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_dland_value_changed(GtkWidget *button, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_nland_value_changed(GtkWidget *button, Logbook *logbook)
{
  edctrl_set_modified(logbook->flights_edctrl);
}

void on_flights_armdel_btn_toggled(GtkToggleButton *button, Logbook *logbook)
{
  edctrl_armdel_btn_toggled(logbook->flights_edctrl);
}

void on_flights_del_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_del_btn_clicked(logbook->flights_edctrl);
}

void on_flights_new_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_new_btn_clicked(logbook->flights_edctrl);
}

void on_flights_save_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_save_btn_clicked(logbook->flights_edctrl);
}
