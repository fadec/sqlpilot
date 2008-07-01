#include "summaries.h"
#include <gtkhtml/gtkhtml.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


static gchar *summaries_get_selected_script_filename(Sqlpilot *sqlpilot)
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

static void parameter_fwrite(Parameter *param, FILE *stream)
{
  if (param->type == PARAMETER_TYPE_DATE) {
    fprintf(stream, ": %s", gtk_entry_get_text(GTK_ENTRY(param->widget)));
  }
}

static void parameter_pane_fwrite(ParameterPane *ppane, FILE *stream)
{
  g_list_foreach(ppane->parameters, (GFunc)parameter_fwrite, (gpointer)stream);
}

void summaries_refresh(Sqlpilot *sqlpilot)
{
  gchar *script;
  gchar *sstdout, *sstderr;
  gchar *html_src;
  GError *error=NULL;
  int exit_code;
  gchar *argv[3];

  script = summaries_get_selected_script_filename(sqlpilot);
  argv[0] = script;
  argv[1] = sqlpilot->db_filename;
  argv[2] = NULL;

  parameter_pane_fwrite(sqlpilot->summaries_parameter_pane, stderr);

  if (spawn_script(NULL, argv, NULL, "", &sstdout, &sstderr, &exit_code, &error, NULL, NULL)) {
    html_src = sstdout;
  } else {
    html_src = error->message;
  }
  gtk_html_load_from_string(GTK_HTML(sqlpilot->summaries_html), html_src, -1);

  g_free(script);
  g_free(sstdout);
  g_free(sstderr);
}

static void summaries_html_view_init(Sqlpilot *sqlpilot)
{
  sqlpilot->summaries_html = gtk_html_new();
  gtk_container_add(GTK_CONTAINER(sqlpilot->summaries_sw), sqlpilot->summaries_html);
  gtk_widget_show(sqlpilot->summaries_html);
}

static void summaries_selector_init(Sqlpilot *sqlpilot)
{
  GtkListStore *store = gtk_list_store_new(2, G_TYPE_STRING, G_TYPE_STRING);
  GtkTreeIter iter;
  GtkCellRenderer *renderer;

  DIR *dp;
  struct dirent *dirp;
  struct stat statinfo;
  char filename[256];

  if ((dp = opendir(SUMMARIES_DIR)) == NULL) {
    fprintf(stderr, "Can't open summary dir\n");
    exit(1);
  }
  while ((dirp = readdir(dp)) != NULL) {
    snprintf(filename, sizeof(filename), "%s/%s", SUMMARIES_DIR, dirp->d_name);
    stat(filename, &statinfo);
    if (S_ISREG(statinfo.st_mode) || S_ISLNK(statinfo.st_mode)) {
      gtk_list_store_append(store, &iter);
      gtk_list_store_set (store, &iter,
			  SUMMARIES_COL_FILENAME, filename,
			  SUMMARIES_COL_TITLE, dirp->d_name,
			  -1);
    }
  }
  closedir(dp);

  gtk_combo_box_set_model(GTK_COMBO_BOX(sqlpilot->summaries_select_summary), GTK_TREE_MODEL(store));
  gtk_cell_layout_clear(GTK_CELL_LAYOUT(sqlpilot->summaries_select_summary));
  renderer = gtk_cell_renderer_text_new();
  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(sqlpilot->summaries_select_summary), renderer, TRUE);
  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(sqlpilot->summaries_select_summary), renderer,
				 "text", SUMMARIES_COL_TITLE,
				 NULL);

  gtk_combo_box_set_active(GTK_COMBO_BOX(sqlpilot->summaries_select_summary), 0);
}

void summaries_init(Sqlpilot *sqlpilot)
{
  summaries_html_view_init(sqlpilot);
  summaries_selector_init(sqlpilot);
}

/* Leaves widget intact */
static void parameter_destroy(Parameter *param)
{
  g_free(param->name);
  g_slice_free(Parameter, param);
}

static Parameter *parameter_new(ParameterType type, const gchar *name, GtkWidget *widget)
{
  Parameter *param;
  param = g_slice_new0(Parameter);
  param->widget = widget;
  param->type = type;
  param->name = g_strdup(name);
  return param;
}

static void parameter_pane_append_date(ParameterPane *ppane, const gchar *name)
{
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *entry;

  vbox = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new(name);
  entry = gtk_entry_new();

  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(ppane->container), vbox, FALSE, TRUE, 0);
  gtk_widget_show_all(vbox);

  ppane->parameters = g_list_append(ppane->parameters, parameter_new(PARAMETER_TYPE_DATE, name, entry));
}

static void parameter_pane_interpret_spec(ParameterPane *ppane, const char *line)
{
  char
    *fieldname=NULL,
    *fieldtype=NULL;
  if (sscanf(line, "%a[^:]:%as", &fieldname, &fieldtype) == 2) {
    if (!strcmp(fieldtype, "date")) {
      parameter_pane_append_date(ppane, fieldname);
    }
  }
  g_free(fieldname);
  g_free(fieldtype);
}

static void parameter_pane_clear(ParameterPane *ppane)
{
  gtk_container_foreach(GTK_CONTAINER(ppane->container), (GtkCallback)gtk_widget_destroy, NULL);
  g_list_foreach(ppane->parameters, (GFunc)parameter_destroy, NULL);
  g_list_free(ppane->parameters);
  ppane->parameters = NULL;
}

void parameter_pane_rebuild(ParameterPane *ppane, const char *script_filename)
{
  GPid pid;
  int fdout;
  FILE *fout;
  GError *error=NULL;
  gchar *argv[] = {(gchar*)script_filename, NULL};
  char line[256];

  parameter_pane_clear(ppane);

  if (g_spawn_async_with_pipes(NULL, argv, NULL, G_SPAWN_STDERR_TO_DEV_NULL, NULL, NULL, &pid, NULL, &fdout, NULL, &error)) {
    if (!(fout = fdopen(fdout, "rb"))) {
      fprintf(stderr, "fdopen failed\n");
      exit(1);
    }
    while (fgets(line, sizeof(line), fout)) {
      parameter_pane_interpret_spec(ppane, line);
    }
  }
  
  fclose(fout);
  g_spawn_close_pid(pid);
}

void summaries_rebuild_parameters(Sqlpilot *sqlpilot)
{
  gchar *script;

  script = summaries_get_selected_script_filename(sqlpilot);
  parameter_pane_rebuild(sqlpilot->summaries_parameter_pane, script);
  g_free(script);
  
}
