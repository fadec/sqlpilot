#include "sqlpilot.h"
#include "cb/cb.h"

void on_window_destroy(GtkObject *object, Sqlpilot *sqlpilot)
{
  gtk_main_quit();
}

void on_notebook_switch_page(GtkNotebook *notebook, GtkNotebookPage *page, guint npage, Sqlpilot *sqlpilot)
{
  switch (npage) {
  case NOTEBOOK_PAGE_FLIGHTS:
    if (sqlpilot->flights_stale) flights_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_ROLES:
    if (sqlpilot->roles_stale) roles_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_AIRCRAFT:
    if (sqlpilot->aircraft_stale) aircraft_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_TYPES:
    if (sqlpilot->types_stale) types_refresh(sqlpilot);
    break;
  case NOTEBOOK_PAGE_AIRPORTS:
    if (sqlpilot->airports_stale) airports_refresh(sqlpilot);
    break;
  }
}
