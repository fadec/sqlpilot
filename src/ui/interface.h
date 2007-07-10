
#ifndef INTERFACE_H
#define INTERFACE_H

#include <gtk/gtk.h>
#include <hildon-widgets/hildon-program.h>
#include <appdata.h>

#define _(String) gettext(String)

/* confirmation responses */
#define CONFRESP_YES 1
#define CONFRESP_NO 2
#define CONFRESP_OK 3
#define CONFRESP_CANCEL 4
#define CONFRESP_ERROR 5

#define MAIN_VIEW_NAME "MainView"

typedef enum {
    SQLPILOT_NO_ERROR = 0,
    SQLPILOT_ERROR_INVALID_URI,
    SQLPILOT_ERROR_SAVE_FAILED,
    SQLPILOT_ERROR_OPEN_FAILED
} SqlpilotError;

/* Struct to include view's information */
typedef struct _MainView MainView;
struct _MainView
{
    /* Handle to app's data */
    AppData *data;

    /* Fullscreen mode is on (TRUE) or off (FALSE) */
    gboolean fullscreen;

    /* Items for menu */
    GtkWidget *file_item;
    GtkWidget *new_item;
    GtkWidget *open_item;
    GtkWidget *save_item;
    GtkWidget *saveas_item;
    GtkWidget *edit_item;
    GtkWidget *cut_item;
    GtkWidget *copy_item;
    GtkWidget *paste_item;
    GtkWidget *font_item;
    GtkWidget *fullscreen_item;

    /* Toolbar */
    GtkWidget* toolbar;
    GtkWidget* iconw;
    GtkToolItem* new_tb;
    GtkToolItem* open_tb;
    GtkToolItem* save_tb;
    GtkToolItem* saveas_tb;
    GtkToolItem* separator_tb1;
    GtkToolItem* cut_tb;
    GtkToolItem* copy_tb;
    GtkToolItem* paste_tb;
    GtkToolItem* separator_tb2;
    GtkToolItem* font_tb;
    GtkToolItem* fullscreen_tb;

    /* Textview related */
    GtkWidget* scrolledwindow;   /* textview is under this widget */
    GtkWidget* textview;         /* widget that shows the text */
    GtkTextBuffer* buffer;       /* buffer that contains the text */
    GtkClipboard* clipboard;     /* clipboard for copy/paste */
   
    PangoFontDescription* font_desc;    /* font used in textview */

    gboolean file_edited;     /* tells is our file on view edited */
    gchar* file_name;         /* directory/file under editing */
};

/* Publics: */
MainView* interface_main_view_new( AppData* data );
void interface_main_view_destroy( MainView* main );
char* interface_file_chooser( MainView* main, GtkFileChooserAction action );
PangoFontDescription* interface_font_chooser( MainView * main );
gint interface_save_changes_note( MainView* main );
void interface_error( SqlpilotError me, MainView *main );

#endif
