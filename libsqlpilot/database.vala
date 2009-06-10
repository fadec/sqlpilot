using Sqlite;
using GLib;

namespace Sqlp {
	public abstract class Database : Object {

		private Sqlite.Database _db;
		public unowned Sqlite.Database db { get {return _db;} }

		public string filename { get; construct; }

		public Transaction transaction { get; private set; }

		private Statement largest_autoincrement_stmt;

		public abstract Database (string filename) {
			this.filename = filename;
		}

		~Database () {
			_db.exec ("VACUUM;");
		}

		construct {
			var code = Sqlite.Database.open (filename, out _db);
			if (code != OK) {
				stderr.printf ( "Some db error while opening" );
			}
			transaction = new Transaction.on_database (this);
			largest_autoincrement_stmt = prepare_statement ("SELECT seq FROM sqlite_sequence WHERE name = ?");
		}

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

		public void use_exclusive_locking () {
			_db.exec ("PRAGMA locking_mode = EXCLUSIVE;");
		}

		public int64 largest_autoincrement (string table_name) {
			int64 id = 0;
			largest_autoincrement_stmt.bind_text (1, table_name);
			if (largest_autoincrement_stmt.step () == Sqlite.ROW)
				id = largest_autoincrement_stmt.column_int64 (0);
			largest_autoincrement_stmt.reset ();
			largest_autoincrement_stmt.clear_bindings ();
			return id;
			

		}
	}
}