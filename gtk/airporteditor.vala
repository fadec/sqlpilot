using Sqlp;

namespace SqlpGtk {
	public class AirportEditor : TableEditorWithFields {

		public AirportEditor (AirportTable table) {
			this.table = table;
			this.gui_name = "table_editor_with_fields";
		}

		construct {
			fieldset = new AirportFields (table);
			table_view.set_column_visible (0, false); // id
		}

		private override void on_edit (Record record, string column_name, string new_text) {
			var airport = record as Airport;
			switch (column_name) {
			case "Abbreviation":
				airport.abbreviation = new_text;
				break;
			case "ICAO":
				airport.icao = new_text;
				break;
			case "IATA":
				airport.iata = new_text;
				break;
			}
			airport.save ();
		}

		private override void prepare_for_insertion (Record record) {
			var airport = record as Airport;
			airport.abbreviation = "NEW:AIRPORT-" + (table.largest_autoincrement () + 1).to_string ();
		}
	}
}
