// All records reference a crud for performing operations
// on some logbook's database. The Crud belongs to
// the database and is passed to the record when it
// is instantiated by the crud.

using Sqlite;
namespace SqlPilot {
	public abstract class Record {
		
		protected weak Crud crud;

		public int64 id;

		public bool is_new;

		public bool is_modified;

		public abstract Record ( Crud c ) {
			crud = c;
			is_modified = true;
			is_new = true;
			id = 0;
		}

		public bool save () {
			if ( ! is_modified ) return true;
			int ncol = 0;
			weak Statement stmt;
			save_dependencies ();
			if (is_new) {
				stmt = crud.insert;
				ncol = bind_for_save (stmt);
			} else {
				stmt = crud.update;
				bind_for_save (stmt);
				ncol = bind_for_save (stmt);
				stmt.bind_int64 (ncol, id);
			}
			// If my bind_for_save methods are correct the counts should match.
			if (ncol != crud.column_names.length) {
				message ("bind_for_save returned incorrect count of %d for %s which has %d columns",
						 ncol, crud.table_name, crud.column_names.length);
				assert (false);
			}
			stmt.step ();
			stmt.reset ();
			stmt.clear_bindings ();
			if (is_new) {
				id = stmt.db_handle().last_insert_rowid ();
				is_new = false;
			}
			save_dependents ();
			return true;
		}

		public bool delete () {
			if (is_new) return false;
			weak Statement stmt = crud.destroy;
			stmt.bind_int64 (1, id);
			stmt.step ();
			stmt.reset ();
			stmt.clear_bindings ();
			return true;
		}

		// The correct statements will be passed to concrete objects as long as
		// that concrete object has a Crud of the correct type.
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
		
		public virtual bool is_valid () { return false; }
	}
}
