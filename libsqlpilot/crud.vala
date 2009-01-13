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

		protected void init_crud_statements (Logbook logbook,
										string find_sql,
										string insert_sql,
										string update_sql,
										string destroy_sql) {
			this.logbook = logbook;
			find   = logbook.prepare_statement (find_sql);
			insert = logbook.prepare_statement (insert_sql);
			update = logbook.prepare_statement (update_sql);
			destroy = logbook.prepare_statement (destroy_sql);
		}

		public abstract Record new_record ();

 		protected Record? record_find_by_id (int64 id) {
			find.bind_int64 (1, id);
 			return record_find_first(find);
 		}

		protected Record? record_find_first (Statement stmt) {
			Record? record;
			if (stmt.step () != DONE) {
				record = new_record ();
				record.set_from_stmt (stmt);
				record.modified = false;
				record.new_record = false;
			} else {
				record = null;
			}
			stmt.reset ();
			stmt.clear_bindings ();
			return record;
		}

		protected List<Record> record_find_all (Statement stmt) {
			return new List <Record>();
		}

	}
}
