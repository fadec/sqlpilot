using GLib;
using Gtk;
using Sqlp;
using Sqlite;

namespace SqlpGtk {

	// This class observes a Table via signals and reacts on
	// crud ops to that table.
	// The colums come from a query that must relate to the
	// observed table only on the id column.
	// The identity of a row in this store must be related to
	// the identity of a Record in Table.
	// IOW it Observes a table object but its internal data is
	// for the store is read directly from the Database object.
	public class TableObserverStore : ListStore {

		public unowned Sqlp.Database _database;
		public unowned Sqlp.Database database {
			get { return _database; }
			set construct {
				_database = value;
				init ();
			}
		}
	
		private string[] _column_names;
		public string[] column_names {
			get { return _column_names; }
		}
		public int column_count {
			get { return _column_names.length; }
		}

		private string _select_sql;
		public string select_sql {
			get { return _select_sql; }
			construct set {
				this._select_sql = value;
				init ();
			}
		}

		private string _scope_sql;
		public string scope_sql {
			get { return _scope_sql != null ? _scope_sql : "1 = 1"; }
			set construct {
				_scope_sql = value;
				init ();
			}
		}

		public delegate int BindScopeType (Statement stmt);
		public BindScopeType bind_scope;

		private string _id_column_name;
		public string id_column_name {
			get { return _id_column_name != null ? _id_column_name : "id"; }
			set construct {
				_id_column_name = value;
				init ();
			}
		}

		public string scoped_select_sql {
			get;
			private set;
		}

		private Statement select_statement;
		private Statement select_by_id_statement;

		// keep init from running in property setter
		private bool ready_for_init;

		public TableObserverStore.with_view (Sqlp.Database database, string view_name) {
			this.database = database;
			this.select_sql = "SELECT * FROM " + view_name;
		}

		construct {
			this.bind_scope = default_bind_scope;
			ready_for_init = true;
			init ();
		}

		private int default_bind_scope (Statement stmt) {
			// bound 0 variables
			return 0;
		}

		protected void init () {
			if (! ready_for_init) return;
			if (database == null ||
				select_sql == null ||
				id_column_name == null ||
				scope_sql == null) return;
			_scoped_select_sql = select_sql + " WHERE (" + scope_sql + ")";
			select_statement = database.prepare_statement (scoped_select_sql);
			select_by_id_statement = database.prepare_statement (scoped_select_sql + " AND (" + id_column_name + " = ?)");
			if (column_names == null) {
				// do once
				init_column_types ();
				init_column_names ();
			}
			repopulate ();
		}

		// Catch all table events.
		// Scope applied elsewhere.
		public void observe (Sqlp.Table table) {
			table.inserted += (table, record) => {
				add_id (record.id);
			};
			table.updated += (table, record) => {
				update_id (record.id);
			};
			table.deleted += (table, id) => {
				remove_id (id);
			};
			table.destroyed += (table, record) => {
				remove_id (record.id);
			};
		}

		private int init_column_names () {
			int ncol;
			int n;
			ncol = select_statement.column_count ();
			_column_names.resize (ncol);
			for (n = 0; n < ncol; n++) {
				_column_names[n] = select_statement.column_name (n);
			}
			return n;
		}

		private void init_column_types () {
			int ncol;
			int n;
			GLib.Type[] types = {};
		
			ncol = select_statement.column_count ();
			types.resize (ncol);
		
			types[0] = typeof (int64);
			for (n = 1; n < ncol; n++) {
				types[n] = typeof(string);
			}
			set_column_types (types);
		}


		private TreeIter add_id (int64 id) {
			select_by_id_statement.bind_int64 (1, id);

			TreeIter iter = TreeIter ();
			while (select_by_id_statement.step () == Sqlite.ROW) {
				append (out iter);
				set_row (iter, select_by_id_statement);
			}
			select_by_id_statement.reset ();
			select_by_id_statement.clear_bindings ();
			return iter;
		}

		private void remove_id (int64 id) {
			message (id.to_string ());
			var iter = get_iter_at_id (id);
			remove (iter);
		}

		private void update_id (int64 id) {
			select_by_id_statement.bind_int64 (1, id);
			while (select_by_id_statement.step () == Sqlite.ROW) {
				var iter = get_iter_at_id (id);
				set_row (iter, select_by_id_statement);
			}
			select_by_id_statement.reset ();
			select_by_id_statement.clear_bindings ();
		}

		private TreeIter get_iter_at_id (int64 id) {
			int64 anid = 0;
			var iter = TreeIter ();
			if (get_iter_first (out iter)) {
				// could become up to n^2 if updating multiple rows.
				do {
					get (iter, 0, &anid);
					if (id == anid) return iter;
				} while (iter_next (ref iter));
			}
			return iter;
		}

		public void repopulate () {
			TreeIter iter;
			clear ();
			bind_scope (select_statement);
			while (select_statement.step () == Sqlite.ROW) {
				append (out iter);
				set_row (iter, select_statement);
			}
			select_statement.reset ();
			select_statement.clear_bindings ();
		}

		private void set_row (TreeIter iter, Statement statement) {
			set (iter, 0, statement.column_int64 (0));
			var ncol = statement.column_count ();
			for (int n=1; n < ncol; n++) {
				set (iter, n, statement.column_text (n));
			}			
		}
	}
}