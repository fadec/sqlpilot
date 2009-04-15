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

			var role_tag_manager = new TagManager (table, logbook.role_taggings, logbook.role_tags);
			fieldset = new RoleFields (table, role_tag_manager);
			table_view = new TableView.with_model (table_store);			
		}
	}
}