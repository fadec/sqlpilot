using Sqlp;
using Gtk;

namespace SqlpGtk {

	public abstract class ParentChildEditor : Pane {

		public unowned Sqlp.Table parent_table { get; construct; }
		public unowned Sqlp.Table child_table { get; construct; }

		public string child_view_sql { get; construct; }
		public string parent_id_fkey_column_name { get; construct; }

		private int64 _parent_id;
		public int64 parent_id {
			get { return _parent_id; }
			set {
				_parent_id = value;
				table_store.repopulate ();
				on_parent_id_changed (value);
			}
		}

		private Button add_button;
		private Button remove_button;
		private Label summary_label;
		private TableObserverStore table_store;
		protected TableView table_view;


		// example (child as Glide).flight = parent_table.find_by_id (parent_id);
		public abstract void associate_parent (Record child, int64 parent_id);

		private abstract void on_table_view_edited (TableView view, int64 id, string column_name, string new_text);

		private virtual void on_parent_id_changed (int64 id) {}

		public ParentChildEditor (Sqlp.Table parent_table, Sqlp.Table child_table, string child_view_sql, string parent_id_fkey_column_name) {
			// for example
			this.gui_name = "parent_child_editor";
			this.parent_table = parent_table;
			this.child_table = child_table;
			this.child_view_sql = child_view_sql;
			this.parent_id_fkey_column_name = parent_id_fkey_column_name;
		}

		construct {
			add_button = gui.object ("add") as Button;
			remove_button = gui.object ("remove") as Button;
			summary_label = gui.object ("summary") as Label;

			table_store = new TableObserverStore ();
			table_store.select_sql = child_view_sql;
			table_store.scope_sql = parent_id_fkey_column_name + " = ?";
			table_store.bind_scope = (stmt, i) => {
				stmt.bind_int64 (i++, parent_id);
				return i;
			};
			table_store.database = parent_table.database;
			table_store.observe (child_table);
			table_view = make_table_view (table_store);
			table_view.edited += call_on_table_view_edited;
			set_slot ("list_view", table_view);

			summary_label.set_text (summary_label_text ());
		}

		protected virtual TableView make_table_view (TableObserverStore store) {
			return new TableView.with_model (table_store);
		}

		private void call_on_table_view_edited (TableView view, int64 id, string column_name, string new_text) {
			on_table_view_edited (view, id, column_name, new_text);
			summary_label.set_text (summary_label_text ());
		}


		[CCode (instance_pos = -1)]
		public void on_add_clicked (Button button) {
			var child = child_table.new_record ();
			associate_parent (child, parent_id);
			child.save ();
			summary_label.set_text (summary_label_text ());
		}

		[CCode (instance_pos = -1)]
		public void on_remove_clicked (Button button) {
			var ids = table_view.get_selected_ids ();
			foreach (var id in ids) {
				child_table.delete_id (id);
			}
			summary_label.set_text (summary_label_text ());
		}
		
		private virtual string summary_label_text () {
			return "%d records".printf(table_view.count_visible_rows ());
		}
	}
}