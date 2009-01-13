// All records reference a crud for performing operations
// on some logbook's database. The Crud belongs to
// the database and is passed to the record when it
// is instantiated by the crud.

using Sqlite;
namespace SqlPilot {
	public abstract class Record {
		
		protected weak Crud crud;

		public int64 id;

		public bool new_record;
		public bool modified;

		public abstract Record ( Crud c ) {
			crud = c;
			modified = true;
			new_record = true;
		}

		public bool save () {
			if ( ! modified ) return true;
			weak Statement stmt;
			save_dependencies ();
			if (new_record) {
				stmt = crud.insert;
				bind_for_save (stmt);
			} else {
				stmt = crud.update;
				bind_for_save (stmt);
				var where_id_column = bind_for_save (stmt);
				stmt.bind_int64 (where_id_column, id);
			}
			stmt.step ();
			stmt.reset ();
			stmt.clear_bindings ();
			if (new_record) {
				id = stmt.db_handle().last_insert_rowid ();
				new_record = false;
			}
			save_dependents ();
			return true;
		}

		public bool delete () {
			if (new_record) return false;
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
