using Gtk;
using Gdk;
using Sqlp;

namespace SqlpGtk {

	public class ModelFields : Fieldset <Model> {

		public TagManager tag_manager {	get; construct;	}

		public ModelFields (TagManager tag_manager) {
			this.gui_name = "model_fields";
			this.tag_manager = tag_manager;
		}

		construct {
			Logbook l; // Vala seems to need a declaration to generate correct dependencies in c headers.

			tag_manager.add_tagging_button = gui.object ("add_tagging") as Button;
			tag_manager.remove_tagging_button = gui.object ("remove_tagging") as Button;
			(gui.object ("tags") as ScrolledWindow).add_with_viewport (tag_manager.tags_view);
			(gui.object ("taggings") as ScrolledWindow).add_with_viewport (tag_manager.taggings_view);
			tag_manager.tags_view.show ();
			tag_manager.taggings_view.show ();
		}

		protected override void set_fields_from_record () {
			tag_manager.object_id = record.id;
		}

		protected override void set_record_from_fields () {
		}
	}
}
