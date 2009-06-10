using Sqlp;

namespace SqlpGtk {
	public class RoleEditor : TableEditorWithFields {

		public RoleEditor (RoleTable table) {
			this.table = table;
			this.gui_name = "table_editor_with_fields";
		}

		construct {
			fieldset = new RoleFields (table);
			table_view.set_column_visible (0, false); // id
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var role = record as Role;
			switch (column_name) {
			case "Abbreviation":
				role.abbreviation = new_text;
				break;
			case "Description":
				role.description = new_text;
				break;
			}
			role.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var role = record as Role;
			role.abbreviation = "ROLE-" + (table.largest_autoincrement () + 1).to_string ();
			role.description = "New Role";
		}


	}
}
