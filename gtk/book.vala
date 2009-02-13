
using Gtk;
using Sqlite;
using SqlPilot;
namespace SqlPilotGtk {
	// A Book is the gui wrapper for the Logbook data object
	public class Book : Pane {
		public Logbook logbook;

		private FlightEditor flight_editor;

		public Book ( string filename ) {
			base ( "logbook" );
			logbook = new Logbook ();
			logbook.open ( filename );
			add_pages ();
		}

		private void add_pages () {
			flight_editor = new FlightEditor ( this );
			add_child ( "flights", flight_editor );
		}
	}
}