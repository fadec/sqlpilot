

GtkWidget *flights_pane_new(void)
{
  GtkWidget *b_outer;
  GtkWidget *b_edit;
  GtkWidget *log_pane;

  return b_outer;
}

GtkWidget *log_pane_new(void)
{
  GtkTreeStore *store;
  GtkTreeView *view;
  DBStatement *stmt;
  GtkWidget *sw;
  const char *sql = "select aircraft, depart, arrive, out, in, duration from flights order by out";

  logbook_get_table(app->db, sql);

  return sw;
