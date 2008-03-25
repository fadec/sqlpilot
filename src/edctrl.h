#ifndef EDCTRL_H
#define EDCTRL_H

#include <gtk/gtk.h>
#include "db/db.h"
#define BUF_ED_TODEL 64

/* Editing states for view */
typedef enum {
  EDSTATE_EMPTY,
  EDSTATE_SELECTED,
  EDSTATE_MODIFIED,
  EDSTATE_DELETEARMED
} Edstate;

typedef struct Edctrl Edctrl;
struct Edctrl {
  Edstate edstate;
  GtkWidget *new_btn;
  GtkWidget *save_btn;
  GtkWidget *armdel_btn;
  GtkWidget *todel_lbl;
  GtkWidget *del_btn;
  GtkTreeSelection *selection;
  DBStatement *delete_stmt;
  DBStatement *select_by_id_stmt;
  void (*load_selection)(void *data);
  void *load_selection_data;
  int (*selection_show)(GtkTreeSelection *, char *, size_t);
  int (*can_delete)(GtkTreeSelection *);
  void (*after_change)(void *data);
  void *after_change_data;
  DBint64 (*save)(const char *id, void *data);
  void *save_data;
};

void edctrl_set_empty(Edctrl *ec);
void edctrl_set_selected(Edctrl *ec);
void edctrl_set_modified(Edctrl *ec);
void edctrl_set_deletearmed(Edctrl *ec);

void edctrl_selection_changed(Edctrl *ec);
void edctrl_new_btn_clicked(Edctrl *ec);
void edctrl_save_btn_clicked(Edctrl *ec);
void edctrl_del_btn_clicked(Edctrl *ec);

void edctrl_register_load_selection(Edctrl *ec, void func(void *), void *data);
void edctrl_register_save(Edctrl *ec, DBint64 func(const char *, void *), void *data);
void edctrl_register_after_change(Edctrl *ec, void (void *), void *);

void edctrl_armdel_btn_toggled(Edctrl *ec);

#endif
