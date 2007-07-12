
#include <gtk/gtk.h>

#include <libintl.h>
#include <locale.h>

#define _(String) gettext(String)

#include <config.h>

#include <ui/interface.h>

int main( int argc, char* argv[] )
{
    GtkWidget* main_window;
    
    setlocale ( LC_ALL, "" );
    bindtextdomain ( GETTEXT_PACKAGE, LOCALEDIR );
    bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
    textdomain ( GETTEXT_PACKAGE );

    gtk_init( &argc, &argv );

    g_set_application_name ( _("Sql Pilot") );

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_signal_connect( G_OBJECT(main_window), "delete_event", gtk_main_quit, NULL );    
    
    /* Begin the main app */
    gtk_widget_show ( main_window );
    gtk_main();

    return 0;
}     

