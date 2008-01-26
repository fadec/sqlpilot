#include "sqlpilot.h"

int main( int argc, char **argv )
{
	gtk_init( &argc, &argv );
	g_set_application_name ( _("Sql Pilot") );

	sqlpilot_init();

 	sqlpilot->interface = interface_create();
 	gtk_widget_show ( sqlpilot->interface->window );
 	gtk_main();
 	
	interface_destroy(sqlpilot->interface);

	sqlpilot_finalize();
	return 0;
}     

