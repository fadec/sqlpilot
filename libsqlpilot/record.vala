// All records reference a table for performing operations
// on some logbook's database. The Table belongs to
// the database and is passed to the record when it
// is instantiated by the table.
// The type parameter is so subclasses of Table (for
// a specific table) will expose their methods here
// for convenience.

using Sqlite;
namespace Sqlp {
	public abstract class Record <TableType> : Object {
		
		private weak Table _table;
		public TableType table {
			get { return _table; }
			construct {
				_table = value as Table;
			}
		}

		public int64 id;

		public bool is_modified;

		public abstract Record ( Table c ) {
			this.table = c;
			this.is_modified = true;
			this.id = 0;
		}

		public bool is_new () {
			return (id == 0);
		}
		
		// Flight -> Role, Aircraft, Airport, Airport
		// Aircraft -> Model
		// Routing -> Flight, Airport
		// ---------------------------------------------
		// Save loop: flight_1 -> routing_1 -> flight_1 -> ...
		// Double update: flight_1 -> airport_1; flight_1 -> routing_1 -> airport_1 /"route a to b to a to c"
		// Double insert: ""
		// Overwrite: flight_1 -> airport_1; flight_1 -> routing_1 -> clone of airport_1
		// save/delete: flight_1 -> save airport_1; flight_1 -> delete routing -> airport_1
		public bool save () {
//			if ( ! is_modified ) return true;
			if (! valid ()) return false;
			int ncol = 0;
			weak Statement stmt;
			weak Transaction transaction = _table.logbook.transaction;
			transaction.begin ();
			message ("<%s>", _table.table_name);
			if (! save_dependencies ()) {
				transaction.rollback ();
				return false;
			}
			if (is_new ()) {
				stmt = _table.insert;
				ncol = bind_for_save (stmt);
			} else {
				stmt = _table.update;
				bind_for_save (stmt);
				ncol = bind_for_save (stmt);
				stmt.bind_int64 (ncol, id);
			}
			// If my bind_for_save methods are correct the counts should match.
			if (ncol != _table.column_names.length) {
				message ("bind_for_save returned incorrect count of %d for %s which has %d columns",
						 ncol, _table.table_name, _table.column_names.length);
				transaction.rollback ();
				return false;
			}
			message("before step %s", _table.table_name);
			stmt.step ();
			message("after step %s", _table.table_name);
			stmt.reset ();
			stmt.clear_bindings ();
			if (is_new ()) {
				id = stmt.db_handle().last_insert_rowid ();
				}
			if (! save_dependents ()) {
				transaction.rollback ();
				return false;
			}
			transaction.commit ();
			message ("</%s>", _table.table_name);
			return true;
		}

		public bool delete () {
			if (is_new ()) return false;
			weak Statement stmt = _table.destroy;
			stmt.bind_int64 (1, id);
			stmt.step ();
			stmt.reset ();
			stmt.clear_bindings ();
			return true;
		}

		// The correct statements will be passed to concrete objects as long as
		// that concrete object has a Table of the correct type.
		protected abstract void set_from_stmt (Statement stmt);
		protected abstract int bind_for_save (Statement stmt);

		protected virtual bool save_dependencies () {
			return true;
		}

		protected virtual bool save_dependents () {
			return true;
		}

		public virtual string to_string () { return ""; }
		
		public virtual bool deletable () { return false; }
		
		public virtual bool valid () { return true; }

		// Use Table#prepare_unique_column_statement to prep query for first argument.
		protected bool is_unique_text (Statement unique_stmt, string value) {
			unique_stmt.bind_text (1, value);
			unique_stmt.bind_int64 (2, id);
			var status = unique_stmt.step ();
			unique_stmt.reset ();
			unique_stmt.clear_bindings ();
			return (status != Sqlite.ROW);
		}

	}
}
