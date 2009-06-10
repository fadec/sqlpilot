using GLib;
using Gtk;

namespace SqlpGtk {
	public abstract class TableEditorWithFields : TableEditor {

		private Fieldset _fieldset;
		public Fieldset fieldset {
			get { return _fieldset; }
			set construct {
				_fieldset = value;
				if (_fieldset == null) return;
				setup_table_view ();
				set_fieldset_sensitivity ();
				set_slot ("fields", _fieldset);
			}
		}

		public TableEditorWithFields (Sqlp.Table table) {
			this.table = table;
			this.gui_name = "table_editor_with_fields";
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

		private void setup_table_view () {
			table_view.selection_changed += (table_view) => {
				if (fieldset.edited) fieldset.save ();

				if (table_view.count_selected_rows() == 1) {
					fieldset.record = table.find_by_id (table_view.get_selected_ids ()[0]);
					fieldset.top_widget.sensitive = true;
					//this.allow_delete = fieldset.record.deletable ();
				} else {
					fieldset.record = table.new_record ();
					fieldset.top_widget.sensitive = false;
					//this.allow_delete = false;
				}
  			};
		}

		private void set_fieldset_sensitivity () {
			if (_fieldset == null) return;
			if (table_view == null) {
				_fieldset.top_widget.sensitive = false;
				return;
			}
			_fieldset.top_widget.sensitive = (table_view.count_selected_rows () == 1);
		}

	}
}