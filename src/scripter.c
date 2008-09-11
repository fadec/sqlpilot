
#include "scripter.h"

static int scripter_parameter_snap_value(ScripterParameter *param, char *buf, int bufn)
{
  int flag_width=0, value_width=0;
  *buf = '\0';
  if (param->type == PARAMETER_TYPE_DATE) {
    if (param->flag) { flag_width = snprintf(buf, bufn, "%s", param->flag); }
    value_width = snprintf(buf+flag_width, bufn-flag_width, "%s", gtk_entry_get_text(GTK_ENTRY(param->widget)));
  } else if (param->type == PARAMETER_TYPE_TOGGLE) {
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(param->widget))) {
      if (param->flag) { flag_width = snprintf(buf, bufn, "%s", param->flag); }
    }
  }
  return flag_width + value_width;
}

/* Leaves widget intact */
static void scripter_parameter_destroy(ScripterParameter *param)
{
  if (GTK_IS_WIDGET(param->widget)) { gtk_widget_destroy(param->widget); }
  g_free(param->name);
  g_free(param->flag);
  g_slice_free(ScripterParameter, param);
}

static ScripterParameter *scripter_parameter_new(ScripterParameterType type, const gchar *name, GtkWidget *widget, const gchar *flag)
{
  ScripterParameter *param;
  param = g_slice_new0(ScripterParameter);
  param->widget = widget;
  param->type = type;
  param->name = g_strdup(name);
  param->flag = g_strdup(flag);;
  return param;
}

static void scripter_append_date(Scripter *ss, const gchar *name, gchar *flag)
{
  GtkWidget *vbox;
  GtkWidget *label;
  GtkWidget *entry;

  vbox = gtk_vbox_new(FALSE, 0);
  label = gtk_label_new(name);
  entry = gtk_entry_new();

  gtk_box_pack_start(GTK_BOX(vbox), label, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, TRUE, 0);
  gtk_box_pack_start(GTK_BOX(ss->parameters_box), vbox, FALSE, TRUE, 0);
  g_signal_connect(entry, "focus-out-event", (GtkSignalFunc)entry_format_date_on_focus_out, entry);
  gtk_widget_show_all(vbox);

  ss->parameters = g_list_append(ss->parameters, scripter_parameter_new(PARAMETER_TYPE_DATE, name, entry, flag));
}

static void scripter_append_toggle(Scripter *ss, const gchar *name, gchar *flag)
{
  GtkWidget *cbutton = gtk_check_button_new_with_label(name);
  gtk_box_pack_start(GTK_BOX(ss->parameters_box), cbutton, FALSE, TRUE, 0);
  gtk_widget_show_all(cbutton);
  ss->parameters = g_list_append(ss->parameters, scripter_parameter_new(PARAMETER_TYPE_TOGGLE, name, cbutton, flag));
}

static void scripter_interpret_parameter_spec(Scripter *ss, const char *line)
{
  char
    *fieldname=NULL,
    *fieldtype=NULL,
    *flag=NULL;
  if (sscanf(line, "%a[^:]:%as%as", &fieldname, &fieldtype, &flag) >= 2) {
    if (!strncmp(fieldtype, "date", 4)) {
      scripter_append_date(ss, fieldname, NULL);
    } else if (!strncmp(fieldtype, "toggle", 6)) {
      scripter_append_toggle(ss, fieldname, flag);
    }
  }
  g_free(flag);
  g_free(fieldname);
  g_free(fieldtype);
}

static void scripter_parameters_clear(Scripter *ss)
{
  gtk_container_foreach(GTK_CONTAINER(ss->parameters_box), (GtkCallback)gtk_widget_destroy, NULL);
  g_list_foreach(ss->parameters, (GFunc)scripter_parameter_destroy, NULL);
  g_list_free(ss->parameters);
  ss->parameters = NULL;
}

static void scripter_parameters_rebuild(Scripter *ss)
{
  GPid pid;
  int fdout;
  FILE *fout;
  GError *error=NULL;
  const gchar *argv[] = {filename_combo_box_get_current_full_filename(ss->script_selector), ss->gui_query, NULL};
  char line[256];

  scripter_parameters_clear(ss);

  if (g_spawn_async_with_pipes(NULL, (gchar**)argv, NULL, G_SPAWN_STDERR_TO_DEV_NULL, NULL, NULL, &pid, NULL, &fdout, NULL, &error)) {
    if (!(fout = fdopen(fdout, "rb"))) {
      fprintf(stderr, "fdopen failed\n");
      exit(1);
    }
    while (fgets(line, sizeof(line), fout)) {
      scripter_interpret_parameter_spec(ss, line);
    }
    fclose(fout);
    g_spawn_close_pid(pid);
  } else {
    fprintf(stderr, "Failed to execute script\n");
  }
}

static void scripter_script_selector_on_change(GtkComboBox *cbox, Scripter *ss)
{
  scripter_parameters_rebuild(ss);
}

void scripter_init(Scripter *ss, GtkComboBox *script_selector, GtkBox *parameters_box, const char *gui_query)
{
  ss->script_selector = script_selector;
  ss->parameters_box = parameters_box;
  ss->gui_query = gui_query;
  filename_combo_box_build_model(GTK_COMBO_BOX(ss->script_selector));
  g_signal_connect (G_OBJECT (script_selector), "changed",
		    G_CALLBACK (scripter_script_selector_on_change),
		    ss);
}

void scripter_merge_script_dir(Scripter *ss, const char *dirname)
{
  filename_combo_box_merge_dir(GTK_COMBO_BOX(ss->script_selector), dirname);
}

gchar *scripter_get_script_filename(Scripter *ss)
{
  char *filename;
  filename = filename_combo_box_get_current_full_filename(ss->script_selector);
  ss->filename_buf[SCRIPTER_FILENAME_BUF-1] = 0;
  strncpy(ss->filename_buf, filename, SCRIPTER_FILENAME_BUF);
  assert(ss->filename_buf[SCRIPTER_FILENAME_BUF-1] == 0);
  return ss->filename_buf;
}

void scripter_get_parameter_values(Scripter *ss, char **return_values)
{
  GList *node=ss->parameters;
  ScripterParameter *param;
  int buf_offset=0,param_width=0, i=0;

  while (node) {
    param = (ScripterParameter*)(node->data);
    return_values[i] = ss->arg_buf + buf_offset;
    param_width = scripter_parameter_snap_value(param, ss->arg_buf + buf_offset, SCRIPTER_ARG_BUF - buf_offset);
    //    if (return_values[i]) fprintf(stderr, "%s\n", return_values[i]);
    if (param_width) {
      buf_offset += param_width + 1;
      i++;
    }
    node = node->next;
  }
  return_values[i] = NULL;
}
