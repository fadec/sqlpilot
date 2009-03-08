using GLib;
using Gtk;
using Sqlite;
using Sqlp;

namespace SqlpGtk {
	public class FilterTree : Component {

		public unowned List<string> where_clauses { private set; get; }

		public signal void selection_changed ();

		private ScrolledWindow scrolled_window;
		private TreeStore store;
		private TreeView view;

		public FilterTree () {
		}

		construct {
			this.where_clauses = new List<string> ();
			store = new TreeStore.newv ({ typeof (string), typeof (string) });
			populate (store);
			view = new TreeView.with_model (store);
			add_columns (view);
			set_selection (view);
			view.headers_visible = false;
			view.rubber_banding = true;
			view.show ();
			scrolled_window = new ScrolledWindow (null, null);
			scrolled_window.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
			scrolled_window.add_with_viewport (view);
			scrolled_window.show ();
			top_widget = scrolled_window;
		}

		public void select_none () {
			view.get_selection ().unselect_all ();
		}

		public int count_selected_rows () {
			return view.get_selection ().count_selected_rows ();
		}

		private void add_columns (TreeView view) {

			var renderer = new CellRendererText ();
			var column = new TreeViewColumn.with_attributes ("Filter Name",
															 renderer,
															 "text",
															 0,
															 null);
			column.set_resizable (true);
			view.insert_column (column, 0);
		}

		private void set_selection (TreeView view) {
			var selection = view.get_selection ();
			selection.set_mode (SelectionMode.MULTIPLE);
			selection.changed += on_selection_changed;
		}

		private void on_selection_changed () {
			var selection = view.get_selection ();
			var paths = selection.get_selected_rows (out store);
			TreeIter iter;
			string sql;
			where_clauses = new List<string> ();
			foreach (var path in paths) {
				store.get_iter (out iter, path);
				store.get (iter, 1, out sql);
				where_clauses.append (sql);
			}
			selection_changed ();
		}

		private void populate (TreeStore store) {
			TreeIter y_iter;
			TreeIter m_iter;
			for (int y=1997; y<=2007; y++) {
				store.append(out y_iter, null);
				store.set(y_iter, 0, y.to_string ());
				var year_str = "Date LIKE '" + y.to_string () + "-";
				store.set(y_iter, 1, year_str + "%'");
				for (int m=1; m<=12; m++) {
					store.append(out m_iter, y_iter);
					store.set(m_iter, 0, Sqlp.Date.month_name(m));
					var year_month_str = year_str + "%02d".printf (m) + "-";
					store.set(m_iter, 1, year_month_str + "%'");
				}
			}
		}
	}
}