using Sqlite;
using GLib;

namespace Sqlp {
	public class Logbook : Sqlp.Database {

		public FlightCrud flight;
 		public RoleCrud role;
 		public AircraftCrud aircraft;
 		public ModelCrud model;
 		public AirportCrud airport;
		public RoutingCrud routing;

		public Logbook (string filename) {
			this.filename = filename;
		}

		construct {
			load_db_extensions ();
 			flight = new FlightCrud (this);
			role = new RoleCrud (this);
			aircraft = new AircraftCrud (this);
			model = new ModelCrud (this);
			airport = new AirportCrud (this);
			routing = new RoutingCrud (this);
		}

		private void load_db_extensions () {
			var	sofile = "./libsqlpilot-sqlite3ext.so";
			if ( ! FileUtils.test (sofile, FileTest.EXISTS)) {
				sofile = "libsqlpilot-sqlite3ext.so";
			}
			load_extension (sofile);
		}
	}
}