#include "summaries.h"
#include <gtkhtml/gtkhtml.h>

static gchar *summaries_get_script_filename(Sqlpilot *sqlpilot)
{
  GtkTreeIter iter;
  gchar *filename=NULL;

  if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(sqlpilot->summaries_select_summary), &iter)) {
    gtk_tree_model_get(gtk_combo_box_get_model(GTK_COMBO_BOX(sqlpilot->summaries_select_summary)), &iter,
		       SUMMARIES_COL_FILENAME, &filename,
		       -1);
  }
  return filename;
}

void summaries_refresh(Sqlpilot *sqlpilot)
{
  gchar *sstdout, *sstderr;
  gchar *html_src;
  GError *error=NULL;
  int exit_code;
  gchar *argv[2];

  argv[0] = summaries_get_script_filename(sqlpilot);
  argv[1] = sqlpilot->db_filename;
  
  if (spawn_script(NULL, argv, NULL, "", &sstdout, &sstderr, &exit_code, &error, NULL, NULL)) {
    html_src = sstdout;
  } else {
    html_src = error->message;
  }
  gtk_html_load_from_string(GTK_HTML(sqlpilot->summaries_html), html_src, -1);

  g_free(argv[0]);
}

void summaries_init(Sqlpilot *sqlpilot)
{
  sqlpilot->summaries_html = gtk_html_new();
  gtk_container_add(GTK_CONTAINER(sqlpilot->summaries_sw), sqlpilot->summaries_html);
  gtk_widget_show(sqlpilot->summaries_html);

  GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
  GtkTreeIter iter;
  GtkCellRenderer *renderer;

  gtk_list_store_append(store, &iter);
  gtk_list_store_set (store, &iter,
		      SUMMARIES_COL_FILENAME, "script/summary/experience",
		      SUMMARIES_COL_TITLE, "Experience",
		      -1);
  gtk_list_store_append(store, &iter);
  gtk_list_store_set (store, &iter,
		      SUMMARIES_COL_FILENAME, "script/summary/rest",
		      SUMMARIES_COL_TITLE, "Rest",
		      -1);

  gtk_combo_box_set_model(GTK_COMBO_BOX(sqlpilot->summaries_select_summary), GTK_TREE_MODEL(store));
  gtk_cell_layout_clear(GTK_CELL_LAYOUT(sqlpilot->summaries_select_summary));
  renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(sqlpilot->summaries_select_summary), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(sqlpilot->summaries_select_summary), renderer,
				 "text", SUMMARIES_COL_TITLE,
				 NULL);

  gtk_combo_box_set_active(GTK_COMBO_BOX(sqlpilot->summaries_select_summary), 0);

}
