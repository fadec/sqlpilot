
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

		public Book ( string filename ) {
			this.gui_name = "book";
			this.logbook_filename = filename;
		}

		construct {
			logbook = new Logbook (logbook_filename);
			add_pages ();
		}

		private void add_pages () {
 			this.flight_browser = new Browser ("Flights", logbook.flight, this);
 			flight_browser.fieldset = new FlightFields ();

 			this.role_browser = new Browser ("Roles", logbook.role, this);
 			role_browser.fieldset = new RoleFields ();

 			this.aircraft_browser = new Browser ("Aircraft", logbook.aircraft, this);
 			aircraft_browser.fieldset = new AircraftFields ();

 			this.model_browser = new Browser ("Models", logbook.model, this);
 			model_browser.fieldset = new ModelFields ();

 			this.airport_browser = new Browser ("Airports", logbook.airport, this);
 			airport_browser.fieldset = new AirportFields ();
		}
	}
}