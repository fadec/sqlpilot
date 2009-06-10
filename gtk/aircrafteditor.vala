using Sqlp;

namespace SqlpGtk {
	public class AircraftEditor : TableEditorWithFields {

		public AircraftEditor (AircraftTable table) {
			this.table = table;
			this.gui_name = "table_editor_with_fields";
		}

		construct {
			fieldset = new AircraftFields (table);
			table_view.set_column_visible (0, false); // id
			table_view.set_column_visible (1, false); // model_id
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var aircraft = record as Aircraft;
			switch (column_name) {
			case "Registration":
				aircraft.registration = new_text;
				break;
			case "Tail":
				aircraft.tail = new_text;
				break;
			}
			aircraft.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var aircraft = record as Aircraft;
			aircraft.registration = "NEW:AC-" + (table.largest_autoincrement () + 1).to_string ();
		}


	}
}
