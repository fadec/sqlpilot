#include "sqlpilot.h"

void on_roles_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_roles_ident(entry);
  edctrl_set_modified(sqlpilot->roles_edctrl);
}

void on_roles_del_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_del_btn_clicked(sqlpilot->roles_edctrl);
}

void on_roles_save_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_save_btn_clicked(sqlpilot->roles_edctrl);
}

void on_roles_new_btn_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_new_btn_clicked(sqlpilot->roles_edctrl);
}

void on_roles_name_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_pic_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_sic_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_fe_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_solo_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_dual_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_instruct_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}
void on_roles_total_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_set_modified(sqlpilot->roles_edctrl);
}

void on_roles_armdel_btn_toggled(GtkButton *button, Sqlpilot *sqlpilot)
{
  edctrl_armdel_btn_toggled(sqlpilot->roles_edctrl);
}

void on_roles_selection_changed(GtkTreeSelection *selection, Sqlpilot *sqlpilot)
{
  edctrl_selection_changed(sqlpilot->roles_edctrl);
}
