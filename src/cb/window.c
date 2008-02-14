#include "sqlpilot.h"

void on_window_destroy(GtkObject *object, Sqlpilot *sqlpilot)
{
  gtk_main_quit();
}

void on_notebook_switch_page(GtkNotebook *notebook, GtkNotebookPage *page, guint npage, Sqlpilot *sqlpilot)
{
  switch (npage) {
  case NOTEBOOK_PAGE_FLIGHTS:
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->flights_treemodel), sqlpilot->flights_select_all);
    break;
  case NOTEBOOK_PAGE_ROLES:
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->roles_treemodel), sqlpilot->roles_select_all);
    break;
  case NOTEBOOK_PAGE_AIRCRAFT:
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->aircraft_treemodel), sqlpilot->aircraft_select_all);
    break;
  case NOTEBOOK_PAGE_TYPES:
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->types_treemodel), sqlpilot->types_select_all);
    break;
  case NOTEBOOK_PAGE_AIRPORTS:
    store_repopulate_from_stmt(GTK_LIST_STORE(sqlpilot->airports_treemodel), sqlpilot->airports_select_all);
    break;
  }
}
