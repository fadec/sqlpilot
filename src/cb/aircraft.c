#include "sqlpilot.h"

void aircraft_write_entries(Sqlpilot *sqlpilot, const gchar *id)
{
  const gchar
    *ident,
    *type;
  gchar *notes;
  DBStatement *stmt;

  ident    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_ident));
  type     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->aircraft_type));
  notes    = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->aircraft_notes));
  
  /* Write entries to database */
  if (id) {
    stmt = sqlpilot->aircraft_update;
    db_bind_text(stmt, AIRCRAFT_WRITE_ID, id);
  } else {
    stmt = sqlpilot->aircraft_insert;
  }
  db_bind_text(stmt, AIRCRAFT_WRITE_IDENT, ident);
  bind_id_of(stmt, AIRCRAFT_WRITE_TYPE, "types", "ident", type);
  db_bind_text(stmt, AIRCRAFT_WRITE_NOTES, notes);

  db_step(stmt);
  db_reset(stmt);
  db_clear_bindings(stmt);

  g_free(notes);
}

void aircraft_load_entries_from_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id,
    *ident,
    *type,
    *notes;

  if (gtk_tree_selection_get_selected (logb->aircraft_selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       AIRCRAFT_COL_ID, &id,
		       AIRCRAFT_COL_IDENT, &ident,
		       AIRCRAFT_COL_TYPE, &type,
		       AIRCRAFT_COL_NOTES, &notes,
		       -1);

    gtk_entry_set_text(GTK_ENTRY(logb->aircraft_ident), EMPTY_IF_NULL(ident));
    gtk_entry_set_text(GTK_ENTRY(logb->aircraft_type), EMPTY_IF_NULL(type));
    text_view_set_text(GTK_TEXT_VIEW(logb->aircraft_notes), EMPTY_IF_NULL(notes));

    g_free(id);
    g_free(ident);
    g_free(type);
    g_free(notes);

  }
}

void on_aircraft_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_aircraft_ident(entry);
}

void on_aircraft_insert_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  DBStatement *stmt;
  GtkTreeIter iter;
  DBint64 inserted_id;

  aircraft_write_entries(sqlpilot, NULL);
  inserted_id = db_last_insert_rowid(sqlpilot->db); 

  /* Read row into treemodel */
  stmt = sqlpilot->aircraft_select_by_id;
  db_bind_int64(stmt, 1, inserted_id);
  gtk_list_store_insert(GTK_LIST_STORE(sqlpilot->aircraft_treemodel), &iter, 0);
  store_update_row(GTK_LIST_STORE(sqlpilot->aircraft_treemodel), &iter, stmt);
  
  db_reset(stmt);
  db_clear_bindings(stmt);  
}

void on_aircraft_update_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  DBStatement *stmt;
  
  if (gtk_tree_selection_get_selected(sqlpilot->aircraft_selection, &sqlpilot->aircraft_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->aircraft_treemodel, &iter, AIRCRAFT_COL_ID, &id, -1);

    aircraft_write_entries(sqlpilot, id);

    /* Read row into treemodel */
    stmt = sqlpilot->aircraft_select_by_id;
    db_bind_text(stmt, 1, id);
    store_update_row(GTK_LIST_STORE(sqlpilot->aircraft_treemodel), &iter, stmt);
  
    db_reset(stmt);
    db_clear_bindings(stmt);
    g_free(id);
  }
}

void on_aircraft_delete_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  
  if (gtk_tree_selection_get_selected(sqlpilot->aircraft_selection, &sqlpilot->aircraft_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->aircraft_treemodel, &iter, AIRCRAFT_COL_ID, &id, -1);
    
    db_bind_text(sqlpilot->aircraft_delete, 1, id);

    db_step(sqlpilot->aircraft_delete);
    db_reset(sqlpilot->aircraft_delete);
    db_clear_bindings(sqlpilot->aircraft_delete);
  
    gtk_list_store_remove(GTK_LIST_STORE(sqlpilot->aircraft_treemodel), &iter);

    g_free(id);
  }
}
void on_aircraft_cancel_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  aircraft_load_entries_from_selection(sqlpilot);
}

void on_aircraft_selection_changed(GtkTreeSelection *selection, Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;

  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    aircraft_load_entries_from_selection(logb);
  }
}


