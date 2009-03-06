using GLib;
using Gtk;
using Sqlite;

namespace SqlpGtk {
	public class FilterTree : Component {

		private ScrolledWindow scrolled_window;
		private TreeStore store;
		private TreeView view;


		public FilterTree () {
		}

		construct {
			store = new TreeStore.newv ( {typeof (string), typeof (string) } );
			populate (store);
			view = new TreeView.with_model (store);
			add_columns (view);
			set_selection (view);
			view.show ();
			scrolled_window = new ScrolledWindow (null, null);
			scrolled_window.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
			scrolled_window.add_with_viewport (view);
			scrolled_window.show ();
			top_widget = scrolled_window;
		}

		private void add_columns (TreeView view) {
			int ncol = 2;
			int n;

			for (n = 0; n < ncol; n++) {
				var renderer = new CellRendererText ();
				var column = new TreeViewColumn.with_attributes ("whoami",
															 renderer,
															 "text",
															 n,
															 null);
				column.set_sort_column_id (n);
				column.set_reorderable (true);
				column.set_resizable (true);
				view.insert_column (column, n);
			}
		}

		private void set_selection (TreeView view) {
			view.get_selection ().set_mode (SelectionMode.MULTIPLE);
		}

		private void populate (TreeStore store) {
			TreeIter y_iter;
			TreeIter m_iter;
			for (int y=1997; y<=2007; y++) {
				store.append(out y_iter, null);
				store.set(y_iter, 0, y.to_string (), -1);
				for (int m=1; m<=12; m++) {
					store.append(out m_iter, y_iter);
					store.set(m_iter, 0, m.to_string (), -1);
				}
			}
		}
	}
}