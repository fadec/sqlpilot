
#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <ui/interface.h>
#include <gtk/gtk.h>
#include <appdata.h>
#include <osso-helplib.h>

#define HELP_TOPIC_ID "Example_SqlPilot_Content"

/* edit-> cut/copy/paste */
void callback_edit_cut ( GtkAction * action, gpointer data );
void callback_edit_copy ( GtkAction * action, gpointer data );
void callback_edit_paste ( GtkAction * action, gpointer data );
/* file-> new/open/save */
void callback_file_new ( GtkAction * action, gpointer data );
void callback_file_open ( GtkAction * action, gpointer data );
void callback_file_save ( GtkAction * action, gpointer data );
void callback_file_saveas ( GtkAction * action, gpointer data );
/* font */
void callback_font ( GtkAction * action, gpointer data );
/* fullscreen */
void callback_fullscreen ( GtkAction * action, gpointer data );
/* help */
void callback_help ( GtkAction * action, gpointer data );

/* buffer modified */
void callback_buffer_modified ( GtkAction * action, gpointer data );

#endif
