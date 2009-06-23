using Sqlp;
using Gtk;

namespace SqlpGtk {

	public class ApproachEditor : ParentChildEditor {

		public ApproachEditor (Logbook logbook) {
			this.parent_table = logbook.flight;
			this.child_table = logbook.approaches;
			this.child_view_name = "Approaches";
			this.parent_id_fkey_column_name = "flight_id";
			this.gui_name = "parent_child_editor";
		}

		public override TableView make_table_view (TableObserverStore store) {
			return new ApproachView.with_model (store);
		}

		public override void associate_parent (Record child, int64 parent_id) {
			(child as Approach).flight = parent_table.find_by_id (parent_id) as Flight;
		}

		private override void on_table_view_edited (TableView view, int64 id, string column_name, string new_text) {
		}

		private override string summary_label_text () {
			return "[ Approaches: %d ]       [ Approaches: %d, Day: %d, Night: %d ]".printf (3, 1, 2, 1);
		}
	}
}