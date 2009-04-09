using Sqlite;
using GLib;

namespace Sqlp {
	public class Logbook : Sqlp.Database {

		public FlightTable flight;
		public GlideTable glides;
		public LaunchTypeTable launch_types;
 		public RoleTable role;
 		public AircraftTable aircraft;
 		public ModelTable model;
 		public AirportTable airport;
		public RoutingTable routing;

		public ModelTagTable model_tags;
		public ModelTaggingTable model_taggings;
		public RoleTagTable role_tags;
		public RoleTaggingTable role_taggings;
		public FlightTagTable flight_tags;
		public FlightTaggingTable flight_taggings;

		public Logbook (string filename) {
			this.filename = filename;
		}

		construct {
			load_db_extensions ();
 			flight = new FlightTable (this);
			glides = new GlideTable (this);
			launch_types = new LaunchTypeTable (this);
			role = new RoleTable (this);
			aircraft = new AircraftTable (this);
			model = new ModelTable (this);
			airport = new AirportTable (this);
			routing = new RoutingTable (this);

			model_tags = new ModelTagTable (this);
			model_taggings = new ModelTaggingTable (this);
			role_tags = new RoleTagTable (this);
			role_taggings = new RoleTaggingTable (this);
			flight_tags = new FlightTagTable (this);
			flight_taggings = new FlightTaggingTable (this);
		   
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