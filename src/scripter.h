#ifndef SCRIPTER_H
#define SCRIPTER_H

#include "sqlpilot.h"

#define PARAMETER_MAX 30
#define PARAMETER_VALUE_BUFSIZE 256

typedef enum {
  PARAMETER_TYPE_DATE
} ParameterType;

struct ParameterPane {
  GList *parameters;
  GtkWidget *container;
  char values[PARAMETER_MAX][PARAMETER_VALUE_BUFSIZE];
};

typedef struct Parameter {
  ParameterType type;
  GtkWidget *widget;
  char *name;
} Parameter;

#endif
