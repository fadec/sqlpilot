#include "sqlpilot.h"

void on_aircraft_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_aircraft_ident(entry);
  edctrl_set_modified(sqlpilot->aircraft_edctrl);
}

void on_aircraft_type_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_types_ident(entry);
  edctrl_set_modified(sqlpilot->aircraft_edctrl);
}

void on_aircraft_fleetno_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->aircraft_edctrl);
}

void on_aircraft_selection_changed(GtkTreeSelection *selection, Sqlpilot *sqlpilot)
{
  edctrl_selection_changed(sqlpilot->aircraft_edctrl);
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
