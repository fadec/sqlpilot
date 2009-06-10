using Sqlp;

namespace SqlpGtk {
	public class ModelEditor : TableEditorWithFields {

		public ModelEditor (ModelTable table) {
			this.table = table;
			this.gui_name = "table_editor_with_fields";
		}

		construct {
			fieldset = new ModelFields (table);
			table_view.set_column_visible (0, false); // id
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var model = record as Model;
			switch (column_name) {
			case "Abbreviation":
				model.abbreviation = new_text;
				break;
			case "Make":
				model.make = new_text;
				break;
			case "Name":
				model.name = new_text;
				break;
			}
			model.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var model = record as Model;
			model.abbreviation = "NEW:MODEL-" + (table.largest_autoincrement () + 1).to_string ();
		}


	}
}
