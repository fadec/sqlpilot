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
#include "airports.h"
#include "logbook.h"

void on_airports_iata_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_airports_iata(entry);

  if (airports_iata_validate(logbook)) {
    gtk_image_set_from_stock(GTK_IMAGE(logbook->airports_iata_valid_wart), IMAGE_INVAL, GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_stock(GTK_IMAGE(logbook->airports_iata_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
  }
  edctrl_set_modified(logbook->airports_edctrl);
}

void on_airports_icao_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_airports_icao(entry);

  if (airports_icao_validate(logbook)) {
    gtk_image_set_from_stock(GTK_IMAGE(logbook->airports_icao_valid_wart), IMAGE_INVAL, GTK_ICON_SIZE_BUTTON);
  } else {
    gtk_image_set_from_stock(GTK_IMAGE(logbook->airports_icao_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
  }
  edctrl_set_modified(logbook->airports_edctrl);
}

void on_airports_selection_changed(GtkTreeSelection *selection, Logbook *logbook)
{
  edctrl_selection_changed(logbook->airports_edctrl);
  gtk_image_set_from_stock(GTK_IMAGE(logbook->airports_iata_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
  gtk_image_set_from_stock(GTK_IMAGE(logbook->airports_icao_valid_wart), IMAGE_KEY, GTK_ICON_SIZE_BUTTON);
}

int on_airports_icao_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  return FALSE;
}

void on_airports_tzone_changed(GtkEntry *entry, Logbook *logbook)
{
  entry_clamp_text(entry, BUF_TZ - 1, 0, NULL);
  edctrl_set_modified(logbook->airports_edctrl);
}

int on_airports_tzone_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  char zonedir[] = TZ_DIR;
  char zonepath[sizeof(zonedir) + BUF_TZ - 1];
  char *zonename = zonepath + sizeof(zonedir) - 1;
  char *p;
  FILE *file;

  strncpy(zonepath, zonedir, sizeof(zonepath));
  strncpy(zonename, gtk_entry_get_text(entry), BUF_TZ);
  zonename[BUF_TZ - 1] = '\0';

  for (p=zonename; *p; p++) {
    if (*p == ' ') *p = '_';
  }

  if (strlen(gtk_entry_get_text(entry)) && (file = fopen(zonepath, "r"))) {
    fclose(file);
    gtk_entry_set_text(entry, zonename);
  } else {
    gtk_entry_set_text(entry, "");
  }
  
  return FALSE;
}

void on_airports_lat_changed(GtkEntry *entry, Logbook *logbook)
{
  /* 10 chars for -12.123456 */
  entry_clamp_text(entry, 10, 0, is_num_char);
  edctrl_set_modified(logbook->airports_edctrl);

}
int on_airports_lat_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  double f;
  char fstr[11];

  if (strlen(gtk_entry_get_text(entry))) {
    sscanf(gtk_entry_get_text(entry), "%lf", &f);
    
    if (f > 90) f = 90;
    else if (f < -90) f = -90;
    
    snprintf(fstr, sizeof(fstr), "%.6lf", f);
    
    gtk_entry_set_text(entry, fstr);
  }

  return FALSE;
}

void on_airports_lon_changed(GtkEntry *entry, Logbook *logbook)
{
  /* 11 chars for -123.123456 */
  entry_clamp_text(entry, 11, 0, is_num_char);
  edctrl_set_modified(logbook->airports_edctrl);
}

int on_airports_lon_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  double f;
  char fstr[12];

  if (strlen(gtk_entry_get_text(entry))) {
    sscanf(gtk_entry_get_text(entry), "%lf", &f);
    
    if (f > 180) f = 180;
    else if (f < -180) f = -180;
    
    snprintf(fstr, sizeof(fstr), "%.6lf", f);
    
    gtk_entry_set_text(entry, fstr);
  }
    
    return FALSE;
}

void on_airports_elev_changed(GtkEntry *entry, Logbook *logbook)
{
  /* 5 chars for 99999 or -9999, should cover most airport elevations in feet */
  entry_clamp_text(entry, 5, 0, is_num_char);
  edctrl_set_modified(logbook->airports_edctrl);
}

int on_airports_elev_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Logbook *logbook)
{
  long elev;
  char elevstr[6];

  if (strlen(gtk_entry_get_text(entry))) {
    sscanf(gtk_entry_get_text(entry), "%ld", &elev);
    
    if (elev > 99999) elev = 99999;
    else if (elev < -9999) elev = -9999;
    
    snprintf(elevstr, sizeof(elevstr), "%ld", elev);
    
    gtk_entry_set_text(entry, elevstr);
  }

  return FALSE;
}

void on_airports_name_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->airports_edctrl);
}

void on_airports_city_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->airports_edctrl);
}

void on_airports_province_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->airports_edctrl);
}

void on_airports_country_changed(GtkEntry *entry, Logbook *logbook)
{
  edctrl_set_modified(logbook->airports_edctrl);
}

void on_airports_notes_changed(GtkTextBuffer *tb, Logbook *logbook)
{
  edctrl_set_modified(logbook->airports_edctrl);
}

void on_airports_del_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_del_btn_clicked(logbook->airports_edctrl);
}

void on_airports_save_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_save_btn_clicked(logbook->airports_edctrl);
}

void on_airports_new_btn_clicked(GtkButton *button, Logbook *logbook)
{
  edctrl_new_btn_clicked(logbook->airports_edctrl);
}

void on_airports_armdel_btn_toggled(GtkButton *button, Logbook *logbook)
{
  edctrl_armdel_btn_toggled(logbook->airports_edctrl);
}
