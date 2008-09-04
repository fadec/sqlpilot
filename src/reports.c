
#include "reports.h"

void reports_refresh(Logbook *logbook)
{
  static int lock=0;

  long nrows;
  int err;
  char results_summary[128];
  char *sql;
  DBStatement *select = NULL;
  GtkTreeModel *model;
  GtkWidget *view;

  /* refreshing while already refreshing is bad */
  if (lock) return;
  lock = 1;

  gtk_widget_hide(logbook->reports_results_summary);
  gtk_widget_show(logbook->reports_query_progress);

  sql = text_view_get_text(GTK_TEXT_VIEW(logbook->reports_sql_text));
  gtk_label_set_text(GTK_LABEL(logbook->reports_results_summary), "");
  gtk_widget_hide(logbook->reports_err_msg);

  if (sql && strlen(sql)) {
    if ((err = db_prepare(logbook->db, sql, &select)) == DB_OK) {
      if (db_column_count(select)) {
	store_build_query_stmt_widget(select, NULL, &view, &model);
	nrows = store_repopulate_from_stmt_with_progress(GTK_LIST_STORE(model), select, GTK_PROGRESS_BAR(logbook->reports_query_progress));
	snprintf(results_summary, sizeof(results_summary), "%ld Rows", nrows);
	if (logbook->reports_treeview) { gtk_widget_destroy(logbook->reports_treeview); }
	gtk_container_add(GTK_CONTAINER(logbook->reports_sw), view);
	gtk_widget_show(GTK_WIDGET(view));
	logbook->reports_treeview = view;
      } else {
	snprintf(results_summary, sizeof(results_summary), "Wrong SQL");
	gtk_label_set_text(GTK_LABEL(logbook->reports_err_msg), "The statement does not have any colums");
	gtk_widget_show(logbook->reports_err_msg);
      }
    } else {
      snprintf(results_summary, sizeof(results_summary), "SQL Error");
      gtk_label_set_text(GTK_LABEL(logbook->reports_err_msg), db_errmsg(logbook->db));
      gtk_widget_show(logbook->reports_err_msg);
    }
  } else {
    snprintf(results_summary, sizeof(results_summary), "No SQL");
  }

  gtk_label_set_text(GTK_LABEL(logbook->reports_results_summary), results_summary);
  gtk_widget_hide(logbook->reports_query_progress);
  gtk_widget_show(logbook->reports_results_summary);
  
  if (select) { db_finalize(select); }

  lock = 0;
}
