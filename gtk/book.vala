
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

			flight_store = new TableObserverStore (logbook, "Flights");
			flight_store.observe (logbook.flight);

			role_store = new TableObserverStore (logbook, "Roles");
			role_store.observe (logbook.role);

			aircraft_store = new TableObserverStore (logbook, "Aircraft");
			aircraft_store.observe (logbook.aircraft);

			model_store = new TableObserverStore (logbook, "Models");
			model_store.observe (logbook.model);

			airport_store = new TableObserverStore (logbook, "Airports");
			airport_store.observe (logbook.airport);

			add_pages ();
		}

		private void add_pages () {
			this.flight_browser = new Browser ();
			flight_browser.table = logbook.flight;
			flight_browser.fieldset = new FlightFields ();
			flight_browser.table_view = new TableView.with_model (flight_store);

			this.role_browser = new Browser ();
			role_browser.table = logbook.role;
			role_browser.fieldset = new RoleFields ();
			role_browser.table_view = new TableView.with_model (role_store);

			this.aircraft_browser = new Browser ();
			aircraft_browser.table = logbook.aircraft;
			aircraft_browser.fieldset = new AircraftFields ();
			aircraft_browser.table_view = new TableView.with_model (aircraft_store);

			this.model_browser = new Browser ();
			model_browser.table = logbook.model;
			model_browser.fieldset = new ModelFields ();
			model_browser.table_view = new TableView.with_model (model_store);

			this.airport_browser = new Browser ();
			airport_browser.table = logbook.airport;
			airport_browser.fieldset = new AirportFields ();
			airport_browser.table_view = new TableView.with_model (airport_store);

		}
	}
}