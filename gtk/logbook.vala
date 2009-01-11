
using Gtk;

namespace SqlPilot {

	public class Logbook : Pane {
		private NiceDB db;

		private FlightEditor flight_editor;

		public Logbook ( string filename ) throws NiceDBError {
			base ( "data/ui/logbook.xml" );
			db = new NiceDB ( filename );
			add_pages ();
		}

		private void add_pages () {
			flight_editor = new FlightEditor ( this );
			add_child ( "flights", flight_editor );
		}
	}
}