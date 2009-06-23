using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class ApproachTypeEditor : TableEditor {

		public ApproachTypeEditor (Sqlp.Table table) {
			this.table = table;
			this.gui_name = "table_editor";
		}

		construct {
			table_view.set_column_visible (0, false);
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var approach_type = record as ApproachType;
			switch (column_name) {
			case "Abbreviation":
				approach_type.abbreviation = new_text;
				break;
			case "Description":
				approach_type.description = new_text;
				break;
			}
			approach_type.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var approach_type = record as ApproachType;
			approach_type.abbreviation = "APRCH-" + (table.largest_autoincrement () + 1).to_string ();
		}
	}
}
