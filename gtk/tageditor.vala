using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class TagEditor : TableEditor {

		public TagEditor (Sqlp.Table table) {
			this.table = table;
			this.gui_name = "table_editor";
		}

		construct {
			view.set_column_visible (0, false); // hide id
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var tag = record as Tag;
			switch (column_name) {
			case "Abbreviation":
				tag.abbreviation = new_text;
				break;
			case "Description":
				tag.description = new_text;
				break;
			}
			tag.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var tag = record as Tag;
			tag.abbreviation = "TAG-" + (table.largest_autoincrement () + 1).to_string ();
			tag.description = "New Tag";
		}
	}
}
