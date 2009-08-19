using Gtk;
using Sqlite;
using Sqlp;
namespace SqlpGtk {
	// A Book is the gui wrapper for the Logbook data object
	public class Book : Pane {
		public Logbook logbook { get; private set; }
		public string logbook_filename { get; construct; }

		private Browser _flight_browser;
		private Browser flight_browser {
			get { return _flight_browser; }
			set {
				_flight_browser = value;
				if (_flight_browser == null) return;
				set_slot ("flights", _flight_browser);
			}
		}

		private TableObserverStore role_store;
		private TableObserverStore aircraft_store;
		private TableObserverStore model_store;
		private TableObserverStore airport_store;

		public Book ( string filename ) {
			this.gui_name = "book";
			this.logbook_filename = filename;
		}

		construct {
			logbook = new Logbook (logbook_filename);
			// So this gui is the only thing that can chage db and thereby stay in sync with it.
			logbook.use_exclusive_locking ();
			logbook.db.exec ("SAVEPOINT outer");
			add_pages ();
		}

		private void add_pages () {
			this.flight_browser = new FlightBrowser (logbook.flight);

			var flight_tags = new TagEditor (logbook.flight_tags);
			set_slot ("flight_tags", flight_tags);

			var roles = new RoleEditor (logbook.role);
			set_slot ("roles", roles);

			var role_properties = new TagEditor (logbook.role_tags);
			set_slot ("role_properties", role_properties);

			var aircraft = new AircraftEditor (logbook.aircraft);
			set_slot ("aircraft", aircraft);

			var models = new ModelEditor (logbook.model);
			set_slot ("models", models);

			var model_properties = new TagEditor (logbook.model_tags);
			set_slot ("model_properties", model_properties);
			
			var airports = new AirportEditor (logbook.airport);
			set_slot ("airports", airports);

			var approach_types = new ApproachTypeEditor (logbook.approach_types);
			set_slot ("approach_types", approach_types);

			var surfaces = new SurfaceEditor (logbook.surfaces);
			set_slot ("surfaces", surfaces);

			var launch_types = new LaunchTypeEditor (logbook.launch_types);
			set_slot ("launch_types", launch_types);

			var people = new PersonEditor (logbook.people);
			set_slot ("people", people);
		}
	}
}
