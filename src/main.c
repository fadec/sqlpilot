
#include <gtk/gtk.h>

// For future localization
#define _(String) String

#include "ui/interface.h"

int main( int argc, char* argv[] )
{
    GtkWidget* main_window;
    
    gtk_init( &argc, &argv );

    g_set_application_name ( _("Sql Pilot") );

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect( G_OBJECT(main_window), "delete_event", gtk_main_quit, NULL );    
    
    /* Begin the main app */
    gtk_widget_show ( main_window );
    gtk_main();

    return 0;
}     

