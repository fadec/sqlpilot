#include "sqlpilot.h"

int main( int argc, char **argv )
{
	gtk_init( &argc, &argv );
	g_set_application_name ( _("Sql Pilot") );

	sqlpilot_init();

 	gtk_widget_show ( sqlpilot->window );
 	gtk_main();
 	
	sqlpilot_finalize();
	return 0;
}     

