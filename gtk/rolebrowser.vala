using Sqlp;

namespace SqlpGtk {
	public class RoleBrowser : Browser {

		public RoleBrowser (RoleTable table) {
			this.table = table;
			this.gui_name = "browser";
		}

		construct {
			var logbook = table.database as Sqlp.Logbook;
			table_store = new TableObserverStore.with_view (logbook, "Roles");
			table_store.observe (table);

			fieldset = new RoleFields (table);
			table_view = new TableView.with_model (table_store);
			table_view.set_column_visible (0, false); // id
		}
	}
}
