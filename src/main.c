
#include <ui/interface.h>
#include <appdata.h>

#include <gtk/gtkmain.h>

#include <hildon-widgets/hildon-program.h>

#include <libintl.h>
#include <locale.h>
#include <libosso.h>

#define _(String) gettext(String)

#include <config.h>

/*
 * create our application specific data
 * This consists of allocating some space
 * for the AppData structure, and also
 * creating the list store that will act
 * as a model for the contact list.
 * The other elements of the AppData structure
 * are implicitly set by the call to g_new0
 * which zeroes the memory of the AppData, thereby
 * setting create_new to FALSE, and iter to NULL.
 *
 */

static AppData *create_data ()
{
    AppData *result = g_new0( AppData, 1 );

    result->osso = osso_initialize ( PACKAGE, VERSION, TRUE, NULL );
    g_assert ( result->osso );
    return result;
}

/* Here we clean up all the data that we allocated */
static void destroy_data ( AppData *data )
{
    g_free ( data );
}

int main( int argc, char* argv[] )
{
    AppData* data; 
    HildonProgram* program; 
    MainView* main_view;

    /* Initialise the locale stuff */
    setlocale ( LC_ALL, "" );
    bindtextdomain ( GETTEXT_PACKAGE, LOCALEDIR );
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain ( GETTEXT_PACKAGE );

    /* Init the gtk - must be called before any hildon stuff */
    gtk_init( &argc, &argv );

    /* Create the hildon application and setup the title */
    program = HILDON_PROGRAM ( hildon_program_get_instance () );
    g_set_application_name ( _("SqlPilot") );

    /* Create the data and views for our application */
    data = create_data ();
    data->program = program;
    main_view = interface_main_view_new ( data );
    hildon_program_add_window( data->program, data->window );
    
    gtk_signal_connect( G_OBJECT(data->window), "delete_event", gtk_main_quit, NULL );    
    
    /* Begin the main app */
    gtk_widget_show ( GTK_WIDGET ( program ) );
    gtk_main();

    /* Clean up */
    interface_main_view_destroy ( main_view );
    destroy_data ( data );

    return 0;
}     

