
#include "reports.h"

DBStatement *reports_make_statement(Logbook *logbook)
{
  DBStatement *select = NULL;
  char *sql = text_view_get_text(GTK_TEXT_VIEW(logbook->reports_sql_text));
  char results_summary[128];

  gtk_widget_hide(logbook->reports_err_msg);
  if (sql && strlen(sql)) {
    if (db_prepare(logbook->db, sql, &select) == DB_OK) {
      if (db_column_count(select)) {
	return select;
      } else {
	snprintf(results_summary, sizeof(results_summary), "Wrong SQL");
	gtk_label_set_text(GTK_LABEL(logbook->reports_err_msg), "The statement does not have any colums");
	gtk_widget_show(logbook->reports_err_msg);
	gtk_expander_set_expanded(GTK_EXPANDER(logbook->reports_sql_expander), TRUE);
      }
    } else {
      snprintf(results_summary, sizeof(results_summary), "SQL Error");
      gtk_label_set_text(GTK_LABEL(logbook->reports_err_msg), db_errmsg(logbook->db));
      gtk_widget_show(logbook->reports_err_msg);
      gtk_expander_set_expanded(GTK_EXPANDER(logbook->reports_sql_expander), TRUE);
    }
  } else {
    snprintf(results_summary, sizeof(results_summary), "No SQL");
    gtk_label_set_text(GTK_LABEL(logbook->reports_err_msg), "Enter an SQL select statement");
    gtk_expander_set_expanded(GTK_EXPANDER(logbook->reports_sql_expander), TRUE);
  }
  if (select) { db_finalize(select); }
  gtk_label_set_text(GTK_LABEL(logbook->reports_results_summary), results_summary);
  return NULL;
}

void reports_refresh(Logbook *logbook)
{
  static int lock=0;

  long nrows;
  char results_summary[128];
  DBStatement *select = NULL;
  GtkTreeModel *treemod;
  GtkWidget *view;

  /* refreshing while already refreshing is bad */
  if (lock) return;
  lock = 1;

  gtk_widget_hide(logbook->reports_results_summary);
  gtk_widget_show(logbook->reports_query_progress);

  gtk_label_set_text(GTK_LABEL(logbook->reports_results_summary), "");

  if ((select = reports_make_statement(logbook))) {
    store_build_query_stmt_widget(select, NULL, &view, &treemod);
    nrows = store_repopulate_from_stmt_with_progress(GTK_LIST_STORE(treemod), select, GTK_PROGRESS_BAR(logbook->reports_query_progress));
    snprintf(results_summary, sizeof(results_summary), "%ld Rows", nrows);
    if (logbook->reports_treeview) { gtk_widget_destroy(logbook->reports_treeview); }
    gtk_container_add(GTK_CONTAINER(logbook->reports_sw), view);
    gtk_widget_show(GTK_WIDGET(view));
    logbook->reports_treeview = view;
    gtk_label_set_text(GTK_LABEL(logbook->reports_results_summary), results_summary);
  }

  gtk_widget_hide(logbook->reports_query_progress);
  gtk_widget_show(logbook->reports_results_summary);
  
  if (select) { db_finalize(select); }

  lock = 0;
}

void reports_title_combo_reload(Logbook *logbook)
{
  GtkListStore *store;
  GtkTreeIter iter;
  DBStatement *stmt;

  store = (GtkListStore*)gtk_combo_box_get_model(GTK_COMBO_BOX(logbook->reports_title_combo));
  gtk_list_store_clear(store);
  stmt = db_prep(logbook->db, "SELECT title FROM Reports;");
  while (db_step(stmt) == DB_ROW) {
    gtk_list_store_append(store, &iter);
    gtk_list_store_set (store, &iter,
			0, db_column_text(stmt, 0),
			-1);
  }
  db_finalize(stmt);
}


void reports_save(Logbook *logbook)
{
  DBStatement *stmt;

  if ((stmt = reports_make_statement(logbook))) {
    db_finalize(stmt);

    db_stp_res_clr(logbook->ta_begin);
    db_bind_text(logbook->reports_delete_by_title, 1, gtk_entry_get_text(GTK_ENTRY(logbook->reports_title)));
    db_stp_res_clr(logbook->reports_delete_by_title);
    db_bind_text(logbook->reports_insert, REPORTS_WRITE_TITLE, gtk_entry_get_text(GTK_ENTRY(logbook->reports_title)));
    db_bind_text(logbook->reports_insert, REPORTS_WRITE_SQL, text_view_get_text(GTK_TEXT_VIEW(logbook->reports_sql_text)));
    db_stp_res_clr(logbook->reports_insert);
    db_stp_res_clr(logbook->ta_commit);

    reports_title_combo_reload(logbook);
    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(logbook->reports_armdel_btn), FALSE);
    gtk_widget_set_sensitive(logbook->reports_armdel_btn, TRUE);
    gtk_widget_set_sensitive(logbook->reports_del_btn, FALSE);
    gtk_widget_set_sensitive(logbook->reports_save_btn, FALSE);
    gtk_expander_set_expanded(GTK_EXPANDER(logbook->reports_sql_expander), FALSE);
  }
}

void reports_delete(Logbook *logbook)
{
  db_bind_text(logbook->reports_delete_by_title, 1, gtk_entry_get_text(GTK_ENTRY(logbook->reports_title)));
  db_stp_res_clr(logbook->reports_delete_by_title);
  gtk_entry_set_text(GTK_ENTRY(logbook->reports_title), "");
  text_view_set_text(GTK_TEXT_VIEW(logbook->reports_sql_text), "");
  reports_title_combo_reload(logbook);
}

void reports_title_combo_init(Logbook *logbook)
{
  GtkListStore *store = gtk_list_store_new(1, G_TYPE_STRING);
  gtk_combo_box_set_model(GTK_COMBO_BOX(logbook->reports_title_combo), GTK_TREE_MODEL(store));
  gtk_combo_box_entry_set_text_column(GTK_COMBO_BOX_ENTRY(logbook->reports_title_combo), 0);
  reports_title_combo_reload(logbook);
/*   gtk_combo_box_set_active(GTK_COMBO_BOX(logbook->reports_title_combo), 0); */
}
