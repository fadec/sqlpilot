using GLib;
using Gtk;
using Sqlite;
using Gee;

namespace SqlpGtk {
	public class QueryList : Component {

 		private Statement _statement;
		public Statement statement {
			get { return _statement; }
			owned construct {
				_statement = (owned) value;
			}
		}

		public signal void selection_changed ();

		private enum ColumnKind {
			STRING, STRING_NUMERIC, STRING_FLOAT
		}

		private class ColumnInfo {
			public ColumnKind kind;
			public bool visible;
		}

		private ScrolledWindow scrolled_window;
		private ListStore store;
		private SetTreeModelFilter<int64?> filter;
		private TreeModelSort sort;
		private TreeView view;
		private HashTable <string, ColumnInfo> column_infos;

 		public QueryList (owned Statement stmt) {
 			this.statement = (owned) stmt;
 		}

		construct {
			column_infos = new HashTable <string, ColumnInfo> (str_hash, str_equal);
			store = store_from_stmt (this.statement);
			filter = new SetTreeModelFilter<int64?> (null, 0, store);
			sort = new TreeModelSort.with_model (filter);
			view = new TreeView.with_model (sort);
			init_tree_selection (view);
			view.rubber_banding = true;
			view.set_rules_hint (true);
			view.set_search_column (0);
			add_view_columns (this.statement);
			view.show ();
			scrolled_window = new ScrolledWindow (null, null);
			scrolled_window.set_policy (PolicyType.AUTOMATIC, PolicyType.AUTOMATIC);
			scrolled_window.add (view);
			scrolled_window.show ();
			this.top_widget = scrolled_window;
			populate ();
		}

		private void init_tree_selection (TreeView view) {
			var selection = view.get_selection ();
			selection.set_mode (SelectionMode.MULTIPLE);
			selection.changed += on_selection_changed;
		}

 		private void on_selection_changed () {
			var selection = view.get_selection ();
			var paths = selection.get_selected_rows (out sort);
			selection_changed ();
 		}

		public bool select_id (int64 id) {
			return false;
		}

		public int count_selected_rows () {
			return view.get_selection ().count_selected_rows ();
		}

		public int count_all_rows () {
			return 10;
		}

		public int count_visible_rows () {
			return 10;
		}

		public int64[] get_selected_ids () {
			var selection = view.get_selection ();
			var paths = selection.get_selected_rows (out sort);
			int64[] ids = {0};
			int i=0;
			foreach (var path in paths) {
				TreeIter iter;
				int64 id;
				store.get_iter (out iter, path);
				store.get (iter, 0, out id);
				if (i > ids.length) ids.resize (ids.length += 2000);
				ids[i] = id;
			}
			return ids;
		}

		public void set_visible_ids (HashSet<int64?> ids) {
			//  This is fast and screws up iter conversion from filter to sort
 			this.filter = new SetTreeModelFilter <int64?> (ids, 0, this.store);
 			this.sort = new TreeModelSort.with_model (this.filter);
 			this.view.model = this.sort;

			// this is slower and still screws up iters.
//			this.filter.visible = ids;
		}

		private ListStore store_from_stmt (Statement stmt) {
			int ncol;
			int n;
			GLib.Type[] types = {};

			ncol = stmt.column_count ();
			types.resize (ncol);

			types[0] = typeof (int64);
			for (n = 1; n < ncol; n++) {
				types[n] = typeof(string);
			}

			var store = new ListStore.newv (types);

// 			for (n = 0; n < ncol; n++) {
// 				switch (column_infos.lookup (stmt.column_name (n)).kind) {
// 				default:
// 					break;
// 				case ColumnKind.STRING:
// 					break;
// 				case ColumnKind.STRING_NUMERIC:
// 					break;
// 				case ColumnKind.STRING_FLOAT:
// 					store.set_sort_func (n, iter_compare_by_str_float_column);
// 					break;
// 				}
// 			}
			return store;
		}

