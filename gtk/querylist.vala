using GLib;
using Gtk;
using Sqlite;

namespace SqlpGtk {
	public class QueryList : Widget {

		protected enum ColumnKind {
			STRING, STRING_NUMERIC, STRING_FLOAT
		}

		protected class ColumnInfo {
			public ColumnKind kind;
			public bool visible;
		}

		protected ListStore store;
		protected TreeView view;
		protected HashTable <string, ColumnInfo> column_infos;

		public QueryList (Statement stmt) {
			column_infos = new HashTable <string, ColumnInfo> (str_hash, str_equal);
			store = store_from_stmt (stmt);
			view = new TreeView.with_model (store);
			view.set_rules_hint (true);
			view.set_search_column (0);
			add_view_columns (stmt);
		}

		private ListStore store_from_stmt (Statement stmt) {
			int ncol;
			int n;
			GLib.Type[] types = {};

			ncol = stmt.column_count ();
			types.resize (ncol);

			for (n = 0; n < ncol; n++) {
				types[n] = typeof(string);
			}

			var store = new ListStore.newv (types);

			for (n = 0; n < ncol; n++) {
				switch (column_infos.lookup (stmt.column_name (n)).kind) {
				default:
					break;
				case ColumnKind.STRING:
					break;
				case ColumnKind.STRING_NUMERIC:
					break;
				case ColumnKind.STRING_FLOAT:
					store.set_sort_func (n, iter_compare_by_str_float_column);
					break;
				}
			}
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