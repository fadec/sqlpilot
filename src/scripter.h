#ifndef SCRIPTER_H
#define SCRIPTER_H

#include "sqlpilot.h"

#define SCRIPTER_ARG_BUF 4096
#define SCRIPTER_FILENAME_BUF 256

typedef enum {
  PARAMETER_TYPE_DATE
} ScripterParameterType;

struct Scripter {
  GtkComboBox *script_selector;
  GtkBox *parameters_box;
  char *gui_query;
  GList *parameters;
  char filename_buf[SCRIPTER_FILENAME_BUF];
  char arg_buf[SCRIPTER_ARG_BUF];
};

typedef struct ScripterParameter {
  ScripterParameterType type;
  GtkWidget *widget;
  char *name;
  char *flag;
} ScripterParameter;

void scripter_init(Scripter *ss, GtkComboBox *script_selector, GtkBox *parameters_box, const char *gui_query);
void scripter_merge_script_dir(Scripter *ss, const char *dirname);
void scripter_read_parameters(Scripter *ppane, char **return_values);
gchar *scripter_get_script_filename(Scripter *ss);
void scripter_get_parameter_values(Scripter *ss, char **return_values);
#endif
