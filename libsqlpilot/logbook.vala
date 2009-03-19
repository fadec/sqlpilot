using Sqlite;
using GLib;

namespace Sqlp {
	public class Logbook : Sqlp.Database {

		public FlightTable flight;
 		public RoleTable role;
 		public AircraftTable aircraft;
 		public ModelTable model;
 		public AirportTable airport;
		public RoutingTable routing;

		public Logbook (string filename) {
			this.filename = filename;
		}

		construct {
			load_db_extensions ();
 			flight = new FlightTable (this);
			role = new RoleTable (this);
			aircraft = new AircraftTable (this);
			model = new ModelTable (this);
			airport = new AirportTable (this);
			routing = new RoutingTable (this);
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