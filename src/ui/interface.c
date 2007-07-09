
#include <ui/interface.h>
#include <ui/callbacks.h>
#include <gtk/gtk.h>
#include <libintl.h>

#include <hildon-widgets/hildon-file-chooser-dialog.h>
#include <hildon-widgets/hildon-note.h>
#include <hildon-widgets/hildon-font-selection-dialog.h>


/* Privates: */

static void create_menu( MainView *main );
static void create_toolbar( MainView *main );
void create_textarea( MainView *main );

/* Creates and initialises a main_view */
MainView* interface_main_view_new( AppData *data )
{
    /* Zero memory with g_new0 */
    MainView* result = g_new0( MainView, 1 );
    /* Create GTK vbox which will be our text area */
    GtkWidget *main_vbox = gtk_vbox_new( FALSE, 0 );
    result->clipboard = gtk_clipboard_get (GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_can_store(result->clipboard, NULL, 0);

    /* Initialize interface data */
    result->file_edited = FALSE;
    /* Store handle to app's data to view's data */
    result->data = data;
    /* Store handle to hildon_window to app's data */
    result->data->window = HILDON_WINDOW(hildon_window_new());
    /* Create all our necessary parts */
    create_textarea( result );
    create_toolbar( result ); 
    create_menu( result );
    /* Put GTK vbox to app's view */
    gtk_container_add( GTK_CONTAINER( result->data->window ), main_vbox );
    /* Put scrolledwindow to GTK vbox and show it */
    gtk_box_pack_start( GTK_BOX( main_vbox ), result->scrolledwindow, TRUE, TRUE, 0 );
    gtk_widget_show( main_vbox );
    /* Focus on textarea */
    gtk_widget_grab_focus( GTK_WIDGET(result->textview ));
    /* Initialize our textview's file (no file opened yet) */
    result->file_edited=FALSE;
    result->file_name=NULL;
    /* Return with new mainview handle */
    return result;
}

/* clean up the allocated memory */
void interface_main_view_destroy( MainView *main )
{
    g_free( main );
}

/* File chooser */
gchar* interface_file_chooser(MainView * mainview, GtkFileChooserAction action)
{
    GtkWidget *dialog;
    gchar* filename = NULL;

    dialog = hildon_file_chooser_dialog_new( GTK_WINDOW(mainview->data->program), action);
    gtk_widget_show_all (GTK_WIDGET(dialog));
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
    {
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    }
    gtk_widget_destroy (dialog);
    return filename;
}

/* Font chooser */
PangoFontDescription* interface_font_chooser( MainView * main )
{
    HildonFontSelectionDialog *dialog = NULL;

    PangoFontDescription *font = NULL;
    gint size;
    gboolean bold, italic;
    gchar *family = NULL;

    font = pango_font_description_new();

    /* create dialog */
    dialog = HILDON_FONT_SELECTION_DIALOG ( hildon_font_selection_dialog_new( NULL, NULL ) );

    gtk_widget_show_all (GTK_WIDGET(dialog));
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_OK)
    {
        g_object_get(G_OBJECT(dialog),
                    "family", &family,
                    "size", &size,
                    "bold", &bold,
                    "italic", &italic,
                    NULL);
        pango_font_description_set_family(font, family);
        pango_font_description_set_size(font, size * PANGO_SCALE);
        if (bold) {
            pango_font_description_set_weight(font, PANGO_WEIGHT_BOLD);
        } else {
            pango_font_description_set_weight(font, PANGO_WEIGHT_NORMAL);
        }
        if (italic) {
            pango_font_description_set_style(font, PANGO_STYLE_ITALIC);
        } else {
            pango_font_description_set_style(font, PANGO_STYLE_NORMAL);
        }
    }
    gtk_widget_destroy(GTK_WIDGET(dialog));

    return font;
}           

/* Save changes note */
gint interface_save_changes_note(MainView * main)
{
    HildonNote *hn = NULL;
    gint response = FALSE;

    g_assert(main != NULL && main->data->program != NULL);

    hn = HILDON_NOTE(hildon_note_new_confirmation_add_buttons
                     (GTK_WINDOW(main->data->program),
                      _("sqlpilot_save_changes_made"),
                      _("sqlpilot_yes"), CONFRESP_YES,
                      _("sqlpilot_no"), CONFRESP_NO,
                      NULL, NULL));
    response = gtk_dialog_run(GTK_DIALOG(hn));
    gtk_widget_destroy(GTK_WIDGET(hn));
    return response;
}

