
#include <ui/callbacks.h>
#include <ui/interface.h>
#include <gtk/gtk.h>
#include <libintl.h>
/* strlen needed from string.h */
#include <string.h>
/* Gnome VFS for file i/o */
#include <libgnomevfs/gnome-vfs.h>

/* Privates: */
void read_file_to_buffer ( MainView* mainview );
void write_buffer_to_file ( MainView* mainview );

/* cut */
void callback_edit_cut( GtkAction * action, gpointer data )
{
    /* connect pointer to our MainView struct */
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* do cut */
    gtk_text_buffer_cut_clipboard(GTK_TEXT_BUFFER(mainview->buffer), mainview->clipboard, TRUE);
}

/* copy */
void callback_edit_copy( GtkAction * action, gpointer data )
{
    /* connect pointer to our MainView struct */
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* do copy */
    gtk_text_buffer_copy_clipboard (GTK_TEXT_BUFFER(mainview->buffer), mainview->clipboard);
}

/* paste */
void callback_edit_paste( GtkAction * action, gpointer data )
{
    /* connect pointer to our MainView struct */
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* do paste */
    gtk_text_buffer_paste_clipboard (GTK_TEXT_BUFFER (mainview->buffer), mainview->clipboard, NULL, TRUE);
}

/* new */
void callback_file_new(GtkAction * action, gpointer data)
{
    gint answer;
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* save changes note if file is edited */
    if( mainview->file_edited ) {
        answer = interface_save_changes_note( mainview );
        if( answer == CONFRESP_YES ) {
            if( mainview->file_name == NULL ) {
                mainview->file_name = interface_file_chooser ( mainview, GTK_FILE_CHOOSER_ACTION_SAVE );
            }
            write_buffer_to_file ( mainview );
        }
    }
    /* clear buffer, filename and free buffer text */
    gtk_text_buffer_set_text ( GTK_TEXT_BUFFER (mainview->buffer), "", -1 );
    mainview->file_name = NULL;
    mainview->file_edited = FALSE;
}

/* open */
void callback_file_open(GtkAction * action, gpointer data)
{
    gint answer;
    gchar* filename = NULL;
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* save changes note if file is edited */
    if( mainview->file_edited ) {
        answer = interface_save_changes_note( mainview );
        if( answer == CONFRESP_YES ) {
            /* check is we had a new file */
            if( mainview->file_name == NULL ) {
                mainview->file_name = interface_file_chooser ( mainview, GTK_FILE_CHOOSER_ACTION_SAVE );
            }
            write_buffer_to_file ( mainview );
        }
    }

    /* open new file */
    filename = interface_file_chooser ( mainview, GTK_FILE_CHOOSER_ACTION_OPEN );

    /* if we got a file name from chooser -> open file */
    if( filename != NULL ) {
        mainview->file_name = filename;
        read_file_to_buffer ( mainview );
        mainview->file_edited = FALSE;
    }
}

/* save */
void callback_file_save(GtkAction * action, gpointer data)
{
    gchar* filename = NULL;
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* check is we had a new file */
    if( mainview->file_name != NULL ) {
        write_buffer_to_file ( mainview );
    } else { 
        filename = interface_file_chooser ( mainview, GTK_FILE_CHOOSER_ACTION_SAVE );
        /* if we got a file name from chooser -> save file */
        if( filename != NULL ) {
            mainview->file_name = filename;
            write_buffer_to_file ( mainview );
            mainview->file_edited = FALSE;
        }
    }
}

/* save as... */
void callback_file_saveas(GtkAction * action, gpointer data)
{
    gchar* filename = NULL;
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    filename = interface_file_chooser ( mainview, GTK_FILE_CHOOSER_ACTION_SAVE );

    /* if we got a file name from chooser -> save file */
    if( filename != NULL ) {
        mainview->file_name = filename;
        write_buffer_to_file ( mainview );
        mainview->file_edited = FALSE;
    }
}

/* font */
void callback_font( GtkAction * action, gpointer data )
{
    PangoFontDescription* new_font = NULL;
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    new_font = interface_font_chooser( mainview );

    /* if we got a new font from chooser -> change font */
    if( new_font != NULL ) {
        mainview->font_desc = new_font;
        gtk_widget_modify_font ( mainview->textview, mainview->font_desc );
    }    
}

