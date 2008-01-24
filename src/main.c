
#include <gtk/gtk.h>

#define _(String) String

#include <sqlpilot.h>
//#include <ui/interface.h>
//#include <db/db.h>

int main( int argc, char **argv )
{
	MainView* main_view;
	Sqlpilot *sqlpilot;

	gtk_init( &argc, &argv );
	g_set_application_name ( _("Sql Pilot") );

	sqlpilot_init(&sqlpilot);

/* 	main_view = interface_main_view_create(); */
/* 	gtk_widget_show ( main_view->window ); */
/* 	gtk_main(); */
/* 	interface_main_view_destroy(main_view); */


	return 0;
}     

