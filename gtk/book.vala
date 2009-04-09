
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
		private Browser _role_browser;
		private Browser role_browser {
			get { return _role_browser; }
			set {
				_role_browser = value;
				if (_role_browser == null) return;
				set_slot ("roles", _role_browser);
			}
		}

		private Browser _aircraft_browser;
		private Browser aircraft_browser {
			get { return _aircraft_browser; }
			set {
				_aircraft_browser = value;
				if (_aircraft_browser == null) return;
				set_slot ("aircraft", _aircraft_browser);
			}
		}

		private Browser _model_browser;
		private Browser model_browser {
			get { return _model_browser; }
			set {
				_model_browser = value;
				if (_model_browser == null) return;
				set_slot ("models", _model_browser);
			}
		}

		private Browser _airport_browser;
		private Browser airport_browser {
			get { return _airport_browser; }
			set {
				_airport_browser = value;
				if (_airport_browser == null) return;
				set_slot ("airports", _airport_browser);
			}
		}

		private TableObserverStore flight_store;
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
			flight_store = new TableObserverStore ();
			flight_store.default_column_type = typeof(string);
			flight_store.select_sql = "SELECT * FROM Flights";
			flight_store.database = logbook;
			flight_store.observe (logbook.flight);

			role_store = new TableObserverStore.with_view (logbook, "Roles");
			role_store.observe (logbook.role);

			aircraft_store = new TableObserverStore.with_view (logbook, "Aircraft");
			aircraft_store.observe (logbook.aircraft);

			model_store = new TableObserverStore.with_view (logbook, "Models");
			model_store.observe (logbook.model);

			airport_store = new TableObserverStore.with_view (logbook, "Airports");
			airport_store.observe (logbook.airport);

			add_pages ();
		}

		private void add_pages () {
			this.flight_browser = new Browser ();
			flight_browser.table = logbook.flight;
			flight_browser.fieldset = new FlightFields (logbook.flight);
			flight_browser.table_view = new TableView.with_model (flight_store);

			this.role_browser = new Browser ();
			role_browser.table = logbook.role;
			var role_tag_manager = new TagManager (logbook.role, logbook.role_taggings, logbook.role_tags);
			role_browser.fieldset = new RoleFields (logbook.role, role_tag_manager);
			role_browser.table_view = new TableView.with_model (role_store);

			this.aircraft_browser = new Browser ();
			aircraft_browser.table = logbook.aircraft;
			aircraft_browser.fieldset = new AircraftFields (logbook.aircraft);
			aircraft_browser.table_view = new TableView.with_model (aircraft_store);

			this.model_browser = new Browser ();
			model_browser.table = logbook.model;
			var model_tag_manager = new TagManager (logbook.model, logbook.model_taggings, logbook.model_tags);
			model_browser.fieldset = new ModelFields (logbook.model, model_tag_manager);
			model_browser.table_view = new TableView.with_model (model_store);

			this.airport_browser = new Browser ();
			airport_browser.table = logbook.airport;
			airport_browser.fieldset = new AirportFields (logbook.airport);
			airport_browser.table_view = new TableView.with_model (airport_store);
		}
	}
}