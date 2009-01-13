
using Gtk;
using Sqlite;

namespace SqlPilotGtk {

	public class Logbook : Pane {
		private SqlPilot.Logbook logbook;

		private FlightEditor flight_editor;

		public Logbook ( string filename ) {
			base ( "data/ui/logbook.xml" );
			logbook = new SqlPilot.Logbook ();
			logbook.open ( filename );
			add_pages ();
		}

		private void add_pages () {
			flight_editor = new FlightEditor ( this );
			add_child ( "flights", flight_editor );
		}
	}
}