/* fullscreen */
void callback_fullscreen( GtkAction * action, gpointer data )
{
    /* connect pointer to our MainView struct */
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* toggle fullscreen on<->off */
    mainview->fullscreen = !mainview->fullscreen;

    if (mainview->fullscreen) {
        gtk_window_fullscreen(GTK_WINDOW(mainview->data->window));
    } else {
        gtk_window_unfullscreen(GTK_WINDOW(mainview->data->window));
    }
}

/* help */
void callback_help( GtkAction * action, gpointer data )
{
    osso_return_t retval;

    /* connect pointer to our MainView struct */
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    retval = ossohelp_show(
      mainview->data->osso, /* osso_context */
      HELP_TOPIC_ID,        /* topic id */
      OSSO_HELP_SHOW_DIALOG);
}

/* buffer edited */
void callback_buffer_modified ( GtkAction * action, gpointer data )
{
    /* connect pointer to our MainView struct */
    MainView *mainview = NULL;
    mainview = ( MainView * ) data;
    g_assert(mainview != NULL && mainview->data != NULL );

    /* change state */
    mainview->file_edited = TRUE;
}

/* Privates */

/* read file */
void read_file_to_buffer ( MainView* mainview )
{
    GnomeVFSResult vfs_result;
    GnomeVFSHandle *handle = NULL;
    GnomeVFSFileSize in_bytes;
    GnomeVFSFileInfo finfo;
    gchar *temp_buffer = NULL;
    g_assert( mainview != NULL );

    /* try to get file info */
    vfs_result = gnome_vfs_get_file_info(mainview->file_name, &finfo, GNOME_VFS_FILE_INFO_DEFAULT);
    if ( vfs_result != GNOME_VFS_OK ) {
        interface_error( MAEMOPAD_ERROR_OPEN_FAILED, mainview );
        return;
    }
    /* try to create handle to file */
    vfs_result = gnome_vfs_open(&handle, mainview->file_name, GNOME_VFS_OPEN_READ);
    if ( vfs_result != GNOME_VFS_OK ) {
        interface_error( MAEMOPAD_ERROR_OPEN_FAILED, mainview );
        return;
    }

    /* allocate memory for temp_buffer */
    temp_buffer = g_malloc(finfo.size + 1);
    memset(temp_buffer, 0, finfo.size + 1);
    
    /* read from file to buffer */
    gnome_vfs_read(handle, temp_buffer, finfo.size, &in_bytes);

    /* set text to screen */
    gtk_text_buffer_set_text( GTK_TEXT_BUFFER (mainview->buffer), temp_buffer, -1);

    /* free temp, close file and return */
    g_free(temp_buffer);
    gnome_vfs_close(handle);
}



/* write to file */
void write_buffer_to_file ( MainView* mainview )
{
    GnomeVFSResult vfs_result;
    GnomeVFSHandle *handle = NULL;
    GnomeVFSFileSize out_bytes;
    gchar *temp_buffer = NULL;
    GtkTextIter start, end;
    g_assert( mainview != NULL );

    /* try to create handle to file */
    vfs_result = gnome_vfs_create(&handle, mainview->file_name, GNOME_VFS_OPEN_WRITE, 0, 0600);
    if ( vfs_result != GNOME_VFS_OK ) {
        interface_error( MAEMOPAD_ERROR_SAVE_FAILED, mainview );
        return;
    }

    /* find start and end of text */
    gtk_text_buffer_get_bounds( GTK_TEXT_BUFFER (mainview->buffer), &start, &end);

    /* copy all text from screen to temp_buffer */
    temp_buffer = gtk_text_buffer_get_slice( GTK_TEXT_BUFFER (mainview->buffer), &start, &end, TRUE);

    /* write text to file */
    gnome_vfs_write(handle, temp_buffer, strlen(temp_buffer), &out_bytes);

    /* free temp, close file and return */
    g_free(temp_buffer);
    gnome_vfs_close(handle);
}
