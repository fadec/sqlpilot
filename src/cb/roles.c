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

/* void on_roles_insert_clicked(GtkButton *button, Sqlpilot *sqlpilot) */
/* { */
/*   DBStatement *stmt; */
/*   GtkTreeIter iter; */
/*   DBint64 inserted_id; */

/*   roles_write_entries(sqlpilot, NULL); */
/*   inserted_id = db_last_insert_rowid(sqlpilot->db);  */

/*   /\* Read row into treemodel *\/ */
/*   stmt = sqlpilot->roles_select_by_id; */
/*   db_bind_int64(stmt, 1, inserted_id); */
/*   gtk_list_store_insert(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter, 0); */
/*   store_update_row(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter, stmt); */
  
/*   db_reset(stmt); */
/*   db_clear_bindings(stmt); */

/*   set_dependents_stale(sqlpilot); */
/* } */

/* void on_roles_update_clicked(GtkButton *button, Sqlpilot *sqlpilot) */
/* { */
/*   GtkTreeIter iter; */
/*   gchar *id; */
/*   DBStatement *stmt; */
  
/*   if (gtk_tree_selection_get_selected(sqlpilot->roles_selection, &sqlpilot->roles_treemodel, &iter)) { */
/*     gtk_tree_model_get(sqlpilot->roles_treemodel, &iter, ROLES_COL_ID, &id, -1); */

/*     roles_write_entries(sqlpilot, id); */

/*     /\* Read row into treemodel *\/ */
/*     stmt = sqlpilot->roles_select_by_id; */
/*     db_bind_text(stmt, 1, id); */
/*     store_update_row(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter, stmt); */
  
/*     db_reset(stmt); */
/*     db_clear_bindings(stmt); */
/*     g_free(id); */
    
/*     set_dependents_stale(sqlpilot); */
/*   } */
/* } */

/* void on_roles_delete_clicked(GtkButton *button, Sqlpilot *sqlpilot) */
/* { */
/*   GtkTreeIter iter; */
/*   gchar *id; */
  
/*   if (gtk_tree_selection_get_selected(sqlpilot->roles_selection, &sqlpilot->roles_treemodel, &iter)) { */
/*     gtk_tree_model_get(sqlpilot->roles_treemodel, &iter, ROLES_COL_ID, &id, -1); */
    
/*     db_bind_text(sqlpilot->roles_delete, 1, id); */

/*     db_step(sqlpilot->roles_delete); */
/*     db_reset(sqlpilot->roles_delete); */
/*     db_clear_bindings(sqlpilot->roles_delete); */
  
/*     gtk_list_store_remove(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter); */

/*     g_free(id); */

/*     set_dependents_stale(sqlpilot); */
/*   } */
/* } */
/* void on_roles_cancel_clicked(GtkButton *button, Sqlpilot *sqlpilot) */
/* { */
/*   roles_load_entries_from_selection(sqlpilot); */
/* } */

void on_roles_selection_changed(GtkTreeSelection *selection, Sqlpilot *sqlpilot)
{
  edctrl_selection_changed(sqlpilot->roles_edctrl);
}
