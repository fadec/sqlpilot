


/* void reports_refresh(Sqlpilot *sqlpilot) */
/* { */
/*   static int lock=0; */
/*   //  DBStatement *select; */
/*   char sql[4096]; */
/*   const char *where_clause; */
/*   long nrows; */
/*   int err; */
/*   char results_summary[128]; */

/*   /\* refreshing while already refreshing is bad *\/ */
/*   if (lock) return; */
/*   lock = 1; */

/*   gtk_widget_hide(sqlpilot->flights_results_summary); */
/*   gtk_widget_show(sqlpilot->flights_query_progress); */

/*   where_clause = gtk_entry_get_text(GTK_ENTRY(sqlpilot->flights_where)); */

/*   if (strlen(where_clause)) { */
/*     snprintf(sql, sizeof(sql), "%s where %s ;", FLIGHTS_SELECT, where_clause); */
/*     //    snprintf(sql_nrows, sizeof(sql_nrows), "select count(*) from flights where %s ;"); */
/*   } else { */
/*     snprintf(sql, sizeof(sql), "%s ;", FLIGHTS_SELECT); */
/*   } */
/*   db_finalize(sqlpilot->flights_select_all); */
/*   if ((err = db_prepare(sqlpilot->db, sql, &sqlpilot->flights_select_all)) != DB_OK) { */
/*     if (sqlpilot->flights_select_all) { */
/*       db_finalize(sqlpilot->flights_select_all); */
/*     } */
/*     sqlpilot->flights_select_all = db_prep(sqlpilot->db, "SELECT 42 WHERE 0 = 1;"); */
/*   } */


/*   gtk_label_set_text(GTK_LABEL(sqlpilot->flights_results_summary), ""); */
/*   nrows = store_repopulate_from_stmt_with_progress(GTK_LIST_STORE(sqlpilot->flights_treemodel), */
/* 						   sqlpilot->flights_select_all, */
/* 						   GTK_PROGRESS_BAR(sqlpilot->flights_query_progress)); */

/*   if (err == DB_OK) { */
/*     snprintf(results_summary, sizeof(results_summary), "%ld Flights", nrows); */
/*   } else { */
/*     snprintf(results_summary, sizeof(results_summary), "Error"); */
/*   } */
/*   gtk_label_set_text(GTK_LABEL(sqlpilot->flights_results_summary), results_summary); */

/*   flights_load_selection(sqlpilot); */
/*   sqlpilot->flights_stale = FALSE; */

/*   flights_refresh_utilization(sqlpilot); */

/*   gtk_widget_hide(sqlpilot->flights_query_progress); */
/*   gtk_widget_show(sqlpilot->flights_results_summary); */

/*   lock = 0; */
/* } */
