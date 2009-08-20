// Table is a create, retrieve, update, delete base class
// that knows about the database to which it belongs and
// contains statements.
// Every record object must hold a reference to a table
// that matches the record's class.
// Concrete Tables find and instantiate new records.
// Once a record is instantiated it uses its table reference
// to save, delete, etc. or to lookup associations.

using Sqlite;
namespace Sqlp {
	public abstract class Table : Object {
		private unowned Sqlp.Database _database;
		public unowned Database database {
			construct { _database = value as Sqlp.Database; }
			get {
				return _database;
			}
		}
		public string table_name { construct; get; }
		public Type record_type { construct; get; }
		protected Statement find_stmt;
		protected Statement insert_stmt;
		protected Statement update_stmt;
		protected Statement delete_stmt;
		private string[] _column_names;
		public string[] column_names { get { return _column_names; } }
		private int _column_count;
		public int column_count { get { return _column_count; } }

		public signal void inserted (Record record);
		public signal void updated (Record record);
		public signal void destroyed (Record record);
		public signal void deleted (int64 id);

		private GLib.HashTable <string, int> column_indexes;

		protected Table (Sqlp.Database database, Type record_type, string table_name) {
			this.record_type = record_type;
			this.database = database;
			this.table_name = table_name;
		}

		construct {
			_column_names = table_column_names (table_name);
			_column_count = _column_names.length;
			find_stmt   = _database.prepare_statement (make_find_sql (table_name));
			insert_stmt = _database.prepare_statement (make_insert_sql (table_name));
			update_stmt = _database.prepare_statement (make_update_sql (table_name));
			delete_stmt = _database.prepare_statement (make_delete_sql (table_name));
		}

		public Record new_record () {
			var record = Object.new (this.record_type, "table", this) as Record;
			return record;
		}

		public void tell_destroyed (Record record) {
			destroyed (record);
		}
		
		public void tell_inserted (Record record) {
			inserted (record);
		}

		public void tell_updated (Record record) {
			updated (record);
		}

		private string make_find_sql ( string table_name ) {
			return "SELECT * FROM " + table_name + " WHERE id = ?";
		}

		public string[] table_column_names ( string table_name ) {
			var statement = _database.prepare_statement (make_select_all_sql (table_name));
			return statement_colunm_names (statement);
		}

		private string[] statement_colunm_names ( Statement select ) {
			int ncol = 0;
			ncol = select.column_count ();
			var names = new string[] {};
			names.resize (ncol);
			for (int i = 0; i < ncol; i++) {
				names[i] = select.column_name (i);
			}
			return names;
		}
		
		public virtual Record? find_by_id (int64 id) {
			if (id == 0) return null; // 0 is the id of a new record, not one saved in the db.
			find_stmt.bind_nonzero_int64 (1, id);
 			return find_first(find_stmt);
 		}

		public virtual Record find_by_id_or_new_record (int64 id) {
			var record = find_by_id (id);
			if (record == null) record = new_record ();
			return record;
		}

		public virtual Record? find_first (Statement stmt) {
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

		public List<Record> find_all (Statement stmt) {
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

		public void delete_id (int64 id) {
			unowned Statement stmt = this.delete_stmt;
			stmt.bind_nonzero_int64 (1, id);
			int err_code = stmt.step ();
			if (err_code != Sqlite.DONE) {
				warning ("Database error deleting %s with id = %s\n%d: %s\n",
						 this.table_name,
						 id.to_string (),
						 err_code,
						 stmt.db_handle().errmsg ());
			}
			stmt.reset ();
			stmt.clear_bindings ();
			deleted (id);
		}

		public int64 largest_autoincrement () {
			return database.largest_autoincrement (table_name);
		}

		// First column is 1. -1 if not found.
		public int column_index (string name) {
			if (column_indexes == null) {
				column_indexes = new HashTable <string, int> (str_hash, str_equal);
				for (int i=0; i < column_count; i++) {
					column_indexes.insert (column_names[i], i + 1);
				}
			}
			var idx = column_indexes.lookup (name);
			return idx > 0 ? idx : -1;
		}

		// Begins at 0.
		public int column_offset (string name) {
			int i = column_index (name);
			return i > 0 ? i - 1 : -1;
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
			sql += ")";
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
			sql += " WHERE id = ?";
			return sql;
		}

		private string make_delete_sql ( string table_name ) {
			return "DELETE FROM " + table_name + " WHERE id = ?";
		}

		// Subclasses can use these to make unique checks for valid();
		protected Statement prepare_unique_column_statement (string column, bool use_like = false) {
			return _database.prepare_statement (make_find_duplicate_sql (table_name, column, use_like));
		}

		// Case insensitive dup finder
		private string make_find_duplicate_sql (string table_name, string column, bool use_like) {
			return
			"SELECT 1 as Found FROM " + table_name + " WHERE " + column +
			(use_like ? " LIKE" : " =") + " ? AND id != ?";
		}

// 		private string make_delete_all_sql (string table_name, Collection<int64?> ids) {
// 			var sb = new StringBuilder ();
// 			sb.append ("DELETE FROM ");
// 			sb.append (table_name);
// 			sb.append (" WHERE ");
// 			sb.append (" id IN (");
// 			builder_append_strings (sb, ids, ", ");
// 			sb.append (")");
// 		}

// 		private Collection<string> strings_from_ids (Collection<int64?> ids) {
// 			var ls = new Gee.ArrayList<string> ();
// 			foreach (var id in ids) {
// 				ls.append (id);
// 			}
// 			return ls;
// 		}

		private string make_select_all_sql ( string table_name ) {
			return "SELECT * FROM " + table_name; 
		}
	}
}
