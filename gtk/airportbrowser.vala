using Sqlp;

namespace SqlpGtk {
	public class AirportBrowser : Browser {

		public AirportBrowser (AirportTable table) {
			this.table = table;
			this.gui_name = "browser";
		}

		construct {
			table_store = new TableObserverStore.with_view (table.database, "Airports");
			table_store.observe (table);
			fieldset = new AirportFields (table);
			table_view = new TableView.with_model (table_store);
		}
	}
}