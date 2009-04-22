using Sqlp;
using Gtk;

namespace SqlpGtk {

	public class TakeoffEditor : ParentChildEditor {

		public TakeoffEditor (Logbook logbook) {
			this.parent_table = logbook.flight;
			this.child_table = logbook.takeoffs;
			this.child_view_name = "Takeoffs";
			this.parent_id_fkey_column_name = "flight_id";
			this.gui_name = "parent_child_editor";
		}
		
		construct {
			table_view.set_column_visible (0, false); // id
			table_view.set_column_visible (1, false); // flight_id
			table_view.set_column_visible (2, false); // airport_id
			table_view.set_column_visible (3, false); // surface_id
			table_view.set_column_visible (4, false); // Sequence
		}


		public override void associate_parent (Record child, int64 parent_id) {
			(child as Takeoff).flight = parent_table.find_by_id (parent_id) as Flight;
		}

		private override void on_table_view_edited (TableView view, int64 id, string column_name, string new_text) {
		}

	}
}