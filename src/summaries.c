#include <gtkhtml/gtkhtml.h>

#include "sqlpilot.h"
#include "summaries.h"
#include "logbook.h"

static gchar *summaries_get_selected_script_filename(Logbook *logbook)
{
  GtkTreeIter iter;
  gchar *filename=NULL;

  if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(logbook->summaries_select_summary), &iter)) {
    gtk_tree_model_get(gtk_combo_box_get_model(GTK_COMBO_BOX(logbook->summaries_select_summary)), &iter,
		       SUMMARIES_COL_FILENAME, &filename,
		       -1);
  }

  return filename;
}

static void parameter_value_tostr(Parameter *param, char *buf, int bufn)
{
  if (param->type == PARAMETER_TYPE_DATE) {
    snprintf(buf, bufn, "%s", gtk_entry_get_text(GTK_ENTRY(param->widget)));
  }
}

static void parameter_pane_snapshot_values(ParameterPane *ppane)
{
  GList *node=ppane->parameters;
  Parameter *param;
  int n;

  for(n=0; n<PARAMETER_MAX; n++) {
    if (node) {
      param = (Parameter*)(node->data);
      parameter_value_tostr(param, ppane->values[n], PARAMETER_VALUE_BUFSIZE);
      node = node->next;
    } else {
      ppane->values[n][0] = '\0';
    }
  }
}

void summaries_refresh(Logbook *logbook)
{
  gchar *script;
  gchar *sstdout, *sstderr;
  gchar *html_src;
  GError *error=NULL;
  int exit_code;
  gchar *argv[PARAMETER_MAX+3]; /* +3 for script command, database arg, and NULL terminator */
  int i;

  script = summaries_get_selected_script_filename(logbook);
  argv[0] = script;
  argv[1] = logbook->db_filename;

  parameter_pane_snapshot_values(logbook->summaries_parameter_pane);
  for (i=0; i<PARAMETER_MAX; i++) {
    argv[i+2] = logbook->summaries_parameter_pane->values[i];
  }
  argv[i++] = NULL;

  if (spawn_script(NULL, argv, NULL, "", &sstdout, &sstderr, &exit_code, &error, NULL, NULL)) {
    html_src = sstdout;
  } else {
    html_src = error->message;
  }
  gtk_html_load_from_string(GTK_HTML(logbook->summaries_html), html_src, -1);

  g_free(script);
  g_free(sstdout);
  g_free(sstderr);
}

static void summaries_html_view_init(Logbook *logbook)
{
  logbook->summaries_html = gtk_html_new();
  gtk_container_add(GTK_CONTAINER(logbook->summaries_sw), logbook->summaries_html);
  gtk_widget_show(logbook->summaries_html);
}

static void summaries_selector_init(Logbook *logbook)
{
  filename_combo_box_build_model(GTK_COMBO_BOX(logbook->summaries_select_summary));
  filename_combo_box_merge_dir(GTK_COMBO_BOX(logbook->summaries_select_summary), SUMMARIES_DIR);
  gtk_combo_box_set_active(GTK_COMBO_BOX(logbook->summaries_select_summary), 0);
}

void summaries_init(Logbook *logbook)
{
  summaries_html_view_init(logbook);
  summaries_selector_init(logbook);
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
  g_signal_connect(entry, "focus-out-event", (GtkSignalFunc)entry_format_date_on_focus_out, entry);
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

void summaries_rebuild_parameters(Logbook *logbook)
{
  gchar *script;

  script = summaries_get_selected_script_filename(logbook);
  parameter_pane_rebuild(logbook->summaries_parameter_pane, script);
  g_free(script);
  
}
