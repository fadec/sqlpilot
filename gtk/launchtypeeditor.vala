using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class LaunchTypeEditor : TableEditor {

		public LaunchTypeEditor (Sqlp.Table table) {
			this.table = table;
			this.gui_name = "table_editor";
		}

		construct {
			table_view.set_column_visible (0, false);
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var launch_type = record as LaunchType;
			switch (column_name) {
			case "Abbreviation":
				launch_type.abbreviation = new_text;
				break;
			case "Description":
				launch_type.description = new_text;
				break;
			}
			launch_type.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var launch_type = record as LaunchType;
			launch_type.abbreviation = "LAUNCH-" + (table.largest_autoincrement () + 1).to_string ();
		}
	}
}
