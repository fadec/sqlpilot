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
			int ncol = 0;
			weak Statement stmt;
			weak Transaction transaction = crud.logbook.transaction;
			transaction.begin ();
			if (! save_dependencies ()) {
				transaction.rollback ();
				return false;
			}
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
				transaction.rollback ();
				return false;
			}
			stmt.step ();
			stmt.reset ();
			stmt.clear_bindings ();
			if (is_new) {
				id = stmt.db_handle().last_insert_rowid ();
				is_new = false;
			}
			if (! save_dependents ()) {
				transaction.rollback ();
				return false;
			}
			transaction.commit ();
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
