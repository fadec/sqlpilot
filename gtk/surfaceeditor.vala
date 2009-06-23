using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class SurfaceEditor : TableEditor {

		public SurfaceEditor (Sqlp.Table table) {
			this.table = table;
			this.gui_name = "table_editor";
		}

		construct {
			table_view.set_column_visible (0, false);
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var surface = record as Surface;
			switch (column_name) {
			case "Abbreviation":
				surface.abbreviation = new_text;
				break;
			case "Description":
				surface.description = new_text;
				break;
			}
			surface.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var surface = record as Surface;
			surface.abbreviation = "SURFACE-" + (table.largest_autoincrement () + 1).to_string ();
		}
	}
}