/* Error note */
void interface_error( SqlpilotError me, MainView *main )
{
    GtkWidget *dialog = NULL;
    gchar *msg = NULL;

    switch (me) {

    case MAEMOPAD_ERROR_INVALID_URI:
        msg = g_strdup( _("sqlpilot_invalid_uri") );
        break;

    case MAEMOPAD_ERROR_SAVE_FAILED:
        msg = g_strdup( _("sqlpilot_save_failed") );
        break;

    case MAEMOPAD_ERROR_OPEN_FAILED:
        msg = g_strdup( _("sqlpilot_open_failed") );
        break;

    case MAEMOPAD_NO_ERROR:
        return;

    default:
        g_assert_not_reached();
    }

    dialog = hildon_note_new_information( GTK_WINDOW( main->data->program ), msg );

    gtk_dialog_run( GTK_DIALOG( dialog ) );
    gtk_widget_destroy( dialog );
    g_free( msg );
}

/* Privates */

/* Create the menu items needed for the drop down menu */
static void create_menu( MainView *main )
{
    /* Create needed handles */
    GtkMenu *main_menu;
    GtkWidget *file_menu, *edit_menu;
    GtkWidget *separator = NULL;
    GtkWidget *help = NULL, *close = NULL;

    /* Create main menu and new menus for submenus in our drop down menu */
    main_menu = GTK_MENU( gtk_menu_new () );
    file_menu = gtk_menu_new ();
    edit_menu = gtk_menu_new ();

    /* Create the menu items */
    main->file_item = gtk_menu_item_new_with_label ( _("File") );
    main->new_item = gtk_menu_item_new_with_label ( _("New") );
    main->open_item = gtk_menu_item_new_with_label ( _("Open") );
    main->save_item = gtk_menu_item_new_with_label ( _("Save") );
    main->saveas_item = gtk_menu_item_new_with_label ( _("Save As...") );
    main->edit_item = gtk_menu_item_new_with_label ( _("Edit") );
    main->cut_item = gtk_menu_item_new_with_label ( _("Cut") );
    main->copy_item = gtk_menu_item_new_with_label ( _("Copy") );
    main->paste_item = gtk_menu_item_new_with_label ( _("Paste") );
    main->font_item = gtk_menu_item_new_with_label ( _("Font") );
    main->fullscreen_item = gtk_menu_item_new_with_label ( _("Full Screen") );
    separator = gtk_separator_menu_item_new();
    help = gtk_menu_item_new_with_label( _("Help") );
    close = gtk_menu_item_new_with_label( _("Close") );

    /* Add menu items to right menus */
    gtk_menu_append( main_menu, main->file_item );
    gtk_menu_append( file_menu, main->new_item );
    gtk_menu_append( file_menu, main->open_item );
    gtk_menu_append( file_menu, main->save_item );
    gtk_menu_append( file_menu, main->saveas_item );
    gtk_menu_append( main_menu, main->edit_item );
    gtk_menu_append( edit_menu, main->cut_item );
    gtk_menu_append( edit_menu, main->copy_item );
    gtk_menu_append( edit_menu, main->paste_item );
    gtk_menu_append( main_menu, main->font_item );
    gtk_menu_append( main_menu, main->fullscreen_item );
    gtk_menu_append( main_menu, separator );
    gtk_menu_append( main_menu, help );
    gtk_menu_append( main_menu, close );

    /* Add submenus to the right items */
    gtk_menu_item_set_submenu( GTK_MENU_ITEM (main->file_item), file_menu );
    gtk_menu_item_set_submenu( GTK_MENU_ITEM (main->edit_item), edit_menu );

    /* Attach the callback functions to the activate signal */
    g_signal_connect( G_OBJECT( main->new_item ), "activate", 
		      G_CALLBACK ( callback_file_new), main );
    g_signal_connect( G_OBJECT( main->open_item ), "activate", 
		      G_CALLBACK ( callback_file_open), main );
    g_signal_connect( G_OBJECT( main->save_item ), "activate", 
              G_CALLBACK ( callback_file_save), main );
    g_signal_connect( G_OBJECT( main->saveas_item ), "activate", 
		      G_CALLBACK ( callback_file_saveas), main );
    g_signal_connect( G_OBJECT( main->cut_item ), "activate", 
		      G_CALLBACK ( callback_edit_cut), main );
    g_signal_connect( G_OBJECT( main->copy_item ), "activate", 
		      G_CALLBACK( callback_edit_copy ), main );
    g_signal_connect( G_OBJECT( main->paste_item ), "activate", 
		      G_CALLBACK( callback_edit_paste ), main );
    g_signal_connect( G_OBJECT( main->font_item ), "activate", 
		      G_CALLBACK ( callback_font ), main );
    g_signal_connect( G_OBJECT( main->fullscreen_item ), "activate", 
		      G_CALLBACK ( callback_fullscreen ), main );
    g_signal_connect( G_OBJECT( help ), "activate", 
		      G_CALLBACK ( callback_help ), main );
    g_signal_connect( G_OBJECT( close ), "activate", gtk_main_quit, NULL );

    /* Add menu to HildonWindow */
    hildon_window_set_menu(main->data->window, main_menu);

    /* We need to show menu items */
    gtk_widget_show_all( GTK_WIDGET( main_menu ) );
}