		private void add_view_columns (Statement stmt) {
			int ncol;
			int n;
			ncol = stmt.column_count ();

			for (n = 0; n < ncol; n++) {
				var renderer = new CellRendererText ();
				var column = new TreeViewColumn.with_attributes (stmt.column_name (n),
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

		// returns iter for base model
		public TreeIter append (Statement statement) {
			TreeIter iter = TreeIter ();
			while (statement.step () == Sqlite.ROW) {
				store.append (out iter);
				set_row (iter, statement);
			}
			statement.reset ();
			statement.clear_bindings ();

//			TreeRowReference rowref = TreeRowReference.new (list, list.get_path (iter));
			return iter;
		}

		public void focus_iter (TreeIter iter) {
			set_selection_and_cursor_on_iter (iter);
			scroll_to_cursor ();
		}

		public bool iter_is_visible (TreeIter iter) {
			var sort_iter = sort_iter_from_model_iter (iter);
			var sort_path = sort.get_path (sort_iter);
			return (sort_path != null);
		}

		private void set_selection_and_cursor_on_iter (TreeIter model_iter) {
			var sort_iter = sort_iter_from_model_iter (model_iter);
			view.get_selection ().unselect_all ();
			view.get_selection ().select_iter (sort_iter);
			var sort_path = sort.get_path (sort_iter);
			view.set_cursor (sort_path, null, false);
		}

		// stack is TreeView(TreeModelSort(TreeModelFilter(TreeModel)))
		private TreeIter sort_iter_from_model_iter (TreeIter model_iter) {
			TreeIter filter_iter;
			TreeIter sort_iter;
			filter.convert_child_iter_to_iter (out filter_iter, model_iter);
			sort.convert_child_iter_to_iter (out sort_iter, filter_iter);
			return sort_iter;
		}

		private void scroll_to_cursor () {
			TreePath path;
			TreeViewColumn column;
			view.get_cursor (out path, out column);
			view.scroll_to_cell (path, column, false, 0, 0);
		}

		private void populate () {
			TreeIter iter;
			while (statement.step () == Sqlite.ROW) {
				store.append (out iter);
				set_row (iter, statement);
			}
			statement.reset ();
			statement.clear_bindings ();
		}

		private void set_row (TreeIter iter, Statement statement) {
			store.set (iter, 0, statement.column_int64 (0));
			var ncol = statement.column_count ();
			for (int n=1; n < ncol; n++) {
				store.set (iter, n, statement.column_text (n));
			}			
		}

		// Parse int compare that filters out non-[0-9] characters before parse
// 		private static int iter_compare_by_noisy_str_int_column(GtkTreeModel *treemod, GtkTreeIter *a, GtkTreeIter *b, gpointer column)
// 		{
// 			char *a_val, *b_val, *pread, *pwrite;
// 			gint ret;

// 			gtk_tree_model_get(treemod, a, (gint)column, &a_val, -1);
// 			gtk_tree_model_get(treemod, b, (gint)column, &b_val, -1);
  
// 			for (pread = pwrite = a_val; a_val && *pread; pread++) {
// 				if (isdigit(*pread) || *pread == '-') {
// 					*pwrite = *pread;
// 					pwrite++;
// 				}
// 			}

// 			for (pread = pwrite = b_val; b_val && *pread; pread++) {
// 				if (isdigit(*pread) || *pread == '-') {
// 					*pwrite = *pread;
// 					pwrite++;
// 				}
// 			}

// 			ret = atol(EMPTY_IF_NULL(a_val)) - atol(EMPTY_IF_NULL(b_val));

// 			g_free(a_val);
// 			g_free(b_val);
// 			return ret;
// 		}

		private static int iter_compare_by_str_float_column(TreeModel treemod, TreeIter a, TreeIter b)
		{
			int column;
			SortType order;
			char *a_val = null;
			char *b_val = null;
			int ret;

			(treemod as TreeSortable).get_sort_column_id (out column, out order);
			treemod.get(a, column, &a_val, -1);
			treemod.get(b, column, &b_val, -1);
  
			ret = 0;//atof(EMPTY_IF_NULL(a_val)) - atof(EMPTY_IF_NULL(b_val));

			free(a_val);
			free(b_val);
			return ret;
		}
	}
}