#include "sqlpilot.h"


#include "sqlpilot.h"

void on_types_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_types_ident(entry);
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
