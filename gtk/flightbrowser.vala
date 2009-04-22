using Sqlp;

namespace SqlpGtk {
	class FlightBrowser : Browser {
		public FlightBrowser (FlightTable table) {
			this.table = table;
			this.gui_name = "browser";
		}

		construct {
			table_store = new TableObserverStore ();
			table_store.default_column_type = typeof(string);
			table_store.select_sql = "SELECT * FROM Flights";
			table_store.database = table.database;
			table_store.observe (table);
			fieldset = new FlightFields (table);
			table_view = new TableView.with_model (table_store);
			table_view.set_column_visible (0, false);
			table_view.set_column_visible (1, false);
			table_view.set_column_visible (2, false);
			table_view.set_column_visible (3, false);
			table_view.set_column_visible (4, false);
		}
	}
}