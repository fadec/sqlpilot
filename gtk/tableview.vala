using GLib;
using Gtk;
using Sqlite;
using Gee;
using Sqlp;

namespace SqlpGtk {
	public class TableView : Component {

		public signal void selection_changed ();

		private ScrolledWindow scrolled_window;
		private TableObserverStore store;
		private SetTreeModelFilter<int64?> filter;
		private TreeModelSort sort;
		private TreeView view;

 		public TableView.with_store (TableObserverStore store) {
			this.store = store;
 		}

		construct {
			filter = new SetTreeModelFilter<int64?> (null, 0, store);
			sort = new TreeModelSort.with_model (filter);
			view = new TreeView.with_model (sort);
			init_tree_selection ();
			view.rubber_banding = true;
			view.set_rules_hint (true);
			view.set_search_column (0);
			add_view_columns ();
			view.show ();
			scrolled_window = new ScrolledWindow (null, null);
			scrolled_window.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
			scrolled_window.add (view);
			scrolled_window.show ();
			this.top_widget = scrolled_window;
		}

		public int count_selected_rows () {
			return view.get_selection ().count_selected_rows ();
		}

		public int64[] get_selected_ids () {
			var selection = view.get_selection ();
			var paths = selection.get_selected_rows (out sort);
			int64[] ids = {0};
			ids.resize (selection.count_selected_rows ());
			int i=0;
			foreach (var path in paths) {
				TreeIter iter;
				int64 id;
				store.get_iter (out iter, convert_sort_path_to_model_path(path));
				store.get (iter, 0, out id);
				ids[i++] = id;
			}
			return ids;
		}

		public void set_visible_ids (HashSet<int64?> ids) {
			this.filter.visible = ids;
		}


		private void init_tree_selection () {
			var selection = view.get_selection ();
			selection.set_mode (SelectionMode.MULTIPLE);
			selection.changed += on_selection_changed;
		}

 		private void on_selection_changed () {
			var selection = view.get_selection ();
			var paths = selection.get_selected_rows (out sort);
			selection_changed ();
 		}

		private void add_view_columns () {
			int ncol = store.column_count;
			for (int n = 0; n < ncol; n++) {
				var renderer = new CellRendererText ();
				var column = new TreeViewColumn.with_attributes (store.column_names[n],
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

		public void focus_iter (TreeIter iter) {
			assert (store.iter_is_valid (iter));
			set_selection_and_cursor_on_model_iter (iter);
			scroll_to_cursor ();
		}

		public bool iter_is_visible (TreeIter iter) {
			return (convert_model_path_to_sort_path (store.get_path (iter)) != null);
		}

		private void set_selection_and_cursor_on_model_iter (TreeIter model_iter) {
			var model_path = store.get_path (model_iter);
			var sort_path = convert_model_path_to_sort_path (model_path);
			view.get_selection ().unselect_all ();
			view.get_selection ().select_path (sort_path);
			view.set_cursor (sort_path, null, false);
		}

		// stack is TreeView(TreeModelSort(TreeModelFilter(TreeModel)))
		private TreePath? convert_model_path_to_sort_path (TreePath model_path) {
			var filter_path = filter.convert_child_path_to_path (model_path);
			
			if (filter_path != null) {
				return filter.convert_child_path_to_path (filter_path);
			} else {
				return null;
			}
		}

		private TreePath convert_sort_path_to_model_path (TreePath sort_path) {
			var filter_path = sort.convert_path_to_child_path (sort_path);
			var model_path = filter.convert_path_to_child_path (filter_path);
			return model_path;
		}

		private void scroll_to_cursor () {
			TreePath path;
			TreeViewColumn column;
			view.get_cursor (out path, out column);
			view.scroll_to_cell (path, column, false, 0, 0);
		}

	}
}