/* Create toolbar to mainview */
static void create_toolbar ( MainView *main )
{
    /* Create new GTK toolbar */
    main->toolbar = gtk_toolbar_new ();

 	/* Set toolbar properties */
    gtk_toolbar_set_orientation( GTK_TOOLBAR(main->toolbar), GTK_ORIENTATION_HORIZONTAL);
    gtk_toolbar_set_style( GTK_TOOLBAR(main->toolbar), GTK_TOOLBAR_BOTH_HORIZ);

    /* Make menus and buttons to toolbar: */
    /* Create toolitems using defined items from stock */
    main->new_tb = gtk_tool_button_new_from_stock(GTK_STOCK_NEW);
    main->open_tb = gtk_tool_button_new_from_stock(GTK_STOCK_OPEN);
    main->save_tb = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE);
    main->saveas_tb = gtk_tool_button_new_from_stock(GTK_STOCK_SAVE_AS);
    main->separator_tb1 = gtk_separator_tool_item_new();
    main->cut_tb = gtk_tool_button_new_from_stock(GTK_STOCK_CUT);
    main->copy_tb = gtk_tool_button_new_from_stock(GTK_STOCK_COPY);
    main->paste_tb = gtk_tool_button_new_from_stock(GTK_STOCK_PASTE);
    main->separator_tb2 = gtk_separator_tool_item_new();
    main->font_tb = gtk_tool_button_new_from_stock( GTK_STOCK_SELECT_FONT );
    main->fullscreen_tb = gtk_tool_button_new_from_stock( GTK_STOCK_ZOOM_FIT );

    /* Insert items to toolbar */
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->new_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->open_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->save_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->saveas_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->separator_tb1, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->cut_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->copy_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->paste_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->separator_tb2, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->font_tb, -1);
    gtk_toolbar_insert ( GTK_TOOLBAR(main->toolbar), main->fullscreen_tb, -1);

    /* Connect signals to buttons */
    g_signal_connect(G_OBJECT(main->new_tb), "clicked",
                     G_CALLBACK(callback_file_new), main);
    g_signal_connect(G_OBJECT(main->open_tb), "clicked",
                     G_CALLBACK(callback_file_open), main);
    g_signal_connect(G_OBJECT(main->save_tb), "clicked",
                     G_CALLBACK(callback_file_save), main);
    g_signal_connect(G_OBJECT(main->saveas_tb), "clicked",
                     G_CALLBACK(callback_file_saveas), main);
    g_signal_connect(G_OBJECT(main->cut_tb), "clicked",
                     G_CALLBACK(callback_edit_cut), main);
    g_signal_connect(G_OBJECT(main->copy_tb), "clicked",
                     G_CALLBACK(callback_edit_copy), main);
    g_signal_connect(G_OBJECT(main->paste_tb), "clicked",
                     G_CALLBACK(callback_edit_paste), main);
    g_signal_connect(G_OBJECT(main->font_tb), "clicked",
                     G_CALLBACK(callback_font), main);
    g_signal_connect(G_OBJECT(main->fullscreen_tb), "clicked",
                     G_CALLBACK(callback_fullscreen), main);
	
    /* Add toolbar to the HildonWindow */
    hildon_window_add_toolbar(main->data->window, GTK_TOOLBAR(main->toolbar));

    /* Show toolbar */
    gtk_widget_show_all (GTK_WIDGET(main->toolbar));
    gtk_widget_show_all (GTK_WIDGET(main->data->window) );
}

/* Create the text area */
void create_textarea( MainView *main )
{
    /* Scrolled window */
    main->scrolledwindow = gtk_scrolled_window_new(NULL, NULL);
    gtk_widget_show(main->scrolledwindow);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(main->scrolledwindow),
                                   GTK_POLICY_AUTOMATIC,
                                   GTK_POLICY_AUTOMATIC);
    /* Text view */
    main->textview = gtk_text_view_new ();

    /* Some text view settings */
    gtk_text_view_set_editable ( GTK_TEXT_VIEW (main->textview), TRUE );
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW (main->textview), 10 );
    gtk_text_view_set_right_margin (GTK_TEXT_VIEW (main->textview), 10 );

    /* Get handle */
    main->buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (main->textview));

    /* Enable Rich Text Support */
    gtk_text_buffer_set_can_paste_rich_text ( main->buffer, TRUE );
    gtk_text_buffer_set_rich_text_format ( main->buffer, "RTF" );

    /* Put textview under scrolledwindow and show it*/
    gtk_container_add(GTK_CONTAINER(main->scrolledwindow), main->textview);
    gtk_widget_show(main->textview);

    /* Change default font throughout the widget */
    main->font_desc = pango_font_description_from_string ("Monospace Regular 22");
    gtk_widget_modify_font (main->textview, main->font_desc);

    /* Connect signals */
    g_signal_connect (G_OBJECT (main->buffer), "modified-changed",
                      G_CALLBACK(callback_buffer_modified), main);
    g_signal_connect (G_OBJECT (main->buffer), "changed",
                      G_CALLBACK(callback_buffer_modified), main);
}
