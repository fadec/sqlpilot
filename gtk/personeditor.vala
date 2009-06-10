using GLib;
using Gtk;
using Sqlp;

namespace SqlpGtk {
	public class PersonEditor : TableEditor {

		public PersonEditor (Sqlp.Table table) {
			this.table = table;
			this.gui_name = "table_editor";
		}

		construct {
			table_view.set_column_visible (0, false);
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var person = record as Person;
			switch (column_name) {
			case "LastName":
				person.last_name = new_text;
				break;
			case "FirstName":
				person.first_name = new_text;
				break;
			case "Identification":
				person.identification = new_text;
				break;
			case "Notes":
				person.notes = new_text;
				break;
			}
			person.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var person = record as Person;
			person.last_name = "PERSON-" + (table.largest_autoincrement () + 1).to_string ();
		}
	}
}