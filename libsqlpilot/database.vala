using Sqlite;
using GLib;

namespace Sqlp {
	public abstract class Database : Object {

		private Sqlite.Database _db;
		public unowned Sqlite.Database db { get {return _db;} }

		public string filename { get; construct; }

		public Transaction transaction { get; private set; }

		public abstract Database (string filename) {
			this.filename = filename;
		}

		construct {
			var code = Sqlite.Database.open (filename, out _db);
			if (code != OK) {
				stderr.printf ( "Some db error while opening" );
			}
			transaction = new Transaction (this);
		}

		public virtual bool after_open () { return true; }

		public Statement? prepare_statement (string sql) {
			Statement stmt;
			//	int err;
			if (_db.prepare (sql, (int)sql.length, out stmt) == OK) {
				return stmt;
			} else {
				message ("Error preparing statement.\n%s\n%s", sql, db.errmsg ());
				return null;
			}
		}

		public bool load_extension (string filename) {
			string errmsg;
			var query = "SELECT load_extension(\"" + filename + "\");";
			db.enable_load_extension (true);
			if (db.exec (query, null, out errmsg) != OK) {
				message ("Failed to load sqlite3 extension\n%s", errmsg);
				return false;
			}
			return true;
		}
	}
}