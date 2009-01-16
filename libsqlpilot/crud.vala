// Crud is a create, retrieve, update, delete base class
// that knows about the logbook to which it belongs and
// contains statements for the logbook's database.
// Every record object must hold a reference to a crud
// that matches the record's class.
// Concrete Cruds find and instantiate new records.
// Once a record is instantiated it uses its crud reference
// to save, delete, etc. or to lookup associations.

using Sqlite;
namespace SqlPilot {
	public abstract class Crud {
		public weak Logbook logbook;
		public Statement find;
		public Statement insert;
		public Statement update;
		public Statement destroy;
		public string table_name;
		public string[] column_names;

		protected Crud (Logbook logbook, string table_name) {
				
			this.logbook = logbook;
			this.table_name = table_name;
			column_names = table_column_names (table_name);
			find   = logbook.prepare_statement (make_find_sql (table_name));
			insert = logbook.prepare_statement (make_insert_sql (table_name));
			update = logbook.prepare_statement (make_update_sql (table_name));
			destroy = logbook.prepare_statement (make_destroy_sql (table_name));
		}

		// new_record is abstract so it can call the correct contsructor on the record,
		// thus ensuring that the correct crud gets passed to the constructor.
		// Abstract records can't be instantiated anyway so new_record must know its
		// friend Record type.
		// Unfortunately it can only return a Record so each concrete Crud should
		// implement a beget() function that casts new_record() to its friend type.
		// e.g. FlightCrud#beget() returns a Flight
		public abstract Record new_record ();
 		

		private string make_find_sql ( string table_name ) {
			return "SELECT * FROM " + table_name + " WHERE id = ?;";
		}

		private string make_insert_sql ( string table_name ) {
			var names = table_column_names (table_name);
			string sql = "INSERT INTO " + table_name + " (";
			// start at index 1 to skip "id" column at index 0
			for (int i = 1; i < names.length; i++) {
				if (i > 1) sql += ",";
				sql += names[i];
			}
			sql += ") VALUES (";
			for (int i = 1; i < names.length; i++) {
				if (i > 1) sql += ",";
				sql += "?";
			}
			sql += ");";
			return sql;
		}

		private string make_update_sql ( string table_name ) {
			var names = table_column_names (table_name);
			string sql = "UPDATE " + table_name + " SET ";
			for (int i = 1; i < names.length; i++) {
				if (i > 1) sql += ",";
				sql += names[i];
				sql += " = ?";
			}
			sql += " WHERE id = ?;";
			return sql;
		}

		private string make_destroy_sql ( string table_name ) {
			return "DELETE FROM " + table_name + " WHERE id = ?;";
		}

		public string make_select_all_sql ( string table_name ) {
			return "SELECT * FROM " + table_name + ";"; 
		}

		public string[] table_column_names ( string table_name ) {
			var statement = logbook.prepare_statement (make_select_all_sql (table_name));
			return statement_colunm_names (statement);
		}

		public string[] statement_colunm_names ( Statement select ) {
			int ncol = 0;
			ncol = select.column_count ();
			var names = new string[] {};
			names.resize (ncol);
			for (int i = 0; i < ncol; i++) {
				names[i] = select.column_name (i);
			}
			return names;
		}

		public Record? record_find_by_id (int64 id) {
			find.bind_int64 (1, id);
 			return record_find_first(find);
 		}

		protected Record? record_find_first (Statement stmt) {
			Record? record;
			if (stmt.step () == ROW) {
				record = new_record ();
				record.id = stmt.column_int64 (0);
				record.set_from_stmt (stmt);
				record.is_modified = false;
			} else {
				record = null;
			}
			stmt.reset ();
			stmt.clear_bindings ();
			return record;
		}

		protected List<Record> record_find_all (Statement stmt) {
			Record? record;
			var records = new List<Record> ();
			while (stmt.step () == ROW) {
				record = new_record ();
				record.id = stmt.column_int64 (0);
				record.set_from_stmt (stmt);
				record.is_modified = false;
				records.append (record);
			}
			stmt.reset ();
			stmt.clear_bindings ();
			return records;
		}

	}
}
