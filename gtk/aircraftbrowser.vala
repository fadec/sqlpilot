using Sqlp;

namespace SqlpGtk {
	public class AircraftBrowser : Browser {

		public AircraftBrowser (AircraftTable table) {
			this.table = table;
			this.gui_name = "browser";
		}

		construct {
			table_store = new TableObserverStore.with_view (table.database, "Aircraft");
			table_store.observe (table);
			fieldset = new AircraftFields (table);
			table_view = new TableView.with_model (table_store);
			table_view.set_column_visible (0, false); // id
			table_view.set_column_visible (1, false); // model_id
		}
	}
}