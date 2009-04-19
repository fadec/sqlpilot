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
		
		public bool save () {
//			if ( ! is_modified ) return true;
			if (! valid ()) return false;
			int ncol = 0;
			weak Statement stmt;
			weak Transaction transaction = ((Sqlp.Database)_table.database).transaction;
			before_save ();
			transaction.begin ();
			message ("<%s>", _table.table_name);
			if (! save_dependencies ()) {
				transaction.rollback ();
				return false;
			}
			if (is_new ()) {
				stmt = _table.insert_stmt;
				ncol = bind_for_save (stmt);
			} else {
				stmt = _table.update_stmt;
				bind_for_save (stmt);
				ncol = bind_for_save (stmt);
				stmt.bind_nonzero_int64 (ncol, id);
			}
			// If my bind_for_save methods are correct the counts should match.
			if (ncol != _table.column_count) {
				warning ("bind_for_save returned incorrect count of %d for %s which has %d columns",
						 ncol, _table.table_name, _table.column_count);
				transaction.rollback ();
				return false;
			}
			message("before step %s", _table.table_name);
			if (stmt.step () != Sqlite.OK) {
				((Sqlp.Database)_table.database).dump_error ();
			}
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
			if (stmt == _table.insert_stmt) _table.tell_inserted (this);
			else _table.tell_updated (this);
			return true;
		}

		public bool destroy () {
			if (is_new () || ! deletable ()) return false;
			weak Statement stmt = _table.delete_stmt;
			stmt.bind_nonzero_int64 (1, id);
			stmt.step ();
			stmt.reset ();
			stmt.clear_bindings ();
			_table.tell_destroyed (this);
			return true;
		}

		protected abstract void set_from_stmt (Statement stmt);
		protected abstract int bind_for_save (Statement stmt);

		protected virtual bool save_dependencies () {
			return true;
		}

		protected virtual bool save_dependents () {
			return true;
		}

		public virtual string summary () { return ""; }
		
		public virtual bool deletable () { return true; }
		
		public virtual void before_save () {}

		public virtual bool valid () { return true; }

		// Use Table#prepare_unique_column_statement to prep query for first argument.
		protected bool is_unique_text (Statement unique_stmt, string? value) {
			unique_stmt.bind_nonempty_text (1, value);
			unique_stmt.bind_nonzero_int64 (2, id);
			var status = unique_stmt.step ();
			unique_stmt.reset ();
			unique_stmt.clear_bindings ();
			return (status != Sqlite.ROW);
		}

	}
}
