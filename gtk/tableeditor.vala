using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public abstract class TableEditor : Pane {

		public unowned Sqlp.Table table { get; construct; }
		protected TableObserverStore table_store;
		protected TableView table_view;

		public TableEditor (Sqlp.Table table) {
			this.table = table;
			this.gui_name = "table_editor";
		}

		construct {
			table_store = new TableObserverStore ();
			table_store.select_sql = "SELECT * FROM " + table.table_name;
			table_store.database = table.database;
			table_store.observe (table);
			table_view = new TableView.with_model (table_store);
			table_view.edited += table_view_edited;
			set_slot ("view", table_view);
		}

		protected abstract void on_edit (Record record, string column_name, string new_text);
		protected abstract void prepare_for_insertion (Record record);

		private void table_view_edited (TableView tv, int64 id, string column_name, string new_text) {
			var record = table.find_by_id (id);
			on_edit (record, column_name, new_text);
		}

		[CCode (instance_pos = -1)]
		public void on_add_clicked (Button button) {
			var record = table.new_record ();
			prepare_for_insertion (record);
			record.save ();
		}

		[CCode (instance_pos = -1)]
		public void on_remove_clicked (Button button) {
			var ids = table_view.get_selected_ids ();
			foreach (var id in ids) {
				table.delete_id (id);
			}
		}
	}
}
