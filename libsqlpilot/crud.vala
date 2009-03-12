// Crud is a create, retrieve, update, delete base class
// that knows about the logbook to which it belongs and
// contains statements for the logbook's database.
// Every record object must hold a reference to a crud
// that matches the record's class.
// Concrete Cruds find and instantiate new records.
// Once a record is instantiated it uses its crud reference
// to save, delete, etc. or to lookup associations.

using Sqlite;
namespace Sqlp {
	public abstract class Crud <RT> : Object {
		public weak Logbook logbook { construct; get; }
		public string table_name { construct; get; }
		public Type record_type { construct; get; }
		public Statement find;
		public Statement insert;
		public Statement update;
		public Statement destroy;
		public string[] column_names;

		private GLib.HashTable <string, int> column_indexes;

		protected Crud (Type record_type, Logbook logbook, string table_name) {
			this.record_type = record_type;
			this.logbook = logbook;
			this.table_name = table_name;
		}

		construct {
			column_names = table_column_names (table_name);
			find   = logbook.prepare_statement (make_find_sql (table_name));
			insert = logbook.prepare_statement (make_insert_sql (table_name));
			update = logbook.prepare_statement (make_update_sql (table_name));
			destroy = logbook.prepare_statement (make_destroy_sql (table_name));
		}

		public virtual RT new_record () {
			return Object.new (this.record_type, "crud", this);
		}

		private string make_find_sql ( string table_name ) {
			return "SELECT * FROM " + table_name + " WHERE id = ?;";
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
		
		public virtual RT? find_by_id (int64 id) {
			find.bind_int64 (1, id);
 			return find_first(find);
 		}

		public virtual RT? find_first (Statement stmt) {
			Record? record;
			if (stmt.step () == ROW) {
				record = new_record () as Record;
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

		public List<RT> find_all (Statement stmt) {
			Record? record;
			var records = new List<Record> ();
			while (stmt.step () == ROW) {
				record = new_record () as Record;
				record.id = stmt.column_int64 (0);
				record.set_from_stmt (stmt);
				record.is_modified = false;
				records.append (record);
			}
			stmt.reset ();
			stmt.clear_bindings ();
			return records;
		}

// 		public void destroy_ids (List<int64> ids) {
			
// 		}

		public void destroy_id (int64 id) {
			unowned Statement stmt = this.destroy;
			stmt.bind_int64 (1, id);
			stmt.step ();
			stmt.reset ();
			stmt.clear_bindings ();
		}

		// First column is 1. -1 if not found.
		protected int column_index (string name) {
			if (column_indexes == null) {
				column_indexes = new HashTable <string, int> (str_hash, str_equal);
				for (int i=0; i < column_names.length; i++) {
					column_indexes.insert (column_names[i], i + 1);
				}
			}
			var idx = column_indexes.lookup (name);
			return idx > 0 ? idx : -1;
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

// 		private string make_destroy_all_sql (string table_name, Collection<int64?> ids) {
// 			var sb = new StringBuilder ();
// 			sb.append ("DELETE FROM ");
// 			sb.append (table_name);
// 			sb.append (" WHERE ");
// 			sb.append (" id IN (");
// 			builder_append_strings (sb, ids, ", ");
// 			sb.append (");");
// 		}

// 		private Collection<string> strings_from_ids (Collection<int64?> ids) {
// 			var ls = new Gee.ArrayList<string> ();
// 			foreach (var id in ids) {
// 				ls.append (id);
// 			}
// 			return ls;
// 		}

		private string make_select_all_sql ( string table_name ) {
			return "SELECT * FROM " + table_name + ";"; 
		}
	}
}
