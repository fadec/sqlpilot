using Sqlite;
using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {

	class IdTreeModelFilter : Gtk.TreeModelFilter {

		public string table_name { get; construct; }
		public string where_clause { get; construct; }
		public Sqlp.Logbook logbook { get; construct; }

		private HashTable <int64?, bool> idset;

		public IdTreeModelFilter (Logbook logbook, string table_name, string where_clause, TreeModel child_model) {
			this.logbook = logbook;
			this.table_name = table_name;
			this.where_clause = where_clause;
		}

		construct {
			idset = new HashTable <int64?, bool> (int_hash, int_equal);
			var statement = logbook.prepare_statement (query_sql ());
			while (statement.step () == Sqlite.ROW) {
				idset.insert (statement.column_int64 (0), true);
			}
			set_visible_func (is_in_idset);
		}

		private string query_sql () {
			string s = "SELECT id FROM " + table_name + " WHERE " + where_clause + ";";
			return s;
		}

		[CCode (instance_pos = -1)]
		public bool is_in_idset (TreeModel model, TreeIter iter) {
			int value;
			model.get(iter, 0, out value);
			return (idset.lookup (value));
		}


	}
	
}