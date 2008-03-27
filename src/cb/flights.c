#include "sqlpilot.h"
#include "flights.h"

void on_flights_trip_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int is_numeric_date_char(char c)
{
  return (c >= '0' && c <= '9') || c == '-';
}
int on_flights_date_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  return FALSE;
}
int on_flights_date_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_date_on_focus_out(entry);
  return FALSE;
}
void on_flights_date_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 10, 0, is_numeric_date_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
} 

void on_flights_role_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_roles_ident(entry);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_fltno_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_aircraft_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_aircraft_ident(entry);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_dep_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_airports_ident(entry);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_dep_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_utc))) {
    reconcile_time_entries(sqlpilot,
			   GTK_ENTRY(sqlpilot->flights_sout),
			   GTK_ENTRY(sqlpilot->flights_sout),
			   GTK_ENTRY(sqlpilot->flights_sin),
			   GTK_ENTRY(sqlpilot->flights_sdur));
    reconcile_time_entries(sqlpilot,
			   GTK_ENTRY(sqlpilot->flights_aout),
			   GTK_ENTRY(sqlpilot->flights_aout),
			   GTK_ENTRY(sqlpilot->flights_ain),
			   GTK_ENTRY(sqlpilot->flights_dur));
  }
  return FALSE;
}

void on_flights_arr_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_airports_ident(entry);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_arr_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  if (!gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->flights_utc))) {
    reconcile_time_entries(sqlpilot,
			   GTK_ENTRY(sqlpilot->flights_sin),
			   GTK_ENTRY(sqlpilot->flights_sout),
			   GTK_ENTRY(sqlpilot->flights_sin),
			   GTK_ENTRY(sqlpilot->flights_sdur));
    reconcile_time_entries(sqlpilot,
			   GTK_ENTRY(sqlpilot->flights_ain),
			   GTK_ENTRY(sqlpilot->flights_aout),
			   GTK_ENTRY(sqlpilot->flights_ain),
			   GTK_ENTRY(sqlpilot->flights_dur));
  }
  return FALSE;
}

int on_flights_sout_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_sout_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(sqlpilot,
			 GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sdur));
  return FALSE;
}
void on_flights_sout_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
  //fprintf(stderr, "changed sout\n");
}

int on_flights_sin_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_sin_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(sqlpilot,
			 GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sdur));
  return FALSE;
}
void on_flights_sin_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_sdur_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_sdur_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  reconcile_time_entries(sqlpilot,
			 GTK_ENTRY(sqlpilot->flights_sdur),
			 GTK_ENTRY(sqlpilot->flights_sout),
			 GTK_ENTRY(sqlpilot->flights_sin),
			 GTK_ENTRY(sqlpilot->flights_sdur));
  return FALSE;
}

void on_flights_sdur_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_aout_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_aout_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(sqlpilot,
			 GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_dur));
  return FALSE;
}
void on_flights_aout_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}


int on_flights_ain_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_ain_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, ':');
  reconcile_time_entries(sqlpilot,
			 GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_dur));
  return FALSE;
}
void on_flights_ain_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_dur_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_dur_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  reconcile_time_entries(sqlpilot,
			 GTK_ENTRY(sqlpilot->flights_dur),
			 GTK_ENTRY(sqlpilot->flights_aout),
			 GTK_ENTRY(sqlpilot->flights_ain),
			 GTK_ENTRY(sqlpilot->flights_dur));
  return FALSE;
}
void on_flights_dur_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_inst_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_inst_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_inst_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_siminst_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_siminst_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_siminst_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

int on_flights_night_focus_in_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_in(entry);
  return FALSE;
}
int on_flights_night_focus_out_event(GtkEntry *entry, GdkEventFocus *event, Sqlpilot *sqlpilot)
{
  entry_format_time_on_focus_out(entry, '+');
  return FALSE;
}
void on_flights_night_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 5, 1, is_time_char);
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_selection_changed(GtkTreeSelection *selection, Sqlpilot *sqlpilot)
{
  edctrl_selection_changed(sqlpilot->flights_edctrl);
}

void on_flights_utc_toggled(GtkToggleButton *button, Sqlpilot *logb)
{
  char deptz[BUF_TZ], arrtz[BUF_TZ];
  char *deptz1, *deptz2, *arrtz1, *arrtz2;
  const char *dep, *arr, *date, *aout, *ain, *sout, *sin;
  char strdate[BUF_DATE], strtime[BUF_TIME];

  dep  = gtk_entry_get_text(GTK_ENTRY(logb->flights_dep));
  arr  = gtk_entry_get_text(GTK_ENTRY(logb->flights_arr));
  date = gtk_entry_get_text(GTK_ENTRY(logb->flights_date));
  aout = gtk_entry_get_text(GTK_ENTRY(logb->flights_aout));
  ain  = gtk_entry_get_text(GTK_ENTRY(logb->flights_ain));
  sout = gtk_entry_get_text(GTK_ENTRY(logb->flights_sout));
  sin  = gtk_entry_get_text(GTK_ENTRY(logb->flights_sin));

  tz_of_airport_ident(logb->db, dep, deptz, sizeof(deptz));
  tz_of_airport_ident(logb->db, arr, arrtz, sizeof(arrtz));

  if (gtk_toggle_button_get_active(button)) {
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
  //printf("%s -> %s, %s -> %s\n", deptz1, deptz2, arrtz1, arrtz2);

  /* Do the switch */
  if (strlen(aout)) {
    move_time(deptz1, deptz2, date, aout, strdate, strtime);
    //fprintf(stderr, "aout: %s %s -> %s %s\n", aout, deptz1, strtime, deptz2);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_aout), strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_date), strdate);
  } 
  if (strlen(sout)) {
    move_time(deptz1, deptz2, date, sout, strdate, strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sout), strtime);
    if (!strlen(aout)) gtk_entry_set_text(GTK_ENTRY(logb->flights_date), strdate);
  }
  if (strlen(ain)) {
    move_time(arrtz1, arrtz2, date, ain, strdate, strtime);
    //    fprintf(stderr, "ain: %s -> %s\n", aout, strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_ain), strtime);
  }
  if (strlen(sin)) {
    move_time(arrtz1, arrtz2, date, sin, strdate, strtime);
    gtk_entry_set_text(GTK_ENTRY(logb->flights_sin), strtime);
  }
  gtk_label_set_text(GTK_LABEL(logb->flights_utc_lbl),
		     gtk_toggle_button_get_active(button) ? "UTC" : "Local");
      
}

void on_flights_aprch_changed(GtkWidget *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_xc_toggled(GtkWidget *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_hold_toggled(GtkWidget *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_dland_value_changed(GtkWidget *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_nland_value_changed(GtkWidget *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->flights_edctrl);
}

void on_flights_armdel_btn_toggled(GtkToggleButton *button, Sqlpilot *sqlpilot)
{
  edctrl_armdel_btn_toggled(sqlpilot->flights_edctrl);
}

void on_flights_del_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_del_btn_clicked(sqlpilot->flights_edctrl);
}

void on_flights_new_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_new_btn_clicked(sqlpilot->flights_edctrl);
}

void on_flights_save_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_save_btn_clicked(sqlpilot->flights_edctrl);
}
