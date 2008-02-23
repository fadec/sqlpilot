#include "sqlpilot.h"

static void set_dependents_stale(Sqlpilot *sqlpilot)
{
  sqlpilot->flights_stale = TRUE;
}

void roles_write_entries(Sqlpilot *sqlpilot, const gchar *id)
{
  const gchar
    *ident,
    *name;
  gboolean
    pic,
    sic,
    fe,
    solo,
    dual,
    instruct;
  DBStatement *stmt;

  ident    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->roles_ident));
  name     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->roles_name));
  pic      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_pic));
  sic      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_sic));
  fe       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_fe));
  solo     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_solo));
  dual     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_dual));
  instruct = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_instruct));
  
  /* Write entries to database */
  if (id) {
    stmt = sqlpilot->roles_update;
    db_bind_text(stmt, ROLES_WRITE_ID, id);
  } else {
    stmt = sqlpilot->roles_insert;
  }
  db_bind_text(stmt, ROLES_WRITE_IDENT, ident);
  db_bind_text(stmt, ROLES_WRITE_NAME, name);
  db_bind_int(stmt, ROLES_WRITE_PIC, pic ? 1 : 0);
  db_bind_int(stmt, ROLES_WRITE_SIC, sic ? 1 : 0);
  db_bind_int(stmt, ROLES_WRITE_FE, fe ? 1 : 0);
  db_bind_int(stmt, ROLES_WRITE_SOLO, solo ? 1 : 0);
  db_bind_int(stmt, ROLES_WRITE_DUAL, dual ? 1 : 0);
  db_bind_int(stmt, ROLES_WRITE_INSTRUCT, instruct ? 1 : 0);

  db_step(stmt);
  db_reset(stmt);
  db_clear_bindings(stmt);
}

void roles_load_entries_from_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id=NULL,
    *ident=NULL,
    *name=NULL,
    *pic=NULL,
    *sic=NULL,
    *fe=NULL,
    *solo=NULL,
    *dual=NULL,
    *instruct=NULL;

  if (gtk_tree_selection_get_selected (logb->roles_selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       ROLES_COL_ID, &id,
		       ROLES_COL_IDENT, &ident,
		       ROLES_COL_NAME, &name,
		       ROLES_COL_PIC, &pic,
		       ROLES_COL_SIC, &sic,
		       ROLES_COL_FE, &fe,
		       ROLES_COL_SOLO, &solo,
		       ROLES_COL_DUAL, &dual,
		       ROLES_COL_INSTRUCT, &instruct,
		       -1);
  }
  gtk_entry_set_text(GTK_ENTRY(logb->roles_ident), EMPTY_IF_NULL(ident));
  gtk_entry_set_text(GTK_ENTRY(logb->roles_name), EMPTY_IF_NULL(name));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->roles_pic), str_bool(pic));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->roles_sic), str_bool(sic));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->roles_fe), str_bool(fe));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->roles_solo), str_bool(solo));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->roles_dual), str_bool(dual));
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->roles_instruct), str_bool(instruct));

  g_free(id);
  g_free(ident);
  g_free(name);
  g_free(pic);
  g_free(sic);
  g_free(fe);
  g_free(solo);
  g_free(dual);
  g_free(instruct);
}

void roles_refresh(Sqlpilot *sqlpilot)
{
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->roles_treemodel), sqlpilot->roles_select_all);
    roles_load_entries_from_selection(sqlpilot);
    sqlpilot->roles_stale = FALSE;
}

void on_roles_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_roles_ident(entry);
}

void on_roles_insert_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  DBStatement *stmt;
  GtkTreeIter iter;
  DBint64 inserted_id;

  roles_write_entries(sqlpilot, NULL);
  inserted_id = db_last_insert_rowid(sqlpilot->db); 

  /* Read row into treemodel */
  stmt = sqlpilot->roles_select_by_id;
  db_bind_int64(stmt, 1, inserted_id);
  gtk_list_store_insert(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter, 0);
  store_update_row(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter, stmt);
  
  db_reset(stmt);
  db_clear_bindings(stmt);

  set_dependents_stale(sqlpilot);
}

void on_roles_update_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  DBStatement *stmt;
  
  if (gtk_tree_selection_get_selected(sqlpilot->roles_selection, &sqlpilot->roles_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->roles_treemodel, &iter, ROLES_COL_ID, &id, -1);

    roles_write_entries(sqlpilot, id);

    /* Read row into treemodel */
    stmt = sqlpilot->roles_select_by_id;
    db_bind_text(stmt, 1, id);
    store_update_row(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter, stmt);
  
    db_reset(stmt);
    db_clear_bindings(stmt);
    g_free(id);
    
    set_dependents_stale(sqlpilot);
  }
}

void on_roles_delete_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  
  if (gtk_tree_selection_get_selected(sqlpilot->roles_selection, &sqlpilot->roles_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->roles_treemodel, &iter, ROLES_COL_ID, &id, -1);
    
    db_bind_text(sqlpilot->roles_delete, 1, id);

    db_step(sqlpilot->roles_delete);
    db_reset(sqlpilot->roles_delete);
    db_clear_bindings(sqlpilot->roles_delete);
  
    gtk_list_store_remove(GTK_LIST_STORE(sqlpilot->roles_treemodel), &iter);

    g_free(id);

    set_dependents_stale(sqlpilot);
  }
}
void on_roles_cancel_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  roles_load_entries_from_selection(sqlpilot);
}

void on_roles_selection_changed(GtkTreeSelection *selection, Sqlpilot *logb)
{
  roles_load_entries_from_selection(logb);
}
