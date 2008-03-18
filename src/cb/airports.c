#include "sqlpilot.h"

static void set_dependents_stale(Sqlpilot *sqlpilot)
{
  sqlpilot->flights_stale = TRUE;
}

static void airports_write_entries(Sqlpilot *sqlpilot, const gchar *id)
{
  const gchar
    *ident,
    *icao,
    *name,
    *tzone;
  gchar *notes;
  DBStatement *stmt;

  ident    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_ident));
  icao     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_icao));
  name     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_name));
  tzone    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->airports_tzone));
  notes    = text_view_get_text(GTK_TEXT_VIEW(sqlpilot->airports_notes));

  
  /* Write entries to database */
  if (id) {
    stmt = sqlpilot->airports_update;
    db_bind_text(stmt, AIRPORTS_WRITE_ID, id);
  } else {
    stmt = sqlpilot->airports_insert;
  }
  db_bind_text(stmt, AIRPORTS_WRITE_IDENT, ident);
  db_bind_text(stmt, AIRPORTS_WRITE_ICAO, icao);
  db_bind_text(stmt, AIRPORTS_WRITE_NAME, name);
  db_bind_text(stmt, AIRPORTS_WRITE_TZONE, tzone);
  db_bind_text(stmt, AIRPORTS_WRITE_NOTES, notes);
  
  db_step(stmt);
  db_reset(stmt);
  db_clear_bindings(stmt);

  g_free(notes);
}

static void airports_load_entries_from_selection(Sqlpilot *logb)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *id=NULL,
    *ident=NULL,
    *icao=NULL,
    *name=NULL,
    *tzone=NULL,
    *notes=NULL;

  if (gtk_tree_selection_get_selected (logb->airports_selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       AIRPORTS_COL_ID, &id,
		       AIRPORTS_COL_IDENT, &ident,
		       AIRPORTS_COL_ICAO, &icao,
		       AIRPORTS_COL_NAME, &name,
		       AIRPORTS_COL_TZONE, &tzone,
		       AIRPORTS_COL_NOTES, &notes,
		       -1);
  }
  
  gtk_entry_set_text(GTK_ENTRY(logb->airports_ident), EMPTY_IF_NULL(ident));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_icao), EMPTY_IF_NULL(icao));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_name), EMPTY_IF_NULL(name));
  gtk_entry_set_text(GTK_ENTRY(logb->airports_tzone), EMPTY_IF_NULL(tzone));
  text_view_set_text(GTK_TEXT_VIEW(logb->airports_notes), EMPTY_IF_NULL(notes));

  g_free(id);
  g_free(ident);
  g_free(icao);
  g_free(name);
  g_free(tzone);
  g_free(notes);
}

void airports_refresh(Sqlpilot *sqlpilot)
{
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->airports_treemodel), sqlpilot->airports_select_all);
    airports_load_entries_from_selection(sqlpilot);
    sqlpilot->airports_stale = FALSE;
}

void on_airports_ident_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_airports_ident(entry);
}

void on_airports_insert_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  DBStatement *stmt;
  GtkTreeIter iter;
  DBint64 inserted_id;

  airports_write_entries(sqlpilot, NULL);
  inserted_id = db_last_insert_rowid(sqlpilot->db); 

  /* Read row into treemodel */
  stmt = sqlpilot->airports_select_by_id;
  db_bind_int64(stmt, 1, inserted_id);
  gtk_list_store_insert(GTK_LIST_STORE(sqlpilot->airports_treemodel), &iter, 0);
  store_update_row(GTK_LIST_STORE(sqlpilot->airports_treemodel), &iter, stmt);
  
  db_reset(stmt);
  db_clear_bindings(stmt);

  set_dependents_stale(sqlpilot);
}

void on_airports_update_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  DBStatement *stmt;
  
  if (gtk_tree_selection_get_selected(sqlpilot->airports_selection, &sqlpilot->airports_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->airports_treemodel, &iter, AIRPORTS_COL_ID, &id, -1);

    airports_write_entries(sqlpilot, id);

    /* Read row into treemodel */
    stmt = sqlpilot->airports_select_by_id;
    db_bind_text(stmt, 1, id);
    store_update_row(GTK_LIST_STORE(sqlpilot->airports_treemodel), &iter, stmt);
  
    db_reset(stmt);
    db_clear_bindings(stmt);
    g_free(id);

    set_dependents_stale(sqlpilot);
  }
}

void on_airports_delete_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *id;
  
  if (gtk_tree_selection_get_selected(sqlpilot->airports_selection, &sqlpilot->airports_treemodel, &iter)) {
    gtk_tree_model_get(sqlpilot->airports_treemodel, &iter, AIRPORTS_COL_ID, &id, -1);
    
    db_bind_text(sqlpilot->airports_delete, 1, id);

    db_step(sqlpilot->airports_delete);
    db_reset(sqlpilot->airports_delete);
    db_clear_bindings(sqlpilot->airports_delete);
  
    gtk_list_store_remove(GTK_LIST_STORE(sqlpilot->airports_treemodel), &iter);

    g_free(id);

    set_dependents_stale(sqlpilot);
  }
}
void on_airports_cancel_clicked(GtkButton *button, Sqlpilot *sqlpilot)
{
  airports_load_entries_from_selection(sqlpilot);
}

void on_airports_selection_changed(GtkTreeSelection *selection, Sqlpilot *logb)
{
  airports_load_entries_from_selection(logb);
}

void on_airports_icao_changed(GtkEntry *entry, Sqlpilot *sqlpilot)
{
  entry_clamp_text(entry, 4, 1, is_ident_char);
}
