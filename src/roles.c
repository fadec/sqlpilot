
#include "sqlpilot.h"

void roles_after_change(Sqlpilot *sqlpilot)
{
  sqlpilot->flights_stale = TRUE;
}

DBint64 roles_write_entries(const gchar *id, Sqlpilot *sqlpilot)
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
    instruct,
    total;
  DBStatement *stmt;

  ident    = gtk_entry_get_text(GTK_ENTRY(sqlpilot->roles_ident));
  name     = gtk_entry_get_text(GTK_ENTRY(sqlpilot->roles_name));
  pic      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_pic));
  sic      = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_sic));
  fe       = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_fe));
  solo     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_solo));
  dual     = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_dual));
  instruct = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_instruct));
  total    = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(sqlpilot->roles_total));
  
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
  db_bind_int(stmt, ROLES_WRITE_TOTAL, total ? 1 : 0);

  db_stp_res_clr(stmt);

  if (id) {
    return 0;
  } else {
    return db_last_insert_rowid(sqlpilot->db);
  }
}

void roles_load_selection(Sqlpilot *logb)
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
    *instruct=NULL,
    *total=NULL;

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
	       	       ROLES_COL_TOTAL, &total,
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
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logb->roles_total), str_bool(total));

  g_free(id);
  g_free(ident);
  g_free(name);
  g_free(pic);
  g_free(sic);
  g_free(fe);
  g_free(solo);
  g_free(dual);
  g_free(instruct);
  g_free(total);
}

void roles_refresh(Sqlpilot *sqlpilot)
{
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->roles_treemodel), sqlpilot->roles_select_all);
    roles_load_selection(sqlpilot);
    sqlpilot->roles_stale = FALSE;
}

int roles_selection_show(GtkTreeSelection *selection, char *show, size_t size)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar
    *ident=NULL,
    *name=NULL;
  if (gtk_tree_selection_get_selected (selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter,
		       ROLES_COL_IDENT, &ident,
		       ROLES_COL_NAME, &name,
		       -1);
    snprintf(show, size, "%s - %s", name, ident);
    return 1;
  } else {
    show[0] = '\0';
    return -1;
  }
}

int roles_can_delete(GtkTreeSelection *selection)
{
  GtkTreeIter iter;
  GtkTreeModel *model;
  gchar *flights=NULL;
  long _flights=0;

  if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
    gtk_tree_model_get(model, &iter, ROLES_COL_FLIGHTS, &flights, -1);
    sscanf(flights, "%ld", &_flights);
  }

  g_free(flights);

  return !_flights;
}
