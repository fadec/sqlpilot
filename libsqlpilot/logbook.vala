using Sqlite;
using GLib;

namespace SqlPilot {
	public class Logbook {

		private Database _db;
		public weak Database db { public get {return _db;} }

		public Transaction transaction;
		public FlightCrud flight;
 		public RoleCrud role;
 		public AircraftCrud aircraft;
 		public ModelCrud model;
 		public AirportCrud airport;
		public RoutingCrud routing;

		public bool open ( string filename ) {
			int code;
			code = Database.open(filename, out _db);
			if (code != OK) {
				stderr.printf ( "Some db error while opening" );
				return false;
			}
			load_db_extensions ();
			transaction = new Transaction (this);
 			flight = new FlightCrud (this);
			role = new RoleCrud (this);
			aircraft = new AircraftCrud (this);
			model = new ModelCrud (this);
			airport = new AirportCrud (this);
			routing = new RoutingCrud (this);

			return true;
		}


		public Statement? prepare_statement ( string sql ) {
			Statement stmt;
			int err;
			if (db.prepare (sql, (int)sql.length, out stmt) == OK) {
				return stmt;
			} else {
				message ("Error preparing statement.\n%s\n%s", sql, db.errmsg ());
				return null;
			}
		}

		private bool load_db_extensions () {
			string errmsg;
			var	sofile = "libsqlpilot-sqlite3ext.so";
			if ( ! FileUtils.test (sofile, FileTest.EXISTS)) {
				sofile = "../sqlite3ext/.libs/libsqlpilot-sqlite3ext.so";
			}
			var query = "SELECT load_extension(\"" + sofile + "\");";
			db.enable_load_extension (true);
			if (db.exec (query, null, out errmsg) != OK) {
				message ("Failed to load sqlite3 extension\n%s", errmsg);
			};
			return true;
		}
	}

	public class Transaction {
		private static const string ta_begin_sql    = "BEGIN TRANSACTION;";
		private static const string ta_commit_sql   = "COMMIT;";
		private static const string ta_rollback_sql = "ROLLBACK;";

		private weak Logbook logbook;

		private Statement ta_begin;
		private Statement ta_commit;
		private Statement ta_rollback;

		public Transaction ( Logbook logb ) {
			logbook = logb;
			ta_begin = logbook.prepare_statement (ta_begin_sql);
			ta_commit = logbook.prepare_statement (ta_commit_sql);
			ta_rollback = logbook.prepare_statement (ta_rollback_sql);
		}

		public void begin () {
			ta_begin.step ();
			ta_begin.reset ();
		}

		public void commit () {
			ta_commit.step ();
			ta_commit.reset ();
		}

		public void rollback () {
			ta_rollback.step ();
			ta_rollback.reset ();
		}
	}
}