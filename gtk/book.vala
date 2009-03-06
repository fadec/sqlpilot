
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

		public Book ( string filename ) {
			this.gui_name = "book";
			this.logbook_filename = filename;
		}

		construct {
			logbook = new Logbook ();
			logbook.open ( logbook_filename );
			add_pages ();
		}

		private void add_pages () {
			this.flight_browser = new Browser ("Flights", logbook.flight, this);
			flight_browser.fieldset = new FlightFields ();

			this.role_browser = new Browser ("Roles", logbook.role, this);
			role_browser.fieldset = new FlightFields ();
		}
	}
}