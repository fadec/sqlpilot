using GLib;
using Gtk;
using Sqlp;
using Sqlite;

namespace SqlpGtk {
	public class TableObserverStore : ListStore {
		// Database for view.
		public unowned Sqlp.Database database { construct; get; }
		// Name of view from which to load data.
		public string view_name { construct; get; }
	
		private string[] _column_names;
		public string[] column_names {
			get { return _column_names; }
		}
		public int column_count {
			get { return _column_names.length; }
		}

		private Statement select_statement;
		private Statement select_id_statement;

		public TableObserverStore (Sqlp.Database database, string view_name) {
			this.database = database;
			this.view_name = view_name;
		}

		construct {
			select_statement = database.prepare_statement (select_sql ());
			select_id_statement = database.prepare_statement (select_id_sql ());
			init_column_types ();
			init_column_names ();
			populate ();
		}

		public void observe (Sqlp.Table table) {
			table.inserted += (table, record) => {
				add_id (record.id);
			};
			table.deleted += (table, record) => {
				remove_id (record.id);
			};
			table.updated += (table, record) => {
				update_id (record.id);
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
			select_id_statement.bind_int64 (1, id);

			TreeIter iter = TreeIter ();
			while (select_id_statement.step () == Sqlite.ROW) {
				append (out iter);
				set_row (iter, select_id_statement);
			}
			select_id_statement.reset ();
			select_id_statement.clear_bindings ();

			return iter;
		}

		private void remove_id (int64 id) {
			var iter = get_iter_at_id (id);
			remove (iter);
		}

		private void update_id (int64 id) {
			select_id_statement.bind_int64 (1, id);
			while (select_id_statement.step () == Sqlite.ROW) {
				var iter = get_iter_at_id (id);
				set_row (iter, select_id_statement);
			}
			select_id_statement.reset ();
			select_id_statement.clear_bindings ();
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

		private void populate () {
			TreeIter iter;
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

		private string select_sql () {
			return "SELECT * FROM " + view_name + ";";
		}

		private string select_id_sql () {
			return "SELECT * FROM " + view_name + " WHERE id = ?";
		}

	}
}