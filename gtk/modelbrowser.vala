using Sqlp;

namespace SqlpGtk {
	public class ModelBrowser : Browser {

		public ModelBrowser (ModelTable table) {
			this.table = table;
			this.gui_name = "browser";
		}

		construct {
			var logbook = table.database as Logbook;
			table_store = new TableObserverStore.with_view (table.database, "Models");
			table_store.observe (table);
			var model_tag_manager = new TagManager (table, logbook.model_taggings, logbook.model_tags);
			fieldset = new ModelFields (table, model_tag_manager);
			table_view = new TableView.with_model (table_store);
		}
	}
}